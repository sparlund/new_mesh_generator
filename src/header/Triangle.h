#pragma once

#include "Point.h"
#include "Edge.h"

#include <vector>
#include <iostream>

class Triangle
{
public:
        Point* A;
        Point* B;
        Point* C;
        size_t id;
        bool is_bad = false;
        static size_t triangle_counter;
        std::vector<size_t> point_ids;
        // Rule of 5
        Triangle() = default;
        Triangle(const Triangle&) = default;
        Triangle(Triangle&&) = default;
        Triangle(Point* A_,Point* B_,Point* C_)
        :A(A_), B(B_), C(C_)
        {
            id = triangle_counter;
            triangle_counter++;
            std::cout << "Creating triangle id " << id << ", A->B->C id: " << A->id << "->" << B->id << "->" << C->id << std::endl;
        };
        Triangle& operator=(const Triangle&) = default;
        ~Triangle()
        {
            std::cout << "Deleting triangle with id " << id << std::endl;
        };
        bool contains_point(Point& P)
        {
            if (P.id == A->id | P.id == B->id | P.id == C->id)
            {
                return true;
            }
            return false;
        }
        friend std::ostream& operator<<(std::ostream& os, Triangle& T);
};