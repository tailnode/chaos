// NQueen.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "define.h"

int _tmain(int argc, _TCHAR* argv[])
{
	const int N = 10;

	vector<vector<POSTION>> allResult;
	vector<RESULT> outResult;
	RESULT temp;
	
	for (int i = 1; i < N + 1; i++)
	{
		calcAllNQueen(allResult, i);
		temp.n = i;
		temp.resultNum = allResult.size();
		outResult.push_back(temp);
		allResult.clear();
	}

	writeToFile("E:\\NQueenResult.txt", outResult);

	return 0;
}

