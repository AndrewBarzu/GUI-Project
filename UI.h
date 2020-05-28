#pragma once
#include "controller.h"

class UI
{
private:
	Controller controller;
public:
	UI(Controller controller) : controller{ controller } {}
	void start();
};

