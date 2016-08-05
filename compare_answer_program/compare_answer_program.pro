#-------------------------------------------------
#
# Project created by QtCreator 2016-08-02T09:31:01
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = compare_answer_program
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    Shape_configuration.cpp \
    Object_management.cpp \
    clipper.cpp \
    CompareToPolygon.cpp

HEADERS  += mainwindow.h \
    Shape_configuration.h \
    Object_management.h \
    clipper.hpp \
    CompareToPolygon.hpp

FORMS    += mainwindow.ui
