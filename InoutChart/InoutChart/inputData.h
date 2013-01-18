#pragma once

#include "define.h"

// 功能：	将文件中的日期-余额信息保存到info变量中，如果一天内有多个数据则只保存当天最后的余额
// 参数：	filePath：	保存信息的文件路径
//			info：		信息存入info变量中
// 返回值：	true：		取得信息成功
//			false：		取得信息失败
bool getDataFromFile(const wchar_t* filePath, DAY_REMAIN_DATA_VECTOR &dataVector);
