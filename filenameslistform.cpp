#include "filenameslistform.h"
#include "ui_filenameslistform.h"
#include <QFileInfoList>
#include <QListView>
#include <QListWidgetItem>
#include <QDebug>

FileNamesListForm::FileNamesListForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FileNamesListForm)
{
    ui->setupUi(this);
    ui->show_filenames_list->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

FileNamesListForm::~FileNamesListForm()
{
    delete ui;
}

void FileNamesListForm::set_list(QStringList filenames_list)
{
    url_list = filenames_list;
    QFileInfo name;
    for(int i=0;i<url_list.length();i++)
    {
        name.setFile(url_list[i]);
        url_list[i] = name.fileName();
    }
    ui->show_filenames_list->addItems(url_list);
}

void FileNamesListForm::on_shift_up_clicked()
{

}

void FileNamesListForm::on_shift_down_clicked()
{

}

void FileNamesListForm::on_clear_clicked()
{
    ui->show_filenames_list->clear();
}

void FileNamesListForm::on_delete_this_clicked()
{

}

void FileNamesListForm::on_show_filenames_list_doubleClicked(const QModelIndex &index)
{
    qDebug() << index.row();
}
