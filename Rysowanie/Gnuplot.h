//
// Created by Przemo on 2018-06-01.
//

#ifndef KWIATKI_GNUPLOT_H
#define KWIATKI_GNUPLOT_H


#include <iostream>
#include <string>
#include <fstream>

using std::cerr;
using std::string;

class Gnuplot
{
private:
    FILE *pipe;
public:
    Gnuplot();
    ~Gnuplot();
    void operator () (const string &command);


};


static void rysuj(string str)
{
    Gnuplot gnuplot;

    //string a = "0.0";
    //string b = "1.0";

    gnuplot("set term png truecolor\n"); //ustawienie terminala na png
    //gnuplot("set output \"plikStaryZLY.png\" \n");  //ustawienie pliku do którego maj¹ zostaæ zapisane dane
    gnuplot("set output \""+str+".png\" \n");  //ustawienie pliku do którego maj¹ zostaæ zapisane dane
    gnuplot("set xlabel 'Epoka'\n"); //ustawienie opisu osi x
    gnuplot("set ylabel 'Wartosc bledu kwantyzacji'\n"); // ustawienie osi y
    //gnuplot("set boxwidth 0.5 relative\n");
    //gnuplot("set style fill transparent solid 0.5 noborder\n");
   // gnuplot("set xtics rotate out\n");
    gnuplot("set key title \"Legenda\" box\n");
    //gnuplot("plot 'plikStaryZLY.txt' with points ps 0.3\n");
    gnuplot("plot '"+str+".txt'"+" w points pt 7 ps 1\n");//                         " with points ps 0.3\n"); //pt 5 - kwadraty, pt 7 - kropki powiny byc ale trpche nie do konca dobrze
    //gnuplot("plot 'attract.txt' using 2:xtic(1) ti \"");
    //gnuplot(a);
    //gnuplot("\" w boxes lc rgb'red', 'attract_small.txt' using 2:xtic(1) ti \"");
    //gnuplot(b);
    //gnuplot("\" w boxes lc rgb'green'\n");
    //gnuplot("exit\n");

}

static void rysuj1(string komenda, string nazwaPlikuPNG)
{
    Gnuplot gnuplot;

    gnuplot("set term png truecolor\n"); //ustawienie terminala na png
    gnuplot("set output \""+nazwaPlikuPNG+".png\" \n");  //ustawienie pliku do którego maja zostac zapisane dane
    gnuplot("set xlabel 'Wartosci'\n"); //ustawienie opisu osi x
    gnuplot("set ylabel 'liczebnosc'\n"); // ustawienie osi y
    gnuplot("set boxwidth 0.5 relative\n");
    gnuplot("set style fill transparent solid 0.5 noborder\n");
    gnuplot("set xtics rotate out\n");
    gnuplot("set key noautotitle;\n");  // nie wyswietla legendy
    //gnuplot("set key outside right top;");
    //gnuplot("set key title \"Legenda\" box\n");
    //gnuplot("set key off"); // nie rusuje legendy

    gnuplot(komenda);
    gnuplot("exit\n");

}

static void rysuj0(string komendaGnuplota)
{
    Gnuplot gnuplot;

    gnuplot(komendaGnuplota);
    gnuplot("exit\n");
}


#endif //KWIATKI_GNUPLOT_H
