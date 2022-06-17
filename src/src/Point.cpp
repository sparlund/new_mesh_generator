#include "../header/Point.h"

size_t Point::id_counter = 0;
std::ostream& operator<<(std::ostream& os, Point& P)
{
    os << "("<< P.x << ", " << P.y << ")";
}