#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <opencv/highgui.h>

#define uchar unsigned char
/*
//Pour Images 512*512
#define imgsize_cpu 512*512*3
#define imgsize_omp 512*128*3     // 512*512
#define bgrey 512*128    //512*512
#define width 512
#define height 512
*/

// Pour Images 1024*1024
#define imgsize_cpu 1920*1080*3
#define imgsize_omp 1920*270*3
#define bgrey 1920*270
#define width 1920
#define height 1080

void bwimage_cpu(uchar *array, uchar *bwa)
{
	int i;
	int j;
	for(i=0,j=0;i<imgsize_cpu;i=i+3,j++)
			bwa[j] = 0.3*array[i] + 0.59*array[i+1] + 0.11*array[i+2];
}

void bwimage_1(uchar *array, uchar *bwa)
{
	int i;
	int j;
	for(i=0,j=0;i<imgsize_omp;i=i+3,j++)
			bwa[j] = 0.3*array[i] + 0.59*array[i+1] + 0.11*array[i+2];
}
void bwimage_2(uchar *array, uchar *bwa)
{
	int i;
	int j;
	for(i=imgsize_omp,j=0;i<imgsize_omp*2;i=i+3,j++)
			bwa[j] = 0.3*array[i] + 0.59*array[i+1] + 0.11*array[i+2];
}
void bwimage_3(uchar *array, uchar *bwa)
{
	int i;
	int j;
	for(i=imgsize_omp*2,j=0;i<imgsize_omp*3;i=i+3,j++)
			bwa[j] = 0.3*array[i] + 0.59*array[i+1] + 0.11*array[i+2];
}
void bwimage_4(uchar *array, uchar *bwa)
{
	int i;
	int j;
	for(i=imgsize_omp*3,j=0;i<imgsize_omp*4;i=i+3,j++)
			bwa[j] = 0.3*array[i] + 0.59*array[i+1] + 0.11*array[i+2];
}	

void bwimage_omp(uchar *array, uchar *bwa)
{
	#pragma omp parallel num_threads(4)
	{
		uchar *bwa_omp;
		#pragma omp sections firstprivate(bwa)
		{
			#pragma omp section
			{
				bwimage_1(array,bwa_omp);
			}
			#pragma omp section
			{
				bwimage_2(array,bwa_omp);
			}
			#pragma omp section
			{
				bwimage_3(array,bwa_omp);
			}
			#pragma omp section
			{
				bwimage_4(array,bwa_omp);
			}
		}
		bwa = bwa_omp;
	}
}


int main (int argc, char* argv[])
{
	double starttime,stoptime,extime,mips;
	IplImage* img = NULL;
	IplImage* bwimg = NULL;
	IplImage* bwimgnor = NULL;
	uchar *data;
	uchar *bw;
	uchar *bwnor;
	const char* window_title = "Hello, openMP!";
	int index,tmp;
	int n=width*height;
	char nameout[64];
	float bwessa[n];
	float mean=0.0, var=0.0, svar=0.0i, std;

	if (argc < 2)
	{
		fprintf (stderr, "usage: %s IMAGE\n", argv[0]);
		return EXIT_FAILURE;
	}
	img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED);
	if (img == NULL)
	{
		fprintf (stderr, "couldn't open image file: %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	data = (uchar *) img->imageData;

  /*
  // Pour images 512*512
  bwimg = cvCreateImage(cvSize(512,512),IPL_DEPTH_8U,1);
  bw = (uchar *)bwimg->imageData;
  bwimgnor = cvCreateImage(cvSize(512,512),IPL_DEPTH_8U,1);
  bwnor = (uchar *)bwimgnor->imageData;
  */

  // Pour images 1024*1024
	bwimg = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	bw = (uchar *)bwimg->imageData;
	bwimgnor = cvCreateImage(cvSize(width,height),IPL_DEPTH_8U,1);
	bwnor = (uchar *)bwimgnor->imageData;

	omp_set_num_threads(4);

// Execution CPU
// get the starttime
	starttime=omp_get_wtime() ;
	bwimage_omp(data,bw);
	bzero(nameout,64);
	strcpy(nameout,"wb"); // white and black
	strcat(nameout,argv[1]);

	cvSaveImage(nameout,bwimg,0);

//Calculate the mean
	for(index=0; index<n;index++){
		mean = mean + bw[index];
	}
	mean = mean/n;
	printf("mean=%f\n",mean);

// Calculate the standard deviation of the image intensities
	for(index=0; index<n;index++){svar=(float)(bw[index])-mean;var+=svar*svar;}
	var/=(float)n;
	std=sqrtf(var);
	printf("std=%f\n",std);

// Rescale using the calculated mean and standard deviation
	for(index=0;index<n;index++) {
		bwessa[index]= mean*(float)((bw[index]-(uchar)mean)/std);


		bwnor[index]= (uchar)bwessa[index];
	}

// get the stoptime and evaluate execution time
    stoptime=omp_get_wtime() ;
	extime= stoptime-starttime ;
    printf("Total CPU execution time : %3.6f ms\n",extime*1000) ;


// Save and display
/*
  cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
  cvShowImage (window_title, bwimg);
  cvWaitKey(0);
*/
  cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);
  cvMoveWindow (window_title, width,height);
  cvShowImage (window_title, bwimgnor);
  cvWaitKey(0);
  cvDestroyAllWindows();
  cvReleaseImage(&bwimgnor);
  return EXIT_SUCCESS;
}

// 497.8 ms CPU

