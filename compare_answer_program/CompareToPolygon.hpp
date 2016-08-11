#ifndef COMPARETOPOLYGON_HPP
#define COMPARETOPOLYGON_HPP

#include <stdio.h>
#include <math.h>
#include <map>

#include "clipper.hpp"

#define PARTITION_ANGLE 1
#define ZOOMIN          1
#define PI              3.141592


class CompareToPolygon
{

public:
    // true : answer_sheet , false : test_sheet
    void setPolygonData(bool type, int shape_type, int center_point_x, int center_point_y, int length_x, int length_y);
    double OverlapArea();
    void init();

private:
    void convertPolygon_OF_clipperForm(bool type, int shape_type, int center_point_x, int center_point_y, int length_x, int length_y);
    ClipperLib::Path circle2polygon(const int center_x, const int center_y, const double r, const int partition_angle);
    ClipperLib::Path rect2polygon(const int center_x, const int center_y, int length_x, int length_y);

private:
    //const int PARTITION_ANGLE = 1;
    //const int ZOOMIN = 1000;
    //const double PI  = 3.141592;

private:
    enum shape_enum {dot_point, circle, rectangle};
    std::map<std::string, shape_enum> levels;

    //Answer_polygon
    int Answer_shape_type;
    int Answer_center_point_x, Answer_center_point_y;
    int Answer_length_x,       Answer_length_y;

    ClipperLib::Path Answer_polygon;


    //Test_polygon
    int Test_shape_type;
    int Test_center_point_x, Test_center_point_y;
    int Test_length_x,       Test_length_y;

    ClipperLib::Path Test_polygon;



};

#endif // COMPARETOPOLYGON_HPP
