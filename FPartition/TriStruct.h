#pragma once
#include"HalfEdgeStruct.h"	
struct Vert {
	Vec3 pos;
	Vec3 selectedColor;
	Vec3 aloaColor;
	Vec3 disColor;
	Vec3 difColor;
	Vec3 normColor;
	double disValue;
	double difValue;
	double uValue;
	Vert() :aloaColor(Vec3()), disColor(Vec3()), difColor(Vec3()), normColor(Vec3()), disValue(-1.0), difValue(4.0), uValue(0.0) { }
};
struct Face
{
	int index[3];
	Face() {
	}
	Face(int i, int j, int k) { 
		index[0] = i; 
		index[1] = j;
		index[2] = k;
	}
};
