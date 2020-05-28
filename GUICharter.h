#pragma once

#include <QtCharts>
#include "controller.h"

class GUICharter: public QChartView
{
public:
	GUICharter();
private:

	QChart* chart;
	QValueAxis* axisY;

	void init();

public slots:
	void chartSlot(const Controller& controller);
};

