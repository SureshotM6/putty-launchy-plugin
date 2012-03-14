#include "PuttySessions.h"
#include "PuTTY.h"
#include "enumser.h"
#include <QTextStream>

const TCHAR PuttySessions::REG_POS[] = L"Software\\SimonTatham\\PuTTY\\Sessions";

const QString PuttySessions::PUTTY_CMDLINE = PuttyPlugin::PLUGIN_NAME + ".cmdline";
const QString PuttySessions::PUTTY_REGISTRY = PuttyPlugin::PLUGIN_NAME + ".registry";
const QString PuttySessions::PUTTY_FILESYSTEM = PuttyPlugin::PLUGIN_NAME + ".filesystem";
const QString PuttySessions::PUTTY_COMPORT = PuttyPlugin::PLUGIN_NAME + ".comport";

PuttySessions::PuttySessions(SessionType type)
	: QStringList()
{
	switch (type) {
	case REGISTRY:
		loadRegSessions();
		break;
	case FILESYSTEM:
		loadFsSessions();
		break;
	case COMPORT:
		loadComPorts();
		break;
	default:
		break;
	}
}

QString PuttySessions::unmungeString(const TCHAR in[])
{
	QString out = QString();

	for (int i = 0; in[i]; ++i) {
		if (in[i] == '%' && in[i+1] && in[i+2]) {
			int upper_nibble, lower_nibble;

			upper_nibble  = in[i+1] - '0';		//ascii to integer
			upper_nibble -= (upper_nibble > 9 ? 7 : 0);	//compensate for a-f
			lower_nibble  = in[i+2] - '0';
			lower_nibble -= (lower_nibble > 9 ? 7 : 0);

			out += (TCHAR)((upper_nibble << 4) + lower_nibble);		//create byte with nibbles

			i += 2;
		}else{
			out += in[i];
		}
	}

	return out;
}

void PuttySessions::loadRegSessions()
{
	QStringList sessions;
	HKEY key;
	DWORD i, keyname_length;
	TCHAR keyname[256];

	if (RegOpenKeyEx(HKEY_CURRENT_USER, PuttySessions::REG_POS, 0, KEY_ENUMERATE_SUB_KEYS, &key) == ERROR_SUCCESS) {
		keyname_length = sizeof(keyname)/sizeof(TCHAR);
		for (i=0; RegEnumKeyExW(key, i, keyname, &keyname_length, NULL, NULL, NULL, NULL) == ERROR_SUCCESS; ++i) {
			this->append(unmungeString(keyname));
			keyname_length = sizeof(keyname)/sizeof(TCHAR);
		}

		RegCloseKey(key);
	}
}

void PuttySessions::loadFsSessions()
{
	QDir dir;

	if (PuttyPlugin::opt->pathToPutty.isEmpty()) {
		dir = QDir("c:/program files/putty/"); 
	}else{
		dir = QDir(PuttyPlugin::opt->pathToPutty);
		dir.cdUp();
	}

	if (dir.cd(PuttyPlugin::opt->sessionsFromFsDir)) {
		*this += dir.entryList(QDir::Files);
	}
}

void PuttySessions::loadComPorts()
{
	CSimpleArray<UINT> ports;
	CSimpleArray<CString> friendlyNames;
	int i;

	if (!CEnumerateSerial::UsingSetupAPI1(ports, friendlyNames)) {
		return;
	}

	for (i=0; i<ports.GetSize(); i++) {
		QString name;
		QTextStream(&name) << "COM" << ports[i] << " (" << QString::fromWCharArray((LPCTSTR)friendlyNames[i], friendlyNames[i].GetLength()) << ")";
		*this += name;
	}
}

void PuttySessions::EscapeQuotes(QString &s)
{
	s.replace("\"", "\\\"");
}

QStringList PuttySessions::matchKeywords(const QString& needle) const
{
	QStringList list;

	foreach(QString keyword, needle.split(" ")) {
		list = filter(keyword, Qt::CaseInsensitive);
	}

	return list;
}

QStringList PuttySessions::matchLiteral(const QString& needle) const
{
	return this->filter(needle, Qt::CaseInsensitive);
}

QString PuttySessions::TypeToString(SessionType type)
{
	switch (type) {
	case REGISTRY:
		return PUTTY_REGISTRY;
	case FILESYSTEM:
		return PUTTY_FILESYSTEM;
	case COMPORT:
		return PUTTY_COMPORT;
	default:
		return PUTTY_CMDLINE;
	}
}

PuttySessions::SessionType PuttySessions::StringToType(QString string)
{
	if (string.endsWith("." + PUTTY_REGISTRY)) {
		return REGISTRY;
	}else
	if (string.endsWith("." + PUTTY_FILESYSTEM)) {
		return FILESYSTEM;
	}else
	if (string.endsWith("." + PUTTY_COMPORT)) {
		return COMPORT;
	}else
	if (string.endsWith("." + PUTTY_CMDLINE)) {
		return CMDLINE;
	}else{
		return UNKNOWN;
	}
}
