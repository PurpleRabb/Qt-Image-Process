#ifndef IMAGEPRO_H
#define IMAGEPRO_H

#include <QObject>
#include <QImage>
#include <QImage>
#include "fouriertransform.h"

class ImagePro;

enum Task {
    GRAY,
    BINARY,
    HISTOGRAM,
    HISTOGRAM_EQUAL,
    GAUSSIAN_BLUR,
    H_SHARPEN,
    V_SHARPEN,
    DUAL_SHARPEN,
    SOBEL,
    MED_FILTER,
    SQUARE_MASK,
    FOURIER,
};

class ImagePro : public QObject
{
    Q_OBJECT

signals:
    void showSrc(QImage *);
    void showDst(QImage *);
    void disableUpdate(bool);

private:
    explicit ImagePro(QObject *parent = nullptr);
    ~ImagePro();

public:
    bool setPic(QString &filename);
    QImage *doProcess(Task t,bool flag=false,int value=100);
    static ImagePro *Instance()
    {
       static ImagePro IP;
       return &IP;
    }
    double* calHistogram();
    double* his_equal();
    void swap();

private:
    QImage* toGray();
    QImage* toBinary(int threshold=100);
    QImage* gaussian_blur();
    QImage* h_sharpen();
    QImage* v_sharpen();
    QImage* sobel(bool enhanced = false);
    QImage* dual_sharpen();
    QImage* med_filter();
    QImage* square_mask();
    void fourier();

public slots:

private:
    QImage *src = nullptr;
    QImage *dst = nullptr;
    double *histo = nullptr;
};

#endif // IMAGEPRO_H
