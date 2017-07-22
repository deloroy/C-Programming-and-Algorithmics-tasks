#include "eleve.h"

//Fonction d'affichage des éléments du vecteur des élèves

void print(const vector<Eleve>& V){
    vector<Eleve>::const_iterator it=V.begin();  //itérateur sur le vecteur d'élèves
    for (; it!=V.end(); it++){
        cout<<"("<<(*it).get_nom()<<","<<(*it).get_note()<<")"<<endl;  //affichage sous la forme (nom, note)
    }
}

int main() {

    srand(time(0));

    vector<Eleve> v;
    for (int i=0;i<20;i++){  //choix de 20 élèves aux notes et aux noms aléatoires (longueur aléatoire entre 4 et 10)
        v.push_back(Eleve());
    }
    v.push_back(Eleve("Turing", 20));  //test du constructeur "déterministe"

    cout<<"Liste non triee des eleves et de leurs notes"<<endl;
    print(v);
    cout<<endl;

    cout<<"Tri par ordre alphabetique"<<endl;
    int cpt=0; //nombre de comparaisons effectuees
    sort(v.begin(),v.end(),CompareNom(cpt)); //tri selon la relation d'ordre définie dans le foncteur CompareNom
    print(v);
    cout<<"Nombre de comparaisons effectuees : "<<cpt<<endl;

    cout<<endl;

    cout<<"Tri par les notes (ordre decroissant)"<<endl;
    cpt=0;  //remise à 0 du nombre de comparaisons
    sort(v.begin(),v.end(),CompareNote(cpt));
    print(v);
    cout<<"Nombre de comparaisons effectuees : "<<cpt<<endl;

}

