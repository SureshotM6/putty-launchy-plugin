#pragma once

using namespace System;
using namespace System::ComponentModel;
using namespace System::Collections;
using namespace System::Windows::Forms;
using namespace System::Data;
using namespace System::Drawing;


namespace PuTTY {

	/// <summary>
	/// Summary for PluginOptionsForm
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class PluginOptionsForm : public System::Windows::Forms::Form
	{
	public:
		PluginOptionsForm(String^ PathToPutty)
		{
			InitializeComponent();
			
			TextBoxPathToPutty->Text = PathToPutty;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~PluginOptionsForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::OpenFileDialog^  FileBrowseDialog;
	protected: 

	private: System::Windows::Forms::TextBox^  TextBoxPathToPutty;
	protected: 

	private: System::Windows::Forms::Button^  ButtonBrowse;
	private: System::Windows::Forms::Button^  ButtonCancel;


	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Button^  ButtonOk;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->FileBrowseDialog = (gcnew System::Windows::Forms::OpenFileDialog());
			this->TextBoxPathToPutty = (gcnew System::Windows::Forms::TextBox());
			this->ButtonBrowse = (gcnew System::Windows::Forms::Button());
			this->ButtonCancel = (gcnew System::Windows::Forms::Button());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->ButtonOk = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// FileBrowseDialog
			// 
			this->FileBrowseDialog->FileName = L"PuTTY.exe";
			this->FileBrowseDialog->Filter = L"PuTTY.exe|PuTTY.exe";
			this->FileBrowseDialog->InitialDirectory = L"C:\\Program Files\\PuTTY";
			// 
			// TextBoxPathToPutty
			// 
			this->TextBoxPathToPutty->Location = System::Drawing::Point(12, 29);
			this->TextBoxPathToPutty->Name = L"TextBoxPathToPutty";
			this->TextBoxPathToPutty->Size = System::Drawing::Size(468, 20);
			this->TextBoxPathToPutty->TabIndex = 0;
			// 
			// ButtonBrowse
			// 
			this->ButtonBrowse->Location = System::Drawing::Point(12, 55);
			this->ButtonBrowse->Name = L"ButtonBrowse";
			this->ButtonBrowse->Size = System::Drawing::Size(75, 23);
			this->ButtonBrowse->TabIndex = 1;
			this->ButtonBrowse->Text = L"Browse...";
			this->ButtonBrowse->UseVisualStyleBackColor = true;
			this->ButtonBrowse->Click += gcnew System::EventHandler(this, &PluginOptionsForm::ButtonBrowse_Click);
			// 
			// ButtonCancel
			// 
			this->ButtonCancel->DialogResult = System::Windows::Forms::DialogResult::Cancel;
			this->ButtonCancel->Location = System::Drawing::Point(405, 55);
			this->ButtonCancel->Name = L"ButtonCancel";
			this->ButtonCancel->Size = System::Drawing::Size(75, 23);
			this->ButtonCancel->TabIndex = 3;
			this->ButtonCancel->Text = L"Cancel";
			this->ButtonCancel->UseVisualStyleBackColor = true;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->BackColor = System::Drawing::SystemColors::Control;
			this->label1->Location = System::Drawing::Point(13, 10);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(155, 13);
			this->label1->TabIndex = 5;
			this->label1->Text = L"Full Path to \'PuTTY.exe\' binary:";
			// 
			// ButtonOk
			// 
			this->ButtonOk->DialogResult = System::Windows::Forms::DialogResult::OK;
			this->ButtonOk->Location = System::Drawing::Point(324, 55);
			this->ButtonOk->Name = L"ButtonOk";
			this->ButtonOk->Size = System::Drawing::Size(75, 23);
			this->ButtonOk->TabIndex = 2;
			this->ButtonOk->Text = L"OK";
			this->ButtonOk->UseVisualStyleBackColor = true;
			// 
			// PluginOptionsForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(492, 84);
			this->Controls->Add(this->ButtonOk);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->ButtonCancel);
			this->Controls->Add(this->ButtonBrowse);
			this->Controls->Add(this->TextBoxPathToPutty);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedDialog;
			this->MaximizeBox = false;
			this->MinimizeBox = false;
			this->Name = L"PluginOptionsForm";
			this->ShowIcon = false;
			this->StartPosition = System::Windows::Forms::FormStartPosition::CenterParent;
			this->Text = L"PuTTY Plugin Options";
			this->TopMost = true;
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
private: System::Void ButtonBrowse_Click(System::Object^  sender, System::EventArgs^  e);
public:
	System::String^ PathToPutty() {
		return this->TextBoxPathToPutty->Text;
	}
};
}
