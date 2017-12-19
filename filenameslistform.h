#ifndef FILENAMESLISTFORM_H
#define FILENAMESLISTFORM_H

#include <QDialog>
#include <QStringListModel>
#include <QStringList>
#include <QListWidgetItem>

namespace Ui {
class FileNamesListForm;
}

class FileNamesListForm : public QDialog
{
    Q_OBJECT

public:
    explicit FileNamesListForm(QWidget *parent = 0);
    ~FileNamesListForm();
    void set_list(QStringList filenames_list);

private slots:
    void on_shift_up_clicked();

    void on_shift_down_clicked();

    void on_clear_clicked();

    void on_delete_this_clicked();

    void on_show_filenames_list_doubleClicked(const QModelIndex &index);

private:
    Ui::FileNamesListForm *ui;
    QStringList url_list;
};

#endif // FILENAMESLISTFORM_H
