#include <string>
#include <time.h>
using namespace std;

//Ajouts permettant de faire tourner le programme sur C++ <11
#include <limits>
#include <cstdlib>

#ifndef srcPath
#define srcPath "."
#endif
// The path and name of the town file
string townFile = srcPath "/villes.txt";

#include "town.h"
#include "neighbors.h"


//Stocke dans neighbors les plus proches voisins de p parmis les villes de towns. Retourne la distance entre le plus proche voisin de p et p.
float search_without_quadtree(vector< Point2D<string> > &neighbors, vector< Point2D<string> > towns,Point2D<string> p){
    float rmin=100000.; // distance "infinie" (plus grande que la distance maximale entre deux villes des données)
    for (int i=0;i<towns.size();i++){
        Point2D<string> point_visite=towns[i];
        if ((point_visite.info()!=p.info())||(point_visite!=p)){ //ce n'est pas la même ville (coordonnées ou nom différents)
            float distance2=point_visite.dist2(p);
            if (distance2==rmin*rmin){
                neighbors.push_back(point_visite);
            }
            if (distance2<rmin*rmin){
                rmin=sqrt(distance2);
                neighbors.clear();
                neighbors.push_back(point_visite);
            }
        }
    }
    return rmin;
}

int main()
{
    /////
    ///// Read town file
    /////
    // Create an empty list of towns
    vector<Town> towns;
    // Prepare limits
    float xmin = std::numeric_limits<float>::max();
    float ymin = std::numeric_limits<float>::max();
    float xmax = std::numeric_limits<float>::min();
    float ymax = std::numeric_limits<float>::min();
    cout << "Lecture en cours du fichier : " << townFile << endl;
    clock_t t1 = clock();
    int nb_towns = Town::read_file(townFile, towns, xmin, ymin, xmax, ymax);
    clock_t t2 = clock();
    if (nb_towns < 0) {
    cout << "Aucune ville trouvée" << endl;
	return 1;
    }
    cout << "Fichier lu en : " << ((float)(t2-t1)/CLOCKS_PER_SEC) << " s" << endl;
    cout << "Nombre de villes dans le fichier : " << nb_towns << endl;
    cout << "Bornes de l'espace des coordonnées des villes : (" << xmin<< "," << ymin << ")-("
	 << xmax << "," << ymax << ")" << endl;
    cout<<endl;

    /////
    ///// Operate on quadtree
    /////

    //A. Création d'un vecteur de points 2D (coordonnées) où l'information portée par un point est le nom de la ville
    std::vector< Point2D<string> > points_villes;
    vector<Town>::iterator it = towns.begin();
    for (;it!=towns.end();it++){
        points_villes.push_back(Point2D<string>((*it).x(),(*it).y(),(*it).name()));
    }

    //B. Création du quadtree pour stocker efficacement les coordonnées des villes
    t1 = clock();
    QuadTree< Point2D<string> >* arbre=build_tree(points_villes,xmin,ymin,max(xmax-xmin,ymax-ymin)+1);
    t2 = clock();
    float temps_construction_quadtree=((float)(t2-t1)/CLOCKS_PER_SEC);
    cout << "Quadtree construit en " << temps_construction_quadtree << " s" << endl;
    //display(arbre);
    //Taille et profondeur du quadtree
    cout<<"Nombre de feuilles du Quadtree : "<<arbre->nbLeaves()<<endl;
    cout<<"Nombre de noeuds du Quadtree : "<<arbre->nbNodes()<<endl;
    cout<<"Taille du Quadtree : "<<arbre->nbLeaves()+arbre->nbNodes()<<endl;
    cout<<"Profondeur du Quadtree : "<<profondeur(arbre)<<endl;
    cout<<endl;


    //C. Recherche des plus proches voisins d'une ville dont le nom est demandée à l'utilisateur
    //Si plusieurs villes portent le même nom, on choisit la première ville dans l'ordre alphabétique
    string name;
    cout << "De quelle ville voulez-vous connaître le plus proche voisin ? ";
    getline(cin, name); // Lit le nom, en tenant compte des espaces, par opposition à cin
    vector<Town> ntowns = Town::towns_with_name(towns, name); //Liste des villes ayant ce même nom
    cout << "Nombre de villes possédant ce nom : " << ntowns.size() << endl;

    if (ntowns.size()>0){  //si la ville entrée pas l'utilisateur est dans le vecteur de villes
        if (ntowns.size()>1){
            cout<<"On va chercher les plus proches voisins de la première ville dans l'ordre alphabétique portant ce nom"<<endl;
        }
        Town ville_cible=*(ntowns.begin()); //ville dont on recherche le(s) plus proche(s) voisin(s)
        cout<<"On recherche donc le plus proche voisin de : "<<ville_cible.name()<<endl;
        cout<<"De coordonnées Lambert93 : ("<<ville_cible.x()<<","<<ville_cible.y()<<")"<<endl;

        std::vector< Point2D<string> > voisins; //on y stockera les plus proches voisins de ville_cible
        //Recherche des voisins et stockage dans voisins, calcul du nombre de points visités dans le quadtree au cours de la recherche
        int N_points_visites=search(voisins,arbre,Square(xmin,ymin,max(xmax-xmin,ymax-ymin)),Point2D<string>(ville_cible.x(),ville_cible.y(),ville_cible.name()));
        cout<<"Nombre de points visités dans le quadtree au cours de la recherche : "<<N_points_visites<<endl;
        cout<<"Les plus proches voisins sont donc : "<<endl;
        typename std::vector< Point2D<string> >::const_iterator it_voisins=voisins.begin();
        for(;it_voisins!=voisins.end();it_voisins++){
            Point2D<string> voisin=*it_voisins;
            cout<<voisin.info()<<" situé aux coordonnées : ("<<voisin.x()<<","<<voisin.y()<<")"<<endl;
        }
        cout<<"Distance de "<<voisins[0].dist(Point2D<string>(ville_cible.x(),ville_cible.y(),ville_cible.name()))<<" km"<<endl;
    }
    cout<<endl;

    //D. Calcul de rentabilité du quadtree
    //Constitution d'un échantillon de 100 villes prises au hasard
    srand(clock());
    vector< Point2D<string> > echantillon_villes;
    for (int n=0;n<100;n++){
        echantillon_villes.push_back(points_villes[rand() % towns.size()]);
    }
    cout<<"Constitution d'un échantillon de "<<echantillon_villes.size()<<" villes prises au hasard"<<endl;

    //D.a. Temps moyen de recherche d'un plus proche voisin avec quadtree (test sur l'échantillon)
    float temps_moyen_quadtree;
    for (int n=0;n<echantillon_villes.size();n++){
        Point2D<string> ville = echantillon_villes[n];
        std::vector< Point2D<string> > voisinage;
        t1 = clock();
        int N_points_visites=search(voisinage,arbre,Square(xmin,ymin,max(xmax-xmin,ymax-ymin)),Point2D<string>(ville));
        t2 = clock();
        temps_moyen_quadtree+=(float)(t2-t1)/CLOCKS_PER_SEC;
        //Affichage des résultats (pour vérification)
        /*cout<<"Les plus proches voisins de "<<ville.info()<<" situé aux coordonnées : ("<<ville.x()<<","<<ville.y()<<")"<<" sont donc : "<<endl;
        typename std::vector< Point2D<string> >::const_iterator it_voisinage=voisinage.begin();
        for(;it_voisinage!=voisinage.end();it_voisinage++){
            Point2D<string> voisin=*it_voisinage;
            cout<<voisin.info()<<" situé aux coordonnées : ("<<voisin.x()<<","<<voisin.y()<<")"<<endl;
        }
        cout <<"Recherche en " << ((float)(t2-t1)/CLOCKS_PER_SEC) << " s" << endl;*/
    }
    temps_moyen_quadtree/=echantillon_villes.size();
    cout<<"Temps moyen de recherche d'un plus proche voisin dans le quadtree (après sa construction) : "<<temps_moyen_quadtree<<" s"<<endl;

    //D.b. Temps moyen de recherche d'un plus proche voisin sans quadtree, avec un vector (test sur le même échantillon de villes)
    float temps_moyen_vector;
    for (int n=0;n<echantillon_villes.size();n++){
        Point2D<string> ville = echantillon_villes[n];
        std::vector< Point2D<string> > voisinage;
        t1 = clock();
        float distance=search_without_quadtree(voisinage,points_villes,ville);
        t2 = clock();
        temps_moyen_vector+=(float)(t2-t1)/CLOCKS_PER_SEC;
        //Affichage des résultats (pour vérification)
        /*cout<<"Les plus proches voisins de "<<ville.info()<<" situé aux coordonnées : ("<<ville.x()<<","<<ville.y()<<")"<<" sont donc : "<<endl;
        typename std::vector< Point2D<string> >::const_iterator it_voisinage=voisinage.begin();
        for(;it_voisinage!=voisinage.end();it_voisinage++){
            Point2D<string> voisin=*it_voisinage;
            cout<<voisin.info()<<" situé aux coordonnées : ("<<voisin.x()<<","<<voisin.y()<<")"<<endl;
        }
        cout << "Recherche en " << ((float)(t2-t1)/CLOCKS_PER_SEC) << " s" << endl;*/
    }
    temps_moyen_vector/=echantillon_villes.size();
    cout<<"Temps moyen de recherche d'un plus proche voisin sans quadtree (parcours d'un vector) : "<<temps_moyen_vector<<" s"<<endl;

    //D.c. CONCLUSION
    cout<<"Nombre moyen de recherches de plus proches voisins pour rentabiliser la construction du quadtree : ";
    cout<<temps_construction_quadtree/(temps_moyen_vector-temps_moyen_quadtree)<<endl;
}
