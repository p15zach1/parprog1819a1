#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void get_walltime(double *wct){
	struct timeval tp;
	gettimeofday(& tp, NULL);
	*wct = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(){
	float *a, *b, *c, *pa, *pb, *pc, sum;
	int i, j, k;
	double ts, te, mflops;

	a = (float*) malloc(N*N*sizeof(float));
	if(a==NULL){
		printf("Error with memory allocation\n");
		exit(1);
	}
	b = (float*) malloc(N*N*sizeof(float));
	if(b==NULL){
		printf("Error with memory allocation\n");
		free(a);
		exit(1);
	}
	c = (float*) malloc(N*N*sizeof(float));
	if(c==NULL){
		printf("Error with memory allocation\n");
		free(a);
		free(b);
		exit(1);
	}

	for(i=0; i<N*N; i++){
		a[i] = 2.0;
		b[i] = 3.0;
		c[i] = 10.0;
	}

	get_walltime(&ts);
	pc = c;
	for(k=0; k<N; k++){
		pb = b;
		for(j=0; j<N; j++){
			pa = a + k*N;
			sum = 0.0;
			for(i=0; i<N; i++){
				sum += (*pa)*(*pb);
				pa++;
				pb++;
			}
			*pc = sum;
			pc++;
		}
	}
	get_walltime(&te);

	for(i=0; i<N*N; i++){
		if(c[i]!=(2.0*3.0*N)){
			printf("ERROR\n");
			exit(1);
		}
	}
	
	mflops = (2.0*3.0*N*N)/((te-ts)*1e6);
	printf("MFLOP/s = %f\n", mflops);

	free(a);
	free(b);
	free(c);

	return 0;
}
