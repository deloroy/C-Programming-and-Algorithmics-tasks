#include "square.h"

#include "quadtree.h"

Square Square::subsquare(int dir){
    //immédiat avec la définition du carré par son coin bas-gauche et sa diagonale
    if (dir==SW)
        return Square(x,y,w/2);
    if (dir==NW)
        return Square(x,y+w/2,w/2);
    if (dir==SE)
        return Square(x+w/2,y,w/2);
    if (dir==NE)
        return Square(x+w/2,y+w/2,w/2);
}

bool Square::intersects_disk(Point_2D p, float r){
     //On recherche d'abord les coordonnées du point du carré qui est le plus proche du cercle (projeté sur le carré du centre du cercle)
     float x_projete,y_projete;
     //pour connaitre son abcisse, on regarde où se situe l'abcisse du centre du cercle par rapport aux abcisses des coins du carré
     //ie on projette le centre du cercle sur le côté horizontal du carré qui se confond avec l'axe des abcisses
     if (p.x()<x){
         x_projete=x;
     }
     else{
         if(p.x()>x+w){
             x_projete=x+w;
         }
         else{
             x_projete=p.x();
         }
     }
     if (p.y()<y){        //idem pour l'ordonnée
         y_projete=y;
     }
     else{
         if(p.y()>y+w){
             y_projete=y+w;
         }
         else{
             y_projete=p.y();
         }
     }
     //Il ne reste plus qu'à comparer à r la distance entre le centre du cercle et le projeté (point le plus proche appartenant au carré)
     return (p.dist2(Point_2D(x_projete,y_projete))<=r*r);
     //Rq : le test donne aussi le bon résultat quand le centre du cercle est à l'intérieur du carré
     //(dans ce cas, le projeté est le centre du cercle et la distance est nulle)
}
