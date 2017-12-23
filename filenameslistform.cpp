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

void FileNamesListForm::set_list(QStringList input_list)
{
    filename_list.clear();
    url_list.clear();

    filename_list = url_list = input_list;

    QFileInfo name;
    for(int i=0;i<filename_list.length();i++)
    {
        name.setFile(filename_list[i]);
        filename_list[i] = name.fileName();
    }

    ui->show_filenames_list->setRowCount(filename_list.length());
    ui->show_filenames_list->setColumnCount(1);
    for(int i=0;i<filename_list.length();i++)
    {
        QTableWidgetItem* item = new QTableWidgetItem(filename_list[i]);
        ui->show_filenames_list->setItem(i,0,item);
    }
}

void FileNamesListForm::on_shift_up_clicked()
{
    if(selected_filename_index > 0)
    {
        filename_list.move(selected_filename_index,selected_filename_index-1);
        url_list.move(selected_filename_index,selected_filename_index-1);
        ui->show_filenames_list->item(selected_filename_index,0)->setText(filename_list.at(selected_filename_index));
        ui->show_filenames_list->item(selected_filename_index-1,0)->setText(filename_list.at(selected_filename_index-1));
        selected_filename_index -= 1;
        ui->show_filenames_list->selectRow(selected_filename_index);
    }
}

void FileNamesListForm::on_shift_down_clicked()
{
    if(selected_filename_index < filename_list.length()-1 && selected_filename_index > -1)
    {
        filename_list.move(selected_filename_index,selected_filename_index+1);
        url_list.move(selected_filename_index,selected_filename_index+1);
        ui->show_filenames_list->item(selected_filename_index,0)->setText(filename_list.at(selected_filename_index));
        ui->show_filenames_list->item(selected_filename_index+1,0)->setText(filename_list.at(selected_filename_index+1));
        selected_filename_index += 1;
        ui->show_filenames_list->selectRow(selected_filename_index);
    }
}

void FileNamesListForm::on_clear_clicked()
{
    ui->show_filenames_list->clearContents();
    filename_list.clear();
    url_list.clear();
    selected_filename_index = -1;
    ui->show_filenames_list->setRowCount(0);
}

void FileNamesListForm::on_delete_this_clicked()
{
    filename_list.removeAt(selected_filename_index);
    url_list.removeAt(selected_filename_index);
    ui->show_filenames_list->removeRow(selected_filename_index);
    selected_filename_index = -1;
}

void FileNamesListForm::on_show_filenames_list_cellClicked(int row, int column)
{
    selected_filename_index = row;
}

void FileNamesListForm::on_show_filenames_list_cellDoubleClicked(int row, int column)
{
    emit send_url(url_list.at(row));
}

