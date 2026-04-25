#ifndef EQUIPO_H
#define EQUIPO_H
#include <string>
#include "jugador.h"
#include "Lista.h"

class Equipo {
private:
    std::string pais, confederacion, tecnico;
    int ranking;
    int gf, gc, pg, pe, pp, amarillas, rojas, faltas;
    Lista<Jugador> plantilla;

public:
    Equipo();
    Equipo(std::string p, std::string c, std::string t, int r, int gf, int gc, int pg, int pe, int pp);
    void generarPlantilla();
    void repartirGolesHistoricos();
    std::string getPais() const;
    std::string getConfederacion() const;
    int getRanking() const;
    int getGF() const;
    int getGC() const;

    // Funciones agregadas para obtener los partidos
    int getPG() const;
    int getPE() const;
    int getPP() const;

    Lista<Jugador>& getPlantilla();
    void actualizarHistorico(int gFavor, int gContra, bool gan, bool emp, bool per);
    bool operator==(const Equipo& otro) const;
};
#endif