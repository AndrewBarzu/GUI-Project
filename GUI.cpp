#include "GUI.h"
#include <qdebug.h>
#include <unordered_map>
#include "setDebugNew.h"
#define new DEBUG_NEW

void QtGUI_Hybris::initGUI()
{
	ui.setupUi(this);

	this->charter = new GUICharter();
	this->ui.tabWidget->insertTab(1, charter, tr("Chart"));
	/*auto elems = this->controller.print();
	for (auto t : elems)
	{
		this->repo.add(t);
	}*/
	this->model = new TowerModel(this->controller);
	this->filter = new QSortFilterProxyModel();
	this->filter->setSourceModel(model);
	this->ui.towerTableView->setModel(this->filter);
	this->ui.towerTableView->verticalHeader()->hide();
	this->ui.towerTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	widget = new TestWidget(this->filter, this);
	widget->show();


	
	this->undo = new QShortcut(QKeySequence(QKeySequence::Undo), this);
	this->redo = new QShortcut(QKeySequence(QKeySequence::Redo), this);

	this->connectSignalsAndSlots();
	this->setFocusPolicy(Qt::ClickFocus);
	emit populateTowerList("");
	emit populateSavedList();
}

void QtGUI_Hybris::connectSignalsAndSlots()
{
	QObject::connect(this, &QtGUI_Hybris::towersUpdateSignal, this, &QtGUI_Hybris::populateTowerList);
	QObject::connect(this, &QtGUI_Hybris::savedUpdateSignal, this, &QtGUI_Hybris::populateSavedList);

	QObject::connect(this->ui.addButton, &QPushButton::clicked, this, &QtGUI_Hybris::addTowerHandler);
	QObject::connect(this->ui.deleteButton, &QPushButton::clicked, this, &QtGUI_Hybris::deleteTowerHandler);
	QObject::connect(this->ui.updateButton, &QPushButton::clicked, this, &QtGUI_Hybris::updateTowerHandler);
	QObject::connect(this->ui.saveButton, &QPushButton::clicked, this, &QtGUI_Hybris::saveTowerHandler);
	QObject::connect(this->ui.filterButton, &QPushButton::clicked, this, &QtGUI_Hybris::filterHandler);
	QObject::connect(this->ui.undoButton, &QPushButton::clicked, this, &QtGUI_Hybris::undoHandler);
	QObject::connect(this->ui.redoButton, &QPushButton::clicked, this, &QtGUI_Hybris::redoHandler);
	QObject::connect(this->undo, &QShortcut::activated, this, &QtGUI_Hybris::undoHandler);
	QObject::connect(this->redo, &QShortcut::activated, this, &QtGUI_Hybris::redoHandler);

	QObject::connect(this->ui.tabWidget, &QTabWidget::currentChanged, this, &QtGUI_Hybris::changedTabHandler);

	QObject::connect(this, SIGNAL(towerAddSignal(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&)),
		this, SLOT(addTower(const std::string&, const std::string&, const std::string&, const std::string&, const std::string&)));

	
}

void QtGUI_Hybris::addTower(const std::string& location, const std::string& size, const std::string& auraLevel, const std::string& parts, const std::string& vision)
{
	try
	{
		this->controller.add(location, size, auraLevel, parts, vision);
		//this->repo.add(Tower({ location, size, auraLevel, parts, vision }));
		//emit this->model->updateModel(TowerModel::UpdateType::add);
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(0, "Error", e.what());
		//this->errorMessageBox->setText(QString::fromStdString(e.what()));
		//this->errorMessageBox->exec();
		return;
	}
	emit towersUpdateSignal();
	this->model->update();
}

void QtGUI_Hybris::populateTowerList(const std::string& filterSize)
{
	if (this->ui.towerList->count() > 0)
		this->ui.towerList->clear();

	if (filterSize != "")
	{
		this->ui.filterLabel->setText(QString::fromStdString("Towers: filtered by size \"" + filterSize + "\""));
	}
	else
	{
		this->ui.filterLabel->setText(QString::fromStdString("Towers: not filtered"));
	}
	auto towers = this->controller.print(filterSize);
	for (auto& tower: towers)
	{
		//qDebug() << tower.get_location().c_str();
		//QString itemString = QString::fromStdString((*tower).get_location() + ", " + (*tower).get_size() + ", " + (*tower).get_parts() + ", " + (*tower).get_aura_level() + ", " + (*tower).get_vision());
		QTowerListItem* item = new QTowerListItem{ tower };
		this->ui.towerList->addItem(item);
	}

	/*if (this->ui.towerList->count() > 0)
		this->ui.towerList->setCurrentRow(0);*/
}

void QtGUI_Hybris::populateSavedList()
{
	if (this->ui.savedList->count() > 0)
		this->ui.savedList->clear();
	auto towers = this->controller.getSaved();
	for (auto tower: towers)
	{
		//QString itemString = QString::fromStdString((*tower).get_location() + ", " + (*tower).get_size() + ", " + (*tower).get_parts() + ", " + (*tower).get_aura_level() + ", " + (*tower).get_vision());
		QTowerListItem* item = new QTowerListItem{ tower };
		this->ui.savedList->addItem(item);
	}

	if (this->ui.savedList->count() > 0)
		this->ui.savedList->setCurrentRow(0);
}

void QtGUI_Hybris::addTowerHandler()
{
	QString location = this->ui.locationLineEdit->text();
	QString size = this->ui.sizeLineEdit->text();
	QString parts = this->ui.partsLineEdit->text();
	QString level = this->ui.auraLineEdit->text();
	QString vision = this->ui.visionLineEdit->text();
	emit towerAddSignal(location.toStdString(), size.toStdString(), level.toStdString(), parts.toStdString(), vision.toStdString());
}

void QtGUI_Hybris::deleteTowerHandler()
{
	if (this->ui.towerList->count() == 0)
	{
		QMessageBox::critical(0, "Error", "No element to remove!");
		//this->errorMessageBox->setText("No element to remove!");
		//this->errorMessageBox->exec();
		return;
	}
	if (this->ui.towerList->selectedItems().size() == 0)
	{
		QMessageBox::critical(this, "No item selected", "You haven't selected an item!", QMessageBox::Ok);
		return;
	}
	QTowerListItem* item = (QTowerListItem*)this->ui.towerList->selectedItems()[0];
	try 
	{
		this->controller.remove(item->get_location());
		//this->repo.remove(item->get_location());
		//emit this->model->updateModel(TowerModel::UpdateType::remove);
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(0, "Error", e.what());
		//this->errorMessageBox->setText(QString::fromStdString(e.what()));
		//this->errorMessageBox->exec();
		return;
	}
	emit towersUpdateSignal();
	emit savedUpdateSignal();
	this->model->update();
}

void QtGUI_Hybris::updateTowerHandler()
{
	QString location = this->ui.locationLineEdit->text();
	QString size = this->ui.sizeLineEdit->text();
	QString parts = this->ui.partsLineEdit->text();
	QString level = this->ui.auraLineEdit->text();
	QString vision = this->ui.visionLineEdit->text();
	try
	{
		this->controller.update(location.toStdString(), size.toStdString(), level.toStdString(), parts.toStdString(), vision.toStdString());
		//this->repo.update(Tower({ location.toStdString(), size.toStdString(), level.toStdString(), parts.toStdString(), vision.toStdString() }));
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(0, "Error", e.what());
		//this->errorMessageBox->setText(QString::fromStdString(e.what()));
		//this->errorMessageBox->exec();
		return;
	}
	//emit this->model->updateModel(TowerModel::UpdateType::update);
	emit towersUpdateSignal();
	emit savedUpdateSignal();
	this->model->update();
}

void QtGUI_Hybris::saveTowerHandler()
{
	if (this->ui.towerList->count() == 0)
	{
		QMessageBox::critical(0, "Error", "Nothing to save!");
		return;
	}

	auto selected = (QTowerListItem*)this->ui.towerList->selectedItems()[0];

	try
	{
		this->controller.save(selected->get_location());
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(0, "Error", e.what());
		return;
	}

	emit savedUpdateSignal();
}

void QtGUI_Hybris::filterHandler()
{
	auto filter = this->ui.sizeLineEdit->text();
	this->filter->setFilterRegExp(QRegExp(filter));
	this->filter->setFilterKeyColumn(1);
	emit towersUpdateSignal(filter.toStdString());
}

void QtGUI_Hybris::changedTabHandler(const int& tab)
{
	switch (tab)
	{
	case 1:
		this->charter->chartSlot(this->controller);
		break;
	default:
		break;
	}
}

void QtGUI_Hybris::undoHandler()
{
	try
	{
		this->controller.undo();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(0, "Error", e.what());
	}
	this->populateSavedList();
	this->populateTowerList();
	this->model->update();
}

void QtGUI_Hybris::redoHandler()
{
	try
	{
		this->controller.redo();
	}
	catch (const std::exception& e)
	{
		QMessageBox::critical(0, "Error", e.what());
	}
	this->populateSavedList();
	this->populateTowerList();
	this->model->update();
}

//void QtGUI_Hybris::keyPressEvent(QKeyEvent* myEvent)
//{
//	if ((myEvent->modifiers() & Qt::ControlModifier) != false)
//		switch (myEvent->key())
//		{
//		case Qt::Key_Z:
//			this->undoHandler();
//			break;
//		case Qt::Key_Y:
//			this->redoHandler();
//			break;
//		default:
//			break;
//		}
//}
