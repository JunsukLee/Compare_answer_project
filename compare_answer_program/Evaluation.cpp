#include "Evaluation.hpp"
Evaluation::Evaluation(){

}
/*Evaluation::~Evaluation(){


    delete[] this->compareToPolygon;
    delete[] this->answer_object_data;
    delete[] this->test_object_data;
    delete[] this->typename_evaluation_strName;

    for(int i=0; i < (int)(sizeof(typename_evaluation_Score)/sizeof(typename_evaluation_Score[0])) ; i++){
        delete[] *(typename_evaluation_Score+i);
    }
    delete[] this->typename_evaluation_Score;

    delete[] this->test_check_mapping_table;

    this->compareToPolygon = NULL;
    this->answer_object_data = NULL;
    this->test_object_data = NULL;
    this->typename_evaluation_strName = NULL;
    this->typename_evaluation_Score = NULL;
    this->test_check_mapping_table = NULL;
}*/

bool Evaluation::start_Evaluation(double _OVERLAP_RATE, QUrl answer_folder_url, QString answer_main_Filename, QUrl test_folder_url, QString test_main_Filename)
{    
    if(Evaluation::targetFileOpen(answer_folder_url, answer_main_Filename, test_folder_url, test_main_Filename)){
        this->_OVERLAP_RATE = _OVERLAP_RATE*100;
        if(Evaluation::run_evaluation()){
            return true;
        }
    }
    return false;
}

bool Evaluation::targetFileOpen(QUrl answer_folder_url, QString answer_main_Filename, QUrl test_folder_url, QString test_main_Filename)
{
    // Start
    this->answer_object_data = new Object_management();
    this->answer_object_data->setObject_path(answer_folder_url.toString(), answer_main_Filename);
    if(!this->answer_object_data->readXMLDataset()){
        return false;
    }

    this->test_object_data = new Object_management();
    this->test_object_data->setObject_path(test_folder_url.toString(), test_main_Filename);
    if(!this->test_object_data->readXMLDataset()){
        return false;
    }
    this->test_folder_url = test_folder_url;

    //initalize Answer_mapping_table
    answer_check_mapping_table = new bool[answer_object_data->getCount()];
    memset(answer_check_mapping_table, false, answer_object_data->getCount() * sizeof(bool));

    //initalize Test_mapping_table
    test_check_mapping_table   = (Test_check_mapping_table *)malloc(sizeof(struct Test_check_mapping_table) * (TYPENAME_COUNT_MAXSIZE));
    for(int i=0 ; i<TYPENAME_COUNT_MAXSIZE ; i++){
        memset(test_check_mapping_table[i].type_name, 0, 64 * sizeof(char));
        test_check_mapping_table[i].condition = 9;
        test_check_mapping_table[i].selected_from_answer = false;
    }

    this->answer_folder_url = answer_folder_url;
    this->test_folder_url   = test_folder_url;

    return true;

}


// ---------------------------------------------------------------------------------------------------------
bool Evaluation::run_evaluation()
{    
    //initalize answer value
    answer_shape_type    = 0;
    answer_shape_centerX = 0;
    answer_shape_centerY = 0;
    answer_shape_lengthX = 0;
    answer_shape_lengthY = 0;

    //initalize test value
    test_shape_type    = 0;
    test_shape_centerX = 0;
    test_shape_centerY = 0;
    test_shape_lengthX = 0;
    test_shape_lengthY = 0;

    //initalize index
    this->answer_index = 0;
    this->test_index   = 0;

    //Detection typename
    Evaluation::detect_typename(typename_evaluation_strName);
    for(int i =0 ; i < TYPENAME_COUNT_MAXSIZE ; i++){
        if(typename_evaluation_strName[i].compare("") == 0)
            break;
    }
    //Detection typename_END

    while(answer_index < (answer_object_data->getCount())){

        int best_score[2] = {-1, -1}; // {index, score}

        while(test_index < (test_object_data->getCount())){

            //compare to overlap
            if(!test_check_mapping_table[test_index].selected_from_answer){
                answer_shape_type    = answer_object_data->getShape_config(answer_index)->getShape_type();
                answer_shape_centerX = answer_object_data->getShape_config(answer_index)->getCenter_point_x();
                answer_shape_centerY = answer_object_data->getShape_config(answer_index)->getCenter_point_y();
                answer_shape_lengthX = answer_object_data->getShape_config(answer_index)->getLength_x();
                answer_shape_lengthY = answer_object_data->getShape_config(answer_index)->getLength_y();
                compareToPolygon->setPolygonData(true, answer_shape_type, answer_shape_centerX, answer_shape_centerY, answer_shape_lengthX, answer_shape_lengthY);

                test_shape_type    = test_object_data->getShape_config(test_index)->getShape_type();
                test_shape_centerX = test_object_data->getShape_config(test_index)->getCenter_point_x();
                test_shape_centerY = test_object_data->getShape_config(test_index)->getCenter_point_y();
                test_shape_lengthX = test_object_data->getShape_config(test_index)->getLength_x();
                test_shape_lengthY = test_object_data->getShape_config(test_index)->getLength_y();
                compareToPolygon->setPolygonData(false, test_shape_type, test_shape_centerX, test_shape_centerY, test_shape_lengthX, test_shape_lengthY);

                double score = compareToPolygon->OverlapArea() * 100;

                if(score > 0){ // Positive
                    strcpy(test_check_mapping_table[test_index].type_name, test_object_data->getType_name(test_index).c_str());
                    test_check_mapping_table[test_index].condition = Condition_FP;

                    // true :: score>=0.8 AND answer_Object_type_name == test_Object_type_name
                    if(score >= _OVERLAP_RATE && !answer_object_data->getType_name(answer_index).compare(test_object_data->getType_name(test_index))){
                        //strcpy(test_check_mapping_table[test_index].type_name, answer_object_data->getType_name(answer_index).c_str());
                        //test_check_mapping_table[test_index].condition = Condition_FP;
                        if(best_score[1] < score){
                            best_score[0] = test_index;
                            best_score[1] = score;
                        }

                     // false :: score < 0.8 OR answer_Object_type_name != test_Object_type_name
                    }else if(score < _OVERLAP_RATE){
                        strcpy(test_check_mapping_table[test_index].type_name, test_object_data->getType_name(test_index).c_str());
                        test_check_mapping_table[test_index].condition = Condition_FP;
                    }
                }else if(score == 0){// Negative
                    if(test_check_mapping_table[test_index].condition != Condition_FP){
                        //memset(test_check_mapping_table[test_index].type_name, 0, 64*sizeof(char));
                        strcpy(test_check_mapping_table[test_index].type_name, test_object_data->getType_name(test_index).c_str());
                        test_check_mapping_table[test_index].condition = Condition_FN;
                    }
                }
            }
            test_index++;
            compareToPolygon->init();
        }

        if(best_score[1] >= _OVERLAP_RATE){
            answer_check_mapping_table[answer_index] = true;
            test_check_mapping_table[best_score[0]].condition = Condition_TP;
            test_check_mapping_table[best_score[0]].selected_from_answer = true;
        }
        answer_index++;
        test_index = 0;
    }


    // COUNT condition
    Evaluation::typename_evaluation();
    total_object += test_object_data->getCount();

    //Delete
    this->answer_object_data->~Object_management();
    this->test_object_data->~Object_management();

    return true;
}

// ---------------------------------------------------------------------------------------------------------

bool Evaluation::writeResultReport(int answer_file_totalCount, int current_filenNumber)
{
    //--------------------------------------------------------------------------------
    QFile *file = new QFile;

    file->setFileName(this->test_folder_url.toString()+"/Result_report.txt");
    if(file->exists()){
        file->remove();
    }
    if(!file->open(QIODevice::WriteOnly))
        return false;
    //--------------------------------------------------------------------------------


    QString str;
    str = QString("## OVERLAB RATE\t\t\t:\t").append(QString::number(_OVERLAP_RATE)).append(QString("% \n"));
    str.append(QString("## Answer Folder URL\t\t:\t")).append(this->answer_folder_url.toString()).append(QString("\n"));
    str.append(QString("## Test Folder URL\t\t:\t")).append(this->test_folder_url.toString()).append(QString("\n"));
    str.append(QString("## Date\t\t\t\t:\t")).append(QDateTime::currentDateTimeUtc().toUTC().toString()).append(QString("\n\n"));
    str.append(QString("==================================================================\n"));
    str.append(QString("Total target files\t\t:\t")).append(QString::number(answer_file_totalCount)).append(QString("\n"));
    str.append(QString("Total successful files\t\t:\t").append(QString::number(current_filenNumber)).append(QString("\n")));
    str.append(QString("Total accuracy\t\t\t:\t").append(QString::number(Evaluation::getTotal_accuracy()*PERCENTILE)).append(QString("%\n")));
    str.append(QString("Total precision\t\t\t:\t").append(QString::number(Evaluation::getTotal_precision()*PERCENTILE)).append(QString("%\n")));
    str.append(QString("Total false_discovery_rate(FDR)\t:\t").append(QString::number(total_false_discovery_rate)).append(QString("\n")));
    str.append(QString("Total object count\t\t:\t").append(QString::number(total_true_positive_object+total_false_positive_object+total_true_negative_object+total_false_negative_object)).append(QString("\n")));
    str.append(QString("Total true positive object\t:\t").append(QString::number(total_true_positive_object)).append(QString("\n")));
    str.append(QString("Total false positive object\t:\t").append(QString::number(total_false_positive_object)).append(QString("\n")));
    str.append(QString("Total true negative object\t:\t").append(QString::number(total_true_negative_object)).append(QString("\n")));
    str.append(QString("Total false negative object\t:\t").append(QString::number(total_false_negative_object)).append(QString("\n")));


    for(int addrow=0; addrow< Evaluation::getTypename_count_maxsize() ; addrow++){
        if(Evaluation::getTypename_evaluation_strName()[addrow].compare("") == 0){
            break;
        }
        int TP = Evaluation::getTypename_evaluation_Score()[addrow][0];
        int FP = Evaluation::getTypename_evaluation_Score()[addrow][1];
        int TN = Evaluation::getTypename_evaluation_Score()[addrow][2];
        int FN = Evaluation::getTypename_evaluation_Score()[addrow][3];

        double accuracy  = Evaluation::cal_Accuracy(TP, FP, TN, FN);
        double precision = Evaluation::cal_Precision(TP, FP);


        str.append(QString("\n\n"));
        str.append(QString("==================================================================\n"));
        str.append(QString::fromStdString(Evaluation::getTypename_evaluation_strName()[addrow])).append(" accuracy\t\t\t:\t");
        str.append(QString::number(accuracy*PERCENTILE).append(QString("%")));

        str.append(QString("\n"));
        str.append(QString::fromStdString(Evaluation::getTypename_evaluation_strName()[addrow])).append(" precision\t\t\t:\t");
        str.append(QString::number(precision*PERCENTILE).append(QString("%")));

        str.append(QString("\n"));
        str.append(QString::fromStdString(Evaluation::getTypename_evaluation_strName()[addrow])).append(" true positive object\t:\t");
        str.append(QString::number(TP));

        str.append(QString("\n"));
        str.append(QString::fromStdString(Evaluation::getTypename_evaluation_strName()[addrow])).append(" false positive object\t:\t");
        str.append(QString::number(FP));

        str.append(QString("\n"));
        str.append(QString::fromStdString(Evaluation::getTypename_evaluation_strName()[addrow])).append(" true negative object\t:\t");
        str.append(QString::number(TN));

        str.append(QString("\n"));
        str.append(QString::fromStdString(Evaluation::getTypename_evaluation_strName()[addrow])).append(" false negative object\t:\t");
        str.append(QString::number(FN).append(QString("\n")));
    }

    //--------------------------------------------------------------------------------
    QByteArray _str;
    _str.append(str);

    file->write(_str);
    file->close();

    return true;

}

void Evaluation::init()
{
    //initalize typename_evaluation_strName
    //initalize typename_evaluation_Score
    typename_evaluation_strName = new std::string[TYPENAME_COUNT_MAXSIZE];
    typename_evaluation_Score= new int*[TYPENAME_COUNT_MAXSIZE];

    for(int i =0; i < TYPENAME_COUNT_MAXSIZE ; i++){
        typename_evaluation_strName[i] = "";
        typename_evaluation_Score[i] = new int[4]; // {TP, FP, TN, FN}
        memset(typename_evaluation_Score[i], 0, 4 * sizeof(int));
        typename_evaluation_Score[i][0] = 0;
        typename_evaluation_Score[i][1] = 0;
        typename_evaluation_Score[i][2] = 0;
        typename_evaluation_Score[i][3] = 0;

    }


    Evaluation::total_object = 0;
    Evaluation::total_true_positive_object = 0;
    Evaluation::total_false_positive_object = 0;
    Evaluation::total_true_negative_object = 0;
    Evaluation::total_false_negative_object = 0;
    Evaluation::total_accuracy = (double)0;
    Evaluation::total_precision = (double)0;
    Evaluation::total_false_discovery_rate = (double)0;

    compareToPolygon = new CompareToPolygon();
}

int Evaluation::scanFileList(QString strDir, QString filename)
{
    QStringList strFilters;
    int n = 0;

    strFilters += filename.append("*.xml");
    //strFilters += "*.h";

    QDirIterator iterDir(strDir, strFilters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    while (iterDir.hasNext()){
        iterDir.next();
        n++;
    }

    return n;
}

void Evaluation::detect_typename(std::string *typename_evaluation_strName)
{
    for(int i=0; i < this->test_object_data->getCount(); i++){
        for(int j=0; j < TYPENAME_COUNT_MAXSIZE; j++){
            if(typename_evaluation_strName[j].compare(this->test_object_data->getType_name(i)) == 0){
                break;
            }
            if(typename_evaluation_strName[j].compare("") == 0){
                typename_evaluation_strName[j] = this->test_object_data->getType_name(i);
                break;
            }
        }
    }
}

void Evaluation::typename_evaluation()
{
    int test_index = 0;        

    while(test_index < test_object_data->getCount()){

        //total count
        if(test_check_mapping_table[test_index].condition == Condition_TP){
            total_true_positive_object++;
        }else if(test_check_mapping_table[test_index].condition == Condition_FP){
            total_false_positive_object++;
        }else if(test_check_mapping_table[test_index].condition == Condition_TN){
            total_true_negative_object++;
        }else if(test_check_mapping_table[test_index].condition == Condition_FN){
            total_false_negative_object++;            
        }


        for(int i = 0; i < TYPENAME_COUNT_MAXSIZE ; i++){
            if(this->typename_evaluation_strName[i].compare("") == 0)
                break;

            //if(test_object_data->getType_name(test_index).compare(this->typename_evaluation_strName[i]) == 0){
            if(strcmp(test_check_mapping_table[test_index].type_name, typename_evaluation_strName[i].c_str()) == 0){
                if(test_check_mapping_table[test_index].condition == Condition_TP){
                    this->typename_evaluation_Score[i][0]++;
                }else if(test_check_mapping_table[test_index].condition == Condition_FP){
                    this->typename_evaluation_Score[i][1]++;
                }else if(test_check_mapping_table[test_index].condition == Condition_TN){
                    this->typename_evaluation_Score[i][2]++;
                }else if(test_check_mapping_table[test_index].condition == Condition_FN){
                    this->typename_evaluation_Score[i][3]++;
                }
            }
        }
        test_index++;
    }
}

double Evaluation::cal_Accuracy(int TP, int FP, int TN, int FN){return ((TP+FP+TN+FN) == 0) ? 0 : (double)(TP+TN)/(double)(TP+FP+TN+FN);}
double Evaluation::cal_Precision(int TP, int FP){return ((TP+FP) == 0) ? 0 : (double)(TP)/(double)(TP+FP);}
int Evaluation::getTotal_object(){return total_object;}
int Evaluation::getTotal_true_positive_object(){return total_true_positive_object;}
int Evaluation::getTotal_false_positive_object(){return total_false_positive_object;}
int Evaluation::getTotal_true_negative_object(){return total_true_negative_object;}
int Evaluation::getTotal_false_negative_object(){return total_false_negative_object;}
double Evaluation::getTotal_accuracy(){return cal_Accuracy(total_true_positive_object, total_false_positive_object, total_true_negative_object, total_false_negative_object);}
double Evaluation::getTotal_precision(){return cal_Precision(total_true_positive_object, total_false_positive_object);}
double Evaluation::getTotal_false_discovery_rate(){return (1.0 - getTotal_accuracy());}
std::string *Evaluation::getTypename_evaluation_strName(){return typename_evaluation_strName;}
int **Evaluation::getTypename_evaluation_Score(){return typename_evaluation_Score;}
int Evaluation::getTypename_count_maxsize(){return TYPENAME_COUNT_MAXSIZE;}
