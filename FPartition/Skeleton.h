#pragma once
#include "baseType.h"
#include "StreamLinePointCloud.h"
#include "Parameter.h"
typedef vector<SVertex> Curve;
class Branch
{
public:
	Curve curve;
	Vec3 back_up_head;
	Vec3 back_up_tail;
	int branch_id;
	ParameterSet *para;
public:
	Branch():back_up_head(Vec3(-10,-10,-10)),
		back_up_tail(Vec3(-10,-10,-10)),branch_id(0) {
		
	}
	~Branch() {
	}
	Branch(const Branch &b) {
		curve = b.curve;
		back_up_head = b.back_up_head;
		back_up_tail = b.back_up_tail;
		branch_id = b.branch_id;
	}
	Branch &operator=(const Branch &b) {
		if (&b != this) {
			curve = b.curve;
			back_up_head = b.back_up_head;
			back_up_tail = b.back_up_tail;
			branch_id = b.branch_id;
		}
	}
	void pushBackVertex(SVertex &new_v);
	SVertex getSVertexOfIndex(int index) { return curve[index]; }
	int getSize();
	bool isEmpty();
	bool isHeadBridge() { return curve[0].is_skel_bridge; }
	bool isTailBridge() { return curve[curve.size() - 1].is_skel_bridge; }
	bool isBridgeHeadHealthy();
	bool isBridgeTailHealthy();
	void rememberBridgeHead();
	void rememberBridgeTail();

	double getNodeAngle(int idx);
	double  getHeadAngle() { return getNodeAngle(0); }
	double getTailAngle() { return getNodeAngle(curve.size() - 1); }

	double getNodeLengthEulerDist(int idx);
	double getNodeLengthEulerDistSquare(int idx);
	double getHeadLengthEulerDist();
	double getHeadLengthEulerDistSquare();
	double getTailLengthEulerDist();
	double getTailLengthEulerDistSquare();

	Vec3 getBridgeTailDirection();
	Vec3 getTrueTailDirection();
	Vec3 getBridgeHeadDirection();
	Vec3 getTrueHeadDirection();

	Vec3 getHead() { return curve[0].pos; }
	Vec3 getTail() { return curve[curve.size() - 1].pos; }

	bool moveHeadToPt(Vec3 p_target);
	bool moveTailToPt(Vec3 p_target);

	void inactiveAndKeepBridgeTail();
	void inactiveAndDropBridgeTail();
	void inactiveAndKeepBridgeHead();
	void inactiveAndDropBridgeHead();

	void reverseBranch();
};

class RecordItem

{
public:
	RecordItem(){}
	RecordItem(int i, int j, int k = 0) :branch_i(i), node_j(j), sample_index(k) {}
	int branch_i;
	int node_j;
	int sample_index;
};

class Skeleton
{
public:
	Skeleton() { size = 0; branch_num = 0; }
	Skeleton(const Skeleton &s) {
		branches = s.branches;
		size = s.size;
		branch_num = s.branch_num;
		branch_sample_map = s.branch_sample_map;
		chosen_branches = s.chosen_branches;
	}
	Skeleton &operator=(const Skeleton &s) {
		branches = s.branches;
		size = s.size;
		branch_num = s.branch_num;
		branch_sample_map = s.branch_sample_map;
		chosen_branches = s.chosen_branches;
		return *this;
	}

	bool isEmpty() { return branches.empty(); }
	void generateBranchSampleMap();

	void clear() {
		branches.clear();
		branch_sample_map.clear();
		size = 0;
	}
public:
	vector<Branch> branches;
	vector<RecordItem> branch_sample_map;
	vector<int> chosen_branches;

	int size;
	int branch_num;
};