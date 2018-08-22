#include <QtGui/QPainter>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QStringList>
#include <QtCore/QEventLoop>
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtGui/QImage>
#include <utils/watermarker.h>

void WaterMarker::add(QImage &image, const QString &text, int pos)
{
	if (text.isEmpty()) return;

	//QRect rect = image.rect().adjusted(0, image.height() - 40, 0, 0);
	QRect rect = image.rect().adjusted(0, 0, 0, 0);
	QFont font;
	//font.setWeight(20);
	//font.setPixelSize(18);
	font.setWeight(image.rect().height() /6);
	font.setPixelSize(image.rect().height() / 7);
	QPainter pt(&image);
	//pt.setPen(Qt::green);
	static unsigned int s_count = 0;
	static unsigned int s_colour = 12;
	int sureChange = s_count % 5;

	if (0 == sureChange)
	{	
		s_colour++;
		if (s_colour > 12)
		{
			s_colour = 7;
		}
	/*	qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
		s_colour = qrand() % 20;*/
	}
	s_count++;
	s_count = s_count > 1000 ? 0 : s_count;
	switch (s_colour)
	{
	case 7:
		pt.setPen(Qt::red);
		break;
	case 8:
		pt.setPen(Qt::green);
		break;
	case 9:
		pt.setPen(Qt::blue);
		break;
	case 10:
		pt.setPen(Qt::cyan);
		break;
	case 11:
		pt.setPen(Qt::yellow);
		break;
	case 12:
		pt.setPen(Qt::yellow);
		break;
	default:
		pt.setPen(Qt::white);
		break;
	}

	//pt.setPen(s_colour);
	pt.setFont(font);
	pt.drawText(rect, text);
}


/************************************************************************/
/*                                                                      */
/************************************************************************/
/************************************************************************/
/*                                                                      */
/************************************************************************/
void WatermarkHandler::addWatermark(QVariantHash hash)
{
	QEventLoop ev;
	QTimer::singleShot(2000, &ev, SLOT(quit()));
	ev.exec();

	const QDir &dir(hash.value("dir").toString());
	const QString &filename = hash.value("filename").toString();
	const QString& name = hash.value("name").toString();
	QStringList filter;
	filter << "*.jpg" << "*.jpeg" << "*.png" << "*.bmp";
	QStringList list = dir.entryList(filter);

	for (int i = 0, l = list.size(); i < l; ++i) {
		const QString &ss = list.at(i);
		if (ss.startsWith(name)) {
			QString suffix = ss.split(QLatin1Char('.')).last();
			QString filePathName = filename + QString(".%1").arg(suffix);
			QImage image(filePathName);
			WaterMarker::add(image, hash.value("watermark").toString());
			QFile::remove(filePathName);
			image.save(filePathName);
			break;
		}
	}
}