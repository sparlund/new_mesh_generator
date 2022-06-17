#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <memory>
#include <unordered_map>
#include <functional>

#include "../header/Mesh.h"
#include "../header/Point.h"
#include "../header/Triangle.h"
#include "../header/Edge.h"


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
    for (auto& point: mesh.points)
    {
        std::cout << "Current point P: id = " << point->id << ", "<< *point << std::endl;
        // Resizing a std::vector invalidates the iterator. Need to either switch to
        // std::list OR reserve enough size at before we take out the iterator.
        mesh.triangles.reserve(1e3);
        auto it = mesh.triangles.begin();
        // Iterate over all triangles, Compare against all formed triangles, 
        while(it != mesh.triangles.end())
        {
            std::cout << " Checking P against current triangle id=" << it->get()->id << std::endl;
            if (mesh.is_point_in_circle(*point, it->get()))
            {
                std::cout << "P=" << *point << " is inside triangle T=" << it->get()->id << std::endl;
                // Create 3 triangles from point P to the current triangles points A, B & C
                auto t1 = std::make_unique<Triangle>(point.get(), it->get()->A, it->get()->B);
                auto t2 = std::make_unique<Triangle>(point.get(), it->get()->B, it->get()->C);
                auto t3 = std::make_unique<Triangle>(point.get(), it->get()->A, it->get()->C);
                // std::cout << "  Creating triangle T1: \n" << T1.get() << std::endl;
                // std::cout << "  Creating triangle T2: \n" << T2.get() << std::endl;
                // std::cout << "  Creating triangle T3: \n" << T3.get() << std::endl;
                // The stack consists of all triangles with edges opposite our current point --> Share edges with T{1,2,3}: B -> C
                // map with pair of points as the key!!
                // for (auto& T: {T1, T2, T3})
                // {
                    /* code */
                // }
                
                std::vector<Triangle*> stack = {};
                // Now we want to find adjacent triangles to t1, t2 & t3.
                // Should be 3 triangles. We find them from the eges of our 3 new triangles
                auto edge_pair1 = std::pair<Point*, Point*>(it->get()->A, it->get()->B);
                auto edge_pair2 = std::pair<Point*, Point*>(it->get()->B, it->get()->C);
                auto edge_pair3 = std::pair<Point*, Point*>(it->get()->A, it->get()->C);
                auto T1 = mesh.points_2_triangle[edge_pair1];
                auto T2 = mesh.points_2_triangle[edge_pair2];
                auto T3 = mesh.points_2_triangle[edge_pair3];
                std::cout << "T1 = " << *T1 << std::endl;
                std::cout << "T2 = " << *T2 << std::endl;
                std::cout << "T3 = " << *T3 << std::endl;
                if (T1)
                {
                    stack.emplace_back(T1);
                }
                if (T2)
                {
                    stack.emplace_back(T2);
                }
                if (T3)
                {
                    stack.emplace_back(T3);
                }
                while (!stack.empty())
                {
                    if (mesh.is_point_in_circle(*point, stack.back()))
                    {
                        std::cout << "P=" << *point << " is inside triangle newly created triangle " << stack.back()->id << std::endl;
                        // swap diagonal of the two triangles who share an edge with P
                        // But how?!


                        
                    }
                }
                    
                
                
                // This function should return adjacent triangles given an edge
                // auto adjacent1 mesh.edge2Triangles(T1.edges);
                

                // Check if any points on newly formed triangles is from the original super triangle
                // This is a stupid place to do this. Should be done last or in ctor?
                // for (auto& t : {T1.get(), T2.get(), T3.get()})
                // {
                //     if (std::find_first_of(t->point_ids.begin(),
                //                            t->point_ids.end(),
                //                            mesh.superTriangle_points_ids.begin(),
                //                            mesh.superTriangle_points_ids.end()) != t->point_ids.end())
                //     {
                //         std::cout << "Triangle with id = " << t->id << "shares node with super triangle" << std::endl;
                //         t->connected_to_super_triangle = true;
                //     }
                    
                // }
                // mesh.triangles.emplace_back(std::move(T1));
                // mesh.triangles.emplace_back(std::move(T2));
                // mesh.triangles.emplace_back(std::move(T3));
                // Delete the current triangle. Erase returns iterator to next element. No need to move the iterator forward manually
                // it = mesh.triangles.erase(it);
                break;
            }
            else
            {
                std::cout << "P=" << *point << " is NOT inside triangle T=" << it->get()->id << std::endl; 
                
                // Continue to next triangle
                ++it;
                std::cout << "e" << std::endl;
            }
        }
    }
    std::cout << "All triangles after algorithm loop: " << std::endl;
    auto counter = 1;
    // Delete all triangles that contains points from the super triangle
    for (const auto& triangle: mesh.triangles)
    {
        std::cout << "#" << counter << ": " << *triangle << std::endl;
        counter++;
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
