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
    return false;
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
    case HISTOGRAM_EQUAL:
        his_equal();
        break;
    }
    return dst;
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
        emit showDst(dst);
    }
    return dst;
}

QImage* ImagePro::toBinary()
{
    if(src == nullptr)
        return nullptr;
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
    emit showDst(dst);
    return dst;
}

float* ImagePro::calHistogram()
{
    if(src == nullptr)
        return nullptr;
    toGray();
    if(histo == nullptr)
        histo = new float[256];
    memset(histo,0,256);

    QColor color;
    for(int row=0;row<dst->height();row++)
    {
        for(int col=0;col<dst->width();col++)
        {
            color = dst->pixel(col,row);
            histo[color.red()]++;
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
    //qDebug() << max << min;
    for(int i=0;i<256;i++)
    {
       histo[i] = (histo[i]-min)/(max-min);//normalize
       if(histo[i] < 0)
       {
           histo[i] = 0.0;
       }
    }
    //归一化还可以用灰度出现的次数除以像素总数
    return histo;
}

float *ImagePro::his_equal()
{
    if(src == nullptr)
        return nullptr;
    toGray();
    if(histo == nullptr)
        histo = new float[256];
    memset(histo,0,256);
    /************************
      算法步骤：
        1.先计算每个像素灰度出现的概率(灰度直方图)
        2.计算累积概率(累积直方图)
        3.灰度级（255）乘以累积概率，得出灰度值
        4.按照映射表改变源像素的灰度值
    *************************/
    QColor color;
//[1]
    int size = dst->height() * dst->width();
    for(int row=0;row<dst->height();row++)
    {
        for(int col=0;col<dst->width();col++)
        {
            color = dst->pixel(col,row);
            histo[color.red()]++;
        }
    }
    for(int i=0;i<256;i++)
    {
       histo[i] = histo[i]/size;//normalize
       if(histo[i] < 0)
       {
           histo[i] = 0.0;
       }
       //qDebug() << histo[i];
    }
//[1]
//[2]
    float cdf[255] = {0};
    cdf[0] = histo[0];
    for(int i=1;i<256;i++)
    {
       cdf[i] = histo[i] + cdf[i-1];
       //qDebug() << cdf[i];
    }
//[2]
//[3]
    int gray_table_equal[255] = {0};
    for(int i=0;i<256;i++)
    {
       gray_table_equal[i] = static_cast<int>(255 * cdf[i]);
       //qDebug() << gray_table_equal[i];
    }
//[3]
    //[4]
    for(int row=0;row<dst->height();row++)
    {
        for(int col=0;col<dst->width();col++)
        {
            color = dst->pixel(col,row);
            color.setRgb(gray_table_equal[color.red()],gray_table_equal[color.red()],gray_table_equal[color.red()]);
            dst->setPixel(col,row,color.rgb());
        }
    }
    //[4]

    emit showDst(dst);

    //灰度出现的次数除以像素总数归一化直方图
    for(int row=0;row<dst->height();row++)
    {
        for(int col=0;col<dst->width();col++)
        {
            color = dst->pixel(col,row);
            histo[color.red()]++;
        }
    }
    for(int i=0;i<256;i++)
    {
       histo[i] = (histo[i]/size)*100;//normalize
       if(histo[i] < 0)
       {
           histo[i] = 0.0;
       }
    }
    return histo;
}
