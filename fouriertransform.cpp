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
     * F(u,v)= sum(0,M-1) sum (0,N-1) f(x,y)e^[-j2*PI*(ux/M+vy/N)]
     * 欧拉公式e^(jx)=cosx + jsinx
     * 傅里叶谱 |F(u,v)| = sqt([R(u,v)^2 + I(u,v)^2])
     * 功率谱 F(u,v)=|F(u,v)|^2
     * 相角 argtan[I(u,v)/R(u,v)]
     ****************************************************************************/
    int width = image->width();
    int height = image->height();
    double im,exp,re;
    double to_center = 0; //中心化频谱

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            re = 0;
            im = 0;

            for (int x = 0; x < height; x++){
                for (int y = 0; y < width; y++){
                    exp = (double)i * x / (double)height +
                            (double)j * y / (double)width;
                    to_center = (x + y) % 2 == 0 ? 1 : -1; //中心化图谱
                    QColor color = image->pixel(y,x);
                    int mean = (color.red() + color.green() + color.blue())/3;
                    re += mean * cos(-2 * M_PI * exp) * to_center;
                    im += mean * sin(-2 * M_PI * exp) * to_center;
                }
            }

            real_array[j][i] = re;
            im_array[j][i] = im;
        }
    }

    for(int u=0;u<width;u++)
    {
        for(int v=0;v<height;v++)
        {
            double res = sqrt(real_array[u][v] * real_array[u][v] + im_array[u][v] * im_array[u][v])/sqrt(width*height);
            dst->setPixel(u,v,qRgb(static_cast<int>(res),static_cast<int>(res),static_cast<int>(res)));
        }
    }
    qDebug() << "###################";
    inv_dft(dst);
}


void FourierTransform::inv_dft(QImage *dst)
{
    //反傅里叶变换
    //f(x,y) = 1/MN sum(0,M-1)sum(0,N-1)F(u,v)e^[j*2*PI(ux/M+vy/N)]
    double real, exp;
    int to_center;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            real = 0;
            for (int x = 0; x < height; x++){
                for (int y = 0; y < width; y++){
                    exp = (double)i * x / (double)height +
                            (double)j * y / (double)width;
                    real += real_array[y][x] * cos(2 * M_PI * exp) -
                            im_array[y][x] * sin(2 * M_PI * exp);
                }
            }
            to_center = (i + j) % 2 == 0 ? 1 : -1;
            short res = (short)(real / (width*height) * to_center);
            dst->setPixel(j,i,qRgb(res,res,res));
        }
    }
}
