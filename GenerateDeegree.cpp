#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>
#include<string>

#include"json.hpp"

using namespace std;
using json = nlohmann::json;

//Sprawdza czy z podanych stopnii można stworzyć graf graficzny
//-------------------------------------------------------------
bool IsGraphic(vector<int> G)
{
    int x;
    if(G.size()==0)
    {
        return false;
    }
    sort(G.begin(), G.end(), greater<>());  //Sortowanie rewersyjne
    x = G[0];                               //Zapisanie 1 elementu
    G.erase(G.begin());                     //Usunięcie elementu z przodu
    if(x == 0)                              //Jeśli zostały tylko stopnie o wartości 0 oznacza, że jest graficzny
    {
        return true;
    }
    if(x>G.size())                          //Jeśli stopień wierzchołka jest większy niż liczba wierzchołków to znaczy, że graf nie jest graficzny
    {
        return false;
    }
    for(int i=0;i<x;i++)                    //Zmniejszanie stopni wierzchołków grafu
    {
        if(G[i]==0)                         //Jeśli równe 0 oznacza, że do wierzchołka o największej stopniu nie można połączyć z resztą wierzchołka (tablica jest posortowana)
        {
            return false;
        }
        else                                //Jeśli nie to zmniejsza stopień (zajmuje wierzchołek)
        {
            G[i] = G[i]-1;
        }
    }
    return IsGraphic(G);                    //Rekurencyjnie sprawdza czy graf bez wierzchołka o największej stopniu jest graficzny
    return true;
}
//Losuje wartość od min do max
//----------------------------
int Random(int min,int max)
{
    return rand()%(max-min)+min;
}
//Generowanie stopnii wierzchołków grafu
//--------------------------------------
int main()
{
    srand(time(NULL));                                          //Losowe ziarno do generowania
    int minDegree,maxDegree,NumberOfVertex,maxNumberOfVertex,amountOfData;
    vector<int> DegreeVector;
    json j_vec;
    json jsonSave;

    //Parametry Wejściowe
    const string fileOutputName = "Deegres.json";               //Nazwa pliku do, którego generowane są stopnie wierzchołków
    bool OnlyOneSize = false;                                   //Czy ma generować stopnie grafów o tej samej liczbie wierzchołków
    amountOfData = 10;                                          //Liczba wygenerowanych stopnii grafu
    maxNumberOfVertex = 6;                                      //Największy możliwy graf do wygenerowania

    for(int i=0;i<amountOfData;i++)
    {
        //Przygotowanie vectora stopnii
        if(OnlyOneSize)
        {
            NumberOfVertex = maxNumberOfVertex;                 //Podana liczba wierzchołków
        }
        else
        {
            NumberOfVertex = Random(3,maxNumberOfVertex);        //Losowanie z zakresu od 3 do n
        }
        DegreeVector.clear();                                   //czyszczenie vectora stopni
        //Losowanie danych
        for(int j=0;j<NumberOfVertex;j++)
        {
            int randomDegree = Random(1,maxNumberOfVertex-1);   //losowanie stopnia wierzchołka
            DegreeVector.push_back(randomDegree);               //dodanie wierzchołka do vectora stopnii
        }
        //Sprawdzenie danych
        if(IsGraphic(DegreeVector))                             //Sprawdzenie czy graf jest graficzny
        {
            for(int j=0;j<DegreeVector.size();j++)              //Wypisanie danych 
            {
                cout<<DegreeVector[j]<<" ";
            }
            //Zapisanie stopni
            j_vec = DegreeVector;
            jsonSave["G"+to_string(i)] = j_vec;                 //Oznaczenie Gn
            cout<<"\n";
        }
        else                                                    //Losowanie następnych stopnii
        {
            i-=1;
            continue;
        } 
    }
    //Zapis do pliku
    ofstream file;
    file.open(fileOutputName);
    file<<jsonSave.dump(1);
    file.close();
    return 0;
}