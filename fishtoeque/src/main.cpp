#include "main.hpp"


int main ()
{
	/*create windows for src and result displaying */
	cv::namedWindow("src Img",cv::WINDOW_NORMAL);		//passing the autosize option prevents user to resize the window
	cv::namedWindow("result Img",cv::WINDOW_NORMAL);	//window's size is the same as the img resolution

	/*create Mat objects to hold Images*/
	cv::Mat srcImg,dstImg;


	cv::Mat mapX,mapY;

	/*load img file*/
//	srcImg=cv::imread("./small.png");
//	srcImg=cv::imread("./fish.jpg");
	srcImg=cv::imread("./fishpat.jpg");


	dstImg.create( srcImg.size().height,srcImg.size().width, srcImg.type() );
	mapX.create( srcImg.size().height,srcImg.size().width, CV_32FC1 );
	mapY.create( srcImg.size().height,srcImg.size().width, CV_32FC1 );


//	std::cout<<srcImg.rows<<"  "<<srcImg.cols;



	computeEqueRemap(&mapX,&mapY);
	remap( srcImg, dstImg, mapX, mapY, CV_INTER_LINEAR, cv::BORDER_CONSTANT, cv::Scalar(0,0, 0) );



	/*display src img to src img window*/
	cv::imshow("src Img",srcImg);
	cv::imshow("result Img",dstImg);

	cv::waitKey();					//wait for user keyboard input and then exit
	return 0;
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
			//std::cout<<"sphericalTheta:"<<sphericalTheta<<"\tsphericalPhi:"<<sphericalPhi<<"\n";

			// compute cartesian coordinates of a pixel in 3d sphere
			sphericalX=cos(sphericalPhi)*sin(sphericalTheta);
			sphericalY=cos(sphericalPhi)*cos(sphericalTheta);
			sphericalZ=sin(sphericalPhi);
			//std::cout<<"sphericalX:"<<sphericalX<<"\tsphericalY:"<<sphericalY<<"\tsphericalZ:"<<sphericalZ<<"\n";

			//compute the polar coordinates of 3d points to planar circular image

			proTheta=atan2(sphericalZ,sphericalX);
			proPhi=atan2(sqrt(pow(sphericalX,2.0)+pow(sphericalZ,2.0)),sphericalY);

			//std::cout<<"proTheta:"<<proTheta<<"\tproPhi:"<<proPhi<<"\n";

			planarRadius=proPhi/(FISHEYE_ANGLE/2.0);//must probide fisheye angle in rad
			planarTheta=proTheta;

			//std::cout<<"planarRadius:"<<planarRadius<<"\tplanarTheta:"<<planarTheta<<"\n";
			//compute pixel location to circular fisheye picture
			cirX=(1+planarRadius*cos(planarTheta))/2;
			cirY=(1+planarRadius*sin(planarTheta))/2;
			mapX->at<float>(equeY,equeX)=(cirX)*mapX->cols;
			mapY->at<float>(equeY,equeX)=mapX->rows-(cirY)*mapX->rows;
			//std::cout<<"x:"<<mapX->at<float>(equeY,equeX)<<"\ty:"<<mapY->at<float>(equeY,equeX)<<"\n";

	    }
	}
}
