/*
 * camera360.cpp
 *
 *  Created on: Mar 11, 2016
 *      Author: dimitris
 */
#include "camera360.hpp"


cv::Point Camera360::getResolution()
{
	int tempHeight,tempWidth;
	cv::Point p;
	tempHeight = this->sensor1.get(CV_CAP_PROP_FRAME_HEIGHT);
	tempWidth = this->sensor1.get(CV_CAP_PROP_FRAME_WIDTH);
	if (tempHeight != this->sensor2.get(CV_CAP_PROP_FRAME_HEIGHT) )
	{
		p.x=-1;
		p.y=-1;
	}
	else if  (tempWidth!=this->sensor2.get(CV_CAP_PROP_FRAME_WIDTH))
	{
		p.x=-1;
		p.y=-1;
	}
	else
	{
		p.x=tempWidth;
		p.y=tempHeight;
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
		return true;
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
		return true;
	else if ( (formatId==2) & (this->getFormat().compare("YUYV")) )
		return true;
	return false;
}

bool Camera360::setBrightness(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_BRIGHTNESS,val);
	this->sensor2.set(CV_CAP_PROP_BRIGHTNESS,val);
	if (this->getBrightness()==val)
		return true;
	return false;
}

bool Camera360::setContrast(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_CONTRAST,val);
	this->sensor2.set(CV_CAP_PROP_CONTRAST,val);
	if (this->getContrast()==val)
		return true;
	return false;
}

bool Camera360::setSaturation(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_SATURATION,val);
	this->sensor2.set(CV_CAP_PROP_SATURATION,val);
	if (this->getSaturation()==val)
		return true;
	return false;
}

bool Camera360::setHue(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_HUE,val);
	this->sensor2.set(CV_CAP_PROP_HUE,val);
	if (this->getHue()==val)
		return true;
	return false;
}

bool Camera360::setGain(double val)
{
	if (val>1)
		return false;
	this->sensor1.set(CV_CAP_PROP_GAIN,val);
	this->sensor2.set(CV_CAP_PROP_GAIN,val);
	if (this->getGain()==val)
		return true;
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

		//Merge frames

		//Apply projections

		break;
	case 1:
		if (this->flipCropSetting)
			this->sensor1.read(this->cameraInitialFrame2);
		else
			this->sensor2.read(this->cameraInitialFrame1);
		break;
	case 2:
		if (this->flipCropSetting)
			this->sensor2.read(this->cameraInitialFrame1);
		else
			this->sensor2.read(this->cameraInitialFrame2);
		break;
	}
	//if either frame is empty stop
	if (this->cameraInitialFrame1.empty() | this->cameraInitialFrame2.empty())
		return false;

	return false;
}
void Camera360::readCropParameters()
{
	ofstream roiFile;
	int sWidthStr,sHeightStr,widthStr,heightStr;
	roiFile.open ("../sensor1_roi.txt",ios::in);
	if (!roiFile.is_open())
		return;
	roiFile >> sWidthStr >> sHeightStr >> widthStr >> heightStr;
	this->cameraROI1.Rect_(sWidthStr,sHeightStr,widthStr,heightStr);
	roiFile.close();
	roiFile.open ("../sensor2_roi.txt",ios::in);
	if (!roiFile.is_open())
		return;
	roiFile >> sWidthStr >> sHeightStr >> widthStr >> heightStr;
	this->cameraROI2.Rect_(sWidthStr,sHeightStr,widthStr,heightStr);
}

void cropFrame(int frameId)
{


}




