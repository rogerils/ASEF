#ifndef _UTILS_H_
#define _UTILS_H_

#include <cv.h>
#include <highgui.h>


void get_camera_properties(CvCapture * capture, int *frame_width, int *frame_height, double *fps);
double calculate_fps();
void draw_markers(CvArr* img, CvRect rect, CvPoint pt1, CvPoint pt2);

#endif