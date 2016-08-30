//ע�⣺
//FindFilesInOneFolder.h"��÷��ڵ�һ��ͷ�ļ�  ���Ϊ����#include <afx.h>���ڵ�һ��ͷ�ļ�������ᱨ��fatal error C1189: #error :  WINDOWS.H already included.  MFC apps must not #include <windows.h>
//�������óɣ���dll��ʹ��MFC�������ھ�̬����ʹ�� MFC
//�������óɣ�ʹ�ö��ֽ��ַ�����Unicode������


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
// �ظ���ͼ��ֻ��һ����������ͼ�����е�����ļ�������ȥ
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
		//1. ȡ�õ�ǰͼ������ֺ���չ����
		int q=strFile.find_last_of('\\');
		int p=strFile.find_last_of('.');
		int difference1 = p - q - 1;
		string name=strFile.substr((q+1),difference1);//img186895
		int path_size = strFile.size();
		//int difference2 = path_size - p;
		string ext_name = strFile.substr(p, path_size);//.bmp
		string NameAndExtname = strFile.substr(q+1, path_size);//img186895.bmp

		cout<<strFile<<endl;


		// ������ͼƬ�Ѿ���֮ǰ��ͼƬ����same����ô����ͼƬ�Ͳ���Ҫ�ټ���ˡ�
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
			//1. ȡ�õ�ǰͼ������ֺ���չ����
			int q=strFile.find_last_of('\\');
			int p=strFile.find_last_of('.');
			int difference1 = p - q - 1;
			string name=strFile.substr((q+1),difference1);//img186895
			int path_size = strFile.size();
			//int difference2 = path_size - p;
			string ext_name = strFile.substr(p, path_size);//.bmp
			string NameAndExtname = strFile.substr(q+1, path_size);//img186895.bmp

			// ������ͼƬ�Ѿ���֮ǰ��ͼƬ����same����ô����ͼƬ�Ͳ���Ҫ�ټ���ˡ�
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

			// ��sameʱ�ļ��д���
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

		}
	}


	fstream outfile;
	//���ļ�����ʼд��  ����һ��txt�ļ�
	// in::out�����Ĵ򿪷�ʽ��in::trunc,�����ļ������ԭ������
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

	//�ر��ļ�
	outfile.close();


	// ��ʼ�����ļ���
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


