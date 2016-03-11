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
	case 4:
		return this->setResolutionHelper(1280,1024);
		break;
	case 5:
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
	if (testRes.x==width & testRes.y==height)
		return true;
	return false;
}

bool Camera360::setFormat(int formatId)
{
	return true;
}

bool Camera360::setBrightness(double val)
{
	return true;
}

bool Camera360::setContrast(double val)
{
	return true;
}

bool Camera360::setSaturation(double val)
{
	return true;
}

bool Camera360::sethue(double val)
{
	return true;
}

bool Camera360::setGain(double val)
{
	return true;
}

bool Camera360::setSettingsHud(int HUDState)
{
	return true;
}




