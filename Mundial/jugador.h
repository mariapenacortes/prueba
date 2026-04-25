#ifndef JUGADOR_H
#define JUGADOR_H
#include <string>

class Jugador {
private:
    std::string nombre;
    std::string apellido;
    int dorsal;
    int partidosJugados, goles, minutosJugados, asistencias, amarillas, rojas, faltas;

public:
    Jugador();
    Jugador(std::string n, std::string a, int d);
    void actualizarEstadisticas(int g, int m, int as, int am, int r, int f);
    int getGoles() const;
    void setGoles(int g);
    std::string getNombreCompleto() const;
    int getDorsal() const;
};
#endif