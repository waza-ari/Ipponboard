///////////////////////////////////////////////////////////////////////////////
// $Id$
///////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2010-2014 Florian Muecke - (ipponboardinfo at googlemail dot com)
// All rights reserved.  Web: http://www.ipponboard.info
//
// This file is part of the Ipponboard project.
//
// It may not be distributed to or shared with the public in any form!
///////////////////////////////////////////////////////////////////////////////


#include "ModeManagerDlg.h"
#include "ui_ModeManagerDlg.h"
#include "..\core\Enums.h"
#include "..\core\EnumStrings.h"

#include <QComboBox>
#include <QStringList>
#include <QMessageBox>
#include <QUuid>

using namespace Ipponboard;

namespace { bool initialized = false; }

ModeManagerDlg::ModeManagerDlg(TournamentMode::List const& modes,
        QStringList const& templates,
        QString const& currentModeId,
        QWidget *parent)
    : QDialog(parent)
    , FMU::DialogResult<TournamentMode::List>(modes)
    , m_pUi(new Ui::ModeManagerDlg)
{
    std::sort(begin(m_dialogData), end(m_dialogData));

    m_pUi->setupUi(this);
    m_pUi->comboBox_template->addItems(templates);

    if (!m_dialogData.empty())
    {
        int pos = 0;
        for (size_t i = 0; i < m_dialogData.size(); ++i)
        {
            TournamentMode const& mode = m_dialogData[i];
            m_pUi->comboBox_mode->addItem(mode.Description(), QVariant(mode.id));

            if (mode.id == currentModeId)
            {
                pos = i;
            }
        }

        initialized = true;

        on_comboBox_mode_currentIndexChanged(pos);
    }
}

ModeManagerDlg::~ModeManagerDlg()
{
}

void ModeManagerDlg::on_comboBox_mode_currentIndexChanged(int i)
{
    if (!initialized)
    {
        return;
    }

    auto const& mode = GetMode(i);
    m_pUi->lineEdit_title->setText(mode.title);
    m_pUi->lineEdit_subtitle->setText(mode.subTitle);
    m_pUi->lineEdit_weights->setText(mode.weights);
    m_pUi->spinBox_rounds->setValue(mode.nRounds);
    m_pUi->spinBox_fightTimeMinutes->setValue(mode.fightTimeInSeconds / 60);
    m_pUi->spinBox_fightTimeSeconds->setValue(mode.fightTimeInSeconds % 60);
    m_pUi->checkBox_doubleWeights->setChecked(mode.weightsAreDoubled);
    update_fights_per_round(mode);

    auto index = m_pUi->comboBox_template->findText(mode.listTemplate);
    if (index != -1)
    {
        m_pUi->comboBox_template->setCurrentIndex(index);
    }

    if (!mode.fightTimeOverrides.empty())
    {
        m_pUi->checkBox_timeOverrides->setChecked(true);
        m_pUi->lineEdit_timeOverrides->setText(mode.GetFightTimeOverridesString());
    }
    else
    {
        m_pUi->checkBox_timeOverrides->setChecked(false);
        m_pUi->lineEdit_timeOverrides->setText(QString());
    }

    m_pUi->checkBox_2013Rules->setChecked(mode.IsOptionSet(eOption_Use2013Rules));
    m_pUi->checkBox_autoIncrement->setChecked(mode.IsOptionSet(eOption_AutoIncrementPoints));
    m_pUi->checkBox_allSubscoresCount->setChecked(mode.IsOptionSet(eOption_AllSubscoresCount));
}

void ModeManagerDlg::on_comboBox_template_currentIndexChanged(const QString &s)
{
    auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.listTemplate = s;
}

void ModeManagerDlg::on_checkBox_timeOverrides_toggled(bool checked)
{
    m_pUi->lineEdit_timeOverrides->setEnabled(checked);
}

void ModeManagerDlg::on_checkBox_doubleWeights_toggled(bool checked)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.weightsAreDoubled = checked;
    update_fights_per_round(mode);
}

void ModeManagerDlg::on_checkBox_2013Rules_toggled(bool checked)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.SetOption(eOption_Use2013Rules, checked);
}

void ModeManagerDlg::on_checkBox_autoIncrement_toggled(bool checked)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.SetOption(eOption_AutoIncrementPoints, checked);
}

void ModeManagerDlg::on_checkBox_allSubscoresCount_toggled(bool checked)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.SetOption(eOption_AllSubscoresCount, checked);
}

void ModeManagerDlg::on_toolButton_add_clicked()
{
    TournamentMode mode;
    mode.id = QUuid::createUuid().toString();
    mode.id = mode.id.mid(1, mode.id.length()-2);  // remove "{}"
    mode.title = tr("*new*");
    mode.weights = "-66;-73;-81;-90;+90";
    mode.fightTimeInSeconds = 300;
    mode.nRounds = 2;
    mode.weightsAreDoubled = true;
    mode.listTemplate = m_pUi->comboBox_template->itemText(0);
    mode.SetOption(eOption_Use2013Rules, true);

    m_dialogData.push_back(mode);

	m_pUi->comboBox_mode->addItem(mode.Description(), QVariant(mode.id));
    auto pos = m_pUi->comboBox_mode->findData(QVariant(mode.id));
    m_pUi->comboBox_mode->setCurrentIndex(pos);

    m_pUi->toolButton_remove->setEnabled(m_pUi->comboBox_mode->count() > 1);
}

void ModeManagerDlg::on_toolButton_remove_clicked()
{
    auto answer = QMessageBox::question(
                this,
                tr("Remove item"),
                tr("Really remove \"%1\"?").arg(m_pUi->comboBox_mode->currentText()),
                tr("Remove"),
                tr("Keep"));

    if (answer == 0)
    {
        auto id = m_pUi->comboBox_mode->itemData(m_pUi->comboBox_mode->currentIndex()).toString();
        auto pos = std::find_if(begin(m_dialogData), end(m_dialogData), [&](TournamentMode const& mode)
        { return mode.id == id; });

        if (pos != end(m_dialogData))
        {
            m_dialogData.erase(pos);
			m_pUi->comboBox_mode->removeItem(m_pUi->comboBox_mode->currentIndex());
        }

        m_pUi->toolButton_remove->setEnabled(m_pUi->comboBox_mode->count() > 1);
    }
}

void ModeManagerDlg::on_spinBox_rounds_valueChanged(int i)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.nRounds = i;
    update_fights_per_round(mode);
}

void ModeManagerDlg::on_spinBox_fightTimeMinutes_valueChanged(int i)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.fightTimeInSeconds = i * 60 + m_pUi->spinBox_fightTimeSeconds->value();
}

void ModeManagerDlg::on_spinBox_fightTimeSeconds_valueChanged(int i)
{
    auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.fightTimeInSeconds = m_pUi->spinBox_fightTimeMinutes->value() * 60 + i;
}

void ModeManagerDlg::on_lineEdit_weights_textChanged(const QString &s)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.weights = s;
    update_fights_per_round(mode);
}

void ModeManagerDlg::on_lineEdit_title_textChanged(const QString &s)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
     mode.title = s;
	 m_pUi->comboBox_mode->setItemText(m_pUi->comboBox_mode->currentIndex(), mode.Description());
}

void ModeManagerDlg::on_lineEdit_subtitle_textChanged(const QString &s)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    mode.subTitle = s;
	m_pUi->comboBox_mode->setItemText(m_pUi->comboBox_mode->currentIndex(), mode.Description());
}

void ModeManagerDlg::on_lineEdit_timeOverrides_textChanged(const QString &s)
{
	auto& mode = GetMode(m_pUi->comboBox_mode->currentIndex());
    if (s.isEmpty() || TournamentMode::ExtractFightTimeOverrides(s, mode.fightTimeOverrides))
    {
        m_pUi->lineEdit_timeOverrides->setStyleSheet("color : black;");
    }
    else
    {
        m_pUi->lineEdit_timeOverrides->setStyleSheet("color : red;");
    }
}

void ModeManagerDlg::update_fights_per_round(const TournamentMode& mode)
{
    m_pUi->label_fightsPerRound->setText(
                QString("%1 fights, %2 per round")
                .arg(mode.FightsPerRound()*mode.nRounds)
                .arg(mode.FightsPerRound()));
}

Ipponboard::TournamentMode& ModeManagerDlg::GetMode(int i)
{
	QString id = m_pUi->comboBox_mode->itemData(i).toString();

	for (auto & mode : m_dialogData)
    {
        if (mode.id == id)
        {
            return mode;
        }
    }

    return std::move(TournamentMode());
}