#include<iostream>
#include<ctime>
#include<fstream>

/* custom libraries */
#include"sort.h"

/* fourier transform libraries */
#include<fftw3.h>
#include<complex.h>

using namespace std;

char signatures_path[] = "./signatures";
// int SAMPLES_PER_INTERVAL = 16317;

// power of 2 
int SAMPLES_PER_INTERVAL = 16384;

int OVERLAP = 512; 
// int OVERLAP = 512; // 11.61ms 
int NUMBER_OF_BANDS = 32;
int FINGERPRINT_SIZE = 40;
int DOWNSAMPLE_RATE = 5000;

int FREQUENCY_UPPERBOUND = 3000;
int FREQUENCY_LOWERBOUND = 300;
int SAMPLE_PER_BAND = 33;
int FACTOR = 1.06;
// int SAMPLE_PER_BAND = 54;

class track {
  private:
    ofstream fileout;
    string name;
    float *left_signal;
    int size;

  public:
    track() {}
    track(float *left, int n) {
      size = n;
      left_signal = new float[size];
      for (int i = 0; i < size; i++) {
        left_signal[i] = left[i];
      }
    }

    void set_overlap(int a) {
      OVERLAP = a;
    }
    int get_overlap() {
      return OVERLAP;
    }

    void set_name(string name) {
      this->name = name;
    }
    string get_name() {
      return this->name;
    }
    void set_signature_filename(char filename[]) {
      fileout.open(filename, fstream::app | fstream::out);
    }

    void sign(int factor, int offset){
      int counter = 0;
      int increment;

      int prev_sum = 0;
      float *prevtime_curband_energy = new float[NUMBER_OF_BANDS];
      float *prevtime_prevband_energy = new float[NUMBER_OF_BANDS];

      for(int i=0; i<size; i+=OVERLAP) {
        fftw_complex *in, *out;
        fftw_plan p;
        in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SAMPLES_PER_INTERVAL);
        out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * SAMPLES_PER_INTERVAL);
        for(int j=0; j<SAMPLES_PER_INTERVAL; j++) {
          in[j][0] = left_signal[i+j];
          in[j][1] = left_signal[i+j];
        }
        p = fftw_plan_dft_1d(SAMPLES_PER_INTERVAL, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
        fftw_execute(p);

        float *amplitudes = new float[SAMPLES_PER_INTERVAL];
        int *frequencies = new int[SAMPLES_PER_INTERVAL];
        for (int j = 0; j < SAMPLES_PER_INTERVAL; j++) {
          amplitudes[j] = sqrt(out[j][0]*out[j][0]+out[j][1]*out[j][1]);
          frequencies[j] = j;
        }

        /* Magic for loop */
        float curtime_prevband_energy = 0;
        float curtime_curband_energy = 0;
        float deltaband;

        SAMPLE_PER_BAND = 33;
        for(int band=0; band<NUMBER_OF_BANDS; band++) {
          // int max_index = find_max(amplitudes, band*SAMPLE_PER_BAND, SAMPLE_PER_BAND*(band+1));
          curtime_prevband_energy = curtime_curband_energy;
          curtime_curband_energy = energy(amplitudes, band*SAMPLE_PER_BAND+FREQUENCY_LOWERBOUND , min(SAMPLE_PER_BAND*(band+1)+300,FREQUENCY_UPPERBOUND) );
          deltaband = curtime_prevband_energy - curtime_curband_energy;
          // fileout << frequencies[max_index] << " ";
          if(band>0) {
            float deltatime = prevtime_curband_energy[band-1] - prevtime_curband_energy[band];
            bool delta = (deltaband-deltatime>0);
            fileout << delta;
          }
          prevtime_curband_energy[band] = curtime_curband_energy;
          SAMPLE_PER_BAND*=FACTOR;
        }
        /*
        for(int m=0; m<SAMPLES_PER_INTERVAL; m+=increment) {
          int limit = SAMPLES_PER_INTERVAL;
          // cout << increment << endl;
          // int max_index = 1;
          // quicksort(amplitudes, frequencies, m, limit-1);
          /* Output signature to file */
          // fileout << frequencies[m] << " ";
        // }
        fileout << endl;

        /* Mini clean up */
        fftw_destroy_plan(p);
        fftw_free(in); 
        fftw_free(out);
        delete amplitudes;
        delete frequencies;

      }
      delete prevtime_curband_energy;
      delete prevtime_prevband_energy;
      fileout << endl;
    }
    void get_signature() {
      sign(2, 1);
    }


    void cleanup() {
      fileout.close();
      delete left_signal;
    }

};


