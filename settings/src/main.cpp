#include "main.hpp"





#define DISPLAY_SETTINGS 1



cv::VideoCapture camera(1);
int main ()
{
	std::ostringstream displayStr;
	bool disp=true;

	//create windows
	cv::namedWindow("settings", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("camera preview",CV_WINDOW_NORMAL);
	//
	int trResolution=5,trFormat=0,trBrightness=50,trContrast=50;


	//create trackbars
	cv::createTrackbar( "resolution", "settings", &trResolution, 7, onTrackbarResolution );
	cv::createTrackbar( "format", "settings", &trFormat, 1, onTrackbarFormat );
	cv::createTrackbar( "brightness", "settings", &trBrightness, 100, onTrackbarBrightness );
	cv::createTrackbar( "contrast", "settings", &trContrast, 100, onTrackbarContrast );
	cv::Mat frame;
	if (!camera.isOpened())
		std::cout<<"camera could not be opened\n";
	camera.set(CV_CAP_PROP_FRAME_HEIGHT,1024);
	camera.set(CV_CAP_PROP_FRAME_WIDTH,1280);
	resolutionStr 	<<" resolution: "<< camera.get(CV_CAP_PROP_FRAME_WIDTH)<< "x" << camera.get(CV_CAP_PROP_FRAME_HEIGHT);
	formatStr 		<<" format    : "<< camera.get(CV_CAP_PROP_FOURCC);//getCameraFormat(camera);
	modeStr 		<<" mode      : "<< camera.get(CV_CAP_PROP_FORMAT);//getCameraMode(camera);
	brightnessStr 	<<" brightness: "<< camera.get(CV_CAP_PROP_BRIGHTNESS);
	contrastStr 	<<" contrast  : "<< camera.get(CV_CAP_PROP_CONTRAST);
	saturationStr 	<<" saturation: "<< camera.get(CV_CAP_PROP_SATURATION);
	hueStr 			<<" hue       : "<< camera.get(CV_CAP_PROP_HUE);
	gainStr 		<<" gain      : "<< camera.get(CV_CAP_PROP_GAIN);

	while(1)
	{
		camera>>frame;

		if (disp)
		{
			int font=2;
			int verticalSpacing=20*font;
			cv::putText(frame, resolutionStr.str()	, cv::Point(5,verticalSpacing)	, cv::FONT_HERSHEY_PLAIN, font, cv::Scalar(255,255,255), 1);
			cv::putText(frame, formatStr.str()		, cv::Point(5,verticalSpacing*2), cv::FONT_HERSHEY_PLAIN, font, cv::Scalar(255,255,255), 1);
			cv::putText(frame, modeStr.str()		, cv::Point(5,verticalSpacing*3), cv::FONT_HERSHEY_PLAIN, font, cv::Scalar(255,255,255), 1);
			cv::putText(frame, brightnessStr.str()	, cv::Point(5,verticalSpacing*4), cv::FONT_HERSHEY_PLAIN, font, cv::Scalar(255,255,255), 1);
			cv::putText(frame, contrastStr.str()	, cv::Point(5,verticalSpacing*5), cv::FONT_HERSHEY_PLAIN, font, cv::Scalar(255,255,255), 1);
			cv::putText(frame, saturationStr.str()	, cv::Point(5,verticalSpacing*6), cv::FONT_HERSHEY_PLAIN, font, cv::Scalar(255,255,255), 1);
			cv::putText(frame, hueStr.str()			, cv::Point(5,verticalSpacing*7), cv::FONT_HERSHEY_PLAIN, font, cv::Scalar(255,255,255), 1);
			cv::putText(frame, gainStr.str()		, cv::Point(5,verticalSpacing*8), cv::FONT_HERSHEY_PLAIN, font, cv::Scalar(255,255,255), 1);

		}

		//display frame
		cv::imshow("camera preview",frame);

		if (cv::waitKey(20)>=0) break;
	}

	return 0;
}

char* getCameraFormat(cv::VideoCapture &dev)
{
	int ex = static_cast<int>(dev.get(CV_CAP_PROP_FOURCC));
	char EXT[] = {ex & 0XFF , (ex & 0XFF00) >> 8,(ex & 0XFF0000) >> 16,(ex & 0XFF000000) >> 24, 0};
	return EXT;
}
char* getCameraMode(cv::VideoCapture &dev)
{
	int ex = static_cast<int>(dev.get(CV_CAP_PROP_MODE));
	char EXT[] = {ex & 0XFF , (ex & 0XFF00) >> 8,(ex & 0XFF0000) >> 16,(ex & 0XFF000000) >> 24, 0};
	return EXT;
}
void onTrackbarResolution( int res, void* )
{
	switch ( res )
	{
	case 0:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,320);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,240);
		break;
	case 1:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,640);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,480);

		break;
	case 2:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,800);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,600);
		break;
	case 3:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,1024);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,768);

		break;
	case 4:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,1280);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,720);
		break;
	case 5:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,1280);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,1024);
		break;
	case 6:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,1920);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
		break;
	}

}
void onTrackbarFormat( int format, void* )
{
	switch(format)
	{
		case 0:
			camera.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M', 'J', 'P', 'G'));
			break;
		case 1:
			camera.set(CV_CAP_PROP_FOURCC,CV_FOURCC('Y', 'U', 'Y', 'V'));
			break;
		//PREPEI NA KANW UPDATE KAI TA FPS
	}
}
void onTrackbarBrightness( int brightness, void* )
{
	camera.set(CV_CAP_PROP_BRIGHTNESS,(float)brightness*0.01);

}
void onTrackbarContrast( int contrast, void* )
{
	camera.set(CV_CAP_PROP_CONTRAST,(float)contrast*0.01);
}
void onTrackbarSaturation( int saturation, void* )
{
	camera.set(CV_CAP_PROP_SATURATION,(float)saturation*0.01);
}
void onTrackbarHue( int hue, void* )
{
	camera.set(CV_CAP_PROP_HUE,(float)hue*0.01);
}
void onTrackbarGain( int gain, void* )
{
	camera.set(CV_CAP_PROP_GAIN,(float)gain*0.01);
}

