﻿#include "MainWindow.h"
#include "MainWindowTeam.h"
#include "../core/TournamentMode.h"
#include "SplashScreen.h"
#include "versioninfo.h"
#include "UpdateChecker.h"
#include "../util/path_helpers.h"

#include <QtGui/QApplication>
#include <QTranslator>
#include <QMessageBox>
#include <QSettings>
#include <QFile>
#include <QLocale>
#include <QtextCodec>



void LangNotFound(const QString& fileName)
{
	QMessageBox::critical(nullptr,
						  QCoreApplication::applicationName(),
						  "Unable to read language file: " + fileName +
						  "\nThe default language is being used.");
}

void SetTranslation(QApplication& app, QTranslator& translator, QString const& langStr)
{
	UNREFERENCED_PARAMETER(app);

	if (langStr == QString("en"))
	{
		return; // default
	}

	if (langStr == QString("de") || langStr == QString("nl"))
	{
		const QString& langPath =
			QCoreApplication::applicationDirPath() + QString("/lang");

		if (translator.load(langStr, langPath))
		{
			app.installTranslator(&translator);
		}
		else
		{
			LangNotFound(langStr);
		}
	}
}

int ShowSplashScreen()
{
	auto t1 = QCoreApplication::tr("the score board for judoka by judoka");
	auto t10 = QCoreApplication::tr("Judo is part of our lives.");
	auto t2 = QCoreApplication::tr("Therefore, Ipponboard is not just a simple display program, but developed by judoka for judoka. It is not only helpful for the people at the time table, but also for the trainers, the audience and the fighters themselves.");
	auto t3 = QCoreApplication::tr("Ipponboard is not only clearly readable but also revolutionary easy and intuitive to use. Therefore, it is appreciated by clubs and organizations around the world and has been used for many years at major championships.");
	auto t4 = QCoreApplication::tr("This version can be used without restriction. Copying in unchanged form is permitted.");
	auto t5 = QCoreApplication::tr("If you like Ipponboard, please support its development by:");
	auto t6 = QCoreApplication::tr("feedback");
	auto t7 = QCoreApplication::tr("wishes and suggestions");
	auto t8 = QCoreApplication::tr("or by giving appreciative donations");
	auto t9 = QCoreApplication::tr("Thank you very much!");

	auto text = QString("<html><body><p><big><span style=\"color:#336699;font-weight:bold\">Ipponboard</span> - %1</big></p>"
						"<blockquote><p><em><b>%10</b> %2</em></p>"
						"<p><em>%3</em></p></blockquote>"
						"<p>%4</p>"
						"<p>%5<ul>"
						"<li>%6</li>"
						"<li>%7</li>"
						"<li>%8</li></ul></p>"
						"<p><em>%9</em></p>"
						"</body></html>").arg(t1, t2, t3, t4, t5, t6, t7, t8, t9).arg(t10);

	SplashScreen::Data splashData;
	splashData.text = text;
	splashData.info = QCoreApplication::applicationName()
					  + " v" + QCoreApplication::applicationVersion()
					  + "\n"
					  + "Build: " + VersionInfo::Date;
	SplashScreen splash(splashData);
	//splash.SetImageStyleSheet("image: url(:/res/images/logo_team.png);");
	//splash.resize(480, 410);

	auto dlgResult = splash.exec();

	if (QDialog::Rejected == dlgResult)
	{
		return 0;
	}

	return dlgResult;
}

int main(int argc, char* argv[])
{
	QApplication a(argc, argv);

	QCoreApplication::setApplicationVersion(VersionInfo::VersionStr);
	QCoreApplication::setOrganizationName("Florian Mücke");
	QCoreApplication::setOrganizationDomain("ipponboard.info");
	QCoreApplication::setApplicationName("Ipponboard");

	// read language code
	QString langStr = QLocale::system().name();
	langStr.truncate(langStr.lastIndexOf('_'));

	const QString ini(QString::fromStdString(fm::GetSettingsFilePath("Ipponboard.ini")));

	QSettings settings(ini, QSettings::IniFormat, &a);
	settings.beginGroup(str_tag_Main);

	if (settings.contains(str_tag_Language))
	{
		langStr = settings.value(str_tag_Language).toString();
	}

	settings.endGroup();

	QTranslator translator;  // Note: this object needs to remain in scope.
	SetTranslation(a, translator, langStr);

	if (UpdateChecker::CheckForNewerVersion())
	{
		return 0;
	}

	auto dlgResult = ShowSplashScreen();

	if (dlgResult == 0)
	{
		return 0;
	}

	std::unique_ptr<MainWindowBase> pMainWnd = nullptr;

	if (dlgResult == QDialog::Accepted + 1)
	{
		pMainWnd = std::make_unique<MainWindowTeam>();
	}
	else
	{
		pMainWnd = std::make_unique<MainWindow>();
	}

	try
	{
		pMainWnd->Init();
	}
	catch (std::exception const&)
	{
		return 0;
	}

	pMainWnd->show();

	return a.exec();
}
