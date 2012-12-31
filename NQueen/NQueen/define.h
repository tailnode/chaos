#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <vector>
using namespace std;

typedef struct
{
	int row;			// 皇后所在的行
	int column;			// 皇后所在的列
} POSTION;

typedef struct
{
	int n;				// N皇后问题的N值
	int resultNum;		// N皇后问题解的个数
} RESULT;

// 判断两个点是否冲突
inline bool isConflict(POSTION p1, POSTION p2);

// 能找到不冲突的位置就返回true，否则返回false
// 将找到的位置保存到第一个参数p中
bool getPos(POSTION &p, const vector<POSTION> &s, int n);

// 将N皇后问题的一个解保存到容器v中
void calcNQueen(vector<POSTION> &v, int n);

// 将N皇后问题的所有解保存到容器vAllResult中
void calcAllNQueen(vector<vector<POSTION>> &vAllResult, int n);

// 将N皇后问题解的个数写入文件
void writeToFile(const char* file, vector<RESULT> &result);

#endif // _DEFINE_H_