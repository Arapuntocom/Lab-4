#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main(){

	float limInfReal = -1;	// x en lim inf izq
	float limInfComp = -1;	// y en lim inf izq
	float limSupReal = 1;	// x en lim sup derecho
	float limSupComp = 1;	// y en lim sup derecho
	
	float muestreo = 0.001;	// muestreo

	float cantFilas = ((fabs(limSupComp) + fabs(limInfComp))/muestreo ) +1;
	
	float cantFilasB = ((limSupComp - limInfComp)/muestreo)+1;
	

	printf("cantFilas %g\n",cantFilas);
	printf("cant B %g\n",cantFilasB);
	
	
	return 0;

}
