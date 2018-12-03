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
        case RDUL: return "rdul"; break;
        case RDLU: return "rdlu"; break;
        case DRUL: return "drul"; break;
        case DRLU: return "drlu"; break;
        case LUDR: return "ludr"; break;
        case LURD: return "lurd"; break;
        case ULDR: return "uldr"; break;
        case ULRD: return "ulrd"; break;

        default: return "Nieznane"; break;
        }
    }
};
