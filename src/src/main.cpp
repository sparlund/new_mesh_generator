#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

class Point
{
public:
    double x,y;
    double normalized_x,normalized_y;
    size_t id;
    Point(double x = 0.0d, double y = 0.0d):x(x), y(y){};
    Point& operator=(const Point&) = default;
    ~Point() = default;
    bool operator==(const Point& rhs)
    {
        return id == rhs.id;
    }
    friend std::ostream& operator<<(std::ostream& os, const Point& P);
};
std::ostream& operator<<(std::ostream& os, const Point& P)
{
    os << "("<< P.x << ", " << P.y << ")";
}
class Triangle
{
public:
        Point A, B, C;
        std::vector<Point> points;
        std::vector<size_t> point_ids;
        Triangle(Point A = Point{}, Point B = Point{}, Point C = Point{}) : A(A), B(B), C(C)
        {
            points.push_back(A);
            points.push_back(B);
            points.push_back(C);
            point_ids.push_back(A.id);
            point_ids.push_back(B.id);
            point_ids.push_back(C.id);
        };
        Triangle& operator=(const Triangle&) = default;
        ~Triangle() = default;
        friend std::ostream& operator<<(std::ostream& os, const Triangle& T);
};
std::ostream& operator<<(std::ostream& os, const Triangle& T)
{
    os << T.A << ", " << T.B << ", " << ", " << T.C; 
}

class Edge
{
public:
    Point A, B;
    Edge(Point A, Point B) : A(A), B(B) {};
    ~Edge() = default;
};


class Mesh
{
private:
        void add_point(Point p)
        {
            points.emplace_back(Point{p.x, p.y});
            if(p.x > x_max)
            {
                x_max = p.x;
            }
            if(p.y > y_max)
            {
                y_max = p.y;
            }
            if(p.x < x_min)
            {
                x_min = p.x;
            }
            if(p.y < y_min)
            {
                y_min = p.y;
            }
        }
public:
        std::vector<std::unique_ptrPoint> points;
        std::vector<std::unique_ptr<Triangle>> triangles;
        std::vector<Edge> edges;
        Triangle superTriangle;
        double x_max = 0, y_max = 0, x_min = 0, y_min = 0;
        Mesh() = default;
        ~Mesh() = default;
        void add_triangle(Point A, Point B, Point C)
        {
            auto t = Triangle{A, B, C};
            triangles.push_back(t);
        }
        void normalize(std::vector<Point>& points)
        {
        }
        void init_superTriangle()
        {
            double x_mid = (x_max + x_max)/2.0d;
            double y_mid = (y_max + y_max)/2.0d;
            double dx = x_max - x_min;
            double dy = y_max - y_min;
            double dmax = (dx > dy ? dx : dy) * 20.0d;
            auto A = Point{x_mid - dmax, y_mid - dmax};
            auto B = Point{x_mid, y_mid + dmax};
            auto C = Point{x_mid + dmax, y_mid - dmax};
            superTriangle.A = A;
            superTriangle.B = B;
            superTriangle.C = C;
            triangles.push_back(superTriangle);
        }
        void print()
        {
            std::cout << "Points:" << std::endl;

            for (const auto& point: points)
            {
                std::cout << point << std::endl;
            }
            
            std::cout << "superTriangle:\n" << superTriangle << std::endl;

        }
};

bool is_point_in_circle(const Point& P, const Triangle& T)
{
    // is a point D in the circumference of the circles created by points A, B and C?
    // A, B & C refers to the three points of triangle T
    const double dx = T.A.x - P.x;
    const double dy = T.A.y - P.y;
    const double ex = T.B.x - P.x;
    const double ey = T.B.y - P.y;
    const double fx = T.C.x - P.x;
    const double fy = T.C.y - P.y;
    
    const double ap = dx * dx + dy * dy;
    const double bp = ex * ex + ey * ey;
    const double cp = fx * fx + fy * fy;
    return (dx * (ey * cp - bp * fy) -
            dy * (ex * cp - bp * fx) +
            ap * (ex * fy - ey * fx)) < 0.0d;
}

// https://www.newcastle.edu.au/__data/assets/pdf_file/0019/22519/23_A-fast-algortithm-for-generating-constrained-Delaunay-triangulations.pdf

int main(int argc, char const *argv[])
{
    Point A{0.0d,0.0d}, B{1.0d,0.0d}, C{0.5d, 1.0d}, D{1.0d,1.0d};
    Mesh mesh;
    mesh.add_point(A);
    mesh.add_point(B);
    mesh.add_point(C);
    // mesh.add_point(D);
    mesh.init_superTriangle();
    mesh.print();
    // Delunat triangulation using the Sloan method
    // 1. Normalize coordinates of points so they're all between 0.1
    //    This is because of less affect of rounding errors and convenience
    // 2. Sort into bins by making a grid over the space (optional)
    // 3. Establish the super triangle
    //    This supertriangle initially defines a Delaunay
    //    triangulation which is comprised of a single triangle.
    //    Its vertices are defined in terms of normalized coordi-
    //    nates and are usually located at a considerable dis-
    //    tance from the window which encloses the set of points.
    // 4. Loop over each point
    // 5. Find a triangle which encloses current point P
    //    Delete this triangle and form three new triangles
    //    by connecting P to each of the three vertices.
    // 6. Place all triangles which are adjacent to the edges
    //    opposite P on a last-in-first-out stack.
    //    There is a maximum of three such triangles.

    // Loop over each point
    std::cout << "Number of points: " << mesh.points.size() << std::endl;
    std::cout << "All triangles: " << std::endl;
    auto counter = 0;
    for (const auto& triangle: mesh.triangles)
    {
        std::cout << "#" << counter << ": " << triangle << std::endl;
    }
    

    for (const auto& point: mesh.points)
    {
        std::cout << "Current point P = " << point << std::endl;
        // Iterate over all triangles, Compare against all formed triangles, 
        for(auto it = mesh.triangles.begin(); it != mesh.triangles.end(); it++)
        {
            std::cout << "mesh.triangles.size()=" << mesh.triangles.size() << std::endl;
            if (is_point_in_circle(point, *it))
            {
                std::cout << "P=" << point << " is inside triangle T=" << *it << std::endl; 
                // Create 3 triangles from point P to the current triangles points A, B & C
                Triangle T1{point, it->A, it->B};
                Triangle T2{point, it->B, it->C};
                Triangle T3{point, it->C, it->A};
                std::cout << "  Creating triangle T1: " << T1 << std::endl;
                std::cout << "  Creating triangle T2: " << T1 << std::endl;
                std::cout << "  Creating triangle T3: " << T1 << std::endl;
                mesh.triangles.push_back(T1);
                mesh.triangles.push_back(T2);
                mesh.triangles.push_back(T3);
                // Create edge from point P to corners of triangle
                mesh.edges.push_back(Edge{it->A, it->B});
                mesh.edges.push_back(Edge{it->B, it->C});
                mesh.edges.push_back(Edge{it->A, it->A});
                // Delete the current triangle
                mesh.triangles.erase(it);
            }
            else
            {
                std::cout << "P=" << point << " is NOT inside triangle T=" << *it << std::endl; 
                // Continue to next triangle
                std::cout << "e" << std::endl;
            }
        }
    }
    std::cout << "f" << std::endl;
    // Delete super triangle and all triangles that have points on it
    std::cout << "mesh.triangles.size()=" << mesh.triangles.size() << std::endl;
    mesh.triangles.erase(mesh.triangles.begin());
    std::cout << "g" << std::endl;
    for(auto it = mesh.triangles.begin(); it != mesh.triangles.end();)
    {
        std::cout << "h" << std::endl;
        for (const auto point: it->points)
        {
            std::cout << "i" << std::endl;
            // Check if current any point on current triangle is shared by the original super triangle
            if(std::find(mesh.superTriangle.point_ids.begin(), mesh.superTriangle.point_ids.end(), point.id) != mesh.superTriangle.point_ids.end())
            {
                std::cout << "j" << std::endl;
                mesh.triangles.erase(it);
                std::cout << "k" << std::endl;
            }
        }
    }
    return 0;
    mesh.print();
}
