#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#define uchar unsigned char
#define imgsize_cpu 1920*1080*3
#define imgsize_omp 1920*270*3

void negimage_cpu(uchar *array) {
    int i ;
    for (i=0 ; i<imgsize_cpu ; i++) array[i] = 255 - array[i] ;
}

void negimage_1(uchar *array) {
    int i ;
    for (i=0 ; i<imgsize_omp ; i++) array[i] = 255 - array[i] ;
}
void negimage_2(uchar *array) {
    int i ;
    for (i=imgsize_omp ; i<imgsize_omp*2 ; i++) array[i] = 255 - array[i] ;
}
void negimage_3(uchar *array) {
    int i ;
    for (i=imgsize_omp*2 ; i<imgsize_omp*3 ; i++) array[i] = 255 - array[i] ;
}
void negimage_4(uchar *array) {
    int i ;
    for (i=imgsize_omp*3 ; i<imgsize_omp*4 ; i++) array[i] = 255 - array[i] ;
}

void negimage_omp(uchar *array) {
	#pragma omp parallel num_threads(4)
	{
		#pragma omp sections
		{
			#pragma omp section
			{
				negimage_1(array);
			}
			#pragma omp section
			{
				negimage_2(array);
			}
			#pragma omp section
			{
				negimage_3(array);
			}
			/*#pragma omp section
			{
				negimage_4(array);
			}*/
		}
	}
}

int main (int argc, char *argv[])
{
    double starttime, stoptime, extime ;
    IplImage* img = NULL ;
    uchar *data ;
    const char* window_title = "Hello, OpenCV !" ;
    // Opening the image file
    if (argc < 2)
    {
        fprintf (stderr, "usage : %s IMAGE\n", argv[0]) ;
        return EXIT_FAILURE ;
    }
    img = cvLoadImage(argv[1], CV_LOAD_IMAGE_UNCHANGED) ;
    if (img == NULL)
    {
        fprintf (stderr, "couldn't open image file : %s\n", argv[1]) ;
        return EXIT_FAILURE ;
    }
    data = (uchar *) img-> imageData ;
    // Getting the execution time
    starttime=omp_get_wtime() ;
    negimage_omp (data) ;
    stoptime=omp_get_wtime() ;
    extime= stoptime-starttime ;
    printf("Total CPU execution time : %3.6f ms\n",extime*1000) ;
    // Displaying the result
    cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE) ;
    cvShowImage (window_title, img) ;
    cvWaitKey(0) ;
    cvDestroyAllWindows() ;
    cvReleaseImage(&img) ;
    return EXIT_SUCCESS ;
}
// 110 ms
// 46 ms
