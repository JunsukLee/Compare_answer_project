#include "CompareToPolygon.hpp"

void CompareToPolygon::setPolygonData(bool type, int shape_type, int center_point_x, int center_point_y, int length_x, int length_y)
{
    if(shape_type == 0 || shape_type == 1 || shape_type == 2){
        if(type){
            Answer_shape_type = shape_type;
            Answer_center_point_x = center_point_x;
            Answer_center_point_y = center_point_y;
            Answer_length_x = length_x;
            Answer_length_y = length_y;
        }else{
            Test_shape_type = shape_type;
            Test_center_point_x = center_point_x;
            Test_center_point_y = center_point_y;
            Test_length_x = length_x;
            Test_length_y = length_y;
        }

        CompareToPolygon::convertPolygon_OF_clipperForm(type, shape_type, center_point_x, center_point_y, length_x, length_y);

    }else{
        printf("Error!! CompareToPolygon::setPolygonData => shape_type (0 or 1 or 2)\n\n");
        return;
    }
}

double CompareToPolygon::OverlapArea()
{
    if(Answer_polygon.empty()){
        printf("Error!! Answer_polygon is empty!\n\n");
        return -1;
    }
    if(Test_polygon.empty()){
        printf("Error!! Test_polygon is empty!\n\n");
        return -1;
    }

    ClipperLib::Clipper clipper;
    ClipperLib::Paths intersection;

    clipper.AddPath(Answer_polygon, ClipperLib::ptSubject, true);
    clipper.AddPath(Test_polygon, ClipperLib::ptClip, true);
    clipper.Execute(ClipperLib::ctIntersection, intersection, ClipperLib::pftNonZero, ClipperLib::pftNonZero);


    double answer_polygon_area = fabs(ClipperLib::Area(Answer_polygon));
    double test_polygon_area = fabs(ClipperLib::Area(Test_polygon));;
    double intersection_area = 0;

    for(int i = 0; i < (int)intersection.size(); i++){
        intersection_area += fabs(ClipperLib::Area(intersection.at(i)));
    }

    if(intersection.size() > 0 && answer_polygon_area !=0 && test_polygon_area != 0){        
        double score = intersection_area / ( answer_polygon_area > test_polygon_area ? answer_polygon_area : test_polygon_area );
        return score;
    }

    return 0;
}

void CompareToPolygon::convertPolygon_OF_clipperForm(bool type, int shape_type, int center_point_x, int center_point_y, int length_x, int length_y)
{
    ClipperLib::Path temp_polygon;

    if(shape_type == 1) // circle
    {
        temp_polygon = CompareToPolygon::circle2polygon(center_point_x, center_point_y, length_x/2, PARTITION_ANGLE);
    }else if(shape_type == 2) // rectangle
    {
        temp_polygon =CompareToPolygon::rect2polygon(center_point_x, center_point_y, length_x, length_y);
    }

    if(type){
        Answer_polygon = temp_polygon;
    }
    if(!type){
        Test_polygon   = temp_polygon;
    }
}

ClipperLib::Path CompareToPolygon::circle2polygon(const int center_x, const int center_y, const double r, const int partition_angle)
{
    double x_coordinate = 0.0;
    double y_coordinate = 0.0;
    ClipperLib::Path circle_polygon;

    for(int radian=0; radian <= 360 ; radian += partition_angle ){
        x_coordinate = center_x + ((r * cos(radian*(PI/180)))) * ZOOMIN;
        y_coordinate = center_y + ((r * sin(radian*(PI/180)))) * ZOOMIN;

        circle_polygon.push_back(ClipperLib::IntPoint((int)x_coordinate, (int)y_coordinate));
    }

    return circle_polygon;
}

ClipperLib::Path CompareToPolygon::rect2polygon(const int center_x, const int center_y, const int length_x, const int length_y)
{
    ClipperLib::Path rectangle_polygon;
    int coordinate11[2];
    int coordinate12[2];
    int coordinate21[2];
    int coordinate22[2];

    coordinate11[0] = center_x - (length_x/2) * ZOOMIN;
    coordinate11[1] = center_y - (length_y/2) * ZOOMIN;

    coordinate12[0] = center_x + (length_x/2) * ZOOMIN;
    coordinate12[1] = center_y - (length_y/2) * ZOOMIN;

    coordinate21[0] = center_x + (length_x/2) * ZOOMIN;
    coordinate21[1] = center_y + (length_y/2) * ZOOMIN;

    coordinate22[0] = center_x - (length_x/2) * ZOOMIN;
    coordinate22[1] = center_y + (length_y/2) * ZOOMIN;

    rectangle_polygon.push_back(ClipperLib::IntPoint(coordinate11[0], coordinate11[1]));
    rectangle_polygon.push_back(ClipperLib::IntPoint(coordinate12[0], coordinate12[1]));
    rectangle_polygon.push_back(ClipperLib::IntPoint(coordinate21[0], coordinate21[1]));
    rectangle_polygon.push_back(ClipperLib::IntPoint(coordinate22[0], coordinate22[1]));

    return rectangle_polygon;
}
void CompareToPolygon::init()
{
    Answer_shape_type = 0;
    Answer_center_point_x = Answer_center_point_y = 0;
    Answer_length_x       = Answer_length_y = 0;
    Answer_polygon.clear();

    Test_shape_type = 0;
    Test_center_point_x = Test_center_point_y = 0;
    Test_length_x       = Test_length_y = 0;
    Test_polygon.clear();
}
