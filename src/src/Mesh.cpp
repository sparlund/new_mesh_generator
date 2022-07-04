#include "../header/Mesh.h"
#include <fstream>

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
double dot(Edge& e1, Edge& e2)
{
    // return the dot product of two vectors
    double ux = e1.A->x - e1.B->x;
    double vx = e2.A->x - e2.B->x;
    double uy = e1.A->y - e1.B->y;
    double vy = e2.A->y - e2.B->y;
    return (ux*vx) + uy*vy;

}

double cross(double i, double j, double k, double l)
{
    // where i,j are the x&y component of vector 1, etc
    // Cross product 2D:
    // | i j |
    // | k l | --> cross(vector(i,j),vector(k,l)) = il - kj
    return ((i*l) - (k*j));
}
double cross(Point* A1,Point* B1,Point* A2,Point* B2)
{

    return cross(A1->x - B1->x,A1->y - B1->y,A2->x - B2->x,A2->y - B2->y);
}
bool Mesh::is_point_to_the_right_of_edge(Point& P, Edge& e)
{
    // Return true if cross(AB, AP)<0, else false because point is to the left of edge
    //    B  x
    //       |
    //       |     x P
    //       |
    //    A  x
    double i = e.B->x - e.A->x;
    double j = P.x - e.A->x;
    double k = e.B->y - e.A->y;
    double l = P.y - e.A->y;
    // if equal to zero they are co-linear. if larger than zero the point is to the left of the line.
    return ((i*l) - (k*l)) < 0;
}
bool Mesh::do_edges_intersect(Edge& e1, Edge& e2)
{
    // for edges to cross one another the two conditions must be true:
    // dot(cross(a1b1,b1b2),cross(a1b1,b1a2)) < 0
    // dot(cross(a2b2,b2b1),cross(a2b2,b2a1)) < 0
    //       x b1
    // b2    |
    // x-----|-----x a2
    //       |
    //       |
    //       x a1
    // index 1 is e1 lol
    // a is A, b is B
    // where a1b1 is e1.A,e1.B
    // ALTERNATIVE METHOD from https://stackoverflow.com/questions/563198/how-do-you-detect-where-two-line-segments-intersect/1968345#1968345
    float p0_x = e1.A->x;
    float p0_y = e1.A->y;

    float p1_x = e1.B->x;
    float p1_y = e1.B->y;

    float p2_x = e2.A->x;
    float p2_y = e2.A->y;

    float p3_x = e2.B->x;
    float p3_y = e2.B->y;

    float s1_x, s1_y, s2_x, s2_y;
    s1_x = p1_x - p0_x;     s1_y = p1_y - p0_y;
    s2_x = p3_x - p2_x;     s2_y = p3_y - p2_y;

    float s, t;
    s = (-s1_y * (p0_x - p2_x) + s1_x * (p0_y - p2_y)) / (-s2_x * s1_y + s1_x * s2_y);
    t = ( s2_x * (p0_y - p2_y) - s2_y * (p0_x - p2_x)) / (-s2_x * s1_y + s1_x * s2_y);

    if (s >= 0 && s <= 1 && t >= 0 && t <= 1)
    {
        // Collision detected
        return true;
    }
    return false;
}

bool sign(double& a)
{
    if(a>0.0d)
    {
        return true;
    }
    return false;
}

bool Mesh::is_quadrilateral_convex(Point* A,Point* B, Point* C, Point* D)
{
    //    D x-------x C
    //     /       /
    //    /       /
    //   x-------x
    //   A       B
    // Return true if the square made up by the two triangles is convex.
    // --> all for sides must get the same sign from equation cross(side_i,side_i+1)
    double sideA_B = cross(A, B, B, C);
    double sideB_C = cross(B, C, C, D);
    double sideC_D = cross(C, D, D, A);
    double sideD_A = cross(D, A, A, B);
    // Check if either all 4 are negative or all 4 positive
    if((sign(sideA_B) == true &&  sign(sideB_C) == true  && sign(sideC_D) == true  && sign(sideD_A) == true) ||
       (sign(sideA_B) == false && sign(sideB_C) == false && sign(sideC_D) == false && sign(sideD_A) == false))
    {
        return true;
    }
    return false;
}

void Mesh::output_to_abaqus_format()
{
    // Print all points as nodes
    // Prints all element as simple trias
    std::string filename = "hej.txt";
    std::ofstream output(filename);
    output << "*NODE" << std::endl;
    for (const auto& p: points)
    {
        output << p->id << "," << p->x << "," << p->y << ", 0.0" << std::endl;
    }
    output << "*ELEMENT, TYPE=CPS3, ELSET=pid" << std::endl;
    for (const auto& T: triangles)
    {
        output << T.id << "," << T.A->id << "," << T.B->id << "," << T.C->id << std::endl;
    }
    output.close();

}