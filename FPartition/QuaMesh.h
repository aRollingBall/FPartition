#pragma once
#include"global.h"
struct Quadra
{
public:
	int slI[4];
	int slP[4];
	int color;
	Quadra(int slI1, int slI2, int slI3, int slI4, int slp1, int slp2, int slp3, int slp4) {
		this->slI[0] = slI1;
		this->slI[1] = slI2;
		this->slI[2] = slI3;
		this->slI[3] = slI4;
		this->slP[0] = slp1;
		this->slP[1] = slp2;
		this->slP[2] = slp3;
		this->slP[3] = slp4;
		this->color = -1;
	}
};
struct vertAloa {
	int faceI;
	int napt;
	int nlpt;
	int vaI[5];	//任何一个节点其邻接点不超过五个,
	int vlI[5];		//任何一个节点其对角点不超过五个
	Vec3 pos;
	Vec3 npos;	//光顺后的位置
	int vertType;	//节点类型，对于需要彻底改变的点设为0，边界上的点设为1，不动点为2
	Vec3 color;

	vertAloa(Vec3 pos, int vertType, int index) :npos(pos), pos(pos), vertType(vertType), faceI(index), napt(0), nlpt(0) { for (int i = 0; i < 5; i++) { vaI[i] = -1; vlI[i] = -1; }this->color = Vec3(); }
	int getVaI(int i) { return vaI[i]; }
	int getVlI(int i) { return vlI[i]; }
	bool addVaI(int value) {
		int i = 0;
		for (; i < 5 && i < napt; i++) {
			if (vaI[i] == value) return true;
			if (i == 4) break;
		}
		return i == napt ? vaI[napt++] = value, true : false;
	}
	bool addVlI(int value) {
		int i = 0;
		for (; i < 5 && i < nlpt; i++) {
			if (vlI[i] == value) return true;
			if (i == 4) break;
		}
		return i == nlpt ? vlI[nlpt++] = value, true : false;
	}
};
struct quadraPInfo {
	double ykbA;	//面积比
	double ykbB;	//长宽比
	double angle[4];
	quadraPInfo(double a, double b, double am[4]) :ykbA(a), ykbB(b) {
		angle[0] = am[0];
		angle[1] = am[1];
		angle[2] = am[2];
		angle[3] = am[3];
	}
	void update(double a, double b, double am[4]) {
		this->ykbA = a;
		this->ykbB = b;
		angle[0] = am[0];
		angle[1] = am[1];
		angle[2] = am[2];
		angle[3] = am[3];
	}
};