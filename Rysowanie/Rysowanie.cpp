// Rysowanie.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include "Dane.h"


int main(int argc, char** argv)
{
	Glowna jedna(argv[1]);

	//jedna.open();
	jedna.wczytaj(); // tu jest open i close
	std::cout << "jeden\n";
	//jedna.test();

    //jedna.wypisz(Strategie::Strategy::bfs);
	std::cout << "dwa\n";
    //jedna.rysuj_Ilosc_odwiedzonych_stanow();
    //jedna.licz_Ilosc_przetworzonych_stanow(Strategie::Strategy::astr);
    jedna.oblicz_wszystko();
	std::cout << "trzy\n";

    std::cout << "Hello World!\n"; 
	//jedna.close();
	system("pause");
}

// Uruchomienie programu: Ctrl + F5 lub menu Debugowanie > Uruchom bez debugowania
// Debugowanie programu: F5 lub menu Debugowanie > Rozpocznij debugowanie

// Porady dotyczące rozpoczynania pracy:
//   1. Użyj okna Eksploratora rozwiązań, aby dodać pliki i zarządzać nimi
//   2. Użyj okna programu Team Explorer, aby nawiązać połączenie z kontrolą źródła
//   3. Użyj okna Dane wyjściowe, aby sprawdzić dane wyjściowe kompilacji i inne komunikaty
//   4. Użyj okna Lista błędów, aby zobaczyć błędy
//   5. Wybierz pozycję Projekt > Dodaj nowy element, aby utworzyć nowe pliki kodu, lub wybierz pozycję Projekt > Dodaj istniejący element, aby dodać istniejące pliku kodu do projektu
//   6. Aby w przyszłości ponownie otworzyć ten projekt, przejdź do pozycji Plik > Otwórz > Projekt i wybierz plik sln
