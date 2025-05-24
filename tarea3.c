#include "tdas/extra.h"
#include "tdas/list.h"
#include "tdas/map.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  List *items;
  List *estados_adj;
  char nombre[100];
  char descripcion[100];
  char arriba[10];
  char abajo[10];
  char izquierda[10];
  char derecha[10];
  char id[10];
  char final[10];
} estados;

typedef struct {
  List *items;
  int peso;
  float tiempo;
} personaje;

typedef struct {
  char nombre[10];
  int peso;
  int puntaje;
} item;

void mostrarMenuPrincipal() {
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

void cargar_laberinto(Map *grafo) {
  FILE *archivo = fopen("data/graphquest.csv", "r");
  if (archivo == NULL) {
    perror("Error al cargar el laberinto");
    return;
  }

  char **campo = leer_linea_csv(archivo, ',');

  while ((campo = leer_linea_csv(archivo, ',')) != NULL) {
    if (!campo[0] || !campo[1] || !campo[2] || !campo[3] ||
        !campo[4] || !campo[5] || !campo[6] || !campo[7] || !campo[8]) {
      continue;
    }

    estados *estado = malloc(sizeof(estados));
    if (!estado) continue;

    strcpy(estado->id, campo[0]);
    strcpy(estado->nombre, campo[1]);
    strcpy(estado->descripcion, campo[2]);
    strcpy(estado->arriba, campo[4]);
    strcpy(estado->abajo, campo[5]);
    strcpy(estado->izquierda, campo[6]);
    strcpy(estado->derecha, campo[7]);

    if (strcmp(campo[8], "Si") == 0)
      strcpy(estado->final, "true");
    else
      strcpy(estado->final, "false");

    estado->estados_adj = list_create();
    List *lista_items = list_create();

    if (campo[3] && strlen(campo[3]) > 0) {
      List *objetos = split_string(campo[3], ";");
      char *str_item = list_first(objetos);

      while (str_item != NULL) {
        List *datos_item = split_string(str_item, ",");
        char *nombre = list_first(datos_item);
        char *peso_str = list_next(datos_item);
        char *puntaje_str = list_next(datos_item);

        if (nombre && peso_str && puntaje_str) {
          item *nuevo_item = malloc(sizeof(item));
          if (nuevo_item) {
            strncpy(nuevo_item->nombre, nombre, sizeof(nuevo_item->nombre));
            nuevo_item->peso = atoi(peso_str);
            nuevo_item->puntaje = atoi(puntaje_str);
            list_pushBack(lista_items, nuevo_item);
          }
        }

        str_item = list_next(objetos);
      }
    }

    estado->items = lista_items;
    map_insert(grafo, estado->id, estado);
  }

  fclose(archivo);

  MapPair *par_grafo = map_first(grafo);
  while (par_grafo != NULL) {
    estados *estado_aux = par_grafo->value;

    if (atoi(estado_aux->arriba) != -1) {
      MapPair *par = map_search(grafo, estado_aux->arriba);
      if (par) list_pushBack(estado_aux->estados_adj, par->value);
    }
    if (atoi(estado_aux->abajo) != -1) {
      MapPair *par = map_search(grafo, estado_aux->abajo);
      if (par) list_pushBack(estado_aux->estados_adj, par->value);
    }
    if (atoi(estado_aux->izquierda) != -1) {
      MapPair *par = map_search(grafo, estado_aux->izquierda);
      if (par) list_pushBack(estado_aux->estados_adj, par->value);
    }
    if (atoi(estado_aux->derecha) != -1) {
      MapPair *par = map_search(grafo, estado_aux->derecha);
      if (par) list_pushBack(estado_aux->estados_adj, par->value);
    }

    par_grafo = map_next(grafo);
  }

  printf("Laberinto cargado correctamente.\n");
}

estados *estado_inicial(Map *grafo) {
  return map_search(grafo, "1")->value;
}

void mostrar_estado(estados *estado, personaje *jugador, int tiempo) {
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

  item *inv = list_first(jugador->items);
  int total_puntaje = 0;
  if (!inv) printf("  (vacio)\n");
  while (inv != NULL) {
    printf("  - %s (Peso: %d, Puntaje: %d)\n", inv->nombre, inv->peso, inv->puntaje);
    total_puntaje += inv->puntaje;
    inv = list_next(jugador->items);
  }

  printf("Peso total: %d | Puntaje: %d\n", jugador->peso, total_puntaje);

  printf("\nDirecciones disponibles:\n");
  if (atoi(estado->arriba) != -1) printf("W) Arriba\n");
  if (atoi(estado->abajo) != -1) printf("S) Abajo\n");
  if (atoi(estado->izquierda) != -1) printf("A) Izquierda\n");
  if (atoi(estado->derecha) != -1) printf("D) Derecha\n");
}

void iniciar_partida(Map *grafo) {
  personaje jugador;
  jugador.items = list_create();
  jugador.peso = 0;
  jugador.tiempo = 10;

  estados *actual = estado_inicial(grafo);

  while (1) {
    mostrar_estado(actual, &jugador, jugador.tiempo);

    if (jugador.tiempo <= 0) {
      printf("\n¡Tiempo agotado! Has perdido.\n");
      return;
    }

    if (strcmp(actual->final, "true") == 0) {
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
      return;
    }

    printf("\n== OPCIONES ==\n");
    printf("1) Recoger items\n");
    printf("2) Descartar items\n");
    printf("3) Avanzar en una direccion\n");
    printf("4) Reiniciar partida\n");
    printf("5) Salir\n");
    printf("Seleccione una opcion: ");

    int opcion;
    scanf("%d", &opcion);

    if (opcion == 1) {
      jugador.tiempo--;
      item *it = list_first(actual->items);
      List *nueva_lista = list_create();

      while (it != NULL) {
        item *siguiente = list_next(actual->items);

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

          free(it);
        } else {
          list_pushBack(nueva_lista, it);
        }

        it = siguiente;
      }

      list_clean(actual->items);
      actual->items = nueva_lista;

    } else if (opcion == 2) {
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

    } else if (opcion == 3) {
      printf("¿A qué dirección quieres ir? (W/A/S/D): ");
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

        int tiempo_desc = (jugador.peso + 1 + 9) / 10;
        jugador.tiempo -= tiempo_desc;
        actual = par->value;
      } else {
        printf("Direccion invalida. Intenta otra.\n");
      }

    } else if (opcion == 4) {
      printf("Reiniciando partida...\n");
      jugador.items = list_create();
      jugador.peso = 0;
      jugador.tiempo = 10;
      actual = estado_inicial(grafo);

    } else if (opcion == 5) {
      printf("Saliendo del juego...\n");
      break;

    } else {
      printf("Opcion invalida. Intenta nuevamente.\n");
    }
  }
}

int main() {
  char opcion;
  Map *grafo = map_create(is_equal_str);

  if (grafo == NULL) {
    printf("Error al crear el mapa.\n");
    return 1;
  }

  do {
    mostrarMenuPrincipal();
    printf("Ingrese su opcion: ");
    scanf(" %c", &opcion);

    switch (opcion) {
    case '1':
      cargar_laberinto(grafo);
      break;
    case '2':
      iniciar_partida(grafo);
      break;
    case '3':
      printf("Saliendo del juego...\n");
      break;
    default:
      printf("Opción invalida.\n");
    }

    presioneTeclaParaContinuar();
  } while (opcion != '3');

  return 0;
}
