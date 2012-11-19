#include "utils.h"
#include <sys/time.h>



void get_camera_properties(CvCapture * capture, int *frame_width, int *frame_height, double *fps){
  	if ( !capture ) return ;
	if ( frame_width ){
		*frame_width = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH);
	}
	if ( frame_height ){
		*frame_height = (int)cvGetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT);
	}
	if ( fps ){
		*fps = cvGetCaptureProperty(capture, CV_CAP_PROP_FPS);
	}
}

double calculate_fps(){
	static long frame_count = 0;
	static struct timeval time_curr, time_prev;
	long tdiff;

	time_prev = time_curr;

	if (frame_count++){
		gettimeofday(&time_curr, NULL);
	}

	tdiff = (time_curr.tv_sec - time_prev.tv_sec) * 1000000 + 
	(time_curr.tv_usec - time_prev.tv_usec);

	return 1.0 / tdiff * 1000000;
}


#define LINE_WIDTH 3
#define LINE_COLOR (CV_RGB(255,0,0))

void draw_markers(CvArr* img, CvRect rect, CvPoint pt1, CvPoint pt2){

	CvScalar color = LINE_COLOR;

	cvCircle(img, pt1, LINE_WIDTH, color, 3, CV_AA, 0);
	cvCircle(img, pt2, LINE_WIDTH, color, 3, CV_AA, 0);	
	cvRectangle( img, cvPoint(rect.x,rect.y), 
		cvPoint(rect.x+rect.width, rect.y+ rect.height), color, LINE_WIDTH, CV_AA, 0);

}