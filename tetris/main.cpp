#include <QCoreApplication>
#include <QTextStream>
#include "tetris.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    QTextStream out(stdout);
    QTextStream in(stdin);

    int width = 0, height = 0;

    out << "---  TETRIS ---\n";
    out << "Ingrese el ancho del tablero (minimo 8, multiplo de 8): ";
    out.flush();
    in >> width;

    out << "Ingrese el alto del tablero (minimo 8): ";
    out.flush();
    in >> height;

    if (width < 8 || width % 8 != 0 || height < 8) {
        out << "Error: Dimensiones invalidas segun los requisitos.\n";
        out.flush();
        return 1;
    }

    Tetris game(width, height);
    game.play();

    return 0;
}