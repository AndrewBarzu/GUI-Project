#pragma once
#include "repository.h"
#include "domain.h"

class Action
{
public:
	virtual void doAction() = 0;
	virtual void undoAction() = 0;
	virtual ~Action() {}
};

class AddAction :
	public Action
{
private:
	RepoInterface* repo;
	Tower tower;
public:
	AddAction(RepoInterface* repo, Tower tower) : repo{ repo }, tower{ tower } {}
	AddAction(const AddAction& other) : repo{ other.repo }, tower{ other.tower } {}
	void doAction() override;
	void undoAction() override;
	~AddAction() {}
};


class RemoveAction :
	public Action
{
private:
	RepoInterface* repo;
	RepoInterface* saved;
	Tower tower;
public:
	RemoveAction(RepoInterface* repo, Tower tower, RepoInterface* saved = nullptr) : repo{ repo }, saved{ saved }, tower{ tower } {}
	RemoveAction(const RemoveAction& other) : repo{ other.repo }, saved{ other.saved }, tower{ other.tower } {}
	void doAction() override;
	void undoAction() override;
	~RemoveAction() {}
};

class UpdateAction :
	public Action
{
private:
	RepoInterface* repo;
	RepoInterface* saved;
	Tower oldTower;
	Tower newTower;
public:
	UpdateAction(RepoInterface* repo, Tower oldTower, Tower newTower, RepoInterface* saved = nullptr) : repo{ repo }, saved{ saved }, oldTower{ oldTower }, newTower{ newTower } {}
	UpdateAction(const UpdateAction& other) : repo{ other.repo }, saved{ other.saved }, oldTower{ other.oldTower }, newTower{ other.newTower } {}
	void doAction() override;
	void undoAction() override;
	~UpdateAction() {}
};