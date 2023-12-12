#include<iostream>
#include<fstream>
#include<iomanip>
#include<bits/stdc++.h>
#include<algorithm>
#include<vector>
#include<numeric>
#include<string>

#include "json.hpp"

using namespace std;
using json = nlohmann::json;

//


// Typy danych dla drzew struktury zbiorów rozłącznych
//----------------------------------------------------
struct TNode
{
  TNode *up;                // Rodzic węzła
  int rank;                 // Ranga
  int data;                 // Zawartość węzła
};

// Tworzy drzewo jednowęzłowe
//---------------------------
void MakeSet(TNode *x)
{
  x->up = x;                // x staje się korzeniem drzewa
  x->rank = 0;              // zerowanie rangi
}

// Zwraca korzeń drzewa i ustawia pola up
// wszystkich węzłów nadrzędnych aż do korzenia
//---------------------------------------------
TNode *FindSet(TNode *x)
{
  if( x->up != x ) 
  {
        x->up = FindSet(x->up);
  }
  return x->up;
}

// Łączy ze sobą drzewa z x i z y
//-------------------------------
void UnionSets(TNode *x, TNode *y)
{
  TNode *rx, *ry;

  rx = FindSet(x);       // Wyznaczenie korzenia drzewa z węzłem x
  ry = FindSet(y);       // Wyznaczenie korzenia drzewa z węzłem y
  if(rx != ry)            // Korzenie muszą być różne
  {
    if(rx->rank > ry->rank) // Porównanie rangi drzew
       {
            ry->up = rx;         //Dołączanie ry
       }
    else
    {
      rx->up = ry;          //Dołączanie rx
      if(rx->rank == ry->rank) 
      {
            ry->rank++;
      }
    }
  }
}

//Sprawdzanie spójności z wykorzystaniem zbiorów rozłącznych

bool Disjoint(vector<vector<int>> Graph,int nVertex)
{
    int edgeSum = 0;                 // Liczba krawędzi
    TNode * T;
    int v, u;
    int numTree = 0;                 //liczba drzew rozpinających                      

    T = new TNode [nVertex];         // Tworzenie tablicy zbiorów rozłącznych

    for(int i=0;i<nVertex;i++)
    {
        edgeSum += accumulate(Graph[i].begin(), Graph[i].end(), 0);
    }
    edgeSum/=2;                     
    
    // Inicjacja Tablicy T
    for(int i=0;i<nVertex;i++)   
    {
        T[i].data = i;    // Numer węzła
        MakeSet(&T[i]);   // Tworzenie zbioru jednoelementowy
    }
    
    // Odczyt kolejnych krawędzi.
    for(int i=0;i<nVertex;i++)
    {
        for(int j=0;j<nVertex;j++)
        {
            if(Graph[i][j] == 1)
            { 
                UnionSets(&T[i], &T[j]);       // Łączenie zbiorów z zgodnie z tablicą macierzy
            }
        }
    }

    // Liczenie drzew rozpinających
    for(int i=0;i<nVertex;i++)
    {
        if(i == FindSet(&T[i])->data)
        {
            numTree+=1;
            //Drzewa rozpinające w grafie
            /*cout << "SCC id =" << setw(3) << i << ":";
            for(int j = 0; j < nVertex; j++ )
            {
                if( i == FindSet (&T[j])->data ) 
                {
                    cout << setw (3) << j;
                }
            }
            cout << endl;*/
        }
    }
    delete [] T;
    //Wynik sprawdzenia
    if(numTree == 1)
    {
        return 1;
    }
    else
    {
        return 0;
    }    
}

//Wypisuje wszystkie wartości z vector<int> jednowymiarowego
//----------------------------------------------------------
void ReadVector1(vector<int> vect)
{
    cout<<"[";
    for(int i=0;i<vect.size();i++)
    {
        cout<<vect[i];
        if(i<vect.size()-1)
        {
            cout<<" ";
        }
    }
    cout<<"]"<<endl;
}

//Generowanie spójnych grafów o podanym stopniu
//---------------------------------------------
int main()
{
    const string fileInputName = "Deegres.json"; 
    const string fileOutputName = "Graphs1.json"; 
    
    bool good;                                                        //Do sprawdzania czy graf spełnia kryteria (ma własciwe stopnie wierzchołków)
    int numberOfGraphs;                                               //Łączna liczba wygenerowanych grafów spełniających kryteria
    int numberOfInputGraph,numVertex,numEdges,maxEdges;               //Liczba danych wejściowych, liczba wierzchołków, liczba krawędzi, maksymalna liczba krawędzi
    json data;                                                        //Do danych z wejścia
    json j_vec;                                                       //Do zapisu vectorów w JSON
    json j_graph;                                                     //Do zapisu grafu w JSON
    json dataToSave;                                                  //Do danych wyjściowych
    vector<vector<int>> Graph;                                        //Macierz grafu
    vector<int> currentDegree;                                        //Stopnie wierzchołków grafu w generowanym grafie                  
    vector<int> inputDegree;                                          //Stopnie wierzchołków na wejściu
    
    //Odczyt danych z pliku JSON
    ifstream file;
    file.open(fileInputName);
    file>>data;
    file.close();

    numberOfInputGraph = 0;
    while(!data["G"+to_string(numberOfInputGraph)].is_null())
    {   
        inputDegree = data["G"+to_string(numberOfInputGraph)].get<std::vector<int>>();
        numVertex = inputDegree.size();
        numberOfInputGraph+=1;                              //Zliczanie liczby grafów na wejściu
        numberOfGraphs = 0;
        maxEdges = numVertex*(numVertex-1)/2;           //Maksymalna dopuszczalna liczba krawędzi w grafie nie skierowanym wynosi n*(n-1)/2    
        
        for(int i=0;i<numVertex*3;i++)
        {
            cout<<"-";
        }
        cout<<"\n";
        ReadVector1(inputDegree);
        for(int i=0;i<((int)(pow(2,maxEdges)));i++)
        {
            //Czyszczenie grafu
            good = true;
            Graph.clear();
            currentDegree.clear();
            for(int gx=0;gx<numVertex;gx++)
            {
                Graph.push_back({});                                   
                Graph[gx].insert(Graph[gx].end(),numVertex,0);        //Zapełnienie grafu 0 (usunięcie relacji między wierzchołkami)
                currentDegree.push_back(0);
            }
            //Generowanie grafu
            for(int j=0;j<maxEdges;j++)
            {
                if((i & (int)(pow(2,j))) == (int)(pow(2,j)))    //Jeśli bitowe i równe potędze 2^j
                {
                    if(j<numVertex)                             //j < liczby wierzcholkow
                    {
                        Graph[j][(j+1)%numVertex] = 1;          //funkcja dodania krawedzi w miejsce j,j+1%liczba wierzcholkow g
                        Graph[(j+1)%numVertex][j] = 1;
                        
                        currentDegree[j]+=1;
                        currentDegree[(j+1)%numVertex]+=1;
                        if(currentDegree[j]>inputDegree[j])
                        {
                            good = false;
                        }
                    }
                    if(j>=numVertex)                         //j>= liczba wierzcholkow
                    {
                        Graph[j%numVertex][(j+2)%numVertex] = 1; //funkcja dodania krawedzi w miejsce [j% liczba wierzcholkow,j+2%liczba wierzcholkow,g] 
                        Graph[(j+2)%numVertex][j%numVertex] = 1;

                        currentDegree[j%numVertex] +=1;
                        currentDegree[(j+2)%numVertex] +=1;
                        if(currentDegree[j%numVertex]>inputDegree[j%numVertex])
                        {
                            good = false;
                        }
                    }
                }
                if(!good)
                {
                    break;
                }
            }
            
            //Sprawdzenie stopnii
            for(int j=0;j<numVertex;j++)
            {
                if(currentDegree[j]!=inputDegree[j])
                {
                    good = false;
                }
            }
            //Algorytm minimalnych drzew rozpinających - czy graf posiada 1 minimalne drzewo rozpinające (jest spójny)
            if(good)
            {
                if(Disjoint(Graph,numVertex))
                {
                    good = true;
                }
                else
                {
                    good = false;
                }
            }
            //Wypisanie
            if(good)
            {
                numberOfGraphs+=1;
                cout<<"\n"; 
                for(int gx=0;gx<numVertex;gx++)
                {
                    j_graph["G"+to_string(numberOfGraphs)] = Graph;
                    ReadVector1(Graph[gx]);
                } 
                //Zapis do formatu JSON
                dataToSave["D"+to_string(numberOfInputGraph)] = j_graph;
            }   
        }
        if(numberOfGraphs==0)
        {
            cout<<"Brak spójnych Grafów"<<endl;
        }
        else
        {
            cout<<"\nLiczba grafów: "<<numberOfGraphs<<endl;
        }
    }
    //Zapis do pliku JSON
    ofstream file2;
    file2.open(fileOutputName);
    file2<<dataToSave.dump(1);
    file2.close(); 
    
    return 0;
}