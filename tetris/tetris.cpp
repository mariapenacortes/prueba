#include "tetris.h"
#include <QTextStream>
#include <QRandomGenerator>

Tetris::Tetris(int width, int height) {
    m_width = width;
    m_height = height;
    m_bytesPerRow = width / 8;
    m_gameOver = false;


    int totalBytes = m_height * m_bytesPerRow;
    m_board = new unsigned char[totalBytes];
    for(int i = 0; i < totalBytes; ++i) {
        m_board[i] = 0;
    }


    m_pieces = new unsigned short[28];
    initPieces();

    spawnPiece();
}

Tetris::~Tetris() {
    delete[] m_board;
    delete[] m_pieces;
}

void Tetris::initPieces() {
    //  nivel de bits (matrices de 4x4 empaquetadas en 16 bits)
    // 1 = bloque ocupado, 0 = bloque vacío

    // 0: Línea (I)
    m_pieces[0] = 0x0F00; m_pieces[1] = 0x2222; m_pieces[2] = 0x00F0; m_pieces[3] = 0x4444;
    // 1: Cuadrado (O)
    m_pieces[4] = 0xCC00; m_pieces[5] = 0xCC00; m_pieces[6] = 0xCC00; m_pieces[7] = 0xCC00;
    // 2: T
    m_pieces[8] = 0x4E00; m_pieces[9] = 0x4640; m_pieces[10] = 0x0720; m_pieces[11] = 0x2620;
    // 3: S
    m_pieces[12] = 0x6C00; m_pieces[13] = 0x4620; m_pieces[14] = 0x0360; m_pieces[15] = 0x2310;
    // 4: Z
    m_pieces[16] = 0xC600; m_pieces[17] = 0x2640; m_pieces[18] = 0x0630; m_pieces[19] = 0x1320;
    // 5: J
    m_pieces[20] = 0x8E00; m_pieces[21] = 0x6440; m_pieces[22] = 0x0710; m_pieces[23] = 0x2260;
    // 6: L
    m_pieces[24] = 0x2E00; m_pieces[25] = 0x4460; m_pieces[26] = 0x0740; m_pieces[27] = 0x6220;
}

unsigned short Tetris::getPieceBits(int pieceIdx, int rot) {
    return m_pieces[(pieceIdx * 4) + rot];
}

bool Tetris::isBitSet(int boardX, int boardY) {
    if (boardX < 0 || boardX >= m_width || boardY < 0 || boardY >= m_height) return true; // Paredes
    int byteIdx = (boardY * m_bytesPerRow) + (boardX / 8);
    int bitIdx = 7 - (boardX % 8);
    return (m_board[byteIdx] >> bitIdx) & 1;
}

void Tetris::setBit(int boardX, int boardY) {
    if (boardX < 0 || boardX >= m_width || boardY < 0 || boardY >= m_height) return;
    int byteIdx = (boardY * m_bytesPerRow) + (boardX / 8);
    int bitIdx = 7 - (boardX % 8);
    m_board[byteIdx] |= (1 << bitIdx);
}

void Tetris::spawnPiece() {
    m_currentPiece = QRandomGenerator::global()->bounded(0, 7);
    m_currentRotation = 0;
    m_x = (m_width / 2) - 2; // Centro del tablero
    m_y = 0;

    // Si choca al salir, Game Over
    if (checkCollision(m_currentPiece, m_currentRotation, m_x, m_y)) {
        m_gameOver = true;
    }
}

bool Tetris::checkCollision(int pieceIdx, int rot, int testX, int testY) {
    unsigned short piece = getPieceBits(pieceIdx, rot);
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            // Extraer el bit específico de la pieza usando máscara
            int bitPos = 15 - (py * 4 + px);
            bool isSolid = (piece >> bitPos) & 1;

            if (isSolid) {
                if (testX + px < 0 || testX + px >= m_width || testY + py >= m_height) return true; // Fuera de límites
                if (testY + py >= 0 && isBitSet(testX + px, testY + py)) return true; // Choca con bits del tablero
            }
        }
    }
    return false;
}

void Tetris::mergePiece() {
    unsigned short piece = getPieceBits(m_currentPiece, m_currentRotation);
    for (int px = 0; px < 4; px++) {
        for (int py = 0; py < 4; py++) {
            int bitPos = 15 - (py * 4 + px);
            if ((piece >> bitPos) & 1) {
                setBit(m_x + px, m_y + py);
            }
        }
    }
    clearLines();
    spawnPiece();
}

void Tetris::clearLines() {
    for (int y = m_height - 1; y >= 0; y--) {
        bool isFull = true;
        // Revisar si todos los bytes de la fila están llenos (255 = 11111111 en binario)
        for (int b = 0; b < m_bytesPerRow; b++) {
            int byteIdx = (y * m_bytesPerRow) + b;
            if (m_board[byteIdx] != 255) {
                isFull = false;
                break;
            }
        }

        if (isFull) {
            // Bajar las filas superiores (manipulación de punteros implícita en arreglos)
            for (int k = y; k > 0; k--) {
                for (int b = 0; b < m_bytesPerRow; b++) {
                    m_board[(k * m_bytesPerRow) + b] = m_board[((k - 1) * m_bytesPerRow) + b];
                }
            }
            // Limpiar la fila superior
            for (int b = 0; b < m_bytesPerRow; b++) m_board[b] = 0;
            y++; // Volver a revisar esta misma fila porque todo bajó
        }
    }
}

void Tetris::printBoard() {
    QTextStream out(stdout);
    unsigned short piece = getPieceBits(m_currentPiece, m_currentRotation);

    for (int y = 0; y < m_height; y++) {
        out << "|";
        for (int x = 0; x < m_width; x++) {
            // Revisar si el bit pertenece a la pieza que está cayendo
            bool isPieceBit = false;
            if (x >= m_x && x < m_x + 4 && y >= m_y && y < m_y + 4) {
                int px = x - m_x;
                int py = y - m_y;
                int bitPos = 15 - (py * 4 + px);
                isPieceBit = (piece >> bitPos) & 1;
            }

            if (isPieceBit) {
                out << "#";
            } else if (isBitSet(x, y)) {
                out << "#";
            } else {
                out << ".";
            }
        }
        out << "|\n";
    }
    out << "Accion: [A]Izq [D]Der [S]Bajar [W]Rotar [Q]Salir: ";
    out.flush();
}

void Tetris::play() {
    QTextStream in(stdin);
    QTextStream out(stdout);

    while (!m_gameOver) {
        printBoard();

        char input;

        // Bucle para ignorar los 'Enters' y espacios invisibles
        do {
            in >> input;
        } while (input == '\n' || input == '\r' || input == ' ' || input == '\t');

        out << "\n"; // Para que el siguiente tablero se imprima limpio abajo
        out.flush();

        int nextX = m_x;
        int nextY = m_y;
        int nextRot = m_currentRotation;

        if (input == 'A' || input == 'a') nextX--;
        else if (input == 'D' || input == 'd') nextX++;
        else if (input == 'W' || input == 'w') nextRot = (m_currentRotation + 1) % 4;
        else if (input == 'S' || input == 's') nextY++;
        else if (input == 'Q' || input == 'q') break;

        // Comprobación de colisiones de turno
        if (input == 'S' || input == 's') {
            if (!checkCollision(m_currentPiece, nextRot, nextX, nextY)) {
                m_y = nextY;
            } else {
                mergePiece(); // Si no puede bajar, se fusiona
            }
        } else {
            if (!checkCollision(m_currentPiece, nextRot, nextX, nextY)) {
                m_x = nextX;
                m_currentRotation = nextRot;
            }
            // Después de moverse a los lados o rotar, la gravedad actúa en el turno
            if (!checkCollision(m_currentPiece, m_currentRotation, m_x, m_y + 1)) {
                m_y++;
            } else {
                mergePiece();
            }
        }
    }

    out << "\n*** GAME OVER ***\n";
    out.flush();
}