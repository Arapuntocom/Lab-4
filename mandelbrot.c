#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>

#define X 0
#define Y 1

int main(int argc, char*argv[]) {
    int option = 0;
    float depth, limInfReal, limInfComp, limSupReal, limSupComp, muestreo;
    //	char* archivo;
    char archivo[] = "archivoa.raw";

    FILE *f1;
    f1 = fopen(archivo, "wb");

    while ((option = getopt(argc, argv, "p:a:b:c:d:s:")) != -1) {
        switch (option) {
            case 'p':
                depth = atof(optarg);
                printf("depth = %f\n", depth);
                break;
            case 'a':
                limInfReal = atof(optarg);
                printf("limInfReal = %f\n", limInfReal);
                break;
            case 'b':
                limInfComp = atof(optarg);
                printf("limInfComp = %f\n", limInfComp);
                break;
            case 'c':
                limSupReal = atof(optarg);
                printf("limSupReal = %f\n", limSupReal);
                break;
            case 'd':
                limSupComp = atof(optarg);
                printf("limSupComp = %f\n", limSupComp);
                break;
            case 's':
                muestreo = atof(optarg);
                printf("muestreo = %f\n", muestreo);
                break;
            case 'f':
                //archivo = optarg;
                //printf("archivo = %s\n",archivo);
                break;
        }
    }
	
    printf("archivo = %s\n", archivo);
    /*falta validar los datos de entrada.
    limSupComp > limInfComp
    limSupReal > limInfReal
    muestreo > 0
    muestreo < (limSupComp-limInfComp)
    muestreo < (limSupReal-limInfReal)
    depth > 0
     */

  
	float cantFilas = ((limSupComp - limInfComp)/muestreo)+1;		
	float cantCol = ((limSupReal - limInfReal)/muestreo)+1;

	printf("cantFilas %g\n",cantFilas);
	printf("cant Columnas B %g\n",cantCol);

	// #### reservando memoria para la Grilla 
    float **grilla;
    grilla = (float**) malloc(cantFilas * sizeof (float*));
    int i = 0, j = 0;
    for (i = 0; i < cantFilas; i++) {
        grilla[i] = (float*) malloc(cantCol * sizeof (float));
    }
    printf("memoria grilla reservada\n");

	// #### estableciendo valores iniciales para el calculo.

    float z[2] = {0.0, 0.0}; 				// para representar el numero complejo
    float c[2] = {limInfReal, limInfComp}; 	// empezaremos a calcular desde el punto ubicado en la esq superior izq del plano.
    printf("c = (%g,%g)\n", c[X], c[Y]);
    int n = 0; 								// contador de iteraciones
    int aux1 = 0;
    int aux2 = 0;

    i = 0;		// para movernos dentro de la grilla, y señalar los puntos a escribir.
    j = 0;

	while(c[X] <= limSupReal){
		while(c[Y] <= limSupComp){
			n=0;
			z[X]=0;
			z[Y]=0;
			while(n<depth){
				n++;
				z[X] = z[X] * z[X] - z[Y]*z[Y] + c[X];
				z[Y] = 2*z[X]*z[Y] +c[Y];
				if(fabs(z[X]) > 2){
					break;
				}
			}
			grilla[i][j] = log10(n) + 1;
			fwrite(&grilla[i][j], sizeof(float), 1, f1);
			aux1++;
			c[Y]=c[Y]+muestreo;
			i++;			
		}	
		j++;
		i=0;
		c[X] = c[X] + muestreo; 
		c[Y] = limInfComp;
		aux2++;
	}

    printf("\ntodos=%d\n", aux1);
	printf("\ncool=%d\n", aux2);

    //fwrite(&grilla, sizeof (float), cantFilas*cantCol, f1);
    fclose(f1);

    printf("listo!\n\n");
    return 0;
}



















