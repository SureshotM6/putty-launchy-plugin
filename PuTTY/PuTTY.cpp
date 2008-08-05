// BlankPlugin.cpp : Defines the entry point for the DLL application.
//

#include "PuTTY.h"

Options *PuttyPlugin::opt = NULL;

const QString PuttyPlugin::PLUGIN_NAME = "PuTTY";
const QString PuttyPlugin::PLUGIN_VERSION = "2.2";
const uint PuttyPlugin::HASH_PUTTY = qHash(PuttyPlugin::PLUGIN_NAME);

PuttyPlugin::PuttyPlugin()
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
		case MSG_PATH:
			setPath((QString*) wParam);
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


unsigned int PuttyPlugin::addSessionsToList(QList<CatItem>* list, PuttySessions::SessionType type, QString matchText)
{
	QStringList matchedSessions;
	PuttySessions sessions = PuttySessions(type);
	QString typestr = PuttySessions::TypeToString(type);

	if (!matchText.isEmpty()) {
		if (opt->keywordSearch) {
			matchedSessions = sessions.matchKeywords(matchText);
		}else{
			matchedSessions = sessions.matchLiteral(matchText);
		}
	}else{
		matchedSessions = sessions;
	}

	foreach (QString session, matchedSessions) {
		list->push_front(CatItem(session + "." + typestr, session, HASH_PUTTY, getIcon()));
	}

	return matchedSessions.size();
}

void PuttyPlugin::getResults(QList<InputData>* id, QList<CatItem>* results)
{
	unsigned int matchCount = 0;
	QString text = id->last().getText();
	bool regexMatch = id->first().hasLabel(HASH_PUTTY);
	bool textMatch = (id->first().getTopResult().id == HASH_PUTTY && id->count() > 1);

	if (textMatch || regexMatch) {
		if (regexMatch) {
			text.replace(QRegExp("^[^ ]* +", Qt::CaseInsensitive), "");
		}

		if (opt->sessionsFromReg) {
			matchCount += addSessionsToList(results, PuttySessions::REGISTRY, text);
		}

		if (opt->sessionsFromFs) {
			matchCount += addSessionsToList(results, PuttySessions::FILESYSTEM, text);
		}

		if (opt->passArgs && matchCount == 0) {
			results->push_front(CatItem(text + "." + PuttySessions::TypeToString(PuttySessions::CMDLINE), text, HASH_PUTTY, getIcon()));
		}
	}
}

QString PuttyPlugin::getIcon()
{
	return libPath + "/icons/putty.png";
}

void PuttyPlugin::setPath(QString * path)
{
    libPath = *path;
}

void PuttyPlugin::getCatalog(QList<CatItem>* items)
{
	foreach (QString trigger, opt->textTriggers)
	{
		items->push_back(CatItem(trigger + "." + PLUGIN_NAME, trigger, HASH_PUTTY, getIcon()));
	}

	if (opt->catalogSessions) {
		if (opt->sessionsFromReg) {
			addSessionsToList(items, PuttySessions::REGISTRY, "");
		}

		if (opt->sessionsFromFs) {
			addSessionsToList(items, PuttySessions::FILESYSTEM, "");
		}
	}
}

void PuttyPlugin::launchItem(QList<InputData>* id, CatItem* item)
{
	QString file = (opt->pathToPutty.isEmpty())?("putty.exe"):(opt->pathToPutty);
	QString args = "";

	CatItem result = id->last().getTopResult();

	QString sessionName = result.shortName;
	PuttySessions::SessionType type = PuttySessions::StringToType(result.fullPath);

	switch (type) {
	case PuttySessions::REGISTRY:
		PuttySessions::EscapeQuotes(sessionName);
		args = "-load \"" + sessionName + "\"";
		break;
	case PuttySessions::FILESYSTEM:
		PuttySessions::EscapeQuotes(sessionName);
		args = "-loadfile \"" + sessionName + "\"";
		break;
	case PuttySessions::CMDLINE:
		args = sessionName;
		break;
	default:
		/* just start putty by itself */
		args = "";
		break;
	}

	if (runProgramWin(file, args, opt->startMaximized) == false) {
		MessageBox(NULL,
			L"Cannot launch PuTTY!  Please make sure that 'putty.exe' is either in your PATH or select the path to PuTTY in the options dialog.",
			NULL,
			MB_OK);
	}
}

void PuttyPlugin::doDialog(QWidget* parent, QWidget** newDlg) {
	*newDlg = opt->getDlg(parent);
}

void PuttyPlugin::endDialog(bool accept) {
	opt->killDlg(accept);
}

bool PuttyPlugin::runProgramWin(QString path, QString args, bool maximize)
{
	SHELLEXECUTEINFO ShExecInfo;

	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_FLAG_NO_UI;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = (LPCTSTR) (path).utf16();
	if (args != "") {
		ShExecInfo.lpParameters = (LPCTSTR) args.utf16();
	} else {
		ShExecInfo.lpParameters = NULL;
	}
	QDir dir(path);
	QFileInfo info(path);
	if (!info.isDir() && info.isFile())
		dir.cdUp();
	ShExecInfo.lpDirectory = (LPCTSTR)QDir::toNativeSeparators(dir.absolutePath()).utf16();
	ShExecInfo.nShow = (maximize)?(SW_SHOWMAXIMIZED):(SW_SHOWNORMAL);
	ShExecInfo.hInstApp = NULL;

	return (bool) ShellExecuteEx(&ShExecInfo);	
}


Q_EXPORT_PLUGIN2(putty, PuttyPlugin) 
