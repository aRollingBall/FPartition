#pragma once
#include "baseType.h"
#include "StreamLinePointCloud.h"
#include "Eigen\Dense"
namespace GlobalFun {
	double computeEulerDist(Vec3 &p1, Vec3 &p2);
	double computeEulerDistSquare(Vec3 &p1, Vec3 &p2);
	double computeRealAngleofTwoVector(Vec3 v0, Vec3 v1);
	double computeProjDist(Vec3 &p1, Vec3 &p2, Vec3 &normal_of_p1);
	void computeEigenIgnoreBranchedPoints(SMesh *_samples);
	void computeEigen(SMesh *_samples);
	bool isTwoPointTheSame(Vec3 &v0, Vec3 &v1);
	bool isTwoPointOpposite(Vec3 &v0, Vec3 &v1);
}