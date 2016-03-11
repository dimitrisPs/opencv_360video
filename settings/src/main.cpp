#include "main.hpp"



cv::VideoCapture camera(1);
int main ()
{
	std::ostringstream displayStr;
	bool disp=true;

	//create windows
	cv::namedWindow("settings", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("camera preview",CV_WINDOW_NORMAL);
	//
	int trResolution=5,trFormat=0,trBrightness=50,trContrast=50,
		trSaturation=50,trHue=50,trGain=0;


	//create trackbars
	cv::createTrackbar( "resolution", "settings", &trResolution, 6, onTrackbarResolution );
	cv::createTrackbar( "format", "settings", &trFormat, 1, onTrackbarFormat );
	cv::createTrackbar( "brightness", "settings", &trBrightness, 100, onTrackbarBrightness );
	cv::createTrackbar( "contrast", "settings", &trContrast, 100, onTrackbarContrast );
	cv::createTrackbar( "saturation", "settings", &trSaturation, 100, onTrackbarResolution );
	cv::createTrackbar( "hue", "settings", &trHue, 100, onTrackbarFormat );
	cv::createTrackbar( "gain", "settings", &trGain, 100, onTrackbarBrightness );
	cv::Mat frame;
	if (!camera.isOpened())
		std::cout<<"camera could not be opened\n";
	camera.set(CV_CAP_PROP_FRAME_HEIGHT,1024);
	camera.set(CV_CAP_PROP_FRAME_WIDTH,1280);

	HUD_FONT=3;
	HUD_TEXT_VERTICAL_SPACING=20*HUD_FONT;

	hudInit();



	while(1)
	{
		camera>>frame;

		if (disp)
		{
			updateHud(frame);
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
	resolutionStr.str("");
	resolutionStr <<" resolution: ";
	switch ( res )
	{
	case 0:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,320);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,240);
		HUD_FONT=1;
		HUD_TEXT_VERTICAL_SPACING=20*HUD_FONT;
		break;
	case 1:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,640);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,480);
		HUD_FONT=1;
		HUD_TEXT_VERTICAL_SPACING=20*HUD_FONT;

		break;
	case 2:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,800);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,600);
		HUD_FONT=2;
		HUD_TEXT_VERTICAL_SPACING=20*HUD_FONT;
		break;
	case 3:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,1024);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,768);
		HUD_FONT=2;
		HUD_TEXT_VERTICAL_SPACING=20*HUD_FONT;

		break;
	case 4:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,1280);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,720);
		HUD_FONT=2;
		HUD_TEXT_VERTICAL_SPACING=20*HUD_FONT;
		break;
	case 5:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,1280);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,1024);
		HUD_FONT=3;
		HUD_TEXT_VERTICAL_SPACING=20*HUD_FONT;
		break;
	case 6:
		camera.set(CV_CAP_PROP_FRAME_WIDTH,1920);
		camera.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
		HUD_FONT=3;
		HUD_TEXT_VERTICAL_SPACING=20*HUD_FONT;
		break;
	}
	resolutionStr <<camera.get(CV_CAP_PROP_FRAME_WIDTH)<< "x" << camera.get(CV_CAP_PROP_FRAME_HEIGHT);

}
void onTrackbarFormat( int format, void* )
{
	formatStr.str("");
	formatStr <<" format    : ";
	switch(format)
	{
		case 0:
			camera.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M', 'J', 'P', 'G'));
			break;
		case 1:
			camera.set(CV_CAP_PROP_FOURCC,CV_FOURCC('Y', 'U', 'Y', 'V'));
			break;
			//na kanw update ta fps
	}
	formatStr << getCameraFormat(camera);
}
void onTrackbarBrightness( int brightness, void* )
{
	float val=(float)brightness*0.01;
	camera.set(CV_CAP_PROP_BRIGHTNESS,val);
	brightnessStr.str("");
	brightnessStr <<" brightness  : "<< val;
}
void onTrackbarContrast( int contrast, void* )
{
	float val=(float)contrast*0.01;
	camera.set(CV_CAP_PROP_CONTRAST,val);
	contrastStr.str("");
	contrastStr <<" contrast  : "<< val;
}
void onTrackbarSaturation( int saturation, void* )
{
	float val=(float)saturation*0.01;
	camera.set(CV_CAP_PROP_SATURATION,val);
	saturationStr.str("");
	saturationStr <<" saturation: "<< val;
}
void onTrackbarHue( int hue, void* )
{
	float val=(float)hue*0.01;
	camera.set(CV_CAP_PROP_HUE,val);
	hueStr.str("");
	hueStr <<" hue       : "<< val;
}
void onTrackbarGain( int gain, void* )
{
	camera.set(CV_CAP_PROP_GAIN,(float)gain*0.01);
	gainStr.str("");
	gainStr <<" gain      : "<< camera.get(CV_CAP_PROP_GAIN);
}

void hudInit()
{
	resolutionStr 	<<" resolution: "<< camera.get(CV_CAP_PROP_FRAME_WIDTH)<< "x" << camera.get(CV_CAP_PROP_FRAME_HEIGHT);
	formatStr 		<<" format    : "<< getCameraFormat(camera);
	modeStr 		<<" mode      : "<< camera.get(CV_CAP_PROP_FORMAT);//getCameraMode(camera);
	brightnessStr 	<<" brightness: "<< camera.get(CV_CAP_PROP_BRIGHTNESS);
	contrastStr 	<<" contrast  : "<< camera.get(CV_CAP_PROP_CONTRAST);
	saturationStr 	<<" saturation: "<< camera.get(CV_CAP_PROP_SATURATION);
	hueStr 			<<" hue       : "<< camera.get(CV_CAP_PROP_HUE);
	gainStr 		<<" gain      : "<< camera.get(CV_CAP_PROP_GAIN);
}

void updateHud(cv::Mat &frame)
{
	cv::putText(frame, resolutionStr.str()	, cv::Point(5,HUD_TEXT_VERTICAL_SPACING)  , cv::FONT_HERSHEY_PLAIN, HUD_FONT, cv::Scalar(255,255,255), 1);
	cv::putText(frame, formatStr.str()		, cv::Point(5,HUD_TEXT_VERTICAL_SPACING*2), cv::FONT_HERSHEY_PLAIN, HUD_FONT, cv::Scalar(255,255,255), 1);
	cv::putText(frame, modeStr.str()		, cv::Point(5,HUD_TEXT_VERTICAL_SPACING*3), cv::FONT_HERSHEY_PLAIN, HUD_FONT, cv::Scalar(255,255,255), 1);
	cv::putText(frame, brightnessStr.str()	, cv::Point(5,HUD_TEXT_VERTICAL_SPACING*4), cv::FONT_HERSHEY_PLAIN, HUD_FONT, cv::Scalar(255,255,255), 1);
	cv::putText(frame, contrastStr.str()	, cv::Point(5,HUD_TEXT_VERTICAL_SPACING*5), cv::FONT_HERSHEY_PLAIN, HUD_FONT, cv::Scalar(255,255,255), 1);
	cv::putText(frame, saturationStr.str()	, cv::Point(5,HUD_TEXT_VERTICAL_SPACING*6), cv::FONT_HERSHEY_PLAIN, HUD_FONT, cv::Scalar(255,255,255), 1);
	cv::putText(frame, hueStr.str()			, cv::Point(5,HUD_TEXT_VERTICAL_SPACING*7), cv::FONT_HERSHEY_PLAIN, HUD_FONT, cv::Scalar(255,255,255), 1);
	cv::putText(frame, gainStr.str()		, cv::Point(5,HUD_TEXT_VERTICAL_SPACING*8), cv::FONT_HERSHEY_PLAIN, HUD_FONT, cv::Scalar(255,255,255), 1);
}


