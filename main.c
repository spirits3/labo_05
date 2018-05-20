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

 Remarque(s) : - Le choix a ete fait de ne laisser que, comme unique tache de la
                 fonction main, les actions en lien avec l'utilisateur (a savoir
                 les saisies). Afin d'observer la procedure complete d'execution
                 de la simulation, il faut donc se rendre au corps de la fonction
                 galton(int, int).

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
// Fonction principale
void galton(unsigned nbrBille, unsigned nbrEtape);

// Fonctions generant les tableaux
unsigned* genereGalton(unsigned nbrBille, unsigned nbrEtape, unsigned* tab);
unsigned* genereHisto(const unsigned* tabDerniereEtape, 
                      unsigned* histo, size_t tailleTab);

// Fonctions d'affichage
void afficheGalton(unsigned nbrEtape, unsigned* tab, unsigned tailleCase);
void afficheHisto(const unsigned* histo, size_t tailleHisto,
                  unsigned tailleCase, char symbole);

// Fonctions utilitaires
size_t getPosAEtape(unsigned etape);
unsigned maxDansTab(const unsigned* tab, size_t tailleTab);
unsigned compteNbrChiffres(unsigned n);
int getIntEntre(int min, int max, const char* requete, const char* erreur);

// ----- Definitions des fonctions -----
void galton(unsigned nbrBille, unsigned nbrEtape) {
    // "Seed" de la fonction rand()
    srand((unsigned)time(NULL));

    // Declaration du tableau destine a contenir la simulation de la table de Galton
    // La taille totale correspond a la position dans le tableau de l'etape suivante
    size_t tailleTab = getPosAEtape(nbrEtape);
    unsigned* tab = (unsigned*)calloc(tailleTab, sizeof(unsigned));
    assert(tab); // SI tab == NULL => ASSERT ERROR

    // Declaration du tableau destine a contenir l'histogramme
    size_t tailleHisto = (size_t)nbrEtape;
    unsigned* histo = malloc(tailleTab * sizeof(unsigned));
    assert(histo); // SI histo == NULL => ASSERT ERROR
    
    // Determiner le nombre max de chiffre pour une cellule du tableau
    unsigned tailleCase = compteNbrChiffres(nbrBille);
    // puis l'arrondir au nombre impair superieur le plus proche
    // (ceci afin de garantir un affichage agreablement justifier)
    tailleCase += (tailleCase + 1) % 2;

   
    // Generation du tableau simultant la table de Galton
    genereGalton(nbrBille, nbrEtape, tab);
    // Generation de l'histogramme a partir de la derniere etape de la simulation
    genereHisto(tab + getPosAEtape(nbrEtape - 1), histo, tailleHisto);
    
    // Affichages de la table et de son histogramme
    afficheGalton(nbrEtape, tab, tailleCase);
    afficheHisto(histo, tailleHisto, tailleCase, HISTO_SYMBOLE); 

    // Liberation de la memoire
    free(tab);
    free(histo);
}

unsigned* genereGalton(unsigned nbrBille, unsigned nbrEtape, unsigned* tab) {
    // Sommet de la table
    *tab = nbrBille;

    for(unsigned etape = 1; etape != nbrEtape; ++etape) {
        for(unsigned decalage = 0; decalage != etape; ++decalage) {
            // Simulation de chaque bille (au nombre indique dans la table a 
            // l'etape precedente)
            for(unsigned bille = 0; 
                bille != *(tab + getPosAEtape(etape-1) + decalage);
                ++bille) 
                // rand()%2 fait "tomber" les billes a gauche ou a droite 
                // avec une prob. de 1/2
                ++*(tab + getPosAEtape(etape) + decalage + rand()%2);
        }
    }

    return tab;
}

unsigned* genereHisto(const unsigned* tabDerniereEtape, 
                      unsigned* histo, size_t tailleTab) {
    // Definition du facteur de proportionnalite entre les billes et l'histogramme 
    // selon la contrainte
    const double FACTEUR = (double)(maxDansTab(tabDerniereEtape, tailleTab)
                                    / HISTO_HAUTEUR_MAX);

    for(size_t i = 0; i != tailleTab; ++i) {
        // Stockage de la proportion dans le tableau, a l'entier le plus proche
        *(histo + i) = (unsigned)(*(tabDerniereEtape + i) / FACTEUR + .5);
    }

    return histo;
}

void afficheGalton(unsigned nbrEtape, unsigned* tab, unsigned tailleCase) {
    printf("\n");
    for(unsigned etape = 0; etape != nbrEtape; ++etape) {
        // Decallage a gauche en fonction de l'etape en cours
        // (Le "." du format permet de n'afficher aucun caractere si la taille
        // mentionnee ensuite est nulle, soit a la derniere etape.)
        printf("%*.s", (tailleCase + 1) * (nbrEtape - 1 - etape) / 2, " ");
        for(unsigned decalage = 0; decalage <= etape; ++decalage) {
            printf("%*d ", tailleCase, *(tab+getPosAEtape(etape) + decalage));
        }
        printf("\n");
    }
}

void afficheHisto(const unsigned* histo, size_t tailleHisto, 
                  unsigned tailleCase, char symbole) {
    printf("\n");
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
        // Buffer STDIN vider dans tous les cas
        VIDER_STDIN;
      // si les bornes ne sont pas respectes, on affiche l'erreur 
    } while((result < min || result > max) && printf("%s", erreur));
      // et on recommence la saisie
    
    return result;
}

int main(void) {

    // Contexte de la simulation de la table de Galton (requetes a l'utilisateur)
    int nbrBille = getIntEntre(NBR_BILLE_MIN, NBR_BILLE_MAX, 
                               TXT_NBR_BILLE, TXT_ERREUR_SAISIE);

    int nbrEtape = getIntEntre(NBR_ETAPE_MIN, NBR_ETAPE_MAX,
                               TXT_NBR_ETAPE, TXT_ERREUR_SAISIE);

    // Delegation du reste de l'execution a la fonction galton(int, int)
    galton((unsigned)nbrBille, (unsigned)nbrEtape);

    return EXIT_SUCCESS;
}
