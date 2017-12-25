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
#include "filenameslistform.h"

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
    filenames.clear();
    filenames = QFileDialog::getOpenFileNames(this,tr("Open Image"),
                                                    "",
                                                    tr("Image File(*.bmp *.jpg *.jpeg *.png)"));
    int input_url_count = filenames.length();
    if(input_url_count == 0)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else
        start_process(filenames[0]);

    if(!url_list_dialog)
        url_list_dialog = new FileNamesListForm(this);
    if(input_url_count > 1)
    {
        url_list_dialog->set_list(filenames);
        url_list_dialog->setGeometry(this->x()+this->width()-360,this->y(),350,210);
        url_list_dialog->show();
        connect(url_list_dialog,SIGNAL(send_url(QString)),this,SLOT(receive_url(QString)));
    }
    else
    {
        url_list_dialog->close();
        delete url_list_dialog;
        url_list_dialog = NULL;
    }

}

void MainWindow::receive_url(QString url)
{
    start_process(url);
}

void MainWindow::start_process(const QString &input_url)
{
    if (input_url.isNull()||input_url == "")
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Image data is null!"));
        msgBox.exec();
    }
    else
    {
        input_img.load(input_url);
        QTime a;
        a.start();

        mat_image = QImage2Mat_with_pointer(input_img);

        recognizer.set_img(mat_image);

        ui->show_input_img->set_image_with_pointer(&input_img);

        binary_img = Mat2QImage_with_pointer(recognizer.pre_processed_img);
        ui->show_binary_img->set_image_with_pointer(&binary_img);

        optimized_img = Mat2QImage_with_pointer(recognizer.fixed_img);
        ui->show_optimized_img->set_image_with_pointer(&optimized_img);
        QImage temp;
        ImageWidget* show_character = NULL;
        QLabel* show_corr2_value;
        for(int i = 0;i < 7;i++)
        {
            temp = Mat2QImage_with_pointer(recognizer.character[i]);
//            qDebug() << &temp << temp.data_ptr();
            show_character = findChild<ImageWidget*>("show_character_"+QString::number(i+1));
            show_character->set_image_with_data(temp);
            show_corr2_value = findChild<QLabel*>("show_correlation_value_"+QString::number(i+1));
            show_corr2_value->setText(QString::number(recognizer.correlation_value[i]));
        }
        ui->ans->setText(recognizer.recognized_Licence_Plate);
        qDebug() << a.elapsed();
    }
}


