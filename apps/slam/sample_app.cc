#include <iostream>
#include <cv.h>
#include <highgui.h>

#include "live_slam_wrapper.h"

#include "util/settings.h"
#include "util/global_funcs.h"

#include "util/Undistorter.h"
#include "io_wrapper\OpenCVImageStreamThread.h"
#include "slam_system.h"

using namespace std;
using namespace lsd_slam;
char key;
//PATH=C:\projects\uni\dissertation\Libraries\g2o\install\bin;C:\projects\uni\dissertation\Libraries\opencv\x86\vc12\bin;%PATH%
int main()
{
	cvNamedWindow("Camera_Output", 1); //Create window
	cvNamedWindow("Camera_Output_Undist", 1); //Create window
	
	CvCapture* capture = cvCaptureFromCAM(CV_CAP_ANY); //Capture using any camera connected to your system
	OpenCVImageStreamThread* inputStream = new OpenCVImageStreamThread();
	inputStream->setCalibration("out_camera_data.xml");
	inputStream->setCameraCapture(capture);
	inputStream->run();

	Output3DWrapper* outputWrapper = NULL;	
	LiveSLAMWrapper slamNode(inputStream, outputWrapper);
	
	slamNode.Loop();

	Undistorter* undistorter = Undistorter::getUndistorterForFile("out_camera_data.xml");

	while (1){ //Create infinte loop for live streaming
		IplImage* frame = cvQueryFrame(capture); //Create image frames from capture
		TimestampedMat bufferItem;
		bufferItem.timestamp = Timestamp::now();
		
		cv::Mat mymat = cv::Mat(frame, true);

		
		undistorter->undistort(frame, mymat);
	    
		cvShowImage("Camera_Output", frame); //Show image frames on created window
		cv::imshow("Camera_Output_Undist", mymat);
		key = cvWaitKey(10); //Capture Keyboard stroke
		if (char(key) == 27){
			break; //If you hit ESC key loop will break.
		}
	}

	if (inputStream != nullptr)
		delete inputStream;
	if (outputWrapper != nullptr)
		delete outputWrapper;

	cvReleaseCapture(&capture); //Release capture.
	cvDestroyWindow("Camera_Output"); //Destroy Window
	return 0;
}