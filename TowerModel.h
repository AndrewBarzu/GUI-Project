#pragma once
#include <qabstractitemmodel.h>
#include "controller.h"

class TowerModel :
	public QAbstractTableModel
{
	Q_OBJECT
private:
	RepoInterface* repo;
public:
	enum class UpdateType {add, remove, update};

	TowerModel(RepoInterface* repo, QObject* parent = nullptr) : repo{ repo }, QAbstractTableModel{ parent } {
		QObject::connect(this, &TowerModel::updateModel, this, &TowerModel::updateSlot);
	}

	int rowCount(const QModelIndex& parent = QModelIndex()) const override;

	int columnCount(const QModelIndex& parent = QModelIndex()) const override;

	QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	
	bool insertRows(int position, int rows, const QModelIndex& parentIndex) override;

	bool removeRows(int position, int rows, const QModelIndex& parentIndex) override;
	void update();

signals:
	void updateModel(UpdateType = UpdateType::add);
private slots:
	void updateSlot(UpdateType type);
};

