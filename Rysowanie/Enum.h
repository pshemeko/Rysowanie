#pragma once

#include <string>

struct Strategie 
{
    enum Strategy
    {
        all = 0,
        bfs = 5,
        dfs,
        astr
    };
    Strategy strategy;


    enum Porzadki
    {
        RDUL = 0,
        RDLU,
        DRUL,
        DRLU,
        LUDR,
        LURD,
        ULDR,
        ULRD
    };

    static std::string toString(const Strategy& str)
    {
        switch (str)
        {
        case bfs: return "bfs"; break;
        case dfs: return "dfs"; break;
        case astr : return "astr"; break;
        case all: return "Wszystkie"; break;
        default: return "Nieznane"; break;
        }
    }

    static std::string toString(const Porzadki& str)
    {
        switch (str)
        {
        case RDUL: return "RDUL"; break;
        case RDLU: return "RDLU"; break;
        case DRUL: return "DRUL"; break;
        case DRLU: return "DRLU"; break;
        case LUDR: return "LUDR"; break;
        case LURD: return "LURD"; break;
        case ULDR: return "ULDR"; break;
        case ULRD: return "ULRD"; break;

        default: return "Nieznane"; break;
        }
    }
};
