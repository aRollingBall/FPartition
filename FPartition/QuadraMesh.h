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
	bool isGenDomainValue;	//��¼�Ƿ����ɾ�����������ֵ
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
	//�����߽磬�õ�����ֵ
	virtual void GetDomainValue(std::vector<LineVs>& traiB) = 0;
	//virtual void GetAloa(std::vector<LineVs>& traiB) = 0;
	void GetInfoFace(Vec3 tarP);

	double GetInfoVToW(Vec3 targetP, TypeField type);
	//ͨ���߶��ҵ���С��������
	void getQuaEAloaF(LineVs vs, std::set<int> &faceI);
	//ͨ�����ҵ���С��������
	void getQuaVAloaF(Vec3 targetP, std::set<int> &faceI);
	void getQuaMeshInfo(int& col, int& row, Vec3& minP, Vec3& maxP, double& specifcQua);
	void getQuaMeshInfo(int& col, int& row, Vec3& minP, Vec3& maxP);

};


