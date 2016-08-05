#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QUrl>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

#include "Object_management.h"
#include "CompareToPolygon.hpp"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:
    void init();

    void on_Btn_answer_folderUrl_clicked();
    void on_Btn_test_folderUrl_clicked();

    int scanFileList(QString strDir, QString filename);
    void on_btn_input_filename_answer_clicked();
    void on_btn_input_filename_test_clicked();
    void on_btn_start_marking_exam_clicked();

private:
    Ui::MainWindow *ui;

    CompareToPolygon  *compareToPolygon;
    int current_filenNumber;

    //Answer
    QUrl answer_folder_url;
    QString answer_main_Filename;
    int answer_file_totalCount;
    Object_management *answer_object_data;

    //Test
    QUrl test_folder_url;
    QString test_main_Filename;
    int test_file_totalCount;
    Object_management *test_object_data;
};

#endif // MAINWINDOW_H
