#include "imagepro.h"
#include <QFile>
#include <QDebug>
#include <QVector2D>
#include <QtMath>

ImagePro::ImagePro(QObject *parent) : QObject(parent)
{

}

bool ImagePro::setPic(QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
    {
        return false;
    }

    if(file.exists()) {
        if(src == nullptr)
        {
            src = new QImage(filename);
            dst = new QImage(*src);
        }
        else
        {
            src->load(filename);
            emit disableUpdate(true);//不让dst跟着刷新
            dst->load(filename);
        }
        qDebug() << src->width() << src->height();
        qDebug() << dst->width() << dst->height();
        if(src != nullptr)
        {
            emit showSrc(src);
            qDebug() << "show src";
            return true;
        }
    }
    file.close();
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
    case GAUSSIAN_BLUR:
        gaussian_blur();
        break;
    case H_SHARPEN:
        h_sharpen();
        break;
    case V_SHARPEN:
        v_sharpen();
        break;
    case SOBEL:
        sobel();
        break;
    case DUAL_SHARPEN:
        dual_sharpen();
        break;
    }
    return dst;
}

ImagePro::~ImagePro()
{
    if(src)
        delete src;
    if(dst)
        delete dst;
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

QImage *ImagePro::gaussian_blur()
{
    /*采用7*7的高斯模板进行卷积*/
    static const int TEMPLATE_SIZE = 7;
    static const double gaussianTemplate[TEMPLATE_SIZE][TEMPLATE_SIZE] =
    {
        {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067},
        {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
        {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
        {0.00038771, 0.01330373, 0.11098164, 0.22508352, 0.11098164, 0.01330373, 0.00038771},
        {0.00019117, 0.00655965, 0.05472157, 0.11098164, 0.05472157, 0.00655965, 0.00019117},
        {0.00002292, 0.00078633, 0.00655965, 0.01330373, 0.00655965, 0.00078633, 0.00002292},
        {0.00000067, 0.00002292, 0.00019117, 0.00038771, 0.00019117, 0.00002292, 0.00000067}
    };

    if(src == nullptr)
        return nullptr;
    int width = src->width();
    int height = src->height();
    QRgb rgb;
    for(int row=TEMPLATE_SIZE/2;row<height;row++)
    {
        for(int col=TEMPLATE_SIZE/2;col<width;col++)
        {
            int t_red = 0;
            int t_green = 0;
            int t_blue = 0;
            for(int i=0;i<TEMPLATE_SIZE;i++)
            {
                for(int j=0;j<TEMPLATE_SIZE;j++)
                {
                    if(((col+j-TEMPLATE_SIZE/2)>=width) || ((row+i-TEMPLATE_SIZE/2) >= height))
                    {
                        continue;
                    }
                    rgb = src->pixel(col+j-TEMPLATE_SIZE/2,row+i-TEMPLATE_SIZE/2);
                    t_red += gaussianTemplate[i][j] * qRed(rgb);
                    t_green += gaussianTemplate[i][j] * qGreen(rgb);
                    t_blue += gaussianTemplate[i][j] * qBlue(rgb);
                }
            }
            dst->setPixel(col,row,qRgb(t_red,t_green,t_blue));
        }
    }
    emit showDst(dst);
    return dst;
}

QImage* ImagePro::h_sharpen()
{
    /***********************************
     * 水平边缘检测:
     *      g(x,y)=|f(x,y) - f(x-1,y)|
    ************************************/
    if(src == nullptr)
        return nullptr;
    int width = src->width();
    int height = src->height();
    QRgb rgb;
    QImage temp(*dst);
    toGray();
    temp.fill(QColor(255,255,255));
    for(int row=1;row<height;row++)
    {
        for(int col=1;col<width;col++)
        {
            rgb = qAbs(dst->pixel(col,row) - dst->pixel(col-1,row));
            temp.setPixel(col,row,rgb);
        }
    }
    *dst = temp.copy(temp.rect());
    emit showDst(dst);
    return dst;
}

QImage* ImagePro::v_sharpen()
{
    /***********************************
     * 垂直边缘检测:
     *      g(x,y)=|f(x,y) - f(x,y-1)|
    ************************************/
    if(src == nullptr)
        return nullptr;
    int width = src->width();
    int height = src->height();
    QRgb rgb;
    QImage temp(*dst);
    toGray();
    temp.fill(QColor(255,255,255));
    for(int row=1;row<height;row++)
    {
        for(int col=1;col<width;col++)
        {
            rgb = qAbs(dst->pixel(col,row) - dst->pixel(col,row-1));
            temp.setPixel(col,row,rgb);
        }
    }
    *dst = temp.copy(temp.rect());
    emit showDst(dst);
    return dst;
}

QImage* ImagePro::dual_sharpen()
{
    /***********************************
     * 双向边缘检测:
     *      g(x,y)=sqt[(f(x,y) - f(x-1,y))^2 + (f(x,y) - f(x,y-1))^2]
    ************************************/
    if(src == nullptr)
        return nullptr;
    int width = src->width();
    int height = src->height();
    QImage temp(*dst);
    toGray();
    temp.fill(QColor(255,255,255));
    for(int row=1;row<height;row++)
    {
        for(int col=1;col<width;col++)
        {
            int delta_x = qRed(dst->pixel(col,row)) - qRed(dst->pixel(col-1,row));
            int delta_y = qRed(dst->pixel(col,row)) - qRed(dst->pixel(col,row-1));
            int delta = qCeil(qSqrt(delta_x*delta_x+delta_y*delta_y));
            temp.setPixelColor(col,row,QColor(delta,delta,delta));
        }
    }
    *dst = temp.copy(temp.rect());
    emit showDst(dst);
    return dst;
}

QImage* ImagePro::sobel()
{
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
       if(histo[i] < 0.001)
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
       if(histo[i] < 0.001)
       {
           histo[i] = 0.0;
       }
    }
    return histo;
}
