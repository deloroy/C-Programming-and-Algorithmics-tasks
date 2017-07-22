#ifndef FONCTION_H
#define FONCTION_H

#include <iostream>
#include <string>
using namespace std;

class Fonction
{
    Fonction* integrale;    //Pointeur sur une primitive de la fonction
public:
    Fonction();                                    //Constructeur vide
    virtual Fonction* clone() const;               //Renvoie un pointeur sur une copie de la fonction (méthode virtuelle, car redéfinie dans les classes enfants : on veut que la fonction pointée soit de la classe la plus "précise")
    virtual float operator()(float x) const;       //Renvoie l'évaluée de la fonction en x (méthode virtuelle, car redéfinie plus "précisément" par Polynome et Trigo)
    virtual Fonction* derivee() const;             //Renvoie un pointeur sur la dérivée de la fonction (méthode virtuelle, car redéfinie plus "précisément" par Polynome)
    float inverse(float y) const;                  //Calcule l'antécédent de y par la fonction par la méthode de Newton
    virtual ~Fonction();                           //Destructeur (nécessairement virtuel, pour que le "bon" destructeur soit appelé !)
};

class Polynome: public Fonction
{
protected:                 //Pour que les attributs soient aussi accessibles par la classe Affine enfant de Polynome
    int degre;             //degré du polynome
    float* coeff;          //pointeur sur les coefficients : pour P = a_0 + a_1*x^1 + a_n*x^n, *coeff = [a_0,a_1,...,a_n] (degré n)
public:
    Polynome();                         //Constructeur vide (appelé par le constructeur de Affine)
    Polynome(int N, float* c);          //Constructeur d'un polynome de degré N et de coefficients *c
    virtual Fonction* clone() const;    //Renvoie un pointeur sur une copie du polynome appartenant à la classe Polynome (virtuelle car redéfinie plus précisément par Affine)
    float operator()(float x) const;
    Polynome* derivee() const;
    virtual ~Polynome();                //(virtual accessoire ici car on ne redéfinit pas de destructeur pour Affine)
};                                      //Héritage de Fonction::inverse avec la mention "public" (pas besoin de redéfinir la méthode)

class Affine: public Polynome
{
public:
    Affine(float a, float b);         //Constructeur d'une fonction affine de coefficient directeur a, et d'ordonnée à l'origine b
    Fonction* clone() const;          //Renvoie un pointeur sur une copie de la fonction affine de la classe Affine
};                                    //Héritage de Fonction::inverse, de Polynome::operator()/derivee
                                      //Héritage du destructeur de Polynome (pas de pointeur en attribut spécifique à Affine en plus de coeff, donc pas besoin de redéfinir le destructeur)

class Trigo: public Fonction
{
    string name;          //"cos", "sin" ou "tan"
public:
    Trigo(string s);                  //Constructeur d'une fonction trigonométrique de nom s (doit appartenir à {"cos","sin","tan"})
    Fonction* clone() const;          //Renvoie un pointeur sur une copie de la fonction Trigo de la classe Trigo
    float operator()(float x) const;
};                                    //Héritage de Fonction::inverse et Fonction::derivee

#endif // FONCTION_H
