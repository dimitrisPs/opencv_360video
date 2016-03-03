#include "main.hpp"


int main ()
{
	cv::Mat frame;
	cv::namedWindow("video",cv::WINDOW_AUTOSIZE);
	cv::VideoCapture camera(0);
	while(1)
	{
		camera>>frame;
		cv::imshow("video",frame);
		if(cv::waitKey(10)>=0) break;
	}
	return 0;
}
