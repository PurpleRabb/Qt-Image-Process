#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QPainter>
#include <QDebug>
#include <QFile>
#include <QDockWidget>
#include <QTextEdit>
#include <QSplitter>
#include <QFileDialog>
#include "imagepro.h"

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

void MainWindow::on_actionGray_triggered()
{
    ImagePro::Instance()->doProcess(GRAY);
}

void MainWindow::on_actionBinary_triggered()
{
     ImagePro::Instance()->doProcess(BINARY);
}
