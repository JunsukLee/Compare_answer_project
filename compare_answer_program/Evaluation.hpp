#ifndef EVALUATION_H
#define EVALUATION_H

#include <QUrl>
#include <QString>
#include <string>

#include "Object_management.h"
#include "CompareToPolygon.hpp"

#define PERCENTILE               100.0
#define TYPENAME_COUNT_MAXSIZE   10

class Evaluation
{

public:
    Evaluation();
    ~Evaluation();
    void init();    
    bool start_Evaluation(QUrl answer_folder_url, QString answer_main_Filename, QUrl test_folder_url, QString test_main_Filename);
    double cal_Accuracy(int TP, int FP, int TN, int FN);
    double cal_Precision(int TP, int FP);

public: //get() set()
    int getTotal_object();
    int getTotal_true_positive_object();
    int getTotal_false_positive_object();
    int getTotal_true_negative_object();
    int getTotal_false_negative_object();
    double getTotal_accuracy();
    double getTotal_precision();
    double getTotal_false_discovery_rate();
    std::string *getTypename_evaluation_strName();
    int **getTypename_evaluation_Score();
    int getTypename_count_maxsize();

private:
    enum Condition{Condition_TP, Condition_FP, Condition_TN, Condition_FN};
    struct Test_check_mapping_table {
        char type_name[64];
        int  condition;
        bool selected_from_answer;
    };


private:
    void run_evaluation();
    bool targetFileOpen(QUrl answer_folder_url, QString answer_main_Filename, QUrl test_folder_url, QString test_main_Filename);
    int scanFileList(QString strDir, QString filename);
    void detect_typename(std::string *typename_evaluation_strName);
    void typename_evaluation();


private:
    CompareToPolygon  *compareToPolygon;
    int current_filenNumber;

    //Answer
    Object_management *answer_object_data;

    //Test
    Object_management *test_object_data;

    //const double PERCENTILE = 100.0;

private:
    int answer_shape_type;
    int answer_shape_centerX;
    int answer_shape_centerY;
    int answer_shape_lengthX;
    int answer_shape_lengthY;

    int test_shape_type;
    int test_shape_centerX;
    int test_shape_centerY;
    int test_shape_lengthX;
    int test_shape_lengthY;

    int total_object;
    int total_true_positive_object;
    int total_false_positive_object;
    int total_true_negative_object;
    int total_false_negative_object;
    double total_accuracy;
    double total_precision;
    double total_false_discovery_rate;

    int answer_index;
    int test_index;

    //const int typename_evaluation_size = 10;
    std::string *typename_evaluation_strName;
    int **typename_evaluation_Score;

    bool *answer_check_mapping_table;
    struct Test_check_mapping_table *test_check_mapping_table;


};


#endif // EVALUATION_H
