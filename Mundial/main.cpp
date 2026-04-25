#include <iostream>
#include <cstdlib>
#include <ctime>
#include "mundial.h"
#include "globales.h"

int main() {
    // Inicializar la semilla para los procesos aleatorios
    srand(static_cast<unsigned>(time(0)));

    Mundial mundial;
    int opcion = 0;

    do {
        // Reiniciamos el contador de iteraciones para cada acción del menú
        iteracionesGlobales = 0;

        std::cout << "\n======================================\n";
        std::cout << "        UdeAWorldCup - MENU          \n";
        std::cout << "======================================\n";
        std::cout << "1. Cargar Datos (.csv)\n";
        std::cout << "2. Conformar Grupos (Sorteo)\n";
        std::cout << "3. Simular Fase de Grupos\n";
        std::cout << "4. Simular Fases Eliminatorias (R32 a Final)\n";
        std::cout << "5. Generar Informe de Estadisticas\n";
        std::cout << "6. Salir\n";
        std::cout << "--------------------------------------\n";
        std::cout << "Seleccione una opcion: ";

        std::cin >> opcion;

        // Validacion para evitar bucles infinitos si se ingresa texto
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            std::cout << "\n[!] Error: Por favor, ingrese un numero valido (1-6).\n";
            continue;
        }

        switch (opcion) {
        case 1:
            // Ruta absoluta configurada para tu equipo
            mundial.cargarDatos("C:\\Users\\Maria\\Desktop\\tareas_Informatica\\Informatica_2_TEORIA\\Mundial\\selecciones_clasificadas_mundial.csv");
            break;
        case 2:
            mundial.conformarGrupos();
            break;
        case 3:
            mundial.simularFaseGrupos();
            break;
        case 4:
            // Esta opcion realiza la transicion y juega hasta la Final
            mundial.simularFasesEliminatorias();
            break;
        case 5:
            mundial.generarEstadisticas();
            break;
        case 6:
            std::cout << "Saliendo del sistema... \n";
            break;
        default:
            std::cout << "Opcion no valida. Intente de nuevo.\n";
            break;
        }

        // Mostrar consumo de recursos si se ejecuto una opcion valida (1 a 5)
        if (opcion >= 1 && opcion <= 5) {
            mundial.mostrarRecursos();
        }

    } while (opcion != 6);

    return 0;
}
