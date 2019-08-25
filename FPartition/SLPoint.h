#pragma once
#include"baseType.h"
class SLPoint
{
public:
	Vec3 color;
	Vec3 pos;
	Vec3 vec;
	int edgeI;
	int vertI;			//非边界点，非奇点，非3000内的点，-2标识；问题点，-3标识
	int slI;
	SLPoint() { this->pos = Vec3(); this->edgeI = -2; this->vertI = -1; this->vec = Vec3();  slI = -1; this->color = Vec3(0.5, 0.5, 0.5); };
	SLPoint(Vec3 pos, int edgeI, int vertI, Vec3 vec, int slI) : pos(pos), edgeI(edgeI), vertI(vertI), vec(vec), slI(slI) { this->color = Vec3(); }
	~SLPoint() {};
	bool isEmpty() { return pos.norm() < interDim  && vec.norm() < interDim && edgeI == -2 && vertI == -1 && slI == -1; }
	void operator = (const SLPoint& sp) { pos = sp.pos; vec = sp.vec; edgeI = sp.edgeI; vertI = sp.vertI; slI = sp.slI; color = sp.color; }
};
struct PCPoint {
	Vec3 pos;
	Vec3 color;
	PCPoint(Vec3 p, Vec3 c) : pos(p), color(c) {}
	PCPoint(Vec3 p) : pos(p) { this->color = Vec3(0.0, 0.0, 1.0); }
	PCPoint() { pos = Vec3(); color = Vec3(1.0, 0.0, 0.0); }
};