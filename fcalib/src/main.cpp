#include "main.hpp"


using namespace std;


int main ()
{

	//read all jpeg files' paths and put it in a vector
	vector <fs::path> jpegPaths;
	fs::path rootFolder("/home/dimitris/Pictures/calib_sample_sens_3/");
	const string jpegExtensionStr =".jpg";
	get_all(rootFolder, jpegExtensionStr, jpegPaths);
	sort(jpegPaths.begin(), jpegPaths.end());



	//perform chessboard detection for every image, delete bad images from lists
	cv::Mat currentChessFrame;
	cv::Mat tmpGrayFrame;
	cv::Size patternSize(6,8);
	vector< vector<cv::Point2f> > patternPoints_sensor;
	vector<cv::Point2f> tmpPoints;
	vector <bool> patternsFound;
	bool tmpFound;
	string tmpImgPath;
	int numOfFails=0;
	int originalImageSampleSize=jpegPaths.size();
	for (uint imgIdx=0;imgIdx<jpegPaths.size();++imgIdx)
	{

		cout<<jpegPaths[imgIdx].string();;
		tmpImgPath = rootFolder.string()+jpegPaths[imgIdx].string();
		currentChessFrame=cv::imread(tmpImgPath);
		tmpFound= cv::findChessboardCorners( currentChessFrame, patternSize,tmpPoints ,
				  CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
		patternsFound.push_back(tmpFound);
		if (tmpFound)
		{
			cvtColor(currentChessFrame, tmpGrayFrame, cv::COLOR_BGR2GRAY);
            cv::cornerSubPix( tmpGrayFrame, tmpPoints, cv::Size(11,11),
                cv::Size(-1,-1), cv::TermCriteria( cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 30, 0.1 ));
			patternPoints_sensor.push_back(tmpPoints);
			cout<<" succeeded in corner extrusion"<<endl;
			cv::drawChessboardCorners( currentChessFrame, patternSize, cv::Mat(tmpPoints), tmpFound );
			cv::imshow("img",currentChessFrame);
			cv::waitKey(0);
		}
		else
		{
			cout<<" fails, removed from list"<<endl;
			jpegPaths.erase(jpegPaths.begin()+imgIdx);
			numOfFails++;
			imgIdx--;
		}
	}
	cout<<numOfFails<< " images FAILED"<<endl<<(originalImageSampleSize-numOfFails)<<" images succeeded in corner extrusion"<<endl;


	return 0;
}












void get_all(const fs::path& root, const std::string& ext, std::vector<fs::path>& ret)
{
    if(!fs::exists(root) || !fs::is_directory(root)) return;

    fs::recursive_directory_iterator it(root);
    fs::recursive_directory_iterator endit;

    while(it != endit)
    {
        if(fs::is_regular_file(*it) && it->path().extension() == ext)
        	ret.push_back(it->path().filename());
        ++it;

    }

}
