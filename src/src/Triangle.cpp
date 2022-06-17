#include "../header/Triangle.h"
size_t Triangle::triangle_counter = 0;
std::ostream& operator<<(std::ostream& os, Triangle& T)
{
    os << "Triangle id = " << T.id << ":\n  " << T.A->x << ", " << T.A->y << "\n  " << T.B->x << ", " << T.B->y << "\n  " << T.C->x << ", " << T.C->y << std::endl;
}