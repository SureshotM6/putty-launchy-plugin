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
	//strings
	opt->pathToPutty = pathToPuttyText->text();

	//boolean
	opt->catalogSessions = catalogSessionsCheck->checkState() == Qt::Checked;
	opt->passArgs = passArgsCheck->checkState() == Qt::Checked;
	opt->keywordSearch = keywordSearchCheck->checkState() == Qt::Checked;
	opt->useRegex = useRegexCheck->checkState() == Qt::Checked;

	//arrays
	opt->textTriggers.clear();
	for(int i = 0; i < textTriggersList->count(); ++i) {
		if (textTriggersList->item(i)->text().isEmpty())
			continue;
		opt->textTriggers.append(textTriggersList->item(i)->text());
	}
}

void OptionsDlg::syncOptions()
{
	//strings
	pathToPuttyText->setText(opt->pathToPutty);

	//boolean
	catalogSessionsCheck->setCheckState(opt->catalogSessions?(Qt::Checked):(Qt::Unchecked));
	passArgsCheck->setCheckState(opt->passArgs?(Qt::Checked):(Qt::Unchecked));
	keywordSearchCheck->setCheckState(opt->keywordSearch?(Qt::Checked):(Qt::Unchecked));
	useRegexCheck->setCheckState(opt->useRegex?(Qt::Checked):(Qt::Unchecked));

	//arrays
	textTriggersList->clear();
	foreach (QString text, opt->textTriggers){
		QListWidgetItem * item = new QListWidgetItem(text);
		item->setFlags( item->flags() | Qt::ItemIsEditable );

		textTriggersList->addItem(item);
	}
}

void OptionsDlg::on_pathToPuttyBrowse_clicked() 
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Locate PuTTY Executable"), "",
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
