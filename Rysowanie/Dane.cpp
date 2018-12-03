#include "pch.h"
#include "Dane.h"
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
	/*
	Dana raz;
	//while (!plik.eof()) {
	std::ifstream fin;
	fin.open(nazwaPliku, std::ios_base::in | std::ios_base::binary);
	if (fin.is_open())
	{
		std::cout << "Zawartosc pliku:\n";
		while (plik.read((char*)&raz, sizeof raz))
		{
//			dane.push_back(raz);
			std::cout << raz.algorytm << std::endl;
		}
	}
	else std::cout << "plik nie zostal otwarty" << std::endl;
	*/

	
	Dana dana;
	if (plik.is_open())
	{
		std::cout << "plik otwartyyyyyy" << std::endl;
		int ile = 0;
		Dana raz,dwa, data;
		//while (!plik.eof()) {
		//std::ifstream fin;
		//fin.open("a.txt", std::ios_base::in | std::ios_base::binary);
		int a = 3;
		while (--a)
		{
			/*
			plik >> data.glebokosc;
			
			plik >> data.numerUkladanki;
			//	plik.get();
			plik >> data.algorytm;
			//	plik.get();
			plik >> data.kolenosc;
			//	plik.get();
				plik >> data.dlugoscRozwiazania;
			//	plik.get();
				plik >> data.iloscStanowOdwiedzonych;
			//	plik.get();
				plik >> data.iloscStanowPrzetworzonych;
			//	plik.get();
				plik >> data.maxGlebokoscrekursji;
			//	plik.get();
				plik >> data.czas;
				*/
					plik >> raz;
 				dane.push_back(raz);
		}
		//dane.push_back(raz);
		//dane.push_back(dwa);

	/*		std::cout << "Zawartosc pliku:\n";
			while (plik.read((char*)&raz, sizeof raz))
			{
				dane.push_back(raz);
				std::cout << raz << std::endl;
			}
*/
		//pêtla nieskoñczona
		//{
		//	//Dana data;
		//	plik >> data.glebokosc >> data.numerUkladanki >> data.algorytm
		//		>> data.kolenosc >> data.dlugoscRozwiazania >> data.iloscStanowOdwiedzonych
		//		>> data.iloscStanowPrzetworzonych >> data.maxGlebokoscrekursji >> data.czas;
		//	dane.push_back(data);
		//	if (plik.good())
		//		std::cout << ile++ << " ";
		//	else
		//		break; //zakoñcz wczytywanie danych - wyst¹pi³ jakiœ b³¹d (np. nie ma wiêcej danych w pliku)

		//}
		//
		
		
		//
		//while (ile--) {
		//	Dana data;

		//	getline(plik, data);
		//	plik >> data.glebokosc;
		//	plik.get();
		//	plik >> data.numerUkladanki;
		//	plik.get();
		//	plik >> data.algorytm;
		//	plik.get();
		//	plik >> data.kolenosc;
		//	plik.get();
		//	plik >> data.dlugoscRozwiazania;
		//	plik.get();
		//	plik >> data.iloscStanowOdwiedzonych;
		//	plik.get();
		//	plik >> data.iloscStanowPrzetworzonych;
		//	plik.get();
		//	plik >> data.maxGlebokoscrekursji;
		//	plik.get();
		//	plik >> data.czas;
		//	plik.get();
		//	dane.push_back(data);
		//}

		//plik.close();
	}
	
	else std::cout << "plik nie zostal otwarty" << std::endl;
	
}

void Glowna::wypisz()
{
	for (auto x : dane)
		std::cout << x << std::endl;
}

void Glowna::test()
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
	else cout << "blad otwarcia pliku";

}


std::ostream& operator<<(std::ostream& os, const Dana& puz)
{
	os << puz.glebokosc << " "
		<< puz.numerUkladanki << " "
		<< puz.algorytm << " "
		<< puz.kolenosc << " "
		<< puz.dlugoscRozwiazania << " "
		<< puz.iloscStanowOdwiedzonych << " "
		<< puz.iloscStanowPrzetworzonych << " "
		<< puz.maxGlebokoscrekursji << " "
		<< puz.czas ;

	return os;
}
std::istream& operator>>(std::istream& os,  Dana& puz)
{
	os >> puz.glebokosc >> 
		 puz.numerUkladanki >>
		 puz.algorytm >>
		 puz.kolenosc >>
		 puz.dlugoscRozwiazania >>
		 puz.iloscStanowOdwiedzonych >>
		 puz.iloscStanowPrzetworzonych >>
		 puz.maxGlebokoscrekursji >>
		 puz.czas  ;

	return os;
}