#include "main.hpp"

float perTheta,perPhi;
cv::Mat mapX,mapY;
cv::Mat frame;
bool recompute=false;


int main ()
{

	cv::Mat gray;

	cv::Mat perspective;

//	cv::VideoCapture video("/home//Desktop/jakarta_city_night_drive_1080.mp4");
//	cv::VideoCapture video("/home//Desktop/Ele.mp4");
	cv::VideoCapture video("/home//Desktop/v1.mp4");

	cv::namedWindow("video");



	int fps = video.get(CV_CAP_PROP_FPS);
	cv::Size frameSize(video.get(CV_CAP_PROP_FRAME_WIDTH),video.get(CV_CAP_PROP_FRAME_HEIGHT));
	int videoLengthFps=video.get(CV_CAP_PROP_FRAME_COUNT);
	int videoPosFps=0;
	int framTime=(float)1000/(float)fps;

	cv::createTrackbar("duration", "video", &videoPosFps, videoLengthFps );

	cv::setMouseCallback("video", CallBackFunc, NULL);

	equeToPerspective(0,0,mapX,mapY,frameSize);

	video >>frame;
	cv::cvtColor(frame,gray,CV_BGR2GRAY);
	perspective.create(mapX.rows,mapX.cols,frame.type());
	while(!frame.empty())
	{
		cv::imshow("video",gray);

		//get next frame
		video>>frame;
		cv::cvtColor(frame,gray,CV_BGR2GRAY);
		if (recompute)
		{
			equeToPerspective(perTheta,perPhi,mapX,mapY,frameSize);
			recompute=false;
		}
		remap( frame, perspective, mapX, mapY, CV_INTER_LINEAR, cv::BORDER_CONSTANT , cv::Scalar(0,0, 0) );


		cv::setTrackbarPos("duration", "video", ++videoPosFps);
		cv::imshow("persp",perspective);
		if (cv::waitKey(framTime)>=0)
			break;
	}
	return 0;
}


void equeToPerspective(float thetaZero, float phiZero, cv::Mat &mapX,cv::Mat &mapY,cv::Size equeSize)
{
	double thetaOffset =45*M_PI/180.0 ;
	double phiOffset = 45*M_PI/180.0 ;

	float tempX,tempY;
	double theta0=thetaZero,phi0=phiZero;
	double planeWidth= 2*tan(thetaOffset);
	double planeHeight= 2*tan(phiOffset);


	int pwidth = 400;
	int pheight=pwidth;

	mapX.create(pwidth,pheight,CV_32FC1);
	mapY.create(pwidth,pheight,CV_32FC1);


	double dx=planeWidth/(double)pwidth;
	double dy=planeHeight/(double)pheight;

	double rx,ry;			//relative position of perspective pixels
	double c,r;
	double phi, theta;
	for( int py = 0; py < mapY.cols; py++ )
	{
		for( int px = 0;px < mapX.rows; px++ )
		{
			rx=(px-pwidth/2)*dx;
			ry=(pheight/2-py)*dy;

			r=sqrt(rx*rx+ry*ry);
			c=atan(r);

			theta=theta0-atan2(r*cos(phi0)*cos(c)-ry*sin(phi0)*sin(c), rx*sin(c));
			phi=asin(cos(c)*sin(phi0)+ (ry*sin(c)*cos(phi0)/r));


			tempX=theta/M_PI* (float)equeSize.width/2-equeSize.width/4;
			tempY=equeSize.height/2  -  phi/M_PI_2* (float)equeSize.height/2;

			if (tempX>equeSize.width)
				mapX.at<float>(py,px)=tempX-equeSize.width;
			else if (tempX<0)
				mapX.at<float>(py,px)=tempX+equeSize.width;
			else
				mapX.at<float>(py,px)=tempX;


			if (tempY>equeSize.height)
				mapY.at<float>(py,px)=tempY-equeSize.height;
			else if (tempY<0)
				mapY.at<float>(py,px)=tempY+equeSize.height;
			else
				mapY.at<float>(py,px)=tempY;

		}
	}
}


void CallBackFunc(int event, int x, int y, int flags, void* userdata)
{
	float width= frame.cols;
	float height = frame.rows;
	if  ( event == cv::EVENT_LBUTTONDOWN )
	{
		perTheta=(float)x/width*2*M_PI-M_PI;
		perPhi=M_PI/2-(float)y/height*M_PI;
		recompute=true;
		std::cout << "theta: " << perTheta *180/M_PI << "\t phi " << perPhi*180/M_PI  << std::endl;
	}
//     else if  ( event == cv::EVENT_RBUTTONDOWN )
//     {
//          cout << "Right button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
//     }
//     else if  ( event == cv::EVENT_MBUTTONDOWN )
//     {
//          cout << "Middle button of the mouse is clicked - position (" << x << ", " << y << ")" << endl;
//     }
//     else if ( event == cv::EVENT_MOUSEMOVE )
//     {
//          cout << "Mouse move over the window - position (" << x << ", " << y << ")" << endl;

//     }
}
