#ifndef studrating_H
#define studrating_H

#include <QtGui/QMainWindow>
#include "ui_mainwindow.h"
#include "studratingmanager.h"
#include "createdbdialog.h"
#include "studentlistdialog.h"
#include "adddatedialog.h"

class QStringListModel;
class QSettings;
class QCloseEvent;

namespace Ui
{
	class MainWindow;
}

class studrating : public QMainWindow
{
	Q_OBJECT
public:
	explicit studrating(QWidget *parent = 0, Qt::WindowFlags flags = 0);
	virtual ~studrating();
	
	void showDisciplinList();
	void showGroupslinList();
	void showCriteriaAdvances();
	void setCriterionList();
	void showRating();
	
private slots:
	void on_contentsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
	void on_createDbButton_clicked();
	void on_selectDbFilePathButton_clicked();
	void on_addDisciplinButton_clicked();
	void on_editDisciplinButton_clicked();
	void on_addGroupButton_clicked();
	void on_studentListButton_clicked();
	void on_addCriterionButton_clicked();
	void on_srDisciplinComboBox_activated();
	void on_srGroupsComboBox_activated();
	void on_ratingDisciplinComboBox_activated();
	void on_ratingGroupsComboBox_activated();
	void on_ratingCriterionComboBox_activated();
	void on_addRatingButton_clicked();
	void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionExit_triggered();
	void on_actionOptions_triggered();
	void on_actionHelp_triggered();
	void on_actionAbout_triggered();
	
protected:
	void closeEvent(QCloseEvent *event);
	
private:
	Ui::MainWindow *ui;
	StudRatingManager *manager;
	QSettings *settings;
	CreateDbDialog *createDbDialog;
	StudentListDialog *studentListDialog;
	AddDateDialog *addDateDialog;
	QString dbFilePath;
	QStringListModel *disciplinModel;
	quint32 currentPageIndex;
	
	void createIcon();
	void writeSettings();
	void readSettings();
};

#endif // studrating_H
