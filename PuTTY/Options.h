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
	QString pathToPutty;

	bool catalogSessions;
	bool passArgs;
	bool keywordSearch;
	bool useRegex;

	QStringList textTriggers;

	Options(QSettings* _settings);
	void readOptions();
	void writeOptions();
	void writeDefaults();
	QWidget* getDlg(QWidget* parent);
	void killDlg(bool save);
	QString getVersion();
	void setVersion(const QString& version);
};
