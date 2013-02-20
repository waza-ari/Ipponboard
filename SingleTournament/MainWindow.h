// Copyright 2010-2013 Florian Muecke. All rights reserved.
// http://www.ipponboard.info (ipponboardinfo at googlemail dot com)
//
// THIS FILE IS PART OF THE IPPONBOARD PROJECT.
// IT MAY NOT BE DISTRIBUTED TO OR SHARED WITH THE PUBLIC IN ANY FORM!
//
#ifndef BASIC_EDITION_MAINWINDOW_H_
#define BASIC_EDITION_MAINWINDOW_H_

#include "../base/mainwindowbase.h"
#include <boost/shared_ptr.hpp>

class MainWindow : public MainWindowBase
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget* parent = nullptr);
    virtual ~MainWindow();

	virtual void Init() final;

	virtual const char* EditionName() const final		{ return "Basic Edition"; }
	virtual const char* EditionNameShort() const final		{ return "Basic"; }

protected:
    //virtual void changeEvent(QEvent* e) override;
    //virtual void closeEvent(QCloseEvent* event) override;
    //virtual void keyPressEvent(QKeyEvent* event) override;
    virtual void update_statebar() override;

private:
	//void update_info_text_color(const QColor& color, const QColor& bgColor) override;
    //void update_text_color_first(const QColor& color, const QColor& bgColor) override;
    //void update_text_color_second(const QColor& color, const QColor& bgColor) override;
    //virtual void update_fighter_name_font(const QFont&) override;
    //virtual void update_views() override;

	/* specific private methods */
	void update_fighter_name_completer(const QString& weight);
    void update_fighters(const QString& s);
    
	/* base class slot overrides */
	virtual void on_actionManageFighters_triggered() override;

protected slots:
    //virtual bool EvaluateSpecificInput(FMlib::Gamepad const* pGamepad) override;

	/* specific private slots */
	void on_actionManageClasses_triggered(); //TODO: rename to categories
    void on_comboBox_weight_currentIndexChanged(const QString&);
    void on_comboBox_name_second_currentIndexChanged(const QString&);
    void on_comboBox_name_first_currentIndexChanged(const QString&);
    void on_checkBox_golden_score_clicked(bool checked);
    void on_comboBox_weight_class_currentIndexChanged(const QString&);

private slots:
    void on_checkBox_use2013rules_toggled(bool checked);
    void on_checkBox_autoIncrement_toggled(bool checked);
	
private:
    /* member */
	boost::shared_ptr<Ipponboard::FightCategoryMgr> m_pCategoryManager;
    QStringList m_CurrentFighterNames;
};

#endif  // BASIC_EDITION_MAINWINDOW_H_
