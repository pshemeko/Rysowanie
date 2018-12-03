#pragma once
#include <fstream>
#include <vector>
#include <string>
#include "Enum.h"

// disable logging info messages. comment this out to enable logging.

//#define SHOW_INFO(msg)
//#define SHOW_DEBUG(msg)
//#define SHOW_ENDING_INFOS(msg)
//#define SHOW_INFOS_FOR_SCRYPTS(msg)

// if LOG_INFO is disabled, we need an empty macro, else it will not compile. comment "#undef LOG_PUZZEL" out, if you want to log messages.

static int MAX_GLEBOKOSC = 7;// wartosc maksymalnej glebokosci czyli dla ilu kolumn bedzie ykres

#ifndef SHOW_INFO
#define SHOW_INFO(msg) std::cout<< msg;
#endif // !SHOW_INFO

#ifndef SHOW_DEBUG
#define SHOW_DEBUG(msg) std::cout<< msg;
#endif // !SHOW_DEBUG

#ifndef SHOW_ENDING_INFOS
#define SHOW_ENDING_INFOS(msg) std::cout<< msg;
#endif // !SHOW_ENDING_INFOS

#ifndef SHOW_INFOS_FOR_SCRYPTS
#define SHOW_INFOS_FOR_SCRYPTS(msg) std::cout<< msg;
#endif // !SHOW_INFOS_FOR_SCRYPTS

struct Dana{
	int glebokosc;// glebokosc z nazwy pliku
	int numerUkladanki;//numer ukladanki z nazwy pliku
	std::string algorytm;//rodzaj algorytmu
	std::string kolenosc;//kolejnosc, metyka
	int dlugoscRozwiazania;// dlugosc znalezionego rozwiazania
	int iloscStanowOdwiedzonych;//ilosc odwiedzonych stanow
	int iloscStanowPrzetworzonych;// ilosc przetworzonych stanow
	int maxGlebokoscrekursji;//maksymalna glebokosc rekurji
	int czas;//czas w milisekundach
	friend std::ostream& operator<< (std::ostream& stream, const Dana& matrix);
	friend std::istream& operator>>(std::istream& os, Dana& puz);
};

class Glowna
{
	char* nazwaPliku;
	std::fstream plik;
	std::vector<Dana> dane; // wszystkie ale niepotrzebne chyba
    std::vector<Dana> dfsy;
    std::vector<Dana> bfsy;
    std::vector<Dana> astars;

public:
	Glowna(char* nazwaPliku);
	~Glowna();
	void open();
	void close();
    void rysuj_Ilosc_przetworzonych_stanow(Strategie::Strategy st);


	void wczytaj(); // parsuje do plikow
	void wypisz(Strategie::Strategy st);

	void test();    // skasuj
    void rysuj_Ilosc_odwiedzonych_stanow();

};

