#pragma once
#include"baseType.h"
#include"SLPoint.h"
/*��㣺����ʵ���ڵģ�Ҳ��Ϊ�˴�����������ġ�
������������������
��1�������������ཻ��
��2��������߽��ཻ��
��3���߽�յ㣻
��4���ڱ߽��������κ����ʱ�����������������һ�㣬������ڴ���ʽ���루2����
*/
enum singularType
{
	SV_UND, SV_COR, SV_BOR, SV_THR, SV_FOU, SV_FIV, SV_THR_BOR
};
class Singular
{
public:
	int faceI;//������������������ֵΪ��-1
	int vertI;//������ڵ����������ֵΪ��-1 
	Vec3 pos;
	int edgeI;//������ڱߵ���������ֵΪ��-1
	int streamLines[5];
	int slc;	//��������
	Vec3 color;
	std::vector<int> bFaceI;
	singularType svt;
	SLPoint dir[5];	//��Ӧ���ߵķ���
	double bgSize;
	bool isSelect;
	std::vector<int> relatedBorderVerticesId;
	Singular()
	{
		bFaceI.clear();
		this->faceI = -1;	//-3�������ߵ���߽�Ľڵ㣻-2�����ɱ߽���������㣬-4����߽���Чת�ǽӽ�90�ȵĵ�
		this->vertI = -1;
		this->pos = Vec3();
		this->edgeI = -1;
		this->slc = 0;
		this->color = Vec3();
		this->svt = SV_UND;
		this->isSelect = false;
		this->bgSize = 0.0;
		//this->color = Vec3(1.0, 0.0, 0.0);
		//���ڱ߽�㣬�������Ԫ���Ǳ߽�����
		for (int i = 0; i < 5; i++) {
			this->streamLines[i] = -1;
		}
	}
	~Singular() {};
	void setColor() {
		switch (this->faceI) {
		case -4:
			this->color = Vec3(1.0, 0.0, 0.0);
			break;
		case -2:
			this->color = Vec3(0.0, 0.0, 1.0);
			break;
		case -3:
			this->color = Vec3(0.0, 1.0, 0.0);
			break;
		default:
			this->color = Vec3(1.0, 0.0, 0.0);
			break;
		}
	}
};
struct fourNode
{
	Vec3 color;
	Vec3 pos;
	int streamLine1;
	int streamLine2;
	int psl[2];
	int singularI;
	int faceI;
	fourNode(Vec3 pos, int streamLine1, int streamLine2, int psl[2], int faceI)
		:pos(pos), streamLine1(streamLine1), streamLine2(streamLine2), singularI(-1), faceI(faceI), color(Vec3(1.0, 0.6, 0.3)) {
		this->psl[0] = psl[0];
		this->psl[1] = psl[1];
	}
};
struct polePoint
{
	Vec3 pos;
	Vec3 color;
	polePoint(Vec3 p, Vec3 c) :pos(p), color(c) {  }
};