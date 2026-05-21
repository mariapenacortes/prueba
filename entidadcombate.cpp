#include "EntidadCombate.h"

EntidadCombate::EntidadCombate(std::string n, int v, int atk, float x, float y, float w, float h)
    : ObjetoJuego(x, y, w, h), nombre(n), vida(v), ataque(atk), velY(0), gravedad(9.8f), enElSuelo(true) {}

void EntidadCombate::saltar() {
    if (enElSuelo) {
        velY = -60.0f;
        enElSuelo = false;
    }
}

void EntidadCombate::recibirDano(int cantidad) {
    vida -= cantidad;
    if (vida < 0) vida = 0;
}


void EntidadCombate::actualizarFisica(float dt) {
    if (!enElSuelo) {
        velY += gravedad * dt;
        posY += velY * dt;


        if (posY >= 400.0f) {
            posY = 400.0f;
            velY = 0;
            enElSuelo = true;
        }
    }
}