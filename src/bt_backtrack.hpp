// bt_backtrack.hpp


#ifndef BT_BACKTRACK_HPP
#define BT_BACKTRACK_HPP

#include <string>
#include <vector>
#include <cctype>
#include <limits>
#include <iostream>
#include "bt_policy.hpp"


char tipoDeCaracter(char c) {
    if (islower((unsigned char)c)) return 'L';
    if (isupper((unsigned char)c)) return 'U';
    if (isdigit((unsigned char)c)) return 'D';
    return 'S';
}


bool esFactible(const Policy& p, int posicionActual,
                 int countLower, int countUpper, int countDigit, int countSymbol) {

    int posicionesRestantes = p.n - posicionActual;

    int faltanLower = (p.minLower - countLower > 0) ? (p.minLower - countLower) : 0;
    int faltanUpper = (p.minUpper - countUpper > 0) ? (p.minUpper - countUpper) : 0;
    int faltanDigit = (p.minDigit - countDigit > 0) ? (p.minDigit - countDigit) : 0;
    int faltanSymbol = (p.minSymbol - countSymbol > 0) ? (p.minSymbol - countSymbol) : 0;

    int totalFaltante = faltanLower + faltanUpper + faltanDigit + faltanSymbol;

    return totalFaltante <= posicionesRestantes;
}

// Backtracking con poda.

void backtrackConPoda(std::string prefijo,
                       int countLower, int countUpper, int countDigit, int countSymbol,
                       char ultimoCaracter,
                       const Policy& p,
                       const std::string& alfabeto,
                       std::vector<std::string>& soluciones,
                       long long& nodosVisitados,
                       int limiteSoluciones,
                       long long maxNodos = std::numeric_limits<long long>::max()) {

    if (nodosVisitados >= maxNodos) {
        return;
    }

    nodosVisitados++;

    // si ya se alcanzo el limite de soluciones pedidas, no seguimos explorando
    if (limiteSoluciones > 0 && (int)soluciones.size() >= limiteSoluciones) {
        return;
    }

    // caso base: el prefijo ya tiene longitud n
    if ((int)prefijo.size() == p.n) {
        if (countLower >= p.minLower && countUpper >= p.minUpper &&
            countDigit >= p.minDigit && countSymbol >= p.minSymbol) {
            soluciones.push_back(prefijo);
        }
        return;
    }

    // poda: si el prefijo actual ya no puede llevar a una solucion valida,
    // se abandona esta rama sin generar ninguno de sus descendientes
    if (!esFactible(p, (int)prefijo.size(), countLower, countUpper, countDigit, countSymbol)) {
        return;
    }

    for (char c : alfabeto) {
        // prohibicion de dos caracteres identicos consecutivos
        if (p.noRepetidosConsecutivos && c == ultimoCaracter) {
            continue;
        }

        if (nodosVisitados >= maxNodos) {
            return; 
        }

        int nuevoLower = countLower;
        int nuevoUpper = countUpper;
        int nuevoDigit = countDigit;
        int nuevoSymbol = countSymbol;

        char tipo = tipoDeCaracter(c);
        if (tipo == 'L') nuevoLower++;
        else if (tipo == 'U') nuevoUpper++;
        else if (tipo == 'D') nuevoDigit++;
        else nuevoSymbol++;

        backtrackConPoda(prefijo + c, nuevoLower, nuevoUpper, nuevoDigit, nuevoSymbol,
                          c, p, alfabeto, soluciones, nodosVisitados, limiteSoluciones, maxNodos);

        if (limiteSoluciones > 0 && (int)soluciones.size() >= limiteSoluciones) {
            return; 
        }
    }
}


std::vector<std::string> resolverBacktracking(const Policy& p, const std::string& alfabeto,
                                               long long& nodosVisitados,
                                               int limiteSoluciones = -1,
                                               long long maxNodos = std::numeric_limits<long long>::max()) {
    std::vector<std::string> soluciones;
    nodosVisitados = 0;
    backtrackConPoda("", 0, 0, 0, 0, '\0', p, alfabeto, soluciones, nodosVisitados, limiteSoluciones, maxNodos);
    return soluciones;
}

void backtrackContarConPoda(std::string& prefijo,
                             int countLower, int countUpper, int countDigit, int countSymbol,
                             char ultimoCaracter,
                             const Policy& p,
                             const std::string& alfabeto,
                             long long& totalSoluciones,
                             long long& nodosVisitados,
                             long long maxNodos = std::numeric_limits<long long>::max(),
                             long long intervaloProgreso = 0) {

    if (nodosVisitados >= maxNodos) {
        return;
    }

    nodosVisitados++;


    if (intervaloProgreso > 0 && nodosVisitados % intervaloProgreso == 0) {
        std::cout << "  ...progreso: " << nodosVisitados << " nodos visitados, "
                  << totalSoluciones << " soluciones encontradas hasta ahora" << std::endl;
    }

    if ((int)prefijo.size() == p.n) {
        if (countLower >= p.minLower && countUpper >= p.minUpper &&
            countDigit >= p.minDigit && countSymbol >= p.minSymbol) {
            totalSoluciones++;
        }
        return;
    }

    if (!esFactible(p, (int)prefijo.size(), countLower, countUpper, countDigit, countSymbol)) {
        return;
    }

    for (char c : alfabeto) {
        if (p.noRepetidosConsecutivos && c == ultimoCaracter) {
            continue;
        }

        if (nodosVisitados >= maxNodos) {
            return;
        }

        int nuevoLower = countLower;
        int nuevoUpper = countUpper;
        int nuevoDigit = countDigit;
        int nuevoSymbol = countSymbol;

        char tipo = tipoDeCaracter(c);
        if (tipo == 'L') nuevoLower++;
        else if (tipo == 'U') nuevoUpper++;
        else if (tipo == 'D') nuevoDigit++;
        else nuevoSymbol++;

        prefijo.push_back(c);
        backtrackContarConPoda(prefijo, nuevoLower, nuevoUpper, nuevoDigit, nuevoSymbol,
                                c, p, alfabeto, totalSoluciones, nodosVisitados, maxNodos, intervaloProgreso);
        prefijo.pop_back();
    }
}


long long contarSoluciones(const Policy& p, const std::string& alfabeto,
                            long long& nodosVisitados,
                            long long maxNodos = std::numeric_limits<long long>::max(),
                            long long intervaloProgreso = 0) {
    std::string prefijo = "";
    long long totalSoluciones = 0;
    nodosVisitados = 0;
    backtrackContarConPoda(prefijo, 0, 0, 0, 0, '\0', p, alfabeto, totalSoluciones, nodosVisitados, maxNodos, intervaloProgreso);
    return totalSoluciones;
}

#endif // BT_BACKTRACK_HPP