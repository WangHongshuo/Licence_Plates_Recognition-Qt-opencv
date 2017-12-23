#ifndef FILENAMESLISTFORM_H
#define FILENAMESLISTFORM_H

#include <QDialog>
#include <QStringListModel>
#include <QStringList>
#include <QListWidgetItem>
#include <QTableWidget>

namespace Ui {
class FileNamesListForm;
}

class FileNamesListForm : public QDialog
{
    Q_OBJECT

public:
    explicit FileNamesListForm(QWidget *parent = 0);
    ~FileNamesListForm();
    void set_list(QStringList input_list);

signals:
    void send_url(QString);

private slots:
    void on_shift_up_clicked();

    void on_shift_down_clicked();

    void on_clear_clicked();

    void on_delete_this_clicked();

    void on_show_filenames_list_cellClicked(int row, int column);

    void on_show_filenames_list_cellDoubleClicked(int row, int column);

private:

    Ui::FileNamesListForm *ui;
    QStringList filename_list;
    QStringList url_list;
    int selected_filename_index = -1;
};

#endif // FILENAMESLISTFORM_H
