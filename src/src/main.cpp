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
    // normalized values
    double x,y;
    const double original_x,original_y;
    size_t id;
    Point(double x_ = 0.0d, double y_ = 0.0d):original_x(x_), original_y(y_)
    {
        std::cout << "Point ctor" << std::endl;
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
        bool connected_to_super_triangle = false;
        static size_t triangle_counter;
        std::vector<size_t> point_ids;
        Triangle(Point* A_, Point* B_, Point* C_)
        {
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
            std::cout << "Deleting triangle with id " << id << std::endl;
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
        std::vector<std::unique_ptr<Point>>     superTriangle_points;
        std::vector<size_t>    superTriangle_points_ids;
        std::vector<std::unique_ptr<Triangle>>  triangles;
        std::vector<std::unique_ptr<Edge>>      edges;
        double x_max = 0, y_max = 0, x_min = 0, y_min = 0;
        double normaliziation_factor_x, normaliziation_factor_y;
        Mesh() {};
        ~Mesh() = default;
        void normalize()
        {
            // Want to normalize between -1 to 1 in both x and y
            // Formula for normalization as function of x: (2*(x - x_min)/(x_max - x_min)) - 1
            for (auto& point: points)
            {
                std::cout << "--- " << point.get() << std::endl;
                std::cout << "normalizaton of point " << point.get() << std::endl;
                point->x = (2*(point->original_x - x_min)/(x_max - x_min)) - 1;
                std::cout << "x before: " << point->original_x << ", after: " << point->x << std::endl;
                point->y = (2*(point->original_y - y_min)/(y_max - y_min)) - 1;
                std::cout << "y before: " << point->original_y << ", after: " << point->y << std::endl;
                std::cout << "--- " << point.get() << std::endl;
            }
            
        }
        void init_superTriangle()
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

            id_2_point[A->id] = A.get();
            id_2_point[B->id] = B.get();
            id_2_point[C->id] = C.get();
            std::cout << "superTriangle:\n" << super_triangle.get() << std::endl;
            superTriangle_points_ids.insert(superTriangle_points_ids.begin(), {A->id, B->id, C->id});
            superTriangle_points.emplace_back(std::move(A));
            superTriangle_points.emplace_back(std::move(B));
            superTriangle_points.emplace_back(std::move(C));
            triangles.emplace_back(std::move(super_triangle));

        }
        void print()
        {
            std::cout << "Points:" << std::endl;

            for (const auto& point: points)
            {
                std::cout << point.get() << std::endl;
            }
            std::cout << "Triangles:" << std::endl;
            for (const auto& triangle: triangles)
            {
                std::cout << triangle.get() << std::endl;
            }
        }
        bool is_point_in_circle(Point* P_, Triangle* T)
        {
            // is a point D in the circumference of the circles created by points A, B and C?
            // A, B & C refers to the three points of triangle T
            auto A = id_2_point[T->A->id];
            auto B = id_2_point[T->B->id];
            auto C = id_2_point[T->C->id];
            auto P = id_2_point[P_->id];
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
    }
};


// https://www.newcastle.edu.au/__data/assets/pdf_file/0019/22519/23_A-fast-algortithm-for-generating-constrained-Delaunay-triangulations.pdf

int main(int argc, char const *argv[])
{
    Mesh mesh;
    mesh.add_point(-4.0d,-4.0dd);
    mesh.add_point(4.0d,-4.0d);
    mesh.add_point(0.0d, 4.0d);
    mesh.add_point(4.0d, 4.0d);
    mesh.normalize();
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
        // Resizing a std::vector invalidates the iterator. Need to either switch to
        // std::list OR reserve enough size at before we take out the iterator.
        mesh.triangles.reserve(1e3);
        auto it = mesh.triangles.begin();
        // Iterate over all triangles, Compare against all formed triangles, 
        while(it != mesh.triangles.end())
        {
            std::cout << " Checking P against current triangle: " << it->get() << std::endl;
            if (mesh.is_point_in_circle(point.get(), it->get()))
            {
                std::cout << "P=" << point.get() << " is inside triangle T=" << it->get() << std::endl; 
                // Create 3 triangles from point P to the current triangles points A, B & C
                auto T1 = std::make_unique<Triangle>(point.get(), it->get()->A, it->get()->B);
                auto T2 = std::make_unique<Triangle>(point.get(), it->get()->B, it->get()->C);
                auto T3 = std::make_unique<Triangle>(point.get(), it->get()->A, it->get()->C);
                std::cout << "  Creating triangle T1: \n" << T1.get() << std::endl;
                std::cout << "  Creating triangle T2: \n" << T2.get() << std::endl;
                std::cout << "  Creating triangle T3: \n" << T3.get() << std::endl;
                for (auto& t : {T1.get(), T2.get(), T3.get()})
                {
                    // Check if any points on newly formed triangle is from the original super triangle
                    if (std::find_first_of(t->point_ids.begin(),
                                           t->point_ids.end(),
                                           mesh.superTriangle_points_ids.begin(),
                                           mesh.superTriangle_points_ids.end()) != t->point_ids.end())
                    {
                        std::cout << "Triangle with id = " << t->id << "shares node with super triangle" << std::endl;
                        t->connected_to_super_triangle = true;
                    }
                    
                }
                mesh.triangles.emplace_back(std::move(T1));
                mesh.triangles.emplace_back(std::move(T2));
                mesh.triangles.emplace_back(std::move(T3));
                // Delete the current triangle. Erase returns iterator to next element. No need to move the iterator forward manually
                it = mesh.triangles.erase(it);
                break;
            }
            else
            {
                std::cout << "P=" << point.get() << " is NOT inside triangle T=" << it->get() << std::endl; 
                
                // Continue to next triangle
                ++it;
                std::cout << "e" << std::endl;
            }
        }
    }
    std::cout << "All triangles after algorithm loop: " << std::endl;
    auto counter = 0;
    // Delete all triangles that contains points from the super triangle
    for (const auto& triangle: mesh.triangles)
    {
        std::cout << "#" << counter << ": " << triangle.get() << std::endl;
    }
    // std::cout << "f" << std::endl;
    std::cout << "g" << std::endl;
    std::vector<size_t> superTriangle_points =  {mesh.triangles.at(0)->A->id,mesh.triangles.at(0)->B->id,mesh.triangles.at(0)->C->id};
    size_t i = 0;
    while(i < mesh.triangles.size())
    {
        if (mesh.triangles.at(i)->connected_to_super_triangle)
        {
            mesh.triangles.erase(mesh.triangles.begin() + i);
        }
        else
        {
            i++;
        }
        
    }
    mesh.print();
    return 0;
}
