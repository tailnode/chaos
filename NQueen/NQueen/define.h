#ifndef _DEFINE_H_
#define _DEFINE_H_

#include <vector>
using namespace std;

typedef struct
{
	int row;			// �ʺ����ڵ���
	int column;			// �ʺ����ڵ���
} POSTION;

typedef struct
{
	int n;				// N�ʺ������Nֵ
	int resultNum;		// N�ʺ������ĸ���
} RESULT;

// �ж��������Ƿ��ͻ
inline bool isConflict(POSTION p1, POSTION p2);

// ���ҵ�����ͻ��λ�þͷ���true�����򷵻�false
// ���ҵ���λ�ñ��浽��һ������p��
bool getPos(POSTION &p, const vector<POSTION> &s, int n);

// ��N�ʺ������һ���Ᵽ�浽����v��
void calcNQueen(vector<POSTION> &v, int n);

// ��N�ʺ���������нⱣ�浽����vAllResult��
void calcAllNQueen(vector<vector<POSTION>> &vAllResult, int n);

// ��N�ʺ������ĸ���д���ļ�
void writeToFile(const char* file, vector<RESULT> &result);

#endif // _DEFINE_H_