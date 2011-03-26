#include "settings.hpp"

using namespace cv;
using namespace std;

Size Settings::CAM_SIZE = Size(640, 480);
string Settings::WIN1 = "kamera 1";
string Settings::WIN2 = "kamera 2";
string Settings::RECTIFIED = "rectified";
string Settings::CORNERS = "corners";
string Settings::ALGORITHMS = "stereo matching algorithms";

const int Settings::BEFORE_CALIBRATION = 0;
const int Settings::CALIBRATION = 1;
const int Settings::AFTER_CALIBRATION = 2;
const int Settings::MATCH = 3;


Settings::Settings(){
/*:
BEFORE_CALIBRATION(0),
CALIBRATION(1),
AFTER_CALIBRATION(2),
MATCH(3){
*/

	state = BEFORE_CALIBRATION;
	changed = false;
}

Settings::~Settings(){

}

bool Settings::settingsChanged(){
	return false;
}
