#pragma once
#include <QWidget>
#include <QImage>
#include <QPainter>
#include <QWheelEvent>
#include <QMenu>
#include <QCursor>
#include <QFileDialog>
#include <QSplitter>
#include <QMessageBox>
#include <QDebug>
#include "selectrect.h"

enum MouseDown{Left,Mid,Right,No};

class ImageWidget :
	public QWidget
{
	Q_OBJECT
public:
	ImageWidget(QWidget *parent);
	~ImageWidget();

    void set_image(QImage* img, bool clone_image = false, bool is_default = true)
	{
        if(clone_image)
        {
            mp_img = new QImage;
            qDebug() << "12121";
            qDebug() << mp_img << img;
            *mp_img = img->copy(0,0,img->width(),img->height());
            qDebug() << "12121";
        }
        else
            mp_img = img;
        is_image_load = true;
        if(is_default)
        {
            scalex = scaley = 1.0;
            xtranslate = ytranslate = 0;
            last_x_pos = last_y_pos = 0;
        }
        mouse = No;
		update();
	}

	void clear()
	{
        if(is_image_load)
		{
            is_image_load = false;
			//delete mp_img;
            mp_img = NULL;
            update();
		}
	}
    void only_show_image(bool flag = false);


public slots:

private slots:
    void reset_image();
    void save();
    void select();
    void is_select_mode_exit();

private:

    void zoom_out();
    void zoom_in();
    void translate(int x,int y);

    void create_contextmenu();
    void wheelEvent(QWheelEvent *e);
    void mouseMoveEvent(QMouseEvent * e);
    void mousePressEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void contextMenuEvent(QContextMenuEvent *e);

    QImage *mp_img;
    double scalex;
    double scaley;
	int xtranslate;
	int ytranslate;
    int last_x_pos = 0;
    int last_y_pos = 0;

	int mousePosX;
	int mousePosY;
    bool is_image_load = false;
    bool is_select_mode = false;
    bool is_only_show_image = false;

	MouseDown mouse;
	
	QAction* mActionResetPos;
	QAction* mActionSave;
    QAction* mActionSelect;

	QMenu* mMenu;

};
