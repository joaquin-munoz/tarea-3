tarea 3 :  joaquin muñoz

## Descripción

Este proyecto implementa un juego de laberinto en lenguaje C, en el que el jugador recorre distintos escenarios recogiendo objetos y avanzando hasta llegar al estado final. Los escenarios y objetos se cargan desde un archivo CSV (`data/graphquest.csv`), y se representan mediante un grafo usando listas y mapas.

### Requisitos previos

* **Visual Studio Code** (u otro editor de tu preferencia).
* Extensión de C/C++ de Microsoft en VS Code.
* **GCC** (MinGW en Windows o gcc en Linux/macOS).

### Pasos

1. Clona o descarga el repositorio y descomprímelo.
2. Abre la carpeta del proyecto en VS Code.
3. Abre la terminal integrada (`Terminal > Nueva terminal`).
4. Compila el proyecto con:

   ```
   gcc tdas/*.c tarea3.c -o tarea3 -Wno-unused-result
   ```
5. Ejecuta la aplicación:

   ```
   ./tarea3
   ```

## Funcionalidades

* **Carga de laberinto desde CSV:** Lee escenarios, objetos y conexiones.
* **Representación de grafo:** Cada estado almacena adyacentes, objetos y metadatos.
* **Interfaz de texto:** Menú principal con opciones cargar, jugar y salir.
* **Recoger y descartar objetos:** Control de inventario y peso.
* **Movimiento dirigido:** Mostrar direcciones disponibles y calcular tiempo de desplazamiento según peso.
* **Detección de estado final:** Al alcanzar el escenario marcado como final, el juego termina.
* **Reinicio de partida:** Permite volver al estado inicial manteniendo menú.

## Funcionamiento correcto

* El laberinto se carga sin errores si el CSV está bien formateado.
* El jugador puede recoger/descartar objetos y avanzar según las direcciones disponibles.
* El tiempo restante se actualiza correctamente según el peso del inventario.
* Al llegar al estado final, muestra el puntaje acumulado y finaliza la partida.

## Problemas conocidos

* Actualmente no hay restricción de peso máximo, por lo que el jugador puede cargar objetos indefinidamente.
* No se guarda el historial de movimientos para permitir deshacer pasos.
* El código asume que los IDs de escenarios en el CSV comienzan en `1`.

## A mejorar

* **Peso máximo:** Limitar el inventario para aumentar el reto.
* **Guardado de partida:** Persistir el progreso y permitir cargar partidas.
* **Historial de movimientos:** Poder deshacer pasos o mostrar ruta recorrida.
* **Interfaz gráfica:** Migrar a GUI para mayor interactividad.

## Ejemplo de uso

```txt
========================================
     Bienvenido a Juego del Laberinto
========================================
1) Cargar laberinto
2) Iniciar partida
3) Salir
Ingrese su opción: 1
Laberinto cargado correctamente.

=== ESTADO ACTUAL ===
Escenario: Entrada principal
Descripción: Una puerta rechinante abre paso a esta mansión.

Items disponibles:
  1) Cuchillo (Peso: 3, Puntaje: 1)
  2) Pan (Peso: 2, Puntaje: 1)

Tiempo restante: 10

Inventario:
  (vacío)
Peso total: 0 | Puntaje: 0

Direcciones disponibles:
  ↑ Arriba
  ↓ Abajo

== OPCIONES ==
1) Recoger ítems
2) Descartar ítems
3) Avanzar en una dirección
4) Reiniciar partida
5) Salir
Seleccione una opción: 1
¿Deseas recoger Cuchillo? (1 = sí, 0 = no): 1
¿Deseas recoger Pan? (1 = sí, 0 = no): 0

=== ESTADO ACTUAL ===
Escenario: Entrada principal
...
```
