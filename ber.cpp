#include<iostream>
#include<fstream>
#include<string>
#include<bitset>
#include<vector>
#include<stdlib.h>
using namespace std;

int main(int argc, char *argv[]) {
  const size_t N = 32;
  /* tiny tester program */
  /*
  string a_str = "10101", b_str = "01010";
  bitset<N> a(a_str), b(b_str);
  bitset<N> c = a ^ b;
  cout << c.count() << endl;
  */


  /* open the sample file */
  string STRING; 
  ifstream infile;

  int samplesize = 0;
  // infile.open("./signatures/twodoorcut");
  infile.open(argv[1]);
  vector<bitset<N> > sample;
  while(!infile.eof()) {
    getline(infile, STRING);
    bitset<N> curbitset(STRING);
    sample.push_back(curbitset);
    samplesize++;
  }
  infile.close();


  /* open the database file */
  ifstream trackfile;
  // trackfile.open("./signatures/twodoor");
  trackfile.open(argv[2]);
  int tracksize = 0;
  vector<bitset<N> > track;
  while(!trackfile.eof()) {
    getline(trackfile, STRING);
    bitset<N> curbitset(STRING);
    track.push_back(curbitset);
    tracksize++;
  }
  trackfile.close();

  int ber;
  int cur;
  int sampleindex = 5;
  int besttrackindex = 0;
  int bestsampleindex = 0;
  double min = 999999;
  for(int i=0; i<tracksize-samplesize; i++) {
    ber = 0;
    sampleindex = 0;
    for(int j=0; j<samplesize; j++) {
      cur = (track[j+i]^sample[sampleindex]).count();
      ber += cur;
      sampleindex ++;
    }
    if(ber<min) {
      min = ber;
      besttrackindex = i;
    }
  }
  float total = 32*samplesize;
  float ratio = min/total;
  float samplerate = 44100;
  float overlap = 521;
  cout << ratio << " (BER), starting at time " << besttrackindex*overlap/samplerate << " seconds in the track of the signature file ";
  
  /* benchmark output */
  // cout << ratio << " " << besttrackindex*overlap/samplerate;

  return 0;
}
