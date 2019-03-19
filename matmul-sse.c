#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>

void get_walltime(double *wct){
	struct timeval tp;
	gettimeofday(& tp, NULL);
	*wct = (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

int main(){
	float *a, *b, *c, *product, *pc, sum;
	int i, j, k;
	double ts, te, mflops;
	__m128 *va, *vb, *vproduct;

	i = posix_memalign((void**)&a, 16, N*N*sizeof(float));
	if(i!=0){
		printf("Error with memory allocation\n");
		exit(1);
	}
	i = posix_memalign((void**)&b, 16, N*N*sizeof(float));
	if(i!=0){
		printf("Error with memory allocation\n");
		free(a);
		exit(1);
	}
	i = posix_memalign((void**)&c, 16, N*N*sizeof(float));
	if(i!=0){
		printf("Error with memory allocation\n");
		free(a);
		free(b);
		exit(1);
	}
	i = posix_memalign((void**)&product, 16, 4*sizeof(float));
	if(i!=0){
		printf("Error with memory allocation\n");
		free(a);
		free(b);
		free(c);
		exit(1);
	}

	for(i=0; i<N*N; i++){
		a[i] = 2.0;
		b[i] = 3.0;
		c[i] = 10.0;
	}

	get_walltime(&ts);
	vproduct = (__m128*)product;
	pc = c;
	for(k=0; k<N; k++){
		vb = (__m128*)b;
		for(j=0; j<N; j++){
			va = (__m128*)(a + k*N);
			sum = 0;
			for(i=0; i<N; i+=4){
				*vproduct = _mm_mul_ps(*va, *vb);
				va++;
				vb++;
				sum += product[0] + product[1] + product[2] + product[3];
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
	free(product);

	return 0;
}
