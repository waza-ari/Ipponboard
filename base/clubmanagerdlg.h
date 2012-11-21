#ifndef BASE__CLUBMANAGERDLG_H_
#define BASE__CLUBMANAGERDLG_H_

#include <QDialog>
#include <memory>

// forwards
namespace Ui { class ClubManagerDlg; }
namespace Ipponboard { class ClubManager; }
class QListWidgetItem;

class ClubManagerDlg : public QDialog
{
	Q_OBJECT
public:
    ClubManagerDlg(
            std::tr1::shared_ptr<Ipponboard::ClubManager> pMgr,
            QWidget* parent = nullptr);
	~ClubManagerDlg();

protected:
	void changeEvent(QEvent* e);

private:
	void select_club(int index);
	void update_ui();

    Ui::ClubManagerDlg* ui;
    std::tr1::shared_ptr<Ipponboard::ClubManager> m_pClubMgr;
	int m_SelectedClub;

private slots:
	void on_pushButton_browseLogo_pressed();
	void on_pushButton_save_pressed();
	void on_lineEdit_logoFile_textEdited(QString const& fileName);
	void on_comboBox_club_currentIndexChanged(int index);
	void on_pushButton_remove_pressed();
	void on_pushButton_add_pressed();
};

#endif  // BASE__CLUBMANAGERDLG_H_
