#include "imagepro.h"
#include <QFile>
#include <QDebug>
#include <QVector2D>

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
    case HISTOGRAM:
        calHistogram();
        break;
    default:
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

float* ImagePro::calHistogram()
{
    if(src == nullptr)
        return nullptr;
    toGray();
    if(histo == nullptr)
        histo = new float[256];
    memset(histo,0,256);
    if(src)
    {
        QColor color;
        for(int row=0;row<dst->height();row++)
        {
            for(int col=0;col<dst->width();col++)
            {
                color = dst->pixel(col,row);
                histo[color.red()]++;
            }
        }
    }
    float max = 0.0;
    float min = 0.0;
    for(int i=0;i<256;i++)
    {
        if(histo[i] > max)
            max = histo[i];
        if(histo[i] < min)
            min = histo[i];
    }
    qDebug() << max << min;
    for(int i=0;i<256;i++)
    {
       histo[i] = histo[i]/(max-min);//normalize
       if(histo[i] < 0)
           histo[i] = 0;
    }
    return histo;
}
