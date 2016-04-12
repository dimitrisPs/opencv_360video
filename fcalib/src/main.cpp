#include "main.hpp"


using namespace std;


int main ()
{

	//read all jpeg files' paths and put it in a vector
	vector <fs::path> jpegPaths;
	fs::path rootFolder("/home/lary/Pictures/calib_sample_sens_3/");
	const string jpegExtensionStr =".jpg";
	get_all(rootFolder, jpegExtensionStr, jpegPaths);




	//perform chessboard detection for every image, delete bad images from lists
	cv::Mat currentChessFrame;
	cv::Mat tmpGrayFrame;
	cv::Size patternSize(PATTERN_WIDTH,PATTERN_HEIGHT);
	vector< vector<cv::Point2f> > patternPoints_sensor;
	vector<cv::Point2f> tmpPoints;
	vector <bool> patternsFound;
	bool tmpFound;
	string tmpImgPath;
	int numOfFails=0;
	int usrInput;



	for (uint imgIdx=0;imgIdx<20;++imgIdx)
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

			cout<<" succeeded in corner extrusion.";
			cv::drawChessboardCorners( currentChessFrame, patternSize, cv::Mat(tmpPoints), tmpFound );
			cv::imshow("img",currentChessFrame);

			cout <<"   is the extraction correct? [y/n]"<<endl;
			usrInput=cv::waitKey(0);
			if (usrInput=='n' || usrInput=='N')
				tmpFound=false;
			else
				patternPoints_sensor.push_back(tmpPoints);

		}
		if (!tmpFound)
		{
			cout<<" fails, removed from list"<<endl;
			jpegPaths.erase(jpegPaths.begin()+imgIdx);
			numOfFails++;
			imgIdx--;
		}
	}
	cout<<numOfFails<< " images FAILED"<<endl<<(jpegPaths.size())<<" images succeeded in corner extrusion"<<endl;

	//construct vector with world pat points

	vector<vector<cv::Point3f> > objectPoints(1);
	calcBoardCornerPositions(patternSize, SQUARE_SIZE, objectPoints[0]);
	objectPoints.resize(patternPoints_sensor.size(),objectPoints[0]);


	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64F);

    int flags = 0;
    flags |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flags |= cv::fisheye::CALIB_CHECK_COND;
    flags |= cv::fisheye::CALIB_FIX_SKEW;

    cv::Mat K;
    cv::Mat D;
    cv::fisheye::calibrate(objectPoints, patternPoints_sensor, currentChessFrame.size(), K, D,
            cv::noArray(), cv::noArray(), flags);//oti einai novects exei na kanei me extrinsic parameters





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
    sort(ret.begin(), ret.end());

}

static void calcBoardCornerPositions(cv::Size boardSize, float squareSize, vector<cv::Point3f>& corners)
{
    corners.clear();

	for( int i = 0; i < boardSize.height; ++i )
		for( int j = 0; j < boardSize.width; ++j )
			corners.push_back(cv::Point3f(j*squareSize, i*squareSize, 0));
}
