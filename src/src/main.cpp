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
    mesh.add_point(0.57999953d, 0.17683099d);
    mesh.add_point(0.93249719d, 0.82588046d);
    mesh.add_point(0.48789419d, 0.94970689d);
    mesh.add_point(0.28401174d, 0.1877854d);
    mesh.add_point(0.10437661d, 0.9318586d);
    mesh.add_point(0.67862826d, 0.15387296d);
    mesh.add_point(0.70414281d, 0.19888374d);
    mesh.add_point(0.79286248d, 0.52596994d);
    mesh.add_point(0.90283226d, 0.8652227d);
    mesh.add_point(0.35389043d, 0.34447912d);
    mesh.add_point(0.89302998d, 0.24955246d);
    mesh.add_point(0.45334856d, 0.96482907d);
    mesh.add_point(0.21281714d, 0.18958733d);
    mesh.add_point(0.16252637d, 0.66224278d);
    mesh.add_point(0.40605858d, 0.17457709d);
    mesh.add_point(0.09700246d, 0.0062263d);
    mesh.add_point(0.58946072d, 0.27928575d);
    mesh.add_point(0.42268084d, 0.43206649d);
    mesh.add_point(0.27712504d, 0.33229486d);
    mesh.add_point(0.07195633d, 0.70720021d);
    mesh.add_point(0.82399834d, 0.88912371d);
    mesh.add_point(0.12918101d, 0.24072897d);
    mesh.add_point(0.0834147d, 0.82638309d);
    mesh.add_point(0.08926189d, 0.50984403d);
    mesh.add_point(0.99488906d, 0.08139511d);
    mesh.add_point(0.36721945d, 0.39594196d);
    mesh.add_point(0.25757772d, 0.43899996d);
    mesh.add_point(0.52880561d, 0.97749779d);
    mesh.add_point(0.1551352d, 0.60635949d);
    mesh.add_point(0.79085032d, 0.71378463d);
    mesh.add_point(0.27366661d, 0.39663291d);
    mesh.add_point(0.42065202d, 0.11654551d);
    mesh.add_point(0.32058973d, 0.48130712d);
    mesh.add_point(0.18030784d, 0.84200259d);
    mesh.add_point(0.77365087d, 0.49618022d);
    mesh.add_point(0.19242206d, 0.92447336d);
    mesh.add_point(0.48966483d, 0.24895988d);
    mesh.add_point(0.15049661d, 0.32094534d);
    mesh.add_point(0.26398247d, 0.94966878d);
    mesh.add_point(0.18603591d, 0.84780664d);
    mesh.add_point(0.31810148d, 0.43213858d);
    mesh.add_point(0.79060873d, 0.31577686d);
    mesh.add_point(0.79238014d, 0.14942884d);
    mesh.add_point(0.64720052d, 0.36919321d);
    mesh.add_point(0.85880178d, 0.97542818d);
    mesh.add_point(0.17096435d, 0.83326518d);
    mesh.add_point(0.14441177d, 0.54220985d);
    mesh.add_point(0.71447577d, 0.58003291d);
    mesh.add_point(0.10489533d, 0.5704999d);
    mesh.add_point(0.19180812d, 0.27897113d);
    mesh.add_point(0.06437286d, 0.56928809d);
    mesh.add_point(0.09742068d, 0.37239859d);
    mesh.add_point(0.3631888d, 0.42158724d);
    mesh.add_point(0.98906047d, 0.44471309d);
    mesh.add_point(0.72828926d, 0.54083673d);
    mesh.add_point(0.2441257d, 0.68271326d);
    mesh.add_point(0.01176776d, 0.37942701d);
    mesh.add_point(0.9992295d, 0.71005428d);
    mesh.add_point(0.03643857d, 0.66819248d);
    mesh.add_point(0.58530477d, 0.55389196d);
    mesh.add_point(0.82489761d, 0.24659197d);
    mesh.add_point(0.99453185d, 0.14538279d);
    mesh.add_point(0.77986053d, 0.46631291d);
    mesh.add_point(0.8547889d, 0.28748791d);
    mesh.add_point(0.89661789d, 0.08765166d);
    mesh.add_point(0.9570299d, 0.36689992d);
    mesh.add_point(0.55358002d, 0.81988124d);
    mesh.add_point(0.92816645d, 0.20707428d);
    mesh.add_point(0.31599173d, 0.56833454d);
    mesh.add_point(0.95457334d, 0.61730257d);
    mesh.add_point(0.54717979d, 0.61956603d);
    mesh.add_point(0.56639413d, 0.22892725d);
    mesh.add_point(0.25264123d, 0.97984186d);
    mesh.add_point(0.92766964d, 0.30672903d);
    mesh.add_point(0.82751648d, 0.9789986d);
    mesh.add_point(0.6069603d, 0.91692431d);
    mesh.add_point(0.65747567d, 0.02843203d);
    mesh.add_point(0.06810332d, 0.4430097d);
    mesh.add_point(0.33390687d, 0.66720316d);
    mesh.add_point(0.71176434d, 0.96175797d);
    mesh.add_point(0.8223953d, 0.07162743d);
    mesh.add_point(0.94926903d, 0.01405288d);
    mesh.add_point(0.59537534d, 0.97984379d);
    mesh.add_point(0.23772483d, 0.83341318d);
    mesh.add_point(0.62323386d, 0.45595499d);
    mesh.add_point(0.8628433d, 0.58840899d);
    mesh.add_point(0.84118933d, 0.3657079d);
    mesh.add_point(0.74858927d, 0.40448676d);
    mesh.add_point(0.58528537d, 0.82624698d);
    mesh.add_point(0.86106598d, 0.01257945d);
    mesh.add_point(0.91127339d, 0.04721323d);
    mesh.add_point(0.0840069d, 0.57187571d);
    mesh.add_point(0.49161891d, 0.01589083d);
    mesh.add_point(0.6227627d, 0.0036666d);
    mesh.add_point(0.23534945d, 0.65536485d);
    mesh.add_point(0.79096108d, 0.72911141d);
    mesh.add_point(0.18582286d, 0.74294024d);
    mesh.add_point(0.08008338d, 0.31413118d);
    mesh.add_point(0.13006888d, 0.25366822d);
    mesh.add_point(0.92777118d, 0.17183519d);
    mesh.normalize();
    mesh.init_superTriangle();
    size_t stAid = mesh.triangles[0].A->id;
    size_t stBid = mesh.triangles[0].B->id;
    size_t stCid = mesh.triangles[0].C->id;
    std::vector<Point*> stp = {mesh.triangles[0].A, mesh.triangles[0].B, mesh.triangles[0].C};
    std::vector<size_t> stIds = {stAid,stBid,stCid};

    // Loop over each point
    for (auto& point: mesh.points)
    {
        std::cout << "Current point P: id = " << point->id << ", "<< *point << std::endl;
        std::cout << "mesh.triangles.size() = " << mesh.triangles.size() << std::endl;
        std::cout << "mesh.points.size() = " << mesh.points.size() << std::endl;
        std::vector<Edge> edges;
        // Iterate over all triangles, compare against all formed triangles.
        // TODO: split points into bins and make this loop search a limited number of triangles or in a smart order
        for(auto it = mesh.triangles.begin();it != mesh.triangles.end();it++)
        {
            std::cout << " Checking P against current triangle id=" << it->id << std::endl;
            if (mesh.is_point_in_circle(*point, *it))
            {
                std::cout << "  P=" << *point << " is inside triangle T=" << it->id << std::endl;
                it->is_bad = true;
                std::cout << "  Marking triangle id=" << it->id << " bad and creating 3 new edges" << std::endl;
                edges.push_back(Edge{it->A,it->B});
                edges.push_back(Edge{it->B,it->C});
                edges.push_back(Edge{it->C,it->A});
            }
        }
        // Remove all triangles marked bad
        mesh.triangles.erase(std::remove_if(
                                mesh.triangles.begin(),
                                mesh.triangles.end(),
                                [](Triangle& T)
                                {
                                    // TODO: remove T from maps of adjacent triangles
                                    return T.is_bad;
                                }),
                                mesh.triangles.end());
        // Check edge is not shared.
        // TODO: switch to hash table lookup for faster execution?
        for (auto edge1 = edges.begin(); edge1 != edges.end(); edge1++)
        {
            for (auto edge2 = edge1 + 1; edge2 != edges.end(); edge2++)
            {
                if (mesh.are_edges_equal(*edge1,*edge2))
                {
                    std::cout << "edge1 and edge2 are equal: edge1: " << edge1->A->id << "->" << edge1->B->id << ", edge2:" << edge1->A->id << "->" << edge1->B->id << std::endl;
                    edge1->is_bad = true;
                    edge2->is_bad = true;
                }
            }
        }
        // Remove all bad edges
        edges.erase(std::remove_if(
            edges.begin(),
            edges.end(),
            [](Edge& E){return E.is_bad;}),
            edges.end());
        // Create new triangles from non-bad edges
        for (auto& edge: edges)
        {
            // TODO: add new triangle to map of adjacent triangles
            mesh.triangles.emplace_back(Triangle(edge.A, edge.B, point.get()));
            std::cout << "tror vi dödar T här, men mesh.triangles.size()=" << mesh.triangles.size() << std::endl;
        }
    }
    // Clean up triangles connected to supertriangle
    mesh.triangles.erase(std::remove_if(mesh.triangles.begin(),
                                        mesh.triangles.end(),
                                        [stp](Triangle& T)
                                        {
                                            std::cout << "should we remove T.id = " << T.id <<"? T.A->id, T.A->id, T.A->id = " << T.A->id << " " << T.B->id << " " << T.C->id << std::endl;
                                            for (const auto& p: stp)
                                            {
                                                if (T.contains_point(*p))
                                                {
                                                    // TODO: remove triangle from map of adjacent triangles
                                                    std::cout << "yes we remove!" << std::endl;
                                                    return true;
                                                }
                                                
                                            }
                                            std::cout << "--" << std::endl;
                                        }),
                                        mesh.triangles.end());
    // Now we've completed a the Delaunay triangulation, now let's introduce constraints to it.
    // --> Making it a constrained Delaunay triangulation
    // Constrained edges, has to be user input:
    std::vector<Edge> constrained_edges;

    std::cout << "After algorithm loop: " << std::endl;
    mesh.print();
    return 0;
}
