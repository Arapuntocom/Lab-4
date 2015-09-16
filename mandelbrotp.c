#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <time.h>

typedef struct {
	int
		id,
		filaInicial,	// fila que inicia escribiendo
		cantFilas;	// cantidad de filas que debe escribir		
}mandel;

//#### Datos Globales
float depth, limInfReal, limInfComp, limSupReal, limSupComp, muestreo; 	// para datos de entrada getopt

float **grilla;	// grilla del plano para guardar los resultados de cada punto c.


void *maldelbrot(void*argTh){
	
	mandel *param = (mandel*) argTh;
	printf("hebra %d, escribo -> %d filas, partiendo en la -> %d\n", param->id, param->cantFilas, param->filaInicial);
	
	float cy = limInfComp+muestreo*param->filaInicial;
	float cx = limInfReal;
	int contadorFilas;
	
	float zx,zy; 	// para representar el numero complejo dentro de la iteracion.
	int n; 			// contador de iteraciones
	int i,j; 			// para escribir dentro de la grilla.
	
	i = param->filaInicial;
	j = 0;
		
	float valor =0;
	
	contadorFilas=0;
	while(contadorFilas <  param->cantFilas ){
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
			valor = log10(n) +1;
			grilla[i][j] = valor;
			
			j++;
			cx = cx + muestreo;			
		}
		i++;
		cy = cy + muestreo;		
		contadorFilas++;
	}

	
	printf("se despide la hebra %d\n",param->id);
	pthread_exit(NULL);
}

int main(int argc, char*argv[]){
	
	//##########  GETOPT, exceptuando datos globales
	int option = 0; 		//opción para getopt
	char *archivo;
	int cantThreads = 0;	// cantidad de threads.
	
	
	while ( (option = getopt(argc, argv, "p:a:b:c:d:s:f:t:")) != -1){
		switch(option){
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
				printf("archivo = %s\n", archivo);
				break;
			case 't':
				cantThreads = atoi(optarg);
				printf("threads = %d\n", cantThreads);
				break;
		}
	}
	
	
	//######### Validando datos de entrada.	
	if(limInfComp > limSupComp || limInfReal > limSupReal){
		printf("los limites superiores deben ser superiores que los limites inferiores\n");
		exit(-1);
	}
	if(muestreo<=0.0 || depth <= 0 || cantThreads <=0){
		printf("la cantidad de muestreo, iteraciones y threads deben ser positivas.\n");
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
	printf("grilla de [ %g ]x[ %g ]\n", tamFilas, tamCol);
	
	//#########  Generando grilla del plano para escribir los resultados
	//float **grilla;  //declarada como dato global.
    grilla = (float**) malloc(tamFilas * sizeof (float*));
    int i = 0, j = 0;
    for (i = 0; i < tamFilas; i++) {
        grilla[i] = (float*) malloc(tamCol * sizeof (float));
    }

	
	//######### Iniciando Threads
	pthread_t *threads = malloc(sizeof(pthread_t) * cantThreads);
	
	int cantMinima = tamFilas/cantThreads;
	int resto = (int)tamFilas%cantThreads;
	int filaIniAux =0;
	for(i = 0; i<cantThreads; i++){
		mandel *args = (mandel*)malloc(sizeof(mandel));
		args->id = i+1;
		args->cantFilas = cantMinima;
		if(resto>0){
			args->cantFilas = args->cantFilas +1;
			resto--;
		}
		args->filaInicial = filaIniAux;
		filaIniAux = filaIniAux+args->cantFilas;		
		pthread_create(&threads[i], NULL, &maldelbrot, args);	
	}	
	
	//#### Esperando que cada Thread finalize
	for(i=0; i < cantThreads; i++){
		pthread_join(threads[i], NULL);
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
	
	printf("listo!\n");
	return 0;
}