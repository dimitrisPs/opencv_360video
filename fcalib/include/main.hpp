
#include <math.h>
#include <stdio.h>
#include <iostream>

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

#define PATTERN_WIDTH 	6
#define PATTERN_HEIGHT	8
#define SQUARE_SIZE 	24.5



void get_all(const fs::path& root, const std::string& ext, std::vector<fs::path>& ret);
static void calcBoardCornerPositions(cv::Size boardSize, float squareSize, std::vector<cv::Point3f>& corners);
