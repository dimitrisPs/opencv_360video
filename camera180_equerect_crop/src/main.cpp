/*
 * main.cpp
 *
 *  Created on: Oct 21, 2016
 *      Author: lary
 */
#include <opencv2/opencv.hpp>
#define FISHEYE_ANGLE 184 * ( M_PI /180 )//3.22885912//M_PI//
void computeEqueRemap(cv::Mat *mapX,cv::Mat *mapY);
int main()
{
	cv::Mat initFrame;
	cv::Mat croppedFrame;
	cv::Mat equeRectFrame;
	cv::Mat mapX,mapY;
	cv::Rect fisheyeRoi(256,128,789,789);
//	cv::Rect fisheyeRoi(247,153,794,794);
	cv::VideoCapture camera(0);
	cv::namedWindow("cropped",cv::WINDOW_AUTOSIZE);
	cv::namedWindow("eque",cv::WINDOW_AUTOSIZE);

	//set camera parameters
	camera.set(CV_CAP_PROP_FRAME_WIDTH,1280);
	camera.set(CV_CAP_PROP_FRAME_HEIGHT,1024);


	//compute remap
	camera >> initFrame;
	croppedFrame = initFrame(fisheyeRoi);
	equeRectFrame.create( croppedFrame.size().height,croppedFrame.size().width, croppedFrame.type() );
	mapX.create( croppedFrame.size().height,croppedFrame.size().width, CV_32FC1 );
	mapY.create( croppedFrame.size().height,croppedFrame.size().width, CV_32FC1 );
	computeEqueRemap(&mapX,&mapY);

	//main loop
	while(1)
	{

		//get frame
		camera >> initFrame;

		//crop frame
		croppedFrame = initFrame(fisheyeRoi);
		//compute eque image
		remap( croppedFrame, equeRectFrame, mapX, mapY, CV_INTER_LINEAR, cv::BORDER_WRAP , cv::Scalar(0,0, 0) );

		//display
		cv::imshow("cropped",croppedFrame);
		cv::imshow("eque",equeRectFrame);
		if (cv::waitKey(33)>=0)
			break;
	}
}

void computeEqueRemap(cv::Mat *mapX,cv::Mat *mapY)
{
	/*we need to implement the reverse mapping and not the forward.
	 * that is we have to compute the transformation from plane to sphere
	 * and then from sphere to 2d circular image.
	 * Equations from http://paulbourke.net/dome/fish2/index.html
	 */

	float planarTheta, planarRadius;
	float sphericalTheta,sphericalPhi;
	float sphericalX,sphericalY,sphericalZ;
	float proTheta,proPhi;//projection angles
	const float sphericalRadius=1;

	int equeX,equeY;
	float cirX,cirY;
	for( int equeY = 0; equeY < mapX->rows; equeY++ )
	{
		for( int equeX = 0;equeX < mapX->cols;equeX++ )
	    {
			//from equerectangular image pixels x,y to spherical cordinates.
			//the radius of the sphere is ! for simplicity


			// remember for dual fisheye we must change theta to be from -pi to pi
			sphericalTheta 	= -M_PI_2 * (1-2*(float)(equeX)  /  (mapX->cols-1));//devide with width-1 to get number in[-pi/2,pi/2]
			sphericalPhi	= M_PI_2 *(1-2*(float)(equeY)  /  (mapX->rows-1));

			// compute cartesian coordinates of a pixel in 3d sphere
			sphericalX=cos(sphericalPhi)*sin(sphericalTheta);
			sphericalY=cos(sphericalPhi)*cos(sphericalTheta);
			sphericalZ=sin(sphericalPhi);

			//compute the polar coordinates of 3d points to planar circular image

			proTheta=atan2(sphericalZ,sphericalX);
			proPhi=atan2(sqrt(pow(sphericalX,2.0)+pow(sphericalZ,2.0)),sphericalY);


			planarRadius=proPhi/(FISHEYE_ANGLE/2.0);//must probide fisheye angle in rad
			planarTheta=proTheta;

			//compute pixel location to circular fisheye picture
			cirX=(1+planarRadius*cos(planarTheta))/2;
			cirY=(1+planarRadius*sin(planarTheta))/2;
			mapX->at<float>(equeY,equeX)=(cirX)*mapX->cols;
			mapY->at<float>(equeY,equeX)=mapX->rows-(cirY)*mapX->rows;

	    }
	}
}
