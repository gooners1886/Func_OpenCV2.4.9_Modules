#define _USE_MATH_DEFINES
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <iostream>  
#include <math.h>
#include <float.h>
#include <math.h>

// 引用C/C++库中的宏来表示PI的方法:  
// 1. #define _USE_MATH_DEFINES   --- 注意，这个需要放在math.h的前面
// 2. #include <math.h>

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



// (已知4个点)两条直线求交点
// p1 p2 为第一条直线上的点， p3 p4为第2条直线上的点
// 算式：
//  (x-x1)/(y-y1) = (x1-x2)/(y1-y2)   --- 方程1
//  (x-x3)/(y-y3) = (x3-x4)/(y3-y4)   --- 方程2
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

// 平行相交
// 已知： p1 p2是直线1   p3 p4是直线2   点p5
// 求点p, 在直线1上，且pp5连线平行于p3p4连线
// 算式：
//  (x-x1)/(y-y1) = (x1-x2)/(y1-y2)   --- 方程1
//  (x-x5)/(y-y5) = (x3-x4)/(y3-y4)   --- 方程2
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



// 垂直相交
// 已知： p1 p2是直线1   p3 是不在直线上的点
// 求点p, pp3 垂直于直线p1p2
// 算式：
//  (y-y3)/(x-x3) = (-1)*(x1-x2)/(y1-y2)   --- 方程1   pp3 垂直于直线p1p2
//  (x-x1)/(y-y1) = (x1-x2)/(y1-y2)        --- 方程2   p点在p1p2直线上
cv::Point2f calVerticalIntersection( cv::Point2f p1, cv::Point2f p2, cv::Point2f p3 )
{
	cv::Point2f p;
	float fFenMu = (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y);

	if ( myFloatEqualZero(fFenMu) )
	{
		std::cout<<"Two Points p1 p2 are same!"<<std::endl;
		std::cout<<"p1: "<<p1<<" p2:"<<p2<<" p3:"<<p3<<std::endl;
		exit(-1);
	}


	p.y = ( (p3.x-p1.x)*(p1.x-p2.x)*(p1.y-p2.y)+p1.y*(p1.x-p2.x)*(p1.x-p2.x)+p3.y*(p1.y-p2.y)*(p1.y-p2.y) )/fFenMu;
	p.x = ( (p3.y-p1.y)*(p1.x-p2.x)*(p1.y-p2.y)+p3.x*(p1.x-p2.x)*(p1.x-p2.x)+p1.x*(p1.y-p2.y)*(p1.y-p2.y) )/fFenMu;

	return p;
}



double calDist( cv::Point2f p1, cv::Point2f p2 )
{
	return sqrt( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) );
}

// 坐标旋转
//    对图片上任意点(x,y)，绕一个坐标点(rx0,ry0)逆时针旋转a角度后的新的坐标设为(x0, y0)，有公式：
//    x0= (x - rx0)*cos(a) - (y - ry0)*sin(a) + rx0 ;    y0= (x - rx0)*sin(a) + (y - ry0)*cos(a) + ry0 ;
cv::Point2f calCoordinateRotation( cv::Point2f center, cv::Point2f p, double dTheta )
{
	cv::Point2f pNew;
	pNew.x = (p.x-center.x)*cos(dTheta*M_PI/180) - (p.y-center.y)*sin(dTheta*M_PI/180) + center.x;
	pNew.y = (p.x-center.x)*sin(dTheta*M_PI/180) + (p.y-center.y)*cos(dTheta*M_PI/180) + center.y;

	return pNew;
}


// 通过坐标旋转，画半圆上的点
void calPtsInSemicircle( cv::Point2f p1, cv::Point2f p2, int iNum, std::vector<cv::Point2f>& vPtsInSemicircle )
{
	double dDeltaAngle = 180.0/iNum;
	cv::Point2f mid( (p1.x+p2.x)/2.0, (p1.y+p2.y)/2.0 );
	for ( double dTheta = dDeltaAngle; dTheta < 180.0; dTheta+=dDeltaAngle)
	{
		cv::Point2f p;
		//p.x = cos(dTheta*M_PI/180)*dRadius + mid.x;
		//p.y = sin(dTheta*M_PI/180)*dRadius + mid.y;

		p = calCoordinateRotation(mid, p2, dTheta);

		vPtsInSemicircle.push_back(p);
	}

	return;
}


int main()  
{  
	// 显示 两直线求交点
	int iShowIntersectionSwitch = 1;
	if ( iShowIntersectionSwitch )
	{
		// *************************  两条直线求交点  *****************************
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


		cv::namedWindow("两条直线求交点");  
		cv::imshow("两条直线求交点", mat);
		cv::waitKey(0);  
		cv::destroyWindow("两条直线求交点");
	}

	// 显示 平行相交  求交点
	int iShowParaIntersectionSwitch = 1;
	if ( iShowParaIntersectionSwitch )
	{
		// *************************  平行相交  *****************************
		cv::Mat mat(500, 500, CV_8UC3, cvScalar(250,190,190));
		// line 1
		cv::Point2f p1(100, 200);
		cv::Point2f p2(450, 450);
		// line 2
		cv::Point2f p3(300, 50);
		cv::Point2f p4(90, 400);

		cv::Point2f p5(350, 150);


		cv::Point2f p = calParaIntersection(p1, p2, p3, p4, p5);

		// line 1: red
		cv::circle( mat, p1, 2, cvScalar(0,0,255), 2 );
		cv::circle( mat, p2, 2, cvScalar(0,0,255), 2 );
		cv::line( mat, p1, p2, cvScalar(0,0,255) );
		// line 2: green
		cv::circle( mat, p3, 2, cvScalar(0,255,0), 2 );
		cv::circle( mat, p4, 2, cvScalar(0,255,0), 2 );
		cv::line( mat, p3, p4, cvScalar(0,255,0) );
		// point 
		cv::circle( mat, p5, 2, cvScalar(255,0,0), 2 );
		// Intersection Point : yellow
		cv::circle( mat, p, 2, cvScalar(0,0,0), 2 );
		cv::line( mat, p5, p, cvScalar(0,0,0) );



		cv::namedWindow("平行相交");  
		cv::imshow("平行相交", mat);
		cv::waitKey(0);  
		cv::destroyWindow("平行相交");
	}

	// 显示 垂直相交  求交点
	int iShowVerticalIntersectionSwitch = 1;
	if ( iShowVerticalIntersectionSwitch )
	{
		// *************************  平行相交  *****************************
		cv::Mat mat(500, 500, CV_8UC3, cvScalar(250,190,190));
		// line 1
		cv::Point2f p1(100, 100);
		cv::Point2f p2(150, 300);
		// point 
		cv::Point2f p3(200, 50);

		//// line 1
		//cv::Point2f p1(0, 0);
		//cv::Point2f p2(0, 450);
		//// point 
		//cv::Point2f p3(300, 50);



		cv::Point2f p = calVerticalIntersection(p1, p2, p3);

		// line 1: red
		cv::circle( mat, p1, 2, cvScalar(0,0,255), 2 );
		cv::circle( mat, p2, 2, cvScalar(0,0,255), 2 );
		cv::line( mat, p1, p2, cvScalar(0,0,255) );
		// line 2: green
		cv::circle( mat, p3, 2, cvScalar(0,255,0), 2 );
		cv::line( mat, p3, p, cvScalar(0,255,0) );

		cv::circle( mat, p, 2, cvScalar(255,0,0), 2 );


		cv::namedWindow("垂直相交");  
		cv::imshow("垂直相交", mat);
		cv::waitKey(0);  
		cv::destroyWindow("垂直相交");
	}



	int iShowSemiCircleSwitch = 1;
	if ( iShowSemiCircleSwitch )
	{
		// *************************  平行相交  *****************************
		cv::Mat mat(500, 500, CV_8UC3, cvScalar(250,190,190));
		// line 1
		cv::Point2f p1(100, 100);
		cv::Point2f p2(300, 300);

		cv::Point2f mid( (p1.x+p2.x)/2.0, (p1.y+p2.y)/2.0 );


		cv::circle( mat, p1, 2, cvScalar(0,0,255), 2 );
		cv::circle( mat, p2, 2, cvScalar(0,0,255), 2 );
		cv::circle( mat, mid, 2, cvScalar(0,0,255), 2 );
		cv::circle( mat, cv::Point2f( (p1.x+p2.x)/2.0, (p1.y+p2.y)/2.0 ), calDist(p1, mid), cvScalar(0,0,255), 2 );

		std::vector<cv::Point2f> vPtsInSemicircle;
		calPtsInSemicircle( p1, p2, 9, vPtsInSemicircle );

		for ( int iPtIdx = 0; iPtIdx < vPtsInSemicircle.size(); iPtIdx++ )
		{
			cv::circle( mat, vPtsInSemicircle[iPtIdx], 4, cvScalar(255,0,0), 2 );
		}

		
		


		cv::namedWindow("通过坐标旋转画半圆上的点");  
		cv::imshow("通过坐标旋转画半圆上的点", mat);
		cv::waitKey(0);  
		cv::destroyWindow("通过坐标旋转画半圆上的点");
	}

	return 0;  
} 