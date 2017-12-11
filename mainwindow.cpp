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
        a.start();

        recognizer.set_img(image);
//        qDebug() << a.elapsed();
        input_img = Mat2QImage(image);
        ui->show_input_img->set_image(&input_img);

        binary_img = Mat2QImage(recognizer.pre_processed_img);
        ui->show_binary_img->set_image(&binary_img);

        optimized_img = Mat2QImage(recognizer.fixed_img);
        ui->show_optimized_img->set_image(&optimized_img);

        for(int i = 0;i < 7;i++)
        {
            temp = Mat2QImage(recognizer.character[i]);
//            qDebug() << &temp << temp.data_ptr();
            ImageWidget* show_character = findChild<ImageWidget*>("show_character_"+QString::number(i+1));
            show_character->set_image(&temp);
        }

        ui->ans->setText(recognizer.recognized_Licence_Plate);
        qDebug() << a.elapsed();
    } 
}


