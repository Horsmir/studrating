#include <QtCore/QSettings>
#include <QtGui/QFileDialog>
#include <QtCore/QDir>
#include <QtGui/QCloseEvent>
#include <QtGui/QStringListModel>
#include <QtGui/QInputDialog>
#include <QtGui/QMessageBox>
#include "helpbrowser.h"
#include "studrating.h"


studrating::studrating(QWidget *parent, Qt::WindowFlags flags): QMainWindow(parent, flags), ui(new Ui::MainWindow)
{
	ui->setupUi(this);
	
	manager = new StudRatingManager(this);
	dbFilePath = QString();
	disciplinModel = new QStringListModel(this);
	ui->disciplinsListView->setModel(disciplinModel);
	
	
	createDbDialog = new CreateDbDialog(this);
	studentListDialog = new StudentListDialog(this);
	addDateDialog = new AddDateDialog(this);
	
	htmlGenerator = new HtmlGenerator(this);
	
	settings = new QSettings( QSettings::NativeFormat, QSettings::UserScope, APP_NAME, QString(), this);
	readSettings();
	
	if(!dbFilePath.isEmpty())
	{
		if(manager->loadDB(dbFilePath))
		{
			ui->dbFilePathEdit->setText(dbFilePath);
			ui->aboutDbTextEdit->setPlainText(manager->getAboutDB());
			ui->label_14->setVisible(false);
			setWindowTitle(APP_NAME + "-" + VERSION + " - " + dbFilePath);
		}
		else
		{
			QMessageBox::warning(this, trUtf8("Загрузка базы данных"), trUtf8("Не возможно загрузить файл ") + dbFilePath);
			dbFilePath.clear();
		}
	}
	
	createIcon();
	if(!dbFilePath.isEmpty())
	{
		if(manager->groupsIsEmpty() || manager->disciplinsIsEmpty())
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags ^ Qt::ItemIsEnabled);
			}
		}
	}
	else
	{
		for(int i = 1; i <= 5; i++)
		{
			Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
			ui->contentsWidget->item(i)->setFlags(flags ^ Qt::ItemIsEnabled);
		}
	}
	
	ui->pagesWidget->setCurrentIndex(currentPageIndex);
	ui->contentsWidget->setCurrentRow(currentPageIndex);
	
	connect(ui->actionAboutQt, SIGNAL(triggered(bool)), qApp, SLOT(aboutQt()));
}

studrating::~studrating()
{
	delete ui;
}

void studrating::createIcon()
{
	QListWidgetItem *homeButton = new QListWidgetItem(ui->contentsWidget);
	homeButton->setIcon(QIcon(":/icons/home.png"));
	homeButton->setText(trUtf8("Главная"));
	homeButton->setTextAlignment(Qt::AlignHCenter);
	homeButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	QListWidgetItem *disciplinButton = new QListWidgetItem(ui->contentsWidget);
	disciplinButton->setIcon(QIcon(":/icons/disciplins.png"));
	disciplinButton->setText(trUtf8("Дисциплины"));
	disciplinButton->setTextAlignment(Qt::AlignHCenter);
	disciplinButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	QListWidgetItem *groupButton = new QListWidgetItem(ui->contentsWidget);
	groupButton->setIcon(QIcon(":/icons/groups.png"));
	groupButton->setText(trUtf8("Группы"));
	groupButton->setTextAlignment(Qt::AlignHCenter);
	groupButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	QListWidgetItem *setRatingButton = new QListWidgetItem(ui->contentsWidget);
	setRatingButton->setIcon(QIcon(":/icons/conf_rating.png"));
	setRatingButton->setText(trUtf8("Настройка рейтинга"));
	setRatingButton->setTextAlignment(Qt::AlignHCenter);
	setRatingButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	QListWidgetItem *ratingButton = new QListWidgetItem(ui->contentsWidget);
	ratingButton->setIcon(QIcon(":/icons/rating.png"));
	ratingButton->setText(trUtf8("Рейтинг"));
	ratingButton->setTextAlignment(Qt::AlignHCenter);
	ratingButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	QListWidgetItem *resultButton = new QListWidgetItem(ui->contentsWidget);
	resultButton->setIcon(QIcon(":/icons/results.png"));
	resultButton->setText(trUtf8("Итоги"));
	resultButton->setTextAlignment(Qt::AlignHCenter);
	resultButton->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
}

void studrating::on_contentsWidget_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
	if(!current)
		current = previous;
	
	ui->pagesWidget->setCurrentIndex(ui->contentsWidget->row(current));
	
	switch(ui->pagesWidget->currentIndex())
	{
		case 0:
			break;
		case 1:
			showDisciplinList();
			break;
		case 2:
			showGroupslinList();
			break;
		case 3:
			manager->setDisciplinView(ui->srDisciplinComboBox);
			manager->setGroupView(ui->srGroupsComboBox);
			showCriteriaAdvances();
			break;
		case 4:
			manager->setDisciplinView(ui->ratingDisciplinComboBox);
			manager->setGroupView(ui->ratingGroupsComboBox);
			setCriterionList();
			showRating();
			break;
		case 5:
			manager->setDisciplinView(ui->resultDisciplinComboBox);
			manager->setGroupView(ui->resultGroupComboBox);
			showResults();
			break;
		default:
			break;
	}
}

void studrating::on_createDbButton_clicked()
{
	if(createDbDialog->exec() == QDialog::Accepted)
	{
		dbFilePath = createDbDialog->getFilePath();
		manager->createDB(dbFilePath, createDbDialog->getAbout());
		ui->dbFilePathEdit->setText(dbFilePath);
		ui->aboutDbTextEdit->setPlainText(manager->getAboutDB());
		ui->label_14->setVisible(false);
		setWindowTitle(APP_NAME + "-" + VERSION + " - " + dbFilePath);
		
		for(int i = 1; i <= 2; i++)
		{
			Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
			ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
		}
		
		for(int i = 3; i <= 5; i++)
		{
			Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
			ui->contentsWidget->item(i)->setFlags(flags ^ Qt::ItemIsEnabled);
		}
	}
}

void studrating::on_selectDbFilePathButton_clicked()
{
	QFileDialog::Options options;
	options |= QFileDialog::DontUseNativeDialog;
	
	QString selectedFilter;
	
	QString fileName = QFileDialog::getOpenFileName(this, trUtf8("Файл базы данных"), QDir::homePath(), trUtf8("Файлы базы данных (*.dbsr);;Все файлы (*.*)"), &selectedFilter, options);
	if(!fileName.isEmpty())
	{
		dbFilePath = fileName;
		manager->loadDB(dbFilePath);
		ui->dbFilePathEdit->setText(dbFilePath);
		ui->aboutDbTextEdit->setPlainText(manager->getAboutDB());
		ui->label_14->setVisible(false);
		setWindowTitle(APP_NAME + "-" + VERSION + " - " + dbFilePath);
		
		for(int i = 1; i <= 2; i++)
		{
			Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
			ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
		}
		if(!manager->groupsIsEmpty() && !manager->disciplinsIsEmpty())
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
			}
		}
		else
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags ^ Qt::ItemIsEnabled);
			}
		}
	}
}

void studrating::closeEvent(QCloseEvent *event)
{
	manager->saveDb();
	writeSettings();
	QWidget::closeEvent(event);
}

void studrating::showDisciplinList()
{
	QStringList disciplins = manager->getDisciplinsList();
	disciplinModel->setStringList(disciplins);
	ui->disciplinsListView->setCurrentIndex(disciplinModel->index(0));
}

void studrating::on_addDisciplinButton_clicked()
{
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Добавить дисциплину"), trUtf8("Название:"), QLineEdit::Normal, QString(), &ok);
	if(ok && !text.isEmpty())
	{
		manager->addDisciplin(text);
		showDisciplinList();
		if(!manager->groupsIsEmpty())
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
			}
		}
	}
}

void studrating::on_editDisciplinButton_clicked()
{
	QString oldName = ui->disciplinsListView->currentIndex().data(Qt::DisplayRole).toString();
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Изменить дисциплину"), trUtf8("Название:"), QLineEdit::Normal, oldName, &ok);
	if (ok && !text.isEmpty())
	{
		manager->renameDisciplin(oldName, text);
		showDisciplinList();
	}
}

void studrating::readSettings()
{
	dbFilePath = settings->value("general/CurrentDBFilePath", QString()).toString();
	precision = settings->value("general/Precision", 2).toInt();
	currentPageIndex = settings->value("view/LastPage", 0).toInt();
	restoreGeometry(settings->value("view/Geometry", saveGeometry()).toByteArray());
}

void studrating::writeSettings()
{
	settings->setValue("general/ApplicationName", APP_NAME);
	settings->setValue("general/ApplicationVersion", VERSION);
	settings->setValue("general/CurrentDBFilePath", dbFilePath);
	settings->setValue("general/Precision", precision);
	settings->setValue("view/LastPage", ui->pagesWidget->currentIndex());
	settings->setValue("view/Geometry", saveGeometry());
}

void studrating::showGroupslinList()
{
	ui->groupsTreeView->setModel(manager->getGroupModel());
	manager->getGroupModel()->setMod(groupMod);
}

void studrating::on_addGroupButton_clicked()
{
	bool ok;
	
	QString text = QInputDialog::getText(this, trUtf8("Добавить группу"), trUtf8("Название:"), QLineEdit::Normal, QString(), &ok);
	if(ok && !text.isEmpty())
	{
		manager->addGroup(text);
		showGroupslinList();
		if(!manager->disciplinsIsEmpty())
		{
			for(int i = 3; i <= 5; i++)
			{
				Qt::ItemFlags flags =  ui->contentsWidget->item(i)->flags();
				ui->contentsWidget->item(i)->setFlags(flags | Qt::ItemIsEnabled);
			}
		}
	}
}

void studrating::on_studentListButton_clicked()
{
	studentListDialog->setCurrentGroup(ui->groupsTreeView->currentIndex().data(Qt::UserRole).toUInt());
	studentListDialog->setManager(manager);
	studentListDialog->show();
}

void studrating::showCriteriaAdvances()
{
	quint32 disciplinId = ui->srDisciplinComboBox->itemData(ui->srDisciplinComboBox->currentIndex()).toUInt();
	quint32 groupId = ui->srGroupsComboBox->itemData(ui->srGroupsComboBox->currentIndex()).toUInt();
	CriteriaModel *cr = manager->getCriteriaModel();
	
	cr->setParams(disciplinId, groupId);
	ui->criteriaTableView->setModel(cr);
}

void studrating::on_addCriterionButton_clicked()
{
	quint32 disciplinId = ui->srDisciplinComboBox->itemData(ui->srDisciplinComboBox->currentIndex()).toUInt();
	quint32 groupId = ui->srGroupsComboBox->itemData(ui->srGroupsComboBox->currentIndex()).toUInt();
	manager->addCriterion(disciplinId, groupId);
	manager->getCriteriaModel()->setParams(disciplinId, groupId);
}

void studrating::on_srDisciplinComboBox_activated()
{
	quint32 disciplinId = ui->srDisciplinComboBox->itemData(ui->srDisciplinComboBox->currentIndex()).toUInt();
	quint32 groupId = ui->srGroupsComboBox->itemData(ui->srGroupsComboBox->currentIndex()).toUInt();
	manager->getCriteriaModel()->setParams(disciplinId, groupId);
}

void studrating::on_srGroupsComboBox_activated()
{
	on_srDisciplinComboBox_activated();
}

void studrating::setCriterionList()
{
	quint32 disciplinId = ui->ratingDisciplinComboBox->itemData(ui->ratingDisciplinComboBox->currentIndex()).toUInt();
	quint32 groupId = ui->ratingGroupsComboBox->itemData(ui->ratingGroupsComboBox->currentIndex()).toUInt();
	manager->setCriterionView(disciplinId, groupId, ui->ratingCriterionComboBox);
	manager->getRatingModel()->setParam(disciplinId, groupId);
}

void studrating::showRating()
{
	quint32 disciplinId = ui->ratingDisciplinComboBox->itemData(ui->ratingDisciplinComboBox->currentIndex()).toUInt();
	quint32 groupId = ui->ratingGroupsComboBox->itemData(ui->ratingGroupsComboBox->currentIndex()).toUInt();
	qint32 criterionId = ui->ratingCriterionComboBox->itemData(ui->ratingCriterionComboBox->currentIndex()).toInt();
	
	RatingModel *rm = manager->getRatingModel();
	rm->setParam(disciplinId, groupId, criterionId);
	rm->setPrecision(precision);
	ui->ratingTableView->setModel(rm);
}

void studrating::on_ratingCriterionComboBox_activated()
{
	quint32 disciplinId = ui->ratingDisciplinComboBox->itemData(ui->ratingDisciplinComboBox->currentIndex()).toUInt();
	quint32 groupId = ui->ratingGroupsComboBox->itemData(ui->ratingGroupsComboBox->currentIndex()).toUInt();
	qint32 criterionId = ui->ratingCriterionComboBox->itemData(ui->ratingCriterionComboBox->currentIndex()).toInt();
	manager->getRatingModel()->setParam(disciplinId, groupId, criterionId);
}

void studrating::on_ratingDisciplinComboBox_activated()
{
	setCriterionList();
}

void studrating::on_ratingGroupsComboBox_activated()
{
	setCriterionList();
}

void studrating::on_addRatingButton_clicked()
{
	quint32 disciplinId = ui->ratingDisciplinComboBox->itemData(ui->ratingDisciplinComboBox->currentIndex()).toUInt();
	quint32 groupId = ui->ratingGroupsComboBox->itemData(ui->ratingGroupsComboBox->currentIndex()).toUInt();
	qint32 criterionId = ui->ratingCriterionComboBox->itemData(ui->ratingCriterionComboBox->currentIndex()).toInt();
	
	QString date;
	
	if(addDateDialog->exec() == QDialog::Accepted)
	{
		date = addDateDialog->getDate();
		manager->addDateRating(disciplinId, groupId, date);
		manager->getRatingModel()->setParam(disciplinId, groupId, criterionId);
	}
}

void studrating::on_actionAbout_triggered()
{
	QString str1, str2, str3, str4;
	
	str1 = trUtf8("<h2>%1 %2</h2><p><b>%1</b> - предназначена для учёта рейтинга студентов по различным дисциплинам, а также вычисления и вывода итоговых результатов.</p><p>Copyright &copy;  2012 Роман Браун</p><p>Icons: Copyright &copy; <a href=\"http://kde-look.org/usermanager/search.php?username=frag\">frag F-L</a>").arg(APP_NAME).arg(VERSION);
	str2 = trUtf8("<p>Это программа является свободным программным обеспечением. Вы можете распространять и/или модифицировать её согласно условиям Стандартной Общественной Лицензии GNU, опубликованной Фондом Свободного Программного Обеспечения, версии 3 или, по Вашему желанию, любой более поздней версии.</p>");
	str3 = trUtf8("<p>Эта программа распространяется в надежде, что она будет полезной, но БЕЗ ВСЯКИХ ГАРАНТИЙ, в том числе подразумеваемых гарантий ТОВАРНОГО СОСТОЯНИЯ ПРИ ПРОДАЖЕ и ГОДНОСТИ ДЛЯ ОПРЕДЕЛЁННОГО ПРИМЕНЕНИЯ. Смотрите Стандартную Общественную Лицензию GNU для получения дополнительной информации.</p>");
	str4 = trUtf8("<p>Вы должны были получить копию Стандартной Общественной Лицензии GNU вместе с программой. В случае её отсутствия, посмотрите <a href=\"http://www.gnu.org/licenses/\">http://www.gnu.org/licenses/</a>.</p><p>E-Mail: <a href=\"mailto:firdragon76@gmail.com\">firdragon76@gmail.com</a><br>Сайт программы: <a href=\"github.com/Horsmir/%1\">github.com/Horsmir/%1</a></p>").arg(APP_FILE_NAME);
	
	QMessageBox::about(this, trUtf8("О программе"), str1 + str2 + str3 + str4);
}

void studrating::on_actionExit_triggered()
{
	close();
}

void studrating::on_actionHelp_triggered()
{
	QString docDir;
#ifdef Q_OS_LINUX
	docDir = APP_PATH + "/share/doc/" + APP_FILE_NAME + "/html";
#endif
#ifdef Q_OS_WIN
	docDir = "doc/html";
#endif
	HelpBrowser::showPage(docDir, "index.html");
}

void studrating::on_actionNew_triggered()
{
	on_createDbButton_clicked();
}

void studrating::on_actionOpen_triggered()
{
	on_selectDbFilePathButton_clicked();
}

void studrating::on_actionOptions_triggered()
{
	//TODO
}

void studrating::on_actionSave_triggered()
{
	manager->saveDb();
}

void studrating::showResults()
{
	quint32 disciplinId = ui->resultDisciplinComboBox->itemData(ui->resultDisciplinComboBox->currentIndex()).toUInt();
	quint32 groupId = ui->resultGroupComboBox->itemData(ui->resultGroupComboBox->currentIndex()).toUInt();
	QString totalText = htmlGenerator->htmlHead() + htmlGenerator->htmlHeader().arg(ui->resultDisciplinComboBox->currentText()).arg(ui->resultGroupComboBox->currentText()) + htmlGenerator->tableHead();
	
	QStringList students = manager->getStudentNameIdList(groupId);
	if(students.isEmpty())
	{
		ui->resultTextEdit->clear();
		return;
	}
	QString ststr;
	foreach(ststr, students)
	{
		totalText += htmlGenerator->tableRow().arg(ststr.split("|").at(0)).arg(manager->getAllRatingStudent(disciplinId, groupId, ststr.split("|").at(1).toUInt()), 0, 'f', precision);
	}
	totalText += htmlGenerator->htmlEnd().arg(manager->getMidleRating(disciplinId, groupId), 0, 'f', precision).arg(manager->getAllRating(disciplinId, groupId), 0, 'f', precision);
	
	ui->resultTextEdit->clear();
	ui->resultTextEdit->setHtml(totalText);
}

void studrating::on_resultDisciplinComboBox_activated()
{
	showResults();
}

void studrating::on_resultGroupComboBox_activated()
{
	showResults();
}

#include "studrating.moc"
