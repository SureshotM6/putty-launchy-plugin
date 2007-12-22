#pragma once

#include <QStringList>
#include <Windows.h>

class PuttySessions : public QStringList
{
private:
	static const TCHAR REG_POS[];
	QString unmungeString(const TCHAR in[]) const;
	void loadSessions();

public:
	PuttySessions();

	QStringList matchKeywords(const QString& needle) const;
	QStringList matchLiteral(const QString& needle) const;

	static void EscapeQuotes(QString &s);
	//bool FoundAllKeywords(const QString &haystack, const QString &keywords);
};
