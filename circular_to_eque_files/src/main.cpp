/*
 * main.cpp
 *
 *  Created on: Oct 21, 2016
 *      Author: lary
 *      to trexeis sto folder me ta jpegs tou fisheye kai sou ta metatrepei se equrectangular projection
 */
#include <opencv2/opencv.hpp>
#define BOOST_FILESYSTEM_VERSION 3
#define BOOST_FILESYSTEM_NO_DEPRECATED
#include <boost/filesystem.hpp>
#include <string>
#include <vector>
#include <stdio.h>
#define FISHEYE_ANGLE 184 * ( M_PI /180 )//3.22885912//M_PI//
namespace fs = ::boost::filesystem;

// return the filenames of all files that have the specified extension




void computeEqueRemap(cv::Mat *mapX,cv::Mat *mapY);
void get_all(const fs::path& root, const std::string& ext, std::vector<fs::path>& ret);
int main()
{
	std::vector <fs::path> filenames;
	cv::Mat croppedFrame;
	cv::Mat equeRectFrame;
	cv::Mat mapX,mapY;
	cv::Rect fisheyeRoi(256,128,789,789);
//	cv::Rect fisheyeRoi(247,153,794,794);

	//find all files in the directory
	get_all("./", ".png", filenames);



	if (filenames.size()==0)//no files found
		return-1;

	//compute remap
	croppedFrame = cv::imread(filenames[0].string(), CV_LOAD_IMAGE_COLOR);
	equeRectFrame.create( croppedFrame.size().height,croppedFrame.size().width, croppedFrame.type() );
	mapX.create( croppedFrame.size().height,croppedFrame.size().width, CV_32FC1 );
	mapY.create( croppedFrame.size().height,croppedFrame.size().width, CV_32FC1 );
	computeEqueRemap(&mapX,&mapY);
	std::stringstream saveName;
	//main loop
	for(int i=0;i<filenames.size();i++)
	{

		//get image
		croppedFrame = cv::imread(filenames[i].string(), CV_LOAD_IMAGE_COLOR);
		//compute eque image
		remap( croppedFrame, equeRectFrame, mapX, mapY, CV_INTER_LINEAR, cv::BORDER_WRAP , cv::Scalar(0,0, 0) );

		//save image
		saveName.str("");
		saveName<<"./"<<"eque_"<<filenames[i].filename().string();
//
		imwrite(saveName.str(), equeRectFrame );

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
// in the specified directory and all subdirectories
void get_all(const fs::path& root, const std::string& ext, std::vector<fs::path>& ret)
{
    if(!fs::exists(root) || !fs::is_directory(root))
    	return;

    fs::recursive_directory_iterator it(root);
    fs::recursive_directory_iterator endit;

    while(it != endit)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ext)
        {
        	ret.push_back(it->path());
        	std::string tmp=it->path().string();
        	std::cout<<tmp<<std::endl;
        }
        ++it;
    }
}

