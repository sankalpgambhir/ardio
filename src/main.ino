// main.ino

#include "ArduinoSTL.h"

#define assert(x) (void*)0
#define SIZE 200

// bit mask storage
struct doobit{
  uint_fast8_t data;

  doobit(int8_t x = 0, int8_t y = 0){ // handles all our casts too
    this->storelow(x);
    this->storehigh(y);
  }

  void storelow(int8_t);
  void storehigh(int8_t);

  int8_t getlow();
  int8_t gethigh();

  int16_t operator*(doobit& b){
    auto highprod = this->gethigh() * b.gethigh();
    auto lowprod = this->getlow() * b.getlow();
    return (highprod + lowprod);
  }
};
typedef doobit signal;

// functions and constants
int correlation(signal*, signal*);
int crosscorrelation(signal*, signal*, int = 0);
std::vector<float> checkcorr(signal*, std::vector<float>);

std::vector<float> freq{};

const float corr_threshold = 0.1;

// data input
bool recording = false;
uint16_t recorded = 0;
String text;
uint8_t has_num = 0;
uint8_t k[] = {0, 0};

signal f[SIZE];

void setup(){
    Serial.begin(9600);

  // clear data just in case
  for(int i = 0; i < SIZE; i++){
    f[i] = 0;
  }
  
  for(float i = 0.1; i < 1.0; i += 0.1){
    freq.push_back(i);  
  }
}

void loop(){

    if(recording){
        // recording data
		if (Serial.available()){
			text = Serial.readStringUntil('$');
			k[has_num] = text.toInt();
			has_num++;
		}
		if ((has_num >= 2) && recorded < SIZE) {
			recorded++;
			f[recorded] = signal(k[0], k[1]); 
			has_num = 0;
		}
		if(recorded >= SIZE){
			Serial.write("G"); // we Good
			digitalWrite(13,1);
			recording = false;
		}
    }
    else{
        // calculate with the data

        // f coming from data
        /*
		auto f_gen = [](int x){
                    return (7.0*(sin(0.3 * x) + 4*sin(0.5 * x) + sin(0.6*x + 0.6))/6.0);
                    };

        for(int i = 0; i < SIZE; i++){
            f[i] = signal(f_gen(2*i), f_gen(2*i+1)); 
        }
		*/

        auto wpresent = checkcorr(f, freq);

        for(auto w : wpresent){
            Serial.println(w);
        }

        Serial.println("TEST$$$$$$$");
    }

    return;
}

// definitions


void doobit::storelow(int8_t x){
  this->data &= 0b11110000;  // clear for storage

  x += 7; // remove signed component
  assert((!(x & 0b11110000)) && "doobit range violation");

  this->data |= x;
}

void doobit::storehigh(int8_t x){
  this->data &= 0b00001111;  // clear for storage

  x += 7; // remove signed component
  assert((!(x & 0b11110000)) && "doobit range violation");

  this->data |= (x << 4);
}

int8_t doobit::getlow(){
  int8_t x = (data & 0b00001111); // bitmask
  return (x-7); // reinsert sign
}

int8_t doobit::gethigh(){
  int8_t x = ((data & 0b11110000) >> 4); // bitmask and shift
  return (x-7); // reinsert sign
}


int correlation(signal* f, signal* g){
  int sum = 0;

  for(int i = 0; i < SIZE; i++){
    sum += f[i] * g[i];
  }
  return sum;
}

int crosscorrelation(signal* f, signal* g, int m){
  int sum = 0;

  if(m >= 0){
    for(int i = 0; i < SIZE - m; i++){
      sum += f[i] * g[i+m];
    }
    for(int i = 0; i < m; i++){
      sum += f[i+SIZE-m] * g[i];
    }
  }
  else{
    m = -m;
    for(int i = 0; i < m; i++){
      sum += f[i] * g[i+SIZE-m];
    }
    for(int i = m; i < SIZE; i++){
      sum += f[i] * g[i-m];
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
          return 7.0*sum/float(wlist.size());
        };

  auto g = new signal[SIZE];
  
  for(int i = 0; i < SIZE; i++){
    g[i] = signal(g_gen(2*i), g_gen(2*i + 1));
  }

  auto norm_coeff = sqrt((correlation(f, f) * (double)correlation(g, g)));
  norm_coeff = 1/norm_coeff;

  for(int i = -(SIZE/5) + 1; i < SIZE/5; i++){
    auto corr = crosscorrelation(f, g, i);
    maxcorr = maxcorr > corr ? maxcorr : corr;
    if (maxcorr < corr){
      maxcorr = corr;
      if(maxcorr*norm_coeff > corr_threshold) break;
    }
  }

  // clean memory just in case it isn't deallocated 
  // before recursion else we run over quota
  delete[] g; 

  if(maxcorr*norm_coeff < corr_threshold) return std::vector<float>{};

  if(wlist.size() == 1) return wlist;

  auto wl = checkcorr(f, std::vector<float>(wlist.begin(), wlist.begin() + (wlist.size()/2)));
  auto wr = checkcorr(f, std::vector<float>(wlist.begin() + (wlist.size()/2), wlist.end()));

  wl.insert(wl.end(), wr.begin(), wr.end());

  return wl;
}