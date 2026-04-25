#include "partido.h"
#include "globales.h"
#include <cmath>
#include <cstdlib>
#include <iostream>

Partido::Partido() : eq1(nullptr), eq2(nullptr), goles1(0), goles2(0), posesion1(0), posesion2(0), esEmpate(false) {}

Partido::Partido(std::string f, std::string s, Equipo* e1, Equipo* e2)
    : fecha(f), sede(s), arb1("codArbitro1"), arb2("codArbitro2"), arb3("codArbitro3"),
    eq1(e1), eq2(e2), goles1(0), goles2(0), posesion1(0), posesion2(0), esEmpate(false) {}

double Partido::calcularLambda(Equipo* a, Equipo* b) {
    double mu = 1.35;
    double alpha = 0.6;
    double beta = 0.4;

    // 1. Calcular total de partidos jugados historicamente
    int pjA = a->getPG() + a->getPE() + a->getPP();
    int pjB = b->getPG() + b->getPE() + b->getPP();

    // 2. Sacar el promedio de goles (evitando division por cero)
    double promGFA = (pjA > 0) ? (double)a->getGF() / pjA : 1.0;
    double promGCB = (pjB > 0) ? (double)b->getGC() / pjB : 1.0;

    // 3. Aplicar la formula original
    double baseA = promGFA / mu;
    double baseB = promGCB / mu;

    if (baseA <= 0) baseA = 0.01;
    if (baseB <= 0) baseB = 0.01;

    return mu * pow(baseA, alpha) * pow(baseB, beta);
}

void Partido::simular(bool faseEliminatoria) {
    iteracionesGlobales++;
    double lambda1 = calcularLambda(eq1, eq2);
    double lambda2 = calcularLambda(eq2, eq1);

    goles1 = std::round(lambda1);
    goles2 = std::round(lambda2);

    int r1 = eq1->getRanking();
    int r2 = eq2->getRanking();
    int totalR = r1 + r2;
    posesion1 = (double)r2 / totalR * 100.0;
    posesion2 = 100.0 - posesion1;

    esEmpate = (goles1 == goles2);

    if (faseEliminatoria && esEmpate) {
        if (r1 < r2) goles1++; else goles2++;
        esEmpate = false;
    }

    eq1->actualizarHistorico(goles1, goles2, goles1 > goles2, esEmpate, goles1 < goles2);
    eq2->actualizarHistorico(goles2, goles1, goles2 > goles1, esEmpate, goles2 < goles1);
}

void Partido::imprimirResultado() const {
    std::cout << "[" << fecha << " - " << sede << "] "
              << eq1->getPais() << " " << goles1 << " - "
              << goles2 << " " << eq2->getPais() << "\n";
}

Equipo* Partido::getGanador() {
    if (goles1 > goles2) return eq1;
    if (goles2 > goles1) return eq2;
    return nullptr;
}