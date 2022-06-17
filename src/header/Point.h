#pragma once
#include <iostream>

class Point
{
public:
    static size_t id_counter;
    // normalized values
    double x,y;
    const double original_x,original_y;
    size_t id;
    Point(double x_ = 0.0d, double y_ = 0.0d):original_x(x_), original_y(y_)
    {
        std::cout << "Point ctor" << std::endl;
        id = id_counter;
        id_counter++;
    };
    Point& operator=(const Point&) = default;
    ~Point()
    {
        std::cout << "deleting point with id = " << id << std::endl;
    };
    bool operator==(const Point& rhs)
    {
        return id == rhs.id;
    }
    friend std::ostream& operator<<(std::ostream& os, Point& P);
};