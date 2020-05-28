#include "Action.h"

void AddAction::doAction()
{
	this->repo->add(this->tower);
}

void AddAction::undoAction()
{
	this->repo->remove(this->tower.get_location());
}

void RemoveAction::doAction()
{
	this->repo->remove(this->tower.get_location());
	if (saved != nullptr)
		this->saved->remove(this->tower.get_location());
}

void RemoveAction::undoAction()
{
	this->repo->add(this->tower);
	if (saved != nullptr)
		this->saved->add(this->tower);
}

void UpdateAction::doAction()
{
	this->repo->update(this->newTower);
	if (this->saved != nullptr)
		this->saved->update(this->newTower);
}

void UpdateAction::undoAction()
{
	this->repo->update(oldTower);
	if (this->saved != nullptr)
		this->saved->update(oldTower);
}
