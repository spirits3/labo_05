#include <stdio.h>
#include <stdlib.h>

int main(void) {

	int nombreBille;
	int nombreCompteur;

	do {
		printf("%s", "Entrez le nombre de billes [1000 - 30000] :");
		scanf("%4d", &nombreBille);
	    while (getchar()!='\n');

	} while(nombreBille < 1000 || nombreBille > 3000);

	do {
		printf("%s", "Entrez le nombre de rangees de compteurs [10 - 20] :");
		scanf("%2d", &nombreCompteur);
		while(getchar() != '\n');
	} while(nombreCompteur < 10 || nombreCompteur > 20);

	return EXIT_SUCCESS;
}