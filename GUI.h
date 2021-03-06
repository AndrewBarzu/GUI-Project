#pragma once

#include <qwidget.h>
#include <QtWidgets/QMainWindow>
#include <qlistwidget.h>
#include <qformlayout.h>
#include <qlineedit.h>
#include <qlabel.h>
#include <qpushbutton.h>
#include <qmessagebox.h>
#include <qtabwidget.h>
#include <QtCharts>
#include <qsortfilterproxymodel.h>
#include "domain.h"
#include "controller.h"
#include "GUICharter.h"
#include "ui_QtGUI_Hybris.h"
#include "TowerModel.h"

class QTowerListItem : public QListWidgetItem, public Tower
{
public:
	QTowerListItem(Tower tower) : QListWidgetItem{ QString::fromStdString(tower.print()) }, Tower{ tower }{}

};

class ImageDelegate : public QStyledItemDelegate {
protected:
	void paint(QPainter* painter, const QStyleOptionViewItem& opt, const QModelIndex& index) const {

		/*if (opt.state & QStyle::State_Selected) {
			painter->setBrush(Qt::green);
		}
		else {
			painter->setBrush(Qt::blue);
		}*/
		QPixmap image{ index.data().toString() + ".png" };

		painter->drawPixmap(opt.rect, image);
	}
};


class TestWidget : public QDialog
{
private:
	QTableView* view;
public:
	TestWidget(TowerModel* model, QWidget* parent = nullptr) : QDialog{ parent }
	{
		this->setFixedWidth(755);
		this->setMinimumHeight(500);
		QHBoxLayout* layout = new QHBoxLayout(this);
		this->view = new QTableView;
		this->view->setModel(model);
		QHeaderView* verticalHeader = this->view->verticalHeader();
		verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
		verticalHeader->setDefaultSectionSize(200);
		verticalHeader->hide();
		this->view->setColumnWidth(4, 200);
		layout->addWidget(this->view);
		this->view->setItemDelegateForColumn(4, new ImageDelegate);
	}
};


class QtGUI_Hybris : public QMainWindow
{
	Q_OBJECT
public:
	QtGUI_Hybris(Controller& controller, QWidget* parent = Q_NULLPTR) : QMainWindow(parent), controller{ controller } { initGUI(); };
	~QtGUI_Hybris() { delete model; }

private:
	Ui::QtGUI_HybrisClass ui;

	Controller controller;
	TowerModel* model;
	TowerModel* savedModel;
	QSortFilterProxyModel* filter;
	QShortcut* undo;
	QShortcut* redo;

	TestWidget* widget;
	//RepoInMemory repo;

	GUICharter* charter;

	void initGUI();
	void connectSignalsAndSlots();
	void populateTowerList(const std::string& sizeFilter = "");
	void populateSavedList();

	//void keyPressEvent(QKeyEvent* myEvent);

	void addTowerHandler();
	void deleteTowerHandler();
	void updateTowerHandler();
	void saveTowerHandler();
	void filterHandler();
	void changedTabHandler(const int& tab);
	void undoHandler();
	void redoHandler();

	void populateLineEdits();

signals:
	void towersUpdateSignal(const std::string& sizeFilter = "");
	void savedUpdateSignal();
	void towerAddSignal(const std::string& location, const std::string& size, const std::string& auraLevel, const std::string& parts, const std::string& vision);


private slots:
	void addTower(const std::string& location, const std::string& size, const std::string& auraLevel, const std::string& parts, const std::string& vision);
};
