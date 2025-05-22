#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

typedef struct {// datos sobre los estados 
  List *items;
  List *estados_adj;
  char nombre[100];
  char descripcion[1000];
  char arriba[10];
  char abajo[10];
  char izquierda[10];
  char derecha[10];
  char id[10];
  bool es_final;
} estados;

typedef struct {//datos sobre el personaje 
  List *items;
  int peso;
  float tiempo;
} personaje;

typedef struct {//datos sobre cada item
  char nombre[10];
  int peso;
  int puntaje;
} item;

void mostrarMenuPrincipal() { //primera pantalla que se muestra al abrir la aplicacion
  limpiarPantalla();
  puts("========================================");
  puts("     Bienvenido a Juego del Laberinto");
  puts("========================================");
  puts("1) Cargar laberinto");
  puts("2) Iniciar partida");
  puts("3) Salir");
}

int is_equal_str(void *key1, void *key2) {
  return strcmp((char *)key1, (char *)key2) == 0;
}

void cargar_laberinto(Map *grafo) { //funcion que cargara el archivo csv el cual contiene la informacion sobre el laberinto 
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) {
    perror("Error al cargar el laberinto");
    return;
  }

  char **campo = leer_linea_csv(archivo, ',');
  List* lista_aux = list_create();
  
  while ((campo = leer_linea_csv(archivo, ',')) != NULL) { //pregunta por si se intentara acceder a un campo invalido 
    if (!campo[0] || !campo[1] || !campo[2] || !campo[3] ||
        !campo[4] || !campo[5] || !campo[6] || !campo[7] || !campo[8]) {
      continue;
    }

    estados *estado = malloc(sizeof(estados)); //asignacion de memoria a cada estado(escenario posible)
    if (!estado) continue;

    strcpy(estado->id, campo[0]);
    strcpy(estado->nombre, campo[1]);
    strcpy(estado->descripcion, campo[2]);
    strcpy(estado->arriba, campo[4]);
    strcpy(estado->abajo, campo[5]);
    strcpy(estado->izquierda, campo[6]);
    strcpy(estado->derecha, campo[7]);
    estado->es_final = (strcmp(campo[8], "Si") == 0);

    estado->estados_adj = list_create();
    List *lista_items = list_create();

    if (campo[3] && strlen(campo[3]) > 0) { //funcion para trabajar con uno o mas items separando sus datos y almacenandolos 
      List *objetos = split_string(campo[3], ";"); //separa seccion por objeto
      char *str_item = list_first(objetos);

      while (str_item != NULL) {
        List *datos_item = split_string(str_item, ","); //separa datos de cada objeto y los almacena 
        char *nombre = list_first(datos_item);
        char *peso_str = list_next(datos_item);
        char *puntaje_str = list_next(datos_item);

        if (nombre && peso_str && puntaje_str) { // se confirma que objeto con sus datos sea valido 
          item *nuevo_item = malloc(sizeof(item));
          if (nuevo_item) { // se registra nuevo item en el escenario 
            strncpy(nuevo_item->nombre, nombre, sizeof(nuevo_item->nombre));
            nuevo_item->peso = atoi(peso_str);
            nuevo_item->puntaje = atoi(puntaje_str);
            list_pushBack(lista_items, nuevo_item);
          }
        }

        str_item = list_next(objetos); // se comprueba el siguiente posible objeto en caso de que exista
      }
    }

    estado->items = lista_items; // se agrega la lista con los items que existen 
    map_insert(grafo, estado->id, estado); // se interta el escenario en el grafo
    list_pushBack(lista_aux,estado);
  }

  fclose(archivo);
  estados* estado = list_first(lista_aux);
  while (estado != NULL) { //revisa las direcciones posibles guardandolos en estados adj como las posibles direcciones

    if (atoi(estado->arriba) != -1) {
      MapPair *par = map_search(grafo, estado->arriba);
      if (par) list_pushBack(estado->estados_adj, par->value);
    }
    if (atoi(estado->abajo) != -1) {
      MapPair *par = map_search(grafo, estado->abajo);
      if (par) list_pushBack(estado->estados_adj, par->value);
    }
    if (atoi(estado->izquierda) != -1) {
      MapPair *par = map_search(grafo, estado->izquierda);
      if (par) list_pushBack(estado->estados_adj, par->value);
    }
    if (atoi(estado->derecha) != -1) {
      MapPair *par = map_search(grafo, estado->derecha);
      if (par) list_pushBack(estado->estados_adj, par->value);
    }

    estado = list_next(lista_aux);
  }
  free(lista_aux);

  printf("Laberinto cargado correctamente.\n");
}

estados *estado_inicial(Map *grafo) { //se posiciona en el escenario inicial 
  return map_search(grafo, "1")->value;
}

void mostrar_estado(estados *estado, personaje jugador, int tiempo) { //muestra el estado al momento de moverse 
  printf("\n=== ESTADO ACTUAL ===\n");
  printf("Escenario: %s\n", estado->nombre);
  printf("Descripcion: %s\n", estado->descripcion);

  printf("\nItems disponibles:\n");
  item *it = list_first(estado->items);
  int index = 1;
  while (it != NULL) {
    printf("  %d) %s (Peso: %d, Puntaje: %d)\n", index++, it->nombre, it->peso, it->puntaje);
    it = list_next(estado->items);
  }

  printf("\nTiempo restante: %d\n", tiempo);
  printf("\nInventario:\n");

  item *inv = list_first(jugador.items);
  int total_puntaje = 0;
  if (!inv) printf("  (vacio)\n");
  while (inv != NULL) {
    printf("  - %s (Peso: %d, Puntaje: %d)\n", inv->nombre, inv->peso, inv->puntaje);
    total_puntaje += inv->puntaje;
    inv = list_next(jugador.items);
  }

  printf("Peso total: %d | Puntaje: %d\n", jugador.peso, total_puntaje);

  printf("\nDirecciones disponibles:\n");
  if (atoi(estado->arriba) != -1) printf("Arriba\n");
  if (atoi(estado->abajo) != -1) printf("Abajo\n");
  if (atoi(estado->izquierda) != -1) printf("Izquierda\n");
  if (atoi(estado->derecha) != -1) printf("Derecha\n");
  printf("A que direccion quieres ir? (W/A/S/D): ");
}

void iniciar_partida(Map *grafo) {
  personaje jugador; //incializa los datos principales del jugador 
  jugador.items = list_create();
  jugador.peso = 0;
  jugador.tiempo = 10;

  estados *actual = estado_inicial(grafo); 

  while (1) {
    mostrar_estado(actual, jugador, jugador.tiempo); // muestra el escenario principal

    if (jugador.tiempo <= 0) { //en caso de que el tiempo del jugador se acabe 
      printf("\n¡Tiempo agotado! Has perdido.\n");
      return;
    }

    if (actual->es_final) {//pregunta si es que se llega a la meta
      printf("\n¡Has alcanzado el final!\n");
      printf("Objetos en tu inventario:\n");
      item *it = list_first(jugador.items);
      int puntaje = 0;
      while (it != NULL) {
        printf("- %s (Puntaje: %d)\n", it->nombre, it->puntaje);
        puntaje += it->puntaje;
        it = list_next(jugador.items);
      }
      printf("Puntaje total: %d\n", puntaje);
      presioneTeclaParaContinuar();
      return;
    }

    printf("\n== OPCIONES ==\n"); //opciones para que realize el jugador 
    printf("1) Recoger items\n");
    printf("2) Descartar items\n");
    printf("3) Avanzar en una direccion\n");
    printf("4) Reiniciar partida\n");
    printf("5) Salir\n");
    printf("Seleccione una opcion: ");

    int opcion;
    scanf("%d", &opcion);

    if (opcion == 1) {//en caso de elegir la opcion 1 se pregunta si se desea recoger cada objeto del escenario actual 
      jugador.tiempo--;
      item *it = list_first(actual->items);
      while (it != NULL) {
        printf("¿Deseas recoger %s? (1 = si, 0 = no): ", it->nombre);
        int r;
        scanf("%d", &r);
        if (r == 1) {
          item *nuevo = malloc(sizeof(item));
          strcpy(nuevo->nombre, it->nombre);
          nuevo->peso = it->peso;
          nuevo->puntaje = it->puntaje;
          list_pushBack(jugador.items, nuevo);
          jugador.peso += nuevo->peso;
        }
        it = list_next(actual->items);
      }

    } else if (opcion == 2) {//opcion 2 en caso de que se desee eliminar un objeto del inventario actual del jugador 
      jugador.tiempo--;
      item *it = list_first(jugador.items);
      List *nueva = list_create();
      while (it != NULL) {
        printf("¿Deseas descartar %s? (1 = si, 0 = no): ", it->nombre);
        int r;
        scanf("%d", &r);
        if (r == 1) {
          jugador.peso -= it->peso;
          free(it);
        } else {
          list_pushBack(nueva, it);
        }
        it = list_next(jugador.items);
      }
      jugador.items = nueva;

    } else if (opcion == 3) { //opcion 3 para moverse por el laberinto 
      mostrar_estado(actual,jugador,jugador.tiempo);
      char dir;
      scanf(" %c", &dir);

      char *sig = NULL;
      if ((dir == 'W' || dir == 'w') && atoi(actual->arriba) != -1)
        sig = actual->arriba;
      else if ((dir == 'S' || dir == 's') && atoi(actual->abajo) != -1)
        sig = actual->abajo;
      else if ((dir == 'A' || dir == 'a') && atoi(actual->izquierda) != -1)
        sig = actual->izquierda;
      else if ((dir == 'D' || dir == 'd') && atoi(actual->derecha) != -1)
        sig = actual->derecha;

      if (sig) {
        MapPair *par = map_search(grafo, sig);
        if (!par) {
          printf("Error: no se encontro el estado destino.\n");
          continue;
        }

        int tiempo_desc = (jugador.peso + 1) / 10;
        jugador.tiempo -= tiempo_desc;
        actual = par->value;
      } else {
        printf("Dirección invalida. Intenta otra.\n");
      }

    } else if (opcion == 4) { //opcion 4 regresa los valores inciales del jugador ademas de posicionarse en el escenario principal
      printf("Reiniciando partida...\n");
      jugador.items = list_create();
      jugador.peso = 0;
      jugador.tiempo = 10;
      actual = estado_inicial(grafo);

    } else if (opcion == 5) {//opcion para salir del juego 
      printf("Saliendo del juego...\n");
      break;

    } else { //en caso de seleccionar una opcion invalida 
      printf("Opcion invalida. Intenta nuevamente.\n");
    }
  }
}

int main() {
  char opcion;
  Map *grafo = map_create(is_equal_str); //crea mapa para el grafo 

  if (grafo == NULL) {//caso de que crear el mapa falle
    printf("Error al crear el mapa.\n");
    return 1;
  }

  do {
    mostrarMenuPrincipal(); //muestra menu principal
    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_laberinto(grafo); //opcion 1: cargar el laberinto
      break;
    case '2':
      iniciar_partida(grafo); //opcion 2 luego de cargar el laberinto para iniciar la partida
      break;
    case '3':
      printf("Saliendo de la aplicacion...\n"); //opcion 3 pa salir de la aplicacion
      break;
    default:
      printf("Opcion invalida.\n");
    }

    presioneTeclaParaContinuar();
  } while (opcion != '3');

  return 0;
}
