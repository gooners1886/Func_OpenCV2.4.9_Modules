#include "FindFilesInOneFolder.h"



std::vector<std::string> FindFilesInOneFolder(const std::string folder_in)
{
	// 0. Initialize
	vector <string> files_out;
	CFileFind finder;

	// build a string with wildcards
	CString str_root(folder_in.c_str());
	CString str=str_root+_T("\\*.*");
	CString filename;

	// 1. Start working for files
	BOOL bWorking = finder.FindFile(str);

	while (bWorking)
	{
		// 2.
		bWorking = finder.FindNextFile();

		// 3. skip . and .. files; 
		if (finder.IsDots())
			continue;

		// 4. if it's a directory, skip it
		if (finder.IsDirectory())
			continue; 

		// 5. Get filename
		filename=finder.GetFilePath();

		CStringA stra = filename;
		const char* pszStr = stra.GetBuffer();
		string s = pszStr;
		files_out.push_back(s);

		//files_out.push_back((LPCTSTR)filename);
	}

	// 6. Finished
	finder.Close();
	return files_out;
}



