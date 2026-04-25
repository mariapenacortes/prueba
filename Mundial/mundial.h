#ifndef MUNDIAL_H
#define MUNDIAL_H
#include "Lista.h"
#include "equipo.h"
#include "partido.h"
#include <string>

class Mundial {
private:
    Lista<Equipo> equipos;
    Lista<Equipo*> grupos[12];
    Lista<Partido> partidosGrupos;

    // Listas para las fases eliminatorias
    Lista<Equipo*> clasificadosR32;
    Lista<Equipo*> clasificadosR16;
    Lista<Equipo*> clasificadosQF;
    Lista<Equipo*> clasificadosSF;
    Equipo* campeon;
    Equipo* subcampeon;
    Equipo* tercerLugar;
    Equipo* cuartoLugar;

    // Funciones auxiliares
    void ordenarGrupo(int indiceGrupo);
    void seleccionarMejoresTerceros(Lista<Equipo*>& terceros);

public:
    Mundial();
    void cargarDatos(std::string rutaCSV);
    void conformarGrupos();
    void simularFaseGrupos();
    void simularFasesEliminatorias(); // <--- NUEVA FUNCION
    void generarEstadisticas();
    void mostrarRecursos() const;
};
#endif