#include "eleve.h"

Eleve::Eleve(){
    int longueur=4+rand()%7; //longueur du nom aléatoire entre 4 et 10
    char c=65+rand()%26;  //première lettre en majuscule : 65 encode 'A'
    nom=c;
    for (int i=1;i<longueur;i++){
        char d=97+rand()%26;  //autres lettres en minuscule : 97 encode 'a'
        nom+=d;
    }
    note=rand()%21; //note aléatoire entre 0 et 20
}

Eleve::Eleve(string name, int N){
    nom=name;
    note=N;
}

string Eleve::get_nom() const{
    return nom;
}

int Eleve::get_note() const{
    return note;
}


CompareNote::CompareNote(){
    ptr = new int[1];   //allocation mémoire
    *ptr=0;   //initialisation de la valeur pointée à 0
}

CompareNote::CompareNote(int& compteur){
    ptr = &compteur;   //ainsi ptr pointe sur compteur
}

CompareNote::~CompareNote(){
    delete[] ptr; //libération mémoire
}

bool CompareNote::operator()(Eleve A, Eleve B) const{
    (*ptr)++;  //1 comparaison supplémentaire
    return A.get_note()>B.get_note();  //Ordre décroissant
}


CompareNom::CompareNom(){
    ptr = new int[1];
    *ptr=0;
}

CompareNom::CompareNom(int& compteur){
    ptr = &compteur;
}

CompareNom::~CompareNom(){
    delete[] ptr;
}

bool CompareNom::operator()(Eleve A, Eleve B) const{
    (*ptr)++;
    return A.get_nom()<B.get_nom();  //Ordre alphabétique
}
