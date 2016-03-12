/*
 * camera360.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: dimitris
 */
#include "camera360.hpp"




Camera360::Camera360(int cameraIdx1,int cameraIdx2)
{
	this->sensor1.open(cameraIdx1);
	if (!this->sensor1.isOpened())
	{
		cerr<<"camera 1 could not be opened"<<endl;
		this->cameraCanStream=false;
		return;
	}
	this->sensor2.open(cameraIdx2);
	if (!this->sensor2.isOpened())
	{
		cerr<<"camera 2 could not be opened"<<endl;
		this->cameraCanStream=false;
		return;
	}
	this->cameraCanStream=true;
	//default settings
	if (!this->setBrightness(0.5) |
		!this->setContrast(0.5) |
		!this->setGain(0) |
		!this->setResolution(CAMERA360_RESOLUTION_1280_X_1024) |
		!this->setHue(0.5))
	{
		cerr<<"couldn't set correctly camera parameters"<<endl;
		return;
	}
	if (!this->readCropParameters(CAMERA360_SENSOR_1))
	{
		cerr<<"camera parameters for first sensor not found"<<endl;
		this->cameraCanStream=false;
		this->sensor1HasCropParameters=false;
		this->cameraROI1.x=0;
		this->cameraROI1.y=0;
		this->cameraROI1.width=this->sensor1.get(CV_CAP_PROP_FRAME_WIDTH);
		this->cameraROI1.height=this->sensor1.get(CV_CAP_PROP_FRAME_HEIGHT);
	}
	else
		this->sensor1HasCropParameters=true;
	if  (!this->readCropParameters(CAMERA360_SENSOR_2))
	{
		cerr<<"camera parameters for second sensor not found"<<endl;
		this->cameraCanStream=false;
		this->sensor2HasCropParameters=false;
		this->cameraROI2.x=0;
		this->cameraROI2.y=0;
		this->cameraROI2.width=this->sensor2.get(CV_CAP_PROP_FRAME_WIDTH);
		this->cameraROI2.height=this->sensor2.get(CV_CAP_PROP_FRAME_HEIGHT);
	}
	else
		this->sensor2HasCropParameters=true;
	HUDParameterInit();
	this->displayParamHUD=false;
	this->flipCropSetting=false;
}

Camera360::~Camera360()
{
	this->sensor1.release();
	this->sensor2.release();
	this->cameraInitialFrame1.release();
	this->cameraInitialFrame2.release();
	this->equeMapX.release();
	this->equeMapY.release();

}

cv::Point Camera360::getResolution()
{
	int sensor1Height,sensor1Width, sensor2Height,sensor2Width;
	cv::Point p;
	sensor1Height = this->sensor1.get(CV_CAP_PROP_FRAME_HEIGHT);
	sensor1Width = this->sensor1.get(CV_CAP_PROP_FRAME_WIDTH);
	sensor2Height = this->sensor2.get(CV_CAP_PROP_FRAME_HEIGHT);
	sensor2Width = this->sensor2.get(CV_CAP_PROP_FRAME_WIDTH);
	if (sensor1Height != sensor1Height )
	{
		p.x=-1;
		p.y=-1;
	}
	else if  (sensor1Width!=sensor2Width)
	{
		p.x=-1;
		p.y=-1;
	}
	else
	{
		p.x=sensor1Width;
		p.y=sensor1Height;
	}
	return p;
}
string Camera360::getFormat()
{
	int ex1 = static_cast<int>(this->sensor1.get(CV_CAP_PROP_FOURCC));
	int ex2 = static_cast<int>(this->sensor2.get(CV_CAP_PROP_FOURCC));
	string out;
	char EXT1[] = {ex1 & 0XFF , (ex1 & 0XFF00) >> 8,(ex1 & 0XFF0000) >> 16,(ex1 & 0XFF000000) >> 24, 0};
	char EXT2[] = {ex2 & 0XFF , (ex2 & 0XFF00) >> 8,(ex2 & 0XFF0000) >> 16,(ex2 & 0XFF000000) >> 24, 0};
	if (strcmp(EXT1,EXT2))
	{
		out=EXT1;
		return out;
	}
	out="/0";
	return out;
}
double Camera360::getBrightness()
{
	double bVal1 = this->sensor1.get(CV_CAP_PROP_BRIGHTNESS);
	double bVal2 =this->sensor1.get(CV_CAP_PROP_BRIGHTNESS);
	if (bVal1==bVal2)
		return bVal1;
	return -1;
}
double Camera360::getContrast()
{
	double cVal1 = this->sensor1.get(CV_CAP_PROP_CONTRAST);
	double cVal2 =this->sensor1.get(CV_CAP_PROP_CONTRAST);
	if (cVal1==cVal2)
		return cVal1;
	return -1;
}
double Camera360::getSaturation()
{
	double sVal1 = this->sensor1.get(CV_CAP_PROP_SATURATION);
	double sVal2 =this->sensor1.get(CV_CAP_PROP_SATURATION);
	if (sVal1==sVal2)
		return sVal1;
	return -1;
}

double Camera360::getHue()
{
	double hVal1 = this->sensor1.get(CV_CAP_PROP_HUE);
	double hVal2 =this->sensor1.get(CV_CAP_PROP_HUE);
	if (hVal1==hVal2)
		return hVal1;
	return -1;
}
double Camera360::getGain()
{
	double gVal1 = this->sensor1.get(CV_CAP_PROP_GAIN);
	double gVal2 =this->sensor1.get(CV_CAP_PROP_GAIN);
	if (gVal1==gVal2)
		return gVal1;
	return -1;
}
bool Camera360::setResolution(int resolutionId)
{
	switch (resolutionId)
	{
	case 1:
		return this->setResolutionHelper(320,240);
		break;
	case 2:
		return this->setResolutionHelper(640,480);
		break;
	case 3:
		return this->setResolutionHelper(800,600);
		break;
	case 4:
		return this->setResolutionHelper(1024,768);
		break;
	case 5:
		return this->setResolutionHelper(1280,720);
		break;
	case 6:
		return this->setResolutionHelper(1280,1024);
		break;
	case 7:
		return this->setResolutionHelper(1920,1080);
		break;
	}
	return false;
}

bool Camera360::setResolutionHelper(int width,int height)
{
	cv::Point testRes;
	this->sensor1.set(CV_CAP_PROP_FRAME_WIDTH,width);
	this->sensor1.set(CV_CAP_PROP_FRAME_HEIGHT,height);
	this->sensor2.set(CV_CAP_PROP_FRAME_WIDTH,width);
	this->sensor2.set(CV_CAP_PROP_FRAME_HEIGHT,height);
	testRes =this->getResolution();
	if ((testRes.x==width) & (testRes.y==height) )
	{
		this->HUDresolutionStr.str("");
		this->parameterSensorsResolution.x=width;
		this->parameterSensorsResolution.y=height;
		this->HUDresolutionStr	<<" resolution: "<< this->parameterSensorsResolution.x<< "x" << this->parameterSensorsResolution.y;
		return true;
	}
	return false;
}

bool Camera360::setFormat(int formatId)
{

	switch(formatId)
	{
		case 1:
			this->sensor1.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M', 'J', 'P', 'G'));
			this->sensor2.set(CV_CAP_PROP_FOURCC,CV_FOURCC('M', 'J', 'P', 'G'));
			break;
		case 2:
			this->sensor1.set(CV_CAP_PROP_FOURCC,CV_FOURCC('Y', 'U', 'Y', 'V'));
			this->sensor2.set(CV_CAP_PROP_FOURCC,CV_FOURCC('Y', 'U', 'Y', 'V'));
			break;
	}
	if ((formatId==1) & (this->getFormat().compare("MPEG")) )
	{
		this->parameterformat = "MPEG";
		this->HUDformatStr		<<" format    : "<< this->parameterformat;
		return true;
	}
	else if ( (formatId==2) & (this->getFormat().compare("YUYV")) )
	{
		this->parameterformat = "YUYV";
		this->HUDformatStr		<<" format    : "<< this->parameterformat;
		return true;
	}
	return false;
}

bool Camera360::setBrightness(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_BRIGHTNESS,val);
	this->sensor2.set(CV_CAP_PROP_BRIGHTNESS,val);
	if (this->getBrightness()==val)
	{
		this->parameterBrightness = val;
		this->HUDbrightnessStr	<<" brightness: "<< this->parameterBrightness;
		return true;
	}
	return false;
}

bool Camera360::setContrast(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_CONTRAST,val);
	this->sensor2.set(CV_CAP_PROP_CONTRAST,val);
	if (this->getContrast()==val)
	{
		this->parameterContrast= val;
		this->HUDcontrastStr	<<" contrast  : "<< this->parameterContrast;
		return true;
	}
	return false;
}

bool Camera360::setSaturation(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_SATURATION,val);
	this->sensor2.set(CV_CAP_PROP_SATURATION,val);
	if (this->getSaturation()==val)
	{
		this->parameterSaturation= val;
		this->HUDsaturationStr	<<" saturation: "<< this->parameterSaturation;
		return true;
	}
	return false;
}

bool Camera360::setHue(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_HUE,val);
	this->sensor2.set(CV_CAP_PROP_HUE,val);
	if (this->getHue()==val)
	{
		this->parameterHue=val;
		this->HUDhueStr			<<" hue       : "<< this->parameterHue;
		return true;
	}
	return false;
}

bool Camera360::setGain(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_GAIN,val);
	this->sensor2.set(CV_CAP_PROP_GAIN,val);
	if (this->getGain()==val)
	{
		this->parameterGain=val;
		this->HUDgainStr 		<<" gain      : "<< this->parameterGain;
		return true;
	}
	return false;
}

void Camera360::setSettingsHud(int HUDState)
{
	switch(HUDState)
	{
	case 1:
		this->displayParamHUD=true;
		break;
	case 2:
		this->displayParamHUD=true;
		break;
	}
}

bool Camera360::readFrame(cv::Mat &frameMat, int sensorId)
{
	switch(sensorId)
	{
	case 0:
		this->sensor1.grab();
		this->sensor2.grab();
		if (this->flipCropSetting)
		{
			this->sensor1.retrieve(this->cameraInitialFrame2);
			this->sensor2.retrieve(this->cameraInitialFrame1);
		}
		else
		{
			this->sensor1.retrieve(this->cameraInitialFrame1);
			this->sensor2.retrieve(this->cameraInitialFrame2);
		}
		//crop frames
		if (this->sensor1HasCropParameters)
			this->cameraInitialFrame1=cameraInitialFrame1(this->cameraROI1);
		if (this->sensor2HasCropParameters)
			this->cameraInitialFrame2=cameraInitialFrame2(this->cameraROI2);
		//Merge frames
		this->mergeFrames();
		//Apply projections
		frameMat=this->mergedFrame;
		break;
	case 1:
		if (this->flipCropSetting)
		{
			this->sensor1.read(this->cameraInitialFrame2);
			frameMat=this->cameraInitialFrame2;
		}
		else
		{
			this->sensor1.read(this->cameraInitialFrame1);
			frameMat=this->cameraInitialFrame1;
		}


		break;
	case 2:
		if (this->flipCropSetting)
		{
			this->sensor2.read(this->cameraInitialFrame1);
			frameMat=this->cameraInitialFrame1;
		}
		else
		{
			this->sensor2.read(this->cameraInitialFrame2);
			frameMat=this->cameraInitialFrame2;
		}
		break;
	}
	//if either frame is empty stop
	if (this->cameraInitialFrame1.empty() | this->cameraInitialFrame2.empty())
		return false;


	return false;
}
bool Camera360::readCropParameters(int sensorId)
{
	ifstream roiFile;
	int sWidthStr,sHeightStr,widthStr,heightStr;
	roiFile.open ("../sensor1_roi.txt",fstream::in);
	if (!roiFile.is_open())
		return false;
	roiFile >> sWidthStr >> sHeightStr >> widthStr >> heightStr;
	this->cameraROI1 = cv::Rect(sWidthStr,sHeightStr,widthStr,heightStr);
	roiFile.close();
	roiFile.open ("../sensor2_roi.txt",ios::in);
	if (!roiFile.is_open())
		return false;
	roiFile >> sWidthStr >> sHeightStr >> widthStr >> heightStr;
	this->cameraROI2 = cv::Rect(sWidthStr,sHeightStr,widthStr,heightStr);
	return true;
}

void Camera360::mergeFrames()
{
	int mergedHeight,mergedWidth;
	if (this->sensor1HasCropParameters | this->sensor2HasCropParameters)
	{
		if (this->cameraROI1.height > this->cameraROI2.height)
		{
			mergedHeight=this->cameraROI1.height;
		}
		else
		{
			mergedHeight=this->cameraROI2.height;
		}
		//this is because if the image is croped it will be square
		mergedWidth=mergedHeight;

		cv::resize(this->cameraInitialFrame1,this->cameraInitialFrame1,cv::Size(mergedHeight,mergedHeight));
		cv::resize(this->cameraInitialFrame2,this->cameraInitialFrame2,cv::Size(mergedHeight,mergedHeight));
	}
	else
	{
		//if they are not cropped will have the same resolution
		mergedHeight=this->cameraROI1.height;
		mergedWidth=this->cameraROI1.width;

	}
	cv::Rect rightResultROI(0,0,mergedWidth,mergedHeight);
	cv::Rect leftResultROI(mergedWidth,0,mergedWidth,mergedHeight);
	cv::Mat ResultROI;

	this->mergedFrame.create(mergedHeight,mergedWidth*2,this->cameraInitialFrame1.type());

	//resize images

	//mearge images
	ResultROI = this->mergedFrame(rightResultROI);
	this->cameraInitialFrame1.copyTo(ResultROI);
	ResultROI = this->mergedFrame(leftResultROI);
	this->cameraInitialFrame2.copyTo(ResultROI);
}

void Camera360::setCropArea(int sensorId,cv::Rect roi)
{

	stringstream filename;
	ofstream roiFile;
	filename<<"./sensor"<< sensorId<<"_"<<this->getResolution().x<<"x"<<this->getResolution().y<<".txt";
	roiFile.open(filename.str().c_str());
	roiFile<<roi.x<<"\t"<<roi.y<<"\t"<<roi.width<<"\t"<<roi.height;

	//update local class roi
	if (sensorId==1)
		this->cameraROI1=roi;
	else
		this->cameraROI2=roi;
}

void Camera360::HUDUpdateInfos()
{
	cv::putText(this->outputFrame, this->HUDresolutionStr.str()	, cv::Point(5,this->HUDVerticalSpacing)  , cv::FONT_HERSHEY_PLAIN, this->HUDFont, cv::Scalar(255,255,255), 1);
	cv::putText(this->outputFrame, this->HUDformatStr	.str()		, cv::Point(5,this->HUDVerticalSpacing*2), cv::FONT_HERSHEY_PLAIN, this->HUDFont, cv::Scalar(255,255,255), 1);
	cv::putText(this->outputFrame, this->HUDbrightnessStr.str()	, cv::Point(5,this->HUDVerticalSpacing*4), cv::FONT_HERSHEY_PLAIN, this->HUDFont, cv::Scalar(255,255,255), 1);
	cv::putText(this->outputFrame, this->HUDcontrastStr.str()	, cv::Point(5,this->HUDVerticalSpacing*5), cv::FONT_HERSHEY_PLAIN, this->HUDFont, cv::Scalar(255,255,255), 1);
	cv::putText(this->outputFrame, this->HUDsaturationStr.str()	, cv::Point(5,this->HUDVerticalSpacing*6), cv::FONT_HERSHEY_PLAIN, this->HUDFont, cv::Scalar(255,255,255), 1);
	cv::putText(this->outputFrame, this->HUDhueStr.str()			, cv::Point(5,this->HUDVerticalSpacing*7), cv::FONT_HERSHEY_PLAIN, this->HUDFont, cv::Scalar(255,255,255), 1);
	cv::putText(this->outputFrame, this->HUDgainStr .str()		, cv::Point(5,this->HUDVerticalSpacing*8), cv::FONT_HERSHEY_PLAIN, this->HUDFont, cv::Scalar(255,255,255), 1);


}

void Camera360::HUDParameterInit()
{

	this->parameterSensorsResolution= this->getResolution();
	this->parameterformat			= this->getFormat();
	this->parameterBrightness		= this->getBrightness();
	this->parameterContrast 		= this->getContrast();
	this->parameterSaturation		= this->getSaturation();
	this->parameterHue				= this->getHue();
	this->parameterGain				= this->getGain();

	this->HUDresolutionStr	<<" resolution: "<< this->parameterSensorsResolution.x<< "x" << this->parameterSensorsResolution.y;
	this->HUDformatStr		<<" format    : "<< this->parameterformat;
	this->HUDbrightnessStr	<<" brightness: "<< this->parameterBrightness;
	this->HUDcontrastStr	<<" contrast  : "<< this->parameterContrast;
	this->HUDsaturationStr	<<" saturation: "<< this->parameterSaturation;
	this->HUDhueStr			<<" hue       : "<< this->parameterHue;
	this->HUDgainStr 		<<" gain      : "<< this->parameterGain;
}
