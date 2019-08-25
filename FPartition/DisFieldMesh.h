#pragma once
#include"QuadraMesh.h"

class DisFieldMesh : public QuadraMesh
{

public:
	bool isGenDFM;	//记录是否得到了距离场	
	DisFieldMesh(Vec3 minPoint, Vec3 maxPoint, double spec) :QuadraMesh(minPoint, maxPoint, spec)
	{
		this->minPoint = getMinPos();//在父类中最小点做了微调
		this->maxPoint = maxPoint;
		this->specifcQua = spec;
		this->disCol = this->getCol();
		this->disRow = this->getRow();
		this->disValue = 0.0;
		this->isGenDFM = false;
	}
	~DisFieldMesh();
	//给定线段边界得到矩形域边界
	void  GetDomainValue(std::vector<LineVs>& traiB);
	void  GetDomainValue(std::vector<Vec3>& traiB,const int len);
	
private:
	Vec3 minPoint;
	Vec3 maxPoint;
	double specifcQua;
	int disCol;
	int disRow;
	double disValue;
	//计算点到给定线段的最短距离
	void getMinDisV(int targetPI, LineVs vs);
	//距离场扫描方法
	void FSMP();
	//辅助扫描计算，参数二为扫描方向
	void auxFun(int vertI, FS_Dir Dir);
	//根据给定值得到顶点颜色值
	Vec3 SelectColor(double vc);
	//通过给定范围内的任意点计算其距离（该点到任意边界的最短距离）
	//double GetDisPToM(Vec3 targetP);	
};

