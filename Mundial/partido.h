#ifndef PARTIDO_H
#define PARTIDO_H
#include "Equipo.h"
#include "Lista.h"
#include <string>

class Partido {
private:
    std::string fecha, sede, arb1, arb2, arb3;
    Equipo* eq1;
    Equipo* eq2;
    int goles1, goles2;
    double posesion1, posesion2;
    Lista<Jugador*> convocados1;
    Lista<Jugador*> convocados2;
    bool esEmpate;
    double calcularLambda(Equipo* a, Equipo* b);

public:
    Partido();
    Partido(std::string f, std::string s, Equipo* e1, Equipo* e2);
    void simular(bool faseEliminatoria);
    void imprimirResultado() const;
    Equipo* getGanador();
};
#endif