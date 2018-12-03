#include "pch.h"
#include "Dane.h"
#include "Gnuplot.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;


Glowna::Glowna(char* fileOutput)
	: nazwaPliku{fileOutput}
{
}


Glowna::~Glowna()
{
}

void Glowna::open()
{
	//plik.open(nazwaPliku, std::ios::out);
	//plik.open(nazwaPliku, std::ios::in | std::ios::out);
	plik.open(nazwaPliku, std::ios_base::in);
}

void Glowna::close()
{
	plik.close();
}


void Glowna::wczytaj()
{
	if (plik.is_open())
	{
        SHOW_DEBUG("plik otwartyyyyyy" << std::endl;);
		
        Dana elem;
		
		while (!plik.eof())
		{
			plik >> elem;
            if (elem.algorytm == "dfs"  || elem.algorytm == "DFS")  dfsy.push_back(elem);
            if (elem.algorytm == "bfs"  || elem.algorytm == "BFS")  bfsy.push_back(elem);
            if (elem.algorytm == "astr" || elem.algorytm == "ASTR") astars.push_back(elem);
 			dane.push_back(elem);
		}
	}	
    else SHOW_DEBUG("plik nie zostal otwarty" << std::endl;);	
}


void Glowna::wypisz(Strategie::Strategy st)
{
    
    if(st == Strategie::Strategy::all)
    { 
        for (auto x : dane)	SHOW_DEBUG(x << std::endl;);
    }
    if (st == Strategie::Strategy::bfs)
    {
        for (auto x : bfsy)	SHOW_DEBUG(x << std::endl;);
    }
    if (st == Strategie::Strategy::dfs)
    {
        for (auto x : dfsy)	SHOW_DEBUG(x << std::endl;);
    }
    if (st == Strategie::Strategy::astr)
    {
        for (auto x : astars)	SHOW_DEBUG(x << std::endl;);
    }
    
}

void Glowna::test() //skasuj
{
	Dana aa;
	aa.algorytm = "dfs";
	aa.czas = 777;
	aa.dlugoscRozwiazania = 3;
	aa.glebokosc = 3;
	aa.iloscStanowOdwiedzonych = 19;
	aa.iloscStanowPrzetworzonych = 99999393;
	aa.kolenosc = "lurd";
	aa.maxGlebokoscrekursji = 20;
	aa.numerUkladanki = 7;

	std::fstream pp;
	pp.open("z.txt", std::ios::out);
	if (pp.is_open())
	{
		pp << aa <<endl;
		pp << aa;
	}
    else SHOW_DEBUG("blad otwarcia pliku";);

}


void Glowna::rysuj_Ilosc_przetworzonych_stanow(Strategie::Strategy st)
{
    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    std::string nazwaPoczatkowaPliku = "przetworzone_stany_";
    //std::string nazwaAlgorytmu = Strategie::toString(st)+"_";

  
    // gdy wybrano all    
    if (st == Strategie::Strategy::all)
    {
        std::vector<int> dfsWartosci(MAX_GLEBOKOSC); // tworze wektor z suma wartosci dla kazdej glebokosci rekursji
        std::vector<int> bfsWartosci(MAX_GLEBOKOSC);
        std::vector<int> astrWartosci(MAX_GLEBOKOSC);
       // std::vector<int> wartosci(MAX_GLEBOKOSC);
        // gdy all
        //std::vector<int> wartosci(MAX_GLEBOKOSC);
    
        for (int i = 0; i< MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
            int sumaDfs = 0, sumaBfs = 0, sumaAstar = 0;
            for (auto x : *wsk) // przegladam tabloce
            {   
                if (i + 1 == x.glebokosc) // jesli glebokosc jest rta ktora badam
                {
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::bfs)) // i gdy strategia jest ta wybrana
                    {
                        bfsWartosci[i] += x.iloscStanowPrzetworzonych; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaBfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::dfs)) // i gdy strategia jest ta wybrana
                    {
                        dfsWartosci[i] += x.iloscStanowPrzetworzonych; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaDfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::astr)) // i gdy strategia jest ta wybrana
                    {
                        astrWartosci[i] += x.iloscStanowPrzetworzonych; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaAstar;
                    }
                }
            }
            if(sumaDfs != 0 && sumaBfs != 0 && sumaAstar != 0)
            { 
                bfsWartosci[i] = bfsWartosci[i] / sumaBfs;  // licze srednia
                dfsWartosci[i] = dfsWartosci[i] / sumaDfs;
                astrWartosci[i] = astrWartosci[i] / sumaAstar;
            }
            

        }
    
        // zapisz plik z wynikami
        std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            for (int i = 0; i < MAX_GLEBOKOSC; ++i)
            {
                pliczek << i+1 << " " << bfsWartosci[i] << " " << dfsWartosci[i] << " " << astrWartosci[i] << std::endl;
            }
            pliczek.close();
            
        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);
    }
    else if (st == Strategie::Strategy::bfs || st == Strategie::Strategy::dfs)
    {
        //std::vector<int> dfsWartosci(MAX_GLEBOKOSC); // tworze wektor z suma wartosci dla kazdej glebokosci rekursji
        //std::vector<int> bfsWartosci(MAX_GLEBOKOSC);
        //std::vector<int> astrWartosci(MAX_GLEBOKOSC);
        //std::vector<int> wartosci(MAX_GLEBOKOSC);

        std::vector< std::vector<int> > wartosci;
		int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(8)); // 8 porz¹dków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {                 //      x       xor      x              x     x                 x             x               c
            // mamy porzadki 'RDUL', 'RDLU',    'DRUL',     'DRLU',     'LUDR',         'LURD',     'ULDR',         'ULRD'
            //int sumaRDUL = 0, sumaRDLU = 0, sumaDRUL = 0, sumaDRLU = 0, sumaLUDR = 0, sumaLURD = 0, sumaULDR = 0, sumaULRD = 0;
            std::vector<int> suma(8);
			
            for (auto x : *wsk) // przegladam tablice
            {
                if (i + 1 == x.glebokosc)
                {
                    if (x.algorytm == Strategie::toString(st)) // i gdy strategia jest ta wybrana
                    {
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::RDUL)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaRDUL;
                            suma[Strategie::Porzadki::RDUL]++;
                            wartosci[i][Strategie::Porzadki::RDUL] += x.iloscStanowPrzetworzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::RDLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaRDLU;
                            suma[Strategie::Porzadki::RDLU]++;
                            wartosci[i][Strategie::Porzadki::RDLU] += x.iloscStanowPrzetworzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRUL)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRUL;
                            suma[Strategie::Porzadki::DRUL]++;
                            wartosci[i][Strategie::Porzadki::DRUL] += x.iloscStanowPrzetworzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRLU;
                            suma[Strategie::Porzadki::DRLU]++;
                            wartosci[i][Strategie::Porzadki::DRLU] += x.iloscStanowPrzetworzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LUDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLUDR;
                            suma[Strategie::Porzadki::LUDR]++;
                            wartosci[i][Strategie::Porzadki::LUDR] += x.iloscStanowPrzetworzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LURD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLURD;
                            suma[Strategie::Porzadki::LURD]++;
                            wartosci[i][Strategie::Porzadki::LURD] += x.iloscStanowPrzetworzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULDR;
                            suma[Strategie::Porzadki::ULDR]++;
                            wartosci[i][Strategie::Porzadki::ULDR] += x.iloscStanowPrzetworzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULRD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULRD;
                            suma[Strategie::Porzadki::ULRD]++;
                            wartosci[i][Strategie::Porzadki::ULRD] += x.iloscStanowPrzetworzonych;
                        }

                        //wartosci[i]
                        //bfsWartosci[i] += x.iloscStanowPrzetworzonych; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        //++sumaBfs;
                    }
                }            
            }
            for (int j = 0; j < 8; ++j)
            {
             if(suma[j] !=0 ) wartosci[i][j] = wartosci[i][j] / suma[j];
            }
           
        }

        // zapisz plik z wynikami
        std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            for (int i = 0; i < MAX_GLEBOKOSC; ++i) // i to sa numery glebokosci
            { 
                pliczek << i + 1;
                for (int j = 0; j < 8; j++) // j to sa po kolei kolejnosci tj po kolei 'RDUL', 'RDLU', 'DRUL', 'DRLU', 'LUDR',  'LURD', 'ULDR', 'ULRD'
                {
                    pliczek << " " << wartosci[i][j];
                }
                pliczek << std::endl;
            }
            pliczek.close();

        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);




    }

    else if (st == Strategie::Strategy::astr)
    {
        //TODO



    }
}









void Glowna::rysuj_Ilosc_odwiedzonych_stanow()// olej to
{
    // rysuje ilosc odwiedzonych stanow
    std::vector<Dana> doRysowania;
    for (auto x : dane)
    {
        if (x.algorytm == "dfs" || x.algorytm == "DFS") doRysowania.push_back(x);
    }

    std::fstream pliczek;
    
    pliczek.open("rysuj1.txt", std::ios::out);
    if (pliczek.is_open())
    {
        for (int i = 1; i < 8; ++i)
        {
            int suma = 0, ilosc = 0;
            for (auto x : doRysowania)
            {
                if (x.glebokosc == i)
                {
                    suma += x.iloscStanowOdwiedzonych;
                    ++ilosc;
                }
            }
            pliczek << i << " " << suma / ilosc << std::endl;
         }
        pliczek.close();
    }
    else SHOW_DEBUG("\n Nie mozna otworzyc pliku" << "rysuj1.txt";);

    std::string s0 = "plot ";
    string s1 = " w points ps 0.5 pt 7 ";
    s0 = s0 + "'rysuj1.txt' w points ps 2 pt 1 lc rgb 'red' \n";    //TODO zmien i dodaj to zakomentowane na koncu jak cos
    std::string nn = "rysunek";
    rysuj1(s0, nn);
}


std::ostream& operator<<(std::ostream& os, const Dana& element)
{
	os << element.glebokosc << " "
		<< element.numerUkladanki << " "
		<< element.algorytm << " "
		<< element.kolenosc << " "
		<< element.dlugoscRozwiazania << " "
		<< element.iloscStanowOdwiedzonych << " "
		<< element.iloscStanowPrzetworzonych << " "
		<< element.maxGlebokoscrekursji << " "
		<< element.czas ;

	return os;
}
std::istream& operator>>(std::istream& os,  Dana& element)
{
	os >> element.glebokosc >> 
		 element.numerUkladanki >>
		 element.algorytm >>
		 element.kolenosc >>
		 element.dlugoscRozwiazania >>
		 element.iloscStanowOdwiedzonych >>
		 element.iloscStanowPrzetworzonych >>
		 element.maxGlebokoscrekursji >>
		 element.czas  ;

	return os;
}