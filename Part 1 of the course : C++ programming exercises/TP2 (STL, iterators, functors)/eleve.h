#ifndef ELEVE_H
#define ELEVE_H

#include<string>

#include <cstdlib>
#include <ctime>

#include <iostream>

#include<vector>
#include <algorithm>

using namespace std;


class Eleve
{
    string nom;
    int note;

public:

    Eleve();    //construit un élève au nom aléatoire de longueur aléatoire comprise entre 4 et 10
                //et à la note aléatoire entre 0 et 20
    Eleve(string name, int N);  //construit un élève de nom name et de note N

    string get_nom() const;   //accesseur en lecture du nom
    int get_note() const;     //accesseur en lecture de la note

};

class CompareNote{   //Foncteur permettant la comparaison suivant la note
    int* ptr;       //Pointeur sur entier permettant l'incrémentation du nombre de comparaisons
public:
    CompareNote();
    CompareNote(int& compteur);   //Permet à ptr de pointer sur compteur, c'est-à-dire le nombre de comparaisons déjà effectuées
    ~CompareNote();
    bool operator()(Eleve A, Eleve B) const;   //Comparaison selon l'ordre décroissant (et incrémentation du compteur)
};

class CompareNom{  //Foncteur permettant la comparaison suivant le nom
    int* ptr;
public:
    CompareNom();
    CompareNom(int& compteur);
    ~CompareNom();
    bool operator()(Eleve A, Eleve B) const;  //Ordre alphabétique
};

#endif // ELEVE_H
