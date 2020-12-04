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
const float corr_threshold = 0.26;

int main(){
	// construct sample arrays
	signal f[SIZE];

	auto gen_f = [](int x){
				return (7*(sin(0.3 * x) + 4*sin(0.5 * x) + sin(0.8 * x + 0.6))/6);
				};

	for(int i = 0; i < SIZE; i += 2){
		f[i] = signal(gen_f(i), gen_f(i+1)); 
	}

	std::vector<float> flist = {0.3, 0.5, 0.8, 0.9, 0.7, 0.1};

	auto wpresent = checkcorr(f, flist);

	for(auto w : wpresent){
		printf("%f ", w);
	}

	printf("\n");

	return 0;
}

int correlation(signal* f, signal* g){
	int sum = 0;

	for(int i = 0; i < SIZE; i++){
		sum += mop(f[i], g[i]);
	}
	return sum;
}

int crosscorrelation(signal* f, signal* g, int m){
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

std::vector<float> checkcorr(signal* f, std::vector<float> wlist){

	if(wlist.size() == 0) return wlist;

	float maxcorr = -1;

	auto g_gen = [wlist](int x){
					float sum = 0;
					for(auto w : wlist){
						sum += sin(w*x);
					}
					return sum;
				};

	auto g = new signal[SIZE];
	
	for(int i = 0; i < SIZE; i++){
		g[i] = signal(7 * g_gen(i)/float(wlist.size()));
	}

	auto norm_coeff = sqrt((correlation(f, f) * correlation(g, g)));
	norm_coeff = 1/norm_coeff;

	for(int i = -SIZE+1; i < SIZE; i++){
		auto corr = crosscorrelation(f, g, i);
		maxcorr = maxcorr > corr ? maxcorr : corr;
	}

	// clean memory just in case it isn't deallocated 
	// before recursion else we run over quota
	delete[] g; 

	if(maxcorr*norm_coeff < corr_threshold) return {};

	if(wlist.size() == 1) return wlist;

	auto wl = checkcorr(f, std::vector<float>(wlist.begin(), wlist.begin() + (wlist.size()/2)));
	auto wr = checkcorr(f, std::vector<float>(wlist.begin() + (wlist.size()/2), wlist.end()));

	std::move(wr.begin(), wr.end(), std::back_inserter(wl));

	return wl;
}
