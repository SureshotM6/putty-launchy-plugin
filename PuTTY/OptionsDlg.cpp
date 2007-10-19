#include "stdafx.h"
#include "OptionsDlg.h"

using namespace std;

BOOL OnInitDialog(HWND hwnd)
{
	return SetDlgItemText(hwnd, IDC_EDIT_PATH_TO_PUTTY, PathToPutty.c_str());
}


void GetPathToPuTTY(HWND hwnd)
{
	int len;
	wchar_t *buf;
	
	len = GetWindowTextLength(GetDlgItem(hwnd, IDC_EDIT_PATH_TO_PUTTY)) + 1;

	buf = (wchar_t*) malloc(sizeof(wchar_t) * len);

	if (buf)
	{
		GetDlgItemText(hwnd, IDC_EDIT_PATH_TO_PUTTY, buf, len);
		PathToPutty = buf;
		free(buf);
	}
}


BOOL BrowseDialog(HWND hwnd)
{
	OPENFILENAME ofn;       // common dialog box structure
	wchar_t fileName[260];       // buffer for file name

	wcscpy_s(fileName, L"PuTTY.exe");

	// Initialize OPENFILENAME
	memset(&ofn, 0x00, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = fileName;
	ofn.nMaxFile = sizeof(fileName);
	ofn.lpstrFilter = L"PuTTY.exe\0PuTTY.exe\0All Files (*.*)\0*.*\0\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrTitle = L"Select Path to PuTTY.exe";
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = L"c:\\Program Files\\PuTTY\\";
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 
	if (GetOpenFileName(&ofn)==TRUE)
		SetDlgItemText(hwnd, IDC_EDIT_PATH_TO_PUTTY, fileName);

	return TRUE;
}

 
INT_PTR CALLBACK OptionsDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) 
{ 
    switch (message) 
    {
        case WM_INITDIALOG:
			return OnInitDialog(hwndDlg);
			break;
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDOK:
					GetPathToPuTTY(hwndDlg);
                    return EndDialog(hwndDlg, IDOK);
	                break;
                case IDC_BUTTON_BROWSE:
					return BrowseDialog(hwndDlg);
					break;
				case IDCANCEL:
					return EndDialog(hwndDlg, IDCANCEL);
					break;
				default:
					return FALSE;
            }
			break; 
        default: 
            return FALSE; 
    }
}
