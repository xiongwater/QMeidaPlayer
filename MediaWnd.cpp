#include "MediaWnd.h"
#include <QtWidgets/QAction>
#include <QtWidgets/QHBoxLayout>
#include <QtGui/QPainter>
//#include<>
MediaWnd::MediaWnd(QWidget *parent)
	: QMainWindow(parent)
{
	//ui.setupUi(this);
	m_SourceChooseDialog = new videodownloaddialog(this);//选择资源对话框，其他项目直接移植使用
	m_CentralCanvas = new QLabel(this);
	m_CentralWidget = new QWidget(this);
	m_CentralWidget->setStyleSheet("background:black");
	m_CentralWidget->hide();
	QHBoxLayout* lay = new QHBoxLayout(this);
	lay->addWidget(m_CentralCanvas);
	m_CentralWidget->setLayout(lay);
	m_videoplayer = new VideoPlayer(this);
	
	InitUiAndControl();
}

MediaWnd::~MediaWnd()
{
	//m_decodecThread->UpdateFormatContext("");
}

void MediaWnd::SetCentralImage(QImage image)
{
	QImage adjustSizeImage = image.scaled(m_CentralCanvas->size(),Qt::IgnoreAspectRatio);
	m_CentralCanvas->setPixmap(QPixmap::fromImage(adjustSizeImage));
}

void MediaWnd::InitUiAndControl()
{
	QAction* pauseOrResume = new QAction("PAUSE_RESUME", this);
	QAction* chooseSource = new QAction("ChooseSource", this);
	this->menuBar()->addAction(chooseSource);
	this->menuBar()->addAction(pauseOrResume);
	connect(m_videoplayer, SIGNAL(sig_GetOneFrame(QImage)), this, SLOT(slot_GotOneDecodecPicture(QImage)));
	connect(pauseOrResume, SIGNAL(triggered()), this, SLOT(PauseOrResume()));
	connect(chooseSource, SIGNAL(triggered()), m_SourceChooseDialog, SLOT(show()));
	connect(m_SourceChooseDialog, SIGNAL(sign_DownLoad()), this, SLOT(slot_UpdatMediaSource()));
}

void MediaWnd::slot_GotOneDecodecPicture(QImage srcImage)
{
	mImage = srcImage;
	update(); //调用update将执行 paintEvent函数
}


void MediaWnd::PauseOrResume()
{
	m_videoplayer->PauseOrResume();
}
void MediaWnd::slot_UpdatMediaSource()
{
	m_timer.stop();
	QString newSource = m_SourceChooseDialog->GetFilePath();
	QByteArray ba = newSource.toLocal8Bit();
	m_videoplayer->setFileName(newSource);
	m_videoplayer->startPlay();

	m_SourceChooseDialog->setVisible(false);
}

void MediaWnd::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.setBrush(Qt::black);
	painter.drawRect(0, 0, this->width(), this->height()); //先画成黑色

	if (mImage.size().width() <= 0) {
		return;
	}

	///将图像按比例缩放成和窗口一样大小
	QImage img = mImage.scaled(this->size(), Qt::KeepAspectRatio);

	int x = this->width() - img.width();
	int y = this->height() - img.height();

	x /= 2;
	y /= 2;

	painter.drawImage(QPoint(x, y), img); //画出图像
}
