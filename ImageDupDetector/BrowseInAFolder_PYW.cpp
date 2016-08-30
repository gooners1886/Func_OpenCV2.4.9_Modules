//注意：
//FindFilesInOneFolder.h"最好放在第一个头文件  这个为了让#include <afx.h>放在第一个头文件，否则会报错：fatal error C1189: #error :  WINDOWS.H already included.  MFC apps must not #include <windows.h>
//工程设置成：在dll中使用MFC或者是在静态库中使用 MFC
//工程设置成：使用多字节字符或者Unicode都可以


#include <cv.h>
#include <highgui.h>
#include <opencv2/opencv.hpp>    
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include "FindFilesInOneFolder.h"
#include <fstream>

#include <iostream>
using namespace std;
using namespace cv;

string ext1 = ".png";
string ext2 = ".bmp";
string ext3 = ".tif";
string ext4 = ".jpg";
string ext5 = ".ppm";

string strSrcImgFolder = ".\\SrcFolder\\";
string strDstSummaryListPath = ".\\final_summary.txt";
// 重复的图像只留一个，其他的图像会剪切到这个文件夹里面去
string strDupFolder = ".\\DupFolder\\";


void main()
{
	// 1. Find all the filenames	
	cout<<"Find all the filenames ..."<<endl;
	vector<std::string> strFileList_tmp,strFileList;
	int i;
	strFileList.clear();
	strFileList_tmp=FindFilesInOneFolder(strSrcImgFolder);
	int N_tmp=strFileList_tmp.size();
	int j, N;
	for (j=0,N=0;j<N_tmp;j++)//it=FilenamesVector.begin();it!=FilenamesVector.end();it++)
	{
		string tmp=strFileList_tmp[j];
		int q=tmp.find_last_of('.');
		string ext=tmp.substr(q,tmp.size());

		if (ext==ext1||ext==ext2||ext==ext3||ext==ext4||ext==ext5)		
		{
			strFileList.push_back(tmp);		
			N++;
		}
	}

	if (N<1)
	{
		std::cout<<"ERROR: No valid images in folder "<<strSrcImgFolder<<" !"<<std::endl;
		return;
	}

	vector<int> vDupMap;
	for ( int i = 0; i < strFileList.size(); i++ )
	{
		vDupMap.push_back(0);
	}


	vector<vector<string>> vTotalSameImgList;


	for(int id_file = 0; id_file < strFileList.size(); id_file++)
	{
		string strFile = strFileList[id_file];
		//1. 取得当前图像的名字和拓展名：
		int q=strFile.find_last_of('\\');
		int p=strFile.find_last_of('.');
		int difference1 = p - q - 1;
		string name=strFile.substr((q+1),difference1);//img186895
		int path_size = strFile.size();
		//int difference2 = path_size - p;
		string ext_name = strFile.substr(p, path_size);//.bmp
		string NameAndExtname = strFile.substr(q+1, path_size);//img186895.bmp

		cout<<strFile<<endl;


		// 如果这幅图片已经被之前的图片检测出same，那么这张图片就不需要再检测了。
		if ( 1 == vDupMap[id_file] )
		{
			continue;
		}

		vector<string> vSameImgList;
		vSameImgList.push_back(NameAndExtname);

		Mat matCurImg;
		matCurImg = imread(strFile.c_str());
		Mat matCmpImg;
		Mat matDiff;

		for ( int id_file2 = id_file+1; id_file2 < strFileList.size(); id_file2++ )
		{
			string strFile = strFileList[id_file2];
			//1. 取得当前图像的名字和拓展名：
			int q=strFile.find_last_of('\\');
			int p=strFile.find_last_of('.');
			int difference1 = p - q - 1;
			string name=strFile.substr((q+1),difference1);//img186895
			int path_size = strFile.size();
			//int difference2 = path_size - p;
			string ext_name = strFile.substr(p, path_size);//.bmp
			string NameAndExtname = strFile.substr(q+1, path_size);//img186895.bmp

			// 如果这幅图片已经被之前的图片检测出same，那么这张图片就不需要再检测了。
			if ( 1 == vDupMap[id_file2] )
			{
				continue;
			}

			matCmpImg = imread(strFile.c_str());

			bool bSame = true;

			if ( (matCurImg.cols == matCmpImg.cols)&&(matCurImg.rows == matCmpImg.rows) )
			{
				for ( int id_row = 0; id_row < matCurImg.rows; id_row++ )
				{
					if ( !bSame )
					{
						break;
					}

					for ( int id_col = 0; id_col < matCurImg.cols; id_col++)
					{
						Vec3b intensityCur = matCurImg.at<Vec3b>(id_row, id_col);
						Vec3b intensityCmp = matCmpImg.at<Vec3b>(id_row, id_col);

						if ( intensityCur[0] != intensityCmp[0] )
						{
							bSame = false;
							break;
						}
						if ( intensityCur[1] != intensityCmp[1] )
						{
							bSame = false;
							break;
						}
						if ( intensityCur[2] != intensityCmp[2] )
						{
							bSame = false;
							break;
						}
					}
				}
				
			}
			else
			{
				bSame = false;
				continue;
			}

			// 当same时的集中处理
			if ( bSame )
			{
				vSameImgList.push_back(NameAndExtname);
				vDupMap[id_file2] = 1;
				vDupMap[id_file] = 1;
			}

		}


		if ( vSameImgList.size() > 1 )
		{
			vTotalSameImgList.push_back(vSameImgList);

			cout<<"tmp: ";
			for ( int iInner = 0; iInner < vSameImgList.size(); iInner++ )
			{
				cout<<vSameImgList[iInner]<<" == ";
			}
			cout<<endl;
		}
	}


	fstream outfile;
	//打开文件，开始写入  创建一个txt文件
	// in::out隐含的打开方式是in::trunc,即打开文件后清除原有内容
	outfile.open(strDstSummaryListPath.c_str (), ios::out);
	if(!outfile)
	{
		cout<<"can not open the file!   "<<strDstSummaryListPath<<endl;
	}
	else
	{
		cout<<"open txt successfully!!"<<endl;
	}

	outfile<<endl<<"***************** Final Summary *****************"<<endl;
	cout<<endl<<"***************** Final Summary *****************"<<endl;

	for ( int iOut = 0; iOut < vTotalSameImgList.size(); iOut++ )
	{
		for ( int iInner = 0; iInner < vTotalSameImgList[iOut].size(); iInner++ )
		{
			cout<<vTotalSameImgList[iOut][iInner]<<" == ";
			outfile<<vTotalSameImgList[iOut][iInner]<<" == ";
		}

		cout<<endl<<"-----------------------"<<endl;
		outfile<<endl<<"-----------------------"<<endl;
	}

	//关闭文件
	outfile.close();


	// 开始剪切文件夹
	for ( int iOut = 0; iOut < vTotalSameImgList.size(); iOut++ )
	{
		for ( int iInner = 1; iInner < vTotalSameImgList[iOut].size(); iInner++ )
		{
			string strMoveCmd = "move " + strSrcImgFolder + vTotalSameImgList[iOut][iInner] + " " + strDupFolder;
			system(strMoveCmd.c_str());
		}
	}
	


	return;
}


