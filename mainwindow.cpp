#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <QString>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextCodec>
#include <QLabel>
#include <QDebug>
#include <QTime>

#include <assert.h>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include "mat_qimage_convert.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ImageWidget* show_character;
    for(int i = 0;i < 7;i++)
    {
        show_character = findChild<ImageWidget*>("show_character_"+QString::number(i+1));
        show_character->only_show_image(true);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_open_img_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this,tr("Open Image"),"",tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    QTextCodec *code = QTextCodec::codecForName("gb18030");
    std::string name = code->fromUnicode(filename).data();
    image = imread(name);
    if(!image.data)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else
    {
        QTime a;

        recognizer.set_img(image);
        a.start();
//        qDebug() << image.channels();
        input_img = Mat2QImage(image);
        qDebug() << a.elapsed();
        ui->show_input_img->set_image_with_pointer(&input_img);

        binary_img = Mat2QImage(recognizer.pre_processed_img);
        ui->show_binary_img->set_image_with_pointer(&binary_img);

        optimized_img = Mat2QImage(recognizer.fixed_img);
        ui->show_optimized_img->set_image_with_pointer(&optimized_img);
        QImage temp;
        ImageWidget* show_character;
        QLabel* show_corr2_value;
        for(int i = 0;i < 7;i++)
        {
            temp = Mat2QImage(recognizer.character[i]);
//            qDebug() << &temp << temp.data_ptr();
            show_character = findChild<ImageWidget*>("show_character_"+QString::number(i+1));
            show_character->set_image_with_data(temp);
            show_corr2_value = findChild<QLabel*>("show_correlation_value_"+QString::number(i+1));
            show_corr2_value->setText(QString::number(recognizer.correlation_value[i]));
            show_character->only_show_image(true);
        }
        ui->ans->setText(recognizer.recognized_Licence_Plate);
        qDebug() << a.elapsed();
    } 
}


