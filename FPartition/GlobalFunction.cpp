#include "GlobalFunction.h"
#include <iostream>
#include <math.h>
using namespace std;
double GlobalFun::computeEulerDist(Vec3 & p1, Vec3 & p2)
{
	double dist2;
	return 0.0;
}

double GlobalFun::computeEulerDistSquare(Vec3 & p1, Vec3 & p2)
{
	return 0.0;
}

double GlobalFun::computeRealAngleofTwoVector(Vec3 v0, Vec3 v1)
{
	v0=v0.normalized();
	v1 = v1.normalized();
	double angle_cos = v0*v1;
	if (isTwoPointTheSame(v0, v1)) {
		return 0;
	}
	if (isTwoPointOpposite(v0, v1)) {
		return 180;
	}
	if (angle_cos > 1) {
		angle_cos = 0.99;
	}
	if (angle_cos < -1) {
		angle_cos = -0.99;
	}
	if (angle_cos > 0 && angle_cos < 1e-8) {
		return 90;
	}
	double angle = acos(angle_cos)*180.0 / 3.1415926;
	if (angle < 0 || angle>180) {
		cout << "compute angle wrong" << endl;
		return -1;
	}
	return angle;
}

double GlobalFun::computeProjDist(Vec3 & p1, Vec3 & p2, Vec3 & normal_of_p1)
{
	Vec3 v = (p1 - p2)*normal_of_p1;
	return v.norm();
}

void GlobalFun::computeEigenIgnoreBranchedPoints(SMesh * _samples)
{
	vector<vector<int>> neighborMap;
	SMesh::VertexIterator begin = _samples->vert.begin();
	SMesh::VertexIterator end = _samples->vert.end();

	neighborMap.assign(end - begin, vector<int>());

	int curr_index = 0;
	for (SMesh::VertexIterator iter = begin; iter != end; ++iter, curr_index++) {
		if (iter->neighbors.size() <= 3) {
			iter->eigen_confidence = 0.5;
			continue;
		}
		for (int j = 0; j < iter->neighbors.size(); j++) {
			SVertex& t = _samples->vert[iter->neighbors[j]];
			if (t.is_skel_branch || t.is_skel_ignore) {
				continue;
			}
			neighborMap[curr_index].push_back(iter->neighbors[j]);
		}
	}
	curr_index = 0;
	for (SMesh::VertexIterator iter = begin; iter != end; iter++, curr_index++) {
		int neighbor_size = neighborMap[curr_index].size();

		if (neighbor_size < 3) {
			iter->eigen_confidence = 0.95;
			iter->eigen_vector0 = Vec3(0, 0, 0);
			continue;
		}
		Eigen::Matrix3d covariance_matrix;
		Vec3 diff;
		covariance_matrix.setZero();
		int neighborIndex = -1;

		for (int n = 0; n < neighbor_size; n++) {
			neighborIndex = neighborMap[curr_index][n];
			if (neighborIndex < 0)
				break;
			SMesh::VertexIterator neighborIter = begin + neighborIndex;
			diff = iter->pos - neighborIter->pos;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					covariance_matrix(i, j) += diff[i] * diff[j];
				}
			}
		}
		//Vec3 eigenvalues;
		
		//int required_rotations;
		Eigen::EigenSolver<Eigen::MatrixXd> es(covariance_matrix);

		Eigen::Matrix3cd evals = es.eigenvalues();
		Eigen::Vector3d evalsReal=evals.real();
		Eigen::Matrix3d eigenvectors = es.eigenvectors().real();
		int max_col_index, max_row_index,min_col_index,min_row_index;
		//cout <<  << endl;
		evalsReal.maxCoeff(&max_row_index, &max_col_index);
		evalsReal.minCoeff(&min_row_index, &min_col_index);
		double sum_eigen_value = (evalsReal(0) + evalsReal(1) + evalsReal(2));
		iter->eigen_confidence = evalsReal(max_row_index)/sum_eigen_value;
		for (int d = 0; d < 3; d++)
			iter->eigen_vector0[d] = eigenvectors(max_row_index, d);
		for(int d = 0; d < 3; d++)
			iter->eigen_vector1[d] = eigenvectors(3- max_row_index- min_row_index, d);
		for (int d = 0; d < 3; d++)
			iter->N[d]= eigenvectors(min_row_index, d);

		iter->eigen_vector0=iter->eigen_vector0.normalized();
		iter->eigen_vector1 = iter->eigen_vector1.normalized();
		iter->N = iter->N.normalized();
	}

}

void GlobalFun::computeEigen(SMesh * _samples)
{
	vector<vector<int>> neighborMap;
	SMesh::VertexIterator begin = _samples->vert.begin();
	SMesh::VertexIterator end = _samples->vert.end();

	neighborMap.assign(end - begin, vector<int>());

	int curr_index = 0;
	
	for (SMesh::VertexIterator iter = begin; iter != end; iter++, curr_index++) {
		int neighbor_size = neighborMap[curr_index].size();

		if (neighbor_size < 3) {
			iter->eigen_confidence = 0.95;
			iter->eigen_vector0 = Vec3(0, 0, 0);
			continue;
		}
		Eigen::Matrix3d covariance_matrix;
		Vec3 diff;
		covariance_matrix.setZero();
		int neighborIndex = -1;

		for (int n = 0; n < neighbor_size; n++) {
			neighborIndex = neighborMap[curr_index][n];
			if (neighborIndex < 0)
				break;
			SMesh::VertexIterator neighborIter = begin + neighborIndex;
			diff = iter->pos - neighborIter->pos;
			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					covariance_matrix(i, j) += diff[i] * diff[j];
				}
			}
		}
		//Vec3 eigenvalues;

		//int required_rotations;
		Eigen::EigenSolver<Eigen::MatrixXd> es(covariance_matrix);

		Eigen::Matrix3cd evals = es.eigenvalues();
		Eigen::Vector3d evalsReal = evals.real();
		Eigen::Matrix3d eigenvectors = es.eigenvectors().real();
		int max_col_index, max_row_index, min_col_index, min_row_index;
		//cout <<  << endl;
		evalsReal.maxCoeff(&max_row_index, &max_col_index);
		evalsReal.minCoeff(&min_row_index, &min_col_index);
		double sum_eigen_value = (evalsReal(0) + evalsReal(1) + evalsReal(2));
		iter->eigen_confidence = evalsReal(max_row_index) / sum_eigen_value;
		for (int d = 0; d < 3; d++)
			iter->eigen_vector0[d] = eigenvectors(max_row_index, d);
		for (int d = 0; d < 3; d++)
			iter->eigen_vector1[d] = eigenvectors(3 - max_row_index - min_row_index, d);
		for (int d = 0; d < 3; d++)
			iter->N[d] = eigenvectors(min_row_index, d);

		iter->eigen_vector0 = iter->eigen_vector0.normalized();
		iter->eigen_vector1 = iter->eigen_vector1.normalized();
		iter->N = iter->N.normalized();
	}
}

bool GlobalFun::isTwoPointTheSame(Vec3 & v0, Vec3 & v1)
{
	if (abs(v0[0] - v1[0]) < 1e-7 && abs(v0[1] - v1[1]) < 1e-7 && abs(v0[2] - v1[2]) < 1e-7) {
		return true;
	}
	return false;
}

bool GlobalFun::isTwoPointOpposite(Vec3 & v0, Vec3 & v1)
{
	if (abs(-v0[0] - v1[0]) < 1e-7 && abs(-v0[1] - v1[1]) < 1e-7 && abs(-v0[2] - v1[2]) < 1e-7) {
		return true;
	}
	return false;
}
