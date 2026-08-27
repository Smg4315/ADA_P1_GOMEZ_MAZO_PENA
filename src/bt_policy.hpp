// bt_policy.hpp

#ifndef BT_POLICY_HPP
#define BT_POLICY_HPP

#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <cctype>

// Clasifica un caracter segun su tipo (minuscula, mayuscula, digito o simbolo).
char tipoDeCaracter(char c) {
    if (islower((unsigned char)c)) return 'L';
    if (isupper((unsigned char)c)) return 'U';
    if (isdigit((unsigned char)c)) return 'D';
    return 'S';
}

// Estructura que representa la politica de contraseñas de la Seccion 9.2
struct Policy {
    int n;          // longitud fija de la contraseña
    int minLower;   // minimo de minusculas
    int minUpper;   // minimo de mayusculas
    int minDigit;   // minimo de digitos
    int minSymbol;  // minimo de simbolos
    bool noRepetidosConsecutivos; // prohibicion de 2 caracteres iguales seguidos
};

std::string quitarTildes(const std::string& texto) {
    std::vector<std::pair<std::string, char>> tildes = {
        {"\xC3\xA1", 'a'}, // á
        {"\xC3\xA9", 'e'}, // é
        {"\xC3\xAD", 'i'}, // í
        {"\xC3\xB3", 'o'}, // ó
        {"\xC3\xBA", 'u'}  // ú
    };

    std::string resultado = "";
    for (size_t i = 0; i < texto.size(); ) {
        bool reemplazado = false;
        for (size_t k = 0; k < tildes.size(); k++) {
            const std::string& patron = tildes[k].first;
            if (texto.compare(i, patron.size(), patron) == 0) {
                resultado += tildes[k].second;
                i += patron.size();
                reemplazado = true;
                break;
            }
        }
        if (!reemplazado) {
            resultado += texto[i];
            i++;
        }
    }
    return resultado;
}

// Normaliza un apellido: minusculas, sin espacios, sin tildes
std::string normalizarApellido(const std::string& apellido) {
    std::string res = apellido;
    // pasar a minusculas
    for (size_t i = 0; i < res.size(); i++) {
        res[i] = tolower(res[i]);
    }
    // quitar espacios
    res.erase(std::remove(res.begin(), res.end(), ' '), res.end());
    res = quitarTildes(res);
    return res;
}

// Calcula la semilla del equipo a partir de los apellidos de sus integrantes
int calcularSemilla(std::vector<std::string> apellidos) {
    for (size_t i = 0; i < apellidos.size(); i++) {
        apellidos[i] = normalizarApellido(apellidos[i]);
    }

    std::sort(apellidos.begin(), apellidos.end());

    std::string concatenado = "";
    for (int i = 0; i < apellidos.size(); i++) {
        concatenado += apellidos[i];
    }

    int suma = 0;
    for (int i = 0; i < concatenado.size(); i++) {
        suma += concatenado[i];
    }

    int semilla = suma % 100000;
    return semilla;
}

Policy derivarPolitica(int semilla, int n = 8) {
    Policy p;
    p.n = n;
    p.minLower = 2 + (semilla % 3);
    p.minUpper = 1 + (semilla % 2);
    p.minDigit = 1 + (semilla % 3);
    p.minSymbol = 1;
    p.noRepetidosConsecutivos = true;

    int sumaMinimos = p.minLower + p.minUpper + p.minDigit + p.minSymbol;

    // Si la suma de los minimos supera la longitud n, se reduce minLower
    if (sumaMinimos > n) {
        int exceso = sumaMinimos - n;
        p.minLower = p.minLower - exceso;
        // si aun asi queda negativo, se deja en 0 
        if (p.minLower < 0) {
            p.minLower = 0;
        }
        std::cout << "Aviso: se reduce minLower en " << exceso
                  << " para que la politica sea satisfacible con n = " << n << std::endl;
    }

    return p;
}


bool coincideConReferencia(const Policy& p) {
    int refMinLower = 2;
    int refMinUpper = 1;
    int refMinDigit = 1;
    int refMinSymbol = 1;

    if (p.minLower == refMinLower &&
        p.minUpper == refMinUpper &&
        p.minDigit == refMinDigit &&
        p.minSymbol == refMinSymbol) {
        return true;
    }
    return false;
}

#endif // BT_POLICY_HPP