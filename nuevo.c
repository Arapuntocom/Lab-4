#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#define X 0
#define Y 1

//compilar gcc archivo.c -o salida -lm
// donde -lm es para math

int main(){
	printf("hola mundo!\n");

	FILE *f1;
	f1 = fopen("fichero.raw","wb"); //archivo de salida
	//printf("fichero abierto\n");
	
	float a = -1;	// x en lim inf izq
	float b = -1;	// y en lim inf izq
	float c = 1;	// x en lim sup derecho
	float d = 1;	// y en lim sup derecho
	
	float s = 0.001;	// muestreo
	int p = 500;	// lim de iteraciones

	int n = 0;	// contador de iteraciones
	double z[2];	
	z[X] = 0;
	z[Y] = 0;

	double g[2]; // grilla
	g[X] = a;
	g[Y] = b;

int q=1;
//	double plano[2001][2001];

	double **plano;
	int i;
	//int j;
	plano = (double**)malloc(2001*sizeof(double*));
	for(i=0;i<2001;i++){
		plano[i] = (double*)malloc(2001*sizeof(double));
	}
	printf("acaba asig mem\n");

	
	int planoX= 0;
	int planoY = 0;



	double valor;
	double aux = 0.0;
		
	while(g[X] <= c){	// for para recorrer el eje real
		while(g[Y]<= d){	// for para recorrer el eje imaginario 
			
			while(n<p){
				n++;
				z[X]= z[X]*z[X] - z[Y]*z[Y] + g[X];
				z[Y]= 2*z[X]*z[Y] + g[Y];
				aux = fabs(z[X]);
				if( aux>=2 ){
					//printf("abs(%g) = %g\n",z[X],aux);
					break;
				}
				//printf("z%d=(%g,%g)\t",n,z[X],z[Y]);
			}
			valor = log10(n) + 1;
			plano[planoX][planoY] = valor;
			//printf("\n\tvalor %d (%g,%g)=%g\n",q,g[X],g[Y],valor);
			
			n = 0;
			g[Y]=g[Y]+s;
			z[X]=0;
			z[Y]=0;
			planoY++;
			
			q++;
		}
		fwrite(&plano[planoX],sizeof(double), 2001,f1);

		g[Y] = b;		
		g[X] = g[X]+s;
		planoX++;
		//printf("avanza x\n");
	}
//fwrite(&plano[planoX],sizeof(double), 2001,f1);


	fclose(f1);
	printf("q=%d",q);
	printf("listo!\n\n");
	
	return 0;
}
