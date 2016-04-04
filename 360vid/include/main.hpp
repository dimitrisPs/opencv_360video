#include <stdlib.h>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <math.h>
#include <stdio.h>
#include <iostream>


void equeToPerspective(float theta, float phi, cv::Mat &mapX,cv::Mat &mapY,cv::Size equeSize);
void CallBackFunc(int event, int x, int y, int flags, void* userdata);


