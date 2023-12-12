#include<iostream>
#include<fstream>
#include<iomanip>
#include<bits/stdc++.h>
#include<algorithm>
#include<vector>
#include<numeric>
#include<string>

#include "json.hpp"

using json = nlohmann::json;
using namespace std;

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
//----------------------------------------------------------
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
//Generuje binarne kombinacji o podanej wielkości i liczbie jedynek (1)
//---------------------------------------------------------------------
vector<string> GenerateCombinations(int amountOfOnes, int size)
{
    string s = "";
    vector<string> p;
    //p.clear();
    if(size>=amountOfOnes && amountOfOnes>=0)
    {
        for(int i=0;i<size;i++)
        {
            if(size-i>amountOfOnes)
            {
                s+="0";
            }
            else
            {
                s+="1";
            }
        }
        bool val = true;
        p.push_back(s);
        while(val!=false && size > 1)
        {
            val = next_permutation(s.begin(), s.end());
            if (val == true)
            {
                p.push_back(s);
            }
        }
    }
    else
    {
        p.push_back("");
    }
    return p;
}

//Odczytuje cały vector zachowując odstęp między indexami
//--------------------------------------------------------
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

int main()
{
    const string fileInputName = "Deegres.json"; 
    const string fileOutputName = "Graphs3.json";

    json data;
    json j_vec;                                 //Do zapisu vectorów w JSON
    json j_graph;                               //Do zapisu grafu w JSON
    json dataToSave;                            //Do danych wyjściowych
    int numberOfInputGraph;                     //Liczba grafów wejściowych
    bool nextC;                                 //Czy x uległo zwiększeniu
    bool GraphPossible,AllCombinChecked;        //Czy podany graf spełnia kryteria | Czy sprawdzono wszystkie kombinacje dla wierzchołka                  
    int CombinNumber;                           //Liczba kombinacji  
    vector<int> deegre;                         //Podany stopień
    int n;                                      //liczba wierzchołków
    vector<int> avalibleDeegre = deegre;        //Dostępne wierzchołki
    vector<vector<string>> comb;                //Wszystkie kombinacje spełniające część kryterii do sprawdzenia
    vector<int> combinationIndex;               //aktualnie sprawdzana kombinacja
    vector<vector<int>> graph;                  //Graf do wyświetlenia
    int x,amountOfGraphs;                       //Aktualny wierzchołek | liczba utworzonych grafów
    
    //Odczyt danych wejściowych z pliku JSON
    ifstream file;
    file.open(fileInputName);
    file>>data;
    file.close();

    while(!data["G"+to_string(numberOfInputGraph)].is_null())
    {   
        deegre = data["G"+to_string(numberOfInputGraph)].get<std::vector<int>>();
        n = deegre.size();
        numberOfInputGraph+=1;                              //Zliczanie liczby grafów na wejściu

        //Czyszczenie danych
        amountOfGraphs = 0;
        x = 0;
        graph.clear();
        comb.clear();
        avalibleDeegre.clear();
        for(int i=0;i<n;i++)
        {
            avalibleDeegre.push_back(deegre[i]);
            comb.push_back({}); 
            combinationIndex.push_back(0);                                   
        }
        nextC = true;
        AllCombinChecked = true;

        for(int i=0;i<n*3;i++)
        {
            cout<<"-";
        }
        cout<<"\n";
        ReadVector1(avalibleDeegre);
        cout<<"\n";
        while(x>=0 && x<n-1)
        {   
            //Generowanie kombinacji
            if(nextC)
            {
                //Generowanie kombinacji jeśli ich nie ma
                if(AllCombinChecked)        //Czy wszystkie kombinacje zostały sprawdzone
                {
                    comb[x].clear();
                    comb[x]=GenerateCombinations(avalibleDeegre[x],(n-1)-x);    //Generowanie możliwych kombinacji dla wierzchołka
                    combinationIndex[x]=0;
                }
                //Sprawdzanie czy obecna kombinacja spełnia kryteria grafu
                GraphPossible = true;
                for(int j=0;j<comb[x][combinationIndex[x]].size();j++)
                {
                    int numberOfOnes = 0;
                    //Jeśli kombinacja posiada więcej relacji (1) niż pozwala założenie nie będzie dało się stworzyć grafu
                    //o podanych stopniach wierzchołka 
                    if(comb[x][combinationIndex[x]][j] == '1')
                    {
                        numberOfOnes+=1;
                        if(avalibleDeegre[j+x+1]<=0)
                        {
                            GraphPossible = false;
                        }
                    }
                    if(numberOfOnes>avalibleDeegre[x])
                    {
                        GraphPossible = false;
                    }
                }
                if(GraphPossible)
                {
                    //Jeśli spełnia kryteria to aktualizuje obecne stopnie grafu poprzez zmniejszenie dostępnych stopnii
                    for(int j=0;j<comb[x][combinationIndex[x]].size();j++)
                    {
                        if(comb[x][combinationIndex[x]][j] == '1')
                        {
                            
                            if(avalibleDeegre[x]>0)
                            {
                                avalibleDeegre[x]-=1;
                            }
                            if(avalibleDeegre[j+x+1]>0)
                            {
                                avalibleDeegre[j+x+1]-=1;
                            }
                        }
                    }
                    //Jeśli x znajduje się na przed ostatnim miejscu (ostatniego nie trzeba sprawdzać)
                    //Można sprawdzić czy da się utworzyć graf
                    if(x==n-2)
                    {
                        nextC = false;
                        bool lastCheck = true;
                        //Jeśli wszystkie dostępne stopnie są równe 0 oznacza to, że można utworzyć graf
                        for(int i=0;i<n;i++)
                        {
                            if(avalibleDeegre[i]>0)
                            {
                                lastCheck = false;
                            }
                        }
                        //Wypisanie i zapisanie grafu
                        if(lastCheck)
                        {
                            //Przygotowanie grafu
                            graph.clear();
                            for(int i=0;i<n;i++)
                            {
                                graph.push_back({});
                                graph[i].insert(graph[i].end(),n,0); //Zapełnienie grafu 0
                            }
                            
                            string vertexRelation;

                            //Tworzenie macierzy sąsiedztwa grafu
                            for(int i=0;i<n;i++)
                            {
                                //Zamiana kombinacji na relacje wierzchołka w grafie.
                                vertexRelation="";
                                for(int j=0;j<i+1;j++)
                                {
                                    vertexRelation+="0";
                                }
                                //dopisanie brakujących 0 z przodu
                                if(i<n-1)
                                {
                                    vertexRelation+=comb[i][combinationIndex[i]];
                                }
                                //Tworzenie relacji między wierzchołkami
                                for(int j=0;j<vertexRelation.size();j++)
                                {
                                    if(vertexRelation[j]=='1')
                                    {
                                        graph[i][j]=1;
                                        graph[j][i]=1;
                                    }
                                }
                                //ReadVector1(graph[i]);
                            }
                            cout<<"\n";
                            //Sprawdzanie spójności grafu
                            if(Disjoint(graph,n))
                            {
                                amountOfGraphs+=1;
                                for(int i=0;i<n;i++)
                                {
                                    j_graph["G"+to_string(amountOfGraphs)] = graph;
                                    ReadVector1(graph[i]);   
                                }
                            }
                            //Zapisać graf do JSON
                            dataToSave["D"+to_string(numberOfInputGraph)] = j_graph;
                        }
                        //Cofnięcie zmian
                        if(comb[x][combinationIndex[x]][0] == '1')
                        {
                            avalibleDeegre[x]+=1;
                            avalibleDeegre[x+1]+=1;
                        }
                    }
                    else
                    {
                        //Jeśli x nie znajduje się na przed ostatnim wierzchołku to zchodzi do
                        // niższego aby wygenerować nowe kombinacje wierzchołka
                        x+=1;
                        AllCombinChecked = true;
                    }    
                }
                else
                {
                    //Jeżeli nie da się utworzyć grafu to cofa się
                    
                    if(combinationIndex[x]<comb[x].size()-1)
                    {
                        //Przejście do następnej kombinacji wierzchołka
                        combinationIndex[x]+=1;
                        AllCombinChecked = false;
                    }
                    else
                    {
                        //Cofnięcie się do poprzedniego wierzchołka z powodu sprawdzenia wszystkich kombinacji wierzchołka
                        combinationIndex[x] = 0;
                        AllCombinChecked = true;
                        nextC = false;
                        comb[x].clear();

                    }
                }
            }
            else
            {
                //Cofanie się
                if(x>0)
                {
                    //Cofnięcie się do poprzedniego wierzchołka
                    x-=1;
                    //Cofnięcie zmian spowodowanymi odczytaniem kombinacji
                    for(int j=0;j<comb[x][combinationIndex[x]].size();j++)
                    {
                        if(comb[x][combinationIndex[x]][j] == '1')
                        {
                            avalibleDeegre[x]+=1;
                            avalibleDeegre[j+x+1]+=1;
                        }
                    }
                    if(combinationIndex[x]<comb[x].size()-1)
                    {
                        //Pzejście do następnej kombinacji
                        combinationIndex[x]+=1;
                        AllCombinChecked = false;
                        nextC = true;
                    }
                    else
                    {
                        //Cofnięcie się do poprzedniego wierzchołka
                        AllCombinChecked = true;
                        comb[x+1].clear();
                        nextC = false;
                    }
                }
                else
                {
                    //Jeśli wszystkie kombinacje dla każdego wierzchołka zostały sprawdzone ustawi x na wartość -1 co zakończy algorytm
                    x=-1;
                }
            }
        }
        if(amountOfGraphs!=0)
        {
            cout<<"Liczba wygenerowanych grafów: "<<amountOfGraphs<<endl;
        }
        else
        {
            cout<<"Brak rozwiązań"<<endl;
        }
    }
    //Zapis do pliku JSON
    ofstream file2;
    file2.open(fileOutputName);
    file2<<dataToSave.dump(1);
    file2.close(); 

    return 0;
}