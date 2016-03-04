#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>

#define EQUE_HEIGHT 480
#define EQUE_WIDTH	2*EQUE_HEIGHT
#define FISHEYE_ANGLE 180




/*computes the remapping matrices*/
void computeEqueRemap(cv::Mat *mapX,cv::Mat *mapY);



