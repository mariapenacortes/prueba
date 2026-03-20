#ifndef TETRIS_H
#define TETRIS_H

class Tetris {
private:
    int m_width;
    int m_height;
    int m_bytesPerRow; // Cantidad de bytes necesarios por cada fila

    // Punteros para memoria dinámica
    unsigned char* m_board;
    unsigned short* m_pieces; // Arreglo para guardar los bits de las piezas

    // Estado actual del turno
    int m_currentPiece;
    int m_currentRotation;
    int m_x;
    int m_y;
    bool m_gameOver;

    // Métodos privados
    void initPieces();
    void spawnPiece();
    bool checkCollision(int pieceIdx, int rot, int x, int y);
    void mergePiece();
    void clearLines();
    void printBoard();

    // Funciones auxiliares para manipulación de bits
    unsigned short getPieceBits(int pieceIdx, int rot);
    bool isBitSet(int boardX, int boardY);
    void setBit(int boardX, int boardY);

public:
    Tetris(int width, int height);
    ~Tetris();

    void play();
};

#endif // TETRIS_H