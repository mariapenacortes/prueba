#include "equipo.h"
#include "globales.h"

Equipo::Equipo() : ranking(0), gf(0), gc(0), pg(0), pe(0), pp(0), amarillas(0), rojas(0), faltas(0) {}

Equipo::Equipo(std::string p, std::string c, std::string t, int r, int gf, int gc, int pg, int pe, int pp)
    : pais(p), confederacion(c), tecnico(t), ranking(r), gf(gf), gc(gc), pg(pg), pe(pe), pp(pp), amarillas(0), rojas(0), faltas(0) {}

void Equipo::generarPlantilla() {
    for (int i = 1; i <= 26; i++) {
        iteracionesGlobales++;
        std::string n = "nombre" + std::to_string(i);
        std::string a = "apellido" + std::to_string(i);
        plantilla.agregar(Jugador(n, a, i));
    }
}

void Equipo::repartirGolesHistoricos() {
    if (gf <= 0) return; // Evita ciclos infinitos si un equipo tiene 0 goles
    int golesRestantes = gf;
    int idx = 0;
    while (golesRestantes > 0) {
        iteracionesGlobales++;
        int gActual = plantilla[idx].getGoles();
        plantilla[idx].setGoles(gActual + 1);
        golesRestantes--;
        idx = (idx + 1) % plantilla.getTamanio();
    }
}

std::string Equipo::getPais() const { return pais; }
std::string Equipo::getConfederacion() const { return confederacion; }
int Equipo::getRanking() const { return ranking; }
int Equipo::getGF() const { return gf; }
int Equipo::getGC() const { return gc; }

// Implementacion de las funciones de partidos
int Equipo::getPG() const { return pg; }
int Equipo::getPE() const { return pe; }
int Equipo::getPP() const { return pp; }

Lista<Jugador>& Equipo::getPlantilla() { return plantilla; }

void Equipo::actualizarHistorico(int gFavor, int gContra, bool gan, bool emp, bool per) {
    gf += gFavor;
    gc += gContra;
    if (gan) pg++;
    if (emp) pe++;
    if (per) pp++;
}

bool Equipo::operator==(const Equipo& otro) const {
    return this->pais == otro.pais;
}