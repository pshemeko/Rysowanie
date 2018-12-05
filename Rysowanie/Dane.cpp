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
    open();
	if (plik.is_open())
	{
        SHOW_DEBUG("plik otwartyyyyyy" << std::endl;);
		
        Dana elem;
		
		while (!plik.eof())
		{
			plik >> elem;
            if (elem.algorytm == "dfs"  || elem.algorytm == "DFS")  dfsy.push_back(elem);
            if (elem.algorytm == "bfs"  || elem.algorytm == "BFS")  bfsy.push_back(elem);
            if (elem.algorytm == "astr" || elem.algorytm == "ASTR") 
                astars.push_back(elem);
 			dane.push_back(elem);
		}
	}	
    else SHOW_DEBUG("plik nie zostal otwarty" << std::endl;);	
    close();
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


void Glowna::licz_Ilosc_przetworzonych_stanow(Strategie::Strategy st)
{
    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    //std::string nazwaPoczatkowaPliku = "przetworzone_stany_";
    std::string nazwaPliku = "przetworzone_stany_" + Strategie::toString(st) + ".txt";
    std::fstream pliczek;// (nazwaPliku);
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
        //std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        //std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania ze wszystkich porzadkow" << std::endl
                << "#glebokosc, bfs, dfs, astar" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i)
            {
                pliczek << i+1 << " " << bfsWartosci[i] << " " << dfsWartosci[i] << " " << astrWartosci[i] << std::endl;
            }
            pliczek.close();
            
        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        //TO DO rysuj

    }
    else if (st == Strategie::Strategy::bfs || st == Strategie::Strategy::dfs)
    {

        std::vector< std::vector<int> > wartosci;
		int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(8)); // 8 porz¹dków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        { 
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
                    }
                }            
            }
            for (int j = 0; j < 8; ++j)
            {
             if(suma[j] !=0 ) wartosci[i][j] = wartosci[i][j] / suma[j];
            }
           
        }

        // zapisz plik z wynikami
        //std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        //std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) +" !! w zaleznosci od glebokosci rozwiazania" << std::endl 
                << "#glebokosc, 'RDUL', 'RDLU', 'DRUL', 'DRLU', 'LUDR',  'LURD', 'ULDR', 'ULRD'" << std::endl;
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
		// rysowanie

		std::string komenda = " plot '" + nazwaPliku + "' using 1:2 with boxes  fs solid 1; "; //+

			" replot '" + nazwaPliku + "' using 1:3 with boxes fs solid 1; " +
			" replot '" + nazwaPliku + "' using 1:4 with boxes fs solid 1; " +
			" replot '" + nazwaPliku + "' using 1:5 with boxes fs solid 1; " +
			" replot '" + nazwaPliku + "' using 1:6 with boxes fs solid 1; " +
			" replot '" + nazwaPliku + "' using 1:7 with boxes fs solid 1; " +
			" replot '" + nazwaPliku + "' using 1:8 with boxes fs solid 1; " +
			" replot '" + nazwaPliku + "' using 1:9 with boxes fs solid 1; ";
		//// + odchylenie  tj 1:2:10

		std::string dwa = "  set boxwidth 0.5; plot  'c:/a/b.txt' using 1:2 with boxes; replot 'c:/a/b.txt' using 1:2:3 with yerrorbars ; ";
		
		//rysuj1(komenda, nazwaPliku);


    }

    else if (st == Strategie::Strategy::astr)
    {
        //TODO
        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(2)); // 8 porzadków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
            std::vector<int> suma(2); // 2 metryki

            for (auto x : *wsk) // przegladam tablice
            {
                if (i + 1 == x.glebokosc)
                {
                    if (x.algorytm == Strategie::toString(st)) // i gdy strategia jest ta wybrana
                    {
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::manh)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::manh]++;
                            wartosci[i][Strategie::Heuristics::manh] += x.iloscStanowPrzetworzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::hamm)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::hamm]++;
                            wartosci[i][Strategie::Heuristics::hamm] += x.iloscStanowPrzetworzonych;
                        }
                    }
                }
            }
            for (int j = 0; j < 2; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }
        }

        // zapisz plik z wynikami
        //std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        //std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            // nag³ówek pliku 
            pliczek << "#srednie wartosci danych metryk  A* GWIAZDKA w zaleznosci od glebokosci rozwiazania" << std::endl <<"#glebokosc, hamm, manh" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i) // i to sa numery glebokosci
            {
                pliczek << i + 1;
                for (int j = 0; j < 2; j++) // j to sa po kolei kolejnosci tj po kolei: hamm, manh
                {
                    pliczek << " " << wartosci[i][j];
                }
                pliczek << std::endl;
            }
            pliczek.close();
        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        // TODO rysowanie



    }
}


void Glowna::licz_Dlugosc_Znalezionego_rozwiazania(Strategie::Strategy st)
{
    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    std::string nazwaPoczatkowaPliku = "dlugosc_znalezionego_rozwiazania";
    //std::string nazwaAlgorytmu = Strategie::toString(st)+"_";
    std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
    std::fstream pliczek;// (nazwaPliku);


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
                        bfsWartosci[i] += x.dlugoscRozwiazania; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaBfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::dfs)) // i gdy strategia jest ta wybrana
                    {
                        dfsWartosci[i] += x.dlugoscRozwiazania; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaDfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::astr)) // i gdy strategia jest ta wybrana
                    {
                        astrWartosci[i] += x.dlugoscRozwiazania; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaAstar;
                    }
                }
            }
            if (sumaDfs != 0 && sumaBfs != 0 && sumaAstar != 0)
            {
                bfsWartosci[i] = bfsWartosci[i] / sumaBfs;  // licze srednia
                dfsWartosci[i] = dfsWartosci[i] / sumaDfs;
                astrWartosci[i] = astrWartosci[i] / sumaAstar;
            }


        }

        // zapisz plik z wynikami
        //std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        //std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania ze wszystkich porzadkow" << std::endl
                << "#glebokosc, bfs, dfs, astar" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i)
            {
                pliczek << i + 1 << " " << bfsWartosci[i] << " " << dfsWartosci[i] << " " << astrWartosci[i] << std::endl;
            }
            pliczek.close();

        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        //TO DO rysuj

    }
    else if (st == Strategie::Strategy::bfs || st == Strategie::Strategy::dfs)
    {

        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(8)); // 8 porz¹dków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
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
                            wartosci[i][Strategie::Porzadki::RDUL] += x.dlugoscRozwiazania;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::RDLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaRDLU;
                            suma[Strategie::Porzadki::RDLU]++;
                            wartosci[i][Strategie::Porzadki::RDLU] += x.dlugoscRozwiazania;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRUL)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRUL;
                            suma[Strategie::Porzadki::DRUL]++;
                            wartosci[i][Strategie::Porzadki::DRUL] += x.dlugoscRozwiazania;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRLU;
                            suma[Strategie::Porzadki::DRLU]++;
                            wartosci[i][Strategie::Porzadki::DRLU] += x.dlugoscRozwiazania;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LUDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLUDR;
                            suma[Strategie::Porzadki::LUDR]++;
                            wartosci[i][Strategie::Porzadki::LUDR] += x.dlugoscRozwiazania;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LURD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLURD;
                            suma[Strategie::Porzadki::LURD]++;
                            wartosci[i][Strategie::Porzadki::LURD] += x.dlugoscRozwiazania;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULDR;
                            suma[Strategie::Porzadki::ULDR]++;
                            wartosci[i][Strategie::Porzadki::ULDR] += x.dlugoscRozwiazania;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULRD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULRD;
                            suma[Strategie::Porzadki::ULRD]++;
                            wartosci[i][Strategie::Porzadki::ULRD] += x.dlugoscRozwiazania;
                        }
                    }
                }
            }
            for (int j = 0; j < 8; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }

        }

        // zapisz plik z wynikami
        //std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        //std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania" << std::endl
                << "#glebokosc, 'RDUL', 'RDLU', 'DRUL', 'DRLU', 'LUDR',  'LURD', 'ULDR', 'ULRD'" << std::endl;
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
        // rysowanie

        std::string komenda = " plot '" + nazwaPliku + "' using 1:2 with boxes  fs solid 1; "; //+

        " replot '" + nazwaPliku + "' using 1:3 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:4 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:5 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:6 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:7 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:8 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:9 with boxes fs solid 1; ";
        //// + odchylenie  tj 1:2:10

        std::string dwa = "  set boxwidth 0.5; plot  'c:/a/b.txt' using 1:2 with boxes; replot 'c:/a/b.txt' using 1:2:3 with yerrorbars ; ";

        //rysuj1(komenda, nazwaPliku);


    }

    else if (st == Strategie::Strategy::astr)
    {
        //TODO
        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(2)); // 8 porzadków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
            std::vector<int> suma(2); // 2 metryki

            for (auto x : *wsk) // przegladam tablice
            {
                if (i + 1 == x.glebokosc)
                {
                    if (x.algorytm == Strategie::toString(st)) // i gdy strategia jest ta wybrana
                    {
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::manh)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::manh]++;
                            wartosci[i][Strategie::Heuristics::manh] += x.dlugoscRozwiazania;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::hamm)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::hamm]++;
                            wartosci[i][Strategie::Heuristics::hamm] += x.dlugoscRozwiazania;
                        }
                    }
                }
            }
            for (int j = 0; j < 2; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }
        }

        // zapisz plik z wynikami
        //std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        //std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            // nag³ówek pliku 
            pliczek << "#srednie wartosci danych metryk  A* GWIAZDKA w zaleznosci od glebokosci rozwiazania" << std::endl << "#glebokosc, hamm, manh" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i) // i to sa numery glebokosci
            {
                pliczek << i + 1;
                for (int j = 0; j < 2; j++) // j to sa po kolei kolejnosci tj po kolei: hamm, manh
                {
                    pliczek << " " << wartosci[i][j];
                }
                pliczek << std::endl;
            }
            pliczek.close();
        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        // TODO rysowanie



    }
}


void Glowna::licz_Ilosc_odwiedzonych_stanow(Strategie::Strategy st)
{
    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    //std::string nazwaPoczatkowaPliku = "przetworzone_stany_";
    std::string nazwaPliku = "odwiedzone_stany_" + Strategie::toString(st) + ".txt";
    std::fstream pliczek;// (nazwaPliku);
                         //std::string nazwaAlgorytmu = Strategie::toString(st)+"_";


                         // gdy wybrano all    
    if (st == Strategie::Strategy::all)
    {
        std::vector<int> dfsWartosci(MAX_GLEBOKOSC); // tworze wektor z suma wartosci dla kazdej glebokosci rekursji
        std::vector<int> bfsWartosci(MAX_GLEBOKOSC);
        std::vector<int> astrWartosci(MAX_GLEBOKOSC);

        for (int i = 0; i< MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
            int sumaDfs = 0, sumaBfs = 0, sumaAstar = 0;
            for (auto x : *wsk) // przegladam tabloce
            {
                if (i + 1 == x.glebokosc) // jesli glebokosc jest rta ktora badam
                {
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::bfs)) // i gdy strategia jest ta wybrana
                    {
                        bfsWartosci[i] += x.iloscStanowOdwiedzonych; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaBfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::dfs)) // i gdy strategia jest ta wybrana
                    {
                        dfsWartosci[i] += x.iloscStanowOdwiedzonych; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaDfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::astr)) // i gdy strategia jest ta wybrana
                    {
                        astrWartosci[i] += x.iloscStanowOdwiedzonych; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaAstar;
                    }
                }
            }
            if (sumaDfs != 0 && sumaBfs != 0 && sumaAstar != 0)
            {
                bfsWartosci[i] = bfsWartosci[i] / sumaBfs;  // licze srednia
                dfsWartosci[i] = dfsWartosci[i] / sumaDfs;
                astrWartosci[i] = astrWartosci[i] / sumaAstar;
            }


        }

        // zapisz plik z wynikami

        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania ze wszystkich porzadkow" << std::endl
                << "#glebokosc, bfs, dfs, astar" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i)
            {
                pliczek << i + 1 << " " << bfsWartosci[i] << " " << dfsWartosci[i] << " " << astrWartosci[i] << std::endl;
            }
            pliczek.close();

        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        //TO DO rysuj

    }
    else if (st == Strategie::Strategy::bfs || st == Strategie::Strategy::dfs)
    {

        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(8)); // 8 porz¹dków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
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
                            wartosci[i][Strategie::Porzadki::RDUL] += x.iloscStanowOdwiedzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::RDLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaRDLU;
                            suma[Strategie::Porzadki::RDLU]++;
                            wartosci[i][Strategie::Porzadki::RDLU] += x.iloscStanowOdwiedzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRUL)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRUL;
                            suma[Strategie::Porzadki::DRUL]++;
                            wartosci[i][Strategie::Porzadki::DRUL] += x.iloscStanowOdwiedzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRLU;
                            suma[Strategie::Porzadki::DRLU]++;
                            wartosci[i][Strategie::Porzadki::DRLU] += x.iloscStanowOdwiedzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LUDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLUDR;
                            suma[Strategie::Porzadki::LUDR]++;
                            wartosci[i][Strategie::Porzadki::LUDR] += x.iloscStanowOdwiedzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LURD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLURD;
                            suma[Strategie::Porzadki::LURD]++;
                            wartosci[i][Strategie::Porzadki::LURD] += x.iloscStanowOdwiedzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULDR;
                            suma[Strategie::Porzadki::ULDR]++;
                            wartosci[i][Strategie::Porzadki::ULDR] += x.iloscStanowOdwiedzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULRD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULRD;
                            suma[Strategie::Porzadki::ULRD]++;
                            wartosci[i][Strategie::Porzadki::ULRD] += x.iloscStanowOdwiedzonych;
                        }
                    }
                }
            }
            for (int j = 0; j < 8; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }

        }

        // zapisz plik z wynikami

        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania" << std::endl
                << "#glebokosc, 'RDUL', 'RDLU', 'DRUL', 'DRLU', 'LUDR',  'LURD', 'ULDR', 'ULRD'" << std::endl;
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
        // rysowanie

        std::string komenda = " plot '" + nazwaPliku + "' using 1:2 with boxes  fs solid 1; "; //+

        " replot '" + nazwaPliku + "' using 1:3 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:4 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:5 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:6 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:7 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:8 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:9 with boxes fs solid 1; ";
        //// + odchylenie  tj 1:2:10

        std::string dwa = "  set boxwidth 0.5; plot  'c:/a/b.txt' using 1:2 with boxes; replot 'c:/a/b.txt' using 1:2:3 with yerrorbars ; ";

        //rysuj1(komenda, nazwaPliku);


    }

    else if (st == Strategie::Strategy::astr)
    {
        //TODO
        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(2)); // 8 porzadków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
            std::vector<int> suma(2); // 2 metryki

            for (auto x : *wsk) // przegladam tablice
            {
                if (i + 1 == x.glebokosc)
                {
                    if (x.algorytm == Strategie::toString(st)) // i gdy strategia jest ta wybrana
                    {
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::manh)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::manh]++;
                            wartosci[i][Strategie::Heuristics::manh] += x.iloscStanowOdwiedzonych;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::hamm)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::hamm]++;
                            wartosci[i][Strategie::Heuristics::hamm] += x.iloscStanowOdwiedzonych;
                        }
                    }
                }
            }
            for (int j = 0; j < 2; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }
        }

        // zapisz plik z wynikami

        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            // nag³ówek pliku 
            pliczek << "#srednie wartosci danych metryk  A* GWIAZDKA w zaleznosci od glebokosci rozwiazania" << std::endl << "#glebokosc, hamm, manh" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i) // i to sa numery glebokosci
            {
                pliczek << i + 1;
                for (int j = 0; j < 2; j++) // j to sa po kolei kolejnosci tj po kolei: hamm, manh
                {
                    pliczek << " " << wartosci[i][j];
                }
                pliczek << std::endl;
            }
            pliczek.close();
        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        // TODO rysowanie



    }
}


void Glowna::licz_Maksymalna_glebokosc_rekursji(Strategie::Strategy st)
{
    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    //std::string nazwaPoczatkowaPliku = "przetworzone_stany_";
    std::string nazwaPliku = "maksymalna_glebokosc_rekursji_" + Strategie::toString(st) + ".txt";
    std::fstream pliczek;// (nazwaPliku);
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
                        bfsWartosci[i] += x.maxGlebokoscrekursji; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaBfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::dfs)) // i gdy strategia jest ta wybrana
                    {
                        dfsWartosci[i] += x.maxGlebokoscrekursji; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaDfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::astr)) // i gdy strategia jest ta wybrana
                    {
                        astrWartosci[i] += x.maxGlebokoscrekursji; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaAstar;
                    }
                }
            }
            if (sumaDfs != 0 && sumaBfs != 0 && sumaAstar != 0)
            {
                bfsWartosci[i] = bfsWartosci[i] / sumaBfs;  // licze srednia
                dfsWartosci[i] = dfsWartosci[i] / sumaDfs;
                astrWartosci[i] = astrWartosci[i] / sumaAstar;
            }


        }

        // zapisz plik z wynikami
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania ze wszystkich porzadkow" << std::endl
                << "#glebokosc, bfs, dfs, astar" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i)
            {
                pliczek << i + 1 << " " << bfsWartosci[i] << " " << dfsWartosci[i] << " " << astrWartosci[i] << std::endl;
            }
            pliczek.close();

        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        //TO DO rysuj

    }
    else if (st == Strategie::Strategy::bfs || st == Strategie::Strategy::dfs)
    {

        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(8)); // 8 porz¹dków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
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
                            wartosci[i][Strategie::Porzadki::RDUL] += x.maxGlebokoscrekursji;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::RDLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaRDLU;
                            suma[Strategie::Porzadki::RDLU]++;
                            wartosci[i][Strategie::Porzadki::RDLU] += x.maxGlebokoscrekursji;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRUL)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRUL;
                            suma[Strategie::Porzadki::DRUL]++;
                            wartosci[i][Strategie::Porzadki::DRUL] += x.maxGlebokoscrekursji;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRLU;
                            suma[Strategie::Porzadki::DRLU]++;
                            wartosci[i][Strategie::Porzadki::DRLU] += x.maxGlebokoscrekursji;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LUDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLUDR;
                            suma[Strategie::Porzadki::LUDR]++;
                            wartosci[i][Strategie::Porzadki::LUDR] += x.maxGlebokoscrekursji;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LURD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLURD;
                            suma[Strategie::Porzadki::LURD]++;
                            wartosci[i][Strategie::Porzadki::LURD] += x.maxGlebokoscrekursji;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULDR;
                            suma[Strategie::Porzadki::ULDR]++;
                            wartosci[i][Strategie::Porzadki::ULDR] += x.maxGlebokoscrekursji;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULRD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULRD;
                            suma[Strategie::Porzadki::ULRD]++;
                            wartosci[i][Strategie::Porzadki::ULRD] += x.maxGlebokoscrekursji;
                        }
                    }
                }
            }
            for (int j = 0; j < 8; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }

        }

        // zapisz plik z wynikami

        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania" << std::endl
                << "#glebokosc, 'RDUL', 'RDLU', 'DRUL', 'DRLU', 'LUDR',  'LURD', 'ULDR', 'ULRD'" << std::endl;
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
        // rysowanie

        std::string komenda = " plot '" + nazwaPliku + "' using 1:2 with boxes  fs solid 1; "; //+

        " replot '" + nazwaPliku + "' using 1:3 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:4 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:5 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:6 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:7 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:8 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:9 with boxes fs solid 1; ";
        //// + odchylenie  tj 1:2:10

        std::string dwa = "  set boxwidth 0.5; plot  'c:/a/b.txt' using 1:2 with boxes; replot 'c:/a/b.txt' using 1:2:3 with yerrorbars ; ";

        //rysuj1(komenda, nazwaPliku);


    }

    else if (st == Strategie::Strategy::astr)
    {
        //TODO
        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(2)); // 8 porzadków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
            std::vector<int> suma(2); // 2 metryki

            for (auto x : *wsk) // przegladam tablice
            {
                if (i + 1 == x.glebokosc)
                {
                    if (x.algorytm == Strategie::toString(st)) // i gdy strategia jest ta wybrana
                    {
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::manh)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::manh]++;
                            wartosci[i][Strategie::Heuristics::manh] += x.maxGlebokoscrekursji;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::hamm)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::hamm]++;
                            wartosci[i][Strategie::Heuristics::hamm] += x.maxGlebokoscrekursji;
                        }
                    }
                }
            }
            for (int j = 0; j < 2; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }
        }

        // zapisz plik z wynikami
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            // nag³ówek pliku 
            pliczek << "#srednie wartosci danych metryk  A* GWIAZDKA w zaleznosci od glebokosci rozwiazania" << std::endl << "#glebokosc, hamm, manh" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i) // i to sa numery glebokosci
            {
                pliczek << i + 1;
                for (int j = 0; j < 2; j++) // j to sa po kolei kolejnosci tj po kolei: hamm, manh
                {
                    pliczek << " " << wartosci[i][j];
                }
                pliczek << std::endl;
            }
            pliczek.close();
        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        // TODO rysowanie



    }
}


void Glowna::licz_Czas_trwania_procesu(Strategie::Strategy st)
{
    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    //std::string nazwaPoczatkowaPliku = "przetworzone_stany_";
    std::string nazwaPliku = "czas_dzialania" + Strategie::toString(st) + ".txt";
    std::fstream pliczek;// (nazwaPliku);
                         //std::string nazwaAlgorytmu = Strategie::toString(st)+"_";


                         // gdy wybrano all    
    if (st == Strategie::Strategy::all)
    {
        std::vector<int> dfsWartosci(MAX_GLEBOKOSC); // tworze wektor z suma wartosci dla kazdej glebokosci rekursji
        std::vector<int> bfsWartosci(MAX_GLEBOKOSC);
        std::vector<int> astrWartosci(MAX_GLEBOKOSC);

        for (int i = 0; i< MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
            int sumaDfs = 0, sumaBfs = 0, sumaAstar = 0;
            for (auto x : *wsk) // przegladam tabloce
            {
                if (i + 1 == x.glebokosc) // jesli glebokosc jest rta ktora badam
                {
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::bfs)) // i gdy strategia jest ta wybrana
                    {
                        bfsWartosci[i] += x.czas; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaBfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::dfs)) // i gdy strategia jest ta wybrana
                    {
                        dfsWartosci[i] += x.czas; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaDfs;
                    }
                    if (x.algorytm == Strategie::toString(Strategie::Strategy::astr)) // i gdy strategia jest ta wybrana
                    {
                        astrWartosci[i] += x.czas; // licz sume // TODO zrob enuma ktory bedie okreslal ktoore pole
                        ++sumaAstar;
                    }
                }
            }
            if (sumaDfs != 0 && sumaBfs != 0 && sumaAstar != 0)
            {
                bfsWartosci[i] = bfsWartosci[i] / sumaBfs;  // licze srednia
                dfsWartosci[i] = dfsWartosci[i] / sumaDfs;
                astrWartosci[i] = astrWartosci[i] / sumaAstar;
            }


        }

        // zapisz plik z wynikami

        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania ze wszystkich porzadkow" << std::endl
                << "#glebokosc, bfs, dfs, astar" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i)
            {
                pliczek << i + 1 << " " << bfsWartosci[i] << " " << dfsWartosci[i] << " " << astrWartosci[i] << std::endl;
            }
            pliczek.close();

        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        //TO DO rysuj

    }
    else if (st == Strategie::Strategy::bfs || st == Strategie::Strategy::dfs)
    {

        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(8)); // 8 porz¹dków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
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
                            wartosci[i][Strategie::Porzadki::RDUL] += x.czas;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::RDLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaRDLU;
                            suma[Strategie::Porzadki::RDLU]++;
                            wartosci[i][Strategie::Porzadki::RDLU] += x.czas;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRUL)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRUL;
                            suma[Strategie::Porzadki::DRUL]++;
                            wartosci[i][Strategie::Porzadki::DRUL] += x.czas;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::DRLU)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaDRLU;
                            suma[Strategie::Porzadki::DRLU]++;
                            wartosci[i][Strategie::Porzadki::DRLU] += x.czas;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LUDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLUDR;
                            suma[Strategie::Porzadki::LUDR]++;
                            wartosci[i][Strategie::Porzadki::LUDR] += x.czas;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::LURD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaLURD;
                            suma[Strategie::Porzadki::LURD]++;
                            wartosci[i][Strategie::Porzadki::LURD] += x.czas;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULDR)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULDR;
                            suma[Strategie::Porzadki::ULDR]++;
                            wartosci[i][Strategie::Porzadki::ULDR] += x.czas;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Porzadki::ULRD)) // i gdy strategia jest ta wybrana
                        {
                            //++sumaULRD;
                            suma[Strategie::Porzadki::ULRD]++;
                            wartosci[i][Strategie::Porzadki::ULRD] += x.czas;
                        }
                    }
                }
            }
            for (int j = 0; j < 8; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }

        }

        // zapisz plik z wynikami

        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            pliczek << "#srednie wartosci danych metryki !!" + Strategie::toString(st) + " !! w zaleznosci od glebokosci rozwiazania" << std::endl
                << "#glebokosc, 'RDUL', 'RDLU', 'DRUL', 'DRLU', 'LUDR',  'LURD', 'ULDR', 'ULRD'" << std::endl;
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
        // rysowanie

        std::string komenda = " plot '" + nazwaPliku + "' using 1:2 with boxes  fs solid 1; "; //+

        " replot '" + nazwaPliku + "' using 1:3 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:4 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:5 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:6 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:7 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:8 with boxes fs solid 1; " +
            " replot '" + nazwaPliku + "' using 1:9 with boxes fs solid 1; ";
        //// + odchylenie  tj 1:2:10

        std::string dwa = "  set boxwidth 0.5; plot  'c:/a/b.txt' using 1:2 with boxes; replot 'c:/a/b.txt' using 1:2:3 with yerrorbars ; ";

        //rysuj1(komenda, nazwaPliku);


    }

    else if (st == Strategie::Strategy::astr)
    {
        //TODO
        std::vector< std::vector<int> > wartosci;
        int a = 0;
        for (int i = 0; i < MAX_GLEBOKOSC; ++i) wartosci.push_back(std::vector<int>(2)); // 8 porzadków


        for (int i = 0; i < MAX_GLEBOKOSC; ++i)  // przez wszystkie glebokosci
        {
            std::vector<int> suma(2); // 2 metryki

            for (auto x : *wsk) // przegladam tablice
            {
                if (i + 1 == x.glebokosc)
                {
                    if (x.algorytm == Strategie::toString(st)) // i gdy strategia jest ta wybrana
                    {
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::manh)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::manh]++;
                            wartosci[i][Strategie::Heuristics::manh] += x.czas;
                        }
                        if (x.kolenosc == Strategie::toString(Strategie::Heuristics::hamm)) // i gdy strategia jest ta wybrana
                        {
                            suma[Strategie::Heuristics::hamm]++;
                            wartosci[i][Strategie::Heuristics::hamm] += x.czas;
                        }
                    }
                }
            }
            for (int j = 0; j < 2; ++j)
            {
                if (suma[j] != 0) wartosci[i][j] = wartosci[i][j] / suma[j];
            }
        }

        // zapisz plik z wynikami
        //std::string nazwaPliku = nazwaPoczatkowaPliku + Strategie::toString(st) + ".txt";
        //std::fstream pliczek;// (nazwaPliku);
        pliczek.open(nazwaPliku, std::ios::out);
        if (pliczek.is_open())
        {
            // nag³ówek pliku 
            pliczek << "#srednie wartosci danych metryk  A* GWIAZDKA w zaleznosci od glebokosci rozwiazania" << std::endl << "#glebokosc, hamm, manh" << std::endl;
            for (int i = 0; i < MAX_GLEBOKOSC; ++i) // i to sa numery glebokosci
            {
                pliczek << i + 1;
                for (int j = 0; j < 2; j++) // j to sa po kolei kolejnosci tj po kolei: hamm, manh
                {
                    pliczek << " " << wartosci[i][j];
                }
                pliczek << std::endl;
            }
            pliczek.close();
        }
        else SHOW_DEBUG("blad otwarcia w rysowaniu pliku:" << nazwaPliku << std::endl;);

        // TODO rysowanie



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



void Glowna::oblicz_wszystko() {
    std::vector<Strategie::Strategy> strategie{ Strategie::Strategy::all,Strategie::Strategy::bfs,Strategie::Strategy::dfs,Strategie::Strategy::astr };
    for (auto x : strategie)
    {
        licz_Ilosc_przetworzonych_stanow(x);
        licz_Dlugosc_Znalezionego_rozwiazania(x);
        licz_Ilosc_odwiedzonych_stanow(x);
        licz_Maksymalna_glebokosc_rekursji(x);
        licz_Czas_trwania_procesu(x);

    }

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








void Glowna::zapisz_do_wykresu_przetworzonych_stanow(Strategie::Strategy st) // vek przekazuje konkretny ktora strategia
{
	/// potrzebuje w pliku 
	// porzadek LURD, glebokosc, ilosc stanow przetworzonych - naklepsze tak zrobie
	// 4 pliki jeden bfs, jeden dns, jeden astar jeden all
	//kazdy porzadek w osobnym pliku ma byc i wypoisywac poziom oraz wartosc - czyli plikow 8* - nie to zle

	std::string nazwaPliku = Strategie::toString(st) + "_przetworzone_stany_" + ".txt";
	std::fstream pliczek;// (nazwaPliku);
    
	std::shared_ptr<std::vector<Dana>> wsk;
	if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
	if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
	if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
	if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

	pliczek.open(nazwaPliku, std::ios::out);
	if (pliczek.is_open())
	{
		if (st == Strategie::Strategy::all)
		{
			for (auto x : *wsk)
			{
				pliczek << x.algorytm << " " << x.glebokosc << " " << x.iloscStanowPrzetworzonych << std::endl;
			}
		}else
		{
			for (auto x : *wsk)
			{
				pliczek << x.kolenosc << " " << x.glebokosc << " " << x.iloscStanowPrzetworzonych << std::endl;
			}
		}
	}
	else SHOW_DEBUG("Plik " + nazwaPliku + " nie zostal otwarty!!!\n";);
	pliczek.close();
}

void Glowna::zapisz_do_wykresu_dlugosc_rozwiazania(Strategie::Strategy st)
{
    //TODO

    std::string nazwaPliku = Strategie::toString(st) + "_dlugosc_rozwiazania_" + ".txt";
    std::fstream pliczek;// (nazwaPliku);

    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    pliczek.open(nazwaPliku, std::ios::out);
    if (pliczek.is_open())
    {
        if (st == Strategie::Strategy::all)
        {
            for (auto x : *wsk)
            {
                pliczek << x.algorytm << " " << x.glebokosc << " " << x.dlugoscRozwiazania << std::endl;
            }
        }
        else
        {
            for (auto x : *wsk)
            {
                pliczek << x.kolenosc << " " << x.glebokosc << " " << x.dlugoscRozwiazania << std::endl;
            }
        }
    }
    else SHOW_DEBUG("Plik " + nazwaPliku + " nie zostal otwarty!!!\n";);
    pliczek.close();
}

void Glowna::zapisz_do_wykresu_odwiedzonych_stanow(Strategie::Strategy st)
{
    std::string nazwaPliku = Strategie::toString(st) + "_odwiedzone_stany_" + ".txt";
    std::fstream pliczek;// (nazwaPliku);

    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    pliczek.open(nazwaPliku, std::ios::out);
    if (pliczek.is_open())
    {
        if (st == Strategie::Strategy::all)
        {
            for (auto x : *wsk)
            {
                pliczek << x.algorytm << " " << x.glebokosc << " " << x.iloscStanowOdwiedzonych << std::endl;
            }
        }
        else
        {
            for (auto x : *wsk)
            {
                pliczek << x.kolenosc << " " << x.glebokosc << " " << x.iloscStanowOdwiedzonych << std::endl;
            }
        }
    }
    else SHOW_DEBUG("Plik " + nazwaPliku + " nie zostal otwarty!!!\n";);
    pliczek.close();
}

void Glowna::zapisz_do_wykresu_maksymalna_glebokosc_rekursji(Strategie::Strategy st)
{
    std::string nazwaPliku = Strategie::toString(st) + "_glebokosc_rekursji_" + ".txt";
    std::fstream pliczek;// (nazwaPliku);

    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    pliczek.open(nazwaPliku, std::ios::out);
    if (pliczek.is_open())
    {
        if (st == Strategie::Strategy::all)
        {
            for (auto x : *wsk)
            {
                pliczek << x.algorytm << " " << x.glebokosc << " " << x.maxGlebokoscrekursji << std::endl;
            }
        }
        else
        {
            for (auto x : *wsk)
            {
                pliczek << x.kolenosc << " " << x.glebokosc << " " << x.maxGlebokoscrekursji << std::endl;
            }
        }
    }
    else SHOW_DEBUG("Plik " + nazwaPliku + " nie zostal otwarty!!!\n";);
    pliczek.close();
}

void Glowna::zapisz_do_wykresu_czas_procesu(Strategie::Strategy st)
{
    std::string nazwaPliku = Strategie::toString(st) + "_czas_procesu_" + ".txt";
    std::fstream pliczek;// (nazwaPliku);

    std::shared_ptr<std::vector<Dana>> wsk;
    if (st == Strategie::Strategy::all) wsk = std::make_shared<std::vector<Dana>>(dane);
    if (st == Strategie::Strategy::bfs) wsk = std::make_shared<std::vector<Dana>>(bfsy);
    if (st == Strategie::Strategy::dfs) wsk = std::make_shared<std::vector<Dana>>(dfsy);
    if (st == Strategie::Strategy::astr) wsk = std::make_shared<std::vector<Dana>>(astars);

    pliczek.open(nazwaPliku, std::ios::out);
    if (pliczek.is_open())
    {
        if (st == Strategie::Strategy::all)
        {
            for (auto x : *wsk)
            {
                pliczek << x.algorytm << " " << x.glebokosc << " " << x.czas << std::endl;
            }
        }
        else
        {
            for (auto x : *wsk)
            {
                pliczek << x.kolenosc << " " << x.glebokosc << " " << x.czas << std::endl;
            }
        }
    }
    else SHOW_DEBUG("Plik " + nazwaPliku + " nie zostal otwarty!!!\n";);
    pliczek.close();
}
