#ifndef __OpenCVHelper_H__
#define __OpenCVHelper_H__

#include "opencv2/core.hpp"
#include "opencv2/highgui.hpp"

using namespace cv;

namespace OpenCVHelper {
	void SIFT_obj_identify(const Mat& img1, const Mat& img2, std::vector<Point2f>& matchedCorners);
};

#endif
