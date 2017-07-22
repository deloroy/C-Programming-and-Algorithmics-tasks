#pragma once

#include <vector>
#include <cmath>
#include <cassert>
#include <iostream>
using namespace std;

#include "quadrant.h"
#include "quadtree.h"

//
// Add point p in the quadtree representing the points in square s
//

template <typename T>
void insert(QuadTree< Point2D<T> >* &qtree, Square s, Point2D<T> p){
    if (qtree==nullptr){
        //si la case pointée est vide on place le point (assure l'arrêt de la récursion)
        qtree= new QuadLeaf< Point2D<T> >(p);
    }
    if (qtree->isLeaf()){
        //si la case pointée est une feuille, elle est déjà occupée par un point
        //si ce point a des coordonnées différentes, alors on redécoupe la case
        if (qtree->value()!=p){ //on ignore les points ayant même coordonnées qu'un point déjà dans l'arbre
            //Etape 0 : pour découper, il faut savoir dans quel cadran du carré est la feuille, ou encore le point déjà présent
            Quadrant Q_feuille=Quadrant(s,qtree->value());
            //il faut connaître aussi le cadran du carré qui contient le nouveau point.
            Quadrant Q_new_point=Quadrant(s, p);

            //Etape 1 : création d'un nouveau découpage du carré, où on replace la feuille dans le sous-cadran qui la contient
            QuadLeaf< Point2D<T> >* Q[4];
            for (int i=0;i<4;i++){
                if (i==Q_feuille.dir){
                    Q[i]= new QuadLeaf< Point2D<T> >(qtree->value());
                }
                else{
                    Q[i]= nullptr;
                }
            }
            qtree = new QuadNode< Point2D<T> >(Q[0],Q[1],Q[2],Q[3]);

            //Etape 2: insertion du nouveau point dans le sous-cadran qui le contient (appel récursif)
            insert(qtree->son(Q_new_point.dir),Q_new_point.ssq,p);
            //la récursion se termine bien : on redécoupe (ie. on rentre dans le bloc if (qtree->isLeaf())) tant que le nouveau point à
            //insérer est dans le même sous-cadran que le point initialement présent.
            //A terme, on atteint bien une case vide (ie. on rentre dans la boucle (if qtree=nullptr)) dans laquelle insérer le point.
        }
    }

    if (qtree->isNode()){
        //si on arrive à un noeud, on détermine à quel cadran appartient le point p,
        Quadrant Qp=Quadrant(s, p);
        //et on se ramène récursivement au sous-problème : insérer le point p dans ce cadran
        insert(qtree->son(Qp.dir),Qp.ssq,p);
        //la récursion se termine bien : on atteindra à terme une feuille en descendant dans l'arbre
        //(ie. on rentrera à terme dans le bloc if (qtree->isLeaf()))
    }
}


// Crée un arbre QuadTree stockant les points2D contenus dans un vecteur, pour un intervalle carré de |R^2 donné (xmin,xmin+largeur)*(ymin,ymin+largeur)

template <typename T>
QuadTree< Point2D<T> >* build_tree(std::vector< Point2D<T> > points2D, float xmin,float ymin,float largeur){
    assert(largeur>0);
    Square Carre(xmin,ymin,largeur);
    QuadTree< Point2D<T> >* Arbre;
    Arbre = nullptr;   //on crée une feuille vide
    typename std::vector< Point2D<T> >::const_iterator it=points2D.begin();
    for(;it!=points2D.end();it++){
        Point2D<T> p=*it;
        if ((p.x()>=xmin)&&(p.x()<=xmin+largeur)&&(p.y()>=ymin)&&(p.y()<=ymin+largeur)){
            insert(Arbre, Carre, *it); //on insère le point s'il appartient au carré
        }
    }
    return Arbre;
}

//
// Add in vector neighbors the points q in quadtree qtree representing
// points in square s at euclidean distance less than r from point p
//
// Set only the nearest point if nearest is true
//
// Return the number of nodes visited
//


template <typename T>
int search(vector< Point2D<T> > &neighbors,
           QuadTree< Point2D<T> >* qtree, Square s,
           Point2D<T> p, float& r, bool nearest){
    //cas où l'arbre de départ est une feuille
    //on ne passe JAMAIS une de ces 2 conditions lors des appels récursifs (puisqu'on teste directement à partir d'un noeud les feuilles candidates)
    if (qtree==nullptr){
        return 0;
    }
    if (qtree->isLeaf()){
        Point2D<T> point_visite=qtree->value();
        if ((point_visite.info()!=p.info())||(point_visite!=p)){
            if (point_visite.dist2(p)<=r*r){
                r=sqrt(point_visite.dist2(p));
                neighbors.push_back(point_visite);
            }
        }
        return 1;
    }

    else{
        //on va rechercher les voisins dans chacun des sous-arbres (à partir du noeud actuel)
        int N_points_visites=0; //nombre de points visités lors de la recherche des voisins
        for (int i=0;i<4;i++){
            QuadTree< Point2D<T> >* sous_arbre_visite=qtree->son(i);
            if (sous_arbre_visite!=nullptr){
                if (sous_arbre_visite->isLeaf()){
                    //si le sous-arbre est une feuille non vide ie. contient un point différent de p,
                    //on calcule directement la distance de ce point au point p
                    Point2D<T> point_visite=sous_arbre_visite->value();
                    if ((point_visite.info()!=p.info())||(point_visite!=p)){
                        //différent selon information ou coordonnées
                        //(équivalent ici à coordonnées puisqu'on a ignoré dans la construction de l'arbre un point ayant même coordonnée qu'un autre déjà présent)
                        float distance2=point_visite.dist2(p);
                        if (nearest){ //si on recherche les plus proches voisins
                            if (distance2==r*r){
                                //on ajoute le voisin dans la liste s'il égale la distance minimale
                                neighbors.push_back(point_visite);
                            }
                            if (distance2<r*r){
                                //s'il bat la distance courante minimale, il devient seul plus proche voisin
                                r=sqrt(distance2);
                                neighbors.clear();
                                neighbors.push_back(point_visite);
                            }
                        }
                        else{
                            if (distance2<=r*r){ //si on recherche les voisins dans un périmètre circulaire de rayon r
                                neighbors.push_back(point_visite);
                            }
                        }
                    }
                    N_points_visites+=1; //on a visité un point de plus au cours de la recherche (feuille non vide)
                }
                else{
                    //si le sous-arbre ne se réduit pas à une feuille,
                    //on poursuit la recherche dans ce sous-arbre
                    //uniquement si le sous-carré représenté intersecte la boule centrée en p de rayon r
                    //(inutile sinon)
                    Square sous_carre_visite=s.subsquare(i);
                    if (sous_carre_visite.intersects_disk(p, r)){
                        N_points_visites+=search(neighbors,sous_arbre_visite,sous_carre_visite,p,r,nearest);
                        //actualisation du nombre de points visités
                    }
                }
            }
        }
        return N_points_visites;
    }
}


//
// Add in vector neighbors the nearest point of p in the quadtree qtree
// representing the points in square s
//
// Return the number of nodes visited
//
template <typename T>
int search(vector< Point2D<T> > &neighbors,
	   QuadTree< Point2D<T> >* qtree, Square s,
       Point2D<T> p){
    float distance_min=100000.; // distance "infinie" (plus grande que la distance maximale entre deux villes des données)
    return search(neighbors,qtree,s,p,distance_min,true);
}

