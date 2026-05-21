#ifndef ENTIDADCOMBATE_H
#define ENTIDADCOMBATE_H

#include "ObjetoJuego.h"
#include <string>

class EntidadCombate : public ObjetoJuego {
protected:
    std::string nombre;
    int vida;
    int ataque;
    float velY;
    float gravedad;
    bool enElSuelo;

public:
    EntidadCombate(std::string n, int v, int atk, float x, float y, float w, float h);

    void saltar();
    void recibirDano(int cantidad);


    void actualizarFisica(float dt) override;

    int getVida() const { return vida; }
    bool estaVivo() const { return vida > 0; }
};

#endif