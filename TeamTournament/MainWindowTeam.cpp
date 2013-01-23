#include "MainWindowTeam.h"
#include "ui_mainwindow.h" //TODO: may be obsolete

#include "scorescreen.h"
#include "../base/clubmanager.h"
#include "../base/clubmanagerdlg.h"
//#include "../base/fightcategorymanager.h"
//#include "../base/fightcategorymanagerdlg.h"
#include "../base/view.h"
#include "../base/versioninfo.h"
#include "../core/controller.h"
#include "../core/ControllerConfig.h"
#include "../core/tournament.h"
#include "../core/tournamentmodel.h"
#include "../gamepad/gamepad.h"
#include "../util/path_helpers.h"
#include "../widgets/scaledimage.h"
#include "../widgets/scaledtext.h"

#include <QColorDialog>
#include <QComboBox>
#include <QCompleter>
#include <QDesktopServices>
#include <QDesktopWidget>
#include <QDir>
#include <QFileDialog>
#include <QFontDialog>
#include <QInputDialog>
#include <QMenu>
#include <QMessageBox>
#include <QSettings>
#include <QSplashScreen>
#include <QTimer>
#include <QUrl>
#include <functional>

namespace StrTags
{
	static const char* const mode = "Mode";
	static const char* const host = "Host";
}

static const char* const str_mode_1te_bundesliga_nord_m = "1. Bundesliga Nord (M�nner)";
static const char* const str_mode_1te_bundesliga_sued_m = "1. Bundesliga S�d (M�nner)";
static const char* const str_mode_2te_bundesliga_nord_m = "2. Bundesliga Nord (M�nner)";
static const char* const str_mode_2te_bundesliga_sued_m = "2. Bundesliga S�d (M�nner)";
static const char* const str_mode_1te_bundesliga_nord_f = "1. Bundesliga Nord (Frauen)";
static const char* const str_mode_1te_bundesliga_sued_f = "1. Bundesliga S�d (Frauen)";
static const char* const str_mode_2te_bundesliga_nord_f = "2. Bundesliga Nord (Frauen)";
static const char* const str_mode_2te_bundesliga_sued_f = "2. Bundesliga S�d (Frauen)";
static const char* const str_mode_bayernliga_nord_m = "Bayernliga Nord (M)";
static const char* const str_mode_bayernliga_sued_m = "Bayernliga S�d (M)";
static const char* const str_mode_bayernliga_nord_f = "Bayernliga Nord (F)";
static const char* const str_mode_bayernliga_sued_f = "Bayernliga S�d (F)";
static const char* const str_mode_landesliga_nord_m = "Landesliga Nord (M)";
static const char* const str_mode_landesliga_sued_m = "Landesliga S�d (M)";
static const char* const str_mode_landesliga_nord_f = "Landesliga Nord (F)";
static const char* const str_mode_landesliga_sued_f = "Landesliga S�d (F)";
static const char* const str_mode_mm_u17_m = "Deutsche VMM MU17";
static const char* const str_mode_mm_u17_f = "Deutsche VMM FU17";


using namespace FMlib;
using namespace Ipponboard;


MainWindowTeam::MainWindowTeam(QWidget* parent)
    : MainWindowBase(parent)
    , m_pScoreScreen()
    , m_pClubManager()
    , m_htmlScore()
    , m_mode()
    , m_host()
{
	m_pUi->setupUi(this);
}

void MainWindowTeam::Init()
{
    m_pClubManager.reset(new Ipponboard::ClubManager());
    m_pScoreScreen.reset(new Ipponboard::ScoreScreen());

	MainWindowBase::Init();

    // set default background
    m_pScoreScreen->setStyleSheet(m_pUi->frame_primary_view->styleSheet());

    //
    // setup data
    //
    m_pUi->dateEdit->setDate(QDate::currentDate());
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/djb-logo.png"), str_mode_1te_bundesliga_nord_m);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/djb-logo.png"), str_mode_1te_bundesliga_sued_m);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/djb-logo.png"), str_mode_2te_bundesliga_nord_m);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/djb-logo.png"), str_mode_2te_bundesliga_sued_m);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/bjv-logo.png"), str_mode_bayernliga_nord_m);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/bjv-logo.png"), str_mode_bayernliga_sued_m);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/bjv-logo.png"), str_mode_landesliga_nord_m);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/bjv-logo.png"), str_mode_landesliga_sued_m);
    m_pUi->comboBox_mode->addItem(str_mode_mm_u17_m);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/djb-logo.png"), str_mode_1te_bundesliga_nord_f);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/djb-logo.png"), str_mode_1te_bundesliga_sued_f);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/djb-logo.png"), str_mode_2te_bundesliga_nord_f);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/djb-logo.png"), str_mode_2te_bundesliga_sued_f);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/bjv-logo.png"), str_mode_bayernliga_nord_f);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/bjv-logo.png"), str_mode_bayernliga_sued_f);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/bjv-logo.png"), str_mode_landesliga_nord_f);
    m_pUi->comboBox_mode->addItem(QIcon(":leagues/emblems/bjv-logo.png"), str_mode_landesliga_sued_f);
    m_pUi->comboBox_mode->addItem(str_mode_mm_u17_f);

    m_pUi->tableView_tournament_list1->setModel(m_pController->GetTournamentScoreModel(0));
    m_pUi->tableView_tournament_list2->setModel(m_pController->GetTournamentScoreModel(1));
    m_pUi->tableView_tournament_list1->resizeColumnsToContents();
    m_pUi->tableView_tournament_list2->resizeColumnsToContents();
    m_pController->GetTournamentScoreModel(0)->SetExternalDisplays(
        m_pUi->lineEdit_wins_intermediate,
        m_pUi->lineEdit_score_intermediate);
    m_pController->GetTournamentScoreModel(1)->SetExternalDisplays(
        m_pUi->lineEdit_wins,
        m_pUi->lineEdit_score);
    m_pController->GetTournamentScoreModel(1)->SetIntermediateModel(
        m_pController->GetTournamentScoreModel(0));

    update_club_views();

    //m_pUi->comboBox_club_guest->setCurrentIndex(0);

    m_pUi->tableView_tournament_list1->selectRow(0);
    m_pUi->tableView_tournament_list2->selectRow(0);

    int modeIndex = m_pUi->comboBox_mode->findText(m_mode);
    if (-1 == modeIndex)
        modeIndex =  0;
    m_pUi->comboBox_mode->setCurrentIndex(modeIndex);

    // TEMP: hide weight cotrol
//	m_pUi->label_weight->hide();
//	m_pUi->lineEdit_weights->hide();
//	m_pUi->toolButton_weights->hide();
//	m_pUi->gridLayout_main->removeItem(m_pUi->horizontalSpacer_4);
//	delete m_pUi->horizontalSpacer_4;

    //update_weights("-66;-73;-81;-90;+90");
    //FIXME: check why this has not been in branch

    UpdateFightNumber_();

    //m_pUi->button_pause->click();	// we start with pause!
}

void MainWindowTeam::closeEvent(QCloseEvent* event)
{
	MainWindowBase::closeEvent(event);

    if (m_pScoreScreen)
    {
        m_pScoreScreen->close();
    }
}

void MainWindowTeam::keyPressEvent(QKeyEvent* event)
{
    const bool isCtrlPressed = event->modifiers().testFlag(Qt::ControlModifier);
    const bool isAltPressed = event->modifiers().testFlag(Qt::AltModifier);

    //FIXME: copy and paste handling should be part of the table class!
    if (m_pUi->tabWidget->currentWidget() == m_pUi->tab_view)
    {
        switch (event->key())
        {
        case Qt::Key_Left:
            if (isCtrlPressed && isAltPressed)
            {
                m_pUi->button_prev->click();
                qDebug() << "Button [ Prev ] was triggered by keyboard";
            }
            else
            {
                MainWindowBase::keyPressEvent(event);
            }
            break;

        case Qt::Key_Right:
            if (isCtrlPressed && isAltPressed)
            {
                m_pUi->button_next->click();
                qDebug() << "Button [ Next ] was triggered by keyboard";
            }
            else
            {
                MainWindowBase::keyPressEvent(event);
            }
            break;

        case Qt::Key_F4:
            m_pUi->button_pause->click();
            qDebug() << "Button [ ResultScreen ] was triggered by keyboard";
            break;

        default:
            MainWindowBase::keyPressEvent(event);
            break;
        }
    }
    else if (m_pUi->tabWidget->currentWidget() == m_pUi->tab_score_table)
    {
        if (event->matches(QKeySequence::Copy))
        {
            if (QApplication::focusWidget() == m_pUi->tableView_tournament_list1)
            {
                slot_copy_cell_content_list1();
            }
            else if (QApplication::focusWidget() == m_pUi->tableView_tournament_list2)
            {
                slot_copy_cell_content_list2();
            }
        }
        else if (event->matches(QKeySequence::Paste))
        {
            if (QApplication::focusWidget() == m_pUi->tableView_tournament_list1)
            {
                slot_paste_cell_content_list1();
            }
            else if (QApplication::focusWidget() == m_pUi->tableView_tournament_list2)
            {
                slot_paste_cell_content_list2();
            }
        }
        else if (event->matches(QKeySequence::Delete))
        {
            if (QApplication::focusWidget() == m_pUi->tableView_tournament_list1)
            {
                slot_clear_cell_content_list1();
            }
            else if (QApplication::focusWidget() == m_pUi->tableView_tournament_list2)
            {
                slot_clear_cell_content_list2();
            }
        }
        else
        {
            MainWindowBase::keyPressEvent(event);
        }
    }
    else
    {
        //TODO: handle view keys
        //FIXME: handling should be part of the view class!
        //switch (event->key())
        //{
        //default:
            MainWindowBase::keyPressEvent(event);
        //    break;
        //}
    }
}

void MainWindowTeam::write_specific_settings(QSettings &settings)
{
    settings.beginGroup(EditionName());
    settings.setValue(StrTags::mode, m_mode);
    settings.setValue(StrTags::host, m_host);
    settings.setValue(str_tag_LabelHome, m_pController->GetHomeLabel());
    settings.setValue(str_tag_LabelGuest, m_pController->GetGuestLabel());
    settings.endGroup();
}

void MainWindowTeam::read_specific_settings(QSettings &settings)
{
    settings.beginGroup(EditionName());
    {
        m_mode = settings.value(StrTags::mode, "").toString();
        m_host = settings.value(StrTags::host, "").toString();

        m_pController->SetLabels(
            settings.value(str_tag_LabelHome, tr("Home")).toString(),
            settings.value(str_tag_LabelGuest, tr("Guest")).toString());
    }
    settings.endGroup();

    settings.beginGroup(str_tag_Styles);
    if (settings.contains(str_tag_BgStyle))
    {
        const QString styleSheet = settings.value(str_tag_BgStyle).toString();
        m_pScoreScreen->setStyleSheet(styleSheet);
    }
    settings.endGroup();
}

void MainWindowTeam::update_info_text_color(const QColor& color, const QColor& bgColor)
{
    MainWindowBase::update_info_text_color(color, bgColor);
    //m_pScoreScreen->SetInfoTextColor(color, bgColor);
}

void MainWindowTeam::update_text_color_first(const QColor& color, const QColor& bgColor)
{
    MainWindowBase::update_text_color_first(color, bgColor);
    m_pScoreScreen->SetTextColorFirst(color, bgColor);
}

void MainWindowTeam::update_text_color_second(const QColor& color, const QColor& bgColor)
{
    MainWindowBase::update_text_color_second(color, bgColor);
    m_pScoreScreen->SetTextColorSecond(color, bgColor);
}

void MainWindowTeam::update_fighter_name_font(const QFont& font)
{
    MainWindowBase::update_fighter_name_font(font);
    m_pScoreScreen->SetTextFont(font);
}

void MainWindowTeam::update_views()
{
    MainWindowBase::update_views();
    update_score_screen();
}

void MainWindowTeam::update_club_views()
{
    QString oldHost = m_host;

    m_pUi->comboBox_club_host->clear();
    m_pUi->comboBox_club_home->clear();
    m_pUi->comboBox_club_guest->clear();

    for (int i = 0; i < m_pClubManager->ClubCount(); ++i)
    {
        Ipponboard::Club club;
        m_pClubManager->GetClub(i, club);
        QIcon icon(club.logoFile);
        m_pUi->comboBox_club_host->addItem(icon, club.name);
        m_pUi->comboBox_club_home->addItem(icon, club.name);
        m_pUi->comboBox_club_guest->addItem(icon, club.name);
    }

    m_host = oldHost;

    int index = m_pUi->comboBox_club_host->findText(m_host);
    if (-1 == index)
        index =  0;

    m_pUi->comboBox_club_host->setCurrentIndex(index);
    m_pUi->comboBox_club_home->setCurrentIndex(index);

    // set location from host
    m_pUi->lineEdit_location->setText(m_pClubManager->GetAddress(m_host));
}

void MainWindowTeam::UpdateFightNumber_()
{
    const int currentFight = m_pController->GetCurrentFightIndex() + 1;

    m_pUi->label_fight->setText(
        QString("%1 / %2")
        .arg(QString::number(currentFight))
        .arg(QString::number(m_pController->GetFightCount())));
}

void MainWindowTeam::update_score_screen()
{
    const QString home = m_pUi->comboBox_club_home->currentText();
    const QString guest = m_pUi->comboBox_club_guest->currentText();
    m_pScoreScreen->SetClubs(home, guest);
    const QString logo_home = m_pClubManager->GetLogo(home);
    const QString logo_guest = m_pClubManager->GetLogo(guest);
    m_pScoreScreen->SetLogos(logo_home, logo_guest);
    const int score_first = m_pController->GetTeamScore(Ipponboard::eFighter1);
    const int score_second = m_pController->GetTeamScore(Ipponboard::eFighter2);
    m_pScoreScreen->SetScore(score_first, score_second);

    m_pScoreScreen->update();
}

void MainWindowTeam::WriteScoreToHtml_()
{
    QString modeText = get_full_mode_title(m_pUi->comboBox_mode->currentText());
    QString templateFile = get_template_file(m_pUi->comboBox_mode->currentText());
    const QString filePath(
                fmu::GetSettingsFilePath(templateFile.toStdString().c_str()).c_str());

    QFile file(filePath);

    if (!file.open(QFile::ReadOnly))
    {
        QMessageBox::critical(this, tr("File open error"),
                              tr("File could not be opened: ") + file.fileName());
        return;
    }

    QTextStream ts(&file);

    m_htmlScore = ts.readAll();
    file.close();

    m_htmlScore.replace("%TITLE%", modeText);

    m_htmlScore.replace("%HOST%", m_pUi->comboBox_club_host->currentText());
    m_htmlScore.replace("%DATE%", m_pUi->dateEdit->text());
    m_htmlScore.replace("%LOCATION%", m_pUi->lineEdit_location->text());
    m_htmlScore.replace("%HOME%", m_pUi->comboBox_club_home->currentText());
    m_htmlScore.replace("%GUEST%", m_pUi->comboBox_club_guest->currentText());

    // intermediate score
    const std::pair<unsigned,unsigned> wins1st =
        m_pController->GetTournamentScoreModel(0)->GetTotalWins();
    m_htmlScore.replace("%WINS_HOME%", QString::number(wins1st.first));
    m_htmlScore.replace("%WINS_GUEST%", QString::number(wins1st.second));
    const std::pair<unsigned,unsigned> score1st =
        m_pController->GetTournamentScoreModel(0)->GetTotalScore();
    m_htmlScore.replace("%SCORE_HOME%", QString::number(score1st.first));
    m_htmlScore.replace("%SCORE_GUEST%", QString::number(score1st.second));

    // final score
    const std::pair<unsigned,unsigned> wins2nd =
        m_pController->GetTournamentScoreModel(1)->GetTotalWins();
    const std::pair<unsigned,unsigned> totalWins =
            std::make_pair(wins1st.first + wins2nd.first,
                           wins1st.second + wins2nd.second);
    m_htmlScore.replace("%TOTAL_WINS_HOME%", QString::number(totalWins.first));
    m_htmlScore.replace("%TOTAL_WINS_GUEST%", QString::number(totalWins.second));
    const std::pair<unsigned,unsigned> score2nd =
        m_pController->GetTournamentScoreModel(1)->GetTotalScore();
    const std::pair<unsigned,unsigned> totalScore =
            std::make_pair(score1st.first + score2nd.first,
                           score1st.second + score2nd.second);
    m_htmlScore.replace("%TOTAL_SCORE_HOME%", QString::number(totalScore.first));
    m_htmlScore.replace("%TOTAL_SCORE_GUEST%", QString::number(totalScore.second));


    QString winner = tr("tie");
    if( totalWins.first > totalWins.second )
        winner = m_pUi->comboBox_club_home->currentText();
    else if( totalWins.first < totalWins.second )
        winner = m_pUi->comboBox_club_guest->currentText();

    m_htmlScore.replace( "%WINNER%", winner );

    // first round
    QString rounds;

    for (int i(0); i < m_pController->GetFightCount(); ++i)
    {
        const Fight& fight(m_pController->GetFight(0, i));

        QString name_first(fight.fighters[eFighter1].name);
        QString name_second(fight.fighters[eFighter2].name);
        const Score& score_first(fight.scores[eFighter1]);
        const Score& score_second(fight.scores[eFighter2]);

        QString round("<tr>");
        round.append("<td><center>" + QString::number(i + 1) + "</center></td>"); // number
        round.append("<td><center>" + fight.weight + "</center></td>"); // weight
        round.append("<td><center>" + name_first + "</center></td>"); // name
        round.append("<td><center>" + QString::number(score_first.Ippon()) + "</center></td>"); // I
        round.append("<td><center>" + QString::number(score_first.Wazaari()) + "</center></td>"); // W
        round.append("<td><center>" + QString::number(score_first.Yuko()) + "</center></td>"); // Y
        round.append("<td><center>" + QString::number(score_first.Shido()) + "</center></td>"); // S
        round.append("<td><center>" + QString::number(score_first.Hansokumake()) + "</center></td>"); // H
        round.append("<td><center>" + QString::number(fight.HasWon(eFighter1)) + "</center></td>"); // won
        round.append("<td><center>" + QString::number(fight.ScorePoints(eFighter1)) + "</center></td>"); // score
        round.append("<td><center>" + name_second + "</center></td>"); // name
        round.append("<td><center>" + QString::number(score_second.Ippon()) + "</center></td>"); // I
        round.append("<td><center>" + QString::number(score_second.Wazaari()) + "</center></td>"); // W
        round.append("<td><center>" + QString::number(score_second.Yuko()) + "</center></td>"); // Y
        round.append("<td><center>" + QString::number(score_second.Shido()) + "</center></td>"); // S
        round.append("<td><center>" + QString::number(score_second.Hansokumake()) + "</center></td>"); // H
        round.append("<td><center>" + QString::number(fight.HasWon(eFighter2)) + "</center></td>"); // won
        round.append("<td><center>" + QString::number(fight.ScorePoints(eFighter2)) + "</center></td>"); // score
        round.append("<td><center>" + fight.GetRoundTimeUsedText(
                m_pController->GetRoundTimeSecs()) + "</center></td>"); // time
        round.append("<td><center>" + fight.GetRoundTimeRemainingText() + "</center></td>"); // time
        round.append("</tr>\n");
        rounds.append(round);
    }

    m_htmlScore.replace("%FIRST_ROUND%", rounds);

    // second round
    rounds.clear();

    for (int i(0); i < m_pController->GetFightCount(); ++i)
    {
        const Fight& fight(m_pController->GetFight(1, i));

        QString name_first(fight.fighters[eFighter1].name);
        QString name_second(fight.fighters[eFighter2].name);
        const Score& score_first(fight.scores[eFighter1]);
        const Score& score_second(fight.scores[eFighter2]);

        QString round("<tr>");
        round.append("<td><center>" + QString::number(i + 1 + m_pController->GetFightCount()) + "</center></td>"); // number
        round.append("<td><center>" + fight.weight + "</center></td>"); // weight
        round.append("<td><center>" + name_first + "</center></td>"); // name
        round.append("<td><center>" + QString::number(score_first.Ippon()) + "</center></td>"); // I
        round.append("<td><center>" + QString::number(score_first.Wazaari()) + "</center></td>"); // W
        round.append("<td><center>" + QString::number(score_first.Yuko()) + "</center></td>"); // Y
        round.append("<td><center>" + QString::number(score_first.Shido()) + "</center></td>"); // S
        round.append("<td><center>" + QString::number(score_first.Hansokumake()) + "</center></td>"); // H
        round.append("<td><center>" + QString::number(fight.HasWon(eFighter1)) + "</center></td>"); // won
        round.append("<td><center>" + QString::number(fight.ScorePoints(eFighter1)) + "</center></td>"); // score
        round.append("<td><center>" + name_second + "</center></td>"); // name
        round.append("<td><center>" + QString::number(score_second.Ippon()) + "</center></td>"); // I
        round.append("<td><center>" + QString::number(score_second.Wazaari()) + "</center></td>"); // W
        round.append("<td><center>" + QString::number(score_second.Yuko()) + "</center></td>"); // Y
        round.append("<td><center>" + QString::number(score_second.Shido()) + "</center></td>"); // S
        round.append("<td><center>" + QString::number(score_second.Hansokumake()) + "</center></td>"); // H
        round.append("<td><center>" + QString::number(fight.HasWon(eFighter2)) + "</center></td>"); // won
        round.append("<td><center>" + QString::number(fight.ScorePoints(eFighter2)) + "</center></td>"); // score
        round.append("<td><center>" + fight.GetRoundTimeUsedText(
                m_pController->GetRoundTimeSecs()) + "</center></td>"); // time
        round.append("<td><center>" + fight.GetRoundTimeRemainingText() + "</center></td>"); // time
        round.append("</tr>\n");
        rounds.append(round);
    }

    m_htmlScore.replace("%SECOND_ROUND%", rounds);

    const QString copyright = tr("List generated with Ipponboard v") +
                              QApplication::applicationVersion() +
                              ", &copy; " + QApplication::organizationName() + ", 2010-2013";
    m_htmlScore.replace("</body>", "<small><center>" + copyright + "</center></small></body>");
}

void MainWindowTeam::on_actionReset_Scores_triggered()
{
    if (QMessageBox::warning(
                this,
                tr("Reset Scores"),
                tr("Really reset complete score table?"),
                QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        m_pController->ClearFights();

    UpdateFightNumber_();
}

bool MainWindowTeam::EvaluateSpecificInput(const Gamepad* pGamepad)
{
    // back
    if (pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_prev)))
    {
        on_button_prev_clicked();
        // TODO: check: is UpdateViews_(); necessary here?
        // --> handle update views outside of this function
        return true;
    }
    // next
    else if (pGamepad->WasPressed(Gamepad::EButton(m_controllerCfg.button_next)))
    {
        on_button_next_clicked();
        // TODO: check: is UpdateViews_(); necessary here?
        // --> handle update views outside of this function
        return true;
    }

    return false;
}

void MainWindowTeam::on_tabWidget_currentChanged(int /*index*/)
{
    update_views();
}

void MainWindowTeam::on_actionManage_Clubs_triggered()
{
    ClubManagerDlg dlg(m_pClubManager, this);
    dlg.exec();
}

void MainWindowTeam::on_actionLoad_Demo_Data_triggered()
{
    m_pController->ClearFights();																				//  Y  W  I  S  H  Y  W  I  S  H
    update_weights("-90;+90;-73;-66;-81");
    m_pController->SetFight(0, 0, "-90", "Sebastian H�lzl", "TG Landshut", "Oliver Sach", "TSV K�nigsbrunn",			3, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    m_pController->SetFight(0, 1, "-90", "Stefan Gr�nert", "TG Landshut", "Marc Sch�fer", "TSV K�nigsbrunn",			3, 2, 0, 0, 0, 0, 0, 0, 1, 0);
    m_pController->SetFight(0, 2, "+90", "Andreas Neumaier", "TG Landshut", "Daniel Nussb�cher", "TSV K�nigsbrunn",	0, 0, 0, 1, 0, 0, 0, 1, 1, 0);
    m_pController->SetFight(0, 3, "+90", "J�rgen Neumeier", "TG Landshut", "Anderas Mayer", "TSV K�nigsbrunn",			1, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    m_pController->SetFight(0, 4, "-73", "Benny Mahl", "TG Landshut", "Christopher Benka", "TSV K�nigsbrunn"	,		2, 0, 1, 1, 0, 0, 0, 0, 3, 0);
    m_pController->SetFight(0, 5, "-73", "Josef Sellmaier", "TG Landshut", "Jan-Michael K�nig", "TSV K�nigsbrunn",		0, 1, 1, 0, 0, 0, 0, 0, 0, 0);
    m_pController->SetFight(0, 6, "-66", "Alexander Keil", "TG Landshut", "Arthur Sipple", "TSV K�nigsbrunn",			2, 1, 1, 0, 0, 0, 0, 0, 0, 0);
    m_pController->SetFight(0, 7, "-66", "Dominic Bogner", "TG Landshut", "Thomas Schaller", "TSV K�nigsbrunn",		0, 0, 1, 0, 0, 2, 0, 0, 0, 0);
    m_pController->SetFight(0, 8, "-81", "Sebastian Schmieder", "TG Landshut", "Gerhard Wessinger", "TSV K�nigsbrunn",	0, 1, 1, 1, 0, 1, 0, 0, 0, 0);
    m_pController->SetFight(0, 9, "-81", "Rainer Neumaier", "TG Landshut", "Georg Benka", "TSV K�nigsbrunn",			1, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    //  Y  W  I  S  H  Y  W  I  S  H
    m_pController->SetFight(1, 0, "-90", "Sebastian H�lzl", "TG Landshut", "Marc Sch�fer", "TSV K�nigsbrunn",		0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    m_pController->SetFight(1, 1, "-90", "Stefan Grunert", "TG Landshut", "Florian K�rten", "TSV K�nigsbrunn",		0, 1, 1, 0, 0, 0, 0, 0, 0, 0);
    m_pController->SetFight(1, 2, "+90", "Andreas Neumaier", "TG Landshut", "Andreas Mayer", "TSV K�nigsbrunn",	1, 2, 0, 0, 0, 0, 0, 0, 0, 0);
    m_pController->SetFight(1, 3, "+90", "J�rgen Neumaier", "TG Landshut", "Daniel Nussb�cher", "TSV K�nigsbrunn",	0, 0, 0, 2, 0, 0, 0, 1, 2, 0);
    m_pController->SetFight(1, 4, "-73", "Matthias Feigl", "TG Landshut", "Jan-Michael K�nig", "TSV K�nigsbrunn",	2, 1, 0, 1, 0, 0, 0, 0, 1, 0);
    m_pController->SetFight(1, 5, "-73", "Josef Sellmaier", "TG Landshut", "Christopher Benka", "TSV K�nigsbrunn",	0, 0, 1, 0, 0, 0, 0, 0, 0, 0);
    m_pController->SetFight(1, 6, "-66", "J�rg Herzog", "TG Landshut", "Thomas Schaller", "TSV K�nigsbrunn",		0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
    m_pController->SetFight(1, 7, "-66", "Alex Selwitschka", "TG Landshut", "Jonas Allinger", "TSV K�nigsbrunn",	0, 1, 1, 0, 0, 1, 0, 0, 0, 0);
    m_pController->SetFight(1, 8, "-81", "Eugen Makaritsch", "TG Landshut", "Georg Benka", "TSV K�nigsbrunn",		0, 0, 0, 0, 0, 0, 0, 1, 0, 0);
    m_pController->SetFight(1, 9, "-81", "Rainer Neumaier", "TG Landshut", "Gerhard Wessinger", "TSV K�nigsbrunn",	0, 0, 1, 1, 0, 0, 0, 0, 0, 0);
    //m_pController->SetCurrentFight(0);

    m_pUi->tableView_tournament_list1->viewport()->update();
    m_pUi->tableView_tournament_list2->viewport()->update();
}

void MainWindowTeam::on_button_pause_clicked()
{
    if (m_pScoreScreen->isVisible())
    {
        m_pScoreScreen->hide();
        m_pUi->button_pause->setText(tr("Off"));
    }
    else
    {
        update_score_screen();
        const int nScreens(QApplication::desktop()->numScreens());

        if (nScreens > 0 && nScreens > m_secondScreenNo)
        {
            // move to second screen
            QRect screenres =
                QApplication::desktop()->screenGeometry(m_secondScreenNo);
            m_pScoreScreen->move(QPoint(screenres.x(), screenres.y()));
        }

        if (m_bAutoSize)
        {
            m_pScoreScreen->showFullScreen();
        }
        else
        {
            m_pScoreScreen->resize(m_secondScreenSize);
            m_pScoreScreen->show();
        }

        m_pUi->button_pause->setText(tr("On"));
    }
}

void MainWindowTeam::on_button_prev_clicked()
{
    if (0 == m_pController->GetCurrentFightIndex())
        return;

    m_pController->SetCurrentFight(m_pController->GetCurrentFightIndex() - 1);

    UpdateFightNumber_();
}

void MainWindowTeam::on_button_next_clicked()
{
    if (m_pController->GetCurrentFightIndex() == m_pController->GetFightCount() - 1)
        m_pController->SetCurrentFight(m_pController->GetCurrentFightIndex());
    else
        m_pController->SetCurrentFight(m_pController->GetCurrentFightIndex() + 1);

    UpdateFightNumber_();
}

void MainWindowTeam::on_comboBox_mode_currentIndexChanged(const QString& s)
{
    m_mode = s;

    if( s == str_mode_1te_bundesliga_nord_m ||
        s == str_mode_1te_bundesliga_sued_m ||
        s == str_mode_2te_bundesliga_nord_m ||
        s == str_mode_2te_bundesliga_sued_m )
    {
        m_pController->GetTournamentScoreModel(0)->SetNumRows(7);
        m_pController->GetTournamentScoreModel(1)->SetNumRows(7);
        m_pController->SetRoundTime("5:00");
        update_weights("-60;-66;-73;-81;-90;-100;+100");
    }
    else if( s == str_mode_1te_bundesliga_nord_f ||
          s == str_mode_1te_bundesliga_sued_f ||
          s == str_mode_2te_bundesliga_nord_f ||
          s == str_mode_2te_bundesliga_sued_f )
    {
        m_pController->GetTournamentScoreModel(0)->SetNumRows(7);
        m_pController->GetTournamentScoreModel(1)->SetNumRows(7);
        m_pController->SetRoundTime("5:00");
        update_weights("-48;-52;-57;-63;-70;-78;+78");
    }
    else if( s == str_mode_mm_u17_m )
    {
        m_pController->GetTournamentScoreModel(0)->SetNumRows(7);
        m_pController->GetTournamentScoreModel(1)->SetNumRows(7);
        m_pController->SetRoundTime("4:00");
        update_weights("-46;-50;-55;-60;-66;-73;+73");
    }
    else if( s == str_mode_mm_u17_f )
    {
        m_pController->GetTournamentScoreModel(0)->SetNumRows(7);
        m_pController->GetTournamentScoreModel(1)->SetNumRows(7);
        m_pController->SetRoundTime("4:00");
        update_weights("-44;-48;-52;-57;-63;-70;+70");
    }
    else if( s == str_mode_bayernliga_nord_m ||
             s == str_mode_bayernliga_sued_m ||
             s == str_mode_landesliga_nord_m ||
             s == str_mode_landesliga_sued_m )
    {
        m_pController->GetTournamentScoreModel(0)->SetNumRows(10);
        m_pController->GetTournamentScoreModel(1)->SetNumRows(10);
        m_pController->SetRoundTime("5:00");
        update_weights("-66;-73;-81;-90;+90");
    }
    else if( s == str_mode_bayernliga_nord_f ||
             s == str_mode_bayernliga_sued_f ||
             s == str_mode_landesliga_nord_f ||
             s == str_mode_landesliga_sued_f )
    {
        m_pController->GetTournamentScoreModel(0)->SetNumRows(7);
        m_pController->GetTournamentScoreModel(1)->SetNumRows(7);
        m_pController->SetRoundTime("5:00");
        update_weights("-48;-52;-57;-63;-70;-78;+78");
    }
    else
    {
        Q_ASSERT("mode not handled (yet)");
    }

    // set mode text as mat label
    m_MatLabel = s;
    m_pPrimaryView->SetMat(s);
    m_pSecondaryView->SetMat(s);

    m_pPrimaryView->UpdateView();
    m_pSecondaryView->UpdateView();
}

void MainWindowTeam::on_comboBox_club_host_currentIndexChanged(const QString& s)
{
    m_host = s;

    // set location from host
    m_pUi->lineEdit_location->setText(m_pClubManager->GetAddress(m_host));
}

void MainWindowTeam::on_comboBox_club_home_currentIndexChanged(const QString& s)
{
    m_pController->SetClub(Ipponboard::eFighter1, s);
    //UpdateViews_(); --> already done by controller
    update_score_screen();
}

void MainWindowTeam::on_comboBox_club_guest_currentIndexChanged(const QString& s)
{
    m_pController->SetClub(Ipponboard::eFighter2, s);
    //UpdateViews_(); --> already done by controller
    update_score_screen();
}

void MainWindowTeam::on_actionPrint_triggered()
{
    WriteScoreToHtml_();

    QPrinter printer(QPrinter::HighResolution);
    printer.setOrientation(QPrinter::Landscape);
    printer.setPaperSize(QPrinter::A4);
    printer.setPageMargins(15, 10, 15, 5, QPrinter::Millimeter);
    QPrintPreviewDialog preview(&printer, this);
    connect(&preview, SIGNAL(paintRequested(QPrinter*)), SLOT(Print(QPrinter*)));
    preview.exec();
}

void MainWindowTeam::on_actionExport_triggered()
{
    WriteScoreToHtml_();

    // save file to...
    QString selectedFilter;
    QString dateStr(m_pUi->dateEdit->text());
    dateStr.replace('.', '-');
    QString fileName = QFileDialog::getSaveFileName(this,
                       tr("Export file to..."),
                       tr("ScoreList_") + dateStr,
                       tr("PDF File (*.pdf);;HTML File (*.html)"),
                       &selectedFilter);

    if (!fileName.isEmpty())
    {
        // set wait cursor
        QApplication::setOverrideCursor(Qt::WaitCursor);

        if (fileName.endsWith(".html"))
        {
            QFile html(fileName);

            if (html.open(QFile::WriteOnly))
            {
                QTextStream ts(&html);
                ts << m_htmlScore;
                ts.flush();
                html.close();
            }
        }
        else
        {
            QPrinter printer(QPrinter::HighResolution);
            printer.setOrientation(QPrinter::Landscape);
            printer.setOutputFormat(QPrinter::PdfFormat);
            printer.setPaperSize(QPrinter::A4);
            printer.setPageMargins(15, 10, 15, 5, QPrinter::Millimeter);
            printer.setOutputFileName(fileName);
            QTextEdit edit(m_htmlScore, this);
            edit.document()->print(&printer);
        }

        QApplication::restoreOverrideCursor();
    }
}

void MainWindowTeam::on_toolButton_weights_pressed()
{
    bool ok(false);
    const QString weights = QInputDialog::getText(
                                this,
                                tr("Set Weights"),
                                tr("Set weights (separated by ';'):"),
                                QLineEdit::Normal,
                                m_weights,
                                &ok);

    if (ok)
    {
        if (m_pController->GetFightCount()/2-1 != weights.count(';')
            && m_pController->GetFightCount()-1 != weights.count(';'))
        {
            QMessageBox::critical(this, "Wrong values",
                                  tr("You need to specify %1 weight classes separated by ';'!")
                                  .arg(QString::number(m_pController->GetFightCount())));
            on_toolButton_weights_pressed();
        }
        else
        {
            update_weights(weights);
        }
    }
}

void MainWindowTeam::update_weights(QString weightString)
{
    m_weights = weightString;
    m_pController->SetWeights(weightString.split(';'));
}

void MainWindowTeam::on_pushButton_copySwitched_pressed()
{
    m_pController->CopyAndSwitchGuestFighters();
}

void MainWindowTeam::on_actionSet_Round_Time_triggered()
{
    bool ok(false);
    const QString time = QInputDialog::getText(
                             this,
                             tr("Set Value"),
                             tr("Set value to (m:ss):"),
                             QLineEdit::Normal,
                             m_pController->GetRoundTime(),
                             &ok);

    if (ok)
	{
        m_pController->SetRoundTime(time);
	}
}


void MainWindowTeam::on_button_current_round_clicked(bool checked)

{
    m_pController->SetCurrentFight(0);

    if (checked)
    {
        //m_pUi->button_current_round->setText(tr("2nd Round"));
        m_pController->SetCurrentTournament(1);
    }
    else
    {
        //m_pUi->button_current_round->setText(tr("Round"));
        m_pController->SetCurrentTournament(0);
    }

    UpdateFightNumber_();
}

void MainWindowTeam::on_actionScore_Screen_triggered()
{
    m_pUi->tabWidget->setCurrentWidget(m_pUi->tab_score_table);
}

void MainWindowTeam::on_actionScore_Control_triggered()
{
    m_pUi->tabWidget->setCurrentWidget(m_pUi->tab_view);
}

void MainWindowTeam::on_tableView_customContextMenuRequested(
	QTableView* pTableView,
    QPoint const& pos,
    const char* copySlot,
    const char* pasteSlot,
    const char* clearSlot)
{
    QMenu menu;
    QModelIndex index = pTableView->indexAt(pos);
    index = index.sibling(index.row(), 0);

    QModelIndexList selection =
        pTableView->selectionModel()->selectedIndexes();

    if (selection.empty())
    {
        Q_ASSERT(!"empty selection");
        return;
    }

    // Do not allow copy if different columns are selected
    bool copyAllowed(true);
    if (selection.size() > 1)
    {
        for (int i(0); i < selection.size() - 1; ++i)
        {
            if (selection[i].column() != selection[i+1].column())
            {
                copyAllowed = false;
                break;
            }
        }
    }

    // Paste is only allowed for the name cells
    // and if the clipboard is not empty
    const bool pasteAllowed = (selection[0].column() == TournamentModel::eCol_name1
                              || selection[0].column() == TournamentModel::eCol_name2)
                              && !QApplication::clipboard()->text().isEmpty();

    const bool clearAllowed = copyAllowed;

    if (index.isValid())
    {
        QIcon copyIcon(":/res/icons/copy_cells.png");
        QIcon pasteIcon(":/res/icons/paste.png");
        QIcon clearIcon(":/res/icons/clear_cells.png");
        QAction* pAction = nullptr;
        pAction = menu.addAction(copyIcon, tr("Copy"), this, copySlot, QKeySequence::Copy);
        pAction->setDisabled(!copyAllowed);

        pAction = menu.addAction(pasteIcon, tr("Paste"), this, pasteSlot, QKeySequence::Paste);
        pAction->setDisabled(!pasteAllowed);

        pAction = menu.addAction(clearIcon, tr("Clear"), this, clearSlot, QKeySequence::Delete);
        pAction->setDisabled(!clearAllowed);

        menu.exec(QCursor::pos());
    }
}


void MainWindowTeam::on_tableView_tournament_list1_customContextMenuRequested(QPoint const& pos)
{
    on_tableView_customContextMenuRequested(
            m_pUi->tableView_tournament_list1,
            pos,
            SLOT(slot_copy_cell_content_list1()),
            SLOT(slot_paste_cell_content_list1()),
            SLOT(slot_clear_cell_content_list1()));
}

void MainWindowTeam::on_tableView_tournament_list2_customContextMenuRequested(QPoint const& pos)
{
    on_tableView_customContextMenuRequested(
            m_pUi->tableView_tournament_list2,
            pos,
            SLOT(slot_copy_cell_content_list2()),
            SLOT(slot_paste_cell_content_list2()),
            SLOT(slot_clear_cell_content_list2()));
}

void MainWindowTeam::copy_cell_content(QTableView* pTableView)
{
    QModelIndexList selection = pTableView->selectionModel()->selectedIndexes();
    std::sort(selection.begin(), selection.end());

    // Copy is only allowed for single column selection
    for (int i(0); i < selection.size() - 1; ++i)
    {
        if (selection[i].column() != selection[i+1].column())
        {
            QApplication::clipboard()->clear();
            return;
        }
    }

    QString selectedText;
    Q_FOREACH(QModelIndex index, selection)
    {
        QVariant data =
            pTableView->model()->data(index, Qt::DisplayRole);

        selectedText += data.toString() + '\n';
    }
    if (!selectedText.isEmpty())
    {
        selectedText.truncate(selectedText.lastIndexOf('\n'));  // remove last '\n'
        QApplication::clipboard()->setText(selectedText);
    }
}

void MainWindowTeam::paste_cell_content(QTableView* pTableView)
{
    if (QApplication::clipboard()->text().isEmpty())
    {
        QMessageBox::warning(this, QApplication::applicationName(),
                              tr("There is nothing to paste!"));
        return;
    }
    QStringList data = QApplication::clipboard()->text().split('\n');

    QModelIndexList selection = pTableView->selectionModel()->selectedIndexes();
    if (selection.empty())
    {
        QMessageBox::critical(this, QApplication::applicationName(),
                              tr("Can not paste into an empty selection!"));
        return;
    }
    std::sort(selection.begin(), selection.end());

    if (data.size() < selection.size())
    {
        QMessageBox::critical(this, QApplication::applicationName(),
                              tr("There is too few data for the selection in the clipboard!"));
        return;
    }


    if (data.size() > selection.size())
    {
        // extend selection to maximum possible
        QModelIndex index = selection.back();
        const int nRows = pTableView->model()->rowCount();
        while (index.row() < nRows &&
               index.isValid() &&
               data.size() > selection.size())
        {
            index = pTableView->model()->index(
                    index.row() + 1, index.column());
            selection.push_back(index);
            pTableView->selectionModel()->select(index, QItemSelectionModel::Select);
        }

        if (data.size() < selection.size())
        {
            QMessageBox::warning(this, QApplication::applicationName(),
                tr("There is more data available in the clipboard as could be pasted!"));
        }
    }

    int dataIndex(0);
    Q_FOREACH(QModelIndex index, selection)
    {
        if (index.column() == TournamentModel::eCol_name1 ||
                index.column() == TournamentModel::eCol_name2)
        {
            pTableView->model()->setData(
                    index, data[dataIndex], Qt::EditRole);
            ++dataIndex;
        }
    }
}

void MainWindowTeam::clear_cell_content(QTableView* pTableView)
{
    QModelIndexList selection = pTableView->selectionModel()->selectedIndexes();
    std::sort(selection.begin(), selection.end());

    // Clear is only allowed for single column selection
    for (int i(0); i < selection.size() - 1; ++i)
    {
        if (selection[i].column() != selection[i+1].column())
        {
            QApplication::clipboard()->clear();
            return;
        }
    }

    Q_FOREACH(QModelIndex index, selection)
    {
        pTableView->model()->setData(
                index, "", Qt::EditRole);
    }
}

void MainWindowTeam::slot_copy_cell_content_list1()
{
    copy_cell_content(m_pUi->tableView_tournament_list1);
}

void MainWindowTeam::slot_copy_cell_content_list2()
{
    copy_cell_content(m_pUi->tableView_tournament_list2);
}

void MainWindowTeam::slot_paste_cell_content_list1()
{
    paste_cell_content(m_pUi->tableView_tournament_list1);
}

void MainWindowTeam::slot_paste_cell_content_list2()
{
    paste_cell_content(m_pUi->tableView_tournament_list2);
}

void MainWindowTeam::slot_clear_cell_content_list1()
{
    clear_cell_content(m_pUi->tableView_tournament_list1);
}

void MainWindowTeam::slot_clear_cell_content_list2()
{
    clear_cell_content(m_pUi->tableView_tournament_list2);
}

QString MainWindowTeam::get_template_file(QString const& mode)
{
    if (str_mode_1te_bundesliga_nord_m == mode ||
        str_mode_1te_bundesliga_sued_m == mode ||
        str_mode_1te_bundesliga_nord_f == mode ||
        str_mode_1te_bundesliga_sued_f == mode ||
        str_mode_2te_bundesliga_nord_m == mode ||
        str_mode_2te_bundesliga_sued_m == mode ||
        str_mode_2te_bundesliga_nord_f == mode ||
        str_mode_2te_bundesliga_sued_f == mode)
    {
        return "templates\\list_output_bundesliga.html";
    }

    if (str_mode_bayernliga_nord_m == mode ||
        str_mode_bayernliga_sued_m == mode ||
        str_mode_landesliga_nord_m == mode ||
        str_mode_landesliga_sued_m == mode ||
        str_mode_bayernliga_nord_f == mode ||
        str_mode_bayernliga_sued_f == mode ||
        str_mode_landesliga_nord_f == mode ||
        str_mode_landesliga_sued_f == mode)
    {
        return "templates\\list_output_bay.html";
    }

    if (str_mode_mm_u17_m == mode ||
        str_mode_mm_u17_f == mode)
    {
        return "templates\\list_output_mm.html";
    }

    return "";
}


QString MainWindowTeam::get_full_mode_title(QString const& mode)
{
    QString year(QString::number(QDate::currentDate().year()));

    // Bundesliga M�nner
    if (str_mode_1te_bundesliga_nord_m == mode)
        return QString("1. Judo Bundesliga M�nner %1 - Gruppe Nord").arg(year);
    if (str_mode_1te_bundesliga_sued_m == mode)
        return QString("1. Judo Bundesliga M�nner %1 - Gruppe S�d").arg(year);
    if (str_mode_2te_bundesliga_nord_m == mode)
        return QString("2. Judo Bundesliga M�nner %1 - Gruppe Nord").arg(year);
    if (str_mode_2te_bundesliga_sued_m == mode)
        return QString("2. Judo Bundesliga M�nner %1 - Gruppe S�d").arg(year);

    // Bundesliga Frauen
    if (str_mode_1te_bundesliga_nord_f == mode)
        return QString("1. Judo Bundesliga Frauen %1 - Gruppe Nord").arg(year);
    if (str_mode_1te_bundesliga_sued_f == mode)
        return QString("1. Judo Bundesliga Frauen %1 - Gruppe S�d").arg(year);
    if (str_mode_2te_bundesliga_nord_f == mode)
        return QString("2. Judo Bundesliga Frauen %1 - Gruppe Nord").arg(year);
    if (str_mode_2te_bundesliga_sued_f == mode)
        return QString("2. Judo Bundesliga Frauen %1 - Gruppe S�d").arg(year);

    // Bayernliga M�nner
    if (str_mode_bayernliga_nord_m == mode)
        return QString("Judo Bayernliga M�nner %1 - Gruppe Nord").arg(year);
    if (str_mode_bayernliga_sued_m == mode)
        return QString("Judo Bayernliga M�nner %1 - Gruppe S�d").arg(year);

    // Bayernliga Frauen
    if (str_mode_bayernliga_nord_f == mode)
        return QString("Judo Bayernliga Frauen %1 - Gruppe Nord").arg(year);
    if (str_mode_bayernliga_sued_f == mode)
        return QString("Judo Bayernliga Frauen %1 - Gruppe S�d").arg(year);

    // Landesliga M�nner
    if (str_mode_landesliga_nord_m == mode)
        return QString("Judo Landesliga M�nner %1 - Gruppe Nord").arg(year);
    if (str_mode_landesliga_sued_m == mode)
        return QString("Judo Landesliga M�nner %1 - Gruppe S�d").arg(year);

    // Landesliga Frauen
    if (str_mode_landesliga_nord_f == mode)
        return QString("Judo Landesliga Frauen %1 - Gruppe Nord").arg(year);
    if (str_mode_landesliga_sued_f == mode)
        return QString("Judo Landesliga Frauen %1 - Gruppe S�d").arg(year);

    // Mannschafts-Meisterschaften
    if (str_mode_mm_u17_m == mode)
        return QString("Deutsche Judo Vereins-Mannschafts-Meisterschaft MU17");
    if (str_mode_mm_u17_f == mode)
        return QString("Deutsche Judo Vereins-Mannschafts-Meisterschaft FU17");

    return tr("Ipponboard fight list");
}
