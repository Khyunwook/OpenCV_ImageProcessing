

#include "opencv/cv.h"
#include "opencv/highgui.h"
#include <iostream>
#include <cmath>

#define pi 3.141592653589793238462643383279

cv::Mat gMatImage,gMatFlipImage;
bool select_flag = false;
cv::Point point1, point2;
cv::Mat frame;
bool callback = false;

int drag = 0;
CvRect box;

void Yreflect( ){
			
	for(int i=0; i< gMatImage.cols; i++){
		for(int j=0; j< gMatImage.rows; j++){

		gMatFlipImage.at <cv ::Vec3b > (j,i) =  gMatImage.at <cv::Vec3b> (j,i);
		
		}
	}
	for(int i=point1.x; i< point2.x; i++){
		for(int j=point1.y; j< point2.y; j++){

		gMatFlipImage.at <cv ::Vec3b > (j,i) =  gMatImage.at <cv::Vec3b> (j,(point2.x-1)-i+point1.x);
		
		}
	}
	cv::rectangle(gMatFlipImage, point1, point2, CV_RGB(0, 255, 0), 3, 8, 0);
	cv::namedWindow("Yreflect");
	cv::imshow("Yreflect",gMatFlipImage);
}
void Xreflect( ){
			
	for(int i=0; i< gMatImage.cols; i++){
		for(int j=0; j< gMatImage.rows; j++){

		gMatFlipImage.at <cv ::Vec3b > (j,i) =  gMatImage.at <cv::Vec3b> (j,i);
		
		}
	}
	for(int i=point1.x; i< point2.x; i++){
		for(int j=point1.y; j< point2.y; j++){

		gMatFlipImage.at <cv ::Vec3b > (j,i) =  gMatImage.at <cv::Vec3b> ((point2.y-1)-j+point1.y,i);
		
		}
	}
	cv::rectangle(gMatFlipImage, point1, point2, CV_RGB(0, 255, 0), 3, 8, 0);
	cv::namedWindow("Xreflect");
	cv::imshow("Xreflect",gMatFlipImage);
}

void Rotate( ){
	cv::Point rpoint1, rpoint2;
	double radian =90*pi/180.0,
		   cc = cos(radian), ss = sin(-radian);
	double xcenter = (double)(point2.x+point1.x)/2.0, ycenter = (double)(point2.y+point1.y)/2.0;
	
	for(int i=0; i< gMatImage.cols; i++){
		for(int j=0; j< gMatImage.rows; j++){

		gMatFlipImage.at <cv ::Vec3b > (j,i) =  gMatImage.at <cv::Vec3b> (j,i);
		
		}
	}
	rpoint1.y=(int)(ycenter+((double)point1.y-ycenter)*cc-((double)point1.x-xcenter)*ss);
	rpoint1.x=(int)(xcenter+((double)point1.y-ycenter)*ss-((double)point1.x-xcenter)*cc);
	for(int i=point1.x; i< point2.x; i++){
		for(int j=point1.y; j< point2.y; j++){
		 gMatFlipImage.at <cv::Vec3b> (
				rpoint2.y=(int)(ycenter+((double)j-ycenter)*cc-((double)i-xcenter)*ss),
				rpoint2.x=(int)(xcenter+((double)j-ycenter)*ss-((double)i-xcenter)*cc)
					
			)=gMatImage.at <cv ::Vec3b > (j,i);
		}
	}

	cv::rectangle(gMatFlipImage, rpoint1, rpoint2, CV_RGB(0, 255, 0), 3, 8, 0);
	cv::namedWindow("Rotate");
	cv::imshow("Rotate",gMatFlipImage);
}

void Resize(){
	cv::Point rpoint1, rpoint2;
	cv::Mat preResize = cv::Mat::zeros(point2.y-point1.y,point2.x-point1.x,CV_8UC3);
	cv::Mat rearResize;
	for(int i=point1.x; i< point2.x; i++){
		for(int j=point1.y; j< point2.y; j++){
			preResize.at <cv ::Vec3b > (j-point1.y,i-point1.x) =  gMatImage.at <cv::Vec3b> (j,i);
		}
	}
	for(int i=0; i< gMatImage.cols; i++){
		for(int j=0; j< gMatImage.rows; j++){

		gMatFlipImage.at <cv ::Vec3b > (j,i) =  gMatImage.at <cv::Vec3b> (j,i);
		
		}
	}

	float scale = 1.5;
	int boxWidth = (int)((point2.x-point1.x) * scale);
	int boxHeight = (int)((point2.y-point1.y)  * scale);
    cv::resize(preResize,rearResize, cv::Size(boxWidth, boxHeight));
	rpoint1.x=point1.x;
	rpoint1.y=point1.y;
	rpoint2.x=rearResize.cols+point1.x;
	rpoint2.y=rearResize.rows+point1.y;
	for(int i=0; i< rearResize.cols; i++){
		for(int j=0; j< rearResize.rows; j++){
		//printf("%d %d\n",i,j);
		gMatFlipImage.at <cv::Vec3b> (point1.y+j,point1.x+i) =  rearResize.at <cv::Vec3b> (j,i);
		
		}
	}

	cv::rectangle(gMatFlipImage, rpoint1, rpoint2, CV_RGB(0, 255, 0), 3, 8, 0);
	cv::namedWindow("Resize2");
	cv::imshow("Resize2",gMatFlipImage);
}

void on_mouse(int event, int x, int y, int flags, void* params){
	
	if (event == CV_EVENT_LBUTTONDOWN && !drag && !select_flag)
	{
		/* left button clicked. ROI selection begins */
		point1 = cv::Point(x, y);
		drag = 1;
	}

	if (event == CV_EVENT_MOUSEMOVE && drag && !select_flag)
	{
		 /* mouse dragged. ROI being selected */
		cv::Mat img1 = gMatImage.clone();
		point2 = cv::Point(x, y);
		//printf("test\n");
		cv::rectangle(img1, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
		cv::imshow("image", img1);
	}

	if (event == CV_EVENT_LBUTTONUP && drag && !select_flag)
	{
		cv::Mat img2 = gMatImage.clone();
		point2 = cv::Point(x, y);
		drag = 0;
		select_flag = 1;
		cv::rectangle(img2, point1, point2, CV_RGB(255, 0, 0), 3, 8, 0);
		cv::imshow("image", img2);
		callback = true;
		Yreflect();
		Xreflect();
		Rotate();
		Resize();
	}
}




int main(int argc, char *argv[])
{
	box = cvRect(-1, -1, 0, 0);

	gMatImage = cv::imread("aaa.jpg",1);
	gMatFlipImage = cv::Mat::zeros(gMatImage.size(),gMatImage.type());
	
	
	cv::namedWindow("image");
	cv::setMouseCallback("image",on_mouse);
	cv::imshow("image", gMatImage);
	
	
    
	cv::waitKey(0);	
	
	

    return 0;
}