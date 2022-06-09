#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include <unordered_map>

class Point
{
public:
    static size_t id_counter;
    double x,y;
    double normalized_x,normalized_y;
    size_t id;
    Point(double x = 0.0d, double y = 0.0d):x(x), y(y)
    {
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
    friend std::ostream& operator<<(std::ostream& os, Point* P);
};
size_t Point::id_counter = 0;
std::ostream& operator<<(std::ostream& os, Point* P)
{
    os << "("<< P->x << ", " << P->y << ")";
}
class Triangle
{
public:
        Point* A;
        Point* B;
        Point* C;
        size_t id;
        static size_t triangle_counter;
        std::vector<size_t> point_ids;
        Triangle(Point* A_, Point* B_, Point* C_)
        {
            std::cout << "triangle ctor" << std::endl;
            A = A_;
            B = B_;
            C = C_;
            id = triangle_counter;
            triangle_counter++;
            std::cout << "triangle counter = " << triangle_counter << std::endl;
            std::cout << "triangle id = " << id << std::endl;
            point_ids.emplace_back(A->id);
            point_ids.emplace_back(B->id);
            point_ids.emplace_back(C->id);
        };
        Triangle& operator=(const Triangle&) = delete;
        ~Triangle()
        {
            std::cout << "deleting triangle with id = " << id << std::endl;
        };
        friend std::ostream& operator<<(std::ostream& os, Triangle* T);
};
size_t Triangle::triangle_counter = 0;
std::ostream& operator<<(std::ostream& os, Triangle* T)
{
    os << "Triangle id = " << T->id << ":\n  " << T->A->x << ", " << T->A->y << "\n  " << T->B->x << ", " << T->B->y << "\n  " << T->C->x << ", " << T->C->y << std::endl;
}

class Edge
{
public:
    Point* A;
    Point* B;
    Edge(Point* A_, Point* B_)
    {
        A = A_;
        B = B_;
    };
    ~Edge() = default;
};


class Mesh
{
public:
        void add_point(double x, double y)
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
        }
        std::unordered_map<size_t, Point*>      id_2_point;
        std::vector<std::unique_ptr<Point>>     points;
        std::vector<std::unique_ptr<Triangle>>  triangles;
        std::vector<std::unique_ptr<Edge>>      edges;
        double x_max = 0, y_max = 0, x_min = 0, y_min = 0;
        Mesh() {};
        ~Mesh() = default;
        void normalize(std::vector<Point>& points)
        {
        }
        void init_superTriangle()
        {
            double x_mid = (x_max + x_min)/2.0d;
            double y_mid = (y_max + y_min)/2.0d;
            double dx = x_max - x_min;
            double dy = y_max - y_min;
            double dmax = (dx > dy ? dx : dy);
            // std::cout << "x_mid = " << x_mid << std::endl;
            // std::cout << "y_mid = " << y_mid << std::endl;
            // std::cout << "dx = " << dx << std::endl;
            // std::cout << "dy = " << dy << std::endl;
            // std::cout << "dmax = " << dmax << std::endl;
            // Create three points for vertices of the super triangle. Treat it as a normal triangle.
            // We will delete it later.
            auto A = std::make_unique<Point>(x_mid - (2*dmax), y_mid - dmax);
            auto B = std::make_unique<Point>(x_mid, y_mid + (2*dmax));
            auto C = std::make_unique<Point>(x_mid + (2*dmax), y_mid - dmax);
            auto super_triangle = std::make_unique<Triangle>(A.get(), B.get(), C.get());
            id_2_point[A->id] = A.get();
            id_2_point[B->id] = B.get();
            id_2_point[C->id] = C.get();
            std::cout << "superTriangle:\n" << super_triangle.get() << std::endl;
            points.emplace_back(std::move(A));
            points.emplace_back(std::move(B));
            points.emplace_back(std::move(C));
            triangles.emplace_back(std::move(super_triangle));

        }
        void print()
        {
            std::cout << "Points:" << std::endl;

            for (const auto& point: points)
            {
                std::cout << point.get() << std::endl;
            }
        }
        bool is_point_in_circle(Point* P_, Triangle* T)
        {
            // is a point D in the circumference of the circles created by points A, B and C?
            // A, B & C refers to the three points of triangle T
            

            std::cout << "T->A->id = " << T->A->id << std::endl;
            std::cout << "T->B->id = " << T->B->id << std::endl;
            std::cout << "T->C->id = " << T->C->id << std::endl;
            std::cout << "P_->id = "    << P_->id << std::endl;
            for (auto const &pair: id_2_point)
            {
                std::cout << "{" << pair.first << ": " << pair.second << "}\n";
            }
            auto A = id_2_point[T->A->id];
            auto B = id_2_point[T->B->id];
            auto C = id_2_point[T->C->id];
            auto P = id_2_point[P_->id];
            const double dx = A->x - P->x;
            const double dy = A->y - P->y;
            const double ex = B->x - P->x;
            const double ey = B->y - P->y;
            const double fx = C->x - P->x;
            const double fy = C->y - P->y;
            
            const double ap = dx * dx + dy * dy;
            const double bp = ex * ex + ey * ey;
            const double cp = fx * fx + fy * fy;
            return (dx * (ey * cp - bp * fy) -
                    dy * (ex * cp - bp * fx) +
                    ap * (ex * fy - ey * fx)) < 0.0d;
        }
};


// https://www.newcastle.edu.au/__data/assets/pdf_file/0019/22519/23_A-fast-algortithm-for-generating-constrained-Delaunay-triangulations.pdf

int main(int argc, char const *argv[])
{
    Mesh mesh;
    mesh.add_point(0.0d,0.0d);
    mesh.add_point(1.0d,0.0d);
    mesh.add_point(0.5d, 1.0d);
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
    
    https://theor.xyz/mapgen/delaunay-triangulation-triangle-storage/
    for (const auto& point: mesh.points)
    {
        std::cout << "Current point P: id = " << point->id << ", "<< point.get() << std::endl;
        // Iterate over all triangles, Compare against all formed triangles, 
        for(auto it = mesh.triangles.begin(); it != mesh.triangles.end(); it++)
        {
            std::cout << "mesh.triangles.size()=" << mesh.triangles.size() << std::endl;
            if (mesh.is_point_in_circle(point.get(), it->get()))
            {
                std::cout << "P=" << point.get() << " is inside triangle T=" << it->get() << std::endl; 
                // Create 3 triangles from point P to the current triangles points A, B & C
                // *it->get()->
                auto T1 = std::make_unique<Triangle>(point.get(), it->get()->A, it->get()->B);
                auto T2 = std::make_unique<Triangle>(point.get(), it->get()->B, it->get()->C);
                auto T3 = std::make_unique<Triangle>(point.get(), it->get()->A, it->get()->C);
                std::cout << "  Creating triangle T1: \n" << T1.get() << std::endl;
                std::cout << "  Creating triangle T2: \n" << T1.get() << std::endl;
                std::cout << "  Creating triangle T3: \n" << T1.get() << std::endl;
                std::cout << "h1" << std::endl;
                mesh.triangles.emplace_back(std::move(T1));
                std::cout << "h2" << std::endl;
                mesh.triangles.emplace_back(std::move(T2));
                std::cout << "h3" << std::endl;
                mesh.triangles.emplace_back(std::move(T3));
                std::cout << "h4" << std::endl;
                // // Create edge from point P to corners of triangle
                // auto e1 = std::make_unique<Edge>{it->A, it->B};
                // auto e2 = std::make_unique<Edge>{it->B, it->C};
                // auto e3 = std::make_unique<Edge>{it->A, it->A};
                // mesh.edges.push_back(std::move(e1));
                // mesh.edges.push_back(std::move(e2));
                // mesh.edges.push_back(std::move(e3));
                // Delete the current triangle
                // mesh.triangles.
                std::cout << "mesh.triangles.size()=" << mesh.triangles.size() << std::endl;
                // std::cout << "it=" << it->get() << std::endl;
                // it = mesh.triangles.erase(it);
                std::cout << "h5" << std::endl;
            }
            else
            {
                std::cout << "P=" << point.get() << " is NOT inside triangle T=" << it->get() << std::endl; 
                // Continue to next triangle
                std::cout << "e" << std::endl;
            }
        }
    }
    std::cout << "All triangles after algorithm loop: " << std::endl;
    auto counter = 0;
    for (const auto& triangle: mesh.triangles)
    {
        std::cout << "#" << counter << ": " << triangle.get() << std::endl;
    }
    // std::cout << "f" << std::endl;
    // // Delete super triangle and all triangles that have points on it
    // std::cout << "mesh.triangles.size()=" << mesh.triangles.size() << std::endl;
    // mesh.triangles.erase(mesh.triangles.begin());
    // std::cout << "g" << std::endl;
    // for(auto it = mesh.triangles.begin(); it != mesh.triangles.end();)
    // {
    //     std::cout << "h" << std::endl;
    //     for (const auto point: it->points)
    //     {
    //         std::cout << "i" << std::endl;
    //         // Check if current any point on current triangle is shared by the original super triangle
    //         if(std::find(mesh.superTriangle.point_ids.begin(), mesh.superTriangle.point_ids.end(), point.id) != mesh.superTriangle.point_ids.end())
    //         {
    //             std::cout << "j" << std::endl;
    //             mesh.triangles.erase(it);
    //             std::cout << "k" << std::endl;
    //         }
    //     }
    // }
    return 0;
    mesh.print();
}
