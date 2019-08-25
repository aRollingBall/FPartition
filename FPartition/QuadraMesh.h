#pragma once
#include"QuaStruct.h"
class QuadraMesh
{
private:
	Vec3 minPoint;
	Vec3 maxPoint;
	double specifcQua;
	int col;
	int row;
	bool isGenDomainValue;	//记录是否生成矩形网格领域值
	void genQuadraMesh();
protected:
	Vec3 getMinPos() { return minPoint; }

public:
	int getCol() { return this->col; }
	int getRow() { return this->row; }
	std::vector<QuaV> qvert;
	std::vector<QuaVInto> qvertInto;
	std::vector<QuaF> qface;
	std::vector<bool> qft;
	std::vector<bool> bqft;
	bool isGenQuaMesh;
	void setGenDomainValueState(bool is) { this->isGenDomainValue = is; }
	QuadraMesh(Vec3 minPoint, Vec3 maxPoint, double specifcQua)
	{
		this->minPoint = minPoint; this->maxPoint = maxPoint; this->specifcQua = specifcQua;
		this->isGenDomainValue = false;
		genQuadraMesh();
	};
	~QuadraMesh() {	};
	//给定边界，得到场域值
	virtual void GetDomainValue(std::vector<LineVs>& traiB) = 0;
	//virtual void GetAloa(std::vector<LineVs>& traiB) = 0;
	void GetInfoFace(Vec3 tarP);

	double GetInfoVToW(Vec3 targetP, TypeField type);
	//通过线段找到最小包含矩形
	void getQuaEAloaF(LineVs vs, std::set<int> &faceI);
	//通过点找到最小包含矩形
	void getQuaVAloaF(Vec3 targetP, std::set<int> &faceI);
	void getQuaMeshInfo(int& col, int& row, Vec3& minP, Vec3& maxP, double& specifcQua);
	void getQuaMeshInfo(int& col, int& row, Vec3& minP, Vec3& maxP);

};


