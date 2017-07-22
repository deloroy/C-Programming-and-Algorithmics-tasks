#include "fonction.h"

#include <cmath>
#include <cassert>
using namespace std;


Fonction::Fonction(){   //Constructeur vide pour initialiser integrale (appelé ligne 16 dans derivee par exemple)
    integrale = 0;      //pointeur nul
}

Fonction* Fonction::clone() const{    //Renvoie un pointeur sur une copie de la fonction
    return new Fonction(*this);
}

Fonction* Fonction::derivee() const{  //Renvoie un pointeur sur la dérivée de la fonction
    Fonction* d = new Fonction;
    (*d).integrale=clone();           //clone() renvoit bien une fonction de même classe que f (méthode virtuelle, redéfinie dans les classes enfants)
    return d;
}

float Fonction::operator()(float x) const{    //Renvoie l'évaluée de la fonction en x.
    float eps=pow(10,-5);
    assert(integrale!=0);
    return ((*integrale)(x + eps) - (*integrale)(x - eps))/(2*eps);      //calcul de f comme la dérivée d'une de ses primitives
}
//Remarque importante : lorsque la méthode ci-dessus est appelée par la Fonction dérivée d'une fonction de la classe Trigo, la ligne 25 appelle l'évaluateur
//de (*integrale) qui est bien une fonction de la classe Trigo en vertu de la méthode virtuelle clone() redéfinie dans la classe enfant Trigo.


float Fonction::inverse(float y) const{    //Calcule l'antécédent de y par la fonction par la méthode de Newton
    float eps=pow(10,-6);
    float x=0.0f;
    float x_new=1.0f;   //Condition initiale à choisir telle que la dérivée de la fonction à inverser  y soit non nulle
    int n=0;
    Fonction* d=derivee();
    while (abs(x_new-x)>eps){
        float nbr_derive=(*d)(x_new);
        assert(nbr_derive!=0);
        float tmp=x_new;
        x_new=x_new+(y-(*this)(x_new))/nbr_derive;
        x=tmp;
        n+=1;
        if (n>99)   //on s'arrête à 100 itérations maximum
            break;
    }
    delete d;      //Libération mémoire
    return x_new;
}

Fonction::~Fonction(){
    if (integrale!=0)
        delete integrale;   //Libération mémoire
}





Polynome::Polynome(){   //Constructeur vide (appelé par le constructeur de Affine par exemple)
    degre=-1;
    coeff=0;  //pointeur nul
}

Polynome::Polynome(int N, float* c){  //Constructeur d'un polynome de degré N et de coefficients *c
    degre = N;
    coeff = new float[N+1];           //Allocation
    for (int i=0;i<degre+1;i++){
        coeff[i]=c[i];                //On ne juge pas la shallow copy pertinente ici
    }
}

Fonction* Polynome::clone() const{    //Renvoie un pointeur sur une copie du polynome appartenant à la classe Polynome
    return new Polynome(*this);
}

float Polynome::operator()(float x) const{    //Evaluation du polynome en x
    float val = 0;
    for (int i=0;i<degre+1;i++){
        val += coeff[i]*pow(x,i);
    }
    return val;
}

Polynome* Polynome::derivee() const{                    //Renvoie un pointeur sur la dérivée du polynome de la classe Polynome
    float* coeff_derivee = new float[degre];            //Allocation d'un tableau dynamique de degre coefficients ("1 de moins")
    for (int i=0;i<degre;i++){
        coeff_derivee[i]=(i+1)*coeff[i+1];              //Calcul des coefficients
    }
    Polynome* P = new Polynome(degre-1,coeff_derivee);   //Création du pointeur sur le polynome dérivé
    delete[] coeff_derivee;                              //Libération mémoire (pas de shallow copy dans le constructeur)
    return P;
}

Polynome::~Polynome(){
    delete[] coeff;    //Libération mémoire
}





Affine::Affine(float a, float b) {        //Constructeur d'une fonction affine de coefficient directeur a, et d'ordonnée à l'origine b
    degre = 1;                            //(appel du constructeur vide de Polynome)
    coeff = new float[2];
    coeff[0] = b;
    coeff[1] = a;
}

Fonction* Affine::clone() const{      //Renvoie un pointeur sur une copie de la fonction affine de la classe Affine
    return new Affine(*this);
}





Trigo::Trigo(string s){                               //Constructeur d'une fonction trigonométrique de nom s
    assert((s=="cos")||(s=="sin")||(s=="tan"));
    name=s;
}

Fonction* Trigo::clone() const{                      //Renvoie un pointeur sur une copie de la fonction Trigo de la classe Trigo
    return new Trigo(*this);
}

float Trigo::operator()(float x) const{
    if (name=="cos"){
        return cos(x);
    }
    else{
        if (name=="sin"){
            return sin(x);
        }
        return tan(x);
    }
}

