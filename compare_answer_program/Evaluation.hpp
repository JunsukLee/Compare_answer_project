#ifndef EVALUATION_H
#define EVALUATION_H

#include <QUrl>
#include <QString>

#include "Object_management.h"
#include "CompareToPolygon.hpp"

class Evalutaion
{
public:
    void set_Evalutaion_data(QUrl answer_folder_url, QString answer_main_Filename, QUrl test_folder_url, QString test_main_Filename);
    void start_Evaluation();

public: //get() set()

private:


private:
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

    const double PERCENTILE = 100.0;
};


#endif // EVALUATION_H
