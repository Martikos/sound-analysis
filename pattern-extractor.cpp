#include<iostream>
#include<stdio.h>
#include<stdlib.h>
#include<string>
#include<string.h>
#include<dirent.h>
#include<cmath>
#include<math.h>
#include<fstream>

/* custom libraries */
#include"track.h"

/* fourier transform */
#include<fftw3.h>
#include<complex.h>

/* wav file processing */
#include"portsf.h"
using namespace std;

char path[] = "./wavs";
char signaturesPath[] = "./signatures";

int main(int argc, char *argv[]) {

  /*
   * Initializations for processing wav files
   */
	PSF_PROPS props;
	long framesread;
	long totalread;
	/* init all dynamic resources to default states */
	int ifd = -1;
	int error = 0;
	PSF_CHPEAK* peaks = NULL;
	float* frame = NULL;
	psf_format outformat =  PSF_FMT_UNKNOWN;

	if(psf_init()){
		printf("Error: Unable to start portsf.\n");
		return 1;
	}

  /* Open directory and list files, for now */
  DIR *directory;
  struct dirent *entry;
  for(int fileNumber=1; fileNumber<argc-1; fileNumber++) {
        char filename[40] = "";
        strcat(filename, argv[fileNumber]);
        // cout << filename << endl;

        ofstream signatureFile;
        /* forge output file path string */
        char signatureFileName[40] = "";
        // strcat(signatureFileName, "./signatures/");
        strcat(signatureFileName, argv[argc-1]);
        // strcat(signatureFileName, "/");
        // strcat(signatureFileName, "sig.testfile.txt");
        /* clear file */
        signatureFile.open(signatureFileName);
        cout << "Writing to file: " << signatureFileName << endl;
        signatureFile << "";
        signatureFile.close();

        /* get signature file ready */
        signatureFile.open(signatureFileName, fstream::app | fstream::out);

        cout << "Processing " << filename << endl;
        /*
         * Do the processing here
         * use filename variable for the name of the file
         */

        /* open wav file for processing*/
        ifd = psf_sndOpen(filename, &props,0);																		  
        if(ifd < 0){
          printf("Error: unable to open infile %s%d\n",filename, fileNumber);
          return 1;
        }
        /* hide the formats for now */
        /*
        if(props.samptype == PSF_SAMP_16){ printf("Info: infile is in 16 bit format.\n"); }
        if(props.samptype == PSF_SAMP_24){ printf("Info: infile is in 24 bit format.\n");	}
        if(props.samptype == PSF_SAMP_32){ printf("Info: infile is in 32 bit format.\n");	}
        if(props.samptype == PSF_SAMP_IEEE_FLOAT){ printf("Info: infile is already in floats format.\n");	}
        */
        props.samptype = PSF_SAMP_IEEE_FLOAT;

        /* allocate space for one sample frame */
        frame = (float*) malloc(props.chans * sizeof(float));
        if(frame==NULL){
          puts("No memory!\n");
          error++;
          return 1;
        }

        /* and allocate space for PEAK info */
        peaks  =  (PSF_CHPEAK*) malloc(props.chans * sizeof(PSF_CHPEAK));
        if(peaks == NULL){
          puts("No memory!\n");
          error++;
          return 1;
        }
        /* single-frame loop to do copy: report any read/write errors */
        framesread = psf_sndReadFloatFrames(ifd,frame,1);
        int SAMPLES_PER_INTERVAL = 65536;
        int FINGERPRINT_SIZE = 40;

        /* count sample frames as they are copied */
        totalread = 0;		

        /*
         * Read into file for the first time, to get the number of frames needed
         */
        while (framesread == 1){
          totalread++;
          framesread = psf_sndReadFloatFrames(ifd,frame,1);
        }

        /* Reset ifd */
        if(ifd >= 0)
          psf_sndClose(ifd);
        ifd = psf_sndOpen(filename, &props,0);	
        framesread = psf_sndReadFloatFrames(ifd,frame,1);

        totalread = SAMPLES_PER_INTERVAL*(int)(totalread/SAMPLES_PER_INTERVAL)+SAMPLES_PER_INTERVAL;

        int N = totalread;
        totalread = 0;

        float *leftSignal = new float[N];
        while (framesread == 1) {
          totalread++;
          leftSignal[totalread] = frame[0];
          framesread = psf_sndReadFloatFrames(ifd,frame,1);
        }

        /* Null pad the signal with 0s */
        for(totalread=totalread+1; totalread<N; totalread++) {
          leftSignal[totalread] = 0;
        }

        /* TODO: epic processing */
        track *current_track = new track(leftSignal, N);
        current_track->set_signature_filename(signatureFileName);
        current_track->get_signature();

        /* Clean up.*/
        delete leftSignal;
        current_track->cleanup();

        if(ifd >= 0)
          psf_sndClose(ifd);
        cout << "Done." << endl;
        signatureFile.close();
  }
  if(frame)
    free(frame);
  if(peaks)
    free(peaks);
  psf_finish();
  cout << endl;
  return 0;
}
