#include "matrice.h"

#include <cmath>
#include <iostream>
using namespace std;

#include <cstdlib>
#include <ctime>

Matrice InitialiseRandom(int m, int n){ //crée et renvoie une matrice aléatoire de dimensions (m,n) et de coefficients dans {0,1,2}
                                        //(on aurait aussi pu choisir de mettre cette fonction en méthode de la classe Matrice)
        Matrice M(m, n);
        for (int i=0; i<m; i++){
            for (int j=0; j<n; j++){
                M.set(i,j,rand()%3);
            }
        }
        return M;
}

int main(){   //Matrices aléatoires, copies, multiplications... dans une boucle, pour tester la gestion de la mémoire

    srand(time(0));
    cout<<"Matrice :"<<endl;
    Matrice M=InitialiseRandom(2,2); //Matrice aléatoire de taille (2,2)
    M.affiche();

    for (int n=0;n<10;n++){
        cout<<"Mulipliee a gauche par la matrice suivante:"<<endl;
        Matrice R=InitialiseRandom(rand()%2+2,M.nlig()); //nombre de lignes de R égal à 2 ou 3 avec probabilité 1/2 chacun
        Matrice N(R); //utilisation du constructeur par copie (ici, ligne inutile en pratique)
        N.affiche();

        cout<<"Donne la matrice suivante :"<<endl;
        M = N*M; //utilisation de l'opérateur affectation = (destruction puis copie)
        M.affiche();
    }
    return 0;
}
