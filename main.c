#include <stdio.h>
#include <stdlib.h>

int main(void) {

	int nombreBille;

	printf("%s", "Entrez le nombre de billes [1000 - 30000] :");

	do {
		scanf("%d", &nombreBille);		
	} while(nombreBille <= 1000 || nombreBille >=3000)

	return 0;
}