#pragma once
#include"QuadraMesh.h"
class ThermalField :public QuadraMesh
{
public:
	bool isGenTF;	//记录是否生成了燃烧场
	QuadraMesh* disField;
	ThermalField(Vec3 minPoint, Vec3 maxPoint, double spec, QuadraMesh* dis) :QuadraMesh(minPoint, maxPoint, spec) {
		this->minPoint = getMinPos();//在父类中最小点做了微调
		this->maxPoint = maxPoint;
		this->specifcQua = spec;
		this->difCol = this->getCol();
		this->difRow = this->getRow();
		this->isGenTF = false;
		disField = dis;
	}
	~ThermalField();
	//给定线段边界得到矩形域边界
	void GetDomainValue(std::vector<LineVs> &traiB) {};
	void GetDomainValue(std::vector<BELine> &traiB);
private:
	Vec3 minPoint;
	Vec3 maxPoint;
	double specifcQua;
	int difCol;
	int difRow;
	Vec3 SelectColor(double vc);
	bool DifScan();
	void initialBoundaryValue();
	void FSMP();
	void auxFun(int vertI, FS_Dir dir);
};

