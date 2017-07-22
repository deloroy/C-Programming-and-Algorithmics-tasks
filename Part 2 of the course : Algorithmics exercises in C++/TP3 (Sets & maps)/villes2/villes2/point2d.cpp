#include "point2d.h"

// Munit les points 2D d'un ordre total (lexicographique). Renvoit True si le point est avant p
bool Point2D::operator<(const Point2D p) const{
    //Ordre lexicographique sur les coordonnées
    return (x()<p.x())||((x()==p.x())&&(y()<p.y()));
}

//Egalité de deux paires de coordonnées
bool operator==(const Point2D p, const Point2D p2){
    return (p.x()==p2.x())&&(p.y()==p2.y());
}
bool operator!=(const Point2D p, const Point2D p2){
    return !(p2==p);
}

//
// Display 2D point
//
ostream &operator<<(ostream &os, Point2D const &pt) {
    return os << "(" << pt.x() << "," << pt.y() << ")";
}
ostream &operator<<(ostream &os, Point2D* const &p) {
    return os << *p;
}
