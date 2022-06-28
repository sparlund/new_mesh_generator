#pragma once
#include <iostream>

class Point
{
public:
    static size_t id_counter;
    // normalized values
    double x,y;
    // inital values before normalization
    double original_x,original_y;
    size_t id;
    Point(double x_ = 0.0d, double y_ = 0.0d):original_x(x_), original_y(y_)
    {
        id = id_counter;
        std::cout << "Point ctor, id = " << id << ", at x=" << original_x << ", y=" << original_y  << std::endl;
        id_counter++;
    };
    Point& operator=(const Point&) = default;
    ~Point()
    {
        std::cout << "Point dtor for id = " << id << std::endl;
    };
    bool operator==(const Point& rhs)
    {
        return id == rhs.id;
    }
    friend std::ostream& operator<<(std::ostream& os, Point& P);
};