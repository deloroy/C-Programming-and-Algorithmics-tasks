#ifndef MATRICE_H
#define MATRICE_H


class Matrice
{
    int n_lig,n_col;
    double* tab;
    int* compteur; //compteur du nombre de partages mémoires du tableau des coefficients

public:

    Matrice(int m,int n); //constructeur d'une matrice de taille (m,n)
    ~Matrice(); //destructeur
    Matrice(const Matrice& B); //constructeur par copie

    double get(int i,int j); //accesseur en lecture du terme (i,j)
    void set(int i, int j, double x); //accesseur en écriture du terme (i,j)
    int nlig(); //accesseur en lecture du nombre de lignes

    void affiche(); //affichage de la matrice

    void operator=(const Matrice& B); //opérateur affectation =
    Matrice operator*(const Matrice& B); //opérateur multiplication (par B à droite)

};

#endif // MATRICE_H
