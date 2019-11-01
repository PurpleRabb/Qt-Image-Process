#ifndef SRCIMAGEWIDGET_H
#define SRCIMAGEWIDGET_H

#include <QWidget>
#include <QFrame>
#include <QDebug>

namespace Ui {
class SrcImageWidget;
}

class SrcImageWidget : public QFrame
{
    Q_OBJECT

public:
    explicit SrcImageWidget(QWidget *parent = nullptr);
    ~SrcImageWidget();
    void paintEvent(QPaintEvent *event) override;

private slots:
    void showPic(QImage *src);

private:
    Ui::SrcImageWidget *ui;
    QImage *src = nullptr;
};

#endif // SRCIMAGEWIDGET_H
