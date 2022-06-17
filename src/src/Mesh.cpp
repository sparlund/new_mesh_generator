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
    auto B = std::make_unique<Point>(3.0d,-3.0d);
    auto C = std::make_unique<Point>(0.0d,3.0);
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
    auto super_triangle = std::make_unique<Triangle>(A.get(), B.get(), C.get());
    // this is retarted, TODO
    super_triangle->A->x = A->original_x;
    super_triangle->A->y = A->original_y;
    super_triangle->B->x = B->original_x;
    super_triangle->B->y = B->original_y;
    super_triangle->C->x = C->original_x;
    super_triangle->C->y = C->original_y;
    super_triangle->connected_to_super_triangle = true;
    id_2_point[A->id] = A.get();
    id_2_point[B->id] = B.get();
    id_2_point[C->id] = C.get();
    std::cout << "superTriangle:\n" << super_triangle.get() << std::endl;
    superTriangle_points_ids.insert(superTriangle_points_ids.begin(), {A->id, B->id, C->id});

    superTriangle_points.emplace_back(std::move(A));
    superTriangle_points.emplace_back(std::move(B));
    superTriangle_points.emplace_back(std::move(C));
    // The supertriangle's edges should not be considered real edges,
    // so let's not call add_triangle_to_mesh() but rather add it manually.
    add_triangle_to_mesh(std::move(super_triangle));
    // triangles.emplace_back(std::move(super_triangle));
};

Edge* Mesh::get_edge(Point* p1, Point* p2)
{
    for(const auto& edge: edges)
    {
        if ((p1->id == edge->A->id && p2->id == edge->B->id) || (p2->id == edge->A->id && p1->id == edge->B->id))
        {
            // this edge already exists!
            std::cout << "Edge " << p1->id << " to " << p2->id << " exist!" << std::endl;
            return edge.get();
        }
    }
    return nullptr;
};

bool Mesh::edge_exists(Point* p1, Point* p2)
{
    // This is stupid! Should use a hash map, something like points_2_edge that takes a pair of 
    // points and return the edge if it exists.
    for(const auto& edge: edges)
    {
        if ((p1->id == edge->A->id && p2->id == edge->B->id) || (p2->id == edge->A->id && p1->id == edge->B->id))
        {
            // this edge already exists!
            std::cout << "Edge " << p1->id << " to " << p2->id << " already exist!" << std::endl;
            return true;
        }
    }
    return false;
};
void Mesh::add_triangle_to_mesh(std::unique_ptr<Triangle> T)
{
    // Go through all edges of triangle T and add to map.
    // First check which this edge already exists.
    if(!edge_exists(T->A,T->B))
    {
        // Does not matter which order I give points to the ctor for an Edge
        auto edge1 = std::make_unique<Edge>(T->A,T->B);
        std::cout << "Creating edge " << T->A->id << " to " << T->B->id << std::endl;
        auto pair = std::pair<Point*, Point*>(T->A,T->B);
        points_2_triangle[pair] = T.get();
        edges.emplace_back(std::move(edge1));
    }
    if(!edge_exists(T->B,T->C))
    {
        auto edge2 = std::make_unique<Edge>(T->B,T->C);
        std::cout << "Creating edge " << T->B->id << " to " << T->C->id << std::endl;
        auto pair = std::pair<Point*, Point*>(T->B,T->C);
        points_2_triangle[std::move(pair)] = T.get();
        edges.emplace_back(std::move(edge2));
    }
    if(!edge_exists(T->A,T->C))
    {
        auto edge3 = std::make_unique<Edge>(T->A,T->C);
        std::cout << "Creating edge " << T->A->id << " to " << T->C->id << std::endl;
        auto pair = std::pair<Point*, Point*>(T->A,T->C);
        points_2_triangle[std::move(pair)] = T.get();
        edges.emplace_back(std::move(edge3));
    }
    triangles.emplace_back(std::move(T));
};

void Mesh::print()
{
    std::cout << "Points:" << std::endl;

    for (auto& point: points)
    {
        std::cout << *point << std::endl;
    }
    std::cout << "Triangles:" << std::endl;
    for (const auto& triangle: triangles)
    {
        std::cout << triangle.get() << std::endl;
    }
};
bool Mesh::is_point_in_circle(Point& P_, Triangle* T) const
{
    // is a point D in the circumference of the circles created by points A, B and C?
    // A, B & C refers to the three points of triangle T
    auto A = id_2_point.at(T->A->id);
    auto B = id_2_point.at(T->B->id);
    auto C = id_2_point.at(T->C->id);
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

