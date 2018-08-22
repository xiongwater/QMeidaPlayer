
#include "log.h"
#include <time.h>
#include <iostream>
#include <fstream>

using namespace std;

//打印日志接口
void writeLog(const char* operatorType, const char* operatorResult, const char* extraInfor)
{
	TCHAR szCurDirBuffer[MAX_PATH];
	memset(szCurDirBuffer, 0, sizeof(szCurDirBuffer));

	char* strCutDirBuffer[MAX_PATH];
	memset(strCutDirBuffer, 0, sizeof(strCutDirBuffer));

	if (::GetCurrentDirectory(MAX_PATH, szCurDirBuffer) == 0){
		return;
	}

	//ensureDirectory(szCurDirBuffer);
#ifdef UNICODE
	//UNICODE 编码下，TCHAR不能直接转换 故添加条件编译  2018-7-27 XH
	int iLen = WideCharToMultiByte(CP_ACP, 0, szCurDirBuffer, -1, NULL, 0, NULL, NULL);
	char* chRtn = new char[iLen*sizeof(char)];
	WideCharToMultiByte(CP_ACP, 0, szCurDirBuffer, -1, chRtn, iLen, NULL, NULL);
	std::string str(chRtn);
	delete chRtn;
	std::string file = str;
#elif
	std::string file = szCurDirBuffer;
#endif

	file += "/";
	file += "log.log";
	ofstream log;
	log.open(file.c_str(), ios::app | ios::out);

	time_t t = time(0);
	tm* tt = localtime(&t);
	log << "[" << tt->tm_year + 1900 << "-" << tt->tm_mon + 1<< "-" << tt->tm_mday << " " 
		<< tt->tm_hour << ":" << tt->tm_min << ":" << tt->tm_sec << "]";
	log << "\t[操作类型]:" << operatorType;
	log << "\t[操作结果]:" << operatorResult;
	log << "\t[其他信息]:" << extraInfor << endl;
	log.close();
}