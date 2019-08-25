//#include"FPchart.h"
//
//FPchart::FPchart(QWidget *parent) : QWidget(parent) {
//	ui.setupUi(this);
//	QSplineSeries *series = new QSplineSeries();
//	for (float i = 0; i < 100; i++) {
//		series->append(i, sin(0.5*i));
//	}
//	QChart *chart = new QChart();
//	chart->addSeries(series);
//	chart->legend()->hide();
//	chart->createDefaultAxes();
//	chart->setTheme(QChart::ChartThemeDark);
//	ui.widget->setChart(chart);
//}