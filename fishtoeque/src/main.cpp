#include "main.hpp"


int main ()
{
	/*create windows for src and result displaying */
	cv::namedWindow("src Img",cv::WINDOW_AUTOSIZE);		//passing the autosize option prevents user to resize the window
	cv::namedWindow("result Img",cv::WINDOW_AUTOSIZE);	//window's size is the same as the img resolution

	/*create Mat objects to hold Images*/
	cv::Mat srcImg,dstImg;
	srcImg=cv::imread("./fish.jpg");//open img file
	cv::imshow("src Img",srcImg);	//display src img to src img window
	cv::waitKey();					//wait for user keyboard input and then exit
	return 0;
}
