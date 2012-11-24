#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <signal.h>

#include <pthread.h>
#include <semaphore.h>

#include <cv.h>
#include <highgui.h>

#include "asef.h"
#include "utils.h"

#include "list.h"

#define WINDOW_TITLE "ASEF"

char *haar_cascade_path = NULL;
char *asef_locator_path = NULL;
AsefEyeLocator asef;
CvCapture* capture;
CvHaarClassifierCascade* cascade;
char window_title[] = WINDOW_TITLE;

Queue *color_img_queue, * gray_img_queue;

sem_t *sem_processing, *sem_display;

void termination_handler(int signum){
	printf("\n");
	free(haar_cascade_path);
	free(asef_locator_path);
	cvDestroyWindow(window_title);
	asef_destroy(&asef);
	cvReleaseCapture( &capture);
	// cvReleaseImage( &gray_img );
	// cvReleaseImage( &color_img );
	exit(EXIT_SUCCESS);
}

void *processing(void * arg){

	IplImage *img;
	while (1){

		sem_wait(sem_processing);
		dequeue(gray_img_queue, (void **) &img);		
		asef.input_image = img;

		if( asef_detect_face(&asef) ){
			asef_locate_eyes(&asef);
				
		}
		sem_post(sem_display);
		cvReleaseImage(&img);
	}
	return NULL;
}

void *display(void *arg){

	IplImage *img;

	cvNamedWindow(window_title, 0);
	while (1){
		sem_wait(sem_display);

		dequeue(color_img_queue, (void **) &img);
		draw_markers(img, asef.face_rect, asef.left_eye, asef.right_eye);
    	// Mirroring results. assuming you're using a frontal facing camera
		cvFlip(img, img, 1);
		cvShowImage( window_title, img);
		cvReleaseImage(&img);
	}
	return NULL;
}



int main( int argc, char** argv )
{

	sem_processing = sem_open("processing semaphore", O_CREAT, 0600, 0);
	sem_display = sem_open("processing semaphore", O_CREAT, 0600, 0);

	pthread_t thread_processing, thread_display;
	pthread_create(&thread_processing, NULL, processing, NULL);
	pthread_create(&thread_display, NULL, display, NULL);

	if (parse_arguments(argc, argv, &haar_cascade_path, &asef_locator_path))
		return -1;

	struct sigaction act;
	act.sa_handler = termination_handler; 
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGINT, &act, NULL);

	int frame_width, frame_height;

	if ( asef_initialze(&asef, asef_locator_path, haar_cascade_path) )
		return -1;

	if( !(capture = cvCaptureFromCAM(-1)) ) {
		fprintf( stderr, "ERROR: capture is NULL \n" );
		return -1;
	}

	get_camera_properties(capture, &frame_width, &frame_height, NULL);

	CvSize frame_size = cvSize(frame_width, frame_height);

	IplImage* frame = NULL;

	IplImage *color_img, *gray_img;

	color_img_queue = create_queue();
	gray_img_queue = create_queue();


	while (1){
		printf("\r"); fflush(stdout); 
		printf("FPS: %f  ", calculate_fps());

		frame = cvQueryFrame(capture);
		color_img = cvCreateImage( frame_size, IPL_DEPTH_8U, 3 );
		gray_img = cvCreateImage( frame_size, IPL_DEPTH_8U, 1);

		cvCopyImage(frame, color_img);
		cvCvtColor(color_img, gray_img, CV_RGB2GRAY);

		enqueue(color_img_queue, color_img);
		enqueue(gray_img_queue, gray_img);
		
		sem_post(sem_processing);
	} 
	return 0;
}
