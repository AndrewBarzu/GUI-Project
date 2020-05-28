#include "GUICharter.h"
#include <unordered_map>
#include "setDebugNew.h"

#define new DEBUG_NEW

GUICharter::GUICharter()
{
	init();
}

void GUICharter::init()
{
	this->setRenderHint(QPainter::Antialiasing);
	this->chart = new QChart();
	this->setChart(this->chart);
	this->axisY = new QValueAxis(this->chart);
}

void GUICharter::chartSlot(const Controller& controller)
{
	std::unordered_map<std::string, int> chartTab;
	for (auto tower : controller.print())
	{
		if (chartTab.count(tower.get_size()) > 0)
			chartTab[tower.get_size()] += 1;
		else
		{
			chartTab[tower.get_size()] = 1;
		}
	}
	this->chart->removeAllSeries();
	this->chart->removeAxis(this->axisY);

	QBarSeries* series = new QBarSeries();
	int maxRange = 0, i = 0;
	for (auto el : chartTab)
	{
		QBarSet* set = new QBarSet(QString::fromStdString(el.first));
		*set << el.second;
		series->append(set);
		if (el.second > maxRange)
			maxRange = el.second;
	}
	this->chart->addSeries(series);
	this->chart->setTitle("Tower size chart");
	this->chart->setAnimationOptions(QChart::SeriesAnimations);
	this->chart->addAxis(this->axisY, Qt::AlignLeft);
	this->axisY->setRange(0, maxRange);
	this->axisY->applyNiceNumbers();
	this->chart->legend()->setVisible(true);
	this->chart->legend()->setAlignment(Qt::AlignBottom);
}
