#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>
#include <signal.h>

#include <cv.h>
#include <highgui.h>

#include "asef.h"
#include "fd.h"
#include "utils.h"

#define WINDOW_TITLE "ASEF"

AsefEyeLocator asef;
CvCapture* capture;
CvHaarClassifierCascade* cascade;
char window_title[] = WINDOW_TITLE;
IplImage* color_img;
IplImage* gray_img;


void termination_handler(int signum){
  printf("\n");
  cvDestroyWindow(window_title);
  asef_destroy(&asef);
  cvReleaseCapture( &capture);
  cvReleaseHaarClassifierCascade( &cascade );
  cvReleaseImage( &gray_img );
  cvReleaseImage( &color_img );
  exit(EXIT_SUCCESS);
}


int main( int argc, char** argv )
{

  struct sigaction act;
  act.sa_handler = termination_handler; 
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;
  sigaction(SIGINT, &act, NULL);

  CvRect face_rect;
  int frame_width, frame_height;
  CvPoint left_eye, right_eye;

  asef_initialze(&asef, "EyeLocatorASEF128x128.fel");
  cascade = fd_load_detector("haarcascade_frontalface_alt2.xml");  

  capture = cvCaptureFromCAM(-1);
  if( !capture ) {
    fprintf( stderr, "ERROR: capture is NULL \n" );
    return -1;
  }

  get_camera_properties(capture, &frame_width, &frame_height, NULL);

  CvSize frame_size = cvSize(frame_width, frame_height);

  IplImage* frame = NULL;
  color_img = cvCreateImage( frame_size, IPL_DEPTH_8U, 3 );
  gray_img = cvCreateImage( frame_size, IPL_DEPTH_8U, 1);

  cvNamedWindow(window_title, 0);
  while (1){
    printf("\r"); fflush(stdout); 
    printf("FPS: %f", calculate_fps());

    frame = cvQueryFrame(capture);
    cvCopyImage(frame, color_img);
    cvCvtColor(color_img, gray_img, CV_RGB2GRAY);

    if( fd_detect_face(gray_img, cascade, &face_rect) ){
      asef_locate_eyes(&asef, gray_img, face_rect, &left_eye, &right_eye);
      draw_markers(color_img, face_rect, left_eye, right_eye);
    }

    // Mirroring results. assuming you're using a frontal facing camera
    cvFlip(color_img, color_img, 1);
    cvShowImage( window_title, color_img);
  } 
  return 0;
}
