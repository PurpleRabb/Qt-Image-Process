#ifndef FOURIERTRANSFORM_H
#define FOURIERTRANSFORM_H

#include <QObject>
#include <QImage>

class FourierTransform : public QObject
{
    Q_OBJECT
public:
    explicit FourierTransform(QObject *parent = nullptr,QImage *image = nullptr);
    ~FourierTransform();
    void dft(QImage *image,QImage *dst);

signals:

public slots:

private:
    double **real_array; //实部
    double **im_array; //虚部
    int width,height;
};

#endif // FOURIERTRANSFORM_H
