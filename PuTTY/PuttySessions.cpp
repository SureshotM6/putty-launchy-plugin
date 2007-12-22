#include "PuttySessions.h"

const TCHAR PuttySessions::REG_POS[] = L"Software\\SimonTatham\\PuTTY\\Sessions";

PuttySessions::PuttySessions()
	: QStringList()
{
	loadSessions();
}

QString PuttySessions::unmungeString(const TCHAR in[]) const
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

void PuttySessions::loadSessions()
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

void PuttySessions::EscapeQuotes(QString &s)
{
	s.replace("\"", "\\\"");
}

QStringList PuttySessions::matchKeywords(const QString& needle) const
{
	QStringList list = *this;

	foreach(QString keyword, needle.split(" ")) {
		list = list.filter(keyword, Qt::CaseInsensitive);
	}

	return list;
}

QStringList PuttySessions::matchLiteral(const QString& needle) const
{
	return this->filter(needle, Qt::CaseInsensitive);
}
