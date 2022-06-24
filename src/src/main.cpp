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

    // Loop over each point    
    for (auto& point: mesh.points)
    {
        std::cout << "Current point P: id = " << point->id << ", "<< *point << std::endl;
        // Resizing a std::vector invalidates the iterator. Need to either switch to
        // std::list OR reserve enough size at before we take out the iterator.
        mesh.triangles.reserve(1e3);
        auto it = mesh.triangles.begin();
        // Iterate over all triangles, Compare against all formed triangles.
        // Future implementation will be to split points into bins and make this loop
        // search a limited number of triangles
        while(it != mesh.triangles.end())
        {
            std::cout << " Checking P against current triangle id=" << it->get()->id << std::endl;
            if (mesh.is_point_in_circle(*point, it->get()))
            {
                std::cout << "  P=" << *point << " is inside triangle T=" << it->get()->id << std::endl;
                // Delete the current triangle. Erase returns iterator to next element. No need to move the iterator forward manually
                // Create 3 triangles from point P to the current triangles points A, B & C
                auto t1 = std::make_unique<Triangle>(point.get(), mesh.id_2_point.at(it->get()->A->id), mesh.id_2_point.at(it->get()->B->id));
                auto t2 = std::make_unique<Triangle>(point.get(), mesh.id_2_point.at(it->get()->B->id), mesh.id_2_point.at(it->get()->C->id));
                auto t3 = std::make_unique<Triangle>(point.get(), mesh.id_2_point.at(it->get()->A->id), mesh.id_2_point.at(it->get()->C->id));
                // std::cout << "  Creating triangle T1: \n" << *t1 << std::endl;
                // std::cout << "  Creating triangle T2: \n" << *t2 << std::endl;
                // std::cout << "  Creating triangle T3: \n" << *t3 << std::endl;
                // Add new elements to list of real elements! We will later make sure we still fulfill the Delaunay requirement
                mesh.add_triangle_to_mesh(std::move(t1));
                mesh.add_triangle_to_mesh(std::move(t2));
                mesh.add_triangle_to_mesh(std::move(t3));
                // The stack consists of all triangles with edges opposite our current point --> Share edges with T{1,2,3}: B -> C
                // Now we want to find adjacent triangles to t1, t2 & t3.
                // Should be 3 triangles. We find them from the eges of our 3 new triangles
                auto point_pair1 = std::pair<Point*, Point*>(mesh.id_2_point[it->get()->A->id], it->get()->B);
                auto point_pair2 = std::pair<Point*, Point*>(mesh.id_2_point[it->get()->B->id], it->get()->C);
                auto point_pair3 = std::pair<Point*, Point*>(mesh.id_2_point[it->get()->A->id], it->get()->C);
                it = mesh.triangles.erase(it);
                auto edge1 = mesh.points_2_edge.at(point_pair1);
                auto edge2 = mesh.points_2_edge.at(point_pair2);
                auto edge3 = mesh.points_2_edge.at(point_pair3);
                
                auto T1 = mesh.edge_2_Triangles.at(edge1);
                auto T2 = mesh.edge_2_Triangles.at(edge2);
                auto T3 = mesh.edge_2_Triangles.at(edge3);

                std::vector<Triangle*> stack = {};
                if (T1.size())
                {
                    if (T1.size() > 1)
                    {
                        std::cout << "  konstigt!! T1 size större än 1" << std::endl;
                    }
                    std::cout << "  Triangle added stack: T1:\n " << *T1[0] << std::endl;
                    stack.emplace_back(T1[0]);
                }
                if (T2.size())
                {
                    if (T1.size() > 1)
                    {
                        std::cout << "  konstigt!! T2 size större än 1" << std::endl;
                    }
                    stack.emplace_back(T2[0]);
                    std::cout << "  Triangles added to stack: T2:\n " << *T2[0] << std::endl;
                }
                if (T3.size())
                {
                    if (T1.size() > 1)
                    {
                        std::cout << "  konstigt!!T3 size större än 1" << std::endl;
                    }
                    std::cout << "  Triangles added to stack: T3:\n " << *T3[0] << std::endl;
                    stack.emplace_back(T3[0]);
                }
                while (!stack.empty())
                {
                    if (mesh.is_point_in_circle(*point, stack.back()))
                    {
                        std::cout << "  P=" << *point << " is inside triangle newly created triangle " << stack.back()->id << std::endl;
                        // swap diagonal of the two triangles who share an edge with P
                        // The edge in question will be edge1 for T1 etc..hm.
                        // will wife-swappa (stack.back()->)
                        // pop back element everytime since we dont know how to wife swap lol
                        stack.pop_back();
                    }
                }
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
                // break;
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
    // Delete all triangles that contains points from the super triangle
    // std::vector<size_t> superTriangle_points =  {mesh.triangles.at(0)->A->id,mesh.triangles.at(0)->B->id,mesh.triangles.at(0)->C->id};
    // size_t i = 0;
    // while(i < mesh.triangles.size())
    // {
    //     if (mesh.triangles.at(i)->connected_to_super_triangle)
    //     {
    //         mesh.triangles.erase(mesh.triangles.begin() + i);
    //     }
    //     else
    //     {
    //         i++;
    //     }
        
    // }
    std::cout << "After algorithm loop: " << std::endl;
    mesh.print();
    return 0;
}
