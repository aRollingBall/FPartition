#pragma once
#include"QuadraMesh.h"

class DisFieldMesh : public QuadraMesh
{

public:
	bool isGenDFM;	//��¼�Ƿ�õ��˾��볡	
	DisFieldMesh(Vec3 minPoint, Vec3 maxPoint, double spec) :QuadraMesh(minPoint, maxPoint, spec)
	{
		this->minPoint = getMinPos();//�ڸ�������С������΢��
		this->maxPoint = maxPoint;
		this->specifcQua = spec;
		this->disCol = this->getCol();
		this->disRow = this->getRow();
		this->disValue = 0.0;
		this->isGenDFM = false;
	}
	~DisFieldMesh();
	//�����߶α߽�õ�������߽�
	void  GetDomainValue(std::vector<LineVs>& traiB);
	void  GetDomainValue(std::vector<Vec3>& traiB,const int len);
	
private:
	Vec3 minPoint;
	Vec3 maxPoint;
	double specifcQua;
	int disCol;
	int disRow;
	double disValue;
	//����㵽�����߶ε���̾���
	void getMinDisV(int targetPI, LineVs vs);
	//���볡ɨ�跽��
	void FSMP();
	//����ɨ����㣬������Ϊɨ�跽��
	void auxFun(int vertI, FS_Dir Dir);
	//���ݸ���ֵ�õ�������ɫֵ
	Vec3 SelectColor(double vc);
	//ͨ��������Χ�ڵ�������������루�õ㵽����߽����̾��룩
	//double GetDisPToM(Vec3 targetP);	
};

