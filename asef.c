#include "asef.h"
#include <stdio.h>

#define LINE_BUF_SIZE 1024

static int load_asef_filters(const char* file_name, int *p_n_rows, int *p_n_cols, 
  CvRect *left_eye_region, CvRect *right_eye_region, 
  CvMat **p_left_filter, CvMat **p_right_filter);

static int read_line(FILE *fp, char *buf, int size);


void asef_initialze(AsefEyeLocator *asef, const char *file_name){

  load_asef_filters(file_name, &asef->n_rows, &asef->n_cols, &asef->lrect, 
    &asef->rrect, &asef->lfilter, &asef->rfilter);


  asef->lfilter_dft = cvCreateMat(asef->n_rows, asef->n_cols, CV_32FC1);
  asef->rfilter_dft = cvCreateMat(asef->n_rows, asef->n_cols, CV_32FC1);

  asef->image = cvCreateMat(asef->n_rows, asef->n_cols, CV_32FC1);
  asef->image_tile = cvCreateMat(asef->n_rows, asef->n_cols, CV_8UC1);

  asef->lcorr = cvCreateMat(asef->n_rows, asef->n_cols, CV_32FC1);
  asef->rcorr = cvCreateMat(asef->n_rows, asef->n_cols, CV_32FC1);

  asef->lroi = cvCreateMatHeader(asef->n_rows, asef->n_cols, CV_32FC1);
  asef->rroi = cvCreateMatHeader(asef->n_rows, asef->n_cols, CV_32FC1);

  cvDFT(asef->lfilter, asef->lfilter_dft, CV_DXT_FORWARD, 0);
  cvDFT(asef->rfilter, asef->rfilter_dft, CV_DXT_FORWARD, 0);

  cvGetSubRect(asef->lcorr, asef->lroi, asef->lrect);
  cvGetSubRect(asef->rcorr, asef->rroi, asef->rrect);

  asef->lut = cvCreateMat(256, 1, CV_32FC1);
  for (int i = 0; i<256; i++){
    cvmSet(asef->lut, i, 0, 1.0 + i);
  }
  cvLog(asef->lut, asef->lut);
}


void asef_destroy(AsefEyeLocator *asef){
  cvReleaseMat(&asef->lfilter);
  cvReleaseMat(&asef->rfilter);
  cvReleaseMat(&asef->lfilter_dft);
  cvReleaseMat(&asef->rfilter_dft);
  cvReleaseMat(&asef->image);
  cvReleaseMat(&asef->image_tile);
  cvReleaseMat(&asef->lcorr);
  cvReleaseMat(&asef->rcorr);
  cvReleaseMat(&asef->lroi);
  cvReleaseMat(&asef->rroi);
  cvReleaseMat(&asef->lut);
}

void asef_locate_eyes(AsefEyeLocator *asef, IplImage *image, CvRect face_rect, CvPoint *leye, CvPoint *reye){
  asef->face_img.cols = face_rect.width;
  asef->face_img.rows = face_rect.height;
  asef->face_img.type = CV_8UC1;
  asef->face_img.step = face_rect.width;

  cvGetSubRect(image, &asef->face_img, face_rect);

  double xscale = ((double)asef->image_tile->cols)/((double)asef->face_img.cols);
  double yscale = ((double)asef->image_tile->rows)/((double)asef->face_img.rows);

  cvResize(&asef->face_img, asef->image_tile, CV_INTER_LINEAR);

  cvLUT(asef->image_tile, asef->image, asef->lut);

  cvDFT(asef->image, asef->image, CV_DXT_FORWARD, 0);
  cvMulSpectrums(asef->image, asef->lfilter_dft, asef->lcorr, CV_DXT_MUL_CONJ);
  cvMulSpectrums(asef->image, asef->rfilter_dft, asef->rcorr, CV_DXT_MUL_CONJ);

  cvDFT(asef->lcorr, asef->lcorr, CV_DXT_INV_SCALE, 0);
  cvDFT(asef->rcorr, asef->rcorr, CV_DXT_INV_SCALE, 0);

  cvMinMaxLoc(asef->lroi, NULL, NULL, NULL, leye, NULL);
  cvMinMaxLoc(asef->rroi, NULL, NULL, NULL, reye, NULL);

  leye->x = (asef->lrect.x + leye->x)/xscale + face_rect.x;
  leye->y = (asef->lrect.y + leye->y)/yscale + face_rect.y;
  reye->x = (asef->rrect.x + reye->x)/xscale + face_rect.x;
  reye->y = (asef->rrect.y + reye->y)/yscale + face_rect.y;
}



int load_asef_filters(const char* file_name, int *p_n_rows, int *p_n_cols, 
  CvRect *left_eye_region, CvRect *right_eye_region, 
  CvMat **p_left_filter, CvMat **p_right_filter){

  int rv;

  char buf[LINE_BUF_SIZE];

  FILE *fp = fopen(file_name, "r");

  if (!fp){
    return -1;
  }

int n_rows, n_cols; // row and column size


read_line(fp, buf, LINE_BUF_SIZE);
printf("%s\n", buf);
if (strcmp(buf, "CFEL")){
  return -1;
}

// Print comments and copyright
for (int i = 0; i < 2; i++){
  if (read_line(fp, buf, LINE_BUF_SIZE) <= 0){
    return -1;
  }
  printf("%s\n", buf);
}

read_line(fp, buf, LINE_BUF_SIZE);
sscanf(buf, "%d %d", &n_rows, &n_cols);
*p_n_rows = n_rows;
*p_n_cols = n_cols;

int rect_x, rect_y, rect_width, rect_hight;
read_line(fp, buf, LINE_BUF_SIZE);
sscanf(buf, "%d %d %d %d", &rect_x, &rect_y, &rect_width, &rect_hight);

if (left_eye_region){
  *left_eye_region = cvRect(rect_x, rect_y, rect_width, rect_hight); 
}

read_line(fp, buf, LINE_BUF_SIZE);
sscanf(buf, "%d %d %d %d", &rect_x, &rect_y, &rect_width, &rect_hight);

if (right_eye_region){
  *right_eye_region = cvRect(rect_x, rect_y, rect_width, rect_hight); 
}

uint32_t endien_checker;
unsigned long endianness;
read_line(fp, buf, LINE_BUF_SIZE);  
endien_checker = *(uint32_t*)buf;

// printf("%u, %u, %u\n", endien_checker, 0x41424344, 0x44434241);


if ( !strcmp(buf, "ABCD") ){
// Big endian
  endianness = 0;
} else if ( !strcmp(buf, "DCBA")){
// Little endian
// Almost always this case on your x86 machine. 
// Not sure about ARM (Android/iOS), you can test it out :) 
  endianness = 1;
} else {
  endianness = -1;
}

// TODO: handle big endian with byte swap;

size_t filter_data_size = n_rows * n_cols * sizeof(float);
CvScalar mean, std_dev;


unsigned char* lfilter_data = (unsigned char*)malloc(filter_data_size);
rv = fread(lfilter_data, 1, filter_data_size, fp);
assert(rv == filter_data_size);

if (p_left_filter){ 

  CvMat *left_filter = cvCreateMatHeader(n_rows, n_cols, CV_32FC1);
  cvSetData(left_filter, lfilter_data, CV_AUTO_STEP);
  cvAvgSdv(left_filter, &mean, &std_dev, NULL);
  cvScale(left_filter, left_filter, 1.0/std_dev.val[0], -mean.val[0]*1.0/std_dev.val[0]);
  *p_left_filter = left_filter; 

} else{
  free(lfilter_data);
}

unsigned char* rfilter_data = (unsigned char*)malloc(filter_data_size);
rv = fread(rfilter_data, 1, filter_data_size, fp);
assert(rv == filter_data_size);

if (p_right_filter){
  CvMat *right_filter = cvCreateMatHeader(n_rows, n_cols, CV_32FC1);
  cvSetData(right_filter, rfilter_data, CV_AUTO_STEP);
  cvAvgSdv(right_filter, &mean, &std_dev, NULL);
  cvScale(right_filter, right_filter, 1.0/std_dev.val[0], -mean.val[0]*1.0/std_dev.val[0]);
  *p_right_filter = right_filter;
} else{
  free(rfilter_data);
}

fclose(fp);

return 1;

}


int read_line(FILE* fp, char* buf, int size){

  int c, i = 0;
  while (i < (size - 1) && (c = fgetc(fp)) != EOF){
    if ( c == '\n' ) {
      break;
    } 
    buf[i++] = c;
  }

  buf[i] = '\0';
  return i;
}