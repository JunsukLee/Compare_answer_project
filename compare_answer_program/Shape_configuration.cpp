#include "Shape_configuration.h"
#include <stdio.h>

void Shape_configuation::setConfig(std::string shape_type, int center_point_x, int center_point_y, int length_x, int length_y)
{
    Shape_configuation::shape_enum_level();
    std::transform(shape_type.begin(), shape_type.end(), shape_type.begin(), tolower); // set lower

    //printf("[str]shape_type : %s\n", shape_type.c_str());
    //printf("[int]shape_type : %d\n", levels[shape_type]);

    //this->shape_type = levels[shape_type];
    this->shape_type = string2int_shapeType(shape_type);
    this->center_point_x = center_point_x;
    this->center_point_y = center_point_y;
    this->length_x       = length_x;
    this->length_y       = length_y;
}

void Shape_configuation::setShape(int shape_type)
{
    this->shape_type     = shape_type;    
}

void Shape_configuation::setCenter_point(int center_point_x, int center_point_y)
{
    this->center_point_x = center_point_x;
    this->center_point_y = center_point_y;
}

void Shape_configuation::setLength(int length_x, int length_y)
{
    this->length_x       = length_x;
    this->length_y       = length_y;
}

void Shape_configuation::setCenter_point_x(int center_point_x)
{
    this->center_point_x = center_point_x;
}

void Shape_configuation::setCenter_point_y(int center_point_y)
{
    this->center_point_y = center_point_y;
}

void Shape_configuation::setLength_x(int length_x)
{
    this->length_x = length_x;
}

void Shape_configuation::setLength_y(int length_y)
{
    this->length_y = length_y;
}


int Shape_configuation::getShape_type()    { return shape_type;     }
int Shape_configuation::getCenter_point_x(){ return this->center_point_x; }
int Shape_configuation::getCenter_point_y(){ return this->center_point_y; }
int Shape_configuation::getLength_x()      { return this->length_x;       }
int Shape_configuation::getLength_y()      { return this->length_y;       }

void Shape_configuation::shape_enum_level()
{
    //enum shape_enum {dot_point, circle, rectangle};
    levels["dot_point"] = dot_point;
    levels["circle"]    = circle;
    levels["rectangle"] = rectangle;
}

int  Shape_configuation::string2int_shapeType(std::string shape_type)
{
    //enum shape_enum {dot_point, circle, rectangle};
    if(!shape_type.compare("dot_point"))
        return 0;
    if(!shape_type.compare("circle"))
        return 1;
    if(!shape_type.compare("rectangle"))
        return 2;
    return -1;
}
