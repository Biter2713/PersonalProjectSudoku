#include<iostream>
#include<cstring>
#include<string>
#include<vector>
#include<ctime>
#include<cmath>
#include<fstream>
#include<algorithm>
using namespace std;

#pragma region BuildSudokuStatement
char Permutation[15] = { '1','2','3','4','5','7','8','9' };
char OutputData[200000000];
int datap;
int BuildCnt, NeedCnt;
ofstream OutputFile("sudoku.txt");
ofstream OutputFileSolveSudoku("sudoku.txt");
void BuildAns();
void OutputBuildSudoku();
void BuildSudoku(char *row1rule, char*row2rule, char*row3rule);
#pragma endregion
#pragma region SolveSudokuStatement
char vis[3][10][10];
char flag[9][9];
char map[9][9];
bool IsFindAns = false, IsFirstQuestion = true;
void InputSudokuQuestion(int row,char* save);
void SolveSudoku(int r, int c);
void SetVis(int r, int c, int num);
void ResetVis(int r, int c, int num);
#pragma endregion


void BuildSudoku(char *row1rule, char*row2rule, char*row3rule)
{
	for (int i = 0; i < 3; i++)//循环读取平移规则
	{
		//OutputData数组为最终输出数组，要存入数据，空格，换行。
		OutputData[datap++] = Permutation[(8+row1rule[i]-'0')%9];//将学号对应的号码插入排列的尾部
		for (int j = 1;j<17; j++)
		{
			OutputData[datap++] = ' ';//存入空格
			j++;
			OutputData[datap++] = Permutation[((16-j)/2 + row1rule[i] - '0') % 9 ];//将数组转为环形，方便平移
		}
		OutputData[datap++] = '\n';//存入换行符
	}
	for (int i = 0; i < 3; i++)
	{
		OutputData[datap++] = Permutation[(8 + row2rule[i] - '0') % 9 ];
		for (int j = 1; j<17; j++)
		{
			OutputData[datap++] = ' ';
			j++;
			OutputData[datap++] = Permutation[((16-j) / 2  + row2rule[i] - '0') % 9 ];
		}
		OutputData[datap++] = '\n';
	}
	for (int i = 0; i < 3; i++)
	{
		OutputData[datap++] = Permutation[(8 + row3rule[i] - '0') % 9 ];
		for (int j = 1; j<17; j++)
		{
			OutputData[datap++] = ' ';
			j++;
			OutputData[datap++] = Permutation[((16-j) / 2  + row3rule[i] - '0') % 9 ];
		}
		OutputData[datap++] = '\n';
	}
	OutputData[datap++] = '\n';
}
void BuildAns()
{
	char rule1[10][5] = { "036","063" },rule2[10][5] = { "258","285","528","582","825","852" }, rule3[10][5] = { "147","174","417","471","714","741" };
	do
	{
		Permutation[8] = '6';//插入由学号获得的固定数字到排列尾部
		for (int i = 0; i < 2; i++)//组合获得72种排列方式
		{
			for (int j = 0; j < 6; j++)
			{
				for (int k = 0; k < 6; k++)
				{
					BuildSudoku(rule1[i], rule2[j], rule3[k]);//从排列利用变换获得完整数独
					if (++BuildCnt == NeedCnt)
						return;
				}
			}
		}
	} while (next_permutation(Permutation, Permutation + 8));//利用STL库获得下一个全排列
}
void OutputBuildSudoku()
{
	OutputFile << OutputData;
}
void SetVis(int r, int c,int num)
{
	vis[0][r][num] = 1;
	vis[1][c][num] = 1;
	vis[2][r / 3 * 3 + c / 3][num] = 1;
}
void ResetVis(int r, int c, int num)
{
	vis[0][r][num] = 0;
	vis[1][c][num] = 0;
	vis[2][r / 3 * 3 + c / 3][num] = 0;
}
bool CheckCanVis(int r, int c, int num)
{
	if (vis[0][r][num] == 0 && vis[1][c][num] == 0 && vis[2][r / 3 * 3 + c / 3][num] == 0)
		return true;
	else
		return false;
}
void OutputSolveSudoku()
{
	char outputsave[1000];
	int p = 0;
	for (int i = 0; i < 9; i++)
	{
		for (int j = 0; j < 9; j++)
		{
			outputsave[p++] = map[i][j];
			if ((p + 1) % 18 == 0)
				outputsave[p++] = '\n';
			else
				outputsave[p++] = ' ';
		}
	}
	OutputFileSolveSudoku << outputsave;
}
void InputSudokuQuestion(int row,char *save)
{
	for (int j = 0; j < 17; j++)
	{
		if (j % 2 == 0)
		{
			map[row][j / 2] = save[j];
			SetVis(row, j / 2, save[j] - '0');
		}
	}
}
void SolveSudoku(int r, int c)
{
	while (map[r][c] != '0')
	{
		if (c < 8) c++;
		else c = 0, r++;
		if (r == 9)
		{
			IsFindAns = true;
			return;
		}
	}
	bool isSearch = false;
	for (int i = 1; i <= 9; i++)
	{
		if (CheckCanVis(r,c,i))
		{
			SetVis(r, c, i);
			map[r][c] = i+'0';
			isSearch = true;
			SolveSudoku(r, c);
		}
		if (isSearch)
		{
			isSearch = false;
			if (IsFindAns)
				return;
			else
			{
				map[r][c] = '0';
				ResetVis(r, c, i);
			}
		}
	}
}
int main(int argc,char*argv[])
{
	clock_t start, finish;
	FILE *fp1 = fopen(argv[2], "r");
	double totaltime;
	start = clock();
	if (argc == 3 && strcmp(argv[1], "-c") == 0)
	{
		int len = strlen(argv[2]);
		for (int i = 0;i<len; i++)
		{
			if (argv[2][i] >= '0'&&argv[2][i] <= '9')
			{
				NeedCnt += ((argv[2][i] - '0')*pow(10, len - i - 1));
			}
			else
			{
				printf("Error!\n");
				return 0;
			}
		}
		while (BuildCnt < NeedCnt)
			BuildAns();
		OutputData[datap++] = '\0';
		OutputBuildSudoku();
	}
	else if (argc == 3 && strcmp(argv[1], "-s") == 0)
	{
		//和读到字符数组哪个效率高？
		char save[100];
		int cnt = 0;
		while (fgets(save,20,fp1))
		{
			if (strcmp(save, "\n") == 0)
				continue;
			InputSudokuQuestion(cnt,save);
			cnt++;
			if (cnt == 9)
			{
				if (!IsFirstQuestion)
				{
					OutputFileSolveSudoku << endl;
				}
				else
					IsFirstQuestion = false;
				IsFindAns = false;
				SolveSudoku(0,0);
				cnt = 0;
				OutputSolveSudoku();
				memset(vis, 0, sizeof(vis));
			}
		}
	}
	else {
		cout << "Error!" << endl;
	}
}
