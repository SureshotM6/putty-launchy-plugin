#include "PluginOptionsForm.h"

using namespace PuTTY;

void PluginOptionsForm::ButtonBrowse_Click(System::Object ^sender, System::EventArgs ^e)
{
	if (::DialogResult::OK == FileBrowseDialog->ShowDialog()) {
		TextBoxPathToPutty->Text = FileBrowseDialog->FileName;
	}
}