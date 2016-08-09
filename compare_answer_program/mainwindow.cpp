#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    init();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init()
{
    compareToPolygon = new CompareToPolygon();
    answer_file_totalCount = 0;
    test_file_totalCount = 0;
    current_filenNumber = 0;
    ui->progressBar->setValue(0);
    ui->progressBar->setStyleSheet(ui->progressBar->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: green; }");
    ui->progressBar->setTextVisible(true);


    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //ui->tableWidget->setRowCount(10);
    ui->tableWidget->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignCenter);

    //ui->tableWidget->item(1,1)->setTextAlignment(Qt::AlignCenter, Qt::TextAlignmentRole);


}

void MainWindow::on_Btn_answer_folderUrl_clicked()
{
    answer_folder_url = QFileDialog::getExistingDirectory();          //FileDialog 열어서 디렉토리설정
    ui->lineEdit_answer_folderUrl->setText(answer_folder_url.toString());  //File path표시

}

void MainWindow::on_Btn_test_folderUrl_clicked()
{
    test_folder_url = QFileDialog::getExistingDirectory();          //FileDialog 열어서 디렉토리설정
    ui->lineEdit_test_folderUrl->setText(test_folder_url.toString());    //File path표시
}

int MainWindow::scanFileList(QString strDir, QString filename)
{
    QStringList strFilters;
    int n = 0;

    strFilters += filename.append("*.xml");
    //strFilters += "*.h";

    QDirIterator iterDir(strDir, strFilters, QDir::Files | QDir::NoSymLinks, QDirIterator::Subdirectories);

    while (iterDir.hasNext())
    {
        iterDir.next();
        n++;
    }

    return n;
}

void MainWindow::on_btn_input_filename_answer_clicked()
{
    answer_main_Filename = ui->ansersheet_filename_lineEdit->text();

    // load filelist nubmer
    answer_file_totalCount = scanFileList(answer_folder_url.toString(), answer_main_Filename);
    ui->ansersheet_filenumber_textlabel->setText(QString::number(answer_file_totalCount));

}


void MainWindow::on_btn_input_filename_test_clicked()
{
    test_main_Filename = ui->ansersheet_filename_lineEdit->text();

    // load filelist nubmer
    test_file_totalCount = scanFileList(test_folder_url.toString(), test_main_Filename);
    ui->testrsheet_filenumber_textlabel->setText(QString::number(test_file_totalCount));
}

void MainWindow::on_btn_start_marking_exam_clicked()
{
    if(answer_file_totalCount == 0){
        QMessageBox::information(this,"Error","Please Input Answer Sheet.");
    }else if(test_file_totalCount == 0){
        QMessageBox::information(this,"Error","Please Input Test Sheet.");
    }else {
        int answer_shape_type    = 0;
        int answer_shape_centerX = 0;
        int answer_shape_centerY = 0;
        int answer_shape_lengthX = 0;
        int answer_shape_lengthY = 0;

        int test_shape_type    = 0;
        int test_shape_centerX = 0;
        int test_shape_centerY = 0;
        int test_shape_lengthX = 0;
        int test_shape_lengthY = 0;

        int total_object = 0;
        int total_true_positive_object = 0;
        int total_false_positive_object = 0;
        int total_false_negative_object = 0;
        double total_accuracy = 0;
        double total_precision = 0;
        double total_false_discovery_rate = 0;


        const int typename_evaluation_size = 10;
        std::string *typename_evaluation_strName = new std::string[typename_evaluation_size];
        int **typename_evaluation_count;
        typename_evaluation_count= new int*[typename_evaluation_size];

        for(int i =0; i < typename_evaluation_size ; i++){
            typename_evaluation_strName[i] = "";
            typename_evaluation_count[i] = new int[3];
            memset(typename_evaluation_count[i], 0, 3 * sizeof(int));
        }


        current_filenNumber = 1;
        answer_file_totalCount = scanFileList(answer_folder_url.toString(), answer_main_Filename);
        test_file_totalCount = scanFileList(test_folder_url.toString(), test_main_Filename);


        while(current_filenNumber < answer_file_totalCount){
            // Start
            this->answer_object_data = new Object_management();
            this->answer_object_data->setObject_path(answer_folder_url.toString(), QString(answer_main_Filename).append(MainWindow::makefilename(current_filenNumber)).append(QString(".xml")));
            this->answer_object_data->readXMLDataset();

            this->test_object_data = new Object_management();
            this->test_object_data->setObject_path(test_folder_url.toString(), QString(test_main_Filename).append(MainWindow::makefilename(current_filenNumber)).append(QString(".xml")));
            this->test_object_data->readXMLDataset();            

            //initalize mapping_table
            bool *answer_check_mapping_table = new bool[answer_object_data->getCount()];
            std::string **test_check_mapping_table   = new std::string*[test_object_data->getCount()];

            memset(answer_check_mapping_table, false, answer_object_data->getCount() * sizeof(bool));
            for(int i=0; i<test_object_data->getCount(); i++){
                test_check_mapping_table[i] = new std::string[3];
                test_check_mapping_table[i][0] = ""; //
                test_check_mapping_table[i][1] = ""; //TP
                test_check_mapping_table[i][2] = "0";
            }            
            //initalize mapping_table_END

//            std::vector<bool> answer(answer_object_data->getCount());
//            std::array<bool, answer_object_data->getCount()> answer;
//            std::array<std::array<std::string, 3>, test_object_data->getCount()) tcmt;
//            std::map<std::pair<std::string, int>, object> object_map;
//            typedef std::pair<std::string, int> object_id;
//            object a_object(cent, centy, legth,);
//            object[object_id("red_light", 3)] = object(cent, centy, cent,x) ;//a_object;
//            for(std::map<object_id, object>::iterator it = object.begin(); it != object.end(); ++it)
//            {
//                it->first->first;
//            }




            //initalize index
            int answer_index = 0;
            int test_index = 0;
            //initalize index_END

            //Detection typename
            MainWindow::detect_typename(typename_evaluation_strName, typename_evaluation_size);
            for(int i =0 ; i < typename_evaluation_size ; i++){
                if(typename_evaluation_strName[i].compare("") == 0){
                    break;
                }
                std::cout << i << ") typename : " << typename_evaluation_strName[i] << std::endl;
            }
            //Detection typename_END



            while(answer_index < answer_object_data->getCount()){

                int best_score[2] = {-1, -1}; // {index, score}

                while(test_index < test_object_data->getCount()){

                    //compare to overlap
                    if(!test_check_mapping_table[test_index][2].compare("0")){
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

                        double score = compareToPolygon->OverlapArea();
                        std::cout << "[test_index::"<< test_index <<"]Score : " << score << std::endl;                        

                        if(score >= 0.8){ // Positive
                            // answer_Object_type_name == test_Object_type_name
                            if(!answer_object_data->getType_name(answer_index).compare(test_object_data->getType_name(test_index))){
                                test_check_mapping_table[test_index][0] = answer_object_data->getType_name(answer_index);
                                test_check_mapping_table[test_index][1] = "FP";
                                if(best_score[1] < score){
                                    best_score[0] = test_index;
                                    best_score[1] = score;
                                }
                            }else{ // answer_Object_type_name != test_Object_type_name
                                test_check_mapping_table[test_index][0] = answer_object_data->getType_name(answer_index);
                                test_check_mapping_table[test_index][1] = "FP";
                            }
                        }else{ // Negative
                            if(test_check_mapping_table[test_index][1].compare("FP") != 0){
                                test_check_mapping_table[test_index][0] = "";
                                test_check_mapping_table[test_index][1] = "FN";
                            }
                        }
                    }
                    test_index++;
                    compareToPolygon->init();
                }

                if(best_score[1] >= 0.8){
                    answer_check_mapping_table[answer_index] = true;
                    test_check_mapping_table[best_score[0]][1] = "TP";
                    test_check_mapping_table[best_score[0]][2] = "1";
                }

                std::cout << best_score[0] << ", " << best_score[1] << std::endl;
                answer_index++;
                test_index = 0;

            }

            for(int j=0; j < test_object_data->getCount() ; j++){                

                if(test_check_mapping_table[j][1].compare("TP") == 0){
                    total_true_positive_object++;
                }else if(test_check_mapping_table[j][1].compare("FP") == 0){
                    total_false_positive_object++;
                }else if(test_check_mapping_table[j][1].compare("FN") == 0){
                    total_false_negative_object++;
                }
            }

            total_object += test_object_data->getCount();

            MainWindow::typename_evaluation(typename_evaluation_strName, typename_evaluation_count, typename_evaluation_size, test_check_mapping_table);

            this->answer_object_data->init();
            this->test_object_data->init();
            current_filenNumber++;
            ui->progressBar->setValue((current_filenNumber / answer_file_totalCount)*100);
            //ui->progressBar->setFormat(QString::number((current_filenNumber / answer_file_totalCount)*100).append("%"));
        }

        if((total_true_positive_object + total_false_positive_object) == 0)
            total_precision = 0;
        else
            total_precision = ((double)total_true_positive_object / ((double)total_true_positive_object + (double)total_false_positive_object));
        total_accuracy = ((double)total_true_positive_object / ((double)total_true_positive_object + (double)total_false_positive_object + (double)total_false_negative_object));
        total_false_discovery_rate = 1 - total_precision;

        std::cout << "Total Precision : " << total_precision << std::endl;
        std::cout << "Total FDR : " << total_false_discovery_rate << std::endl;


        ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(answer_file_totalCount)));
        ui->tableWidget->setItem(1, 0, new QTableWidgetItem(QString::number(current_filenNumber)));
        ui->tableWidget->setItem(2, 0, new QTableWidgetItem(QString::number(total_accuracy*PERCENTILE)));
        ui->tableWidget->setItem(3, 0, new QTableWidgetItem(QString::number(total_precision*PERCENTILE)));
        ui->tableWidget->setItem(4, 0, new QTableWidgetItem(QString::number(total_false_discovery_rate*PERCENTILE)));

        ui->tableWidget->item(0,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(1,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(2,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(3,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(4,0)->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->insertRow(5);
        ui->tableWidget->setVerticalHeaderItem(5, new QTableWidgetItem(""));


        int addrow=0;
        for(const int i=3; addrow<typename_evaluation_size ; addrow++){
            if(typename_evaluation_strName[addrow].compare("") == 0){
                break;
            }
            int TP = typename_evaluation_count[addrow][0];
            int FP = typename_evaluation_count[addrow][1];
            int TN = 0;
            int FN = typename_evaluation_count[addrow][2];
            double accuracy = 0.0;
            double precision = 0.0;

            if((TP + TN + FP + FN) != 0)
                accuracy = ((double)TP + (double)TN) / ((double)TP + (double)TN + (double)FP + (double)FN);
            if((TP+FP) != 0)
                precision = ((double)TP) / ((double)TP + (double)FP);

            ui->tableWidget->insertRow(6+addrow*i+0);
            ui->tableWidget->insertRow(6+addrow*i+1);
            ui->tableWidget->insertRow(6+addrow*i+2);

            ui->tableWidget->setVerticalHeaderItem(6+addrow*i+0, new QTableWidgetItem(QString::fromStdString(typename_evaluation_strName[addrow]).append(QString(" accuracy"))));
            ui->tableWidget->setItem(6+addrow*i+0, 0, new QTableWidgetItem(QString::number(accuracy*PERCENTILE)));
            ui->tableWidget->item(6+addrow*i+0,0)->setTextAlignment(Qt::AlignCenter);

            ui->tableWidget->setVerticalHeaderItem(6+addrow*i+1, new QTableWidgetItem(QString::fromStdString(typename_evaluation_strName[addrow]).append(QString(" precision"))));
            ui->tableWidget->setItem(6+addrow*i+1, 0, new QTableWidgetItem(QString::number(precision*PERCENTILE)));
            ui->tableWidget->item(6+addrow*i+1,0)->setTextAlignment(Qt::AlignCenter);

            ui->tableWidget->setVerticalHeaderItem(6+addrow*i+2, new QTableWidgetItem(""));
        }

    }
}

void MainWindow::detect_typename(std::string *typename_evaluation_strName, int typename_evaluation_size)
{
    for(int i=0; i < this->test_object_data->getCount(); i++){
        for(int j=0; j < typename_evaluation_size; j++){
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

void MainWindow::typename_evaluation(std::string *typename_evaluation_strName, int **typename_evaluation_count, int typename_evaluation_size, std::string **test_check_mapping_table)
{
    int test_index = 0;
    while(test_index < this->test_object_data->getCount()){
        for(int i = 0; i < typename_evaluation_size ; i++){
            if(typename_evaluation_strName[i].compare("") == 0){
                break;
            }

            if(this->test_object_data->getType_name(test_index).compare(typename_evaluation_strName[i]) == 0){
                if(test_check_mapping_table[i][1].compare("TP") == 0){
                    typename_evaluation_count[i][0]++;
                }else if(test_check_mapping_table[i][1].compare("FP") == 0){
                    typename_evaluation_count[i][1]++;
                }else if(test_check_mapping_table[i][1].compare("FN") == 0){
                    typename_evaluation_count[i][2]++;
                }
            }
        }
        test_index++;
    }
}

QString MainWindow::makefilename(int num){
    int n1 = num / 1000; num -= (n1*1000);
    int n2 = num / 100;  num -= (n2*100);
    int n3 = num / 10;   num -= (n3*10);
    int n4 = num;

    QString result = QString::number(n1).append(QString::number(n2)).append(QString::number(n3)).append(QString::number(n4));

    return result;
}
