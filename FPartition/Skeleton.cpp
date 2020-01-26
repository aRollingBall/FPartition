#include "Skeleton.h"
#include "GlobalFunction.h"
Vec3 Branch::getTrueHeadDirection()
{
	if (!isHeadBridge()) {
		return (curve[0].pos - curve[1].pos).normalized();
	}
	else {
		return (curve[1].pos - curve[2].pos).normalized();
	}
}

bool Branch::isBridgeHeadHealthy()
{
	if(!isHeadBridge())
	    return false;
	double save_bridge_angle = para->getSaveBridgeAngle();
	double save_bridge_dist = para->getBranchesMergeMaxDist();
	double bad_bridge_angle = para->getBadBridgeAngle();
	double follow_dist = para->getFollowSampleRadius();

	double angle = getTailAngle();
	double tail_length = getTailLengthEulerDist();

	//for basket
	if (angle > bad_bridge_angle || tail_length > follow_dist) {
		return false;
	}
	if (angle < save_bridge_angle || (tail_length < save_bridge_dist&&angle < bad_bridge_angle))
	{
		return true;
	}
	else {
		return false;
	}
}

bool Branch::isBridgeTailHealthy()
{
	return false;
}

void Branch::rememberBridgeHead()
{
	if (!isBridgeHeadHealthy()) {
		return;
	}
	Vec3 old_bridge_head = back_up_head;
	if (old_bridge_head[0] < -5) {
		back_up_head = curve[0].pos;
		return;
	}
	Vec3 new_bridge_head = curve[0].pos;
	Vec3 real_head = curve[1].pos;
	double old_length = old_bridge_head.cptEuclideanDistance(real_head);
	double new_length = new_bridge_head.cptEuclideanDistance(real_head);

	if (new_length > old_length) {
		back_up_head = curve[0].pos;
	}
}

void Branch::rememberBridgeTail()
{
}

double Branch::getNodeAngle(int idx)
{
	double angle = -1;
	if (idx < 0 || idx >= curve.size()) {
		return angle;
	}
	Vec3 dir0, dir1;
	if (idx == 0) {
		dir0 = curve[0].pos - curve[1].pos;
		dir1 = curve[1].pos - curve[2].pos;
	}
	else if (idx == curve.size() - 1) {
		dir0 = curve[curve.size() - 1].pos - curve[curve.size() - 2].pos;
		dir1 = curve[curve.size() - 2].pos - curve[curve.size() - 3].pos;
	}
	else {
		dir0 = curve[idx].pos - curve[idx - 1].pos;
		dir1 = curve[idx + 1].pos - curve[idx].pos;
	}
	angle = GlobalFun::computeRealAngleofTwoVector(dir0, dir1);
	return angle;
}

void Branch::inactiveAndKeepBridgeTail()
{
	if (!isTailBridge()) {
		return;
	}
	SVertex &tail = curve[curve.size() - 1];
	tail.is_skel_bridge = false;
}

void Branch::inactiveAndDropBridgeTail()
{
	if (!isTailBridge()) {
		return;
	}
	SVertex &tail = curve[curve.size() - 1];
	tail.is_skel_bridge = false;

	if(back_up_tail[0] < -5) {
		tail.pos = back_up_tail;
		if (getTailAngle() > 100) {
			curve.pop_back();
		}
	}
	else {
		curve.pop_back();
	}
}

void Branch::inactiveAndKeepBridgeHead()
{
	if (!isHeadBridge()) {
		return;
	}
	SVertex &head = curve[0];
	head.is_skel_bridge = false;
}

void Branch::inactiveAndDropBridgeHead()
{
	if (!isHeadBridge()) {
		return;
	}
	SVertex &head = curve[0];
	head.is_skel_bridge = false;
	if (back_up_head[0] > -5) {
		head.pos = back_up_head;
		if (getHeadAngle() > 100) {
			curve.erase(curve.begin());
		}
	}
	else {
		curve.erase(curve.begin());
	}
}

void Branch::reverseBranch()
{
	std::reverse(curve.begin(), curve.end());
	Vec3 temp = back_up_head;
	back_up_head = back_up_tail;
	back_up_tail = temp;
}

double Branch::getNodeLengthEulerDist(int idx)
{
	double length = -1;
	if (idx < 0 || idx >= curve.size()) {
		return length;
	}
	if (idx < curve.size() - 1) {
		length = curve[idx].pos.cptEuclideanDistance(curve[idx + 1].pos);
	}
	else if (idx == curve.size() - 1) {
		length = curve[curve.size() - 1].pos.cptEuclideanDistance(curve[curve.size() - 2].pos);
	}
	return 0.0;
}

double Branch::getNodeLengthEulerDistSquare(int idx)
{
	return 0.0;
}

double Branch::getHeadLengthEulerDist()
{
	return getNodeLengthEulerDist(0);
}

double Branch::getHeadLengthEulerDistSquare()
{
	return getNodeLengthEulerDistSquare(0);
}

double Branch::getTailLengthEulerDist()
{
	return getNodeLengthEulerDist(curve.size());
}

double Branch::getTailLengthEulerDistSquare()
{
	return getNodeLengthEulerDistSquare(curve.size()-1);
}
