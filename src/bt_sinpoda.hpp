// bt_sinpoda.hpp

#ifndef BT_SINPODA_HPP
#define BT_SINPODA_HPP

#include <string>
#include <vector>
#include <limits>
#include "bt_policy.hpp"

// Backtracking SIN PODA.

void backtrackSinPoda(std::string prefijo,
                       int countLower, int countUpper, int countDigit, int countSymbol,
                       char ultimoCaracter,
                       const Policy& p,
                       const std::string& alfabeto,
                       std::vector<std::string>& soluciones,
                       long long& nodosGenerados,
                       long long maxNodos = std::numeric_limits<long long>::max()) {

    if (nodosGenerados >= maxNodos) {
        return;
    }

    nodosGenerados++;

    // caso base: aqui se verifica la politica
    if ((int)prefijo.size() == p.n) {
        if (countLower >= p.minLower && countUpper >= p.minUpper &&
            countDigit >= p.minDigit && countSymbol >= p.minSymbol) {
            soluciones.push_back(prefijo);
        }
        return;
    }

    for (char c : alfabeto) {
        if (p.noRepetidosConsecutivos && c == ultimoCaracter) {
            continue;
        }

        if (nodosGenerados >= maxNodos) {
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

        backtrackSinPoda(prefijo + c, nuevoLower, nuevoUpper, nuevoDigit, nuevoSymbol,
                          c, p, alfabeto, soluciones, nodosGenerados, maxNodos);
    }
}

std::vector<std::string> resolverSinPoda(const Policy& p, const std::string& alfabeto,
                                          long long& nodosGenerados,
                                          long long maxNodos = std::numeric_limits<long long>::max()) {
    std::vector<std::string> soluciones;
    nodosGenerados = 0;
    backtrackSinPoda("", 0, 0, 0, 0, '\0', p, alfabeto, soluciones, nodosGenerados, maxNodos);
    return soluciones;
}

#endif // BT_SINPODA_HPP