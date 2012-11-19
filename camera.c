#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include <cv.h>
#include <cxcore.h>
#include <highgui.h>


static CvHaarClassifierCascade* load_object_detector(const char* cascade_path);
static int detect_objects(IplImage* image, CvHaarClassifierCascade* cascade, CvRect *rect);

int main(int argc, char const *argv[])
{
	 // cvNamedWindow("window_name", CV_WINDOW_AUTOSIZE);
  	CvHaarClassifierCascade* cascade = load_object_detector("haarcascade_frontalface_alt2.xml");	


  	CvCapture* capture = cvCaptureFromCAM(-1);
  	if( !capture ) {
    	fprintf( stderr, "ERROR: capture is NULL \n" );
    	return -1;
  	}

    cvSetCaptureProperty(capture, CV_CAP_PROP_FPS, 30.0);

  	IplImage* cur_frame;
    IplImage* pre_frame;

    struct timeval t1, t2;
    long tdiff;
    double freq;
    gettimeofday(&t1, NULL);

    CvRect rect;

  	while(1){
  		cur_frame = cvQueryFrame(capture);

      detect_objects(cur_frame, cascade, &rect);

      gettimeofday(&t2, NULL);

      tdiff = (t2.tv_sec - t1.tv_sec) * 1000000 + (t2.tv_usec - t1.tv_usec);

      t1 = t2;
      freq = 1.0 / tdiff * 1000000;

      printf("Frequency: %f\n", freq);
  		//cvShowImage("window_name", frame);
  		//cvWaitKey(1);
  	}
	return 0;
}

int detect_objects(IplImage* image, CvHaarClassifierCascade* cascade, CvRect *rect){
  CvMemStorage* storage = cvCreateMemStorage(0);
  CvSeq* faces;

  int rv = 0;

        /* use the fastest variant */
  faces = cvHaarDetectObjects( image, cascade, storage, 1.2, 3, 
    CV_HAAR_DO_CANNY_PRUNING | CV_HAAR_FIND_BIGGEST_OBJECT | CV_HAAR_DO_ROUGH_SEARCH, 
    cvSize(0, 0), cvSize(0, 0));

    // faces = cvHaarDetectObjects( image, cascade, storage, 1.1, 2, 0, 
    // cvSize(0, 0), cvSize(0, 0));


        /* draw all the rectangles */
  if ( faces->total > 0)
  {
        /* extract the rectanlges only */
    *rect = *(CvRect*)cvGetSeqElem( faces, 0 );
    rv = 1;
  } else {
    rv = 0;
  }

  cvReleaseMemStorage( &storage );
  return rv;
}

CvHaarClassifierCascade* load_object_detector( const char* cascade_path )
{
    return (CvHaarClassifierCascade*) cvLoad( cascade_path, NULL, NULL, NULL);
}
