#pragma once
#include"baseType.h"
#include<map>
#include<vector>
struct SFace
{
	int faceI;
	//std::set<int> cps;
	//int:流线索引；Vec3:流线切向量
	std::map<int, Vec3> slIs;	
	//std::set<int> slIs;
	SFace(int faceI, int slI, Vec3 v) :faceI(faceI) { slIs[slI] = v; }
	void addSLI(int slI, Vec3 v) { slIs[slI] = v; }
};