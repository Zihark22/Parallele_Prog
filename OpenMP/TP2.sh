clear;
nom="TP2_2";

gcc -o $nom $nom.c `pkg-config --cflags opencv --libs opencv` -lm -fopenmp;
./$nom river.jpg;

