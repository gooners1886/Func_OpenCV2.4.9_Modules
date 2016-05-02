#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  
#include <math.h>
#include <float.h>

using namespace std;  
using namespace cv;  

bool myFloatEqualZero( float f )
{
	if ( fabs(f)<FLT_EPSILON )
	{
		return true;
	}

	return false;
}



// (��֪4����)����ֱ���󽻵�
// p1 p2 Ϊ��һ��ֱ���ϵĵ㣬 p3 p4Ϊ��2��ֱ���ϵĵ�
// ��ʽ��
//  (x-x1)/(y-y1) = (x1-x2)/(y1-y2)   --- ����1
//  (x-x3)/(y-y3) = (x3-x4)/(y3-y4)   --- ����2
cv::Point2f calIntersectionTwoLinesByFourPts( cv::Point2f p1, cv::Point2f p2, cv::Point2f p3, cv::Point2f p4 )
{
	cv::Point2f p;
	float fFenMu = (p1.x-p2.x)*(p3.y-p4.y)-(p1.y-p2.y)*(p3.x-p4.x);

	if ( myFloatEqualZero(fFenMu) )
	{
		std::cout<<"Two Lines are parallel"<<std::endl;
		std::cout<<"p1: "<<p1<<" p2:"<<p2<<" p3:"<<p3<<" p4"<<p4<<std::endl;
		exit(-1);
	}

	p.y = (p3.x-p1.x)*(p1.y-p2.y)*(p3.y-p4.y)-p3.y*(p3.x-p4.x)*(p1.y-p2.y)+p1.y*(p1.x-p2.x)*(p3.y-p4.y);
	p.y = p.y/fFenMu;

	p.x = p3.x*(p1.x-p2.x)*(p3.y-p4.y)+(p1.y-p3.y)*(p1.x-p2.x)*(p3.x-p4.x)-p1.x*(p1.y-p2.y)*(p3.x-p4.x);
	p.x = p.x/fFenMu;

	return p;
}

// ƽ���ཻ
// ��֪�� p1 p2��ֱ��1   p3 p4��ֱ��2   ��p5
// ���p, ��ֱ��1�ϣ���pp5����ƽ����p3p4����
// ��ʽ��
//  (x-x1)/(y-y1) = (x1-x2)/(y1-y2)   --- ����1
//  (x-x5)/(y-y5) = (x3-x4)/(y3-y4)   --- ����2
cv::Point2f calParaIntersection( cv::Point2f p1, cv::Point2f p2, cv::Point2f p3, cv::Point2f p4, cv::Point2f p5 )
{
	cv::Point2f p;
	float fFenMu = (p1.x-p2.x)*(p3.y-p4.y)-(p1.y-p2.y)*(p3.x-p4.x);

	if ( myFloatEqualZero(fFenMu) )
	{
		std::cout<<"Two Lines are parallel"<<std::endl;
		std::cout<<"p1: "<<p1<<" p2:"<<p2<<" p3:"<<p3<<" p4"<<p4<<std::endl;
		exit(-1);
	}

	p.y = (p5.x-p1.x)*(p1.y-p2.y)*(p3.y-p4.y)-p5.y*(p3.x-p4.x)*(p1.y-p2.y)+p1.y*(p1.x-p2.x)*(p3.y-p4.y);
	p.y = p.y/fFenMu;

	p.x = p5.x*(p1.x-p2.x)*(p3.y-p4.y)+(p1.y-p5.y)*(p1.x-p2.x)*(p3.x-p4.x)-p1.x*(p1.y-p2.y)*(p3.x-p4.x);
	p.x = p.x/fFenMu;

	return p;
}



int main()  
{  
	// ��ʾ ��ֱ���󽻵�
	int iShowIntersectionSwitch = 1;
	if ( iShowIntersectionSwitch )
	{
		// *************************  ����ֱ���󽻵�  *****************************
		cv::Mat mat(500, 500, CV_8UC3, cvScalar(250,190,190));
		// line 1
		cv::Point2f p1(100, 200);
		cv::Point2f p2(220, 300);
		// line 2
		cv::Point2f p3(300, 50);
		cv::Point2f p4(90, 400);

		cv::Point2f p = calIntersectionTwoLinesByFourPts(p1, p2, p3, p4);

		// line 1: red
		cv::circle( mat, p1, 2, cvScalar(0,0,255), 2 );
		cv::circle( mat, p2, 2, cvScalar(0,0,255), 2 );
		cv::line( mat, p1, p2, cvScalar(0,0,255) );
		// line 2: green
		cv::circle( mat, p3, 2, cvScalar(0,255,0), 2 );
		cv::circle( mat, p4, 2, cvScalar(0,255,0), 2 );
		cv::line( mat, p3, p4, cvScalar(0,255,0) );
		// Intersection Point : yellow
		cv::circle( mat, p, 2, cvScalar(0,255,255), 2 );


		cv::namedWindow("����ֱ���󽻵�");  
		cv::imshow("����ֱ���󽻵�", mat);
		cv::waitKey(0);  
		cv::destroyWindow("����ֱ���󽻵�");
	}

	// ��ʾ ƽ���ཻ  �󽻵�
	int iShowParaIntersectionSwitch = 1;
	if ( iShowParaIntersectionSwitch )
	{
		// *************************  ƽ���ཻ  *****************************
		cv::Mat mat2(500, 500, CV_8UC3, cvScalar(250,190,190));
		// line 1
		cv::Point2f p1(100, 200);
		cv::Point2f p2(450, 450);
		// line 2
		cv::Point2f p3(300, 50);
		cv::Point2f p4(90, 400);

		cv::Point2f p5(350, 150);


		cv::Point2f p = calParaIntersection(p1, p2, p3, p4, p5);

		// line 1: red
		cv::circle( mat2, p1, 2, cvScalar(0,0,255), 2 );
		cv::circle( mat2, p2, 2, cvScalar(0,0,255), 2 );
		cv::line( mat2, p1, p2, cvScalar(0,0,255) );
		// line 2: green
		cv::circle( mat2, p3, 2, cvScalar(0,255,0), 2 );
		cv::circle( mat2, p4, 2, cvScalar(0,255,0), 2 );
		cv::line( mat2, p3, p4, cvScalar(0,255,0) );
		// point 
		cv::circle( mat2, p5, 2, cvScalar(255,0,0), 2 );
		// Intersection Point : yellow
		cv::circle( mat2, p, 2, cvScalar(0,0,0), 2 );
		cv::line( mat2, p5, p, cvScalar(0,0,0) );



		cv::namedWindow("ƽ���ཻ");  
		cv::imshow("ƽ���ཻ", mat2);
		cv::waitKey(0);  
		cv::destroyWindow("ƽ���ཻ");
	}

	return 0;  
} 