// Este archivo contiene la implementación de la función hash256 de picosha2.h para generar los hashes de las contraeñas

#include "picosha2.h"
#include <iostream>
#include <string>

using namespace std;

string generarHash(string psw) {

    string hash_hex_str = picosha2::hash256_hex_string(psw); // LLamado a la función que me saca el hash de la cadena de texto
    // cout << "Hash of \"" << psw << "\": " << hash_hex_str << endl;

    return hash_hex_str;
}

/*int main() {

    string contraseña = "aaa"; //x Contraseña a generar
    string hash = generarHash(contraseña); // Llamado a la función que me genera el hash de la contraseña
    cout << "Hash of \"" << contraseña << "\": " << hash << endl;
}

*/