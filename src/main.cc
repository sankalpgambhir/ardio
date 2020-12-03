// main.cc

#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

#define mop(x, y) x*y
#define SIZE 50

int correlation(int*, int*);
int crosscorrelation(int*, int*, int = 0);
std::vector<float> checkcorr(int*, std::vector<float>);

std::vector<float> freq = {0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8};
const float corr_threshold = 0.5;

int main(){
	// construct sample arrays
	int f[SIZE], g[SIZE];

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

std::vector<float> checkcorr(int* f, std::vector<float> wlist){

	if(wlist.size() == 0) return wlist;

	float maxcorr = -1;

	auto g_gen = [wlist](int x){
					float sum = 0;
					for(auto w : wlist){
						sum += sin(w*x);
					}
					return sum;
				};

	auto g = new int[SIZE];
	
	for(int i = 0; i < SIZE; i++){
		g[i] = int(7 * g_gen(i))/float(wlist.size());
	}

	auto norm_coeff = 1/sqrt(correlation(f, f) * correlation(g, g));

	for(int i = 0; i < SIZE; i++){
		auto corr = crosscorrelation(f, g, i) * norm_coeff;
		maxcorr = maxcorr > corr ? maxcorr : corr;
	}

	// clean memory just in case it isn't deallocated 
	// before recursion else we run over quota
	delete[] g; 

	if(maxcorr < corr_threshold) return {};

	if(wlist.size() == 1) return wlist;

	auto wl = checkcorr(f, std::vector<float>(wlist.begin(), wlist.begin() + (wlist.size()/2)));
	auto wr = checkcorr(f, std::vector<float>(wlist.begin() + (wlist.size()/2) + 1, wlist.end()));

	std::move(wr.begin(), wr.end(), std::back_inserter(wl));

	return wl;
}