#pragma once

#include <QStringList>
#include <Windows.h>

class PuttySessions : public QStringList
{
private:
	static const TCHAR REG_POS[];
	static QString unmungeString(const TCHAR in[]);
	void loadRegSessions();
	void loadFsSessions();

	static const QString PUTTY_CMDLINE;
	static const QString PUTTY_REGISTRY;
	static const QString PUTTY_FILESYSTEM;

public:
	static const enum SessionType {
		UNKNOWN,
		REGISTRY,
		FILESYSTEM,
		CMDLINE
	};

	static QString TypeToString(SessionType type);
	static SessionType StringToType(QString string);

	PuttySessions(SessionType type);

	QStringList matchKeywords(const QString& needle) const;
	QStringList matchLiteral(const QString& needle) const;

	static void EscapeQuotes(QString &s);
	//bool FoundAllKeywords(const QString &haystack, const QString &keywords);
};
