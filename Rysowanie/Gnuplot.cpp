//
// Created by Przemo on 2018-06-01.
//

#include "pch.h"
#include "Gnuplot.h"

Gnuplot::Gnuplot() {
    pipe = _popen("gnuplot -persist", "w");
    if(!pipe) {
        cerr << "nie znalazlo Gnuplota";
    }
}

Gnuplot::~Gnuplot() {
    fprintf(pipe, "exit\n");
    _pclose(pipe);
}

void Gnuplot::operator() (const string &command) {
    fprintf(pipe, "%s", command.c_str());
    fflush(pipe);
}
