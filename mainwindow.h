#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include "lprecognizer.h"
#include "ImageWidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private slots:

    void on_open_img_clicked();

private:
    Ui::MainWindow *ui;
    cv::Mat image;

    QImage input_img;
    QImage binary_img;
    QImage optimized_img;

    LPRecognizer recognizer;
};

#endif // MAINWINDOW_H
