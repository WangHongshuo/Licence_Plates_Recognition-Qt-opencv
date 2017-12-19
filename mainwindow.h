#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <opencv2/opencv.hpp>
#include <QString>
#include "lprecognizer.h"
#include "ImageWidget.h"
#include "filenameslistform.h"

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
    void start_process(QString &input_url);

    Ui::MainWindow *ui;
    cv::Mat mat_image;

    QImage input_img;
    QImage binary_img;
    QImage optimized_img;

    LPRecognizer recognizer;
    FileNamesListForm *url_list;
};

#endif // MAINWINDOW_H
