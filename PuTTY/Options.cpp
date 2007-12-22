#include "Options.h"

Options::Options(QSettings* _settings)
	: settings(_settings), dlg(NULL)
{
	readOptions();
}

void Options::readOptions()
{
	int arraySize;

	//strings
	pathToPutty = settings->value("PuTTY/pathToPuTTY").toString();

	//boolean
	catalogSessions = settings->value("PuTTY/catalogSessions").toBool();
	passArgs = settings->value("PuTTY/passArgs").toBool();
	keywordSearch = settings->value("PuTTY/keywordSearch").toBool();
	useRegex = settings->value("PuTTY/useRegex").toBool();

	//arrays
	textTriggers.clear();
	arraySize = settings->beginReadArray("PuTTY/textTriggers");
		for (int i = 0; i < arraySize; ++i) {
			settings->setArrayIndex(i);
			textTriggers.append(settings->value("trigger").toString());
		}
	settings->endArray();
}

void Options::writeOptions()
{
	//strings
	settings->setValue("PuTTY/pathToPuTTY", pathToPutty);

	//boolean
	settings->setValue("PuTTY/catalogSessions", catalogSessions);
	settings->setValue("PuTTY/passArgs", passArgs);
	settings->setValue("PuTTY/keywordSearch", keywordSearch);
	settings->setValue("PuTTY/useRegex", useRegex);

	//arrays
	settings->beginWriteArray("PuTTY/textTriggers");
		for (int i = 0; i < textTriggers.size(); ++i) {
			settings->setArrayIndex(i);
			settings->setValue("trigger", textTriggers.at(i));
		}
	settings->endArray();
}

void Options::writeDefaults()
{
	//strings
	pathToPutty = QString("");

	//boolean
	catalogSessions = false;
	passArgs = true;
	keywordSearch = true;
	useRegex = true;

	//arrays
	textTriggers.clear();
	textTriggers.append("PuTTY");
	textTriggers.append("ssh");

	//write back
	writeOptions();
}

QWidget* Options::getDlg(QWidget* parent)
{
	if (dlg == NULL)
		dlg = new OptionsDlg(parent, this);

	return dlg;
}

void Options::killDlg(bool save)
{
	if (save) {
		dlg->writeBack();

		writeOptions();
	}

	delete dlg;

	dlg = NULL;
}

QString Options::getVersion()
{
	return settings->value("PuTTY/version").toString();
}

void Options::setVersion(const QString& version)
{
	settings->setValue("PuTTY/version", version);
}
