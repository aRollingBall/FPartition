#pragma once
#include <vector>
#include "baseType.h"
using namespace std;
class SVertex
{
public:
	int index;
	Vec3 pos;
	Vec3 N;
	vector<int> neighbors;
	vector<int> original_neighbors;
	bool is_singular_point;
	bool is_skel_ignore;
	bool is_fixed_original_weight;
	/*for skeletonization*/
	double eigen_confidence;
	Vec3 eigen_vector0;//Associate with the biggest eigen value
	Vec3 eigen_vector1;//Also use for remember last betther virtual point
	Vec3 frame_vector0;
	Vec3 frame_vector1;
	bool is_skel_bridge;
	bool is_skel_branch;
	bool is_fixed_sample;

	double skel_radius;//remember radius for branch
public:
	SVertex() :
		index(0),
		eigen_confidence(0),
		is_skel_branch(false),
		is_skel_bridge(false),
		is_singular_point(false),
		is_skel_ignore(false),
		is_fixed_sample(false),
		eigen_vector0(Vec3(0,0,0)),
		eigen_vector1(Vec3(0,0,0)),
		frame_vector0(Vec3(0, 0, 0)), 
		frame_vector1(Vec3(0, 0, 0)),
		pos(Vec3(0, 0, 0)),
		N(Vec3(0,0,0)){

	}

	~SVertex();
	void remove()
	{
		neighboors.clear();
		is_skel_ignore = true;
		pos = Vec3(88888888, 88888888, 88888888);
	}
	bool isSample_Moving()
	{
		return (!is_skel_ignore && !is_fixed_sample && !is_skel_branch);
	}
	bool isSample_JustMoving()
	{
		return (!is_skel_ignore && !is_fixed_sample && !is_skel_branch && !is_skel_bridge);
	}
	bool isSample_MovingAndVirtual()
	{
		return (!is_skel_ignore&&is_fixed_sample && !is_skel_bridge && !is_skel_branch);
	}
	bool isSample_FixedAndBranched()
	{
		return (!is_skel_ignore&&is_fixed_sample && !is_skel_bridge&&is_skel_branch);
	}
	
	void setSample_JustMoving()
	{
		is_fixed_sample = false;
		is_skel_bridge = false;
		is_skel_branch = false;
	}
	void setSample_MovingAndBridge()
	{
		is_fixed_sample = false;
		is_skel_bridge = true;
		is_skel_branch = false;
	}
	void setSample_JustFixed()
	{
		is_fixed_sample = true;
		is_skel_branch = false;
		is_skel_branch = false;
	}
	void setSample_FixedAndBranched()
	{
		is_fixed_sample = true;
		is_skel_branch = true;
		is_skel_bridge = false;
	}

};

class SMesh
{
public:
	SMesh() {}
	//void openFile();

public:
	vector<SVertex> vert;
	typedef vector<SVertex>::iterator VertexIterator;
	int vn;
};