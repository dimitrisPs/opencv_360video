#include "main.hpp"
using namespace std;

cv::VideoCapture camera(0);

int main(){

      if(!camera.isOpened()){
             cout << "Error opening video stream or file" << endl;
             return -1;
      }

   int frame_width=   camera.get(CV_CAP_PROP_FRAME_WIDTH);
   int frame_height=   camera.get(CV_CAP_PROP_FRAME_HEIGHT);
   cv::VideoWriter video("out.avi",CV_FOURCC('M','J','P','G'),10, cv::Size(frame_width,frame_height),true);

   for(;;){

       cv::Mat frame;
       camera >> frame;
       video.write(frame);
//       imshow( "Frame", frame );
       char c = (char)cv::waitKey(33);
       if( c == 27 ) break;
    }
  return 0;
}
