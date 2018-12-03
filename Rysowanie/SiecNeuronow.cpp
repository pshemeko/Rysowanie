//
// Created by Przemo on 2018-05-16.
//

#include "../headers/SiecNeuronow.h"
#include "../headers/Matematyka.h"

SiecNeuronow::SiecNeuronow(vector<int> &wymiar, int iloscCentrow, Zestaw zestawik, double potencjalMinimalny)
    :zestaw(zestawik)
{
    for(int i = 0; i < iloscCentrow; ++i)
    {
        neuronyCentalne.push_back(new Neuron(wymiar));
    }
    Dane::wczytaj_wzorzec(zadanePunkty, zestawik.nazwa);
    odleglosci.clear();
    czyPosortowaneOdleglosci = false;
    potencjalMinimum = potencjalMinimalny;
}

string SiecNeuronow::wypiszZadanePunkty()
{
    ostringstream ss;
    for(int i = 0; i < zadanePunkty.size(); i++)
    {
        ss<< "Punkt[" << i <<"]: ";
        for(int j = 0; j < zadanePunkty[i]->wagi.size(); ++j)
        {
            ss << zadanePunkty[i]->wagi[j] << " ";
        }
        ss << endl;
    }
    return ss.str();
}

string SiecNeuronow::wypiszNeurony()
{
    ostringstream ss;
    for(int i = 0; i < neuronyCentalne.size(); i++)
    {
        for(int j = 0; j < neuronyCentalne[i]->wagi.size(); ++j)
        {
            ss << neuronyCentalne[i]->wagi[j] << " ";
        }
        ss << endl;
    }
    return ss.str();
}

string SiecNeuronow::wypiszOdleglosci()
{
    ostringstream ss;
    for(int i = 0; i < odleglosci.size(); i++)
    {
        ss<<"Punkt " << i <<": ";
        for(int j = 0; j < odleglosci[i].first.size(); ++j)
        {
            ss <<" centr " << odleglosci[i].first[j] <<" odl: " <<odleglosci[i].second[j] ;
        }
        ss << endl;
    }
    return ss.str();
}


void SiecNeuronow::obliczOdleglosci(VEKTORDANYCH &wektor)
{
    odleglosci.clear();
    czyPosortowaneOdleglosci = false;
    //odleglosci.push_back(make_pair(vector<double>(),vector<double>()));
    for(int i = 0 ; i < wektor.size(); ++i)
    {
        vector<double> punkt;
        vector<double> odlegly;
        for(int j = 0; j < neuronyCentalne.size(); ++j)
        {
            double odl = neuronyCentalne[j]->odlegloscEuklidesowa(wektor[i]);
            punkt.push_back(j);
            odlegly.push_back(odl);
        }
        odleglosci.push_back(make_pair(punkt,odlegly));
    }
}

void SiecNeuronow::sortujOdleglosci()   // po 0 sa najmniejsze odleglosci
{

    for(int i = 0; i < odleglosci.size(); ++i)
    {
        for(int j = 1; j < odleglosci[i].first.size(); ++j)
        {
            if( odleglosci[i].second[0] > odleglosci[i].second[j])
            {
                // zamieniamy
                swap(odleglosci[i].first[0], odleglosci[i].first[j]);
                swap(odleglosci[i].second[0], odleglosci[i].second[j]);

            }
        }
    }
    czyPosortowaneOdleglosci = true;
}
void SiecNeuronow::sortujOdleglosciDokladnie()
{
    for(int i = 0; i < odleglosci.size(); ++i)
    {
        for(int j = 0; j < odleglosci[i].first.size() - 1; ++j)
        {
            for(int k = j+1; k < odleglosci[i].first.size(); ++k)
            {
                if( odleglosci[i].second[j] > odleglosci[i].second[k])
                {
                    // zamieniamy
                    swap(odleglosci[i].first[j], odleglosci[i].first[k]);
                    swap(odleglosci[i].second[j], odleglosci[i].second[k]);

                }
            }

        }
    }
    czyPosortowaneOdleglosci = true;
}

double SiecNeuronow::obliczBladKwantyzacji() // TODO zrobic na wykladzie jest i do wykresow dac
{
    // musi byc posortowane
    if(!czyPosortowaneOdleglosci) sortujOdleglosci();

    double blad = 0.0;

    // obliczam z wzoru z wykladow na E
    for(int i = 0; i < odleglosci.size(); ++i )
    {
        //double odl = neuronyCentalne[odleglosci[i].first[0]]->odlegloscEuklidesowa(zadanePunkty[i]); // mozna prosciejponizej
        double odl = odleglosci[i].second[0];// bo second to odleglosc
        odl *=odl; //bo kwadrat we wzorze
        blad += odl;
    }
    blad /= odleglosci.size();
    return blad;
}

void SiecNeuronow::obliczpotencjaly()
{
    int ilecentorw = neuronyCentalne.size();
    for(int i = 0; i < odleglosci.size(); ++i)
    {
        for(int j = 0; j < odleglosci[i].first.size(); ++j)
        {
            if(0 == j) // gdy zwyciezyl odejmnij PMIN
            {
                neuronyCentalne[odleglosci[i].first[j]]->potencjal -= potencjalMinimum;
            }
            else neuronyCentalne[odleglosci[i].first[j]]->potencjal += 1.0 / ilecentorw;
        }
    }
}

void SiecNeuronow::adapptacjaWagWersjaOFFLine(bool czyUwzgledniacPotencjal)
{
    //obliczOdleglosci();
    //sortujOdleglosci();
    int ile = 0;
    vector<double> wspolrzedneNowe = neuronyCentalne[0]->wagi;  // aby mial ten sam rozmiar

    for(int i = 0; i < wspolrzedneNowe.size(); ++i) wspolrzedneNowe[i] = 0.0; // zerowanie

    if(czyUwzgledniacPotencjal)
    {
        obliczpotencjaly();
        sortujOdleglosciDokladnie(); // sortowanie pełne wtedy tzreba zrobic

        for(int i = 0; i < neuronyCentalne.size(); ++i)
        {
            if(neuronyCentalne[i]->potencjal > potencjalMinimum)    // gdy potencjal < PMIN neuron odpoczywa
            {
                for(int j = 0; j < odleglosci.size(); ++j)  // jedziemy wszystkie punkty pokolei
                {
                    int k = 0;
                    while(neuronyCentalne[odleglosci[j].first[k]]->potencjal < potencjalMinimum)
                    {
                        ++k;
                    }

                    if(odleglosci[j].first[k] == i)
                    {
                        for(int l = 0; l < wspolrzedneNowe.size(); ++l) // sumuje kazda wspolrzedna
                        {
                            wspolrzedneNowe[l] +=zadanePunkty[j]->wagi[l];
                        }
                        ile++;
                    }

                }
                if(ile !=0)
                {
                    for(int k = 0; k < wspolrzedneNowe.size(); ++k)
                    {
                        wspolrzedneNowe[k] = wspolrzedneNowe[k] / ile;
                        neuronyCentalne[i]->wagi[k] = wspolrzedneNowe[k];
                    }
                }
                ile = 0;
                for(int i = 0; i < wspolrzedneNowe.size(); ++i) wspolrzedneNowe[i] = 0.0; // zerowanie
            }
        }

    }
    else {  // gdy bez potencjalu jest jak bylo
     // alggorytm adaptacji parametrow funkcji radialnych  procesie samoorganizacji str. 28.
        for(int i = 0; i < neuronyCentalne.size(); ++i)
        {
            for(int j = 0; j < odleglosci.size(); ++j)
            {
                if(odleglosci[j].first[0] == i)
                {
                    for(int l = 0; l < wspolrzedneNowe.size(); ++l) // sumuje kazda wspolrzedna
                    {
                        wspolrzedneNowe[l] +=zadanePunkty[j]->wagi[l];
                    }
                    ile++;
                }
            }
            if(ile !=0)
            {
                for(int k = 0; k < wspolrzedneNowe.size(); ++k)
                {
                    wspolrzedneNowe[k] = wspolrzedneNowe[k] / ile;
                    neuronyCentalne[i]->wagi[k] = wspolrzedneNowe[k];
                }
            }
            ile = 0;
            for(int i = 0; i < wspolrzedneNowe.size(); ++i) wspolrzedneNowe[i] = 0.0; // zerowanie
        }
    }
}
/*
void SiecNeuronow::adaptacjaWagGazNeuronowy(bool czyUwzgledniacPotencjal, double &lambda, double eta, int K_iluSasiadomZmieniamy)//vector<double> wspolczynnikiNaukiSasiadow)
{
    ///////////////////////////// nieskonczone
    //Dane dane;
    //vector<int> kolejnosc = dane.wylosujKolejnoscPobierania(zadanePunkty.size());
    odleglosci.clear();
    int iluSasiadow = K_iluSasiadomZmieniamy;
    if(iluSasiadow > neuronyCentalne.size())
    {
        cout <<endl<<endl<< "!!!!!!!!!!!!!!!!!! BLAD NIE MOZNA ADAPTOWAC WIECEJ SASIADOW NIZ JEST CENTROW" <<endl<<endl;
        exit(99999);
    }
    //obliczOdleglosci();
    //sortujOdleglosciDokladnie();
    int ile = 0;
    //vector<double> wspolrzedneNowe = neuronyCentalne[0]->wagi;  // aby mial ten sam rozmiar
    //for(int i = 0; i < wspolrzedneNowe.size(); ++i) wspolrzedneNowe[i] = 0.0; // zerowanie

    //vector<double> ileRazyZmienionoCentum;
   //for (int i = 0; i < neuronyCentalne.size(); ++i) ileRazyZmienionoCentum.push_back(0.0); // zapisuje i;le razy modyfikowalem wagi danego centum

    if(czyUwzgledniacPotencjal)
    {

    }
    else
    {

            ///////////// wybierz kolejny punkt znajdz najblizsze mu centa i zmien ich wagi
        for(int i = 0; i < zadanePunkty.size(); ++i)
            {
                //VEKTORPAR para;
                vector<double> punkt;
                vector<double> odlegly;
                // zwyliczam odleglosci wszystkich neuronow od tego punktu
                for(int j = 0; j <neuronyCentalne.size(); ++j)
                {
                    double odl = neuronyCentalne[j]->odlegloscEuklidesowa(zadanePunkty[i]);
                    //double odl = zadanePunkty[i]->odlegloscEuklidesowa(neuronyCentalne[j]);

                    punkt.push_back(j);
                    odlegly.push_back(odl);
                }
                odleglosci.push_back(make_pair(punkt,odlegly));

                // teraz sortuje odleglosci od najblizszego
                for(int j = 0; j < odleglosci[i].first.size() - 1; ++j)
                {
                    for(int k = j+1; k < odleglosci[i].first.size(); ++k)
                    {
                        if( odleglosci[i].second[j] > odleglosci[i].second[k])
                        {
                            // zamieniamy
                            swap(odleglosci[i].first[j], odleglosci[i].first[k]);
                            swap(odleglosci[i].second[j], odleglosci[i].second[k]);

                        }
                    }
                }

             // modyfikuje wagi wszystkich neurony od najblizszego do zadanego punktu az do K-tego(z main) czyli iluSasiadow

             for(int j = 0; j < iluSasiadow; ++j)    //TODO zmienic tak jak na wykladzie ze j< neuronyCentalne.size() bo zmieniac trzeba dla wszystkich tylko te sasiedstwo bedzie coraz mniejsza wartosc
             {
                 ostringstream os;
                 // tu njpierw oblicz potencjal gdy wybrano
                 for(int k = 0; k< neuronyCentalne[0]->wagi.size(); ++k)
                 {
                     os << "Waga:" <<neuronyCentalne[odleglosci[i].first[j]]-> wagi[k];
                     // wzor z wykladu strona 39 oraz 42
                     // TODO Wzur na sume jest zly
                    neuronyCentalne[odleglosci[i].first[j]]-> wagi[k]
                            += eta * funkcjaSasiedztwaGazuNeuronowego(j,lambda)
                               * (zadanePunkty[i]->wagi[k] - neuronyCentalne[odleglosci[i].first[j]]-> wagi[k]);

                    os << " wsp. Nauki: " << eta << " Fcja sasiedstawa: " ;
                    os << funkcjaSasiedztwaGazuNeuronowego(j,lambda) << " roznica wag" << zadanePunkty[i]->wagi[k] - neuronyCentalne[odleglosci[i].first[j]]-> wagi[k];
                    os <<" Nowa waga: " <<neuronyCentalne[odleglosci[i].first[j]]-> wagi[k];

                 }
              //  if (0 == i) cout << os.str() << endl;

             }

            }

        }
    czyPosortowaneOdleglosci = true;
}
*/
void SiecNeuronow::aptacjaWagGazNeuronowyOFFLine(bool czyUwzgledniacPotencjal, double &lambda, double eta, int iluSasiadomZmieniamy)
{
    vector<double> iloscZmianNeuronu;
    for(int i = 0; i < neuronyCentalne.size(); ++i) iloscZmianNeuronu.push_back(0.0);   // zeruje
    if(iluSasiadomZmieniamy > neuronyCentalne.size())
    {
        cout <<endl<<endl<< "!!!!!!!!!!!!!!!!!! BLAD NIE MOZNA ADAPTOWAC WIECEJ SASIADOW NIZ JEST CENTROW" <<endl<<endl;
        exit(99999);
    }
    if(czyUwzgledniacPotencjal)
    {
        obliczpotencjaly();
        for(int i = 0; i < odleglosci.size(); ++i)
        {
            int numer = 0; // zminiejszymy gdy neuron pauzuje aby to nastepny byl traktowany jako pierwszy
            for(int j = 0; j < iluSasiadomZmieniamy; ++j) // for(int j = 0; j < odleglosci[i].first.size(); ++j)
            {
                if(neuronyCentalne[odleglosci[i].first[j]]->potencjal > potencjalMinimum)
                {
                    iloscZmianNeuronu[odleglosci[i].first[j]] += 1.0;
                    for(int k = 0; k < neuronyCentalne[0]->wagi.size(); ++k)
                    {
                        neuronyCentalne[odleglosci[i].first[j]]-> wagi[k] +=
                                eta * funkcjaSasiedztwaGazuNeuronowego(numer,lambda)
                                *(zadanePunkty[i] -> wagi[k] - neuronyCentalne[odleglosci[i].first[j]]-> wagi[k]);
                    }
                    neuronyCentalne[odleglosci[i].first[j]]-> iloscZmian++;
                    numer++;
                }
            }
        }
    }
    else
    {  // gdy bez potencjalu jest jak bylo
        // alggorytm adaptacji parametrow funkcji radialnych  procesie samoorganizacji str. 28.


        for(int i = 0; i < odleglosci.size(); ++i)
        {
            for(int j = 0; j < iluSasiadomZmieniamy; ++j) // for(int j = 0; j < odleglosci[i].first.size(); ++j)
            {
                iloscZmianNeuronu[odleglosci[i].first[j]] += 1.0;
                for(int k = 0; k < neuronyCentalne[0]->wagi.size(); ++k)
                {
                    neuronyCentalne[odleglosci[i].first[j]]-> wagi[k] +=
                    eta * funkcjaSasiedztwaGazuNeuronowego(j,lambda)
                            *(zadanePunkty[i] -> wagi[k] - neuronyCentalne[odleglosci[i].first[j]]-> wagi[k]);
                }

            }


        }
        /*
        // jako ze offline dzielimy przes zume zmian chyba
        for(int i = 0; i < iloscZmianNeuronu.size(); ++i)
        {
            for(int k = 0; k < neuronyCentalne[i]->wagi.size(); ++k)
            {
                if(iloscZmianNeuronu[i] != 0.0) neuronyCentalne[i]->wagi[k] /= iloscZmianNeuronu[i];
            }
        }
         */

    }
}

void SiecNeuronow::przesunMartweNeurony(double lambda, double eta)
{
    for(int i = 0; i < neuronyCentalne.size(); ++i)
    {
        if(neuronyCentalne[i]->iloscZmian < 5)
        {
            // to szukam najbliższego zadanwego punktu i przesuwam neuron

            int numerPunktu = 0;
            double odleglosc =  neuronyCentalne[i] ->odlegloscEuklidesowa(zadanePunkty[0]);
            for(int j = 1; j < zadanePunkty.size(); ++j)
            {
                int wartosc = neuronyCentalne[i] ->odlegloscEuklidesowa(zadanePunkty[j]);
                if (wartosc < odleglosc)
                {
                    odleglosc = wartosc;
                    numerPunktu = j;
                }
            }
            // przesuwamy neuron blizej punktu

            for (int k = 0; k < neuronyCentalne[0]->wagi.size(); ++k)
            {
                neuronyCentalne[i]-> wagi[k] +=
                        eta * funkcjaSasiedztwaGazuNeuronowego(0,lambda)
                        *(zadanePunkty[numerPunktu] -> wagi[k] - neuronyCentalne[i]-> wagi[k]);
            }

        }

    }

}


void SiecNeuronow::zapiszDoPliku(VEKTORDANYCH &dana, string nazwaPliku)
{
    ofstream zapis(nazwaPliku);//obiekt tworzymy tylko raz

    for(int i=0; i<dana.size(); i++){
        for(int j=0;j<dana[i]->wagi.size();j++){
            zapis<<dana[i]->wagi[j]<<" ";
        }
        zapis<<endl;
    }

    zapis.close();
}

string SiecNeuronow::zapiszWszystkoWPliku(int iloscCentrow,string nazwaPlikuCentrow, VEKTORDANYCH &dane)
{
    string zwracany; // to co moge wyswietlic w main

    zapiszDoPliku(neuronyCentalne, nazwaPlikuCentrow);   // zapisuje neurony centralne do pliku by moc rysowac je

    // musi byc posortowane
    //if(!czyPosortowaneOdleglosci) sortujOdleglosci();

    string nazwa;

    //int l=0;
    //int a=1;
    VEKTORDANYCH p1;

    for(int i=0;i<iloscCentrow;i++)
    {
        sprintf((char*)nazwa.c_str(), "%d", i);
        string nazwa1 = nazwa.c_str();
        nazwa1=nazwa1+".txt";
                //char *nazwa3 = new char[nazwa1.length() + 1];
                //strcpy(nazwa3, nazwa1.c_str());
        zwracany += nazwa1 +" ";    // zeby wyswietlic w main

        for(int j=0;j<odleglosci.size();j++)
        {
            if(odleglosci[j].first[0] == i)
            {
                p1.push_back(new Neuron(dane[j]->wagi[0], dane[j]->wagi[1]));
                //p1.push_back(vector<double>());
                //p1[l].push_back(wzorzec[j][0]);
                //p1[l].push_back(wzorzec[j][1]);
                //l++;

            }
            // cout<<nazwa<<" ";
        }
        zapiszDoPliku(p1,nazwa1);
        p1.clear();
        //l=0;
    }
    p1.clear();

    return zwracany;
}


string SiecNeuronow::rysujWykres( int iloscCentrow, int numer, VEKTORDANYCH &dane, string komendaJest)
{
    //char *plikCentrow = "neuronyCentralne.txt";
    string nazwaPlikuCentrow = "neuronyCentralne.txt";
    string zwracany;

    zwracany = zapiszWszystkoWPliku(iloscCentrow, nazwaPlikuCentrow, dane);

    string nazwa1;
    string s0;
    if(komendaJest != "a") s0 += komendaJest;

    s0+="plot ";
    string s1=" w points ps 0.5 pt 7 ";
    ostringstream ss;
    string nn;

    for(int i = 0 ; i < iloscCentrow; ++i)  // tworze pliki oraz komnde do plota
    {
        sprintf((char*)nazwa1.c_str(), "%d", i);
        string nazwa2 = nazwa1.c_str();
        nazwa1=nazwa2+".txt";
        s0=s0+"'"+nazwa1+"'"+s1+", ";
    }

    s0=s0+ "'"+nazwaPlikuCentrow + "' w points ps 2 pt 1 lc rgb 'red' \n";    //TODO zmien i dodaj to zakomentowane na koncu jak cos

    ss << numer;
    nn = ss.str();

    zwracany = "\n" + s0 + "\n";
 cout << endl << s0 <<endl;

     rysuj1(s0,nn);

    //rysuj(nazwa2);
    //getchar() ;
return zwracany;
}

void SiecNeuronow::tworzMozaike()
{
/*
    for(double i = zestaw.xmin; i < zestaw.xmax; i = i+0.1)
    {
        for(double j = zestaw.ymin; j < zestaw.ymax; j = j+0.1)
        {
            mozaika.push_back(new Neuron(i,j));
        }
    }
*/
    //poniewaz zamiast 0 pokazuje -1.87905e-14 musze to rozbic na czeci

    for(double i = zestaw.xmin; i <= 0.1; i = i+0.1)
    {
        for(double j = zestaw.ymin; j <= 0.1; j = j+0.1)
        {
            mozaika.push_back(new Neuron(i,j));
        }
        for(double j = 0.0; j < zestaw.ymax; j = j+0.1)
        {
            mozaika.push_back(new Neuron(i,j));
        }
    }

    for(double i = 0.0; i < zestaw.xmax; i = i+0.1)
    {
        for(double j = zestaw.ymin; j <= 0.1; j = j+0.1)
        {
            mozaika.push_back(new Neuron(i,j));
        }
        for(double j = 0.0; j < zestaw.ymax; j = j+0.1)
        {
            mozaika.push_back(new Neuron(i,j));
        }
    }

}



void SiecNeuronow::zapiszCentra()    // potrzebne do testowania ustawien by byly takie same wagi poczatkowe
{
    ofstream foutCenta("Wagi.txt");
    for(int i = 0; i < neuronyCentalne.size(); ++i)
    {
        for(int j = 0; j<neuronyCentalne[i]->wagi.size();++j)
        {
            foutCenta << neuronyCentalne[i]->wagi[j] << " ";
        }
        foutCenta << endl;
    }

}

void SiecNeuronow::wczytajCentra()
{
    string nazwa = "Wagi.txt";
    fstream dane;
    dane.open(nazwa, ios::in | ios::out) ;
    double liczba;
    int i=0;
    int j=0;
    //wzorzec.push_back(vector<double>());
    vector<double> vv;
    if(dane.is_open()){
        neuronyCentalne.clear();
        cout<<"plik otwartyyyyyy"<<endl;
        while(!dane.eof() ){
            dane>>liczba;
            vv.push_back(liczba);
            j=vv.size();
            // cout<<wzorzec[i][j-1]<<"d";
            if(j==2){
                i++;
                Neuron *nn = new Neuron(vv[0],vv[1]);
                neuronyCentalne.push_back(nn);

                vv.clear();
                //wzorzec.push_back(vector<double>());
                // cout<<endl;
            }
        }
        dane.close();
        //wzorzec.pop_back();
    }
    else cout<<"plik nie zostal otwarty"<<endl;
}

void SiecNeuronow::zapiszCetraZPotencjalem(int numer)
{
    string nazwa;
    sprintf((char*)nazwa.c_str(), "%d", numer);
    string nazwa1 = "aaa";
    nazwa1 += nazwa.c_str();
    nazwa1=nazwa1+".txt";

    ofstream foutCenta(nazwa1);
    for(int i = 0; i < neuronyCentalne.size(); ++i)
    {
        for(int j = 0; j<neuronyCentalne[i]->wagi.size();++j)
        {
            foutCenta << neuronyCentalne[i]->wagi[j] << " ";
        }

        foutCenta << " : " << neuronyCentalne[i]->potencjal <<   endl;
    }

}

void SiecNeuronow::zapiszCentraZIlosciaZmian(int numer)
{
    string nazwa;
    sprintf((char*)nazwa.c_str(), "%d", numer);
    string nazwa1 = "bbb";
    nazwa1 += nazwa.c_str();
    nazwa1=nazwa1+".txt";

    ofstream foutCenta(nazwa1);
    for(int i = 0; i < neuronyCentalne.size(); ++i)
    {
        for(int j = 0; j<neuronyCentalne[i]->wagi.size();++j)
        {
            foutCenta << neuronyCentalne[i]->wagi[j] << " ";
        }

        foutCenta << " : " << neuronyCentalne[i]->iloscZmian <<   endl;
    }

}