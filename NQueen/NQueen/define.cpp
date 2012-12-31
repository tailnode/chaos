#include "define.h"
#include <fstream>
using namespace std;

bool isConflict(POSTION p1, POSTION p2)
{
	if ((p1.column == p2.column) ||									// ÁÐ³åÍ»
		(p1.row - p1.column == p2.row - p2.column) ||				// Ð±Ïß³åÍ»
		(p1.row + p1.column == p2.row + p2.column))					// Ð±Ïß³åÍ»
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool getPos(POSTION &p, const vector<POSTION> &v, int n)
{
	bool findPos = false;

	if (v.empty() && p.column < n)
	{
		return true;
	}

	while (p.column < n)
	{
		findPos = true;

		for (size_t j = 0; j < v.size(); j++)
		{
			if (isConflict(p,v.at(j)))
			{
				p.column++;
				findPos = false;
				break;
			}
		}

		if (true == findPos)
		{
			break;
		}
	}

	return findPos;
}

void calcNQueen(vector<POSTION> &v, int n)
{
	POSTION p = {0, 0};

	do
	{		
		if (true == getPos(p, v, n))
		{
			v.push_back(p);
			if (n - 1 == p.row)
			{
				break;
			}
			else
			{
				p.row++;
				p.column = 0;
			}
		}
		else
		{
			p = v.back();
			v.erase(v.end() - 1);
			p.column++;

			if (p.column >= n && p.row == 0)
			{
				break;
			}
		}
	} while (1);
}


void calcAllNQueen(vector<vector<POSTION>> &vAllResult, int n)
{
	POSTION p = {0, 0};
	vector<POSTION> v;

	do
	{		
		if (true == getPos(p, v, n))
		{
			v.push_back(p);
			if (n - 1 == p.row)
			{
				vAllResult.push_back(v);
				p = v.back();
				v.erase(v.end() - 1);
				p.column++;

				if (p.column >= n && p.row == 0)
				{
					break;
				}
			}
			else
			{
				p.row++;
				p.column = 0;
			}
		}
		else
		{
			p = v.back();
			v.erase(v.end() - 1);
			p.column++;

			if (p.column >= n && p.row == 0)
			{
				break;
			}
		}
	} while (1);
}

void writeToFile(const char* file, vector<RESULT> &result)
{
	ofstream outfile(file, ios::out);

	for (size_t i = 0; i < result.size(); i++)
	{
		outfile << "N = "<< result.at(i).n << ",\tNum = " << result.at(i).resultNum << endl;
	}

	outfile.close();
}