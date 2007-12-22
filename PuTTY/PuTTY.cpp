// BlankPlugin.cpp : Defines the entry point for the DLL application.
//

#include "PuTTY.h"

const QString PuttyPlugin::PLUGIN_NAME = "PuTTY";
const QString PuttyPlugin::PLUGIN_VERSION = "2.0";
const uint PuttyPlugin::HASH_PUTTY = qHash(PuttyPlugin::PLUGIN_NAME);
const QString PuttyPlugin::PUTTY_ARGS = PuttyPlugin::PLUGIN_NAME + ".args";

PuttyPlugin::PuttyPlugin()
	: opt(NULL)
{
}

int PuttyPlugin::msg(int msgId, void* wParam, void* lParam)
{
	bool handled = false;
	switch (msgId)
	{		
		case MSG_INIT:
			init();
			handled = true;
			break;
		case MSG_GET_LABELS:
			getLabels((QList<InputData>*) wParam);
			handled = true;
			break;
		case MSG_GET_ID:
			getID((uint*) wParam);
			handled = true;
			break;
		case MSG_GET_NAME:
			getName((QString*) wParam);
			handled = true;
			break;
		case MSG_GET_RESULTS:
			getResults((QList<InputData>*) wParam, (QList<CatItem>*) lParam);
			handled = true;
			break;
		case MSG_GET_CATALOG:
			getCatalog((QList<CatItem>*) wParam);
			handled = true;
			break;
		case MSG_LAUNCH_ITEM:
			launchItem((QList<InputData>*) wParam, (CatItem*) lParam);
			handled = true;
			break;
		case MSG_HAS_DIALOG:
			handled = true;
			break;
		case MSG_DO_DIALOG:
			doDialog((QWidget*) wParam, (QWidget**) lParam);
			handled = true;
			break;
		case MSG_END_DIALOG:
			endDialog((bool) wParam);
			handled = true;
			break;

		default:
      handled = false;
			break;
	}
		
	return handled;
}

void PuttyPlugin::init()
{
	if (opt)
		return;

	opt = new Options(*settings);

	if ( opt->getVersion() == "" ) {
		opt->writeDefaults();
	}
	opt->setVersion(PuttyPlugin::PLUGIN_VERSION);
}

void PuttyPlugin::getID(uint* id)
{
	*id = HASH_PUTTY;
}

void PuttyPlugin::getName(QString* str)
{
	*str = PLUGIN_NAME;
}

void PuttyPlugin::getLabels(QList<InputData>* id)
{
	if (opt->useRegex && id->count() == 1) {
		QString text = id->first().getText();

		foreach (const QString trigger, opt->textTriggers)
		{
			QString regex = trigger;

			regex.replace(" ", "_");

			regex = "^" + regex + " ";

			if (text.contains(QRegExp(regex, Qt::CaseInsensitive)))
			{
				id->first().setLabel(HASH_PUTTY);
			}
		}
	}
}

void PuttyPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{
	QString text = id->last().getText();
	bool regexMatch = id->first().hasLabel(HASH_PUTTY);
	bool textMatch = (id->first().getTopResult().id == HASH_PUTTY && id->count() > 1);

	if (textMatch || regexMatch) {
		if (regexMatch) {
			text.replace(QRegExp("^[^ ]* +", Qt::CaseInsensitive), "");
		}

		PuttySessions sessions = PuttySessions();

		QStringList matchedSessions; 

		if (opt->keywordSearch) {
			matchedSessions = sessions.matchKeywords(text);
		}else{
			matchedSessions = sessions.matchLiteral(text);
		}

		if (!matchedSessions.isEmpty()) {
			foreach (QString session, matchedSessions) {
				results->push_front(CatItem(session + "." + PLUGIN_NAME, session, HASH_PUTTY, getIcon()));
			}
		}else
		if (opt->passArgs) {
			results->push_front(CatItem(text + "." + PUTTY_ARGS, text, HASH_PUTTY, getIcon()));
		}
	}
}

QString PuttyPlugin::getIcon()
{
#ifdef Q_WS_WIN
	return qApp->applicationDirPath() + "/Plugins/icons/PuTTY.ico";
#else
	return NULL;
#endif
}

void PuttyPlugin::getCatalog(QList<CatItem>* items)
{
	foreach (QString trigger, opt->textTriggers)
	{
		items->push_back(CatItem(trigger + "." + PLUGIN_NAME, trigger, HASH_PUTTY, getIcon()));
	}

	if (opt->catalogSessions) {
		PuttySessions sessions = PuttySessions();
		
		foreach (QString session, sessions) {
			items->push_back(CatItem(session + "." + PLUGIN_NAME, session, HASH_PUTTY, getIcon()));
		}
	}
}

void PuttyPlugin::launchItem(QList<InputData>* id, CatItem* item)
{
	QString file = (opt->pathToPutty.isEmpty())?("putty.exe"):(opt->pathToPutty);
	QString args = "";

	CatItem result = id->last().getTopResult();

	QString sessionName = result.shortName;
	bool argumentMatch = result.fullPath.endsWith("." + PUTTY_ARGS);

	if (argumentMatch) {
		if (opt->passArgs) {
			args = sessionName;
		}
	}else{
		PuttySessions::EscapeQuotes(sessionName);
		args = "-load \"" + sessionName + "\"";
	}

	runProgram(file, args);
}

void PuttyPlugin::doDialog(QWidget* parent, QWidget** newDlg) {
	*newDlg = opt->getDlg(parent);
}

void PuttyPlugin::endDialog(bool accept) {
	opt->killDlg(accept);
}

Q_EXPORT_PLUGIN2(putty, PuttyPlugin) 
