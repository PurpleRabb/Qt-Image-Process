#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "m_dialog.h"
#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QDockWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QFileDialog>
#include <QtCharts/QChartView>
#include <QtCharts/QBarSeries>
#include <QtCharts/QBarSet>
#include <QtCharts/QLegend>
#include <QtCharts/QBarCategoryAxis>
#include <QtCharts/QValueAxis>

#include "imagepro.h"

QT_CHARTS_USE_NAMESPACE

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setGeometry(300,300,800,600);
    createDockWidget();

//    QFile file("F:\\Code\\BiliCG\\BiliCG\\bmp256.bmp");
//    if (!file.open(QIODevice::ReadOnly))
//    {
//        qDebug() << "Open Error";
//    }
//    QDataStream in(&file);
//    in.setByteOrder(QDataStream::LittleEndian);
//    uchar header;
//    in >> header;
//    qDebug() << QString(header);
//    in >> header;
//    qDebug() << QString(header);
//    uint32_t size = 0;
//    in >> size;
//    qDebug() << size;
//    file.close();
    connect(ImagePro::Instance(),SIGNAL(showSrc(QImage *)),srcWidget,SLOT(showPic(QImage *)));
    connect(ImagePro::Instance(),SIGNAL(showDst(QImage *)),dstWidget,SLOT(showPic(QImage *)));
    connect(ImagePro::Instance(),SIGNAL(disableUpdate(bool)),dstWidget,SLOT(disableUpdate(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    qDebug() << "~MainWindow";
}

void MainWindow::paintEvent(QPaintEvent *event)
{

}

void MainWindow::createDockWidget()
{
    srcWidget = new SrcImageWidget(this);
    dstWidget = new DstImageWidget(this);
    QSplitter *qs = new QSplitter(this);
    qs->setOrientation(Qt::Horizontal);
    qs->addWidget(srcWidget);
    qs->addWidget(dstWidget);
    setCentralWidget(qs);
}


void MainWindow::on_actionOpen_Image_triggered()
{
    qDebug() << "Open file";
    QString fileName = QFileDialog::getOpenFileName(this,
        tr("Open file"), "F:\\Code\\web-spider-samples\\风景", tr("Image Files (*.png *.jpg *.bmp)"));
    qDebug() << fileName;
    if(!ImagePro::Instance()->setPic(fileName))
    {
        QDialog diag(this);
        diag.setWindowTitle("Open file failed!!!");
        diag.resize(QSize(300,30));
        diag.exec();
        return;
    }
}

void MainWindow::createBarView(double *serial,int size)
{
    if(serial == nullptr)
        return;
    QBarSet *set0 = new QBarSet("Histogram");
    set0->setColor(Qt::black);
    QStringList categories;
    volatile int value;
    for (int i=0;i<size;i++)
    {
        value = static_cast<int>(serial[i]*1000);
        *set0 << value;
        categories << QString(i);
    }

    QBarSeries *series = new QBarSeries();
    series->append(set0);


    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Histogram barchart");
    chart->setAnimationOptions(QChart::SeriesAnimations);

    QBarCategoryAxis *axisX = new QBarCategoryAxis();
    axisX->append(categories);
    chart->addAxis(axisX, Qt::AlignBottom);
    series->attachAxis(axisX);

    QValueAxis *axisY = new QValueAxis();
    axisY->setRange(0,1000);
    chart->addAxis(axisY, Qt::AlignLeft);
    series->attachAxis(axisY);


    chart->legend()->setVisible(true);
    chart->legend()->setAlignment(Qt::AlignBottom);


    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    chartView->resize(800, 300);
    chartView->show();
}

void MainWindow::on_actionGray_triggered()
{
    ImagePro::Instance()->doProcess(GRAY);
}

void MainWindow::on_actionBinary_triggered()
{
     M_Dialog md(this,false,true);
     md.exec();
     ImagePro::Instance()->doProcess(BINARY,false,md.getValue());
}

void MainWindow::on_actionHis_triggered()
{
    createBarView(ImagePro::Instance()->calHistogram(),256);
}

void MainWindow::on_actionHistogram_Equalization_triggered()
{
    createBarView(ImagePro::Instance()->his_equal(),256);
}

void MainWindow::on_actionGua_triggered()
{
    ImagePro::Instance()->doProcess(GAUSSIAN_BLUR);
}

void MainWindow::on_actionsobel_triggered()
{
    M_Dialog md(this,true,false);
    ImagePro::Instance()->doProcess(SOBEL,md.exec());
}

void MainWindow::on_horizontal_triggered()
{
    ImagePro::Instance()->doProcess(H_SHARPEN);
}

void MainWindow::on_vertical_triggered()
{
    ImagePro::Instance()->doProcess(V_SHARPEN);
}

void MainWindow::on_actiondual_triggered()
{
    ImagePro::Instance()->doProcess(DUAL_SHARPEN);
}

void MainWindow::on_actionswitch_triggered()
{
    ImagePro::Instance()->swap();
}
