#include "main.hpp"


int main ()
{
	/*create windows for src and result displaying */
	cv::namedWindow("src Img",cv::WINDOW_AUTOSIZE);		//passing the autosize option prevents user to resize the window
	cv::namedWindow("result Img",cv::WINDOW_AUTOSIZE);	//window's size is the same as the img resolution

	/*create Mat objects to hold Images*/
	cv::Mat srcImg,dstImg;

	/*load img file*/
	srcImg=cv::imread("./fish.jpg");

	/*display src img to src img window*/
	cv::imshow("src Img",srcImg);

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
	//from equerectangular image pixels x,y to spherical cordinates.
	//the radius of the sphere is ! for simplicity

	sphericalTheta 	= -M_PI * (1-2*equeX  /  EQUE_WIDTH);
	sphericalPhi	= M_PI_2 *(1-2*equeY  /  EQUE_HEIGHT );

	// compute cartesian coordinates of a pixel in 3d sphere
	sphericalX=cos(sphericalPhi)*sin(sphericalTheta);
	sphericalY=cos(sphericalPhi)*cos(sphericalTheta);
	sphericalZ=sin(sphericalPhi);

	//compute the polar cordinates of 3d points to planar circular image

	proTheta=atan2(sphericalZ,sphericalX);
	proPhi=atan2(sqrt(pow(sphericalX,2.0)+pow(sphericalZ,2.0)),sphericalY);

	planarTheta=proPhi/(FISHEYE_ANGLE/2.0);



}
