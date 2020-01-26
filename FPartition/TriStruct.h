#pragma once
#include"HalfEdgeStruct.h"	
#include <map>
#include <Eigen\Dense>
struct Vert {
	Vec3 pos;
	Vec3 selectedColor;
	Vec3 aloaColor;
	Vec3 disColor;
	Vec3 difColor;
	Vec3 normColor;
	double disValue;
	double difValue;
	double uValue;
	Vert() :aloaColor(Vec3()), disColor(Vec3()), difColor(Vec3()), normColor(Vec3()), disValue(-1.0), difValue(4.0), uValue(0.0) { }
};
struct Face
{
	int index[3];
	Face() {
	}
	Face(int i, int j, int k) { 
		index[0] = i; 
		index[1] = j;
		index[2] = k;
	}
};
struct LearningResult
{
	int id;
	int vertex_id;
	int tag[2];
    double fv1_angle_100=0.0;
	std::vector<int> potential_cells;
	double frame_vector[4] = { 0.0 };
	double prop[4] = { 0.0 };
	bool potential=false;
	bool flag=false;
	LearningResult(int vid, int tag1, int tag2) {
		vertex_id = vid;
		this->tag[0] = tag1;
		this->tag[1] = tag2;
	}
};

struct BorderVertex
{
	int id;
	int vertexId;
	Vec3 frameVector;
	Vec3 pos;
	Vec3 injectionVector;
	int singularityId;
	int singularityVectorId;
	int prevId;
	int nextId;
	int maxIntersectionBorderId;
	Vec3 maxIntersectionBorderPoint;
	double maxIntersectionBorderDist;
};
struct Intersection {
	int id;
	int prevId;
	int nextId;
	Vec3 pos;
	Vec3 minDistBorderVertexPos;
	Vec3 minDistSingularityPos;
	int borderId;
	int slId;
	int singularityId;
	int tag;
	bool flag;
};
struct NormalVtx {
	int id;
	Vec3 pos;
	double normalizeRate;
	Vec3 vert1_fv[2];
	Vec3 vert2_fv[2];
	Vec3 vert3_fv[2];
	Vec3 vert1Pos;
	Vec3 vert2Pos;
	Vec3 vert3Pos;
	Vec3 nextPointPos;
};
