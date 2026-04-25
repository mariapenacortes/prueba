#include "Jugador.h"

Jugador::Jugador() : dorsal(0), partidosJugados(0), goles(0), minutosJugados(0), asistencias(0), amarillas(0), rojas(0), faltas(0) {}

Jugador::Jugador(std::string n, std::string a, int d) : nombre(n), apellido(a), dorsal(d), partidosJugados(0), goles(0), minutosJugados(0), asistencias(0), amarillas(0), rojas(0), faltas(0) {}

void Jugador::actualizarEstadisticas(int g, int m, int as, int am, int r, int f) {
    goles += g;
    minutosJugados += m;
    asistencias += as;
    amarillas += am;
    rojas += r;
    faltas += f;
    partidosJugados++;
}

int Jugador::getGoles() const {
    return goles;
}

void Jugador::setGoles(int g) {
    goles = g;
}

std::string Jugador::getNombreCompleto() const {
    return nombre + " " + apellido;
}

int Jugador::getDorsal() const {
    return dorsal;
}