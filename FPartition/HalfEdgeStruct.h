#pragma once
#include "baseType.h"
struct HE_vert {
	int aloaN;
	int edgeI;
	Vec3 pos;
	Vec3 vec;
	bool isBoundary;
	int bType;	//18.9.12 to gm2, border index
	int type;	//18.5.15 �߽����������;���߽��������κ������������κ����߾����ñ߽����ߣ�
	int vType;
	HE_vert(Vec3 pos, Vec3 vec) :bType(-1), edgeI(-1), pos(pos), vec(vec), isBoundary(false), type(-1), aloaN(-1) { vType = 0; }
};
struct HE_edge {
	int index;
	int vertI;
	int pairI;
	int faceI;
	int nextI;
	int frontI;
	int flag;
	HE_edge(int index, int vertI, int faceI, int nextI, int frontI)
		:index(index), vertI(vertI), pairI(-1), faceI(faceI), nextI(nextI), frontI(frontI),flag(-1) {}
	HE_edge(const HE_edge& b) {
		index = b.index;
		vertI = b.vertI;
		pairI = b.pairI;
		faceI = b.faceI;
		nextI = b.nextI;
		frontI = b.frontI;
		flag = b.flag;
	}
	void operator = (const HE_edge& b) {
		index = b.index;
		vertI = b.vertI;
		pairI = b.pairI;
		faceI = b.faceI;
		nextI = b.nextI;
		frontI = b.frontI;
		flag = b.flag;
	}
};
struct HE_face {
	int index;
	int edgeI;
	int sfaceI;
	int lfaceI;
	bool is_selected;
	HE_face(int index, int edgeI)
		:index(index), edgeI(edgeI), sfaceI(-1), lfaceI(-1),is_selected(false) {}
	HE_face(const HE_face& b) {
		this->index = b.index;
		edgeI = b.edgeI;
		sfaceI = b.sfaceI;
		lfaceI = b.lfaceI;
		is_selected = b.is_selected;
	}
	void operator = (const HE_face& b) {
		index = b.index;
		edgeI = b.edgeI;
		sfaceI = b.sfaceI;
		lfaceI = b.lfaceI;
		is_selected = b.is_selected;
	}
};

