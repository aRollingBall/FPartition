#pragma once
#include"baseType.h"
#include<vector>
#include<set>
const int VAFNUMS = 50000;
enum TypeField
{
	Distance, Thermal,
};
enum FS_Dir
{
	left_up, left_down, right_up, right_down,
};
struct QuaVInto
{
	double value;
	int dirI[4];		//上，左，下，右
	bool isBoundary;
};
struct QuaV {
	Vec3 pos;
	Vec3 color;
	QuaV() {  }
};
struct QuaF {
	int cornerPI[4];
	QuaF() {  }
	//左上，左下，右下，右上
	QuaF(int i, int j, int z, int k) { cornerPI[0] = i; cornerPI[1] = j; cornerPI[2] = z; cornerPI[3] = k; }
};