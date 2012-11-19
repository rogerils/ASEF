#ifndef _ASEF_H_
#define _ASEF_H_

#include <cv.h>

typedef struct{
	CvMat *lfilter, *rfilter;
	int n_rows, n_cols;
	CvRect lrect, rrect;
	CvMat *lfilter_dft, *rfilter_dft;
	CvMat *image, *image_tile;
	CvMat *lcorr, *rcorr;
	CvMat *lroi, *rroi;
	CvMat *lut;
	CvMat face_img;
} AsefEyeLocator;

void asef_initialze(AsefEyeLocator *asef, const char *file_name);
void asef_destroy(AsefEyeLocator *asef);
void asef_locate_eyes(AsefEyeLocator *asef, IplImage *image, CvRect face_rect, CvPoint *leye, CvPoint *reye);

#endif