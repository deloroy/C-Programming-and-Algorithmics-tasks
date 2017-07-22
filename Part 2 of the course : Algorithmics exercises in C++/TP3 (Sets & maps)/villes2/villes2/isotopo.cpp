#include <iostream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <map>
#include <set>
#include <algorithm>

#include "point2d.h"
#include "town.h"

using namespace std;


#ifndef srcPath
#define srcPath "."
#endif
// The path and name of the town file
string townFile = srcPath "/villes.txt";

/*
 * Study French isotoponyms
 */
int main()
{
    /////
    ///// Read town file
    /////
    // Create an empty list of towns
    vector<Town> towns;
    // Read file
    cout << "Reading town file: " << townFile << endl;
    clock_t time1 = clock();
    int nb_towns = Town::read_file(townFile, towns);
    clock_t time2 = clock();
    // Sanity check
    if (nb_towns < 0) {
        cout << "Error while reading file" << endl;
        return 2;
    }
    if (nb_towns == 0) {
        cout << "No town found" << endl;
        return 1;
    }


    // Initialize random seed
    srand(time(NULL));
    // Say how many towns have been read
    cout << "File read in: "
         << ((float)(time2-time1)/CLOCKS_PER_SEC) << " s" << endl;
    cout << "Number of towns in file: "
         << towns.size() /* same as nb_towns */ << endl;
    cout << "A random town, using format \"name[lat,lon](x,y)\": "
         << towns[rand() % towns.size()] << endl;
    cout<<endl;

    time1 = clock();

    //Question 1

    //Construction d'une table associant à un nom de ville son nombre d'occurences dans Towns
    map<string,int> occ_noms;
    //Structure permise car le standard C++ définit déjà l'ordre lexicographique sur les strings (lettres de l'alphabet latin)
    for(int i=0;i<towns.size();i++){
        string nom=towns[i].name();
        map<string,int>::iterator it_occ=occ_noms.find(nom); //recherche du nom dans occ_noms
        if (it_occ==occ_noms.end()){ //nom n'a pas été trouvé dans occ_noms
            occ_noms[nom]=1;  //on insère la nouvelle clé et initialise sa valeur à 1
        }
        else{  //nom est déjà une clé de la table occ_noms
            it_occ->second+=1; //on incrémente le nombre d'occurences associé à ce nom
            //à l'issue de find, it_occ pointe dans ce cas sur (nom,occ_noms[nom])
        }
    }

    //Construction de l'histogramme sous forme de table : histo_noms[i] est le nombre de noms de villes ayant i occurences dans Towns
    map<int,int> histo_noms;
    map<string,int>::iterator it_occ=occ_noms.begin();
    for(;it_occ!=occ_noms.end();it_occ++){
        int occ=it_occ->second;   //nombre d'occurences d'un nom de ville
        map<int,int>::iterator it_histo=histo_noms.find(occ); //recherche de ce nombre comme clé dans la table histo_noms
        if (it_histo==histo_noms.end()){ //occ n'est pas encore une clé de la table
            histo_noms[occ]=1; //on l'ajoute à la table ; il y a alors un nom de ville partagé par un nombre occ de villes
        }
        else{ //occ est une clé
            it_histo->second+=1; //on incrémente de 1 le nombre de noms de villes partagés par un nombre occ de villes
        }
    }
    //Affichage de l'histogramme
    cout<<"Histogramme des répétitions de noms de villes"<<endl;
    map<int,int>::iterator it_histo=histo_noms.begin();
    for(;it_histo!=histo_noms.end();it_histo++){
        cout<<"Il y a "<<it_histo->second<<" noms de villes ayant "<<it_histo->first<<" occurrences."<<endl;
    }
    cout<<endl;

    //Question 2 (même principe que pour la question 1)

    //Construction d'une table associant à une paire de coordonnées géographiques son nombre d'occurences dans Towns
    map<Point2D,int> occ_coords;
    //Structure permise car on a défini un ordre total sur les Points2D (voir points2D)
    for(int i=0;i<towns.size();i++){
        Point2D coords=towns[i].point();
        map<Point2D,int>::iterator it_occ=occ_coords.find(coords);
        if (it_occ==occ_coords.end()){
            occ_coords[coords]=1;
        }
        else{
            it_occ->second+=1;
        }
    }
    //Construction de l'histogramme sous forme de table : histo_coords[i] est le nombre de points où se situent i villes
    map<int,int> histo_coords;
    map<Point2D,int>::iterator it_occ2=occ_coords.begin();
    for(;it_occ2!=occ_coords.end();it_occ2++){
        int occ=it_occ2->second;
        map<int,int>::iterator it_histo=histo_coords.find(occ);
        if (it_histo==histo_coords.end()){
            histo_coords[occ]=1;
        }
        else{
            it_histo->second+=1;
        }
    }
    //Affichage de l'histogramme
    cout<<"Histogramme du nombre de villes de mêmes coordonnées"<<endl;
    map<int,int>::iterator it_histo2=histo_coords.begin();
    for(;it_histo2!=histo_coords.end();it_histo2++){
        cout<<"Il y a "<<it_histo2->second<<" paires de coordonnées partagées par "<<it_histo2->first<<" villes."<<endl;
    }
    cout<<endl;


    //Question3. Nombre de villes homonymes et isotopes

    set<Town> homonymes;//villes partageant leur nom avec une autre ville
    set<Town> isotopes; //villes partageant leur paire de coordonnées avec une autre ville
    //Structure permise car on a muni les Towns d'un ordre total (cf town)
    for (int i=0;i<towns.size();i++){
        Town ville=towns[i];
        if (occ_noms[ville.name()]>1){ //il y a au moins 2 occurrences du nom de la ville
            homonymes.insert(ville);
        }
        if (occ_coords[ville.point()]>1){ //il y a au moins 2 occurrences de la paire de coordonnées de la ville
            isotopes.insert(ville);
        }
    }


    vector<Town> isotoponymes(min(isotopes.size(),homonymes.size()));
    //villes partageant nom avec une ville et paire de coordonnées avec une autre ville
    //remplissage du vecteur par un nombre de villes fictives égal au cardinal maximal possible de l'intersection
    //OK car constructeur vide de Town (cf town)

    vector<Town>::iterator fin_inter=set_intersection(homonymes.begin(),homonymes.end(),isotopes.begin(),isotopes.end(),isotoponymes.begin());
    //algorithme d'intersection de la STL opérant sur des intervalles triés, comme nos ensembles de Town
    //on stocke les éléments de l'intersection dès le début de isotoponymes

    while (fin_inter!=isotoponymes.end()){
        isotoponymes.pop_back();
    }
    //suppression des villes fictives,l'itérateur fin_inter pointe en effet sur la case du vecteur après les éléments de l'intersection


    cout<<"Il y a "<<homonymes.size()<<" villes homonymes."<<endl;
    cout<<"Il y a "<<isotopes.size()<<" villes isotopes."<<endl;
    cout<<"Il y a "<<isotoponymes.size()<<" villes isotoponymes."<<endl;
    cout<<endl;

    //Question4
    //Combien y a t'il des villes A pour lesquelles il est ambigu de parler de "A toute proche d'une ville B" ?

    set<Town> villes_trompeuses; //ensembles des villes A satisfaisant à la condition d'existence du quatuor (cf. rapport)

    Town ville1,ville2,ville3,ville4; //à chercher dans isotoponymes
    for (vector<Town>::iterator it=isotoponymes.begin();it!=isotoponymes.end();it++){
        ville1=*it;

        set<Town>::iterator it_trompeuse=villes_trompeuses.find(ville1);
        if (it_trompeuse==villes_trompeuses.end()){ //ville1 n'a pas déjà été trouvé dans un quatuor de villes trompeuses, on peut lancer la recherche

            //On stocke les homonymes et les isotopes de ville1 appartenant à isotoponymes une bonne fois pour toutes
            //Dans les vecteurs respectifs homonymes1 et isotopes1
            vector<Town> homonymes1,isotopes1;
            for (vector<Town>::iterator it_iso=isotoponymes.begin();it_iso!=isotoponymes.end();it_iso++){
                if ((ville1.point()!=(*it_iso).point())&&(ville1.name()==(*it_iso).name())){ //pas meme point !
                    homonymes1.push_back(*it_iso);
                }
                if ((ville1.point()==(*it_iso).point())&&(ville1.name()!=(*it_iso).name())){
                    isotopes1.push_back(*it_iso);
                }
            }

            bool trouve=false; //booléen indiquant si on a trouvé un quatuor de villes montrant que la ville1 est une ville trompeuse
            //permet d'arrêter la recherche pour ville1 une fois ce quatuor trouvé

            for (vector<Town>::iterator it2=homonymes1.begin();it2!=homonymes1.end();it2++){
                if (trouve){ //on a exhibé un quatuor montrant que v1 est trompeuse, on peut arrêter la recherche pour ville1
                    break;
                }
                ville2=*it2; //ville2 est un homonyme de ville1 appartenant à isotoponymes
                for (vector<Town>::iterator it3=isotopes1.begin();it3!=isotopes1.end();it3++){
                    if (trouve){ //idem
                        break;
                    }
                    ville3=*it3;  //ville3 est un isotope de ville1 appartenant à isotoponymes
                    for (vector<Town>::iterator it4=isotoponymes.begin();it4!=isotoponymes.end();it4++){
                        if (trouve){ //idem
                            break;
                        }
                        ville4=*it4; //ville4 est un isotoponyme quelconque. Il n'y a plus qu'à tester les deux dernières conditions sur lui.
                        if ((ville4.point()==ville2.point())&&(ville4.name()==ville3.name())){ //Alors ville1 est trompeuse de même que les trois autres villes
                            //On affiche le quatuor en question
                            cout<<"Quatuor de villes trompeuses trouvé : "<<endl;
                            Town quatuor[4]={ville1,ville2,ville3,ville4};
                            for (int i=0;i<4;i++){
                                cout<<quatuor[i].name()<<" située en ("<<quatuor[i].x()<<","<<quatuor[i].y()<<")."<<endl;
                                villes_trompeuses.insert(quatuor[i]); //Ajour des villes du quatuor à villes_trompeuses
                            }
                            trouve=true; //on a exhibé un quatuor telle que ville1 est trompeuse
                        }
                    }
                }
            }
        }
    }

    time2=clock();

    cout<<endl;
    cout<<"Nombre de villes trompeuses : "<<villes_trompeuses.size()<<endl;
    cout << "Temps de recherche : "<<((float)(time2-time1)/CLOCKS_PER_SEC) << " s" << endl;

    //Approche naive
    cout<<endl;
    cout<<"Recherche par approche naive"<<endl;
    time1=clock();

    set<Town> villes_trompeuses2;
    for (vector<Town>::iterator it=towns.begin();it!=towns.end();it++){
        ville1=*it;
        set<Town>::iterator it_trompeuse=villes_trompeuses2.find(ville1);
        if (it_trompeuse==villes_trompeuses2.end()){
            bool trouve=false;
            for (vector<Town>::iterator it2=towns.begin();it2!=towns.end();it2++){
                if (trouve){
                    break;
                }
                ville2=*it2;
                if ((ville2.name()==ville1.name())&&(ville2.point()!=ville1.point())){
                    for (vector<Town>::iterator it3=towns.begin();it3!=towns.end();it3++){
                        if (trouve){
                            break;
                        }
                        ville3=*it3;
                        if ((ville3.point()==ville1.point())&&(ville3.name()!=ville1.name())){
                            for (vector<Town>::iterator it4=towns.begin();it4!=towns.end();it4++){
                                if (trouve){
                                    break;
                                }
                                ville4=*it4;
                                if ((ville4.point()==ville2.point())&&(ville4.name()==ville3.name())){
                                    cout<<"Quatuor de villes trompeuses trouvé : "<<endl;
                                    Town quatuor[4]={ville1,ville2,ville3,ville4};
                                    for (int i=0;i<4;i++){
                                        cout<<quatuor[i].name()<<" située en ("<<quatuor[i].x()<<","<<quatuor[i].y()<<")."<<endl;
                                        villes_trompeuses2.insert(quatuor[i]);
                                    }
                                    trouve=true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    time2=clock();

    cout<<endl;
    cout<<"Nombre de villes trompeuses avec l'approche naive' : "<<villes_trompeuses2.size()<<endl;
    cout << "Temps de recherche avec l'approche naive' : "<<((float)(time2-time1)/CLOCKS_PER_SEC) << " s" << endl;

    return 0;

}
