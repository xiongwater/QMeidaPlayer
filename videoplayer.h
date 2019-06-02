/**
 * 叶海辉
 * QQ群121376426
 * http://blog.yundiantech.com/
 */

#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QtCore/QThread>
#include <QtCore/QMutex>
#include <QtGui/QImage>

extern "C"
{
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavdevice/avdevice.h"  
#include "libavutil/imgutils.h"
#include "libavutil/time.h"
#include "libavutil/pixfmt.h"
#include "libswresample/swresample.h"   
	
}
extern "C"
{
    
    #include <SDL.h>
    #include <SDL_audio.h>
    #include <SDL_types.h>
    #include <SDL_name.h>
    #include <SDL_main.h>
    #include <SDL_config.h>
}

typedef struct PacketQueue {
    AVPacketList *first_pkt, *last_pkt;
    int nb_packets;
    int size;
    SDL_mutex *mutex;
    SDL_cond *cond;
} PacketQueue;

#define VIDEO_PICTURE_QUEUE_SIZE 1
#define AVCODEC_MAX_AUDIO_FRAME_SIZE 192000 // 1 second of 48khz 32bit audio

#define MAX_AUDIO_SIZE (25 * 16 * 1024)
#define MAX_VIDEO_SIZE (25 * 256 * 1024)



//读写的bool值，用于线程退出
class ThreadStatus
{
public:
	enum ThreadStatusValue
	{
		WaiteRun,
		Running,
		QuitCmd,
		End
	};

private:
	ThreadStatusValue status;
	QMutex op_mtx;
public:
	ThreadStatus()
	{
		status = WaiteRun;
	}
	~ThreadStatus()
	{

	}
	bool IsQuitCmd()
	{
		QMutexLocker lock(&op_mtx);
		return QuitCmd == status;
	}
	bool IsWaitting()
	{
		QMutexLocker lock(&op_mtx);
		return WaiteRun == status;
	}
	bool isRunning()
	{
		QMutexLocker lock(&op_mtx);
		return Running == status;
	}
	bool isEnd()
	{
		QMutexLocker lock(&op_mtx);
		return End == status;
	}
	ThreadStatusValue Statue()
	{
		QMutexLocker lock(&op_mtx);
		return status;
	}

	void StatueStart()
	{
		QMutexLocker lock(&op_mtx);
		status = Running;
	}
	void StatueQuitCmd()
	{
		QMutexLocker lock(&op_mtx);
		status = QuitCmd;
	}
	void StatueEnd()
	{
		QMutexLocker lock(&op_mtx);
		status = End;
	}
};




class VideoPlayer; 

class VideoState {

public:
	VideoState()
	{
		m_pFormatContext = NULL;
		audio_frame = NULL;
		audio_stream = NULL;
		audio_pkt_data = NULL;
		audio_buf = audio_buf2;
		swr_ctx = NULL;
		video_st = NULL;
		video_thread = NULL;
		player = NULL;
		//audio_buf2 = NULL;
	}
	~VideoState()
	{

	}
public:
    AVFormatContext *m_pFormatContext;


	//about audio
    AVFrame *audio_frame;// 解码音频过程中的使用缓存
    PacketQueue audioQue;
    AVStream *audio_stream; //音频流
    unsigned int audio_buf_size;
    unsigned int audio_buf_index;
    AVPacket audio_pkt;
    uint8_t *audio_pkt_data;
    int audio_pkt_size;
    uint8_t *audio_buf;
    DECLARE_ALIGNED(16,uint8_t,audio_buf2) [AVCODEC_MAX_AUDIO_FRAME_SIZE * 4];//静态数组缓存 用于缓存音频数据
    enum AVSampleFormat audio_src_fmt;
    enum AVSampleFormat audio_tgt_fmt;
    int audio_src_channels;
    int audio_tgt_channels;
    int64_t audio_src_channel_layout;
    int64_t audio_tgt_channel_layout;
    int audio_src_freq;
    int audio_tgt_freq;
    struct SwrContext *swr_ctx; //用于解码后的音频格式转换
    int audio_hw_buf_size;//Audio buffer size in bytes (calculated)

    double audio_clock; ///音频时钟


	// about video 
    double video_clock; ///<pts of last decoded frame / predicted pts of next decoded frame
    AVStream *video_st;
	PacketQueue videoq;
    SDL_Thread *video_thread;  //视频线程id
    SDL_AudioDeviceID audioID;
	ThreadStatus video_thread_status;

    VideoPlayer *player; //记录下这个类的指针  主要用于在线程里面调用激发信号的函数

};

class VideoPlayer : public QThread
{
    Q_OBJECT

public:
    explicit VideoPlayer(QObject *parent);
    ~VideoPlayer();

    void setFileName(QString path){mFileName = path;}

    void startPlay();
	void PauseOrResume();

    void disPlayVideo(QImage img);

	void QuitChildThread();

signals:
    void sig_GetOneFrame(QImage); //没获取到一帧图像 就发送此信号

protected:
    void run();

private:
    QString mFileName;//路径（也可以是网络流）



    VideoState mVideoState;//存储视频状态

	ThreadStatus m_read_status;
	bool isPause;


};

#endif // VIDEOPLAYER_H
