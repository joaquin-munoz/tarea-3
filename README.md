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
* en el grafo en el escenario final si el campo de objeto no contiene absolutamente nada se vuelve loop infinito al intentar llegar a la meta , si se escribe cualquier palabra o caracter esto se soluciona.

## A mejorar

* Peso máximo: Limitar el inventario para aumentar el reto.

## Ejemplo de uso

## *Paso 1 : Inicio de la partida*

  ´´´
  ========================================
      Bienvenido a Juego del Laberinto
  ========================================
  1) Cargar laberinto
  2) Iniciar partida
  3) Salir

  ´´´

## *Paso 2 : Cargar el laberinto

seleccione opcion 1 
  ´´´
  Laberinto cargado correctamente.
  ´´´

## *Paso 3 : Iniciar la partida*
opcion 2 

  ´´´
  === ESTADO ACTUAL ===
  Escenario: Entrada del Laberinto
  Descripcion: Estás en la entrada de un oscuro laberinto.

  Items disponibles:
    1) linterna (Peso: 1, Puntaje: 10)
    2) llave (Peso: 2, Puntaje: 15)

  Tiempo restante: 10

  Inventario:
    (vacio)
  Peso total: 0 | Puntaje: 0

  Direcciones disponibles:
  W) Arriba
  D) Derecha

  == OPCIONES ==
  1) Recoger items
  2) Descartar items
  3) Avanzar en una direccion
  4) Reiniciar partida
  5) Salir
  ´´´

## *Paso 4.1 : Recoger ítems*
elegimos opcion 1,luego elegimos la 1 o 2 dependiendo:

  ´´´
  ¿Deseas recoger (objeto)? (1 = si, 0 = no): 
  ´´´

## *Paso 4.2: Descartar items*
elegimos opcion 2,luego elegimos la 1 o 2 dependiendo:

  ´´´
  ¿Deseas descartar (objeto)? (1 = si, 0 = no): 
  ´´´

## *Paso 4.3: Moverse por el laberinto*

Seleccionamos la opción 3:

  ´´´
  ¿A qué dirección quieres ir? (W/A/S/D): W
  ´´´

## *Paso 4.4: Reiniciar partida*
opcion 4 y se restablecen los valores de partida

  ´´´
  Reiniciando partida...
  ´´´

## *Paso 5: Salir*
opcion 5

  ´´´
  Saliendo del juego...
  ´´´