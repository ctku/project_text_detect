#include "../include/system.h"
#include "../include/imfeat.h"
#include "../include/textdetect.h"
#include <iostream>
#include <sys/stat.h>
#include <stdio.h>
#include <fstream>
#include <sstream>
#include <time.h>
#include <cv.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/ml.h"

using namespace std;
using namespace cv;

/* global variable */
G_textdetect_t G_td;

int ICDAR2013_generate_ER_candidates(void)
{
	int img_start = 1;
	int img_end = 233;
	int algo = 3;
	int max_width = 1600;
	float img_resize_ratio = 1.0;

	char in[100] =  "../../../../../Dataset/ICDAR_2013/SceneTest/";
	char out[100] = "../../../../../../../LargeFiles/ICDAR_2013/";

	G_td.input_path = in;
	G_td.output_path = out;

	for (int img_id = 205; img_id <= 233; img_id++) {

		char fn[128];
		sprintf(fn, "%simg_%d.jpg", G_td.input_path, img_id);
		CvSize size;

		IplImage *img = cvLoadImage(fn, CV_LOAD_IMAGE_COLOR);
		if (img->width > max_width) {
			img_resize_ratio = max_width*1.0 / img->width;
			size = cvSize(max_width, (int)img->height*resize_ratio);
			IplImage *img_rs = cvCreateImage(size, img->depth, img->nChannels);
			cvResize(img, img_rs);
			cvReleaseImage(&img);
			img = img_rs;
		} else {
			size = cvGetSize(img);
		}
		IplImage *y = cvCreateImage(size, IPL_DEPTH_8U, CV_8UC1),
				 *u = cvCreateImage(size, IPL_DEPTH_8U, CV_8UC1),
				 *v = cvCreateImage(size, IPL_DEPTH_8U, CV_8UC1);
		cvSplit(img, y, u, v, NULL);
		cvReleaseImage(&img);
		Mat yy = Mat(y,0);
		Mat uu = Mat(u,0);
		Mat vv = Mat(v,0);

		generate_ER_candidates(&yy, img_id, 'y', img_resize_ratio, 0, algo);
		generate_ER_candidates(&yy, img_id, 'y', img_resize_ratio, 1, algo);
		generate_ER_candidates(&uu, img_id, 'u', img_resize_ratio, 0, algo);
		generate_ER_candidates(&uu, img_id, 'u', img_resize_ratio, 1, algo);
		generate_ER_candidates(&vv, img_id, 'v', img_resize_ratio, 0, algo);
		generate_ER_candidates(&vv, img_id, 'v', img_resize_ratio, 1, algo);

		yy.release();
		uu.release();
		vv.release();
		cvReleaseImage(&y);
		cvReleaseImage(&u);
		cvReleaseImage(&v);
	}

	return 0;
}

void main(void) {

	ICDAR2013_generate_ER_candidates();

}