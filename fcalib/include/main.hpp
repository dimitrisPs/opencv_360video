#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "opencv2/calib3d.hpp"
#include <string>
#include <vector>
#include <stdlib.h>


#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#define PATTERN_WIDTH 	5
#define  PATTERN_HEIGHT	6



void get_all(const fs::path& root, const std::string& ext, std::vector<fs::path>& ret);
