#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>

std::ostringstream resolutionStr;
std::ostringstream formatStr;
std::ostringstream modeStr;
std::ostringstream brightnessStr;
std::ostringstream contrastStr;
std::ostringstream saturationStr;
std::ostringstream hueStr;
std::ostringstream gainStr;

int HUD_FONT ;
int HUD_TEXT_VERTICAL_SPACING;

int DISPLAY_TEXT_VERTICAL_SPACING=20;
int FONT=1;
using namespace std;

char* getCameraFormat(cv::VideoCapture &dev);

char* getCameraMode(cv::VideoCapture &dev);

void onTrackbarResolution( int, void* );

void onTrackbarFormat( int, void* );

void onTrackbarBrightness( int, void* );

void onTrackbarContrast( int, void* );

void onTrackbarSaturation( int, void* );

void onTrackbarHue( int, void* );

void onTrackbarGain( int, void* );

void hudInit();

void updateHud(cv::Mat &frame);


