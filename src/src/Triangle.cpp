#include "../header/Triangle.h"
size_t Triangle::triangle_counter = 0;
std::ostream& operator<<(std::ostream& os, Triangle& T)
{
    os << "Triangle id = " << T.id << ":" << T.A->x << "," << T.A->y << " | " << T.B->x << "," << T.B->y << " | " << T.C->x << "," << T.C->y;
}