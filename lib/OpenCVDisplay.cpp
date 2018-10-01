
#include "serdprecorder/SerdpRecorder.h"

#include "serdprecorder/OpenCVDisplay.h"



namespace serdprecorder {

  OpenCVDisplay::OpenCVDisplay( SerdpRecorder &parent, bool enabled )
    : _enabled(enabled),
      _previewScale( 0.25 ),
      _parent(parent),
      _thread(active_object::Active::createActive())
  {}


  void OpenCVDisplay::display( vector< cv::Mat > rawImages )
  {
    const unsigned int numImages = rawImages.size();
    vector< cv::Mat > scaledImages( numImages );

    // Display images
    if( numImages == 1 ) {

      resizeImage( rawImages[0], scaledImages[0] );
      cv::imshow("Image", scaledImages[0]);

    } else if ( numImages == 2 ) {

      std::thread resizeThread( &OpenCVDisplay::resizeImage, this, std::cref(rawImages[1]), std::ref(scaledImages[1]) );
      resizeImage( rawImages[0], scaledImages[0] );
      resizeThread.join();

      cv::Mat composite( cv::Size( scaledImages[0].size().width + scaledImages[0].size().width,
      std::max(scaledImages[0].size().height, scaledImages[1].size().height )), scaledImages[0].type() );

      if( !scaledImages[0].empty() ) {
        cv::Mat leftROI( composite, cv::Rect(0,0,scaledImages[0].size().width,scaledImages[0].size().height) );
        scaledImages[0].copyTo( leftROI );
      }

      if( !scaledImages[1].empty() ) {
        cv::Mat rightROI( composite, cv::Rect(scaledImages[0].size().width, 0, scaledImages[1].size().width, scaledImages[1].size().height) );
        scaledImages[1].copyTo( rightROI );
      }

      cv::imshow("Composite", composite );
    }


		char c = cv::waitKey(1);
		_parent.handleKey( c );

  }


  void OpenCVDisplay::resizeImage( const cv::Mat &rawImage, cv::Mat &scaledImage )
  {
  	cv::resize( rawImage, scaledImage, cv::Size(), _previewScale, _previewScale  );

  //				scaledImages[i] = cv::Mat(tmp.rows, tmp.cols, CV_8UC3 );

  	// Image from camera is BGRA  map to RGB
  	// int from_to[] = { 0,0, 1,1, 2,2 };
  	// cv::mixChannels( &tmp, 1, &(scaledImages[i]), 1, from_to, 3 );
  	//cv::cvtColor( tmp, scaledImages[i], cv::COLOR_BGRA2RGB );
  	//cv::extractChannel(tmp, scaledImages[i], 0 );
  }


}
