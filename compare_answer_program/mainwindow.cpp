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
    answer_file_totalCount = 0;
    test_file_totalCount = 0;
    current_filenNumber = 0;

    ui->lineEdit_answer_folderUrl->setReadOnly(true);
    ui->lineEdit_test_folderUrl->setReadOnly(true);
    ui->label_overlap->setAlignment(Qt::AlignCenter);
    ui->lineEdit_overlapRate->setAlignment(Qt::AlignCenter);

    ui->progressBar->setValue(0);
    ui->progressBar->setStyleSheet(ui->progressBar->property("defaultStyleSheet").toString() + " QProgressBar::chunk { background: green; }");
    ui->progressBar->setTextVisible(true);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);    
    ui->tableWidget->horizontalHeaderItem(0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::on_Btn_answer_folderUrl_clicked()
{
    answer_folder_url = QFileDialog::getExistingDirectory();
    ui->lineEdit_answer_folderUrl->setText(answer_folder_url.toString());
}

void MainWindow::on_Btn_test_folderUrl_clicked()
{
    test_folder_url = QFileDialog::getExistingDirectory();
    ui->lineEdit_test_folderUrl->setText(test_folder_url.toString());
}

int MainWindow::scanFileList(QString strDir, QString filename)
{
    QStringList strFilters;
    int n = 0;

    strFilters += filename.append("*.xml");
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

    if(answer_main_Filename.operator ==((""))){
        QMessageBox::information(this,"Error","Please Input Main Filename.");
    }else{
        if(ui->lineEdit_answer_folderUrl->text().operator ==(("")))
            QMessageBox::information(this,"Error","Please Input Answer FolderURL.");
        else{
            // load filelist nubmer
            answer_file_totalCount = scanFileList(answer_folder_url.toString(), answer_main_Filename);
            ui->ansersheet_filenumber_textlabel->setText(QString::number(answer_file_totalCount));
        }
    }

}



void MainWindow::on_btn_input_filename_test_clicked()
{
    test_main_Filename = ui->testsheet_filename_lineEdit->text();

    if(test_main_Filename.operator ==((""))){
        QMessageBox::information(this,"Error","Please Input Main Filename.");
    }else{
        if(ui->lineEdit_test_folderUrl->text().operator ==(("")))
            QMessageBox::information(this,"Error","Please Input Test FolderURL.");
        else{
            // load filelist nubmer
            test_file_totalCount = scanFileList(test_folder_url.toString(), test_main_Filename);
            ui->testrsheet_filenumber_textlabel->setText(QString::number(test_file_totalCount));
        }
    }
}

// -----------------------------------------------------------------------------------------------
void MainWindow::on_btn_start_marking_exam_clicked()
{    
    if(answer_file_totalCount == 0){
        QMessageBox::information(this,"Error","Please Input Answer Sheet.");
    }else if(test_file_totalCount == 0){
        QMessageBox::information(this,"Error","Please Input Test Sheet.");
    }else if(check_overlap_rate()){
        current_filenNumber=1;
        evaluation = new Evaluation();
        evaluation->init();

        while(current_filenNumber <= answer_file_totalCount){
            QString answer_filename_param = answer_main_Filename + (makefilename(current_filenNumber)) + QString(".xml");
            QString test_filename_param   = test_main_Filename + (makefilename(current_filenNumber)) + QString(".xml");

            evaluation->start_Evaluation(ui->lineEdit_overlapRate->text().toDouble(), answer_folder_url, answer_filename_param, test_folder_url, test_filename_param);
            current_filenNumber++;
            ui->progressBar->setValue((current_filenNumber / answer_file_totalCount)*100);
        }
        current_filenNumber--;
        drawingTable();
        if(!evaluation->writeResultReport(answer_file_totalCount, current_filenNumber))
            QMessageBox::information(this,"Error","Fail to save the Result Report.");
    }
}
// -----------------------------------------------------------------------------------------------

QString MainWindow::makefilename(int num){
    int n1 = num / 1000; num -= (n1*1000);
    int n2 = num / 100;  num -= (n2*100);
    int n3 = num / 10;   num -= (n3*10);
    int n4 = num;

    QString result = QString::number(n1).append(QString::number(n2)).append(QString::number(n3)).append(QString::number(n4));

    return result;
}

bool MainWindow::check_overlap_rate()
{
    double overlap_rate = ui->lineEdit_overlapRate->text().toDouble();
    if(overlap_rate > 0.0 && overlap_rate <= 1.0){
        return true;
    }else{
        QMessageBox::information(this,"Error::Setting OVERLAP_RATE","TRUE CONDITION : 0 < OVERLAP_RATE <= 1");
        return false;
    }

}

void MainWindow::drawingTable()
{    
    int row_index=10;
    ui->tableWidget->clearContents();
    while(ui->tableWidget->rowCount() != 10){
        ui->tableWidget->removeRow(row_index);
    }

    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString::number(answer_file_totalCount)));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem(QString::number(current_filenNumber)));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem(QString::number(evaluation->getTotal_accuracy()*PERCENTILE).append(QString("%"))));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem(QString::number(evaluation->getTotal_precision()*PERCENTILE).append(QString("%"))));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem(QString::number(evaluation->getTotal_false_discovery_rate()*PERCENTILE).append(QString("%"))));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem(QString::number(evaluation->getTotal_true_positive_object()+evaluation->getTotal_false_positive_object()+evaluation->getTotal_true_negative_object()+evaluation->getTotal_false_negative_object()))); // total object count
    ui->tableWidget->setItem(6, 0, new QTableWidgetItem(QString::number(evaluation->getTotal_true_positive_object()))); // TP
    ui->tableWidget->setItem(7, 0, new QTableWidgetItem(QString::number(evaluation->getTotal_false_positive_object()))); // FP
    ui->tableWidget->setItem(8, 0, new QTableWidgetItem(QString::number(evaluation->getTotal_true_negative_object()))); // TN
    ui->tableWidget->setItem(9, 0, new QTableWidgetItem(QString::number(evaluation->getTotal_false_negative_object()))); // FN


    ui->tableWidget->item(0,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(1,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(2,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(3,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(4,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(5,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(6,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(7,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(8,0)->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->item(9,0)->setTextAlignment(Qt::AlignCenter);

    ui->tableWidget->item(2,0)->setBackgroundColor(QColor(200,225,255));
    ui->tableWidget->item(3,0)->setBackgroundColor(QColor(200,225,255));
    ui->tableWidget->item(4,0)->setBackgroundColor(QColor(200,225,255));



    int total_rowCount = 10;
    ui->tableWidget->insertRow(total_rowCount);
    ui->tableWidget->setVerticalHeaderItem(total_rowCount, new QTableWidgetItem(""));


    int addrow=0;
    for(const int i=7; addrow<evaluation->getTypename_count_maxsize() ; addrow++){
        if(evaluation->getTypename_evaluation_strName()[addrow].compare("") == 0){
            break;
        }
        int TP = evaluation->getTypename_evaluation_Score()[addrow][0];
        int FP = evaluation->getTypename_evaluation_Score()[addrow][1];
        int TN = evaluation->getTypename_evaluation_Score()[addrow][2];
        int FN = evaluation->getTypename_evaluation_Score()[addrow][3];

        double accuracy  = evaluation->cal_Accuracy(TP, FP, TN, FN);
        double precision = evaluation->cal_Precision(TP, FP);

        ui->tableWidget->insertRow(total_rowCount+addrow*i+0);
        ui->tableWidget->insertRow(total_rowCount+addrow*i+1);
        ui->tableWidget->insertRow(total_rowCount+addrow*i+2);
        ui->tableWidget->insertRow(total_rowCount+addrow*i+3); // TP
        ui->tableWidget->insertRow(total_rowCount+addrow*i+4); // FP
        ui->tableWidget->insertRow(total_rowCount+addrow*i+5); // TN
        ui->tableWidget->insertRow(total_rowCount+addrow*i+6); // FN

        ui->tableWidget->setVerticalHeaderItem(total_rowCount+addrow*i+0, new QTableWidgetItem(""));

        ui->tableWidget->setVerticalHeaderItem(total_rowCount+addrow*i+1, new QTableWidgetItem(QString::fromStdString(evaluation->getTypename_evaluation_strName()[addrow]).append(QString(" accuracy"))));
        ui->tableWidget->setItem(total_rowCount+addrow*i+1, 0, new QTableWidgetItem(QString::number(accuracy*PERCENTILE).append(QString("%"))));
        ui->tableWidget->item(total_rowCount+addrow*i+1,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(total_rowCount+addrow*i+1,0)->setBackgroundColor(QColor(200,225,255));


        ui->tableWidget->setVerticalHeaderItem(total_rowCount+addrow*i+2, new QTableWidgetItem(QString::fromStdString(evaluation->getTypename_evaluation_strName()[addrow]).append(QString(" precision"))));
        ui->tableWidget->setItem(total_rowCount+addrow*i+2, 0, new QTableWidgetItem(QString::number(precision*PERCENTILE).append(QString("%"))));
        ui->tableWidget->item(total_rowCount+addrow*i+2,0)->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->item(total_rowCount+addrow*i+2,0)->setBackgroundColor(QColor(200,225,255));


        //TP
        ui->tableWidget->setVerticalHeaderItem(total_rowCount+addrow*i+3, new QTableWidgetItem(QString::fromStdString(evaluation->getTypename_evaluation_strName()[addrow]).append(QString(" true positive object"))));
        ui->tableWidget->setItem(total_rowCount+addrow*i+3, 0, new QTableWidgetItem(QString::number(TP)));
        ui->tableWidget->item(total_rowCount+addrow*i+3,0)->setTextAlignment(Qt::AlignCenter);

        //FP
        ui->tableWidget->setVerticalHeaderItem(total_rowCount+addrow*i+4, new QTableWidgetItem(QString::fromStdString(evaluation->getTypename_evaluation_strName()[addrow]).append(QString(" false positive object"))));
        ui->tableWidget->setItem(total_rowCount+addrow*i+4, 0, new QTableWidgetItem(QString::number(FP)));
        ui->tableWidget->item(total_rowCount+addrow*i+4,0)->setTextAlignment(Qt::AlignCenter);

        //TN
        ui->tableWidget->setVerticalHeaderItem(total_rowCount+addrow*i+5, new QTableWidgetItem(QString::fromStdString(evaluation->getTypename_evaluation_strName()[addrow]).append(QString(" true negative object"))));
        ui->tableWidget->setItem(total_rowCount+addrow*i+5, 0, new QTableWidgetItem(QString::number(TN)));
        ui->tableWidget->item(total_rowCount+addrow*i+5,0)->setTextAlignment(Qt::AlignCenter);

        //FN
        ui->tableWidget->setVerticalHeaderItem(total_rowCount+addrow*i+6, new QTableWidgetItem(QString::fromStdString(evaluation->getTypename_evaluation_strName()[addrow]).append(QString(" false negative object"))));
        ui->tableWidget->setItem(total_rowCount+addrow*i+6, 0, new QTableWidgetItem(QString::number(FN)));
        ui->tableWidget->item(total_rowCount+addrow*i+6,0)->setTextAlignment(Qt::AlignCenter);

        ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    }

}
