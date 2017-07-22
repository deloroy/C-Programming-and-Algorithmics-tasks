#include "fonction.h"

int main(){

    float* coefficients=new float[4];
    for (int i=0; i<3; i++){
        coefficients[i]=0.0f;
    }
    coefficients[3]=1.0f;
    Polynome P(3,coefficients);    //Polynome P : x->x^3
    cout<<P.inverse(27)<<endl;     //P^(-1)(27) ?
    delete[] coefficients;

    Trigo T("tan");                //Fonction x->tan(x)
    cout<<T.inverse(1)*4<<endl;    //4*atan(1) ?

    Affine A(2,3);                 //Fonction f : x->2*x+3
    cout<<A.inverse(6)<<endl;      //f^(-1)(6) ?

    return 0;
}

