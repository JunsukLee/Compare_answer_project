#include "Object_management.h"

Object_management::Object_management(){

}

void Object_management::setObject_path(QString foldername, QString filename)
{
    Object_management::setFoldername(foldername);
    Object_management::setFilename(filename);
}

void Object_management::setObject_Config(int count)
{
    this->count_id = 0;
    this->count = count;
    this->id = new int[this->count];
    this->str_id    = new std::string[this->count];
    this->type_name = new std::string[this->count];
    shape_config = new Shape_configuation[this->count];
}

bool Object_management::setShape_Config(std::string str_id, std::string type_name, std::string shape_type, int center_point_x, int center_point_y, int length_x, int length_y)
{
    if(this->count_id >= this->count){
        //set Fail
        return false;
    }

    this->id[count_id] = count_id;
    this->str_id[count_id] = str_id;
    this->type_name[count_id] = type_name;
    this->shape_config[count_id].setConfig(shape_type, center_point_x, center_point_y, length_x, length_y);
    count_id++;

    //set Success
    return true;
}

bool Object_management::writeDataset()
{
    QString dirname = (this->foldername)+"_AnswerSheet";

    if(!QDir(dirname).exists()){
        QDir().mkdir(dirname);
    }

    //--------------------------------------------------------

    QFile *file = new QFile;

    std::vector<std::string> xmlfilename_vector = split(this->filename.toUtf8().constData(), '.');
    QString xmlfilename = QString::fromStdString(xmlfilename_vector.at(0)) + ".xml";

    file->setFileName(dirname+"/"+xmlfilename);
    if(file->exists()){
        file->remove();
    }
    file->open(QIODevice::WriteOnly);

    //--------------------------------------------------------

    QString project_name = "Traffic_signal";
    QString filename   = this->filename;

    //--------------------------------------------------------
    //enum shape_enum {dot_point, circle, rectangle};

    QString str;
    str   =    "<?xml version=\"1.0\" encoding=\"UTF-8\" ?>\n";
    str.append("<!-- Dot_point : 0 / Circle : 1 / Rectangle : 2 -->\n");
    str.append("<"+project_name+">\n");
    str.append("\t<image>\n");
    str.append("\t\t<filename>").append(filename).append("</filename>\n");
    str.append("\t\t<count>").append(QString::number(this->count)).append("</count>\n");

    for(int idx=0; idx< (this->count); idx++){
        str.append("\t\t<object>\n");
        str.append("\t\t\t<indexID>").append(QString::fromStdString(this->str_id[idx])).append("</indexID>\n");
        str.append("\t\t\t<type_name>").append(QString::fromStdString(this->type_name[idx])).append("</type_name>\n");
        str.append("\t\t\t<data>\n");
        str.append("\t\t\t\t<type_shape>").append(QString::number(this->shape_config[idx].getShape_type())).append("</type_shape>\n");
        str.append("\t\t\t\t<center_point_x>").append(QString::number(this->shape_config[idx].getCenter_point_x())).append("</center_point_x>\n");
        str.append("\t\t\t\t<center_point_y>").append(QString::number(this->shape_config[idx].getCenter_point_y())).append("</center_point_y>\n");
        str.append("\t\t\t\t<length_x>").append(QString::number(this->shape_config[idx].getLength_x())).append("</length_x>\n");
        str.append("\t\t\t\t<length_y>").append(QString::number(this->shape_config[idx].getLength_y())).append("</length_y>\n");
        str.append("\t\t\t</data>\n");
        str.append("\t\t</object>\n");
    }


    str.append("\t</image>\n");
    str.append("</"+project_name+">\n");





    //--------------------------------------------------------


    QByteArray _str;
    _str.append(str);

    file->write(_str);
    file->close();

    return true;
}


bool Object_management::readXMLDataset()
{
    QFile *file = new QFile;
    file->setFileName(this->fullFolderPath+"/"+this->filename);


    if(!file->exists()){
        qDebug() << "Error : Not found targetFilename";
        return false;
    }

    if(!file->open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug() << "Error : Failed to open the file for reading";
        return false;
    }

    //qDebug() << "start read xmlfile";
    count_id = 0;

    QXmlStreamReader reader;
    reader.addData(file->readAll());

    if (reader.readNextStartElement()) {
            if (reader.name() == "Traffic_signal"){
                while(reader.readNextStartElement()){
                    if(reader.name() == "image"){                                  // [image subelement]
                        while(reader.readNextStartElement()){
                            if(reader.name() == "filename"){
                                QString s = reader.readElementText();                                
                            }
                            else if(reader.name() == "count"){
                                QString s = reader.readElementText();                                
                                std::string s1 = s.toUtf8().constData();
                                this->count = atoi(s1.c_str());
                                {
                                    setObject_Config(this->count);
                                }

                                while(reader.readNextStartElement()){
                                    if(reader.name() == "object"){                 // [object subelement]
                                        while(reader.readNextStartElement()){
                                            if(reader.name() == "indexID"){
                                                QString s = reader.readElementText();                                                
                                                this->str_id[this->count_id] = s.toUtf8().constData();
                                            }else if(reader.name() == "type_name"){
                                                QString s = reader.readElementText();                                                
                                                this->type_name[this->count_id] = s.toUtf8().constData();
                                            }else if(reader.name() == "data"){      // [data subelement]
                                                while(reader.readNextStartElement()){
                                                    if(reader.name() == "type_shape"){
                                                        QString s = reader.readElementText();
                                                        std::string s1 = s.toUtf8().constData();                                                        
                                                        this->shape_config[this->count_id].setShape(atoi(s1.c_str()));
                                                    }else if(reader.name() == "center_point_x"){
                                                        QString s = reader.readElementText();
                                                        std::string s1 = s.toUtf8().constData();                                                        
                                                        this->shape_config[this->count_id].setCenter_point_x(atoi(s1.c_str()));
                                                    }else if(reader.name() == "center_point_y"){
                                                        QString s = reader.readElementText();
                                                        std::string s1 = s.toUtf8().constData();                                                        
                                                        this->shape_config[this->count_id].setCenter_point_y(atoi(s1.c_str()));
                                                    }else if(reader.name() == "length_x"){
                                                        QString s = reader.readElementText();
                                                        std::string s1 = s.toUtf8().constData();                                                        
                                                        this->shape_config[this->count_id].setLength_x(atoi(s1.c_str()));
                                                    }else if(reader.name() == "length_y"){
                                                        QString s = reader.readElementText();
                                                        std::string s1 = s.toUtf8().constData();                                                        
                                                        this->shape_config[this->count_id].setLength_y(atoi(s1.c_str()));
                                                    }else
                                                        reader.skipCurrentElement();
                                                }
                                            }else
                                                reader.skipCurrentElement();
                                        }
                                    }
                                    count_id++;
                                }

                            }else
                                reader.skipCurrentElement();
                        }
                    }
                    else
                        reader.skipCurrentElement();
                }
            }
            else
                reader.raiseError(QObject::tr("Incorrect file"));
    }

    //qDebug() << "end read xmlfile";


    return true;

}



void Object_management::init(){
    this->fullFolderPath = "";
    this->foldername = "";
    this->filename = "";

    this->count_id = 0;
    this->count = 0;
    delete[] this->id;
    delete[] this->str_id;
    delete[] this->type_name;
    delete[] this->shape_config;

    this->id = NULL;
    this->str_id = NULL;
    this->type_name = NULL;
    this->shape_config = NULL;
}

int Object_management::getCount()
{
    return this->count;
}

std::string Object_management::getStr_id(int n)
{
    return str_id[n];
}

std::string Object_management::getType_name(int n)
{
    return type_name[n];
}

Shape_configuation * Object_management::getShape_config(int n)
{
    return &shape_config[n];
}

void Object_management::setFoldername(QString path)
{
    this->fullFolderPath = path;
    std::vector<std::string> fullpath = split(path.toUtf8().constData(), '/');
    this->foldername = QString::fromStdString(fullpath.back());
}
void Object_management::setFilename(QString filename)
{
    this->filename = filename;
}

std::vector<std::string> Object_management::split(const std::string &s, char delim){
    std::vector<std::string> elems;
    std::stringstream ss(s);
    std::string number;
    while(std::getline(ss, number, delim)){
        elems.push_back(number);
    }
    return elems;
}


Object_management::~Object_management()
{
    this->count_id = 0;
    this->count = 0;
    delete[] this->id;
    delete[] this->type_name;
    delete[] this->shape_config;

    this->id = NULL;
    this->type_name = NULL;
    this->shape_config = NULL;
}


