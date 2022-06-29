#pragma once

#include "../header/Point.h"
#include "../header/Triangle.h"
#include "../header/Edge.h"

#include <unordered_map>
#include <memory>


class Mesh
{
public:
        void add_point(double x, double y);
        // Mesh owns all points & triangles and how they're connected.
        std::vector<std::unique_ptr<Point>>                  points;
        std::unordered_map<size_t, Point*>                   id_2_point;
        std::vector<std::unique_ptr<Point>>                  superTriangle_points;
        std::vector<size_t>                                  superTriangle_points_ids;
        std::vector<Triangle>               triangles;
        double x_max = 0, y_max = 0, x_min = 0, y_min = 0;
        double normaliziation_factor_x, normaliziation_factor_y;
        Mesh() = default;
        ~Mesh() = default;
        void normalize();
        void init_superTriangle();
        void print();
        bool is_point_in_circle(Point& P_, Triangle& T) const;
        void swap(Point*, Triangle*);
        bool are_edges_equal(Edge&, Edge&);
        static bool is_point_to_the_right_of_edge(Point&, Edge&);
};