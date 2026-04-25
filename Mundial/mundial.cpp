#include "mundial.h"
#include "globales.h"
#include <iostream>
#include <fstream>
#include <sstream>

Mundial::Mundial() : campeon(nullptr), subcampeon(nullptr), tercerLugar(nullptr), cuartoLugar(nullptr) {}

void Mundial::cargarDatos(std::string rutaCSV) {
    std::ifstream archivo(rutaCSV);
    std::string linea;

    if (archivo.is_open()) {
        std::getline(archivo, linea);
        while (std::getline(archivo, linea)) {
            if (linea.empty() || linea.length() < 5) continue;
            if (linea.find("Ranking") != std::string::npos) continue;

            iteracionesGlobales++;
            std::stringstream ss(linea);
            std::string pais, conf, tec, fed, temp;
            int rank = 0, gf = 0, gc = 0, pg = 0, pe = 0, pp = 0;

            try {
                char sep = ';';

                std::getline(ss, temp, sep); rank = std::stoi(temp);
                std::getline(ss, pais, sep);
                std::getline(ss, tec, sep);
                std::getline(ss, fed, sep);
                std::getline(ss, conf, sep);
                std::getline(ss, temp, sep); gf = std::stoi(temp);
                std::getline(ss, temp, sep); gc = std::stoi(temp);
                std::getline(ss, temp, sep); pg = std::stoi(temp);
                std::getline(ss, temp, sep); pe = std::stoi(temp);
                std::getline(ss, temp, sep); pp = std::stoi(temp);

                Equipo e(pais, conf, tec, rank, gf, gc, pg, pe, pp);
                e.generarPlantilla();
                e.repartirGolesHistoricos();
                equipos.agregar(e);
            } catch (const std::exception& e) {
                std::cout << "[ADVERTENCIA] Error al leer la linea: " << linea << "\n";
            }
        }
        archivo.close();
        std::cout << "\n[EXITO] Datos cargados correctamente.\n";
    } else {
        std::cout << "\n[ERROR] No se pudo encontrar o abrir el archivo en la ruta:\n";
        std::cout << rutaCSV << "\n";
    }
}

void Mundial::conformarGrupos() {
    for (int i = 0; i < equipos.getTamanio(); i++) {
        iteracionesGlobales++;
        grupos[i % 12].agregar(&equipos[i]);
    }

    for (int i = 0; i < 12; i++) {
        std::cout << "Grupo " << (char)('A' + i) << ":\n";
        for (int j = 0; j < grupos[i].getTamanio(); j++) {
            std::cout << " - " << grupos[i][j]->getPais() << " (" << grupos[i][j]->getConfederacion() << ")\n";
        }
    }
}

void Mundial::simularFaseGrupos() {
    for (int i = 0; i < 12; i++) {
        for (int j = 0; j < grupos[i].getTamanio(); j++) {
            for (int k = j + 1; k < grupos[i].getTamanio(); k++) {
                iteracionesGlobales++;
                Partido p("20/06/2026", "nombreSede", grupos[i][j], grupos[i][k]);
                p.simular(false);
                p.imprimirResultado();
                partidosGrupos.agregar(p);
            }
        }
    }
}

void Mundial::ordenarGrupo(int indiceGrupo) {
    for (int i = 0; i < grupos[indiceGrupo].getTamanio() - 1; i++) {
        for (int j = 0; j < grupos[indiceGrupo].getTamanio() - i - 1; j++) {
            Equipo* eq1 = grupos[indiceGrupo][j];
            Equipo* eq2 = grupos[indiceGrupo][j + 1];

            int pts1 = eq1->getPG() * 3 + eq1->getPE();
            int pts2 = eq2->getPG() * 3 + eq2->getPE();
            int dg1 = eq1->getGF() - eq1->getGC();
            int dg2 = eq2->getGF() - eq2->getGC();

            bool swap = false;
            if (pts2 > pts1) swap = true;
            else if (pts2 == pts1 && dg2 > dg1) swap = true;
            else if (pts2 == pts1 && dg2 == dg1 && eq2->getGF() > eq1->getGF()) swap = true;

            if (swap) {
                grupos[indiceGrupo][j] = eq2;
                grupos[indiceGrupo][j + 1] = eq1;
            }
            iteracionesGlobales++;
        }
    }
}

void Mundial::seleccionarMejoresTerceros(Lista<Equipo*>& terceros) {
    for (int i = 0; i < terceros.getTamanio() - 1; i++) {
        for (int j = 0; j < terceros.getTamanio() - i - 1; j++) {
            Equipo* eq1 = terceros[j];
            Equipo* eq2 = terceros[j + 1];

            int pts1 = eq1->getPG() * 3 + eq1->getPE();
            int pts2 = eq2->getPG() * 3 + eq2->getPE();
            int dg1 = eq1->getGF() - eq1->getGC();
            int dg2 = eq2->getGF() - eq2->getGC();

            bool swap = false;
            if (pts2 > pts1) swap = true;
            else if (pts2 == pts1 && dg2 > dg1) swap = true;
            else if (pts2 == pts1 && dg2 == dg1 && eq2->getGF() > eq1->getGF()) swap = true;

            if (swap) {
                terceros[j] = eq2;
                terceros[j + 1] = eq1;
            }
            iteracionesGlobales++;
        }
    }
}

void Mundial::simularFasesEliminatorias() {
    std::cout << "\n--- TRANSICION A DIECISEISAVOS ---\n";
    Lista<Equipo*> todosLosTerceros;

    for (int i = 0; i < 12; i++) {
        ordenarGrupo(i);
        clasificadosR32.agregar(grupos[i][0]);
        clasificadosR32.agregar(grupos[i][1]);
        todosLosTerceros.agregar(grupos[i][2]);
    }

    seleccionarMejoresTerceros(todosLosTerceros);
    for(int i = 0; i < 8; i++) {
        clasificadosR32.agregar(todosLosTerceros[i]);
    }

    std::cout << "32 equipos clasificados para Dieciseisavos de final.\n";

    Lista<Equipo*>* faseActual = &clasificadosR32;
    Lista<Equipo*>* siguienteFase = &clasificadosR16;
    std::string nombresFases[] = {"Dieciseisavos (R32)", "Octavos (R16)", "Cuartos (QF)", "Semifinales (SF)"};

    for(int fase = 0; fase < 4; fase++) {
        std::cout << "\n--- Simulando " << nombresFases[fase] << " ---\n";
        for (int i = 0; i < faseActual->getTamanio(); i += 2) {
            Equipo* eq1 = (*faseActual)[i];
            Equipo* eq2 = (*faseActual)[i + 1];

            Partido p("01/07/2026", "nombreSede", eq1, eq2);
            p.simular(true);
            p.imprimirResultado();
            siguienteFase->agregar(p.getGanador());
        }

        if (fase == 0) { faseActual = &clasificadosR16; siguienteFase = &clasificadosQF; }
        else if (fase == 1) { faseActual = &clasificadosQF; siguienteFase = &clasificadosSF; }
        else if (fase == 2) {
            faseActual = &clasificadosSF;
            Lista<Equipo*> perdedoresSemis;
            Lista<Equipo*> ganadoresSemis;
            for(int i = 0; i < 4; i+=2) {
                Partido p("10/07/2026", "nombreSede", (*faseActual)[i], (*faseActual)[i+1]);
                p.simular(true);
                p.imprimirResultado();
                ganadoresSemis.agregar(p.getGanador());
                perdedoresSemis.agregar((p.getGanador() == (*faseActual)[i]) ? (*faseActual)[i+1] : (*faseActual)[i]);
            }

            std::cout << "\n--- Partido por el Tercer Puesto ---\n";
            Partido p3("14/07/2026", "nombreSede", perdedoresSemis[0], perdedoresSemis[1]);
            p3.simular(true);
            p3.imprimirResultado();
            tercerLugar = p3.getGanador();
            cuartoLugar = (tercerLugar == perdedoresSemis[0]) ? perdedoresSemis[1] : perdedoresSemis[0];

            std::cout << "\n--- GRAN FINAL ---\n";
            Partido pF("15/07/2026", "nombreSede", ganadoresSemis[0], ganadoresSemis[1]);
            pF.simular(true);
            pF.imprimirResultado();
            campeon = pF.getGanador();
            subcampeon = (campeon == ganadoresSemis[0]) ? ganadoresSemis[1] : ganadoresSemis[0];

            break;
        }
    }
}

void Mundial::generarEstadisticas() {
    // Validacion: Evitar que saque estadisticas si no hay campeon aun
    if (campeon == nullptr) {
        std::cout << "\n[!] Error: Debes simular el torneo completo (Opcion 4) antes de ver las estadisticas finales.\n";
        return;
    }

    std::cout << "\n==================================================\n";
    std::cout << "        INFORME ESTADISTICO MUNDIAL 2026          \n";
    std::cout << "==================================================\n";

    // 1. Ranking de los 4 primeros puestos
    std::cout << "\n[1] RANKING DE LOS 4 PRIMEROS PUESTOS:\n";
    std::cout << "  1. " << campeon->getPais() << " (CAMPEON)\n";
    std::cout << "  2. " << subcampeon->getPais() << " (Subcampeon)\n";
    std::cout << "  3. " << tercerLugar->getPais() << " (Tercer Puesto)\n";
    std::cout << "  4. " << cuartoLugar->getPais() << " (Cuarto Puesto)\n";

    // 2. Maximo goleador del equipo campeon
    Lista<Jugador>& pCamp = campeon->getPlantilla();
    Jugador* maxGoleadorCamp = &pCamp[0];
    for (int i = 1; i < pCamp.getTamanio(); i++) {
        iteracionesGlobales++;
        if (pCamp[i].getGoles() > maxGoleadorCamp->getGoles()) {
            maxGoleadorCamp = &pCamp[i];
        }
    }
    std::cout << "\n[2] MAXIMO GOLEADOR DEL CAMPEON (" << campeon->getPais() << "):\n";
    std::cout << "  - " << maxGoleadorCamp->getNombreCompleto()
              << " (Dorsal " << maxGoleadorCamp->getDorsal() << ") con "
              << maxGoleadorCamp->getGoles() << " goles historicos.\n";

    // 3. Los tres mayores goleadores de toda la copa mundial
    Lista<Jugador*> todosJugadores;
    for (int i = 0; i < equipos.getTamanio(); i++) {
        Lista<Jugador>& p = equipos[i].getPlantilla();
        for (int j = 0; j < p.getTamanio(); j++) {
            iteracionesGlobales++;
            todosJugadores.agregar(&p[j]);
        }
    }

    // Ordenamiento Burbuja descendente para todos los jugadores
    for (int i = 0; i < todosJugadores.getTamanio() - 1; i++) {
        for (int j = 0; j < todosJugadores.getTamanio() - i - 1; j++) {
            iteracionesGlobales++;
            if (todosJugadores[j]->getGoles() < todosJugadores[j+1]->getGoles()) {
                Jugador* temp = todosJugadores[j];
                todosJugadores[j] = todosJugadores[j+1];
                todosJugadores[j+1] = temp;
            }
        }
    }

    std::cout << "\n[3] TOP 3 MAYORES GOLEADORES DE LA COPA:\n";
    for (int i = 0; i < 3 && i < todosJugadores.getTamanio(); i++) {
        std::cout << "  " << (i+1) << ". " << todosJugadores[i]->getNombreCompleto()
        << " con " << todosJugadores[i]->getGoles() << " goles.\n";
    }

    // 4. Equipo con mas goles historicos (ya actualizados)
    Equipo* eqMaxGoles = &equipos[0];
    for (int i = 1; i < equipos.getTamanio(); i++) {
        iteracionesGlobales++;
        if (equipos[i].getGF() > eqMaxGoles->getGF()) {
            eqMaxGoles = &equipos[i];
        }
    }
    std::cout << "\n[4] EQUIPO CON MAS GOLES HISTORICOS EN TOTAL:\n";
    std::cout << "  - " << eqMaxGoles->getPais() << " con " << eqMaxGoles->getGF() << " goles a favor.\n";

    // 5. Confederacion con mayor presencia en R16, R8 y R4
    std::string confeds[] = {"UEFA", "CONMEBOL", "CONCACAF", "CAF", "AFC", "OFC"};

    // Conteo manual para R16 (Octavos)
    int countR16[6] = {0};
    for(int i = 0; i < clasificadosR16.getTamanio(); i++){
        iteracionesGlobales++;
        for(int j = 0; j < 6; j++) if(clasificadosR16[i]->getConfederacion() == confeds[j]) countR16[j]++;
    }
    int maxR16 = 0;
    for(int i = 1; i < 6; i++) if(countR16[i] > countR16[maxR16]) maxR16 = i;

    // Conteo manual para R8 (Cuartos - QF)
    int countQF[6] = {0};
    for(int i = 0; i < clasificadosQF.getTamanio(); i++){
        iteracionesGlobales++;
        for(int j = 0; j < 6; j++) if(clasificadosQF[i]->getConfederacion() == confeds[j]) countQF[j]++;
    }
    int maxQF = 0;
    for(int i = 1; i < 6; i++) if(countQF[i] > countQF[maxQF]) maxQF = i;

    // Conteo manual para R4 (Semifinales - SF)
    int countSF[6] = {0};
    for(int i = 0; i < clasificadosSF.getTamanio(); i++){
        iteracionesGlobales++;
        for(int j = 0; j < 6; j++) if(clasificadosSF[i]->getConfederacion() == confeds[j]) countSF[j]++;
    }
    int maxSF = 0;
    for(int i = 1; i < 6; i++) if(countSF[i] > countSF[maxSF]) maxSF = i;

    std::cout << "\n[5] CONFEDERACION DOMINANTE POR ETAPA:\n";
    std::cout << "  - En Octavos de Final (R16): " << confeds[maxR16] << "\n";
    std::cout << "  - En Cuartos de Final (R8):  " << confeds[maxQF] << "\n";
    std::cout << "  - En Semifinales (R4):       " << confeds[maxSF] << "\n";
    std::cout << "==================================================\n\n";
}

void Mundial::mostrarRecursos() const {
    std::cout << "\n[Recursos] Iteraciones: " << iteracionesGlobales
              << " | Memoria activa: " << memoriaGlobal << " bytes\n";
}