#include "MediaWnd.h"
#include <QtWidgets/QApplication>
#include <QtCore/QTranslator>
#undef main
int main(int argc, char *argv[])
{

	 
	QApplication a(argc, argv);
	//加载Qt中的资源文件，使Qt显示中文（包括QMessageBox、文本框右键菜单等）
	/*QTranslator translator;
	translator.load(":/FFMPEG_LEARN/qt_zh_CN");
	a.installTranslator(&translator);*/
	MediaWnd w;
	w.show();
	return a.exec();

}
