#ifndef DSTIMAGEWIDGET_H
#define DSTIMAGEWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QDebug>

namespace Ui {
class DstImageWidget;
}

class DstImageWidget : public QFrame
{
    Q_OBJECT

public:
    explicit DstImageWidget(QWidget *parent = nullptr);
    ~DstImageWidget();
    void paintEvent(QPaintEvent *event) override;

private slots:
    void showPic(QImage *src);
    void disableUpdate(bool _update);

private:
    Ui::DstImageWidget *ui;
    QImage *dst = nullptr;
    bool _update = true;
};

#endif // DSTIMAGEWIDGET_H
