#ifndef _STYLE_HELPER_H_
#define _STYLE_HELPER_H_

//#include <clientcommon_global.h>
#include <QtCore/QObject>
#include <QtCore/QHash>

class QWidget;
class QString;
class QStringList;

class StyleHelper : public QObject
{
	friend class ClientApp;

public:
	enum StyleName { 
		//GLOBAL = 0 // 全局样式，用于app，在更改全局样式时，这个不受影响
		CUSTOM = 10 // 用于widget特有样式，在更改全局样式时，这个受影响
		, DARK // 颜色
		, LIGHT // 颜色
		, TEST // 测试
	};

private:
	explicit StyleHelper(QObject *parent = 0);
	~StyleHelper();
	Q_DISABLE_COPY(StyleHelper)

	void applyStyle(StyleName);

	void registGlobalStyle(const QString &url);
	void registGlobalStyle(StyleName name, const QString &url);

public:
	static void registStyle(StyleName name, QWidget *target, const QString &url);
	static void applyButtonColor(QWidget *widget, const QString &color);
	static void applyBoldFont(QWidget *widget);

	static void polishStyle(QWidget *);

	static void loadStyleSheet(const QString &file, QWidget *widget = 0);
	static void loadStyleSheet(const QStringList &files, QWidget *widget = 0);

private:
	static void replaceColorDefines(QString &);
};

#endif //_STYLE_HELPER_H_