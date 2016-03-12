#include "main.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "camera360.hpp"

Camera360 camera(1,2);

int main ()
{

	cv::namedWindow("settings", CV_WINDOW_AUTOSIZE);
	cv::namedWindow("camera preview",CV_WINDOW_NORMAL);
	cv::Mat mainFrame;

	int trResolution=5,trFormat=0,trBrightness=50,trContrast=50,
		trSaturation=50,trHue=50,trGain=0;

	cv::createTrackbar("resolution", "settings", &trResolution, 6, onTrackbarResolution );
	cv::createTrackbar( "format", "settings", &trFormat, 1, onTrackbarFormat );
	cv::createTrackbar( "brightness", "settings", &trBrightness, 100, onTrackbarBrightness );
	cv::createTrackbar( "contrast", "settings", &trContrast, 100, onTrackbarContrast );
	cv::createTrackbar( "saturation", "settings", &trSaturation, 100, onTrackbarResolution );
	cv::createTrackbar( "hue", "settings", &trHue, 100, onTrackbarFormat );
	cv::createTrackbar( "gain", "settings", &trGain, 100, onTrackbarBrightness );
	while(1)
	{
		camera.readFrame(mainFrame);
		cv::imshow("test",mainFrame);
		if(cv::waitKey(10)>=0)
			break;
	}
	return 0;
}



void onTrackbarResolution( int res, void* )
{
	switch ( res )
	{
	case 0:
		camera.setResolution(CAMERA360_RESOLUTION_320_X_240);
		break;
	case 1:
		camera.setResolution(CAMERA360_RESOLUTION_640_X_480);
		break;
	case 2:
		camera.setResolution(CAMERA360_RESOLUTION_800_X_600);
		break;
	case 3:
		camera.setResolution(CAMERA360_RESOLUTION_1024_X_768);
		break;
	case 4:
		camera.setResolution(CAMERA360_RESOLUTION_1280_X_720);
		break;
	case 5:
		camera.setResolution(CAMERA360_RESOLUTION_1280_X_1024);
		break;
	case 6:
		camera.setResolution(CAMERA360_RESOLUTION_1920_X_1080);
		break;
	}
}

void onTrackbarFormat( int format, void* )
{
	switch(format)
	{
		case 0:
			camera.setFormat(CAMERA360_FORMAT_MPEG);
			break;
		case 1:
			camera.setFormat(CAMERA360_FORMAT_YUYV);
			break;
	}
}

void onTrackbarBrightness( int brightness, void* )
{
	float val=(float)brightness*0.01;
	camera.setBrightness(val);
}
void onTrackbarContrast( int contrast, void* )
{
	float val=(float)contrast*0.01;
	camera.setContrast(val);
}
void onTrackbarSaturation( int saturation, void* )
{
	float val=(float)saturation*0.01;
	camera.setSaturation(val);

}
void onTrackbarHue( int hue, void* )
{
	float val=(float)hue*0.01;
	camera.setHue(val);

}
void onTrackbarGain( int gain, void* )
{
	float val=(float)gain*0.01;
	camera.setGain(val);

}
