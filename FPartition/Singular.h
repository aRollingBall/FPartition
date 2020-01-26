#pragma once
#include"baseType.h"
#include"SLPoint.h"
/*奇点：有真实存在的，也有为了处理方便虚拟出的。
虚拟奇点有如下情况：
（1）流线与流线相交；
（2）流线与边界相交；
（3）边界拐点；
（4）在边界曲线无任何奇点时，随机给出该曲线上一点，该情况在处理方式上与（2）。
*/
enum singularType
{
	SV_UND, SV_COR, SV_BOR, SV_THR, SV_FOU, SV_FIV, SV_THR_BOR
};
class Singular
{
public:
	int faceI;//奇点所在面的索引，初值为：-1
	int vertI;//奇点所在点的索引，初值为：-1 
	Vec3 pos;
	int edgeI;//奇点所在边的索引，初值为：-1
	int streamLines[5];
	int slc;	//流线数量
	Vec3 color;
	std::vector<int> bFaceI;
	singularType svt;
	SLPoint dir[5];	//对应流线的方向
	double bgSize;
	bool isSelect;
	std::vector<int> relatedBorderVerticesId;
	Singular()
	{
		bFaceI.clear();
		this->faceI = -1;	//-3代表流线到达边界的节点；-2代表由边界引出的奇点，-4代表边界有效转角接近90度的点
		this->vertI = -1;
		this->pos = Vec3();
		this->edgeI = -1;
		this->slc = 0;
		this->color = Vec3();
		this->svt = SV_UND;
		this->isSelect = false;
		this->bgSize = 0.0;
		//this->color = Vec3(1.0, 0.0, 0.0);
		//对于边界点，第四五个元素是边界流线
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