#ifndef ENEMIGOIA_H
#define ENEMIGOIA_H
#include "EntidadCombate.h"

class EnemigoIA : public EntidadCombate {
private:
    int nivelDificultad;
    float tiempoEsperaAtaque;

public:
    EnemigoIA(std::string n, int v, int atk, float x, float y, int dificultad)
        : EntidadCombate(n, v, atk, x, y, 50, 100), nivelDificultad(dificultad), tiempoEsperaAtaque(0) {}


    void tomarDecision(float dt, float jugadorX, float jugadorY, bool jugadorAtacando) {
        tiempoEsperaAtaque += dt;


        float distancia = abs(jugadorX - posX);

        if (jugadorAtacando && distancia < 60.0f) {

        } else if (distancia > 100.0f && nivelDificultad == 2) {

            posX -= 50.0f * dt;
        } else if (distancia <= 100.0f && tiempoEsperaAtaque > (nivelDificultad == 1 ? 2.0f : 1.0f)) {

            tiempoEsperaAtaque = 0;

        }
    }
};
#endif