#pragma once

#include <vector>
using std::vector;

#define NULL 0

typedef struct
{
	int day;		// 天数
	int remain;		// 余额
} ONE_DAY_REMAIN;


// 将所有的日期-余额信息保存到vector中
typedef vector<ONE_DAY_REMAIN> DAY_REMAIN_DATA_VECTOR;