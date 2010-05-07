#pragma once

#include <QObject>
#include <QSettings>
#include "OptionsDlg.h"

class OptionsDlg;

class Options
{
private:
	QSettings *settings;
	OptionsDlg* dlg;
public:

#define XSTRING(name, def) \
	QString name;
#define XBOOL(name, def) \
	bool name;
#define XSTRINGLIST(name, def) \
	QStringList name;

#include "options.def"

#undef XSTRING
#undef XBOOL
#undef XSTRINGLIST

	Options(QSettings* _settings);
	void readOptions();
	void writeOptions();
	void writeDefaults();
	void upgradeDefaults();
	QWidget* getDlg(QWidget* parent);
	void killDlg(bool save);
	QString getVersion();
	void setVersion(const QString& version);
};
