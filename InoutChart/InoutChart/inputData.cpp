#include "stdafx.h"
#include "inputData.h"
#include <fstream>
#include <string>
using namespace std;

// days[n]表示一年中n月之前的总天数，设二月有28天
static const int DAYS[13] = {0, 0, 31, 51, 90, 120, 151, 181, 212, 242, 273, 304, 334};

bool getDataFromFile(const wchar_t* filePath, DAY_REMAIN_DATA_VECTOR &dataVector)
{
	ONE_DAY_REMAIN tempData = {0, 0};
	ifstream file;
	file.open(filePath, ios::out|ios::in);

	if (NULL == file)
	{
		return false;
	}

	int month;
	int day;
	string line;
	string sMonth;
	string sDay;
	string remain;
	while (getline(file, line))
	{
		sMonth = line.substr(4, 2);
		sDay = line.substr(6, 2);
		month = atoi(sMonth.c_str());
		day = atoi(sDay.c_str());

		remain = line.substr(9);
		tempData.day = DAYS[month] + day;
		tempData.remain = atoi(remain.c_str());
		if (!dataVector.empty() && tempData.day == dataVector.back().day)
		{
			dataVector.pop_back();
		}
		dataVector.push_back(tempData);
	}

	file.close();

	if (dataVector.empty())
	{
		return false;
	}
	else
	{
		return true;
	}
}