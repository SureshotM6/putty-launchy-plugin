#pragma once

#include <QtPlugin>
#include <QRegExp>
#include "plugin_interface.h"
#include "Options.h"
#include "PuttySessions.h"

class PuttyPlugin : public QObject, public PluginInterface
{
	Q_OBJECT
	Q_INTERFACES(PluginInterface)

private:
	static const QString PLUGIN_NAME;
	static const QString PLUGIN_VERSION;
	static const uint HASH_PUTTY;
	static const QString PUTTY_ARGS;

	Options *opt;
public:
	PuttyPlugin(); 

	int msg(int msgId, void* wParam = NULL, void* lParam = NULL); 

	void getLabels(QList<InputData>*);
	void getID(uint*);
	void getName(QString*);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void getCatalog(QList<CatItem>* items);
	void launchItem(QList<InputData>*, CatItem*);
	void doDialog(QWidget* parent, QWidget**);
	void endDialog(bool accept);
	void init();
	QString getIcon();
};
