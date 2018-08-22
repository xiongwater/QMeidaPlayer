#ifndef WATERMARKER_H
#define WATERMARKER_H


/**
 *
 * 使用方法参见 Realplayer 和 RealplayHandler 的截图相关代码
 *
 */

//#include <clientcommon_global.h>
#include <QtCore/QObject>
#include <QtCore/QVariantHash>

class QImage;

class  WaterMarker
{

public:
	static void add(QImage &, const QString &, int pos = 0);

};


/************************************************************************/
/*                                                                      */
/************************************************************************/
class  WatermarkHandler : public QObject
{
	Q_OBJECT

public slots:
	void addWatermark(QVariantHash);

};


#endif //WATERMARKER_H