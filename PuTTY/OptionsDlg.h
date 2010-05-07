#pragma once

#include <QObject>
#include <QWidget>
#include "ui_putty_options.h"
#include "Options.h"

class Options;

class OptionsDlg : public QWidget, private Ui::PuttyOptions
{
	Q_OBJECT

private:
	Options *opt;
public:
	OptionsDlg(QWidget* parent, Options *options);

	void writeBack();
	void syncOptions();
private slots:
	void on_pathToPuttyBrowse_clicked();
	void on_addTextTriggerButton_clicked();
	void on_removeTextTriggerButton_clicked();
	void on_resetAllButton_clicked();
	void on_sessionsFromFsCheck_stateChanged(int state);
};
