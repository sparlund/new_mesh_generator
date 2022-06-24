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
        bool is_bad;
        // It makes sense that the triangle itself owns it's edges because
        // when a triangle is deleted those edges should disappear
        // But how do we look up who shares that edge..
        Edge* edge1,edge2,edge3;
        bool connected_to_super_triangle = false;
        static size_t triangle_counter;
        std::vector<size_t> point_ids;
        Triangle(Point* A_, Point* B_, Point* C_)
        {
            A = A_;
            B = B_;
            C = C_;
            // When creating a new triangle, we want to see what adjacent triangles it has
            id = triangle_counter;
            triangle_counter++;
            std::cout << "Creating triangle id " << id << ", A->B->C id: " << A->id << "->" << B->id << "->" << C->id << std::endl;
            point_ids.emplace_back(A->id);
            point_ids.emplace_back(B->id);
            point_ids.emplace_back(C->id);
        };
        Triangle& operator=(const Triangle&) = delete;
        ~Triangle()
        {
            std::cout << "Deleting triangle with id " << id << std::endl;
        };
        friend std::ostream& operator<<(std::ostream& os, Triangle& T);
};