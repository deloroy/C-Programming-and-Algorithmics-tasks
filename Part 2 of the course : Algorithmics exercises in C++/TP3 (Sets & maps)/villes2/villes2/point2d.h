/*
 * A simple class to operate on 2D points
 */

#pragma once

#include <iostream>
using std::ostream;


/*
 * A 2D point given by (x,y) coordinates
 */
class Point2D
{
    // Coordinates
    float _x, _y;
public:
    // Create a new 2D point with given coordinates
    Point2D(float x, float y) : _x(x), _y(y) {}
    // Get coordinates
    inline float x() const { return _x; }
    inline float y() const { return _y; }
    // Munit les points 2D d'un ordre total (lexicographique). Renvoit True si le point est avant p
    bool operator<(const Point2D p) const;
};

//
// Display 2D point
//

bool operator==(const Point2D p, const Point2D p2);
bool operator!=(const Point2D p, const Point2D p2);


ostream &operator<<(ostream &os, Point2D const &pt);
ostream &operator<<(ostream &os, Point2D* const &p);
