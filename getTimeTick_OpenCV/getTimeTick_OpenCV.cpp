#include <opencv2/opencv.hpp>
#include <iostream>


double begin_time_opencv[10];
double end_time_opencv[10];
void getBeginTick_OpenCV( int idx )
{
	begin_time_opencv[idx] = (double)cvGetTickCount();
}

void getEndTick_OpenCV(  int idx, std::string str )
{
	end_time_opencv[idx] = (double)cvGetTickCount();

	std::cout<<"Time of "<<str<<" is "<< (end_time_opencv[idx]-begin_time_opencv[idx])/((double)cvGetTickFrequency()*1000.0) <<" ms."<<std::endl;

	return;
}



int main ()
{
	getBeginTick_OpenCV( 0 );

	int iSum = 0;
	for ( int i = 0; i < 100; i++ )
	{
		iSum += i;
	}
	std::cout<<"iSum="<<iSum<<std::endl;


	getEndTick_OpenCV( 0, "calucate sum" );

	return 1;
};