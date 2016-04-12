
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <boost/filesystem.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/calib3d.hpp"

namespace fs = boost::filesystem;
using namespace std;


#define PATTERN_WIDTH 	6
#define PATTERN_HEIGHT	8
#define SQUARE_SIZE 	24.5



void get_all(const fs::path& root, const string& ext, vector<fs::path>& ret);
void calcBoardCornerPositions(cv::Size boardSize, float squareSize, vector<cv::Point3f>& corners);

void saveGoodImagesPaths(const fs::path& root,vector<fs::path>& paths);


void savePatPoints(vector<vector<cv::Point2f > >& corners);

void saveCalibResults(cv::Mat K, cv::Mat D);
void loadFileList(const string listPath,vector <fs::path> &paths);
void loadCorners(const string cornerListPath,vector<vector<cv::Point2f > >& corners);
