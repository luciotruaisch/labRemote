#include "OpenCVHelper.h"
#include <iostream>
#include <opencv2/imgcodecs.hpp>

using namespace cv;
using namespace std;
// #define CAP_PROP_FPS 60

int main(int argc, char* argv[]) 
{
	if(argc != 3 ) {
		return -1;
	}
	Mat img_object = imread( argv[1], IMREAD_GRAYSCALE );
	Mat img_scene = imread( argv[2], IMREAD_GRAYSCALE );

	if( !img_object.data || !img_scene.data ) {
		cout <<" --(!) Error reading images " << endl;
		return -1;
	}

	vector<Point2f> matchedCorners(4);
	OpenCVHelper::SIFT_obj_identify(img_object, img_scene, matchedCorners);
	cout <<" Printing matched corner's locations " << endl;

	for(auto& corner : matchedCorners) {
		cout << corner.x << " " << corner.y << endl; 
	}

	cout << "END" <<endl;

    return 0;
}
