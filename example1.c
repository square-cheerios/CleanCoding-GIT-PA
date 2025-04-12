#include <stdlib.h>
#include <stdio.h>

// ======================== Structuri ========================

typedef struct Nod {
    int id_restaurant; // ID-ul restaurantului
    struct Nod *urmator; // Legătura către următorul nod
} NOD;

typedef struct Graf {
    int numar_restaurante; // Numărul total de restaurante (vârfuri)
    int *vizitat; // Vector pentru a marca restaurantele vizitate
    NOD **lista_adiacenta; // Lista de adiacență (ce restaurante sunt conectate)
} GRAF;

typedef struct Stiva {
    int varf; // Poziția curentă în stivă
    int capacitate; // Capacitatea maximă a stivei
    int *elemente; // Elementele stocate în stivă
} STIVA;


// ======================== Funcții utilitare ========================

NOD *creaza_nod(int id_restaurant) {
    NOD *nod_nou = malloc(sizeof(NOD));
    nod_nou->id_restaurant = id_restaurant;
    nod_nou->urmator = NULL;
    return nod_nou;
}

void adauga_drum(GRAF *graf, int sursa, int destinatie) {
    // Adaugă drum de la sursă la destinație (graf neorientat)
    NOD *nod_nou = creaza_nod(destinatie);
    nod_nou->urmator = graf->lista_adiacenta[sursa];
    graf->lista_adiacenta[sursa] = nod_nou;

    // Adaugă și drumul invers (dacă graful este neorientat)
    nod_nou = creaza_nod(sursa);
    nod_nou->urmator = graf->lista_adiacenta[destinatie];
    graf->lista_adiacenta[destinatie] = nod_nou;
}

GRAF *creaza_graf(int numar_restaurante) {
    GRAF *graf = malloc(sizeof(GRAF));
    graf->numar_restaurante = numar_restaurante;
    graf->lista_adiacenta = malloc(numar_restaurante * sizeof(NOD *));
    graf->vizitat = malloc(numar_restaurante * sizeof(int));

    for (int i = 0; i < numar_restaurante; i++) {
        graf->lista_adiacenta[i] = NULL;
        graf->vizitat[i] = 0; // Inițial, niciun restaurant nu este vizitat
    }

    return graf;
}

STIVA *creaza_stiva(int capacitate) {
    STIVA *stiva = malloc(sizeof(STIVA));
    stiva->elemente = malloc(capacitate * sizeof(int));
    stiva->varf = -1; // Stiva este goală inițial
    stiva->capacitate = capacitate;
    return stiva;
}

void push(int id_restaurant, STIVA *stiva) {
    stiva->varf++;
    stiva->elemente[stiva->varf] = id_restaurant;
}


// ======================== DFS ========================

void parcurge_DFS(GRAF *graf, STIVA *stiva, int id_restaurant) {
    NOD *lista = graf->lista_adiacenta[id_restaurant];
    NOD *temp = lista;

    graf->vizitat[id_restaurant] = 1; // Marchează restaurantul ca vizitat
    printf("%d ", id_restaurant);
    push(id_restaurant, stiva);

    while (temp != NULL) {
        int restaurant_adiacent = temp->id_restaurant;
        if (graf->vizitat[restaurant_adiacent] == 0) {
            parcurge_DFS(graf, stiva, restaurant_adiacent);
        }
        temp = temp->urmator;
    }
}

void adauga_drumuri(GRAF *graf, int numar_drumuri, int numar_restaurante) {
    int sursa, destinatie;
    printf("Introduceti %d drumuri (de la 0 la %d):\n", numar_drumuri, numar_restaurante - 1);
    for (int i = 0; i < numar_drumuri; i++) {
        scanf("%d%d", &sursa, &destinatie);
        adauga_drum(graf, sursa, destinatie);
    }
}

void reseteaza_vizitate(GRAF *graf, int numar_restaurante) {
    for (int i = 0; i < numar_restaurante; i++) {
        graf->vizitat[i] = 0;
    }
}

int exista_drum(GRAF *graf, int numar_restaurante, int start, int sfarsit) {
    STIVA *stiva = creaza_stiva(2 * numar_restaurante);

    reseteaza_vizitate(graf, numar_restaurante);
    parcurge_DFS(graf, stiva, start);

    int rezultat = graf->vizitat[sfarsit]; // 1 dacă există drum, 0 altfel

    free(stiva->elemente);
    free(stiva);

    return rezultat;
}


// ======================== Main ========================

int main() {
    int numar_restaurante, numar_drumuri;
    int sursa, destinatie;

    printf("Cate restaurante are graful? ");
    scanf("%d", &numar_restaurante);

    printf("Cate drumuri exista intre restaurante? ");
    scanf("%d", &numar_drumuri);

    GRAF *graf = creaza_graf(numar_restaurante);
    adauga_drumuri(graf, numar_drumuri, numar_restaurante);

    printf("Introduceti sursa si destinatia pentru verificare: ");
    scanf("%d%d", &sursa, &destinatie);

    if (exista_drum(graf, numar_restaurante, sursa, destinatie)) {
        printf("\nExista drum intre Restaurantul %d si Restaurantul %d\n", sursa, destinatie);
    } else {
        printf("\nNU exista drum intre Restaurantul %d si Restaurantul %d\n", sursa, destinatie);
    }

    // Curăță memoria
    for (int i = 0; i < numar_restaurante; i++) {
        NOD *temp = graf->lista_adiacenta[i];
        while (temp != NULL) {
            NOD *de_sters = temp;
            temp = temp->urmator;
            free(de_sters);
        }
    }

    free(graf->lista_adiacenta);
    free(graf->vizitat);
    free(graf);

    return 0;
}