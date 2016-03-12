/*
 * camera360.hpp
 *
 *  Created on: Mar 11, 2016
 *      Author: dimitris
 */

#ifndef CAMERA360_HPP_
#define CAMERA360_HPP_
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <cerrno>



#define LENS_ANGLE 185

using namespace std;

enum
{
	CAMERA360_RESOLUTION_320_X_240   = 1,
	CAMERA360_RESOLUTION_640_X_480   = 2,
	CAMERA360_RESOLUTION_800_X_600   = 3,
	CAMERA360_RESOLUTION_1024_X_768  = 4,
	CAMERA360_RESOLUTION_1280_X_720  = 5,
	CAMERA360_RESOLUTION_1280_X_1024 = 6,
	CAMERA360_RESOLUTION_1920_X_1080 = 7
};

enum
{
	CAMERA360_FORMAT_MPEG = 1,
	CAMERA360_FORMAT_YUYV = 2
};

enum
{
	CAMERA360_HUD_SETTINGS_ENABLE  = 1,
	CAMERA360_HUD_SETTINGS_DISABLE = 2
};

enum
{
	CAMERA360_SENSOR_BOATH = 0,
	CAMERA360_SENSOR_ONE   = 1,
	CAMERA360_SENSOR_TWO   = 2
};

enum
{
	CAMERA360_ROI1 = 1,
	CAMERA360_ROI2 = 2
};

enum
{
	CAMERA360_INIT_FRAME_1 = 1,
	CAMERA360_INIT_FRAME_2 = 2
};

enum
{
	CAMERA360_SENSOR_1 = 1,
	CAMERA360_SENSOR_2 = 2,
};
enum
{
	CAMERA360_NO_CROP = 1,
	CAMERA360_NO_CROP = 2,
};


class	Camera360
{
	//Camera360();

public:
	Camera360(int cameraIdx1,int cameraIdx2);
	~Camera360();
	//get propetry functions

	cv::Point getResolution();

	std::string getFormat();

	double getBrightness();

	double getContrast();

	double getSaturation();

	double getHue();

	double getGain();

	//set property functions

	//prepei na setarw ta fps
	bool setResolution(int resolutionId);

	//prepei na setarw ta fps
	bool setFormat(int formatId);

	bool setBrightness(double val);

	bool setContrast(double val);

	bool setSaturation(double val);

	bool setHue(double val);

	bool setGain(double val);

	void setSettingsHud(int HUDState);

	//get frames

	bool readFrame(cv::Mat &frameMat, int sensorId=0);

	// compute remapping transformations

	void computeEquerectangularTransformation();

	void setCropArea(int sensorId,cv::Rect roi);



	//operator overloading

	Camera360 operator >> (cv::Mat &frame);

private:

	void cropFrame(int frameId,int roiId);

	bool setResolutionHelper(int width,int height);

	void cropFrame(int frameId);

	void mergeFrames();

	bool readCropParameters(int sensorId);



	bool cameraCanStream;
	bool flipCropSetting;
	bool displayParamHUD;			//for displying parameters on main frame
	bool sensor1HasCropParameters;
	bool sensor2HasCropParameters;

	cv::Point parameterSensorsResolution;
	cv::Point parameterMergedResolution;
	double 	  parameterContrast;
	double    parameterBrightness;
	double    parameterGain;
	double    parameterHue;
	double 	  parameterSaturation;
	string 	  format;

	std::ostringstream HUDresolutionStr;
	std::ostringstream HUDformatStr;
	std::ostringstream HUDmodeStr;
	std::ostringstream HUDbrightnessStr;
	std::ostringstream HUDcontrastStr;
	std::ostringstream HUDsaturationStr;
	std::ostringstream HUDhueStr;
	std::ostringstream HUDgainStr;


	cv::Mat	cameraInitialFrame1;
	cv::Mat	cameraInitialFrame2;
	cv::Mat mergedFrame;
	cv::Mat outputFrame;
	cv::Mat equeMapX;
	cv::Mat equeMapY;
	cv::Rect cameraROI1;
	cv::Rect cameraROI2;

	cv::VideoCapture sensor1;
	cv::VideoCapture sensor2;

};





#endif /* CAMERA360_HPP_ */
