#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define VIDER_STDIN             do{char c; while((c = (char)getchar()) != '\n' && c != EOF);} while(0)

#define HISTOGRAMME_MAX_COLONNE 15
#define NOMBRE_BILLE_MIN        1000
#define NOMBRE_BILLE_MAX        30000
#define NOMBRE_ETAPE_MIN        10
#define NOMBRE_ETAPE_MAX        20

unsigned* genereGalton(unsigned nombreBille, unsigned nombreEtape, unsigned* tab);
void galton(unsigned nombreBille, unsigned nombreEtape);
void afficheGalton(unsigned nombreEtape, unsigned* tab, unsigned tailleCase);
unsigned* genereHistogramme(unsigned* tabDerniereEtape, unsigned* histogramme, size_t tailleTab);
void afficheHistogramme(unsigned* histogramme, size_t tailleHistogramme, unsigned tailleCase);
size_t getPosAEtape(unsigned etape);
unsigned maxDansTab(unsigned* tab, size_t tailleTab);
unsigned compteNombreChiffres(unsigned n);

void galton(unsigned nombreBille, unsigned nombreEtape) {
    unsigned tailleCase = compteNombreChiffres(nombreBille);
    tailleCase += (tailleCase + 1) % 2;

    size_t tabSize = getPosAEtape(nombreEtape);
    unsigned* tab = (unsigned*)calloc(tabSize, sizeof(unsigned));
    if(!tab) return;

    size_t tailleHistogramme = (size_t)nombreEtape;
    unsigned* histogramme = malloc(tabSize * sizeof(unsigned));
    if(!histogramme) return;
    
    genereGalton(nombreBille, nombreEtape, tab);
    genereHistogramme(tab + getPosAEtape(nombreEtape - 1), histogramme, tailleHistogramme);
    
    afficheGalton(nombreEtape, tab, tailleCase);
    printf("\n");
    afficheHistogramme(histogramme, tailleHistogramme, tailleCase); 

    free(tab);
    free(histogramme);
}

unsigned* genereGalton(unsigned nombreBille, unsigned nombreEtape, unsigned* tab) {
    *tab = nombreBille;
    srand((unsigned)time(NULL));
    for(unsigned etape = 1; etape < nombreEtape; ++etape) {
        for(unsigned decalage = 0; decalage < etape; ++decalage) {
            for(unsigned bille = 0; bille < *(tab + getPosAEtape(etape-1) + decalage); ++bille) 
                ++*(tab + getPosAEtape(etape) + decalage + rand()%2);
        }
    }

    return tab;
}

void afficheGalton(unsigned nombreEtape, unsigned* tab, unsigned tailleCase) {
    for(unsigned etape = 0; etape < nombreEtape; ++etape) {
        printf("%*.s", (tailleCase + 1) * (nombreEtape - etape - 1) / 2, " ");
        for(unsigned decalage = 0; decalage <= etape; ++decalage) {
            printf("%*d ", tailleCase, *(tab+getPosAEtape(etape) + decalage));
        }
        printf("\n");
    }
}

unsigned* genereHistogramme(unsigned* tabDerniereEtape, unsigned* histogramme, size_t tailleTab) {
    if(!histogramme) return NULL;

    unsigned valeurMax = maxDansTab(tabDerniereEtape, tailleTab);
    double valeurEtoile = (double)(valeurMax) / HISTOGRAMME_MAX_COLONNE;

    for(size_t i = 0; i < tailleTab; ++i) {
        *(histogramme + i) = (unsigned)(*(tabDerniereEtape + i) / valeurEtoile + 0.5);
    }

    return histogramme;
}

void afficheHistogramme(unsigned* histogramme, size_t tailleHistogramme, unsigned tailleCase) {
    for(unsigned ligne = HISTOGRAMME_MAX_COLONNE; ligne > 0; --ligne) {
        for(size_t decalage = 0; decalage < tailleHistogramme; ++decalage) {
            if(*(histogramme + decalage) >= ligne) {
                printf("%*s* ", tailleCase - 1, "");
            } else {
                printf("%*s ", tailleCase, "");
            }
        }
        printf("\n");
    }
}

size_t getPosAEtape(unsigned etape) {
    return (size_t)(etape * (etape + 1) / 2);
}

unsigned maxDansTab(unsigned* tab, size_t tailleTab) {
    unsigned max = 0;

    for(size_t i = 0; i < tailleTab; ++i) {
        if(*(tab + i) > max) {
            max = *(tab + i);
        }
    }
    
    return max;
}

unsigned compteNombreChiffres(unsigned n) {
    unsigned result = 0;
    
    while(n) {
        n /= 10;
        ++result;
    }

    return result;
}

int main(void) {

    int nombreBille;
    int nombreEtape;

    do {
        printf("Entrez le nombre de billes [%d - %d] : ", NOMBRE_BILLE_MIN, NOMBRE_BILLE_MAX);
        scanf("%d", &nombreBille);
        VIDER_STDIN;
    } while(nombreBille < NOMBRE_BILLE_MIN || nombreBille > NOMBRE_BILLE_MAX);

    do {
        printf("Entrez le nombre de rangees de compteurs [%d - %d] : ", NOMBRE_ETAPE_MIN, NOMBRE_ETAPE_MAX);
        scanf("%d", &nombreEtape);
        VIDER_STDIN;
    } while(nombreEtape < NOMBRE_ETAPE_MIN || nombreEtape > NOMBRE_ETAPE_MAX);
    
    printf("\n");
    galton((unsigned)nombreBille, (unsigned)nombreEtape);

    return EXIT_SUCCESS;
}
