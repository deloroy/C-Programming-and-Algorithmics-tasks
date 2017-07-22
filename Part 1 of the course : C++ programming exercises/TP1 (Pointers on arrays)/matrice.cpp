#include "matrice.h"

#include <cmath>
#include <iostream>
using namespace std;

#include <cassert>

Matrice::Matrice(int m,int n){

    n_lig=m;
    n_col=n;

    tab=new double[m*n]; //allocation mémoire

    compteur= new int; //allocation mémoire
    *compteur=1; //le tableau des coefficients est pointé 1 fois

    cout<<"(construction de matrice ("<<m<<','<<n<<"))"<<endl;
}

Matrice::~Matrice(){

    (*compteur)--; //décrémentation (destruction)

    if (*compteur==0){ //seul le dernier tableau libère la mémoire
        delete[] tab;
        delete compteur;
    }
    cout<<"(destruction de matrice)"<<endl;
}

Matrice::Matrice(const Matrice& B){

    n_lig=B.n_lig;
    n_col=B.n_col;

    tab=B.tab; //partage mémoire du tableau des coefficients (pointé 1 fois de plus)

    compteur=B.compteur; //partage mémoire du compteur
    (*compteur)++; //incrémentation (on a 1 copie mémoire supplémentaire du tableau)

    cout<<"(copie de matrice)"<<endl;
}

double Matrice::get(int i,int j){
    assert((i<n_lig) && (j<n_col));
    return tab[i+n_lig*j];
}

void Matrice::set(int i, int j, double x){
    assert((i<n_lig) && (j<n_col));
    tab[i+n_lig*j]=x;
}

int Matrice::nlig(){
    return n_lig;
}

void Matrice::affiche(){
     for (int i=0; i<n_lig; i++){
         if (i==0)
             cout<<"[";
         else
             cout<<" ";
         for (int j=0; j<n_col; j++){
             cout<<tab[i+n_lig*j];
             if (j!=n_col-1){
                 cout<<",";
             }
             else{
                 if (i==n_lig-1){
                     cout<<"]";
                 }
             }
         }
         cout<<endl;
     }
}

void Matrice::operator=(const Matrice& B){

    delete[] tab; //libération de la mémoire

    n_lig=B.n_lig;
    n_col=B.n_col;

    tab=B.tab; //partage du tableau

    compteur=B.compteur; //partage du compteur
    (*compteur)++; //on a 1 copie mémoire supplémentaire du tableau

    cout<<"(affectation de matrice : destruction puis copie)"<<endl;
}

Matrice Matrice::operator*(const Matrice& B){ //multiplication par B à droite
        assert(n_col==B.n_lig);

        Matrice M(n_lig,B.n_col);

        for (int i=0;i<M.n_lig;i++){
            for (int j=0;j<M.n_col;j++){
                double sum=0; //calcul du terme M(i,j)
                for (int k=0;k<n_col;k++){
                    sum+=tab[i+n_lig*k]*B.tab[k+B.n_lig*j];
                }
                M.tab[i+n_lig*j]=sum;
            }
        }
        return M;
}
