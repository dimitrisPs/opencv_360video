#include "opencv2/core/core.hpp"
#include <string>

using namespace std;

#define FISHEYE_ANGLE 180 * ( M_PI /180 )//3.22885912//M_PI//

bool DISPLAY_HUD=false;
bool CROP_MODE=false;
bool EQUE_MODE=false;
int FOO;

bool CROP_SENSOR_FRONT = false;
bool CROP_SENSOR_REAR  = false;

int CROP_START_X=0;
int CROP_START_Y=0;
int CROP_WIDTH=0;
int CROP_HEIGHT=0;
cv::Rect cropROI;
stringstream mainWindowStatus;
stringstream cropSetingsSaveLabel;

void onTrackbarResolution( int, void* );

void onTrackbarFormat( int, void* );

void onTrackbarBrightness( int, void* );

void onTrackbarContrast( int, void* );

void onTrackbarSaturation( int, void* );

void onTrackbarHue( int, void* );

void onTrackbarGain( int, void* );

void onSensorFlip(int state, void *pointer);

void onHUDPushed(int state, void *pointer);

void onCropModePushed(int state, void *pointer);

void onEqueModePushed(int state, void *pointer);

void onCropFrontSelect(int state, void *pointer);

void onCropRearSelect(int state, void *pointer);

void onCropRearSelect(int state, void *pointer);

void onCropSave(int state, void *pointer);

void computeEqueRemap(cv::Mat *mapX,cv::Mat *mapY);
