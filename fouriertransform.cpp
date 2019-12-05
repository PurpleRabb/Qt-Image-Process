#include "fouriertransform.h"
#include <QtMath>
#include <QDebug>

FourierTransform::FourierTransform(QObject *parent,QImage *image) : QObject(parent)
{
    width = image->width();
    height = image->height();
    real_array = new double*[width];
    for(int i=0;i<width;i++)
        real_array[i] = new double[height];

    im_array = new double*[width];
    for(int i=0;i<width;i++)
        im_array[i] = new double[height];
}

FourierTransform::~FourierTransform()
{
    for(int i=0;i<width;i++)
            delete []real_array[i];
        delete []real_array;

    for(int i=0;i<width;i++)
            delete []im_array[i];
        delete []im_array;
}

void FourierTransform::dft(QImage *image,QImage *dst)
{
    /*****************************************************************************
     * 暴力实现傅里叶变换(未中心化),速度很慢
     * F(u,v)=[1/squre(MN)] sum(0,M-1) sum (0,N-1) f(x,y)e^[-j2*PI*(ux/M+vy/N)]
     * 欧拉公式e^(jx)=cosx + jsinx
     * 傅里叶谱 |F(u,v)| = sqt([R(u,v)^2 + I(u,v)^2])
     * 功率谱 F(u,v)=|F(u,v)|^2
     * 相角 argtan[I(u,v)/R(u,v)]
     ****************************************************************************/
    int width = image->width();
    int height = image->height();
    double real,im,exp;
    double to_center = 0; //中心化频谱
    for(int u=0;u<width;u++)
    {
        for(int v=0;v<height;v++)
        {
            real = 0.0;
            im = 0.0;
            for(int x=0;x<width;x++)
            {
                for(int y=0;y<height;y++)
                {
                    exp = -2.0*M_PI*((double)u*x/(double)width + (double)v*y/(double)height);
                    //to_center = (x + y) % 2 == 0 ? 1 : -1;
                    to_center = 1;
                    real += image->pixel(x,y) * to_center * qCos(exp);
                    im += image->pixel(x,y) * to_center * qSin(exp);
                }
            }
            real_array[u][v] = real;
            im_array[u][v] = im;
        }
    }

    for(int u=0;u<width;u++)
    {
        for(int v=0;v<height;v++)
        {
            double res = sqrt(real_array[u][v] * real_array[u][v] + im_array[u][v] * im_array[u][v]);
            res = res/256.0/(width*height);
            dst->setPixel(u,v,qRgb(static_cast<int>(res),static_cast<int>(res),static_cast<int>(res)));
        }
    }
    inv_dft(dst);
}


void FourierTransform::inv_dft(QImage *dst)
{
    //反傅里叶变换
    //f(x,y) = 1/sqt(MN) sum(0,M-1)sum(0,N-1)F(u,v)e^[j*2*PI(ux/M+vy/N)]
    double real,temp;
    for(int u=0;u<width;u++)
    {
        for(int v=0;v<height;v++)
        {
            for(int x=0;x<width;x++)
            {
                for(int y=0;y<height;y++)
                {
                    temp = (double)u*x/(double)width +
                           (double)v*y/(double)height;
                    real += real_array[x][y] * qCos(2*M_PI*temp) -
                            im_array[x][y] * qSin(2*M_PI*temp);
                }
            }
            qDebug() << (uint)(real / sqrt(width*height));
            dst->setPixel(u,v,(uint)(real / sqrt(width*height)));
        }
    }
}
