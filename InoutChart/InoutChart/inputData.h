#pragma once

#include "define.h"

// ���ܣ�	���ļ��е�����-�����Ϣ���浽info�����У����һ�����ж��������ֻ���浱���������
// ������	filePath��	������Ϣ���ļ�·��
//			info��		��Ϣ����info������
// ����ֵ��	true��		ȡ����Ϣ�ɹ�
//			false��		ȡ����Ϣʧ��
bool getDataFromFile(const wchar_t* filePath, DAY_REMAIN_DATA_VECTOR &dataVector);
