#pragma once
#include <fstream>
#include <vector>
#include <string>

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
	std::vector<Dana> dane;

public:
	Glowna(char* nazwaPliku);
	~Glowna();
	void open();
	void close();
	void wczytaj();
	void wypisz();

	void test();

};

