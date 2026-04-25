#ifndef LISTA_H
#define LISTA_H
#include "Globales.h"

template <typename T>
class Lista {
private:
    T* datos;
    int capacidad;
    int tamanio;

public:
    Lista() : capacidad(10), tamanio(0) {
        datos = new T[capacidad];
    }

    ~Lista() {
        delete[] datos;
    }

    Lista(const Lista& otra) : capacidad(otra.capacidad), tamanio(otra.tamanio) {
        datos = new T[capacidad];
        for (int i = 0; i < tamanio; i++) {
            datos[i] = otra.datos[i];
            iteracionesGlobales++;
        }
    }

    Lista& operator=(const Lista& otra) {
        if (this != &otra) {
            delete[] datos;
            capacidad = otra.capacidad;
            tamanio = otra.tamanio;
            datos = new T[capacidad];
            for (int i = 0; i < tamanio; i++) {
                datos[i] = otra.datos[i];
                iteracionesGlobales++;
            }
        }
        return *this;
    }

    void agregar(const T& elemento) {
        iteracionesGlobales++;
        if (tamanio == capacidad) {
            capacidad *= 2;
            T* nuevos = new T[capacidad];
            for (int i = 0; i < tamanio; i++) {
                nuevos[i] = datos[i];
                iteracionesGlobales++;
            }
            delete[] datos;
            datos = nuevos;
        }
        datos[tamanio++] = elemento;
    }

    T& operator[](int index) {
        return datos[index];
    }

    const T& operator[](int index) const {
        return datos[index];
    }

    int getTamanio() const {
        return tamanio;
    }
};
#endif