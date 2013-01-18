#include "stdafx.h"
#include "Draw.h"

void drawCoorAxis(HDC hdc, POINT basePoint)
{
	static const int X_DATA_NUM = 13;
	static const int Y_DATA_NUM = 6;
	static const struct{
		wchar_t month[7];
		int size;
	} xData[X_DATA_NUM] = {
		{L"2012.1", 6},
		{L"2", 1},
		{L"3", 1},
		{L"4", 1},
		{L"5", 1},
		{L"6", 1},
		{L"7", 1},
		{L"8", 1},
		{L"9", 1},
		{L"10", 2},
		{L"11", 2},
		{L"12", 2},
		{L"2013.1", 6},
	};

	static const struct{
		wchar_t money[6];
		int size;
	} yData[Y_DATA_NUM] = {
		{L"0", 1},
		{L"2000", 4},
		{L"4000", 4},
		{L"6000", 4},
		{L"8000", 4},
		{L"10000", 5}
	};

	MoveToEx(hdc, basePoint.x, basePoint.y, NULL);
	LineTo(hdc, basePoint.x + 1200, basePoint.y);
	MoveToEx(hdc, basePoint.x, basePoint.y, NULL);
	LineTo(hdc, basePoint.x, basePoint.y - 700);
/*
	// 在X坐标上标出时间
	for (int i = 0; i < X_DATA_NUM; i++)
	{
		TextOut(hdc, basePoint.x + 82 * i, basePoint.y + 10, xData[i].month, xData[i].size);
		MoveToEx(hdc, basePoint.x + 82 * i, basePoint.y + 2, NULL);
		LineTo(hdc, basePoint.x + 82 * i, basePoint.y - 2);
	}

	// 在Y坐标上标出余额
	for (int i = 0; i < Y_DATA_NUM; i++)
	{
		TextOut(hdc, basePoint.x - 50, basePoint.y - 140 * i, yData[i].money, yData[i].size);
		
	}
*/
}

void drawLines(HDC hdc, POINT basePoint, DAY_REMAIN_DATA_VECTOR &dataVector)
{
	POINT point;
	if (dataVector.size() > 1)
	{		
		point.x = basePoint.x + dataVector.at(0).day * 1000 / 365;
		point.y = basePoint.y - dataVector.at(0).remain * 700 / 10000;

		MoveToEx(hdc, point.x, point.y, NULL);

		for (size_t i = 1; i < dataVector.size(); i++)
		{
			point.x = basePoint.x + dataVector.at(i).day * 1000 / 365;
			point.y = basePoint.y - dataVector.at(i).remain * 700 / 10000;
			LineTo(hdc, point.x, point.y);
		}
	}
}