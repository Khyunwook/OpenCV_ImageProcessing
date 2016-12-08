#include <opencv2\highgui\highgui.hpp>
#include <opencv2\opencv.hpp>
#include <iostream>
using namespace std;
using namespace cv;

Mat img, gRGB, gYCrCb, grayImg, grayAddImg, skinMask, hand, aMask, bMask, cMask;

int getWhiteCount(Mat &grayImg);
void splitMask();


int main() {
	VideoCapture vc; //0번웹캠 초기화
    if (!vc.isOpened())
		return 0; // 연결실패

    
	vector<Mat> YCrCb;
	Rect roiRect = Rect(0, 0, 300, 300);
	aMask = Mat(300, 300, CV_8UC3);
	bMask = Mat(300, 300, CV_8UC3);
	cMask = Mat(300, 300, CV_8UC3);

	splitMask();

    while (1) {
        vc >> img;
        if (img.empty())
			break;
		cv::rectangle(img, roiRect, Scalar(0, 0, 255), 2);
		Mat roi = img(roiRect);
		
		cv::cvtColor(roi, gRGB, CV_BGRA2BGR);
		cv::cvtColor(gRGB, gYCrCb, CV_BGR2YCrCb);

		cv::split(gYCrCb, YCrCb);

		inRange(YCrCb[2], Scalar(77), Scalar(150), YCrCb[2]);
		inRange(YCrCb[1], Scalar(133), Scalar(173), YCrCb[1]);
		
		cv::bitwise_and(YCrCb[1], YCrCb[2], grayImg);
		
		cv::cvtColor(grayImg, skinMask, CV_GRAY2BGR);

		cv::bitwise_and(skinMask, roi, hand);
		
		//범위에서 pixel이 흰색인 개수를 가지고 옵니다.
		int whitecount = getWhiteCount(grayImg);
		
		//흰색 pixel의 수가 10000개~15000개면 바위로 판별합니다.
		if(whitecount > 10000 && whitecount <15000){
			printf("바위\n");
			cv::bitwise_and(aMask, skinMask, hand);
		}//흰색 pixel의 수가 15000개~20000개면 가위로 판별합니다.
		else if(whitecount > 15000 && whitecount <20000){
			printf("가위\n");
			cv::bitwise_and(bMask, skinMask, hand);
		}//흰색 pixel의 수가 20000개~30000개면 보자기로 판별합니다.
		else if(whitecount >= 20000 && whitecount <30000){
			printf("보\n");
			cv::bitwise_and(cMask, skinMask, hand);
		}
		
		imshow("roi", roi);
        imshow("cam", img);
		imshow("gray", grayImg);
		imshow("hand", hand);
        if (waitKey(10) == 27)
			break; //ESC키 눌리면 종료
    }
    destroyAllWindows();
    return 0;
}

int getWhiteCount(Mat &grayImg){
	int whitecount=0; //white pixel의 개수를 0으로 초기화 합니다.
		for(int i=0; i<grayImg.cols; i++){
			for(int j=0; j<grayImg.rows; j++){
				int value = grayImg.at<uchar>(i,j);
				if(value==255){//해당 pixel이 흰색이면 white pixel의 개수를 1추가합니다. 
					whitecount++;
				}
			}
		}
	//white pixel의 개수를 반환합니다.
	return whitecount;
}
void splitMask() {
	for (int i = 0; i < aMask.rows; i++) {
		for (int j = 0; j < aMask.cols; j++) {
			aMask.at<Vec3b>(i, j)[0] = 0;
			aMask.at<Vec3b>(i, j)[1] = 0;
			aMask.at<Vec3b>(i, j)[2] = 255;

			bMask.at<Vec3b>(i, j)[0] = 0;
			bMask.at<Vec3b>(i, j)[1] = 255;
			bMask.at<Vec3b>(i, j)[2] = 0;

			cMask.at<Vec3b>(i, j)[0] = 255;
			cMask.at<Vec3b>(i, j)[1] = 0;
			cMask.at<Vec3b>(i, j)[2] = 0;
		}
	}
}