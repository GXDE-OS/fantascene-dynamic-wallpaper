#include "memorymonitorwidget.h"
#include <QVBoxLayout>
#include <QAreaSeries>
MemoryMonitorWidget::MemoryMonitorWidget(QWidget *parent) : QWidget(parent)
{
    m_monitor = SystemMonitor::getInstance();

//    setWindowFlags(Qt::WindowStaysOnTopHint | Qt::FramelessWindowHint | Qt::Tool);
//    setAttribute(Qt::WA_TranslucentBackground);

    // 创建内存图表
    m_memoryChart = new QChart();
    m_memoryChart->setTitle(QString(tr("Memory(%1)").arg(m_monitor->getMemoryInfo())));
    m_memoryChart->legend()->hide();
    // 确保图表背景透明
    m_memoryChart->setBackgroundVisible(false);
    m_memoryChart->setPlotAreaBackgroundVisible(false);

    m_memorySeries = new QLineSeries();
    //memoryChart->addSeries(memorySeries);
    m_memorySeries->append(0, 1);
    m_memorySeries->append(0, 2);
    // 创建面积系列，下边界默认为 0
    m_areaSeries = new QAreaSeries(m_memorySeries);
    m_memoryChart->addSeries(m_areaSeries);

    m_memoryAxisX = new QValueAxis();
    m_memoryAxisX->setRange(0, 60);
    m_memoryAxisX->setLabelFormat("%d");
    m_memoryAxisX->setTitleText(tr("Time (s)"));
    m_memoryAxisY = new QValueAxis();
    m_memoryAxisY->setRange(0, 100);

    m_memoryChart->addAxis(m_memoryAxisX, Qt::AlignBottom);
    m_memoryChart->addAxis(m_memoryAxisY, Qt::AlignLeft);


    m_areaSeries->attachAxis(m_memoryAxisX);
    m_areaSeries->attachAxis(m_memoryAxisY);

    m_memoryChartView = new QChartView(m_memoryChart);
    m_memoryChartView->setRenderHint(QPainter::Antialiasing);
    // 设置图表视图背景透明
    m_memoryChartView->setBackgroundBrush(Qt::transparent);
    m_memoryChartView->setFrameShape(QFrame::NoFrame);
    // 确保图表视图的视口背景透明
    m_memoryChartView->viewport()->setAutoFillBackground(false);
    m_memoryChartView->viewport()->setStyleSheet("background-color: transparent;");

    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(m_memoryChartView);

    connect(m_monitor, &SystemMonitor::memoryUsageChanged, this, &MemoryMonitorWidget::updateMemoryUsage);
    connect(m_monitor, &SystemMonitor::memoryUsageChanged, this, &MemoryMonitorWidget::updateMemoryTitle);
}

void MemoryMonitorWidget::updateMemoryUsage(const QVector<double> &usage)
{
    m_memorySeries->clear();
    for (int i = 0; i < usage.size(); ++i) {
        m_memorySeries->append(i, usage[i]);
        if(usage.size() > 0)
        {
            QString str = QString::number(usage.at(usage.size()-1),'f',1)+"%";
            m_memoryAxisX->setTitleText(str);
        }
    }
}

void MemoryMonitorWidget::updateMemoryTitle()
{
    m_memoryChart->setTitle(QString("Memory (%1)").arg(m_monitor->memoryInfo));
}
