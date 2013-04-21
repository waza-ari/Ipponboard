﻿// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//
#include "fightermanagerdlg.h"
#include "ui_fightermanagerdlg.h"

#include "FighterManager.h"
#include "../core/fighter.h"
//#include "../util/path_helpers.h"
#include <QInputDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QRegExp>
#include <QPlainTextEdit>

#include <boost/foreach.hpp>

//using namespace Ipponboard;

//---------------------------------------------------------
FighterManagerDlg::FighterManagerDlg(
	Ipponboard::FighterManager& manager,
	QWidget* parent)
	: QDialog(parent)
	, ui(new Ui::FighterManagerDlg)
	, m_manager(manager)
	, m_filter()
	, m_formatStr(Ipponboard::FighterManager::DefaultExportFormat())
//---------------------------------------------------------
{
	ui->setupUi(this);

	// hide settings buton
	//TODO: improove!
	ui->pushButton_settings->hide();

	// set columns
	auto headerItem = ui->treeWidget_fighters->headerItem();
	headerItem->setText(eColumn_club, tr("Club/Team"));
	headerItem->setText(eColumn_category, tr("Category"));
	headerItem->setText(eColumn_weight, tr("Weight"));
	headerItem->setText(eColumn_firstName, tr("First Name"));
	headerItem->setText(eColumn_lastName, tr("Last Name"));

	// adjust column widths
	ui->treeWidget_fighters->setColumnWidth(eColumn_club, 150);
	ui->treeWidget_fighters->setColumnWidth(eColumn_category, 60);
	ui->treeWidget_fighters->setColumnWidth(eColumn_weight, 50);
	ui->treeWidget_fighters->setColumnWidth(eColumn_firstName, 100);
	ui->treeWidget_fighters->setColumnWidth(eColumn_lastName, 100);

	populate_view();
}

//---------------------------------------------------------
FighterManagerDlg::~FighterManagerDlg()
//---------------------------------------------------------
{
	delete ui;
}

//---------------------------------------------------------
void FighterManagerDlg::SetFilter(FighterManagerDlg::EColumn column, const QString& value)
//---------------------------------------------------------
{
	if (column >= 0 && column < eColumn_MAX)
	{
		m_filter = std::make_pair(column, value);
		ui->treeWidget_fighters->hideColumn(m_filter.first);

		populate_view();
	}
}

//---------------------------------------------------------
void FighterManagerDlg::changeEvent(QEvent* e)
//---------------------------------------------------------
{
	QDialog::changeEvent(e);

	switch (e->type())
	{
	case QEvent::LanguageChange:
		ui->retranslateUi(this);
		break;

	default:
		break;
	}
}

//---------------------------------------------------------
void FighterManagerDlg::on_pushButton_add_pressed()
//---------------------------------------------------------
{
	bool ok(false);

    const QChar splitter(';');
    QString dlgTitle = tr("Add new fighter");
    QString dlgMsg = tr("Use the following format to specify the new fighter.\n"\
                        "Use '%1' as separator. "\
                        "Categories should be one of these: M, F, FU21 etc.\n\nFormat: ").arg(splitter);

    bool hasFilter = false;
    if (m_filter.first == eColumn_club)
    {
        dlgMsg.append("first name;last name;weight;category");
        hasFilter = true;
    }
    else
    {
        dlgMsg.append("first name;last name;club;weight;category");
    }

	QString data = QInputDialog::getText(this,
										 dlgTitle,
										 dlgMsg,
										 QLineEdit::Normal,
										 QString(),
										 &ok);
    auto dataParts = data.split(splitter);

    while (ok && !(hasFilter ? dataParts.size() == 4 : dataParts.size() == 5))
	{
		QMessageBox::critical(this,
							  tr(""),
							  tr("Invalid format. Please correct your input."));

		data = QInputDialog::getText(this,
									 dlgTitle,
									 dlgMsg,
									 QLineEdit::Normal,
									 data,
									 &ok);
        dataParts = data.split(splitter);
	}

	if (ok)
	{
		Ipponboard::Fighter fighter(
			dataParts[0],  // first
			dataParts[1]);  // last
        fighter.club = hasFilter ? m_filter.second : dataParts[2];
        fighter.weight = dataParts[hasFilter? 2 : 3];
        fighter.category = dataParts[hasFilter? 3 : 4];

		m_manager.m_fighters.insert(fighter);

		QStringList contents;

		for (int i = 0; i < eColumn_MAX; ++i) contents.append("");

		contents[eColumn_club] = fighter.club;
		contents[eColumn_category] = fighter.category;
		contents[eColumn_weight] = fighter.weight;
		contents[eColumn_firstName] = fighter.first_name;
		contents[eColumn_lastName] = fighter.last_name;

		QTreeWidgetItem* pItem =
			new QTreeWidgetItem(contents, QTreeWidgetItem::UserType);
		pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
		ui->treeWidget_fighters->addTopLevelItem(pItem);
	}
}

//---------------------------------------------------------
void FighterManagerDlg::populate_view()
//---------------------------------------------------------
{
	ui->treeWidget_fighters->clear();

	const bool hasFilter = !m_filter.second.isEmpty();

	if (hasFilter)
	{
		const QString filterInfo = QString("%0: %1").arg(
									   ui->treeWidget_fighters->headerItem()->text(m_filter.first),
									   m_filter.second);

		ui->label_filterinfo->setText(filterInfo);
		ui->label_filterinfo->show();
	}
	else
	{
		ui->label_filterinfo->hide();
	}

	std::for_each(begin(m_manager.m_fighters), end(m_manager.m_fighters),
				  [&](Ipponboard::Fighter const & f)
	{
		bool skipItem = true;

		if (hasFilter)
		{
			switch (m_filter.first)
			{
			case eColumn_firstName:
				if (m_filter.second == f.first_name)
					skipItem = false;

				break;

			case eColumn_lastName:
				if (m_filter.second == f.last_name)
					skipItem = false;

				break;

			case eColumn_club:
				if (m_filter.second == f.club)
					skipItem = false;

				break;

			case eColumn_weight:
				if (m_filter.second == f.weight)
					skipItem = false;

				break;

			case eColumn_category:
				if (m_filter.second == f.category)
					skipItem = false;

				break;

			default:
				break;
			}
		}
		else
		{
			skipItem = false;
		}

		if (!skipItem)
		{
			QStringList contents;

			for (int i = 0; i < eColumn_MAX; ++i) contents.append("");

			contents[eColumn_club] = f.club;
			contents[eColumn_category] = f.category;
			contents[eColumn_weight] = f.weight;
			contents[eColumn_firstName] = f.first_name;
			contents[eColumn_lastName] = f.last_name;

			QTreeWidgetItem* pItem =
				new QTreeWidgetItem(contents, QTreeWidgetItem::UserType);
			pItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable);
			ui->treeWidget_fighters->addTopLevelItem(pItem);
		}
	});
}

//---------------------------------------------------------
void FighterManagerDlg::on_pushButton_import_pressed()
//---------------------------------------------------------
{
	const QString fileName = QFileDialog::getOpenFileName(this,
							 tr("Select CSV file with fighters"),
							 QCoreApplication::applicationDirPath(),
							 tr("CSV files (*.csv);;Text files (*.txt)"), nullptr, QFileDialog::ReadOnly);

	if (!fileName.isEmpty())
	{
		//TODO: make this right
		on_pushButton_settings_pressed();

		QString errorMsg;

		if (m_manager.ImportFighters(fileName, m_formatStr, errorMsg))
		{
			QMessageBox::information(
				this,
				QCoreApplication::applicationName(),
				errorMsg);

			//TODO?: update_fighter_name_completer(m_pUi->comboBox_weight->currentText());
		}
		else
		{
			QMessageBox::critical(
				this,
				QCoreApplication::applicationName(),
				errorMsg);
		}
	}

	populate_view();
}

//---------------------------------------------------------
void FighterManagerDlg::on_pushButton_export_pressed()
//---------------------------------------------------------
{
	const QString fileName = QFileDialog::getSaveFileName(this,
							 tr("Name CSV file to store fighters in"),
							 QCoreApplication::applicationDirPath(),
							 tr("CSV files (*.csv);;Text files (*.txt)"));

	if (!fileName.isEmpty())
	{
		//TODO: make this right
		on_pushButton_settings_pressed();

		QString errorMsg;

		if (m_manager.ExportFighters(fileName, m_formatStr, errorMsg))
		{
			QMessageBox::information(
				this,
				QCoreApplication::applicationName(),
				errorMsg);
		}
		else
		{
			QMessageBox::critical(
				this,
				QCoreApplication::applicationName(),
				errorMsg);
		}
	}
}

//---------------------------------------------------------
void FighterManagerDlg::on_pushButton_remove_pressed()
//---------------------------------------------------------
{
	auto selectedItems = ui->treeWidget_fighters->selectedItems();

	//QTreeWidgetItem* pItem = ui->treeWidget_fighters->currentItem();

	//if (pItem)
	BOOST_FOREACH(QTreeWidgetItem * pItem, selectedItems)
	{
		Ipponboard::Fighter currentFighter(
			pItem->text(eColumn_firstName),
			pItem->text(eColumn_lastName));
		currentFighter.club = pItem->text(eColumn_club);
		currentFighter.weight = pItem->text(eColumn_weight);
		currentFighter.category = pItem->text(eColumn_category);

		ui->treeWidget_fighters->takeTopLevelItem(
			ui->treeWidget_fighters->indexOfTopLevelItem(pItem));

		m_manager.RemoveFighter(currentFighter);

		delete pItem;
	}
}

//---------------------------------------------------------
void FighterManagerDlg::on_treeWidget_fighters_itemChanged(
	QTreeWidgetItem* pItem, int column)
//---------------------------------------------------------
{
	if (pItem)
	{
		QString firstName = pItem->text(eColumn_firstName);
		QString lastName = pItem->text(eColumn_lastName);
		QString club = pItem->text(eColumn_club);
		QString weight = pItem->text(eColumn_weight);
		QString category = pItem->text(eColumn_category);

		Ipponboard::Fighter changedFighter(firstName, lastName);
		changedFighter.club = club;
		changedFighter.weight = weight;
		changedFighter.category = category;

		qDebug("enum value: %i", column);

		switch (column)
		{
		case eColumn_firstName:
			firstName = m_tmpData;
			break;

		case eColumn_lastName:
			lastName = m_tmpData;
			break;

		case eColumn_club:
			club = m_tmpData;
			break;

		case eColumn_weight:
			weight = m_tmpData;
			break;

		case eColumn_category:
			category = m_tmpData;
			break;

		default:
			qDebug("ERROR: invalid enum value: %i", column);
			break;
		}

		Ipponboard::Fighter originalFighter(firstName, lastName);
		originalFighter.club = club;
		originalFighter.weight = weight;
		originalFighter.category = category;

		if (!m_manager.RemoveFighter(originalFighter))
		{
			qDebug("error: original fighter not found!");
		}

		if (!m_manager.AddFighter(changedFighter))
		{
			ui->treeWidget_fighters->takeTopLevelItem(
				ui->treeWidget_fighters->indexOfTopLevelItem(pItem));

			// due to duplicate entry
			qDebug("removed changed entry due to duplicate: %s %s",
				   changedFighter.first_name.toAscii().data(),
				   changedFighter.last_name.toAscii().data());
		}
	}
}

void FighterManagerDlg::on_treeWidget_fighters_itemClicked(QTreeWidgetItem* item, int column)
{
	m_tmpData = item->text(column);
	qDebug("data: %s", m_tmpData.toLatin1().data());
}

void FighterManagerDlg::on_pushButton_settings_pressed()
{
	bool ok(false);
	QString dlgTitle = tr("Specify import/export format");
	QString dlgMsg = tr("Use valid specifiers and some kind of separator (;,:|/ etc.)"
						"\nValid specifiers are: %1")
					 .arg(Ipponboard::FighterManager::GetSpecifiererDescription());

	QString data = QInputDialog::getText(this,
										 dlgTitle,
										 dlgMsg,
										 QLineEdit::Normal,
										 m_formatStr,
										 &ok);

	QString separator;
	bool isValidSeparator = Ipponboard::FighterManager::DetermineSeparator(data, separator);

	QStringList dataParts;

	if (isValidSeparator)
	{
		dataParts = data.split(separator);
	}

	// at least 3 parts must be set (first, last, ...)
	while (ok && (!isValidSeparator || dataParts.size() < 3))
	{
		QMessageBox::critical(this,
							  tr(""),
							  tr("Invalid format. Please correct your input."));

		data = QInputDialog::getText(this,
									 dlgTitle,
									 dlgMsg,
									 QLineEdit::Normal,
									 data,
									 &ok);
		isValidSeparator =
			Ipponboard::FighterManager::DetermineSeparator(data, separator);

		if (isValidSeparator)
		{
			dataParts = data.split(separator);
		}
	}

	if (ok && isValidSeparator)
	{
		m_formatStr = data;
	}
}