//==============================================================================
// 文件名称: log.h
// 当前版本: 1.0.0
// 设计日期: 2016-11-10
// 内容摘要: 添加日志文件
// 修改记录: 
// 修改记录  日   期 版   本    修改摘要
//
//-------------------------------------------------------------------------------
#ifndef _LOG_H_
#define _LOG_H_

//#define LOG_FILE_PATH  ".\InfoCollectLog"
//#define LOG_FILE_PATHNAME  LOG_FILE_PATH"IOTP_FB_ITCIPCast.Log"

#include "Utils.h"

/************************************************************************/
/* Named:       writeLog
/* Description: 写日志
/* Param:       operatorType  - 操作类型
/*				operatorResult- 操作结果
/*				extraInfor    - 额外信息
/* Created:     2016-06-30 20：43：00
/************************************************************************/
void writeLog(const char* operatorType, const char* operatorResult, const char* extraInfor);

#endif //_LOG_H_