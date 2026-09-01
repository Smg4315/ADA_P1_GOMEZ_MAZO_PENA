# Práctica 1 — Fuerza Bruta y Backtracking
 
Implementación en C++17 de dos módulos independientes sobre el mismo dominio (contraseñas y espacios de cadenas sobre un alfabeto), abordados desde dos paradigmas opuestos:
 
- **Módulo FB (Fuerza Bruta):** dado el hash SHA-256 de una contraseña sintética, recuperarla mediante enumeración exhaustiva del espacio de candidatos.
- **Módulo BT (Backtracking):** dada una política de composición de contraseñas, contar o generar todas las contraseñas que la cumplen, construyéndolas de forma incremental y podando las ramas infactibles.
Universidad EAFIT — Análisis y Diseño de Algoritmos
Docente: Carlos Alberto Álvarez Henao
 
---
 
## Integrantes
 
Listados en orden alfabético por apellido, que es el mismo orden usado para derivar la semilla del equipo (ver *Reproducción de experimentos*):
 
| Integrante | Rol principal |
|---|---|
| Mateo Gómez Giraldo | Casos de prueba y verificación experimental |
| Simón Mazo Gómez | Diseño e implementación del módulo de Fuerza Bruta |
| Johan Alejandro Peña Rivera | Diseño e implementación del módulo de Backtracking |
 
---
 
## Requisitos
 
- Compilador **g++ con soporte C++17** (GCC 7 o superior).
- No se requieren bibliotecas externas ni gestor de dependencias. La única biblioteca de terceros es `picosha2` (SHA-256, header-only, licencia MIT), incluida directamente en `src/third_party/`.
Para verificar que el compilador está disponible:
 
```bash
g++ --version
```
 
En Windows, se recomienda instalar GCC mediante MSYS2 y agregar `C:\msys64\mingw64\bin` al `PATH`. Todos los comandos de este documento funcionan igual en Windows, cambiando `./programa` por `.\programa.exe`.
 
---
 
## Estructura del proyecto
 
```
ADA_P1_GOMEZ_MAZO_PENA/
├── README.md                     Este archivo
├── src/
│   ├── fb_main.cpp               Módulo FB completo: enumeración, semilla, menú
│   ├── bt_main.cpp               Módulo BT: orquestación y menú
│   ├── bt_policy.hpp             Política de contraseñas, cálculo de semilla y derivación de parámetros
│   ├── bt_backtrack.hpp          Función de factibilidad y backtracking con poda (modo generación y modo conteo)
│   ├── bt_sinpoda.hpp            Exploración exhaustiva sin poda, usada como referencia de comparación
│   └── third_party/
│       ├── picosha2.h            Implementación SHA-256 (MIT License, © 2017 okdshin)
│       └── picosha2.cpp          Envoltorio generarHash() sobre picosha2
├── resources/
│   ├── verificar_semilla.cpp     Utilidad para verificar la semilla y los parámetros de la política
│   └── diccionario.txt           Lista sintética de candidatos para el ataque por diccionario
├── results/                      Salidas generadas por la ejecución (CSV de métricas)
├── tests/                        Notas y casos de prueba
└── report/
    └── Informe.pdf               Informe técnico
```
 
Los dos módulos son **independientes en su implementación**: no comparten código y cada uno tiene su propia función `main()`.
 
---
 
## Compilación
 
Cada módulo se compila por separado, ya que ambos definen su propio `main()`. Desde la raíz del repositorio:
 
```bash
# Módulo FB — Fuerza Bruta
g++ -std=c++17 -O2 -o fb_main src/fb_main.cpp
 
# Módulo BT — Backtracking
g++ -std=c++17 -O2 -o bt_main src/bt_main.cpp
 
# Utilidad de verificación de semilla (opcional)
g++ -std=c++17 -O2 -o verificar_semilla resources/verificar_semilla.cpp
```
 
La bandera `-O2` es relevante: sin ella los tiempos medidos aumentan de forma considerable y no son comparables con los reportados en el informe.
 
---
 
## Ejecución
 
Ambos programas son interactivos y se manejan mediante un menú por consola. No reciben argumentos por línea de comandos.
 
### Módulo FB
 
```bash
./fb_main
```
 
| Opción | Acción |
|---|---|
| `1` | Genera las 5 contraseñas objetivo del equipo a partir de la semilla y las recupera por fuerza bruta desde su hash |
| `2` | Mide el tiempo de ejecución para distintos tamaños de entrada y escribe `results/resultados_FB.csv` |
| `3` | Salir |
 
### Módulo BT
 
```bash
./bt_main
```
 
| Opción | Acción |
|---|---|
| `1` | Valida la implementación contra la instancia de referencia común del curso (n = 6) |
| `2` | Ejecuta las 5 variantes de dificultad comparando la versión con poda contra la versión sin poda, y escribe `results/bt_resultados.csv` |
| `3` | Salir |
 
Al elegir la opción `2`, el programa pregunta si se desea imponer un **límite de nodos**. Se recomienda responder `s` e ingresar un límite (por ejemplo `20000000`): dos de las cinco variantes tienen un espacio de búsqueda intratable y sin límite la ejecución puede no terminar en un tiempo razonable. Las variantes que alcancen el límite quedan marcadas con `truncado = SI` en el CSV, indicando que sus resultados son parciales.
 
---
 
## Reproducción de experimentos
 
### Semilla del equipo
 
Todas las instancias privadas del equipo se derivan de forma determinista de una semilla calculada así: se toman los apellidos de los integrantes, se normalizan a minúsculas sin espacios ni tildes, se ordenan alfabéticamente y se concatenan (`gomez` + `mazo` + `pena` = `gomezmazopena`); se suman los códigos ASCII de todos los caracteres de la cadena resultante y se aplica módulo 100000.
 
**Semilla del equipo: `1405`**
 
Para verificarla de forma independiente:
 
```bash
./verificar_semilla Gomez Mazo Pena
```
 
Salida esperada:
 
```
Apellidos leidos: 3
Semilla calculada para el equipo: 1405
 
Parametros de la politica (Modulo BT, Seccion 9.2):
  n         = 8
  minLower  = 3
  minUpper  = 2
  minDigit  = 2
  minSymbol = 1
  prohibicion de repetidos consecutivos: Si
```
 
El mismo valor `1405` es impreso por `fb_main` y por `bt_main` al ejecutar sus respectivas opciones de casos de prueba, lo que permite comprobar que los tres puntos del código que calculan la semilla son consistentes entre sí.
 
### Instancias del Módulo FB
 
Los alfabetos usados son **A1** = `abcdefghijklmnopqrstuvwxyz` (26 símbolos) y **A2** = `abcdefghijklmnopqrstuvwxyz123456789` (35 símbolos). A partir de la semilla se generan 5 contraseñas objetivo mediante el generador congruencial lineal `x = (1103515245 · x + 12345) mod 2^31`, tomando `x mod |Σ|` como índice dentro del alfabeto correspondiente.
 
Ejecutando `./fb_main` y eligiendo la opción `1`, las instancias generadas y recuperadas son:
 
| # | Alfabeto | n | Contraseña | Hash SHA-256 |
|---|---|---|---|---|
| 1 | A1 | 4 | `ilkv` | `ec67d6454b729ec397d7868ca8c2a933a500ba31aff2b54478b46c979eb7bcf1` |
| 2 | A2 | 4 | `kurq` | `ddb9fa99a774f18545474d5dc58cc30f7b510fe6b26d873a14c138db96e44535` |
| 3 | A1 | 5 | `azilo` | `f0255709a827f11edaa1f849f87fa3f06155692b2b2023552fd728fb996ee669` |
| 4 | A2 | 5 | `52tb4` | `e84001c3b3b5ca972673a3d9dc6f8ef5374594c860b3ec9b300262da7a045c52` |
| 5 | A1 | 6 | `odcxgd` | `cc15ba06d04aa7c48cadb208ef4e515128297622657e563a9e081b39d541ac25` |
 
Cualquiera de estos hashes puede verificarse de forma externa al proyecto:
 
```bash
echo -n "ilkv" | sha256sum
```
 
**Instancia de referencia común del curso:** la contraseña `abc12` (A2, n = 5) tiene hash `8d51feb34e3e69f6fa6dffc577e2c60490cf9a7fcd835f9f6af1505b71d74773`, que coincide con el valor publicado en el enunciado. Esta comprobación valida que la implementación de SHA-256 usada es correcta.
 
> **Tiempos esperados:** la opción `1` recupera las cinco contraseñas de forma secuencial. Las dos últimas instancias dominan el tiempo total, ya que su carácter inicial aparece tarde en el orden de recorrido del alfabeto. La ejecución completa puede tardar varios minutos; no es un bloqueo del programa.
 
### Instancias del Módulo BT
 
El alfabeto base tiene 69 símbolos: minúsculas, mayúsculas, dígitos y el conjunto `{! @ # $ %}`. La política del equipo, derivada de la semilla `1405`, es `minLower = 3`, `minUpper = 2`, `minDigit = 2`, `minSymbol = 1`, con longitud `n = 8` y prohibición de caracteres idénticos consecutivos.
 
La opción `2` evalúa cinco variantes de dificultad y escribe `results/bt_resultados.csv` con una fila por variante:
 
| Variante | Descripción |
|---|---|
| (i) | Política completa del equipo, n = 8 |
| (ii) | Misma política, n = 6 |
| (iii) | Misma política, n = 10 |
| (iv) | Política relajada (solo `minLower = 1`), n = 8 |
| (v) | Sin restricciones de composición, n = 6 — calibración del caso de poda nula |
 
El CSV contiene las columnas `variante, n, nodos_con_poda, nodos_sin_poda, soluciones_con_poda, soluciones_sin_poda, tiempo_con_poda_s, tiempo_sin_poda_s, truncado`.
 
**Verificación de correctitud:** para las variantes que terminan sin truncarse, el número de soluciones encontradas por la versión con poda debe coincidir exactamente con el de la versión sin poda. El programa realiza esta comparación automáticamente e imprime `OK` o `FALLO EN CORRECTITUD` por cada variante.
 
---
 
## Notas y limitaciones conocidas
 
- **Alfabeto A2:** la implementación usa 35 símbolos (`a–z` más los dígitos `1–9`), omitiendo el `0`. Los tiempos reportados corresponden a este alfabeto de 35 símbolos, lo cual es consistente con los factores de crecimiento medidos experimentalmente.
- **Instancia de referencia del Módulo BT (opción `1`):** esta ruta del código no aplica límite de nodos. Para la política de referencia (n = 6, mínimos 2/1/1/1) la función de factibilidad casi no logra podar, ya que la suma de los mínimos es muy cercana a `n`, por lo que el árbol recorrido se aproxima al árbol completo (del orden de 10¹¹ nodos). La ejecución puede tardar decenas de minutos; se recomienda lanzarla en segundo plano.
- **Variantes truncadas:** con un límite de 20 000 000 de nodos, las cinco variantes de la opción `2` quedan marcadas como truncadas. En ese régimen la comparación válida no es "cuántos nodos necesitó cada versión", sino qué logró cada una con el mismo presupuesto de nodos.
- **Diferencia de implementación entre las dos versiones de BT:** la versión sin poda construye copias de cadena en cada llamada y almacena las soluciones en un `vector<string>`, mientras que la versión con poda reutiliza una sola cadena y únicamente cuenta. Parte de la diferencia de tiempo entre ambas se debe a esta sobrecarga de memoria y no al efecto de la poda.
---
 
## Alcance ético
 
Todo el trabajo se realiza sobre **contraseñas sintéticas generadas por el propio equipo** mediante el procedimiento reproducible descrito arriba. En ningún momento se ataca, escanea o intenta vulnerar un sistema, cuenta, servicio o credencial real de terceros.
 
---
 
## Informe técnico
 
El informe completo se encuentra en `report/Informe.pdf`.
