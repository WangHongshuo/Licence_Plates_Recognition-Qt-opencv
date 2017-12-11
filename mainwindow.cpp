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
        img = Mat2QImage(image);
        img = img.scaled(ui->input_image->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        //ui->input_image->setScaledContents(true);
        ui->input_image->setAlignment(Qt::AlignCenter);
        ui->input_image->clear();
        ui->input_image->setPixmap(QPixmap::fromImage(img));
        ui->input_image->resize(ui->input_image->pixmap()->size());

        img = Mat2QImage(recognizer.pre_processed_img);
        img = img.scaled(ui->ori_binarization->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        //ui->input_image->setScaledContents(true);
        ui->ori_binarization->setAlignment(Qt::AlignCenter);
        ui->ori_binarization->clear();
        ui->ori_binarization->setPixmap(QPixmap::fromImage(img));
        ui->ori_binarization->resize(ui->ori_binarization->pixmap()->size());


        img = Mat2QImage(recognizer.fixed_img);
        img = img.scaled(ui->binarization->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
        //ui->input_image->setScaledContents(true);
        ui->binarization->setAlignment(Qt::AlignCenter);
        ui->binarization->clear();
        ui->binarization->setPixmap(QPixmap::fromImage(img));
        ui->binarization->resize(ui->binarization->pixmap()->size());


        for(int i = 0;i < 7;i++)
        {
            img = Mat2QImage(recognizer.character[i]);
            QLabel *label = findChild<QLabel*>("c"+QString::number(i+1));
            img = img.scaled(label->size(),Qt::KeepAspectRatio,Qt::SmoothTransformation);
            label->setAlignment(Qt::AlignCenter);
            label->clear();
            label->setPixmap(QPixmap::fromImage(img));
            label->resize(ui->binarization->pixmap()->size());
        }

        ui->ans->setText(recognizer.recognized_Licence_Plate);
        qDebug() << a.elapsed();
    } 
}


