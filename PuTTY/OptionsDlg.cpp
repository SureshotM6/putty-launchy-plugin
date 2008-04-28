#include <QFileDialog>
#include "OptionsDlg.h"

OptionsDlg::OptionsDlg(QWidget* parent, Options *options) 
	: QWidget(parent), opt(options)
{
	setupUi(this);

	syncOptions();
}

void OptionsDlg::writeBack()
{
#define XSTRING(name, def) \
	opt->name = name##Text->text();
#define XBOOL(name, def) \
	opt->name = name##Check->checkState() == Qt::Checked;
#define XSTRINGLIST(name, def) \
	opt->name.clear(); \
	for(int i = 0; i < name##List->count(); ++i) { \
		if (name##List->item(i)->text().isEmpty()) \
			continue; \
		opt->name.append(name##List->item(i)->text()); \
	}

#include "options.def"

#undef XSTRING
#undef XBOOL
#undef XSTRINGLIST
}

void OptionsDlg::syncOptions()
{
#define XSTRING(name, def) \
	name##Text->setText(opt->name);
#define XBOOL(name, def) \
	name##Check->setCheckState(opt->name?(Qt::Checked):(Qt::Unchecked));
#define XSTRINGLIST(name, def) \
	name##List->clear(); \
	foreach (QString text, opt->name) { \
		QListWidgetItem * item = new QListWidgetItem(text); \
		item->setFlags( item->flags() | Qt::ItemIsEditable ); \
		name##List->addItem(item); \
	}

#include "options.def"

#undef XSTRING
#undef XBOOL
#undef XSTRINGLIST
}

void OptionsDlg::on_pathToPuttyBrowse_clicked() 
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Locate PuTTY Executable"), "c:/program files/putty/",
													tr("PuTTY (PuTTY.exe);;All Files (*.*)"));

	if (!fileName.isEmpty()) {
		pathToPuttyText->setText(fileName);
	}
}

void OptionsDlg::on_addTextTriggerButton_clicked()
{
	QString text = textTriggerText->text();

	if (!text.isEmpty()) {
		QListWidgetItem * item = new QListWidgetItem(text);
		item->setFlags( item->flags() | Qt::ItemIsEditable );

		textTriggersList->addItem(item);
		textTriggerText->clear();
	}
}

void OptionsDlg::on_removeTextTriggerButton_clicked()
{
	int index = textTriggersList->currentRow();

	delete textTriggersList->takeItem(index);
}

void OptionsDlg::on_resetAllButton_clicked()
{
	opt->writeDefaults();
	syncOptions();
}
