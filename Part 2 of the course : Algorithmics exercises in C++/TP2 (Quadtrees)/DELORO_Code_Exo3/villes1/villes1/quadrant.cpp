#include "quadrant.h"
#include "quadtree.h"

Quadrant::Quadrant(Square s, Point_2D p){
    //pour déterminer le cadran du carré s dans lequel se trouve p, il suffit
    //de regarder sa position par rapport au centre de s
    float x_centre=s.x+s.w/2;
    float y_centre=s.y+s.w/2;
    if (p.x()<=x_centre){
        if (p.y()<=y_centre)
            dir=SW;
        else
            dir=NW;
    }
    else{
        if (p.y()<=y_centre){
            dir=SE;
        }
        else
            dir=NE;
    }
    ssq=s.subsquare(dir);
}
