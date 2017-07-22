#include <iostream>
#include <string>
#include <vector>

using namespace std;

int Distance(string s, string t, bool est_Damerau){
    //Fonction qui rentourne la distance de Levenshtein (si est_Damerau est False) ou de Damerau-Levenstein (si est Damerau est True)
    int *D[s.size()+1]; //D[i][j] distance de s[:(i-1)] à t[:(j-1)] avec pour convention s[:-1] ou t[:-1] chaîne vide

    for (int i=0;i<s.size()+1;i++){
        D[i] = new int[t.size()+1];
    }

    //instanciation sur les problèmes initiaux
    for (int i=0;i<=s.size();i++){
        D[i][0]=i; //distance de s[:(i-1)] à la chaine vide (i suppressions dans s)
    }
    for (int j=0;j<=t.size();j++){
        D[0][j]=j; //distance de la chaîne vide à t[:(j-1)] (j insertions dans s)
    }

    //calcul de la matrice D des distances entre préfixes
    for (int i=1;i<s.size()+1;i++){
        for (int j=1;j<t.size()+1;j++){
            int p_substitution; //prix de la substitution de s[i-1] par t[j-1]
            if (s[i-1]!=t[j-1]){
                p_substitution=1;
            }
            else{
                //les caractères finaux des préfixes sont identiques, aucune modification à faire (substitution de prix nul)
                p_substitution=0;
            }
            //3 modifications possibles (suppression, insertion, substitution) pour Levenshtein
            //on choisit pour l'opération qui minimise D[i][j]
            D[i][j]=min(min(D[i-1][j]+1,D[i][j-1]+1),D[i-1][j-1]+p_substitution);
            //Les transpositions de caractères consécutifs sont aussi admises pour Damerau
            if (est_Damerau && i>1 && j>1) {
                if ((s[i-1]==t[j-2])&&(s[i-2]==t[j-1])){
                    D[i][j]=min(D[i][j],D[i-2][j-2]+1);
                }
            }
        }
    }

    //sauvegarde de la distance d'édition entre s et t
    int distance=D[s.size()][t.size()];

    //affichage des opérations successives dans le "chemin" optimal
    cout<<"On opere sur '"<<s<<"' pour aboutir a '"<<t<<"':"<<endl;
    vector<char> s_vers_t;
    //s_vers_t est le tableau des caractères de la chaîne s que l'on va modifier au fil des opérations
    for (int i=0;i<s.size();i++){
        s_vers_t.push_back(s[i]);
    }
    pair<int,int> etape=make_pair(s.size(),t.size());
    //etape est un couple d'indices caractérisant le nouveau sous-problème auquel on se ramène, qui appartient au chemin optimal
    //si etape=(i,j), le prochain sous-problème du chemin optimal consistera à évaluer la distance entre s[:i-1] et t[:j-1]

    //on ne s'embête pas dans ce problème à afficher les modifications dans l'ordre où on les a trouvées ("commutation")
    //ainsi on peut partir du problème résolu et remonter dans la matrice de distances entre préfixes (opération sur s en partant de la fin)
    int N=distance;
    while(N>0){
        if ((D[etape.first][etape.second]==D[etape.first-1][etape.second-1])&&(s[etape.first-1]==t[etape.second-1])){
            etape.first--;                                                         //aucune modification (substitution de prix nul)
            etape.second--;
        }
        else{
            if (D[etape.first][etape.second]==D[etape.first-1][etape.second]+1){                     //suppression
                s_vers_t.erase(s_vers_t.begin()+etape.first-1);
                cout<<"(suppression de "<<s[etape.first-1]<<")"<<endl;
                etape.first--;
            }
            else{
                if(D[etape.first][etape.second]==D[etape.first][etape.second-1]+1){                  //insertion
                    s_vers_t.insert(s_vers_t.begin()+etape.first,t[etape.second-1]);
                    cout<<"(insertion de "<<t[etape.second-1]<<")"<<endl;
                    etape.second--;
                }
                else{
                    if(D[etape.first][etape.second]==D[etape.first-1][etape.second-1]+1){            // substitution (de prix 1)
                        s_vers_t[etape.first-1]=t[etape.second-1];
                        cout<<"(substitution de "<<s[etape.first-1]<<" par "<<t[etape.second-1]<<")"<<endl;
                        etape.first--;
                        etape.second--;
                    }
                    else{                                                                            // transposition
                        //si on travaille en distances de Levenshtein, on atteindra jamais ce cas
                        //(la distance minimale a été atteinte pour une des trois opérations ci-dessus)
                        char tmp=s_vers_t[etape.first-1];
                        s_vers_t[etape.first-1]=s_vers_t[etape.first-2];
                        s_vers_t[etape.first-2]=tmp;
                        cout<<"(transposition de "<<s[etape.first-1]<<" et de "<<s[etape.first-2]<<")"<<endl;
                        etape.first-=2;
                        etape.second-=2;
                    }
                }
            }
            N--;   //les 4 opérations coûtent pour une unité de distance
            string mot(s_vers_t.begin(),s_vers_t.end());
            cout<<mot<<endl;   //affichage de la chaîne s modifiée, uniquement s'il y a eu une nouvelle modification
        }
    }

    for (int i=0;i<s.size()+1;i++){  //libération mémoire
        delete[] D[i];
    }

    return distance;
}

int DistanceLevenshtein(string s, string t){
    return Distance(s,t,false);
}

int DistanceDamerauLevenshtein(string s, string t){
    return Distance(s,t,true);
}

int LinearSpace_DistanceLevenshtein(string s, string t){
    //Algorithme linéaire en espace (en la taille de t) qui calcule la distance de Levenshtein
    //mais qui ne permet pas d'écrire la suite des configurations du chemin optimal
    //(je ne garde en mémoire que la dernière ligne de la matrice des distances entre préfixes)
    int *D[2];
    D[0] = new int[t.size()+1];
    D[1] = new int[t.size()+1];

    for (int j=0;j<t.size()+1;j++){
        D[0][j]=j; //distance de la chaîne vide à t[:(j-1)] (j insertions dans s)
    }

    for (int i=1;i<s.size()+1;i++){
        D[1][0]=i;   //distance de s[:(i-1)] à la chaine vide (i suppressions dans s)
        for (int j=1;j<t.size()+1;j++){
            int p_substitution;
            if (s[i-1]!=t[j-1]){
                p_substitution=1;
            }
            else{
                p_substitution=0;
            }
            D[1][j]=min(min(D[1][j-1]+1,D[0][j]+1),D[0][j-1]+p_substitution);
        }
        for (int j=0;j<=t.size();j++){  //copie de D[1] dans D[0] pour mémoire
            D[0][j]=D[1][j];
        }
    }
    int distance=D[1][t.size()];
    delete[] D[0];
    delete[] D[1];

    return distance;
}

int LinearSpace_DistanceDamerauLevenshtein(string s, string t){
    //Même principe (on conserve cette fois les deux dernières lignes de la matrice en permanence, transpositions obligent !)
    int *D[3];
    for (int i=0;i<3;i++){
        D[i] = new int[t.size()+1];
    }
    for (int j=0;j<t.size()+1;j++){
        D[0][j]=j; //distance de la chaîne vide à t[:(j-1)] (j insertions dans s)
    }
    D[1][0]=1;
    for (int j=1;j<t.size()+1;j++){
        int p_substitution;
        if (s[0]!=t[j-1]){
            p_substitution=1;
        }
        else{
            p_substitution=0;
        }
        D[1][j]=min(min(D[0][j]+1,D[1][j-1]+1), D[0][j-1]+p_substitution); //pas de transposition pour les préfixes de s à 1 caractère
    }

    for (int i=2;i<s.size()+1;i++){
        D[2][0]=i;   //distance de s[:i] à la chaine vide (i+1 suppressions dans s)
        for (int j=1;j<t.size()+1;j++){
            int p_substitution;
            if (s[i-1]!=t[j-1]){
                p_substitution=1;
            }
            else{
                p_substitution=0;
            }
            D[2][j]=min(min(D[2][j-1]+1,D[1][j]+1),D[1][j-1]+p_substitution);
            if (i>1 && j>1) {
                if ((s[i-1]==t[j-2])&&(s[i-2]==t[j-1])){
                    D[2][j]=min(D[2][j],D[0][j-2]+1);
                }
            }
        }
        for (int j=0;j<=t.size();j++){  //on garde en mémoire les deux dernières lignes D[1] et D[2]
            D[0][j]=D[1][j];
            D[1][j]=D[2][j];
        }
    }
    int distance=D[2][t.size()];
    for (int i=0;i<3;i++){
        delete[] D[i];
    }
    return distance;
}

int main(){
    string chaine1 = "trier";
    string chaine2 = "retirer";
    cout << "Chaine 1 : " << chaine1 <<endl;
    cout << "Chaine 2 : " << chaine2 <<endl;
    cout<<endl;

    cout<<"Question 5)6) Calcul de la distance de Levenshtein et affichage des modifications successives :"<<endl;
    cout<<"La distance de Levenshtein est egale a "<<DistanceLevenshtein(chaine1,chaine2)<<endl;
    cout<<endl;

    cout<<"Question 7) Calcul de la distance de Damerau-Levenshtein et affichage des modifications successives :"<<endl;
    cout<<"La distance de Damerau-Levenhstein est egale a "<<DistanceDamerauLevenshtein(chaine1,chaine2)<<endl;
    cout<<endl;

    cout<<"Question 8) L'algorithme lineaire en espace donne une distance de Levenshtein de ";
    cout<<LinearSpace_DistanceLevenshtein(chaine1,chaine2)<<endl;
    cout<<endl;

    cout<<"Question 8) L'algorithme lineaire en espace donne une distance de Damerau-Levenshtein de ";
    cout<<LinearSpace_DistanceDamerauLevenshtein(chaine1,chaine2)<<endl;

    return 0;
}

