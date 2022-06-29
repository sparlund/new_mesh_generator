#include "../header/Mesh.h"


void Mesh::add_point(double x, double y)
{
    if(x > x_max)
    {
        x_max = x;
    }
    if(y > y_max)
    {
        y_max = y;
    }
    if(x < x_min)
    {
        x_min = x;
    }
    if(y < y_min)
    {
        y_min = y;
    }
    auto temp = std::make_unique<Point>(x, y);
    id_2_point[temp->id] = temp.get();
    std::cout << "Adding id " << temp->id << " to id_2_point, --> id_2_point["<< temp->id << "]" << "=" << temp.get() << std::endl;
    points.emplace_back(std::move(temp));
};

void Mesh::normalize()
{
    // Want to normalize between -1 to 1 in both x and y
    // Formula for normalization as function of x: (2*(x - x_min)/(x_max - x_min)) - 1
    for (auto& point: points)
    {
        std::cout << "--- " << std::endl;
        std::cout << "normalizaton of point " << *point << std::endl;
        point->x = (2*(point->original_x - x_min)/(x_max - x_min)) - 1;
        std::cout << "x before: " << point->original_x << ", after: " << point->x << std::endl;
        point->y = (2*(point->original_y - y_min)/(y_max - y_min)) - 1;
        std::cout << "y before: " << point->original_y << ", after: " << point->y << std::endl;
        std::cout << "--- " << std::endl;
    }
};

void Mesh::init_superTriangle()
{
    // With normalized points the super triangle will be the same size every time!
    auto A = std::make_unique<Point>(-3.0d,-3.0d);
    std::cout << "Adding id " << A->id << " to id_2_point, --> id_2_point["<< A->id << "]" << "=" << A.get() << std::endl;
    id_2_point[A->id] = A.get();
    
    auto B = std::make_unique<Point>(3.0d,-3.0d);
    std::cout << "Adding id " << B->id << " to id_2_point, --> id_2_point["<< B->id << "]" << "=" << B.get() << std::endl;
    id_2_point[B->id] = B.get();
    
    auto C = std::make_unique<Point>(0.0d,3.0);
    std::cout << "Adding id " << C->id << " to id_2_point, --> id_2_point["<< C->id << "]" << "=" << C.get() << std::endl;
    id_2_point[C->id] = C.get();
    // double x_mid = (x_max + x_min)/2.0d;
    // double y_mid = (y_max + y_min)/2.0d;
    // double dx = x_max - x_min;
    // double dy = y_max - y_min;
    // double dmax = (dx > dy ? dx : dy);
    // std::cout << "x_mid = " << x_mid << std::endl;
    // std::cout << "y_mid = " << y_mid << std::endl;
    // std::cout << "dx = " << dx << std::endl;
    // std::cout << "dy = " << dy << std::endl;
    // std::cout << "dmax = " << dmax << std::endl;
    // Create three points for vertices of the super triangle. Treat it as a normal triangle.
    // We will delete it later.
    // auto A = std::make_unique<Point>(x_mid - (2*dmax), y_mid - dmax);
    // auto B = std::make_unique<Point>(x_mid, y_mid + (2*dmax));
    // auto C = std::make_unique<Point>(x_mid + (2*dmax), y_mid - dmax);
    auto super_triangle = Triangle{A.get(), B.get(), C.get()};
    // this is retarted, TODO
    super_triangle.A->x = A->original_x;
    super_triangle.A->y = A->original_y;
    super_triangle.B->x = B->original_x;
    super_triangle.B->y = B->original_y;
    super_triangle.C->x = C->original_x;
    super_triangle.C->y = C->original_y;

    superTriangle_points.emplace_back(std::move(A));
    superTriangle_points.emplace_back(std::move(B));
    superTriangle_points.emplace_back(std::move(C));
    // The supertriangle's edges should not be considered real edges,
    // so let's not call add_triangle_to_mesh() but rather add it manually.
    // add_triangle_to_mesh(std::move(super_triangle));
    triangles.push_back(std::move(super_triangle));
    std::cout << " dödar vi triangel nu för den går out of scope?" << std::endl;
};

void Mesh::print()
{
    std::cout << "Points:" << std::endl;

    for (auto& point: points)
    {
        std::cout << *point << std::endl;
    }
    std::cout << "Triangles:" << std::endl;
    for (auto& triangle: triangles)
    {
        std::cout << triangle << std::endl;
    }
};
bool Mesh::is_point_in_circle(Point& P_, Triangle& T) const
{
    // is a point D in the circumference of the circles created by points A, B and C?
    // A, B & C refers to the three points of triangle T
    auto A = id_2_point.at(T.A->id);
    auto B = id_2_point.at(T.B->id);
    auto C = id_2_point.at(T.C->id);
    auto P = id_2_point.at(P_.id);
    const double ax = A->x - P->x;
    const double ay = A->y - P->y;
    const double bx = B->x - P->x;
    const double by = B->y - P->y;
    const double cx = C->x - P->x;
    const double cy = C->y - P->y;
    return ((ax*ax + ay*ay) * (bx*cy-cx*by) -
            (bx*bx + by*by) * (ax*cy-cx*ay) +
            (cx*cx + cy*cy) * (ax*by-bx*ay)
        ) > 0.0d;
};

void Mesh::swap(Point*, Triangle*)
{


};

bool Mesh::are_edges_equal(Edge& e1, Edge& e2)
{
    if ((e1.A->id == e2.A->id && e1.B->id == e2.B->id) || (e1.A->id == e2.B->id && e1.B->id == e2.A->id))
    {
        return true;
    }
    return false;
};
bool Mesh::is_point_to_the_right_of_edge(Point&, Edge&)
{
    // Return true if cross(AB, AP)<0, else false because point is to the left of edge
    // float AB = (P.y-A.y)*(A.x-A.x) - (P.x-A.x)*(A.y-A.y);
    // float BC = (P.y-B.y)*(B.x-B.x) - (P.x-B.x)*(B.y-B.y);
    // float CA = (P.y-C.y)*(C.x-C.x) - (P.x-C.x)*(C.y-C.y);
    // Cross product 2D:
    // | i j |
    // | k l | --> cross(vector(i,j),vector(k,l)) = il - kj

}