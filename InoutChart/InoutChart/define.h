#pragma once

#include <vector>
using std::vector;

#define NULL 0

typedef struct
{
	int day;		// ����
	int remain;		// ���
} ONE_DAY_REMAIN;


// �����е�����-�����Ϣ���浽vector��
typedef vector<ONE_DAY_REMAIN> DAY_REMAIN_DATA_VECTOR;