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
    char* archivo;

    while ((option = getopt(argc, argv, "p:a:b:c:d:s:f:")) != -1) {
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
                archivo = optarg;
                printf("archivo = %s\n",archivo);
                break;
        }
    }
	
 
    //######### Validando datos de entrada.	
	if(limInfComp > limSupComp || limInfReal > limSupReal){
		printf("los limites superiores deben ser superiores que los limites inferiores\n");
		exit(-1);
	}
	if(muestreo<=0.0 || depth <= 0){
		printf("la cantidad de muestreo e iteraciones deben ser positivas.\n");
		exit(-1);
	}
	if( muestreo >= (limSupComp-limInfComp) ||  muestreo >= (limSupReal-limInfReal)){
		printf("la cantidad de muestreo invalida, no permite generar un plano.\n");
		exit(-1);
	}

  
	//############  Calculando cantidad de Filas y Columnas	
	float tamFilas = ((limSupComp - limInfComp)/muestreo)+1.0;		
	float tamCol = ((limSupReal - limInfReal)/muestreo)+1.0;
	float tot = tamFilas*tamCol; 
	printf("grilla de [%g]x[%g]\n", tamFilas, tamCol);
	
	
	//#########  Generando grilla del plano para escribir los resultados
	float **grilla;  
    grilla = (float**) malloc(tamFilas * sizeof (float*));
    int i = 0, j = 0;
    for (i = 0; i < tamFilas; i++) {
        grilla[i] = (float*) malloc(tamCol * sizeof (float));
    }

	
	// #### estableciendo valores iniciales para el calculo.
    float cy = limInfComp;		// cx para la parte real del punto c.
	float cx = limInfReal;		// cy para la parte imaginaria del punto c.
	
	float zx, zy; 	// para representar el numero complejo dentro de la iteracion.
	int n; 			// contador de iteraciones
	
	i = 0;			// para avanzar en las filas de la grilla.
	j = 0;			// para avanzar en las columnas de la grilla.
	
	while(cy <= limSupComp){
		cx = limInfReal;
		j=0;	
		while(cx <= limSupReal){
			n=0;
			zx = 0;
			zy = 0;
			while(n<depth){
				n++;
				zx = zx*zx - zy*zy + cx;
				zy = 2*zx*zy + cy;
				if( fabs(zx) > 2){
					break;
				}
			}
			grilla[i][j] = log10(n) +1;		
			j++;
			cx = cx + muestreo;			
		}
		i++;
		cy = cy + muestreo;	
	}
	
	//######### Escribiendo archivo
	FILE *f1;
	f1 = fopen(archivo, "wb");	
	
	for(j =0; j<tamCol; j++){
		for(i =0; i< tamFilas; i++){			
			fwrite(&grilla[i][j], sizeof(float), 1, f1);
		}
	}

	fclose(f1);

    printf("listo!\n\n");
    return 0;
}



















