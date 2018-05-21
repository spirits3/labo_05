/*
 -----------------------------------------------------------------------------------
 Laboratoire : Laboratoire no. 5 Galton
 Fichier     : main.c
 Auteur(s)   : Pierre Kohler <pierre.kohler@heig-vd.ch>
 Date        : 15.08.2018

 But         : - Simuler le probleme de la planche de Galton.
               - Le programme doit afficher les resultats conformement aux exigences
                 fixees, a savoir la comptabilisation du nombre de billes a chaque
                 etape de la planche, ainsi qu'une representation de la situation
                 finale sous forme d'histogramme.

 Remarque(s) :  

 Compilateur : gcc 6.3.0 
 -----------------------------------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>   // time() pour seed rand
#include <assert.h> 

// ----- MACROS -----
// Vidage de stdin portable
#define VIDER_STDIN         do{char c; while((c = (char)getchar()) != '\n' \
                               && c != EOF);} while(0)
// Permet la conversion d'un define entier en chaine de caracteres
// (Source : https://stackoverflow.com/questions/5459868 )
#define STR_AUX(x)          #x
#define STR(x)              STR_AUX(x)

// ----- CONSTANTES ENTIERES (ou CHAR) -----
#define NBR_BILLE_MIN       1000
#define NBR_BILLE_MAX       30000
#define NBR_ETAPE_MIN       10
#define NBR_ETAPE_MAX       20
#define HISTO_HAUTEUR_MAX   15
#define HISTO_SYMBOLE       '*'

// ----- CONSTANTES LITERALLES -----
#define TXT_NBR_BILLE       "Entrez le nombre de billes [" STR(NBR_BILLE_MIN) \
                            " - " STR(NBR_BILLE_MAX) "] : "
#define TXT_NBR_ETAPE       "Entrez le nombre de rangees de compteurs [" \
                            STR(NBR_ETAPE_MIN) " - " STR(NBR_ETAPE_MAX) "] : "
#define TXT_ERREUR_SAISIE   "Saisie incorrecte. Veuillez SVP recommencer.\n"

// ----- Prototypes des fonctions -----
// Fonctions generant les tableaux
void genereGalton(unsigned nbrBille, unsigned nbrEtape, unsigned** tab);
void genereHisto(const unsigned* tab, unsigned** histo, size_t taille);

// Fonctions d'affichage
void afficheGalton(unsigned nbrEtape, unsigned* tab, unsigned tailleCase);
void afficheHisto(const unsigned* histo, size_t tailleHisto,
                  unsigned tailleCase, char symbole);

// Fonctions utilitaires
size_t getPosAEtape(unsigned etape);
size_t getTailleGalton(unsigned nbrEtape);
unsigned getTailleCase(unsigned nbrBille); 
unsigned maxDansTab(const unsigned* tab, size_t tailleTab);
unsigned compteNbrChiffres(unsigned n);
int getIntEntre(int min, int max, const char* requete, const char* erreur);

// ----- Definitions des fonctions -----
void genereGalton(unsigned nbrBille, unsigned nbrEtape, unsigned** tab) {
    free(*tab); // par securite, si *tab n'est pas null
    
    // "Seed" de la fonction rand()
    srand((unsigned)time(NULL));
    
    // Declaration du tableau destine a contenir la simulation de la table de Galton
    size_t tailleTab = getTailleGalton(nbrEtape);
    *tab = (unsigned*)calloc(tailleTab, sizeof(unsigned));
    if(!tab) return;

    // Sommet de la planche 
    **tab = nbrBille;
    
    for(unsigned etape = 1; etape != nbrEtape; ++etape) {
        for(unsigned decalage = 0; decalage != etape; ++decalage) {
            // Simulation de chaque bille (au nombre indique dans le tableau a 
            // l'etape precedente)
            for(unsigned bille = 0; 
                bille != *(*tab + getPosAEtape(etape-1) + decalage);
                ++bille) 
                // rand()%2 fait "tomber" les billes a gauche ou a droite 
                // avec une probabilité de 1/2
                ++*(*tab + getPosAEtape(etape) + decalage + rand()%2);
        }
    }
}

void genereHisto(const unsigned* tab, unsigned** histo, size_t taille) {
    free(*histo); // par securite, si *histo n'est pas null

    // Declaration du tableau destine a contenir l'histogramme
    *histo = malloc(taille * sizeof(unsigned));    
    if(!histo) return; 
    
    // Definition du ratio de proportionnalite entre les billes et l'histogramme 
    // selon la contrainte (hauteur max de l'histogramme)
    const double RATIO = (double)(maxDansTab(tab, taille)
                                    / HISTO_HAUTEUR_MAX);

    for(size_t i = 0; i != taille; ++i) {
        // Stockage de la proportion dans le tableau, a l'entier le plus proche
        *(*histo + i) = (unsigned)(*(tab + i) / RATIO + .5);
    }
}

void afficheGalton(unsigned nbrEtape, unsigned* tab, unsigned tailleCase) {
    for(unsigned etape = 0; etape != nbrEtape; ++etape) {
        // Decallage a gauche en fonction de l'etape en cours
        // (Le "." du format permet de n'afficher aucun caractere si la taille
        // mentionnee ensuite est nulle, soit a la derniere etape.)
        printf("%*.s", (tailleCase + 1) * (nbrEtape - 1 - etape) / 2, "");
        for(unsigned decalage = 0; decalage <= etape; ++decalage) {
            printf("%*d ", tailleCase, *(tab+getPosAEtape(etape) + decalage));
        }
        printf("\n");
    }
}

void afficheHisto(const unsigned* histo, size_t tailleHisto, 
                  unsigned tailleCase, char symbole) {
    // Affichage de l'histogramme en commencant par les valeurs hautes
    for(unsigned ligne = HISTO_HAUTEUR_MAX; ligne != 0; --ligne) {
        for(size_t decalage = 0; decalage != tailleHisto; ++decalage) {
            // pour chaque colonne, afficher le caractere adequat
            printf("%*c ",
                   tailleCase, 
                   *(histo + decalage) >= ligne ? symbole : ' ');
        }
        printf("\n");
    }
}

size_t getPosAEtape(unsigned etape) {
    // somme des n entiers jusqu'a "etape" (Gauss)
    return (size_t)(etape * (etape + 1) / 2);
}

size_t getTailleGalton(unsigned nbrEtape) {
    // La taille totale corespond a la position dans le tableau de l'etape suivante
    return getPosAEtape(nbrEtape);
}

unsigned getTailleCase(unsigned nbrBille) {
    // Determiner le nombre max de chiffres pour une cellule du tableau
    unsigned tailleCase = compteNbrChiffres(nbrBille);
    // puis l'arrondir au nombre impair superieur le plus proche
    // (ceci afin de garantir un affichage agreablement justifier)
    return tailleCase += (tailleCase + 1) % 2; 
}

unsigned maxDansTab(const unsigned* tab, size_t tailleTab) {
    unsigned max = 0;

    for(size_t i = 0; i != tailleTab; ++i) {
        if(*(tab + i) > max) {
            max = *(tab + i);
        }
    }
    
    return max;
}

unsigned compteNbrChiffres(unsigned n) {
    unsigned result = 0;
    
    while(n) {
        n /= 10;
        ++result;
    }

    return result;
}

int getIntEntre(int min, int max, const char* requete, const char* erreur) {
    int result;

    do {
        printf("%s", requete);
        scanf("%d", &result);
        VIDER_STDIN;
      // si les bornes ne sont pas respectes, on affiche l'erreur 
    } while((result < min || result > max) && printf("%s", erreur));
    
    return result;
}

int main(void) {
    unsigned nbrBille = (unsigned)getIntEntre(NBR_BILLE_MIN, NBR_BILLE_MAX, 
                               TXT_NBR_BILLE, TXT_ERREUR_SAISIE);
    unsigned nbrEtape = (unsigned)getIntEntre(NBR_ETAPE_MIN, NBR_ETAPE_MAX,
                               TXT_NBR_ETAPE, TXT_ERREUR_SAISIE);
    
    unsigned *tab = NULL, *histo = NULL;
    size_t tailleHisto = (size_t)nbrEtape;

    genereGalton(nbrBille, nbrEtape, &tab);
    genereHisto(tab + getPosAEtape(nbrEtape - 1), &histo, nbrEtape);
    
    // choix d'une taille de case adaptee au nombre de billes
    // et identique pour la planche et l'histogramme
    unsigned tailleCase = getTailleCase(nbrBille);

    printf("\n");
    afficheGalton(nbrEtape, tab, tailleCase);
    printf("\n");
    afficheHisto(histo, tailleHisto, tailleCase, HISTO_SYMBOLE); 

    free(tab);
    free(histo);

    return EXIT_SUCCESS;
}
