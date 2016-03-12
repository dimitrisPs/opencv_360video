#include "main.hpp"
#include "opencv2/opencv.hpp"
#include "camera360.hpp"



int main ()
{
	Camera360 camera(1,2);
	cv::namedWindow("test");
	cv::Mat mainFrame;
	while(1)
	{
		camera.readFrame(mainFrame);
		cv::imshow("test",mainFrame);
		if(cv::waitKey(10)>=0)
			break;
	}
	return 0;
}
