#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<time.h>
typedef struct graf {
    int *edges;
    int *indices;
    char **words;
    int br_indices;
    int br_edges;
}Graf;
int nadji_indeks(Graf *g, char *rec) {
    if (g == NULL || g->words == NULL) return -1;
    for (int i = 0; i < g->br_indices; i++) {
        if (g->words[i] != NULL && strcmp(g->words[i], rec) == 0) {
            return i;
        }
    }
    return -1;
}
Graf *novi() {
    int n;
    scanf("%d",&n);
    while (n<=0) {
        printf("Unesite ispravan broj\n");
        scanf("%d",&n);
    }
    Graf *g = malloc(sizeof(Graf));
    if (g == NULL) {
        printf("Nije uspesno\n");
        return NULL;
    }
    g->br_indices = n;
    g->br_edges = 0;
    g->indices=calloc(n+1,sizeof(int));
    if (g->indices == NULL) {
        printf("Nije uspesno\n");
        free(g);
        return NULL;
    }
    g->words = calloc(n, sizeof(char*));
    if (g->words == NULL) {
        printf("Nije uspesno\n");
        free(g->indices);
        free(g);
        return NULL;
    }
    char pomoc[100];
    for (int i = 0; i < n; i++) {
        scanf("%s", pomoc);
        while (nadji_indeks(g, pomoc) != -1) {
            printf("Vec postoji\n");
            scanf("%s", pomoc);
        }
        g->words[i] = malloc((strlen(pomoc) + 1) * sizeof(char));
        if (g->words[i] == NULL) {
            printf("Greska pri alokaciji reci!\n");
            return NULL;
        }
        strcpy(g->words[i], pomoc);
    }
    g->edges=NULL;
    return g;

}
void dodaj_granu(Graf *g) {
    if (g==NULL) {
        printf("Graf je prazan\n");
        return;
    }
    char rec_u[50], rec_v[50];
    scanf("%s %s", rec_u, rec_v);
    int u = nadji_indeks(g, rec_u);
    int v = nadji_indeks(g, rec_v);
    while (u == -1 || v == -1) {
        printf("Ne postoji ta rec u grafu\n");
        scanf("%s %s", rec_u, rec_v);
        u = nadji_indeks(g, rec_u);
        v = nadji_indeks(g, rec_v);
    }
    int start = g->indices[u];
    int end = g->indices[u + 1];
    int poz = end;
    for (int i = start; i < end; i++) {
        if (g->edges[i] == v) {
            printf("Ta grana vec postoji\n");
            return;
        }
        if (g->edges[i] > v) {
            poz = i;
            break;
        }
    }
    int *privremeni = realloc(g->edges, (g->br_edges + 1) * sizeof(int));
    if (privremeni == NULL) {
        printf("Nije uspesno\n");
        return;
    }
    g->edges = privremeni;
    for (int i = g->br_edges; i > poz; i--) {
        g->edges[i] = g->edges[i - 1];
    }
    g->edges[poz] = v;
    g->br_edges++;
    for (int i = u + 1; i <= g->br_indices; i++) {
        g->indices[i]++;
    }
}
void ukloni_granu(Graf *g) {
    if (g==NULL) {
        printf("Graf je prazan\n");
        return;
    }
    char rec_u[50], rec_v[50];
    scanf("%s %s", rec_u, rec_v);
    int u = nadji_indeks(g, rec_u);
    int v = nadji_indeks(g, rec_v);
    while (u == -1 || v == -1) {
        printf("Ne postoji ta rec u grafu\n");
        scanf("%s %s", rec_u, rec_v);
        u = nadji_indeks(g, rec_u);
        v = nadji_indeks(g, rec_v);
    }
    int start = g->indices[u];
    int end = g->indices[u + 1];
    int poz = -1;
    for (int i = start; i < end; i++) {
        if (g->edges[i] == v) {
            poz=i;
            break;
        }
    }
    if (poz == -1) {
        printf("Ne postoji ta grana\n");
        return;
    }
    for (int i=poz; i<g->br_edges-1; i++) {
        g->edges[i] = g->edges[i+1];
    }
    g->br_edges--;
    if (g->br_edges == 0) {
        free(g->edges);
        g->edges = NULL;
    }
    else {
        int *tmp=realloc(g->edges, (g->br_edges) * sizeof(int));
        if (tmp != NULL) {
            g->edges = tmp;
        }
    }
    for (int i=u + 1; i <= g->br_indices; i++) {
        g->indices[i]--;
    }
}
void dodaj_cvor(Graf *g) {
    if (g==NULL) {
        printf("Graf je prazan\n");
        return;
    }
    char pomoc[50];
    scanf("%s", pomoc);
    while (nadji_indeks(g, pomoc) != -1) {
        printf("Vec postoji\n");
        scanf("%s", pomoc);
    }
    char **tmp = realloc(g->words, (g->br_indices + 1) * sizeof(char*));
    if (tmp == NULL) {
        printf("Nije uspesno\n");
        return;
    }
    g->words = tmp;
    g->words[g->br_indices] = malloc((strlen(pomoc) + 1) * sizeof(char));
    if (g->words[g->br_indices] == NULL) {
        printf("Nije uspesno\n");
        return;
    }
    strcpy(g->words[g->br_indices], pomoc);
    int *tmp_i = realloc(g->indices, (g->br_indices + 2) * sizeof(int));
    if (tmp_i == NULL) {
        printf("Nije uspesno\n");
        return;
    }
    g->indices = tmp_i;
    g->br_indices++;
    g->indices[g->br_indices] = g->indices[g->br_indices - 1];
}
void ukloni_cvor(Graf *g) {
    if (g==NULL || g->br_indices == 0) {
        printf("Graf je prazan\n");
        return;
    }
    char pomoc[50];
    scanf("%s", pomoc);
    int u = nadji_indeks(g, pomoc);
    while (u == -1) {
        printf("Ne postoji u grafu\n");
        scanf("%s", pomoc);
        u = nadji_indeks(g, pomoc);
    }
    int start = g->indices[u];
    int end = g->indices[u + 1];
    int kon = end - start;
    if (kon > 0) {
        for (int i = start; i < g->br_edges - kon; i++) {
            g->edges[i] = g->edges[i + kon];
        }
        g->br_edges -= kon;
    }
    for (int i = u + 1; i <= g->br_indices; i++) {
        g->indices[i] -= kon;
    }
    int i=0;
    while (i < g->br_edges) {
        if (g->edges[i] == u) {
            for (int j = i; j < g->br_edges - 1; j++) {
                g->edges[j] = g->edges[j + 1];
            }
            g->br_edges--;
            for (int k = 0; k <= g->br_indices; k++) {
                if (g->indices[k] > i) g->indices[k]--;
            }
        } else {
            if (g->edges[i] > u) {
                g->edges[i]--;
            }
            i++;
        }
    }
    if (g->br_edges == 0) {
        free(g->edges);
        g->edges = NULL;
    }
    else {
        int *tmp_e = realloc(g->edges, g->br_edges * sizeof(int));
        if (tmp_e != NULL) {
            g->edges = tmp_e;
        }
    }
    for (int k = u + 1; k < g->br_indices; k++) {
        g->indices[k] = g->indices[k + 1];
    }
    free(g->words[u]);
    for (int k = u; k < g->br_indices - 1; k++) {
        g->words[k] = g->words[k + 1];
    }
    g->br_indices--;
    if (g->br_indices == 0) {
        free(g->words);
        g->words = NULL;
    } else {
        char **tmp_w = realloc(g->words, g->br_indices * sizeof(char*));
        if (tmp_w != NULL) {
            g->words = tmp_w;
        }
    }
    int *tmp_i = realloc(g->indices, (g->br_indices + 1) * sizeof(int));
    if (tmp_i != NULL) {
        g->indices = tmp_i;
    }

}
void ispisi(Graf *g) {
    if (g == NULL || g->br_indices == 0) {
        printf("Graf je prazan\n");
        return;
    }
    printf("\n--- REPREZENTACIJA GRAFA ---\n");
    printf("Niz indices[]: ");
    for (int i = 0; i <= g->br_indices; i++) {
        printf("%d ", g->indices[i]);
    }
    printf("\n");
    printf("Niz edges[]:   ");
    if (g->br_edges == 0) {
        printf("Prazan");
    } else {
        for (int i = 0; i < g->br_edges; i++) {
            printf("%d ", g->edges[i]);
        }
    }
    printf("\n---Indeksi i reci---\n");
    for (int i=0; i < g->br_indices; i++) {
        printf("%d: %s\n", i, g->words[i]);
    }
}
void obrisi(Graf *g) {
    if (!g) return;
    if (g->words) {
        for (int i = 0; i < g->br_indices; i++) {
            if (g->words[i]) {
                free(g->words[i]);
            }
        }
        free(g->words);
    }
    if (g->edges) {
        free(g->edges);
    }
    if (g->indices) {
        free(g->indices);
    }
    free(g);
}
int kaladont_veza(const char *rec1, const char *rec2) {
    int duz1 = strlen(rec1);
    int duz2 = strlen(rec2);
    char kraj1 = rec1[duz1 - 2];
    char kraj2 = rec1[duz1 - 1];
    if (rec2[0] == kraj1 && rec2[1] == kraj2) {
        return 1;
    }
    return 0;
}
Graf* ucitaj_iz_datoteke(const char *ime_fajla) {
    FILE *f = fopen(ime_fajla, "r");
    if (f == NULL) {
        printf("Greska\n");
        return NULL;
    }
    int n;
    if (fscanf(f, "%d", &n) != 1 || n <= 0) {
        printf("Nema ili nije dobar broj.\n");
        fclose(f);
        return NULL;
    }
    Graf *g = malloc(sizeof(Graf));
    if (g == NULL) {
        fclose(f);
        return NULL;
    }
    g->br_indices = n;
    g->br_edges = 0;
    g->indices = calloc(n + 1, sizeof(int));
    g->words = calloc(n, sizeof(char*));
    g->edges = NULL;
    if (g->indices == NULL || g->words == NULL) {
        printf("Greska\n");
        fclose(f);
        free(g->indices);
        free(g->words);
        free(g);
        return NULL;
    }
    char pomoc[100];
    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%s", pomoc) != 1) {
            printf("Greska\n");
            g->br_indices = i;
            obrisi(g);
            fclose(f);
            return NULL;
        }

        g->words[i] = malloc((strlen(pomoc) + 1) * sizeof(char));
        if (g->words[i] == NULL) {
            g->br_indices = i;
            obrisi(g);
            fclose(f);
            return NULL;
        }
        strcpy(g->words[i], pomoc);
    }
    fclose(f);
    for (int i = 0; i < g->br_indices; i++) {
        int brojac = 0;
        for (int j = 0; j < g->br_indices; j++) {
            if (i == j) continue;
            if (kaladont_veza(g->words[i], g->words[j])) {
                brojac++;
            }
        }
        g->indices[i + 1] = brojac;
    }
    for (int i = 0; i < g->br_indices; i++) {
        g->indices[i + 1] += g->indices[i];
    }

    g->br_edges = g->indices[g->br_indices];
    if (g->br_edges > 0) {
        g->edges = malloc(g->br_edges * sizeof(int));
        if (g->edges == NULL) {
            obrisi(g);
            return NULL;
        }
        int *trenutne_pozicije = calloc(g->br_indices, sizeof(int));
        for (int i = 0; i < g->br_indices; i++) {
            for (int j = 0; j < g->br_indices; j++) {
                if (i == j) continue;
                if (kaladont_veza(g->words[i], g->words[j])) {
                    int indeks = g->indices[i] + trenutne_pozicije[i];
                    g->edges[indeks] = j;
                    trenutne_pozicije[i]++;
                }
            }
        }
        free(trenutne_pozicije);
    }

    return g;
}
void korisnost(Graf *g) {
    if (g == NULL || g->br_indices == 0) {
        printf("Graf je prazan.\n");
        return;
    }
    char rec[50];
    scanf("%s", rec);
    int ind = nadji_indeks(g, rec);
    while (ind == -1) {
        printf("Rec ne postoji u recniku!\n");
        scanf("%s", rec);
        ind = nadji_indeks(g, rec);
    }
    int *posecen = calloc(g->br_indices, sizeof(int));
    int *red = malloc(g->br_indices * sizeof(int));
    if (posecen == NULL || red == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        free(posecen);
        free(red);
        return;
    }
    int head = 0;
    int tail = 0;
    red[tail++] = ind;
    posecen[ind] = 1;
    while (head < tail) {
        int trenutni = red[head++];
        int start = g->indices[trenutni];
        int end = g->indices[trenutni + 1];
        for (int i = start; i < end; i++) {
            int sused = g->edges[i];
            if (!posecen[sused]) {
                posecen[sused] = 1;
                red[tail++] = sused;
            }
        }
    }
    int kor = 0;
    for (int i = 0; i < g->br_indices; i++) {
        if (posecen[i] == 1 && i != ind) {
            kor++;
        }
    }
    printf("Korisnost reci '%s' je %d\n", rec, kor);
    free(posecen);
    free(red);
}
void efikasnost(Graf *g) {
    if (g == NULL || g->br_indices == 0) {
        printf("Graf je prazan.\n");
        return;
    }
    char rec[50];
    scanf("%s", rec);
    int ind = nadji_indeks(g, rec);
    while (ind == -1) {
        printf("Rec ne postoji u recniku!\n");
        scanf("%s", rec);
        ind = nadji_indeks(g, rec);
    }
    int *udaljenost = malloc(g->br_indices * sizeof(int));
    int *red = malloc(g->br_indices * sizeof(int));
    if (udaljenost == NULL || red == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        free(udaljenost);
        free(red);
        return;
    }
    for (int i = 0; i < g->br_indices; i++) {
        udaljenost[i] = -1;
    }

    int head = 0;
    int tail = 0;
    red[tail++] = ind;
    udaljenost[ind] = 0;
    int maks_udaljenost = 0;
    while (head < tail) {
        int trenutni = red[head++];
        if (udaljenost[trenutni] > maks_udaljenost) {
            maks_udaljenost = udaljenost[trenutni];
        }
        int start = g->indices[trenutni];
        int end = g->indices[trenutni + 1];
        for (int i = start; i < end; i++) {
            int sused = g->edges[i];
            if (udaljenost[sused] == -1) {
                udaljenost[sused] = udaljenost[trenutni] + 1;
                red[tail++] = sused;
            }
        }
    }
    printf("Efikasnost reci '%s' je %d\n", rec, maks_udaljenost);
    free(udaljenost);
    free(red);
}
void opasnost(Graf *g) {
    if (g == NULL || g->br_indices == 0) {
        printf("Graf je prazan.\n");
        return;
    }
    char rec[50];
    scanf("%s", rec);
    int ind = nadji_indeks(g, rec);
    while (ind == -1) {
        printf("Rec ne postoji u recniku!\n");
        scanf("%s", rec);
        ind = nadji_indeks(g, rec);
    }
    if (g->indices[ind] == g->indices[ind + 1]) {
        printf("Opasnost reci '%s' je 1\n", rec);
        return;
    }
    int *udaljenost = malloc(g->br_indices * sizeof(int));
    int *red = malloc(g->br_indices * sizeof(int));
    if (udaljenost == NULL || red == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        free(udaljenost);
        free(red);
        return;
    }
    for (int i = 0; i < g->br_indices; i++) {
        udaljenost[i] = -1;
    }
    int head = 0;
    int tail = 0;
    red[tail++] = ind;
    udaljenost[ind] = 0;

    int opa = -1;

    while (head < tail) {
        int trenutni = red[head++];
        if (g->indices[trenutni] == g->indices[trenutni + 1]) {
            opa = udaljenost[trenutni] + 1;
            break;
        }
        int start = g->indices[trenutni];
        int end = g->indices[trenutni + 1];
        for (int i = start; i < end; i++) {
            int sused = g->edges[i];
            if (udaljenost[sused] == -1) {
                udaljenost[sused] = udaljenost[trenutni] + 1;
                red[tail++] = sused;
            }
        }
    }
    if (opa == -1) {
        printf("Iz ove reci nije moguce doci do reci bez izlaznih grana.\n");
    } else {
        printf("Opasnost reci '%s' je %d\n", rec, opa);
    }
    free(udaljenost);
    free(red);
}
int pripada_recniku(Graf *g, const char *rec) {
    if (nadji_indeks(g, rec) != -1) {
        return 1;
    }
    return 0;
}
int rack(Graf *g, int start_ind) {
    int *posecen = calloc(g->br_indices, sizeof(int));
    int *red = malloc(g->br_indices * sizeof(int));
    int head = 0, tail = 0;

    red[tail++] = start_ind;
    posecen[start_ind] = 1;

    while (head < tail) {
        int trenutni = red[head++];
        for (int i = g->indices[trenutni]; i < g->indices[trenutni + 1]; i++) {
            int sused = g->edges[i];
            if (!posecen[sused]) {
                posecen[sused] = 1;
                red[tail++] = sused;
            }
        }
    }
    int kor = 0;
    for (int i = 0; i < g->br_indices; i++) {
        if (posecen[i] == 1 && i != start_ind) kor++;
    }
    free(posecen); free(red);
    return kor;
}

int race(Graf *g, int start_ind) {
    int *udaljenost = malloc(g->br_indices * sizeof(int));
    int *red = malloc(g->br_indices * sizeof(int));
    for (int i = 0; i < g->br_indices; i++) udaljenost[i] = -1;

    int head = 0, tail = 0;
    red[tail++] = start_ind;
    udaljenost[start_ind] = 0;
    int maks_udaljenost = 0;

    while (head < tail) {
        int trenutni = red[head++];
        if (udaljenost[trenutni] > maks_udaljenost) maks_udaljenost = udaljenost[trenutni];
        for (int i = g->indices[trenutni]; i < g->indices[trenutni + 1]; i++) {
            int sused = g->edges[i];
            if (udaljenost[sused] == -1) {
                udaljenost[sused] = udaljenost[trenutni] + 1;
                red[tail++] = sused;
            }
        }
    }
    free(udaljenost); free(red);
    return maks_udaljenost;
}

int raco(Graf *g, int start_ind) {
    if (g->indices[start_ind] == g->indices[start_ind + 1]) return 1;

    int *udaljenost = malloc(g->br_indices * sizeof(int));
    int *red = malloc(g->br_indices * sizeof(int));
    for (int i = 0; i < g->br_indices; i++) udaljenost[i] = -1;

    int head = 0, tail = 0;
    red[tail++] = start_ind;
    udaljenost[start_ind] = 0;
    int opa = -1;

    while (head < tail) {
        int trenutni = red[head++];
        if (g->indices[trenutni] == g->indices[trenutni + 1]) {
            opa = udaljenost[trenutni] + 1;
            break;
        }
        for (int i = g->indices[trenutni]; i < g->indices[trenutni + 1]; i++) {
            int sused = g->edges[i];
            if (udaljenost[sused] == -1) {
                udaljenost[sused] = udaljenost[trenutni] + 1;
                red[tail++] = sused;
            }
        }
    }
    free(udaljenost); free(red);
    return opa;
}
int validnost(Graf *g, const char *rec, int *iskorisceno, int trenutno) {
    if (!pripada_recniku(g, rec)) {
        return 1;
    }
    int ind = nadji_indeks(g, rec);
    if (iskorisceno[ind]) {
        return 2;
    }
    int valid = 0;
    int start = g->indices[trenutno];
    int end = g->indices[trenutno + 1];
    for (int i = start; i < end; i++) {
        if (g->edges[i] == ind) {
            valid = 1;
            break;
        }
    }
    if (!valid) {
        return 3;
    }
    return 0;
}
void simulacija_igre(Graf *g) {
    if (g == NULL || g->br_indices == 0) {
        printf("Graf je prazan.\n");
        return;
    }
    int *iskorisceno = calloc(g->br_indices, sizeof(int));
    if (iskorisceno == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return;
    }
    srand(time(NULL));
    int trenutna = rand() % g->br_indices;
    iskorisceno[trenutna] = 1;
    printf("\n=== IGRA KALADONT ===\n");
    printf("Za POMOC u bilo kom trenutku unesi znak ?\n");
    printf("Ako zelite da izadjete unesite 0\n");
    printf("Ako zelite ispis unesite I\n");
    printf("Racunar zadaje prvu rec: >>> %s <<<\n", g->words[trenutna]);
    while (1) {
        char unos[50];
        printf("\nTvoj potez: ");
        scanf("%s", unos);
        if (strcmp(unos, "0") == 0) {
            printf("Hvala sto ste igrali");
            break;
        }
        if (strcmp(unos, "I") == 0) {
            ispisi(g);
            continue;
        }
        if (strcmp(unos, "?") == 0) {
            char pomoc[50];
            printf("Unesite rec");
            scanf("%s", pomoc);
            int status_pomoci = validnost(g, pomoc, iskorisceno, trenutna);
            printf("\n--- OSOBINE RECI '%s' ---\n", pomoc);
            if (status_pomoci == 1) {
                printf("Ne pripada recniku\n");
            } else {
                if (status_pomoci == 0) printf("Dozvoljeno\n");
                else if (status_pomoci == 2) printf("Vec iskorisceno\n");
                else if (status_pomoci == 3) printf("Ne moze da nastavi\n");
                int p_ind = nadji_indeks(g, pomoc);
                printf("- Korisnost: %d\n", rack(g, p_ind));
                printf("- Efikasnost: %d\n", race(g, p_ind));

                int op = raco(g, p_ind);
                if (op == -1) printf("- Beskonacan ciklus\n");
                else printf("- Opasnost: %d\n", op);
            }
            printf("-------------------------\n");
            continue;
        }
        int status = validnost(g, unos, iskorisceno, trenutna);
        if (status == 1) {
            printf("Rec ne pripada recniku igre\n");
            continue;
        } else if (status == 2) {
            printf("Rec je vec iskoriscena u ovoj partiji\n");
            continue;
        } else if (status == 3) {
            printf("Rec '%s' ne nastavlja rec '%s'\n", unos, g->words[trenutna]);
            continue;
        }
        int ind = nadji_indeks(g, unos);
        trenutna = ind;
        iskorisceno[trenutna] = 1;

        int opcija = 0;
        int start = g->indices[trenutna];
        int end = g->indices[trenutna + 1];

        for (int i = start; i < end; i++) {
            if (!iskorisceno[g->edges[i]]) {
                opcija++;
            }
        }

        if (opcija == 0) {
            printf("\nPobeda!\n");
            break;
        }

        int izbor = rand() % opcija;
        int trenutni_brojac = 0;

        for (int i = start; i < end; i++) {
            int sused = g->edges[i];
            if (!iskorisceno[sused]) {
                if (trenutni_brojac == izbor) {
                    trenutna = sused;
                    break;
                }
                trenutni_brojac++;
            }
        }

        iskorisceno[trenutna] = 1;
        printf("Racunar igra: %s \n", g->words[trenutna]);

        int igrac_opcija = 0;
        start = g->indices[trenutna];
        end = g->indices[trenutna+ 1];

        for (int i = start; i < end; i++) {
            if (!iskorisceno[g->edges[i]]) {
                igrac_opcija = 1;
                break;
            }
        }

        if (!igrac_opcija) {
            printf("\nRacunar je pobedio\n");
            break;
        }
    }
    free(iskorisceno);
}
int main(void) {
    Graf *g = NULL;
    int izbor = -1;
    int izbor1 = -1;

    while (1) {
        printf("\n======= GLAVNI MENI =======\n"
               "1. Zadatak 1\n"
               "2. Zadatak 2\n"
               "0. Izlaz\n"
               "===========================\n"
               "Unesite izbor: ");

        if (scanf("%d", &izbor) != 1) {
            printf("Neispravan unos.\n");
            while (getchar() != '\n');
            continue;
        }
        while (getchar() != '\n');
        if (izbor == 1) {
            while (1) {
                printf("\n======= PODMENI ZADATAK 1 =======\n");
                printf("1. Kreiraj novi graf\n");
                printf("2. Dodaj cvor (rec)\n");
                printf("3. Ukloni cvor (rec)\n");
                printf("4. Dodaj granu\n");
                printf("5. Ukloni granu\n");
                printf("6. Ispisi graf\n");
                printf("7. Obrisi graf iz memorije\n");
                printf("0. Povratak u glavni meni\n");
                printf("===========================\n");
                printf("Unesite vas izbor: ");

                if (scanf("%d", &izbor1) != 1) {
                    printf("Neispravan unos.\n");
                    while (getchar() != '\n');
                    continue;
                }
                while (getchar() != '\n');

                if (izbor1 == 1) {
                    if (g != NULL) {
                        printf("Graf vec postoji.\n");
                    } else {
                        g = novi();
                    }
                } else if (izbor1 == 2) {
                    if (g != NULL) {
                        dodaj_cvor(g);
                    } else {
                        printf("Graf ne postoji.\n");
                    }
                } else if (izbor1 == 3) {
                    if (g != NULL) {
                        ukloni_cvor(g);
                    } else {
                        printf("Graf ne postoji!\n");
                    }
                } else if (izbor1 == 4) {
                    if (g != NULL) {
                        printf("Unesite rec_u i rec_v za novu granu: ");
                        dodaj_granu(g);
                    } else {
                        printf("Graf ne postoji!\n");
                    }
                } else if (izbor1 == 5) {
                    if (g != NULL) {
                        printf("Unesite rec_u i rec_v grane koju zelite da uklonite: ");
                        ukloni_granu(g);
                    } else {
                        printf("Graf ne postoji!\n");
                    }
                } else if (izbor1 == 6) {
                    if (g != NULL) {
                        ispisi(g);
                    }
                    else {
                        printf("Graf je prazan/ne postoji!\n");
                    }
                } else if (izbor1 == 7) {
                    if (g != NULL) {
                        obrisi(g);
                        g = NULL;
                    } else {
                        printf("Graf je vec prazan.\n");
                    }
                } else if (izbor1 == 0) {
                    printf("Povratak u glavni meni.\n");
                    break;
                } else {
                    printf("Nepostojeca opcija! Unesite broj od 0 do 7.\n");
                }
            }
        }
        else if (izbor == 2) {
            if (g != NULL) {
                obrisi(g);
                g = NULL;
            }
            char putanja[100];
            printf("Unesi naziv datoteke");
            scanf("%s", putanja);
            g = ucitaj_iz_datoteke(putanja);
            if (g == NULL) {
                printf("Ucitavanje grafa iz datoteke nije uspelo.\n");
                continue;
            }
            simulacija_igre(g);
        }
        else if (izbor == 0) {
            if (g != NULL) {
                obrisi(g);
                g = NULL;
            }
            printf("Kraj programa.\n");
            break;
        }
        else {
            printf("\nNeispravna opcija.\n");
        }
    }
    return 0;
}
