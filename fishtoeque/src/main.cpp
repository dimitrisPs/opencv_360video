#include "main.hpp"


int main ()
{
	/*create windows for src and result displaying */
	cv::namedWindow("src Img",cv::WINDOW_NORMAL);		//passing the autosize option prevents user to resize the window
	cv::namedWindow("result Img",cv::WINDOW_NORMAL);	//window's size is the same as the img resolution

	/*create Mat objects to hold Images*/
	cv::Mat srcImg,dstImg,dstImg2;


	cv::Mat mapX,mapY;

	cv::Mat mapX2,mapY2;

	cv::Mat extImg;


	/*load img file*/
//	srcImg=cv::imread("../../Desktop/test.jpg");
//	srcImg=cv::imread("./fish.jpg");
//	srcImg=cv::imread("./small.jpg");

	srcImg=cv::imread("./fishpat.jpg");
//	srcImg=cv::imread("./fishpat120.jpg");
//	srcImg=cv::imread("./fishpat220.jpg");


	/*the following five functions is when we want to remap square images (we cant see for over 180 degrees)*/
//	dstImg.create( srcImg.size().height,srcImg.size().width, srcImg.type() );
//	mapX.create( srcImg.size().height,srcImg.size().width, CV_32FC1 );
//	mapY.create( srcImg.size().height,srcImg.size().width, CV_32FC1 );

	//	computeEqueRemap(&mapX,&mapY);
	//	remap( srcImg, dstImg, mapX, mapY, CV_INTER_LINEAR, cv::BORDER_WRAP , cv::Scalar(0,0, 0) );

	/*copy the src image to an image with double the width */
	extImg.create(srcImg.size().height,srcImg.size().width*2, srcImg.type());
//	addBoarder(srcImg,extImg);
//	extBoarderLeft(srcImg,extImg);
	mirror(srcImg,extImg);

	dstImg2.create( srcImg.size().height,srcImg.size().width*2, srcImg.type() );
	mapX2.create( srcImg.size().height,srcImg.size().width*2, CV_32FC1 );
	mapY2.create( srcImg.size().height,srcImg.size().width*2, CV_32FC1 );


	computeEqueRemap4(&mapX2,&mapY2);
	remap( extImg, dstImg2, mapX2, mapY2, CV_INTER_LINEAR, cv::BORDER_CONSTANT , cv::Scalar(0,0, 0) );




	/*display src img to src img window*/
	cv::imshow("src Img",extImg);
	cv::imshow("result Img",dstImg2);

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

void computeEqueRemap2(cv::Mat *mapX,cv::Mat *mapY)
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



			sphericalTheta 	= -M_PI * (1-2*(float)(equeX)  /  (mapX->cols-1));//devide with width-1 to get number in[-pi/2,pi/2]
			sphericalPhi	= M_PI_2 *(1-2*(float)(equeY)  /  (mapX->rows-1));

			// compute cartesian coordinates of a pixel in 3d sphere
			sphericalX=cos(sphericalPhi)*sin(sphericalTheta);
			sphericalY=cos(sphericalPhi)*cos(sphericalTheta);
			sphericalZ=sin(sphericalPhi);

			//compute the polar coordinates of 3d points to planar circular image

			proTheta=atan2(sphericalZ,sphericalX);
			proPhi=atan2(sqrt(pow(sphericalX,2.0)+pow(sphericalZ,2.0)),sphericalY);


			planarRadius=proPhi/(FISHEYE_ANGLE/2.0);//must provide fisheye angle in rad
			planarTheta=proTheta;

			//compute pixel location to circular fisheye picture
			cirX=(1+planarRadius*cos(planarTheta))/2;
			cirY=(1+planarRadius*sin(planarTheta))/2;
			mapX->at<float>(equeY,equeX)=(cirX)*mapX->rows+mapX->rows/2;
			mapY->at<float>(equeY,equeX)=mapY->rows-(cirY)*mapY->rows;
	    }
	}
}


void computeEqueRemap3(cv::Mat *mapX,cv::Mat *mapY)
{
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



			sphericalTheta 	= -M_PI * (1-2*(float)(equeX)  /  (mapX->cols-1));//devide with width-1 to get number in[-pi/2,pi/2]
			sphericalPhi	= M_PI_2 *(1-2*(float)(equeY)  /  (mapX->rows-1));

			// compute cartesian coordinates of a pixel in 3d sphere
			sphericalX=cos(sphericalPhi)*sin(sphericalTheta);
			sphericalY=cos(sphericalPhi)*cos(sphericalTheta);
			sphericalZ=sin(sphericalPhi);

			//compute the polar coordinates of 3d points to planar circular image

			proTheta=atan2(sphericalZ,sphericalX);
			proPhi=atan2(sqrt(pow(sphericalX,2.0)+pow(sphericalZ,2.0)),sphericalY);


			planarRadius=proPhi/(FISHEYE_ANGLE/2.0);//must provide fisheye angle in rad
			planarTheta=proTheta;

			//compute pixel location to circular fisheye picture
			cirX=(1+planarRadius*cos(planarTheta))/2;
			cirY=(1+planarRadius*sin(planarTheta))/2;
			mapX->at<float>(equeY,equeX)=(cirX)*mapX->rows;
			mapY->at<float>(equeY,equeX)=mapY->rows-(cirY)*mapY->rows;
	    }
	}
}

void computeEqueRemap4(cv::Mat *mapX,cv::Mat *mapY)
{
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



			sphericalTheta 	= -M_PI * (1-2*(float)(equeX)  /  (mapX->cols-1));//devide with width-1 to get number in[-pi/2,pi/2]
			sphericalPhi	= M_PI_2 *(1-2*(float)(equeY)  /  (mapX->rows-1));

			// compute cartesian coordinates of a pixel in 3d sphere
			sphericalX=cos(sphericalPhi)*sin(sphericalTheta);
			sphericalY=cos(sphericalPhi)*cos(sphericalTheta);
			sphericalZ=sin(sphericalPhi);

			//compute the polar coordinates of 3d points to planar circular image


			if(sphericalTheta>M_PI_2)
			{
				proTheta=-atan2(sphericalZ,sphericalX);
				proPhi=atan2(sqrt(pow(sphericalX,2.0)+pow(sphericalZ,2.0)),sphericalY)-M_PI;
			}
			else if(sphericalTheta<-M_PI_2)
			{
				proTheta=-atan2(sphericalZ,sphericalX);
				proPhi=atan2(sqrt(pow(sphericalX,2.0)+pow(sphericalZ,2.0)),sphericalY)-M_PI;
			}
			else
			{
				proTheta=atan2(sphericalZ,sphericalX);
				proPhi=atan2(sqrt(pow(sphericalX,2.0)+pow(sphericalZ,2.0)),sphericalY);
			}

			planarRadius=proPhi/(FISHEYE_ANGLE/2.0);//must provide fisheye angle in rad
			planarTheta=(proTheta);


			//compute pixel location to circular fisheye picture
			cirX=(1+planarRadius*cos(planarTheta))/2;
			cirY=(1+planarRadius*sin(planarTheta))/2;


			if(sphericalTheta>M_PI_2||sphericalTheta<-M_PI_2)
			{



//				std::cout<<"sphericalX :"<<sphericalX<<"\t sphericalY: "<<sphericalY<<"\t sphericalZ: "<<sphericalX<<"\t planarRadius: "<<planarRadius<<"\t planarTheta: "<<(180/M_PI)*planarTheta<<"\t X: "<<(cirX)*mapX->rows+mapX->rows<<"\n";
//				std::cout<<"proTheta: "<<proTheta<<"\t proPhi: "<<proPhi<<"\n";
//				std::cout<<"planarRadius: "<<planarRadius<<"\t planarTheta: "<<planarTheta<<"\n";
				mapX->at<float>(equeY,equeX)=(cirX)*mapX->rows+mapX->rows;
				mapY->at<float>(equeY,equeX)=mapY->rows-(cirY)*mapY->rows;


			}
			else
			{


//				std::cout<<"sphericalX :"<<sphericalX<<"\t sphericalY: "<<sphericalY<<"\t sphericalZ: "<<sphericalX<<"\t planarRadius: "<<planarRadius<<"\t planarTheta: "<<(180/M_PI)*planarTheta<<"\t X: "<<(cirX)*mapX->rows+mapX->rows<<"\n";
//				std::cout<<"proTheta: "<<proTheta<<"\t proPhi: "<<proPhi<<"\n";
//				std::cout<<"planarRadius: "<<planarRadius<<"\t planarTheta: "<<planarTheta<<"\n";


				mapX->at<float>(equeY,equeX)=(cirX)*mapX->rows;
				mapY->at<float>(equeY,equeX)=mapY->rows-(cirY)*mapY->rows;


			}

			//compute pixel location to circular fisheye picture

	    }
	}
}

void addBoarder(const cv::Mat &src,cv::Mat &result)
{
	cv::Rect roi(src.cols/2,0,src.cols,src.rows);
	cv::Mat  roiDest = result(roi);
	src.copyTo(roiDest);
}

void extBoarderRight(const cv::Mat &src,cv::Mat &result)
{
	cv::Rect roi(0,0,src.cols,src.rows);
	cv::Mat  roiDest = result(roi);
	src.copyTo(roiDest);
}
void extBoarderLeft(const cv::Mat &src,cv::Mat &result)
{
	cv::Rect roi(src.cols,0,src.cols,src.rows);
	cv::Mat  roiDest = result(roi);
	src.copyTo(roiDest);
}

void mirror(const cv::Mat &src,cv::Mat &result)
{
	cv::Mat mirror;
	cv::flip(src,mirror,1);
	cv::Rect roileft(0,0,src.cols,src.rows);
	cv::Rect roiright(src.cols,0,src.cols,src.rows);
	cv::Mat  roi = result(roileft);
	src.copyTo(roi);
	roi = result(roiright);
	mirror.copyTo(roi);
}
