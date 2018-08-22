#include <utils/stylehelper.h>
#include <QtCore/QFile>
#include <QtCore/QStringList>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtCore/QSettings>
#include <QtCore/QDebug>

static const int GLOBAL = StyleHelper::CUSTOM - 1;

typedef QHash<QWidget *, QString> StyleModule;//样式表
static QHash<int, QHash<QString, QString> > _colorConfig;//存储配置文件内容
static QHash<int, QStringList> _globalStyles;//不同风格的qss列表
static QHash<int, StyleModule> _customStyles;
static int _currentStyle = GLOBAL;

StyleHelper::StyleHelper(QObject *parent)
	: QObject(parent)
{
	//将配置文件styleconfig.ini内容存放到哈希表里面去 xh
	QSettings st(":/style/config", QSettings::IniFormat);
	foreach (QString s, st.childGroups()) {
		st.beginGroup(s);
		int id = st.value("id").toInt();
		if (id < CUSTOM) continue;
		QHash<QString, QString> &hash = _colorConfig[id];
		foreach (QString key, st.childKeys()) {
			hash.insert(key, st.value(key).toString());
		}
		st.endGroup();
	}
}

StyleHelper::~StyleHelper()
{

}

void StyleHelper::applyStyle(StyleName name)
{
	if (name <= CUSTOM || name == _currentStyle) return;
	if (!_globalStyles.contains(name)) {
		qWarning() << "StyleHelper::applyStyle called with a invalid sytle name";
		return;
	}
	_currentStyle = name;

	QStringList list(_globalStyles.value(GLOBAL));
	list << _globalStyles.value(name);
	loadStyleSheet(list);

	StyleModule customModule = _customStyles.value(CUSTOM);
	StyleModule styleModule = _customStyles.value(name);
	StyleModule::const_iterator it = styleModule.constBegin();
	while (it != styleModule.constEnd()) {
		QWidget *target = it.key();
		QStringList list;
		if (customModule.contains(target)) {
			list << customModule.value(target);
		}
		list << it.value();
		loadStyleSheet(list, target);
		++it;
	}
}


void StyleHelper::registGlobalStyle(const QString &url)
{
	registGlobalStyle(StyleName(GLOBAL), url);
}

void StyleHelper::registGlobalStyle(StyleName name, const QString &url)
{
	if (!_globalStyles.contains(name)) _globalStyles.insert(name, QStringList());
	QStringList &list = _globalStyles[name];
	list << url;
}

//@static
void StyleHelper::registStyle(StyleName name, QWidget *target, const QString &url)
{
	if (!_customStyles.contains(name)) _customStyles.insert(name, StyleModule());
	StyleModule &module = _customStyles[name];
	if (module.contains(target)) {
		qWarning() << tr("StyleHelper::registStyle target %0 registed a %2 style yet")
			.arg(target->metaObject()->className()).arg(name);
		return;
	}
	module.insert(target, url);
	if (_currentStyle == name) {
		QStringList list(url);
		StyleModule cModule = _customStyles.value(CUSTOM);
		const QString &cStyle = cModule.value(target);
		if (!cStyle.isEmpty()) {
			list << cStyle;
		}
		loadStyleSheet(list, target);
	}
}

//@static
void StyleHelper::applyButtonColor(QWidget *widget, const QString &color)
{
	widget->setProperty("_hw_color_", color);
}

//@static
void StyleHelper::applyBoldFont(QWidget *widget)
{
	widget->setProperty("_hw_bold_font_", "true");
}

//@static
void StyleHelper::polishStyle(QWidget *widget)
{
	Q_ASSERT(widget);
	widget->setStyleSheet(widget->styleSheet());
}

//load style file for a widget
void StyleHelper::loadStyleSheet(const QStringList &files, QWidget *widget)
{
	QString txt;
	foreach (QString s, files) {
		if (s.isEmpty()) continue;
		QFile qss(s);
		if (!qss.open(QFile::ReadOnly)) {
			qWarning() << QObject::tr("Can't find qt stylesheet file %0").arg(qss.fileName());
			return;
		}
		txt.append(qss.readAll());
	}
	if (txt.isEmpty()) { return; }

	replaceColorDefines(txt);

	if (widget) {
		widget->setStyleSheet(txt);
	} else {
		qApp->setStyleSheet(txt);
	}
}

void StyleHelper::loadStyleSheet(const QString &file, QWidget *widget)
{
	loadStyleSheet(QStringList(file), widget);
}

void StyleHelper::replaceColorDefines(QString &txt)
{
	if (!_colorConfig.contains(_currentStyle)) { return; }
	const QHash<QString, QString> &colors = _colorConfig.value(_currentStyle);
	QHash<QString, QString>::const_iterator it = colors.constBegin();
	while (it != colors.constEnd()) {
		txt.replace(QString("$(%0)").arg(it.key()), it.value());
		++it;
	}
}
