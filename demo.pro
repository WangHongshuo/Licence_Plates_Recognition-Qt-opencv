#-------------------------------------------------
#
# Project created by QtCreator 2016-11-04T21:58:09
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = demo
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lprecognizer.cpp \
    mat_qimage_convert.cpp \
    ImageWidget.cpp \
    selectrect.cpp \
    filenameslistform.cpp

INCLUDEPATH += D:/opencv3.3.1/opencv_use/include\
               D:/opencv3.3.1/opencv_use/include/opencv\
               D:/opencv3.3.1/opencv_use/include/opencv2
LIBS += -L D:/opencv3.3.1/opencv_use/lib/libopencv_*.a


HEADERS  += mainwindow.h \
    lprecognizer.h \
    mat_qimage_convert.h \
    ImageWidget.h \
    selectrect.h \
    filenameslistform.h

FORMS    += mainwindow.ui \
    filenameslistform.ui

