#include "srcimagewidget.h"
#include "ui_srcimagewidget.h"
#include <QPainter>

SrcImageWidget::SrcImageWidget(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::SrcImageWidget)
{
    ui->setupUi(this);
    setFrameStyle(QFrame::Panel|QFrame::Sunken);
}

SrcImageWidget::~SrcImageWidget()
{
    delete ui;
}

void SrcImageWidget::paintEvent(QPaintEvent *event)
{
    //qDebug() << "srcImage paint";
    if(src)
    {
        QPainter painter;
        painter.begin(this);
        src->scaled(this->width(),this->height());
        painter.drawImage(QPoint(0,0),*src);
        painter.end();
    }
    QFrame::paintEvent(event);
}

void SrcImageWidget::showPic(QImage *p)
{
    src = p;
    update();
}
