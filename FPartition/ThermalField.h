#pragma once
#include"QuadraMesh.h"
class ThermalField :public QuadraMesh
{
public:
	bool isGenTF;	//��¼�Ƿ�������ȼ�ճ�
	QuadraMesh* disField;
	ThermalField(Vec3 minPoint, Vec3 maxPoint, double spec, QuadraMesh* dis) :QuadraMesh(minPoint, maxPoint, spec) {
		this->minPoint = getMinPos();//�ڸ�������С������΢��
		this->maxPoint = maxPoint;
		this->specifcQua = spec;
		this->difCol = this->getCol();
		this->difRow = this->getRow();
		this->isGenTF = false;
		disField = dis;
	}
	~ThermalField();
	//�����߶α߽�õ�������߽�
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

