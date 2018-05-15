#include <stdio.h>
#include <stdlib.h>

int* genereGalton(int nombreBille, int nombreEtape, int* tab, size_t tabSize);
void* galton(int nombreBille, int nombreEtape);

void afficheGalton(int nombreEtape, int* tab, size_t tabSize) {
    int tailleCase = 6;
    for(int etape = 0; etape < nombreEtape; ++etape) {
        printf("%d : ", etape);
        for(int i = 0; i < nombreEtape - etape; ++i) {
            printf("%*s", tailleCase / 2 + 1, " ");
        }
        for(size_t decalage = 0; decalage <= etape; ++decalage) {
            printf("%*d ", tailleCase, *(tab+etape*(etape+1) / 2 +decalage));
        }
        printf("\n");
    }
}

void* galton(int nombreBille, int nombreEtape) {
    size_t tabSize = nombreEtape * (nombreEtape + 1) / 2;
    int* tab = (int*)calloc(tabSize, sizeof(int));
    if(!tab) return tab;

    genereGalton(nombreBille, nombreEtape, tab, tabSize);
    afficheGalton(nombreEtape, tab, tabSize);
    
    free(tab);
    return tab;
}

int* genereGalton(int nombreBille, int nombreEtape, int* tab, size_t tabSize) {
    *tab = nombreBille;
    srand(time(NULL));
    for(size_t etape = 1; etape < nombreEtape; ++etape) {
        for(size_t decalage = 0; decalage < etape; ++decalage) {
            for(int bille = 0; bille < *(tab + etape*(etape-1)/2 + decalage); ++bille) 
                ++*(tab + (etape+1)*etape / 2 + decalage + rand()%2);
        }
    }

    return tab;
}

int main(void) {

	int nombreBille;
	int nombreEtape;

	do {
		printf("%s", "Entrez le nombre de billes [1000 - 30000] :");
		scanf("%4d", &nombreBille);
	    while (getchar()!='\n');

	} while(nombreBille < 1000 || nombreBille > 3000);

	do {
		printf("%s", "Entrez le nombre de rangees de compteurs [10 - 20] :");
		scanf("%2d", &nombreEtape);
		while(getchar() != '\n');
	} while(nombreEtape < 1 || nombreEtape > 20);


	printf("%p", (void*)galton(nombreBille, nombreEtape));

	return EXIT_SUCCESS;
}
