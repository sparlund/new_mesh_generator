#pragma once

#include "../header/Point.h"
#include "../header/Triangle.h"
#include "../header/Edge.h"

#include <unordered_map>
#include <memory>


// TODO: move over implementation to cpp file
// Used for hashing pairs of Points
class Hasch_point_pair
{
public:
    size_t operator()(const std::pair<Point*, Point*> p) const
    {
        auto h1 = std::hash<size_t>()(p.first->id);
        auto h2 = std::hash<size_t>()(p.second->id);
        // XOR hash. Overly simplistic but we don't care about hashing we're FREAKS
        return h1^h2; 
    }
};
// Used for making a pair of points similar either way you create them:
// map[point1,point2] == map[point2,point1]
class Equal_point_comparator
{
public:
    bool operator()(const std::pair<Point*, Point*> p1, const std::pair<Point*, Point*> p2) const
    {
        const bool p1Asc = p1.first->id < p1.second->id;
        const bool p2Asc = p2.first->id < p2.second->id;
        return p1Asc == p2Asc ? 
               *p1.first == *p2.first && *p1.second == *p2.second :
               *p1.first == *p2.second && *p1.second == *p2.first;
    }
};



class Mesh
{
public:
        void add_point(double x, double y);
        // Mesh owns all points & triangles and how they're connected.
        std::unordered_map<size_t, Point*>                   id_2_point;
        std::vector<std::unique_ptr<Point>>                  points;
        std::vector<std::unique_ptr<Point>>                  superTriangle_points;
        std::vector<size_t>                                  superTriangle_points_ids;
        std::vector<std::unique_ptr<Triangle>>               triangles;
        std::vector<std::unique_ptr<Edge>>                   edges;
        std::unordered_map<Edge*, std::vector<Triangle*>>    edge_2_Triangles;
        std::unordered_map<std::pair<Point*, Point*>, Edge*, Hasch_point_pair, Equal_point_comparator> points_2_edge;
        double x_max = 0, y_max = 0, x_min = 0, y_min = 0;
        double normaliziation_factor_x, normaliziation_factor_y;
        Mesh() = default;
        ~Mesh() = default;
        void normalize();
        void init_superTriangle();
        Edge* get_edge(Point* p1, Point* p2);
        bool edge_exists(Point* p1, Point* p2);
        void add_triangle_to_mesh(std::unique_ptr<Triangle> T);
        void remove_triangle_from_mesh(std::unique_ptr<Triangle> T);
        void print();
        bool is_point_in_circle(Point& P_, Triangle* T) const;
        void swap(Point*, Triangle*);
};