all:
	g++ -w pattern-extractor.cpp portsf.c ieee80.c -lfftw3 -lm -o extract && g++ ber.cpp -o ber
