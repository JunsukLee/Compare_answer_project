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

    ui->tableWidget->setColumnWidth(0, 50);
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
        double total_precision = 0;
        double total_false_discovery_rate = 0;


        while(current_filenNumber < answer_file_totalCount){
            // Start
            this->answer_object_data = new Object_management();
            this->answer_object_data->setObject_path(answer_folder_url.toString(), QString(answer_main_Filename).append(QString::number(current_filenNumber)).append(QString(".xml")));
            this->answer_object_data->readXMLDataset();

            this->test_object_data = new Object_management();
            this->test_object_data->setObject_path(test_folder_url.toString(), QString(test_main_Filename).append(QString::number(current_filenNumber)).append(QString(".xml")));
            this->test_object_data->readXMLDataset();

            std::cout << "filename : " << QString(answer_main_Filename).append(QString::number(current_filenNumber)).append(QString(".xml")).toUtf8().constData() << std::endl;

            bool *answer_check_mapping_table = new bool[answer_object_data->getCount()];
            bool *test_check_mapping_table   = new bool[test_object_data->getCount()];
            memset(answer_check_mapping_table, false, answer_object_data->getCount() * sizeof(bool));
            memset(test_check_mapping_table, false, test_object_data->getCount() * sizeof(bool));

            int answer_index = 0;
            int test_index = 0;



            while(answer_index < answer_object_data->getCount()){

                int best_source[2] = {-1, -1}; // {index, source}

                while(test_index < test_object_data->getCount()){

                    if(!answer_object_data->getType_name(answer_index).compare(test_object_data->getType_name(test_index)) && !test_check_mapping_table[test_index]){
                        compareToPolygon->init();

                        answer_shape_type    = answer_object_data->getShape_config(answer_index)->getShape_type();
                        answer_shape_centerX = answer_object_data->getShape_config(answer_index)->getCenter_point_x();
                        answer_shape_centerY = answer_object_data->getShape_config(answer_index)->getCenter_point_y();
                        answer_shape_lengthX = answer_object_data->getShape_config(answer_index)->getLength_x();
                        answer_shape_lengthY = answer_object_data->getShape_config(answer_index)->getLength_y();

                        printf("[%d][%d]----- : %s   %s\n", answer_index, test_index, answer_object_data->getType_name(answer_index).c_str(), test_object_data->getType_name(test_index).c_str());

                        printf("----- : %d   %d   %d   %d   %d\n", answer_shape_type, answer_shape_centerX, answer_shape_centerY, answer_shape_lengthX, answer_shape_lengthY);

                        compareToPolygon->setPolygonData(true, answer_shape_type, answer_shape_centerX, answer_shape_centerY, answer_shape_lengthX, answer_shape_lengthY);

                        test_shape_type    = test_object_data->getShape_config(test_index)->getShape_type();
                        test_shape_centerX = test_object_data->getShape_config(test_index)->getCenter_point_x();
                        test_shape_centerY = test_object_data->getShape_config(test_index)->getCenter_point_y();
                        test_shape_lengthX = test_object_data->getShape_config(test_index)->getLength_x();
                        test_shape_lengthY = test_object_data->getShape_config(test_index)->getLength_y();

                        compareToPolygon->setPolygonData(false, test_shape_type, test_shape_centerX, test_shape_centerY, test_shape_lengthX, test_shape_lengthY);

                        qDebug() << "answer_index : " << answer_index;
                        qDebug() << "test_index : " << test_index;

                        double source = compareToPolygon->OverlapArea();
                        std::cout << "Source : " << source << std::endl;
                        if(source > 0.8){
                            if(best_source[1] < source){
                                best_source[0] = test_index;
                                best_source[1] = source;
                            }
                        }

                    }else{
                        answer_check_mapping_table[answer_index] = false;

                    }
                    test_index++;
                }

                if(best_source[1] > 0.8){
                    answer_check_mapping_table[answer_index] = true;
                    test_check_mapping_table[best_source[0]] = true;
                }
                std::cout << best_source[0] << ", " << best_source[1] << std::endl;
                answer_index++;
                test_index = 0;
            }

            this->answer_object_data->init();
            this->test_object_data->init();
            current_filenNumber++;
            ui->progressBar->setValue((current_filenNumber / answer_file_totalCount)*100);
            //ui->progressBar->setFormat(QString::number((current_filenNumber / answer_file_totalCount)*100).append("%"));
        }
    }
}
