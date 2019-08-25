#pragma once
#include"DisFieldMesh.h"
class DisMesh
{
public:
	bool isGenDisF;
	bool isLoad;
	std::vector<Vec3> vertP;
	std::vector<Vec3> color;
	std::vector<double> dis;
	std::vector<int> vI;
	std::vector<LineIndex> borderI;
	DisFieldMesh* dfm;
	DisMesh();
	void GenDisF();	
	~DisMesh();
	bool loadTXT(const char*fn);
private:
	Vec3 maxP;
	Vec3 minP;
	double quaSize;
	int borderSize;
	double maxDis;
};
