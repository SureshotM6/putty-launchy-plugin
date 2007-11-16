// BlankPlugin.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "PuTTY.h"
#include "LaunchyPlugin.h"
#include "OptionsDlg.h"
#include "resource.h"
#include <vector>
#include <string>
#include <algorithm>
#include <iterator>
#include <sstream>

HINSTANCE hInstance;
wstring PathToPutty;

void unmungestr(const wstring &in_s, wstring &out_s)
{
	wstring::const_iterator in;

	out_s.clear();
	back_insert_iterator<wstring> out(out_s);

	for (in = in_s.begin(); in != in_s.end(); ++in) {
		if (*in == '%' && in[1] && in[2]) {
			int i, j;

			i = in[1] - '0';
			i -= (i > 9 ? 7 : 0);
			j = in[2] - '0';
			j -= (j > 9 ? 7 : 0);

			*out++ = (i << 4) + j;

			in += 2;
		}else{
			*out++ = *in;
		}
	}
}

void EscapeQuotes(const wstring &in_s, wstring &out_s)
{
	wstring::const_iterator in;

	out_s.clear();
	back_insert_iterator<wstring> out(out_s);

	for (in = in_s.begin(); in != in_s.end(); ++in) {
		if (*in == '"') {
			*out++ = '\\';
			*out++ = '"';
		}else{
			*out++ = *in;
		}
	}
}

void lc(wstring &in) {
	transform(in.begin(), in.end(), in.begin(), tolower);
}

void lc(const wstring &in, wstring &out) {
	back_insert_iterator<wstring> output(out);
	out.clear();
	transform(in.begin(), in.end(), output, tolower);
}

vector<wstring> GenerateSessionNames()
{
	vector<wstring> sessions;
	HKEY key;
	int i;
	wchar_t keyname[255];
	wstring keyname_unmunged;

	if (RegOpenKey(HKEY_CURRENT_USER, PUTTY_SESSION_REG_POS, &key) == ERROR_SUCCESS) {
		for (i=0; RegEnumKey(key, i, keyname, sizeof(keyname)) == ERROR_SUCCESS; ++i) {
			//remove %xx's
			unmungestr(wstring(keyname), keyname_unmunged);

			sessions.push_back(keyname_unmunged);
		}

		RegCloseKey(key);
	}

	return sessions;
}

#ifdef _MANAGED
#pragma managed(push, off)
#endif

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	hInstance = hModule;
    return TRUE;
}

#ifdef _MANAGED
#pragma managed(pop)
#endif

bool PluginOwnsSearch (TCHAR* txt) 
{
	return false;
}



SearchResult* PluginGetIdentifiers (int* iNumResults)
{
	vector<SearchResult> results;
	results.push_back(makeResult(L"PuTTY", L"", L"", NULL));
	results.push_back(makeResult(L"ssh", L"", L"", NULL));

	*iNumResults = (int) results.size();
	return ResultVectorToArray(results);
}

TCHAR* PluginGetRegexs(int* iNumResults)
{
	vector<wstring> vect;
	vect.push_back(L"^[Pp][uU][Tt][Tt][Yy] .*");
	vect.push_back(L"^[sS][sS][hH] .*");
	*iNumResults = (int) vect.size();
	return StringVectorToTCHAR(vect);
}

bool FoundAllKeywords(const wstring &haystack, const wstring &keywords) {
	wstring needle;
	wstringstream tokenizer(keywords);

	while (tokenizer >> needle) {
		if (wstring::npos == haystack.find(needle)) {
			return false;
		}
	}

	return true;
}

SearchResult* PluginUpdateSearch (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults)
{
	vector<SearchResult> results;
	vector<wstring> sessions;
	vector<wstring>::const_iterator session;
	wstring session_lc;
	wstring searchString = FinalString;

	if (NumStrings == 0) {
		//we are in a regex
		wstring::size_type start;
		start = searchString.find_first_of(L" ");
		if (wstring::npos != start)
			searchString = searchString.substr(start);
	}else if (NumStrings >= 2) {
		//person has hit tab twice, so ignore it
		*NumResults = 0;
		return NULL;
	}

	lc(searchString);

	//add sessions here
	sessions = GenerateSessionNames();

	for (session = sessions.begin(); session != sessions.end(); ++session) {
		//make the keyname lowercase for searching
		lc(*session, session_lc);

		//only add if the search string is found
		if (FoundAllKeywords(session_lc, searchString)) {
			results.push_back(makeResult(*session, L"", PLUGIN_NAME L" " + *session, NULL));
		}
	}

	*NumResults = (int) results.size();
	return ResultVectorToArray(results);
}

SearchResult* PluginFileOptions (const TCHAR* FullPath, int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, int* NumResults) 
{
	*NumResults = 0;
	return NULL;
}


void PluginDoAction (int NumStrings, const TCHAR* Strings, const TCHAR* FinalString, const TCHAR* FullPath) {
	wstring cmd, params, profile, safe_profile;
	vector<wstring> sessions;

	cmd = PathToPutty.empty() ? L"putty" : PathToPutty;

	if (NumStrings >= 2) {
		//try to remove header if person hit tab too many times
		vector<wstring> stringVec = TCHARListToVector(NumStrings, Strings);

		profile = stringVec[1];
	}else
	if (wcslen(FullPath) != 0) {
		//we matched a profile
		profile = FullPath;
	}else
	if (NumStrings == 0) {
		//we are in a regex
		profile = FinalString;
		wstring::size_type start;
		start = profile.find_first_of(L" ");
		if (wstring::npos != start) {
			profile = profile.substr(start);
		}else{
			profile = L"";
		}
	}else{
		//we are in normal tab completion with no matches
		profile = FinalString;
	}

	sessions = GenerateSessionNames();

	if (find(sessions.begin(), sessions.end(), profile) != sessions.end()) {
		EscapeQuotes(profile, safe_profile);

		params = L"-load \"" + safe_profile + L"\"";
	}else{
		//not a profile, so try to load as a hostname
		params = profile;
	}

	SHELLEXECUTEINFO ShExecInfo;
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = NULL;
	ShExecInfo.hwnd = NULL;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = cmd.c_str();
	ShExecInfo.lpParameters = params.c_str();
	ShExecInfo.lpDirectory = NULL;
	ShExecInfo.nShow = SW_NORMAL;
	ShExecInfo.hInstApp = NULL;

	ShellExecuteEx(&ShExecInfo);
}

TCHAR* PluginGetSeparator() {
	wstring tmp = L" ";
	return string2TCHAR(tmp);
}

TCHAR* PluginGetName() {
	wstring tmp = PLUGIN_NAME;
	return string2TCHAR(tmp);
}

TCHAR* PluginGetDescription() {
	wstring tmp = PLUGIN_DESCRIPTION;
	return string2TCHAR(tmp);
}

void PluginClose() {
	return;
}

void PluginInitialize() {
	PathToPutty = RetrieveString(L"PUTTY_PLUGIN_PATH_TO_PUTTY");
}

void PluginSaveOptions() {
	StoreString(L"PUTTY_PLUGIN_PATH_TO_PUTTY", PathToPutty);
}

void PluginCallOptionsDlg(HWND parent) {
	DialogBox(hInstance, MAKEINTRESOURCE(IDD_OPTIONS), parent, OptionsDlgProc);
}

bool PluginHasOptionsDlg() {
	return true;
}

