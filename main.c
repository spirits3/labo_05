#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* genereGalton(int nombreBille, int nombreEtape, int* tab);
void* galton(int nombreBille, int nombreEtape);
void afficheGalton(int nombreEtape, int* tab);
void afficheHistogramme(int nombreEtape, int* tab);

void* galton(int nombreBille, int nombreEtape) {
    size_t tabSize = (size_t)(nombreEtape * (nombreEtape + 1) / 2);
    int* tab = (int*)calloc(tabSize, sizeof(int));

    if(tab == NULL) {
      free(tab);
      return tab;
    }
    genereGalton(nombreBille, nombreEtape, tab);
    afficheGalton(nombreEtape, tab);
    afficheHistogramme(nombreEtape, tab);

    
    free(tab);
    return tab;
}

int* genereGalton(int nombreBille, int nombreEtape, int* tab) {
    *tab = nombreBille;
    srand((unsigned int)time(NULL));
    for(int etape = 1; etape < nombreEtape; ++etape) {
        for(int decalage = 0; decalage < etape; ++decalage) {
            for(int bille = 0; bille < *(tab + etape*(etape-1)/2 + decalage); ++bille) 
                ++*(tab + (etape+1)*etape / 2 + decalage + rand()%2);
        }
    }

    return tab;
}

void afficheGalton(int nombreEtape, int* tab) {
    int tailleCase = 5;
    for(int etape = 0; etape < nombreEtape; ++etape) {
        printf("%2d : ", etape + 1);
        for(int i = 0; i < nombreEtape - etape; ++i) {
            printf("%*s", tailleCase / 2 + 1, " ");
        }
        for(int decalage = 0; decalage <= etape; ++decalage) {
            printf("%*d ", tailleCase, *(tab+etape*(etape+1) / 2 +decalage));
        }
        printf("\n");
    }
}

<<<<<<< HEAD
void afficheHistogramme(int nombreEtape, int* tab){
    int tailleCase = 5;
    for(int etape = 0; etape < nombreEtape; ++etape) {
        printf("%2d : ", etape + 1);
        for(int i = 0; i < nombreEtape - etape; ++i) {
            printf("%*s", tailleCase / 2 + 1, " ");
        }
        for(int decalage = 0; decalage <= etape; ++decalage) {
            printf("%*d ", tailleCase, *(tab+etape*(etape+1) / 2 +decalage));
        }
        printf("\n");
    }
}

=======
>>>>>>> 77f12a3c7870931bd3131fb1553d1dbe7403993f
int main(void) {

  int nombreBille;
  int nombreEtape;

<<<<<<< HEAD
  do {
    printf("%s", "Entrez le nombre de billes [1000 - 30000] : ");
    scanf("%5d", &nombreBille);
      while (getchar()!='\n');

  } while(nombreBille < 1000 || nombreBille > 30000);

  do {
    printf("%s", "Entrez le nombre de rangees de compteurs [10 - 20] : ");
    scanf("%2d", &nombreEtape);
    while(getchar() != '\n');
  } while(nombreEtape < 10 || nombreEtape > 20);
=======
	do {
		printf("%s", "Entrez le nombre de billes [1000 - 30000] : ");
		scanf("%5d", &nombreBille);
	    while (getchar()!='\n');

	} while(nombreBille < 1000 || nombreBille > 30000);

	do {
		printf("%s", "Entrez le nombre de rangees de compteurs [10 - 20] : ");
		scanf("%2d", &nombreEtape);
		while(getchar() != '\n');
	} while(nombreEtape < 10 || nombreEtape > 20);
>>>>>>> 77f12a3c7870931bd3131fb1553d1dbe7403993f

  galton(nombreBille, nombreEtape);

<<<<<<< HEAD

  return EXIT_SUCCESS;
}
=======
	printf("%p", (void*)galton(nombreBille, nombreEtape));

	return EXIT_SUCCESS;
}
>>>>>>> 77f12a3c7870931bd3131fb1553d1dbe7403993f
