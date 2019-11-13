#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "srcimagewidget.h"
#include "dstimagewidget.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void createDockWidget();
    void paintEvent(QPaintEvent *event);

private slots:
    void on_actionOpen_Image_triggered();

    void on_actionGray_triggered();

    void on_actionBinary_triggered();

    void on_actionHis_triggered();

    void on_actionHistogram_Equalization_triggered();

    void on_actionGua_triggered();

    void on_actionsobel_triggered();

    void on_horizontal_triggered();

    void on_vertical_triggered();

    void on_actiondual_triggered();

    void on_actionswitch_triggered();

    void on_actionmid_triggered();

private:
    void createBarView(double *serial,int size);

private:
    SrcImageWidget *srcWidget;
    DstImageWidget *dstWidget;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
