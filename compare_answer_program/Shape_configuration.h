#ifndef SHAPE_CONFIGURATION_H
#define SHAPE_CONFIGURATION_H

#include <string>
#include <cctype>
#include <algorithm>
#include <map>

class Shape_configuation
{

public:
    void setConfig(std::string shape_type, int center_point_x, int center_point_y, int length_x, int length_y);
    void setShape(int shape_type);
    void setCenter_point(int center_point_x, int center_point_y);
    void setLength(int length_x, int length_y);
    void setCenter_point_x(int center_point_x);
    void setCenter_point_y(int center_point_y);
    void setLength_x(int length_x);
    void setLength_y(int length_y);

    int getShape_type();
    int getCenter_point_x();
    int getCenter_point_y();
    int getLength_x();
    int getLength_y();

private:
    void shape_enum_level();
    int  string2int_shapeType(std::string shape_type);

private:
    enum shape_enum {dot_point, circle, rectangle};
    std::map<std::string, shape_enum> levels;

    int shape_type;
    int center_point_x, center_point_y;
    int length_x,       length_y;
};

#endif // SHAPE_CONFIGURATION_H
