#include "dstimagewidget.h"
#include "ui_dstimagewidget.h"
#include <QPainter>

DstImageWidget::DstImageWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::DstImageWidget)
{
    ui->setupUi(this);
    setFrameStyle(QFrame::Panel|QFrame::Sunken);
}

DstImageWidget::~DstImageWidget()
{
    delete ui;
}

void DstImageWidget::paintEvent(QPaintEvent *event)
{
    if(!_update)
        return;
    //qDebug() << "dst paintEvent";
    if(dst)
    {
        QPainter painter;
        painter.begin(this);
        painter.drawImage(QPoint(0,0),*dst);
        painter.end();
    }
    QFrame::paintEvent(event);
}

void DstImageWidget::showPic(QImage *p)
{
    dst = p;
    _update = true;
    repaint();
}

void DstImageWidget::disableUpdate(bool _update)
{
    this->_update = !_update;
}
