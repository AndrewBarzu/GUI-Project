#include "TowerModel.h"
#

int TowerModel::rowCount(const QModelIndex& parent) const
{
	return this->controller.size();
}

int TowerModel::columnCount(const QModelIndex& parent) const
{
	return 5;
}

QVariant TowerModel::data(const QModelIndex& index, int role) const
{
	int row = index.row();
	int col = index.column();

	if (row == this->controller.size())
		return QVariant();

	Tower t = this->controller.print()[row];

	if (role == Qt::DisplayRole || role == Qt::EditRole)
	{
		switch (col)
		{
		case 0:
			return QString::fromStdString(t.get_location());
		case 1:
			return QString::fromStdString(t.get_size());
		case 2:
			return QString::fromStdString(t.get_aura_level());
		case 3:
			return QString::fromStdString(t.get_parts());
		case 4:
			return QString::fromStdString(t.get_vision());
		}
	}

	return QVariant();
}

QVariant TowerModel::headerData(int section, Qt::Orientation orientation, int role) const
{

	return QVariant();
}

bool TowerModel::insertRows(int position, int rows, const QModelIndex& parentIndex)
{
	Q_ASSERT(checkIndex(parentIndex));
	if (position<0 || position>rowCount(parentIndex) || rows <= 0)
		return false;
	emit layoutAboutToBeChanged();
	beginInsertRows(parentIndex, position, position + rows - 1);
	endInsertRows();
	emit layoutChanged();
	return true;
}

bool TowerModel::removeRows(int position, int rows, const QModelIndex& parentIndex)
{
	Q_ASSERT(checkIndex(parentIndex));
	if (position<0 || position>rowCount(parentIndex) || rows <= 0)
		return false;
	emit layoutAboutToBeChanged();
	beginRemoveRows(parentIndex, position, position + rows - 1);
	endRemoveRows();
	emit layoutChanged();
	return true;
}

void TowerModel::update()
{
	emit layoutAboutToBeChanged();
	emit layoutChanged();
}

void TowerModel::updateSlot(TowerModel::UpdateType type)
{
	switch (type)
	{
	case UpdateType::add:
		this->insertRow(this->controller.size());
		break;
	case UpdateType::remove:
		this->removeRow(this->controller.size() - 1);
		break;
	case UpdateType::update:
		emit layoutAboutToBeChanged();
		emit layoutChanged();
		break;
	}
}