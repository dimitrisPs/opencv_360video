#define _GLIBCXX_USE_CXX11_ABI 0
#include "main.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "camera360.hpp"

Camera360 camera(1,0);
int main ()
{
	bool first =true;
	cv::namedWindow("test",CV_WINDOW_AUTOSIZE);
	cv::Mat mainFrame;
	cv::Mat eque;
	cv::Mat mapX;
	cv::Mat mapY;

	int trResolution=5,trFormat=0,trBrightness=50,trContrast=50,
		trSaturation=50,trHue=50,trGain=0;

	cv::createTrackbar("resolution", "", &trResolution, 6, onTrackbarResolution );
	cv::createTrackbar( "format", "", &trFormat, 1, onTrackbarFormat );
	cv::createTrackbar( "brightness", "", &trBrightness, 100, onTrackbarBrightness );
	cv::createTrackbar( "contrast", "", &trContrast, 100, onTrackbarContrast );
	cv::createTrackbar( "saturation", "", &trSaturation, 100, onTrackbarResolution );
	cv::createTrackbar( "hue", "", &trHue, 100, onTrackbarFormat );
	cv::createTrackbar( "gain", "", &trGain, 100, onTrackbarBrightness );
	cv::createTrackbar( "track2", "", &trHue, 255, NULL);

	cv::createButton("flip sensors",onSensorFlip,NULL,CV_CHECKBOX,0);
	cv::createButton("HUD",onHUDPushed,NULL,CV_CHECKBOX,0);
	cv::createButton("Crop Mode",onCropModePushed,NULL,CV_CHECKBOX,0);
	cv::createButton("equerectangular Mode",onEqueModePushed,NULL,CV_CHECKBOX,0);
	cv::createButton("Crop Front Sensor",onCropFrontSelect,NULL,CV_RADIOBOX,0);
	cv::createButton("Crop Rear Sensor",onCropRearSelect,NULL,CV_RADIOBOX,0);





	while(1)
	{
		if (CROP_MODE)
		{
			mainWindowStatus.str("");
			mainWindowStatus <<"frame size :  "<< CROP_WIDTH-CROP_START_X<<" x "<<CROP_HEIGHT-CROP_START_Y;
			if (CROP_SENSOR_FRONT)
				camera.readFrame(mainFrame,CAMERA360_SENSOR_1,0);
			else
				camera.readFrame(mainFrame,CAMERA360_SENSOR_2,0);
			mainFrame=mainFrame(cv::Rect( CROP_START_X, CROP_START_Y,CROP_WIDTH-CROP_START_X,CROP_HEIGHT-CROP_START_Y));
			cv::displayStatusBar("test",mainWindowStatus.str());
		}
		else if (EQUE_MODE)
		{
			camera.readFrame(mainFrame,0,1);
			if (first)
			{
				eque.create( mainFrame.size().height,mainFrame.size().width, mainFrame.type() );
				mapX.create( eque.size().height,eque.size().width, CV_32FC1 );
				mapY.create( eque.size().height,eque.size().width, CV_32FC1 );
				computeEqueRemap(&mapX,&mapY);
				first=false;
			}
			cv::remap( mainFrame, mainFrame, mapX, mapY, CV_INTER_LINEAR, cv::BORDER_REFLECT , cv::Scalar(0,0, 0) );
		}
		else
		{
			camera.readFrame(mainFrame,CAMERA360_SENSOR_1,1);
		}
			cv::imshow("test",mainFrame);
		if(cv::waitKey(33)==27)
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
void onSensorFlip(int state, void *pointer)
{
	camera.flipSensors();
}
void onHUDPushed(int state, void *pointer)
{
	camera.setSettingsHud(state);
}

void onCropModePushed(int state, void *pointer)
{
	if (state)
	{
		if (!CROP_SENSOR_FRONT & !CROP_SENSOR_REAR)
			CROP_SENSOR_FRONT=true;
		CROP_START_X=0;
		CROP_START_Y=0;
		CROP_WIDTH=camera.getResolution().x;
		CROP_HEIGHT=camera.getResolution().y;
		cropSetingsSaveLabel.str("");
		if (CROP_SENSOR_FRONT)
			cropSetingsSaveLabel<<"Save Front \nSensor Crop Settings";
		if(CROP_SENSOR_REAR)
			cropSetingsSaveLabel<<"Save Rear \nSensor Crop Settings";
		cv::createTrackbar(cropSetingsSaveLabel.str(),"test", &FOO ,1,onCropSave);
		cv::createTrackbar("start x","test",&CROP_START_X,camera.getResolution().x,NULL);
		cv::createTrackbar("start x","test",&CROP_START_X,camera.getResolution().x,NULL);
		cv::createTrackbar("start y","test",&CROP_START_Y,camera.getResolution().y,NULL);
		cv::createTrackbar("width","test",&CROP_WIDTH,camera.getResolution().x,NULL);
		cv::createTrackbar("height","test",&CROP_HEIGHT,camera.getResolution().y,NULL);
		CROP_MODE=true;
	}
	else
	{
		CROP_MODE=false;
		cv::destroyWindow("test");
	}
}
void onEqueModePushed(int state, void *pointer)
{
	EQUE_MODE = state;
}

void onCropFrontSelect(int state, void *pointer)
{
	CROP_SENSOR_FRONT = true;
	CROP_SENSOR_REAR =false;
}

void onCropRearSelect(int state, void *pointer)
{
	CROP_SENSOR_FRONT = false;
	CROP_SENSOR_REAR =true;
}
void onCropSave(int state, void *pointer)
{
	if (state)
	{
		if (CROP_SENSOR_FRONT)
		{
			camera.setCropArea(CAMERA360_SENSOR_1,cv::Rect( CROP_START_X, CROP_START_Y,CROP_WIDTH-CROP_START_X,CROP_HEIGHT-CROP_START_Y));
		}
		else
		{
			camera.setCropArea(CAMERA360_SENSOR_2,cv::Rect( CROP_START_X, CROP_START_Y,CROP_WIDTH-CROP_START_X,CROP_HEIGHT-CROP_START_Y));
		}

	}
}
void computeEqueRemap(cv::Mat *mapX,cv::Mat *mapY)
{
	float planarTheta, planarRadius;
	float sphericalTheta,sphericalPhi;
	float sphericalX,sphericalY,sphericalZ;
	float proTheta,proPhi;//projection angles
	const float sphericalRadius=1;

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
				mapX->at<float>(equeY,equeX)=(cirX)*mapX->rows+mapX->rows;
				mapY->at<float>(equeY,equeX)=mapY->rows-(cirY)*mapY->rows;
			}
			else
			{
				mapX->at<float>(equeY,equeX)=(cirX)*mapX->rows;
				mapY->at<float>(equeY,equeX)=mapY->rows-(cirY)*mapY->rows;
			}
	    }
	}
}

