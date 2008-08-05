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
	static const uint HASH_PUTTY;

	unsigned int addSessionsToList(QList<CatItem>* list, PuttySessions::SessionType type, QString matchText);

	void getLabels(QList<InputData>*);
	void getID(uint*);
	void getName(QString*);
	void getResults(QList<InputData>* id, QList<CatItem>* results);
	void getCatalog(QList<CatItem>* items);
	void launchItem(QList<InputData>*, CatItem*);
	void doDialog(QWidget* parent, QWidget**);
	void endDialog(bool accept);
	void init();
	bool runProgramWin(QString path, QString args, bool maximize);
	QString getIcon();
	void setPath(QString * path);

	QString libPath;

public:
	static const QString PLUGIN_NAME;
	static const QString PLUGIN_VERSION;

	static Options *opt;

	PuttyPlugin(); 

	int msg(int msgId, void* wParam = NULL, void* lParam = NULL); 
};
