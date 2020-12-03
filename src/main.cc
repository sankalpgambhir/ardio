// main.cc

#include <iostream>
#include <cmath>
#include <functional>

#define mop(x, y) x*y
#define SIZE 50

int correlation(int*, int*);
int crosscorrelation(int*, int*, int = 0);

int main(){
	// construct f
	int f[SIZE];

	// construct g
	int g[SIZE];

	for(int i = 0; i < SIZE; i++){
		f[i] = int(7*(sin(0.3 * i) + 4*sin(0.5 * i) + sin(0.8 * i + 0.6))); 
		g[i] = int(7*(sin(0.3 * i + 0.5) + sin(0.5 * i + 0.9) + sin(0.8 * i))); 
	}

	double corr = correlation(f, g) / sqrt(correlation(f, f) * correlation(g, g));

	for(int i = -SIZE; i < SIZE; i++){
		std::cout << float(crosscorrelation(f, g, i) / sqrt(correlation(f, f) * correlation(g, g))) << std::endl;		
	}

	//std::cout << corr << std::endl;
	return 0;
}

int correlation(int* f, int* g){
	int size = sizeof(*f) / sizeof(int);
	int sum = 0;

	for(int i = 0; i < SIZE; i++){
		sum += mop(f[i], g[i]);
	}
	return sum;
}

int crosscorrelation(int* f, int* g, int m){
	int size = sizeof(*f) / sizeof(int);
	int sum = 0;

	if(m >= 0){
		for(int i = 0; i < SIZE - m; i++){
			sum += mop(f[i], g[i+m]);
		}
		for(int i = 0; i < m; i++){
			sum += mop(f[i+SIZE-m], g[i]);
		}
	}
	else{
		m = -m;
		for(int i = 0; i < m; i++){
			sum += mop(f[i], g[i+SIZE-m]);
		}
		for(int i = m; i < SIZE; i++){
			sum += mop(f[i], g[i-m]);
		}
	}
	return sum;
}
