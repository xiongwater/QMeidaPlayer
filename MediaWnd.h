#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets/QLabel>
#include <QtCore/QTimer>
#include<QtWidgets/QMenubar>
//#include "ui_FFMPEG_LEARN.h"
#include <QtCore/QQueue>
#include <QtCore/QMutex>
//#include "DecodecThread.h"
#include "videodownloaddialog.h"
//#include "AudioPlayer.h"
//#include"QAudioPlayer.h"


//#include "AVReadThread.h"
//#include "VideoDecodecThread.h"
//#include <AudioDecPlay.h>

#include <videoplayer.h>
///由于我们建立的是C++的工程
///编译的时候使用的C++的编译器编译
///而FFMPEG是C的库
///因此这里需要加上extern "C"
///否则会提示各种未定义

#include<iostream>
using namespace std;


class MediaWnd : public QMainWindow
{
	Q_OBJECT

public:
	MediaWnd(QWidget *parent = Q_NULLPTR);
	~MediaWnd();

private:
	QTimer m_timer;
	QWidget* m_CentralWidget;
	QLabel* m_CentralCanvas;
	QQueue<QImage> m_ImagesQueue;
	QMutex m_mutext;
	videodownloaddialog* m_SourceChooseDialog;
	


	VideoPlayer* m_videoplayer;
	QImage mImage;


private:
	void SetCentralImage(QImage image);
	void InitUiAndControl();

	public slots:

	void slot_GotOneDecodecPicture(QImage srcImage);

	void PauseOrResume();
	void slot_UpdatMediaSource();

protected:
	void paintEvent(QPaintEvent *event);
};
