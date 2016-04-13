#include "main.hpp"




int main ()
{

	//read all jpeg files' paths and put it in a vector
	vector <fs::path> jpegPaths;
	fs::path rootFolder("/home/dimitris/Pictures/calib_sample_sens_3/");//home/dimitris/Desktop/sens4///home/dimitris/Pictures/calib_sample_sens_3
	const string jpegExtensionStr =".jpg";
	cv::namedWindow("img preview",cv::WINDOW_NORMAL);
	int keyWaitTime;
	bool goodSampleFound;

	//see if there is a log file for all good images
	if ( fs::exists( rootFolder.string() + "goodSamples.txt" ) )
	{
		cout<<"selected images file found"<<endl;
		loadFileList((rootFolder.string() + "goodSamples.txt" ), jpegPaths);
		keyWaitTime=500;
		goodSampleFound=true;
	}
	else
	{
		get_all(rootFolder, jpegExtensionStr, jpegPaths);
		keyWaitTime=0;
		goodSampleFound=false;
	}

	cout<<"sample images "<<endl;



	vector< vector<cv::Point2f> > patternPoints_sensor;
	cv::Size patternSize(PATTERN_WIDTH,PATTERN_HEIGHT);

//	if (fs::exists( "./corners.txt" ) )
//	{
//		loadCorners("./corners.txt" , patternPoints_sensor);
//	}
//	else
//	{

		for (uint i=0;i<jpegPaths.size();i++)
			cout<<jpegPaths[i].string()<<endl;
		//perform chessboard detection for every image, delete bad images from lists
		cv::Mat currentChessFrame;
		cv::Mat tmpGrayFrame;

		vector<cv::Point2f> tmpPoints;
		bool tmpFound;
		string tmpImgPath;
		int numOfFails=0;
		int usrInput;



		for (uint imgIdx=0;imgIdx<jpegPaths.size();++imgIdx)
		{

			cout<<jpegPaths[imgIdx].filename().string();
			currentChessFrame=cv::imread(jpegPaths[imgIdx].string());
			tmpFound= cv::findChessboardCorners( currentChessFrame, patternSize,tmpPoints ,
					  CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);
			if (tmpFound)
			{
				cvtColor(currentChessFrame, tmpGrayFrame, cv::COLOR_BGR2GRAY);
				cv::cornerSubPix( tmpGrayFrame, tmpPoints, cv::Size(11,11),
					cv::Size(-1,-1), cv::TermCriteria( cv::TermCriteria::EPS+cv::TermCriteria::COUNT, 30, 0.1 ));

				cout<<" succeeded in corner extrusion.";
				cv::drawChessboardCorners( currentChessFrame, patternSize, cv::Mat(tmpPoints), tmpFound );
				cv::imshow("img preview",currentChessFrame);

				cout <<"   is the extraction correct? [y/n]"<<endl;
				usrInput=cv::waitKey(keyWaitTime);
				if (keyWaitTime==0)
				{
					if (usrInput=='n' || usrInput=='N')
						tmpFound=false;
					else
						patternPoints_sensor.push_back(tmpPoints);
				}
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

		if (!goodSampleFound)
			saveGoodImagesPaths(rootFolder,jpegPaths);
		savePatPoints(patternPoints_sensor);
//	}

	//construct vector with world pat points

	vector<vector<cv::Point3f> > objectPoints(1);
	calcBoardCornerPositions(patternSize, SQUARE_SIZE, objectPoints[0]);
	objectPoints.resize(patternPoints_sensor.size(),objectPoints[0]);

	cout<< "size of objPoints "<<objectPoints.size()<<endl;
	cv::Mat cameraMatrix = cv::Mat::eye(3, 3, CV_64F);
	cv::Mat distCoeffs = cv::Mat::zeros(8, 1, CV_64F);



    int flags = 0;
    flags |= cv::fisheye::CALIB_RECOMPUTE_EXTRINSIC;
    flags |= cv::fisheye::CALIB_CHECK_COND;
    flags |= cv::fisheye::CALIB_FIX_SKEW;
//    flags |= cv::fisheye::CALIB_USE_INTRINSIC_GUESS ;
//    flags |= cv::fisheye::CALIB_FIX_K1 ;
//    flags |= cv::fisheye::CALIB_FIX_K2 ;
//    flags |= cv::fisheye::CALIB_FIX_K3 ;
//    flags |= cv::fisheye::CALIB_FIX_K4 ;

    cv::Matx33d K;
//    K(0,0)=25;//1.3*DIGITAL_MULTIPLIER;
//    K(0,1)=0;
//    K(0,2)=650;
//
//    K(1,0)=0;
//    K(1,1)=K(0,0);
//    K(1,2)=523;
//
//    K(2,0)=0;
//    K(2,1)=0;
//    K(2,2)=1;


    cv::Vec4d D;
    std::vector<cv::Vec3d> rvec;
    std::vector<cv::Vec3d> tvec;
    cout<<"test frame "<<jpegPaths[0].string() <<endl;
    cv::Mat testFrame=cv::imread(jpegPaths[0].string().c_str());
    cv::Size imgSize(testFrame.size());

    double error=cv::fisheye::calibrate(objectPoints, patternPoints_sensor, imgSize, K, D,
    		rvec, tvec, flags,cv::TermCriteria(3, 20, 1e-6));//oti einai novects exei na kanei me extrinsic parameters

    cout<<"error is "<<error<<endl;


    cv::Mat originalImage=cv::imread(jpegPaths[0].string().c_str());
    cv::Mat persp;


    cv::fisheye::undistortImage(originalImage, persp, K, D, cv::Mat::eye(3, 3, CV_64F),originalImage.size());
    saveCalibResults(K, D);
    cv::namedWindow("persp",cv::WINDOW_NORMAL);
    cv::imshow("img preview",originalImage);
    cv::imshow("persp",persp);
    cv::waitKey(0);
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
        	ret.push_back(it->path());
        ++it;
    }
    sort(ret.begin(), ret.end());

}


void calcBoardCornerPositions(cv::Size boardSize, float squareSize, vector<cv::Point3f>& corners)
{
    corners.clear();

	for( int i = 0; i < boardSize.height; ++i )
		for( int j = 0; j < boardSize.width; ++j )
			corners.push_back(cv::Point3f(j*squareSize, i*squareSize, 0));
}
void saveGoodImagesPaths(const fs::path& root,vector<fs::path>& paths)
{
	ostringstream filePath;
	filePath<<root.string() << "goodSamples.txt";
	ofstream myfile;
	myfile.open (filePath.str().c_str());

	for (uint i=0;i<paths.size();++i)
	{
		myfile<<root.string()<<paths[i].string()<<endl;
	}
	myfile.close();
}
void savePatPoints(vector<vector<cv::Point2f > >& corners)
{
	string filePath="./corners.txt";
	ofstream myfile;
	myfile.open (filePath.c_str());

	for (uint image=0;image<corners.size();++image)
	{
		for (uint point=0;point<corners[image].size();++point)
		{
			myfile<<(corners[image])[point].x<<" "<<(corners[image])[point].y<<" ";
		}
		myfile<<endl;
	}

	myfile.close();
}

void saveCalibResults(cv::Matx33d K, cv::Vec4d D)
{
	string filePath="./calib_results.txt";
	ofstream myfile;
	myfile.open (filePath.c_str());
	for (int rows=0;rows<K.rows;++rows)
	{
		for (int cols=0;cols<K.cols;++cols)
		{
			myfile<<K(rows,cols)<<" ";
		}
		myfile<<"\n";
	}
	for (int rows=0;rows<K.rows;++rows)
	{
		myfile<<D(rows)<<" ";
	}

	myfile.close();
}

void loadFileList(const string listPath,vector <fs::path> &paths)
{
	string currentFilePath;
	ifstream myfile (listPath.c_str());
	paths.clear();
	if (myfile.is_open())
	{
		while ( getline (myfile,currentFilePath) )
		{
		  paths.push_back(fs::path(currentFilePath));
		}
		myfile.close();
	}

	else cout << "Unable to open file" <<listPath<<endl;
}

void loadCorners(const string cornerListPath,vector<vector<cv::Point2f > >& corners)
{
	string currentFilePath;
	stringstream  lineStream;
	string tmpline;
	float tmpVal_x,tmpVal_y;
	ifstream myfile (cornerListPath.c_str());
	vector <cv::Point2f> tmpPoints;
	corners.clear();
	if (myfile.is_open())
	{
		while ( getline (myfile,tmpline) )//for every line in file (points for every image)
		{
			tmpPoints.clear();
			lineStream.str(tmpline);
			while(lineStream>>tmpVal_x)
			{
				lineStream>>tmpVal_y;
				tmpPoints.push_back(cv::Point2f(tmpVal_x,tmpVal_y));
			}
			corners.push_back(tmpPoints);
		}
		myfile.close();
	}

	else cout << "Unable to open file" <<cornerListPath<<endl;
}
