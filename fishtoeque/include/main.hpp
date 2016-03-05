#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>

#define EQUE_HEIGHT 50
#define EQUE_WIDTH	EQUE_HEIGHT
#define FISHEYE_ANGLE M_PI




/*computes the remapping matrices*/
void computeEqueRemap(cv::Mat *mapX,cv::Mat *mapY);



