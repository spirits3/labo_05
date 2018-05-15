#include <stdio.h>
#include <stdlib.h>

int* genereGalton(int nombreBille, int nombreEtape, int* tab, size_t tabSize);
void* galton(int nombreBille, int nombreEtape);

void* galton(int nombreBille, int nombreEtape) {
    size_t tabSize = nombreEtape * (nombreEtape + 1) / 2;
    int* tab = (int*)calloc(tabSize, sizeof(int));
    if(!tab) return tab;

    genereGalton(nombreBille, nombreEtape, tab, tabSize);
    free(tab);

}

int* genereGalton(int nombreBille, int nombreEtape, int* tab, size_t tabSize) {
    *tab = nombreBille;
    for(size_t etape = 1; etape < nombreEtape; ++etape) {
        for(size_t decalage = 0; decalage < etape; ++decalage) {
            for(int bille = 0; bille < *(tab + (etape - 1) + decalage); ++bille) 
                ++*(tab + etape + decalage + rand()%2);
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
	} while(nombreEtape < 10 || nombreEtape > 20);


	printf("%d", galton(nombreBille, nombreEtape);
)

	return EXIT_SUCCESS;
}