#include "imagepro.h"
#include <QFile>
#include <QDebug>

ImagePro::ImagePro(QObject *parent) : QObject(parent)
{

}

bool ImagePro::setPic(QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly)) {
        return false;
    }

    if(file.exists()) {
        src = new QImage(filename);
        dst = new QImage(*src);
        qDebug() << src->width() << src->height();
        if(src != nullptr)
        {
            emit showSrc(src);
            return true;
        }
    }
}

QImage *ImagePro::doProcess(Task t)
{
    switch (t)
    {
    case GRAY:
        toGray();
        break;
    case BINARY:
        toBinary();
        break;
    }
}

ImagePro::~ImagePro()
{
    if(src)
        delete src;
    qDebug() << "~ImageProc";
}

QImage* ImagePro::toGray()
{
    qDebug() << "to Gray";
    //灰度处理三种算法：平均值法，取最大值法，加权平均值法
    if(src == nullptr)
        return nullptr;
    if(src)
    {
        QColor color;
        //QRgb rgb;
        for(int row=0;row<src->height();row++)
        {
            for(int col=0;col<src->width();col++)
            {
                color = src->pixel(col,row);
                int mean = (color.red() + color.green() + color.blue())/3;
                color.setRgb(mean,mean,mean);
                dst->setPixel(col,row,color.rgb());
            }
        }
    }
    emit showDst(dst);
}

QImage* ImagePro::toBinary()
{
    if(src == nullptr)
        return nullptr;
    if(src)
    {
        QColor color;
        for(int row=0;row<src->height();row++)
        {
            for(int col=0;col<src->width();col++)
            {
                color = src->pixel(col,row);
                int mean = (color.red() + color.green() + color.blue())/3;
                if(mean > 100)
                {
                    color.setRgb(255,255,255);
                }
                else
                {
                    color.setRgb(0,0,0);
                }
                dst->setPixel(col,row,color.rgb());
            }
        }
    }
    emit showDst(dst);
}
