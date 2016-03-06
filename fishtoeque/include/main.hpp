#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <math.h>

#define EQUE_HEIGHT 50
#define EQUE_WIDTH	EQUE_HEIGHT
#define FISHEYE_ANGLE 185 * ( M_PI /180 )//3.22885912//M_PI//




/*computes the remapping matrices*/
void computeEqueRemap(cv::Mat *mapX,cv::Mat *mapY);



void computeEqueRemap2(cv::Mat *mapX,cv::Mat *mapY);

void computeEqueRemap3(cv::Mat *mapX,cv::Mat *mapY);

void computeEqueRemap4(cv::Mat *mapX,cv::Mat *mapY);


void addBoarder(const cv::Mat &src,cv::Mat &result);

void extBoarderRight(const cv::Mat &src,cv::Mat &result);
void extBoarderLeft(const cv::Mat &src,cv::Mat &result);

void mirror(const cv::Mat &src,cv::Mat &result);
