#ifndef IMAGEPRO_H
#define IMAGEPRO_H

#include <QObject>
#include <QImage>
#include <QImage>

class ImagePro;

enum Task {
    GRAY,
    BINARY,
    HISTOGRAM,
    HISTOGRAM_EQUAL,
};

class ImagePro : public QObject
{
    Q_OBJECT

signals:
    void showSrc(QImage *);
    void showDst(QImage *);

private:
    explicit ImagePro(QObject *parent = nullptr);
    ~ImagePro();

public:
    bool setPic(QString &filename);
    QImage *doProcess(Task t);
    static ImagePro *Instance()
    {
       static ImagePro IP;
       return &IP;
    }
    float* calHistogram();
    float* his_equal();

private:
    QImage* toGray();
    QImage* toBinary();


public slots:

private:
    QImage *src = nullptr;
    QImage *dst = nullptr;
    float *histo = nullptr;
};

#endif // IMAGEPRO_H
