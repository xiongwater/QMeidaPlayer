
#include "Utils.h"

#include <direct.h> 
#include <io.h>
#define MAX_PATH_LEN 512

WCHAR * charToWchar(char *s) {

	int w_nlen = MultiByteToWideChar(CP_ACP, 0, s, -1, NULL, 0);

	WCHAR *ret;

	ret = (WCHAR*)malloc(sizeof(WCHAR)*w_nlen);

	memset(ret, 0, sizeof(ret));

	MultiByteToWideChar(CP_ACP, 0, s, -1, ret, w_nlen);

	return ret;

}

char* WCharToChar(WCHAR *s) {

	int w_nlen = WideCharToMultiByte(CP_ACP, 0, s, -1, NULL, 0, NULL, false);

	char *ret = new char[w_nlen];

	memset(ret, 0, w_nlen);

	WideCharToMultiByte(CP_ACP, 0, s, -1, ret, w_nlen, NULL, false);

	return ret;

}

bool fileIsExits(std::string filePathName)
{
	WIN32_FIND_DATAA FindFileData;
	HANDLE hFind;
	hFind = FindFirstFileA(filePathName.data(), &FindFileData);
	if (hFind == INVALID_HANDLE_VALUE) {
		//::CreateDirectoryA(path.data(), NULL);
		::FindClose(hFind);
		return false;
	}
	else {
		::FindClose(hFind);
		return true;
	}
}

bool ensureDirectory(std::string directoryPath) {
	int dirPathLen = directoryPath.length();
	if (dirPathLen > MAX_PATH_LEN)
	{
		return false;
	}
	char tmpDirPath[MAX_PATH_LEN] = { 0 };
	for (int i = 0; i < dirPathLen; ++i)
	{
		tmpDirPath[i] = directoryPath[i];
		if (tmpDirPath[i] == '\\' || tmpDirPath[i] == '/')
		{
			if (_access(tmpDirPath, 0) != 0)
			{
				int ret = _mkdir(tmpDirPath);
				if (ret != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}