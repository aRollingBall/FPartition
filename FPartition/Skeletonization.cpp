#include "Skeletonization.h"
#include <iostream>
#include "GlobalFunction.h"
#include <assert.h>
void Skeletonization::run()
{
	is_skeleton_locked = false;
	runAutoWlopOneStep();
	cout << "***iter num****" << nTimeIterated << endl;
}

void Skeletonization::runAutoWlopOneStep()
{
	runStep0_WLOPIterationAndBranchGrowing();
	if (iterate_error<para->getStopAndGrowError() ||
		iterate_time_in_one_stage>para->getMaxIterateTime()) {
		cout << "**increase radius begin**" << endl;
		runStep1_DetectFeaturePoints();
		runStep2_SearchNewBranches();
		runStep3_UpdateRadius();

		//test if the whole process should stop
		int left_moving_num = getMovingPointsNum();
		double stop_radius = para->getMaxStopRadius();
		double current_radius = para->getGridRadius();
		if (left_moving_num <= para->getAcceptBranchSize() ||
			current_radius > stop_radius) {
			finalProcess();
		}
		cout << "**increase radius stop**" << endl;
		iterate_time_in_one_stage = 0;
	}
	

}

void Skeletonization::runStep0_WLOPIterationAndBranchGrowing()
{
	if (nTimeIterated == 0) {
		double init_radius = para->getGridRadius();
		para->setInitRadius(init_radius);
		iterate_time_in_one_stage = 0;
	}
	updateAllBrachesFollowSamples();
	growAllBranches();
	dealWithBridgesForAllBranch();

	if (para->getUseCleanPointsWhenFollowingStrategy()) {
		cleanPointsNearBranches();
	}

	iterate_error = wlopIterate();
	para->setCurrentMovementError(iterate_error);

	iterate_time_in_one_stage++;
	nTimeIterated++;
}

void Skeletonization::runStep1_DetectFeaturePoints()
{
	cout << "step1" << endl;
	removeTooClosePoints();
	eigenConfidenceSmoothing();
}

void Skeletonization::runStep2_SearchNewBranches()
{
	cout << "step2" << endl;
	searchNewBranches();
	growAllBranches();
}

void Skeletonization::runStep3_UpdateRadius()
{
	cout << "step3" << endl;
	mergeNearEndsGroup();
	cleanPointsNearBranches();
	labelFixOriginal();
	rememberBridgeEnds();
	increaseRadius();
}

void Skeletonization::updateBridgeHeadFollowSamples(Branch & branch)
{
	double follow_dist = para->getFollowSampleRadius();
	double follow_dist2 = follow_dist*follow_dist;

	double too_close_dist = para->getCombineTooCloseThreshold();
	double too_close_dist2 = too_close_dist*too_close_dist;

	double follow_angle = para->getFollowSampleMaxAngle();
	double merge_dist = para->getBranchesMergeMaxDist();
	/*bool use_perpend_dist_strategy =
		para->getBool("Use Plus Perpendicular Dist Strategy");
	bool use_kill_too_close_strategy =
		para->getBool("Use Kill Too Close Strategy");*/
	Curve &curve = branch.curve;
	SVertex &head = curve[0];
	SVertex &v = samples->vert[head.index];
	v.setSample_MovingAndBridge();

	vector<int> follow_cadidates;
	follow_cadidates.push_back(head.index);

	int near_by_bridge_num = 0;
	for (int j = 0; j < v.neighboors.size(); j++) {
		SVertex &t = samples->vert[v.neighboors[j]];
		if (t.isSample_Moving()) {
			follow_cadidates.push_back(v.neighboors[j]);
		}
	}
	if (follow_cadidates.size() < 2) {
		return;
	}

	Vec3 real_head_direction = branch.getTrueHeadDirection();
	Vec3 real_head_P = curve[1].pos;
	Vec3 bridge_head_P = curve[0].pos;
	int min_idx = -1;
	double min_dist = para->getDoubleMaxNum();

	//if (use_perpend_dist_strategy) {}else{}
	for (int i = 0; i < follow_cadidates.size(); i++) {
		SVertex &t = samples->vert[follow_cadidates[i]];

		double dist=real_head_P.cptEuclideanDistance(t.pos);
		double dist2 = dist*dist;
		if (dist2<follow_dist2&&dist2>too_close_dist2) {
			Vec3 bridge_direction = (t.pos - real_head_P).normalized();
			double angle = GlobalFun::computeRealAngleofTwoVector(real_head_direction, bridge_direction);

			if (angle > 0 && angle < follow_angle&&dist2 < min_dist) {
				min_dist = dist2;
				min_idx = follow_cadidates[i];
			}
		}
	}
	if (min_idx >= 0 && (head.index != min_idx)) {
		samples->vert[head.index].setSample_JustMoving();
		head.index = min_idx;
		samples->vert[head.index].setSample_MovingAndBridge();
		head.pos = samples->vert[head.index].pos;

		branch.rememberBridgeHead();
	}
	else {
		branch.curve[0].pos = samples->vert[head.index].pos;
	}
}

void Skeletonization::updateOneBranchFollowSamples(Branch & branch)
{
	if (branch.isHeadBridge()) {
		updateBridgeHeadFollowSamples(branch);
	}
	if (branch.isTailBridge()) {
		branch.reverseBranch();
		updateBridgeHeadFollowSamples(branch);
	}
}

void Skeletonization::updateAllBrachesFollowSamples()
{
	for (int i = 0; i < skeleton->branches.size(); ++i) {
		Branch &branch = skeleton->branches[i];
		if (branch.curve.size() <= 3) {
			continue;
		}

		updateOneBranchFollowSamples(branch);
	}
}

void Skeletonization::dealWithBridgesForTail(Branch & branch)
{
	Curve &curve0 = branch.curve;
	double max_merge_dist = para->getBranchesMergeMaxDist();
	double max_merge_dist2 = max_merge_dist*max_merge_dist;

	bool use_bridge_group_strategy = para->getUseBridgeGroupMergeStrategy();

	//deal with head eat tail problem
	if (branch.isHeadBridge()) {
		auto temp_gethead = branch.getHead();
		auto temp_gettail = branch.getTail();
		double head_tail_dist = temp_gethead.cptEuclideanDistance(temp_gettail);
		double head_tail_dist2 = head_tail_dist*head_tail_dist;
		if (head_tail_dist2 < max_merge_dist/*???*/) {
			double head_angle = branch.getHeadAngle();
			double tail_angle = branch.getTailAngle();
			if(head_angle > tail_angle) {
				branch.inactiveAndDropBridgeHead();
			}
			else {
				branch.inactiveAndDropBridgeTail();
				return;
			}
		}
	}
	//try to combine two nearby bridge
	if (isBridgeTailHealthy(branch)) {
		Vec3 tail_P = curve0[curve0.size() - 1].pos;
		vector<int> branch_ids;
		vector<Branch> &branches = skeleton->branches;
		for (int i = 0; i < branches.size(); i++) {
			Curve &curve1 = branches[i].curve;

			if (isTwoCurvesTheSame(curve0, curve1)) {
				continue;
			}

			double dist_head = curve1[0].pos.cptEuclideanDistance(tail_P);
			double dist_head2 = dist_head*dist_head;
			double dist_tail = curve1[curve1.size() - 1].pos.cptEuclideanDistance(tail_P);
			double dist_tail2 = dist_tail*dist_tail;

			if (dist_head2 < max_merge_dist2 || dist_tail2 < max_merge_dist2) {
				if (!(dist_head2 < max_merge_dist2
					&& dist_tail2 < max_merge_dist2)) {
					branch_ids.push_back(i);
				}
			}
		}

		if (!branch_ids.empty()) {
			is_skeleton_locked = true;
			mergeNearEndsGroupFromP(tail_P);
			is_skeleton_locked = false;
		}
	}
}

void Skeletonization::dealWithBridgesForOneBranch(Branch & branch)
{
	if (branch.isTailBridge()) {
		dealWithBridgesForTail(branch);
	}
	if (branch.isHeadBridge()) {
		branch.reverseBranch();
		dealWithBridgesForTail(branch);
	}

}

void Skeletonization::dealWithBridgesForAllBranch()
{
	for (int i = 0; i < skeleton->branches.size(); i++) {
		Branch &branch = skeleton->branches[i];
		if (branch.curve.size() <= 3) {
			continue;
		}
		dealWithBridgesForOneBranch(branch);
	}
}

bool Skeletonization::isBridgeTailHealthy(Branch & branch)
{
	return false;
}

void Skeletonization::removeTooClosePoints()
{
	double near_threshold = para->getDouble("Combine Too Close Threshold");
	double near_threshold2 = near_threshold * near_threshold;

	for (int i = 0; i < samples->vn; i++) {
		CVertex &v = samples->vert[i];
		for (int j = 0; j < v.neighbors.size(); j++) {
			CVertex &t = samples->vert[v.neighbors[j]];

			if (!t.isSample_JustMoving()) {
				continue;
			}

			double dist2 = GlobalFun::computeEulerDistSquare(v.P(), t.P());
			if (dist2 < near_threshold2) {
				t.remove();
			}
		}
	}
}

void Skeletonization::eigenThresholdIdentification()
{
	int sigma_KNN = para->getDouble("Sigma KNN");

	GlobalFun::computeAnnNeigbhors(
		samples->vert, samples->vert, sigma_KNN, false,
		"void Skeletonization::eigenThresholdClassification()");

	if (para->getBool("Use Compute Eigen Ignore Branch Strategy")) {
		GlobalFun::computeEigenIgnoreBranchedPoints(samples);
	}
	else {
		GlobalFun::computeEigen(samples);
	}

	eigenConfidenceSmoothing();

	for (int i = 0; i < samples->vert.size(); i++) {
		SVertex &v = samples->vert[i];

		if (!v.isSample_Moving()) {
			continue;
		}

		double eigen_psi = v.eigen_confidence;
		double eigen_threshold =
			para->getDouble("Eigen Feature Identification Threshold");

		if (eigen_psi > eigen_threshold) {
			v.is_fixed_sample = true;
		}
		else {
			v.is_fixed_sample = false;
		}
	}
}

void Skeletonization::eigenConfidenceSmoothing()
{
	for (int i = 0; i < samples->vert.size(); i++) {
		SVertex &v = samples->vert[i];
		v.eigen_confidence = 1 - v.eigen_confidence;
	}

	for (int i = 0; i < samples->vert.size(); i++) {
		SVertex &v = samples->vert[i];
		double sum = v.eigen_confidence;
		for (int j = 0; j < v.neighbors.size(); j++) {
			sum += samples->vert[v.neighbors[j]].eigen_confidence;
		}
		v.eigen_confidence = sum / (v.neighbors.size() + 1);
	}

	for (int i = 0; i < samples->vert.size(); i++) {
		SVertex &v = samples->vert[i];
		v.eigen_confidence = 1 - v.eigen_confidence;

		if (v.eigen_confidence < 0) {
			v.eigen_confidence = 0.5;
		}
	}
}

void Skeletonization::searchNewBranches()
{
}

Branch Skeletonization::searchOneBranchFromIndex(int begin_idx)
{
	return Branch();
}

Branch Skeletonization::searchOneBranchFromDirection(int begin_idx, Vec3 head_direction)
{
	return Branch();
}

void Skeletonization::growBridgeTailUntilStop(Branch & branch)
{
	double follow_dist = para->getGrowSearchRadius();
	double follow_dist2 = follow_dist*follow_dist;

	double too_close_dist = para->getCombineTooCloseThreshold();
	double too_close_dist2 = too_close_dist*too_close_dist;

	double grow_accept_sigma = para->getGrowAcceptSigma();
	double angle_threshold = para->getBridgeHeadAcceptAngle();
	double save_bridge_angle = para->getSaveBridgeAngle();

	bool is_tail_growing = true;
	bool is_ignore_somthing = false;

	Curve &curve = branch.curve;
	while (is_tail_growing) {
		if (curve.size() < 3 || !branch.isTailBridge()) {
			return;
		}
		SVertex tail = curve[curve.size() - 1];
		SVertex &v = samples->vert[tail.index];
		if (v.neighboors.empty()) {
			cout << "empty neighboor" << endl;
			return;
		}
		double min_dist = para->getDoubleMaxNum();
		int min_idx = -1;
		int near_moving_count = 0;

		Vec3 tail_direction = branch.getBridgeTailDirection();
		double candidate_sigma = 0;
		for (int j = 0; j < v.neighboors.size(); j++) {
			SVertex &t = samples->vert[v.neighboors[j]];
			if (!t.is_skel_ignore) {
				double dist = v.pos.cptEuclideanDistance(t.pos);
				double dist2 = dist*dist;
				if (dist2 < too_close_dist2 && !t.is_skel_bridge) {
					t.remove();
					continue;
				}
				if (dist2 < follow_dist2) {
					if (t.isSample_Moving()) {
						near_moving_count++;
					}
					double proj_dist = GlobalFun::computeProjDist(tail.pos, t.pos, tail_direction);
					if (proj_dist < 0) return;
					if (dist2 < min_dist) {
						min_dist = dist2;
						min_idx = v.neighboors[j];
candidate_sigma = t.eigen_confidence;
					}
				}
			}
		}
	}
}

void Skeletonization::growOneBranchByBridge(Branch & branch)
{
	if (branch.getSize() < 2) return;
	if (branch.isTailBridge()) {
		growBridgeTailUntilStop(branch);
	}
	if (branch.isHeadBridge()) {
		branch.reverseBranch();
		growBridgeTailUntilStop(branch);
	}
}

void Skeletonization::growAllBranches()
{
	for (int i = 0; i < skeleton->branches.size(); i++) {
		growOneBranchByBridge(skeleton->branches[i]);
	}
}

bool Skeletonization::mergeNearEndsGroupFromP(Vec3 p0)
{
	double max_merge_dist = para->getBranchesMergeMaxDist();
	double max_merge_dist2 = max_merge_dist*max_merge_dist;

	vector<RecordItem> group;

	bool meet_short_branch = false;

	Vec3 average_P = Vec3(0, 0, 0);
	vector<Vec3> dangerous_Pts;
	vector<Vec3> nearby_Pts;
	for (int i = 0; i < skeleton->branches.size(); ++i) {
		Curve &curve = skeleton->branches[i].curve;
		double dist_head = curve[0].pos.cptEuclideanDistance(p0);
		double dist_head2 = dist_head*dist_head;
		double dist_tail = curve[curve.size() - 1].pos.cptEuclideanDistance(p0);
		double dist_tail2 = dist_tail*dist_tail;

		if (dist_head2 < max_merge_dist2&&dist_tail2 < max_merge_dist2) {
			meet_short_branch = true;
			if (dist_head2 < dist_tail2) {
				group.push_back(RecordItem(i, 0));
				average_P += curve[0].pos;
				nearby_Pts.push_back(curve[0].pos);
				dangerous_Pts.push_back(curve[curve.size() - 1].pos);

			}
			else {
				group.push_back(RecordItem(i, curve.size() - 1));
				average_P += curve[curve.size() - 1].pos;
				nearby_Pts.push_back(curve[curve.size() - 1].pos);
				dangerous_Pts.push_back(curve[0].pos);
			}
		}
		else if (dist_head2 < max_merge_dist2) {
			group.push_back(RecordItem(i, 0));
			average_P += curve[0].pos;
			nearby_Pts.push_back(curve[0].pos);
		}
		else if (dist_tail2 < max_merge_dist2) {
			group.push_back(RecordItem(i, curve.size() - 1));
			average_P += curve[curve.size() - 1].pos;
			nearby_Pts.push_back(curve[curve.size() - 1].pos);
		}
	}
	average_P = average_P / group.size();

	assert(nearby_Pts.size() == group.size());
	if (!dangerous_Pts.empty()) {
		vector<int> exclued_ids;
		for (int i = 0; i < nearby_Pts.size(); i++) {
			Vec3 p = nearby_Pts[i];
			double dist_to_avg = average_P.cptEuclideanDistance(p);
			dist_to_avg = dist_to_avg*dist_to_avg;
			bool need_to_exclued = false;
			for (int j = 0; j < dangerous_Pts.size(); j++) {
				double dist_to_dang = dangerous_Pts[j].cptEuclideanDistance(p);
				dist_to_dang = dist_to_dang*dist_to_dang;
				if (dist_to_dang < dist_to_avg) {
					need_to_exclued = true;
					break;
				}
			}
			if (need_to_exclued) {
				exclued_ids.push_back(i);
			}
		}
		if (!exclued_ids.empty()) {
			vector<RecordItem> temp_group;
			vector<Vec3> temp_nearby_pts;
			Vec3 temp_average = Vec3(0, 0, 0);
			for (int i = 0; i < group.size(); i++) {
				if (std::find(exclued_ids.begin(), exclued_ids.end(), i) == exclued_ids.end()){
					temp_group.push_back(group[i]);
					temp_nearby_pts.push_back(nearby_Pts[i]);
					temp_average += nearby_Pts[i];
				}
			}
			average_P = temp_average / temp_nearby_pts.size();
			group = temp_group;
		}
	}

	if (group.size() >= 3) {
		for (int i = 0; i < group.size(); i++) {
			RecordItem &item = group[i];
			Branch &branch = skeleton->branches[group[i].branch_i];
			if (item.node_j == 0) {
				branch.moveHeadToPt(average_P);
			}
			else {
				branch.moveTailToPt(average_P);
			}
		}
		return true;
	}
	else if (group.size() == 2 && !is_skeleton_locked) {
		double close_dist = 1e-6;
		
		RecordItem &item0 = group[0];
		RecordItem &item1 = group[1];

		if (item0.branch_i == item1.branch_i) return false;
		Branch &branch0 = skeleton->branches[item0.branch_i];
		Branch &branch1 = skeleton->branches[item1.branch_i];

		Curve &curve0 = branch0.curve;
		Curve &curve1 = branch1.curve;

		Vec3 end0 = curve0[item0.node_j].pos;
		Vec3 end1 = curve1[item1.node_j].pos;

		double end_dist2 = end0.cptEuclideanDistance(end1);
		end_dist2 = end_dist2*end_dist2;

		CONNECT_TYPE best_c_type;
		double best_angle = -1;
		double min_dist2 = -1;

		compareTwoCurvesEnds(curve0, curve1, min_dist2, best_angle, best_c_type);

		double diff = end_dist2 - min_dist2;
		double diff2 = diff*diff;
		if (diff2 > 1e-6) {
			return false;
		}
		if (min_dist2 > max_merge_dist2)
			return false;
		double angle_threshold = para->getCombineSimilarAngle();
		if (best_angle > angle_threshold) {
			Branch new_branch = mergeTwoBranches(branch0, branch1, best_c_type);
			branch0 = new_branch;
			skeleton->branches.erase(skeleton->branches.begin() + branch1.branch_id);
			skeleton->generateBranchSampleMap();
		}
		else if (end_dist2 < 1e-4) {
			if (item0.node_j == 0) {
				branch0.inactiveAndKeepBridgeHead();
			}
			else {
				branch0.inactiveAndKeepBridgeTail();
			}
			if (item1.node_j == 0) {
				branch1.inactiveAndKeepBridgeHead();
			}
			else {
				branch1.inactiveAndKeepBridgeTail();
			}
		}
	}
	return false;
}

void Skeletonization::mergeNearEndsGroup()
{
}

bool Skeletonization::isPosVisited(vector<Vec3>& visited_pts, Vec3 p, double dist_threshold)
{
	return false;
}

bool Skeletonization::isTwoCurvesTheSame(Curve & c0, Curve & c1)
{
	if(c0.size()!=c1.size())
	    return false;
	if (c0.empty() || c1.empty())
		return false;
	double dist = c0[0].pos.cptEuclideanDistance(c1[0].pos);
	double dist1 = dist*dist;
	if (dist1 > 1e-6) return false;
	dist = c0[c0.size() - 1].pos.cptEuclideanDistance(c1[c1.size() - 1].pos);
	double dist2 = dist*dist;
	if (dist2 > 1e-6) return false;
	return true;
}

void Skeletonization::cleanPointsNearBranches()
{
	double clean_dist = para->getCleanNearBranchesDist();
	double clean_dist2 = clean_dist*clean_dist;
	for (int i = 0; i < samples->vert.size(); i++) {
		SVertex &v = samples->vert[i];
		if (v.is_fixed_sample || v.is_skel_branch) {
			for (int j = 0; j < v.neighboors.size(); j++) {
				SVertex &t = samples->vert[v.neighboors[j]];
				if (t.isSample_JustMoving()) {
					double dist2 = v.pos.cptEuclideanDistance(t.pos);
					dist2 = dist2*dist2;
					if (dist2 < clean_dist2) {
						t.remove();
					}
				}
			}
		}
	}
}

void Skeletonization::initVertexes()
{
	int i = 0;
	SMesh::VertexIterator vi, vi_end;
	vi_end = samples->vert.end();
	for (vi = samples->vert.begin(); vi != vi_end; ++vi) {
		vi->index = i++;
		vi->neighboors.clear();
		if (vi->is_skel_ignore) {
			continue;
		}

	}
	repulsion.assign(samples->vn, Vec3(0, 0, 0));
	average.assign(samples->vn, Vec3(0, 0, 0));
	repulsion_weight_sum.assign(samples->vn, 0);
	average_weight_sum.assign(samples->vn, 0);
}

double Skeletonization::wlopIterate()
{
	initVertexes();
	/*GlobalFun::computeBallNeighbors(
		samples, NULL, para->getDouble("CGrid Radius"), samples->bbox);
	GlobalFun::computeEigenWithTheta(
		samples, para->getDouble("CGrid Radius") /
		sqrt(para->getDouble("H Gaussian Para")));*/
	if (nTimeIterated == 0) {

	}
	/*GlobalFun::computeBallNeighbors(samples, original,
		para->getDouble("CGrid Radius"), box);*/
	computeAverageTerm(samples,original);

	computeRepulsionTerm(samples);
	double min_sigma = para->getDoubleMaxNum();
	double max_sigma = -1;
	for (int i = 0; i < samples->vn; i++) {
		SVertex &v = samples->vert[i];
		if (v.eigen_confidence < min_sigma) {
			min_sigma = v.eigen_confidence;
		}
		if (v.eigen_confidence > max_sigma) {
			max_sigma = v.eigen_confidence;
		}
	}
	double mu_max = para->getRepulsionMu();
	double mu_min = para->getRepulsionMu2();
	double mu_length = abs(mu_max - mu_min);
	double sigma_length = abs(max_sigma - min_sigma);
	Vec3 c;
	int moving_num = 0;
	double max_error = 0;

	for (int i = 0; i < samples->vert.size(); i++) {
		SVertex &v = samples->vert[i];
		if (v.is_fixed_sample || v.is_skel_ignore) { continue; }
		c = v.pos;

		double mu = (mu_length / sigma_length)*(v.eigen_confidence - min_sigma) + mu_min;
		if (average_weight_sum[i] > 1e-20) {
			v.pos = average[i] / average_weight_sum[i];
		}
		if (repulsion_weight_sum[i] > 1e-20 && mu >= 0) {
			v.pos += repulsion[i] * (mu / repulsion_weight_sum[i]);
		}
		if (average_weight_sum[i] > 1e-20 && repulsion_weight_sum[i] > 1e-20) {
			moving_num++;
			Vec3 diff = v.pos - c;
			double move_error = diff.norm();

			error_x += move_error;
		}
	}
	error_x = error_x / moving_num;

	para->setCurrentMovementError(error_x);
	cout << "**finish compute skeletonizaton error:" << error_x << endl;
	return error_x;
}

void Skeletonization::computeAverageTerm(SMesh * samples,SMesh *original)
{
	double average_power = para->getAveragePower();
	bool need_density = para->getNeedComputeDensity();
	double radius = para->getGridRadius();
	double fix_original_weight = para->getFixOriginalWeight();

	double radius2 = radius*radius;
	double iradius16 = -para->getHGaussianPara();

	for (int i = 0; i < samples->vert.size(); i++) {
		SVertex &v = samples->vert[i];
		if (v.is_fixed_sample) {
			average_weight_sum[i] = 0;
			continue;
		}
		for (int j = 0; j < v.original_neighbors.size(); j++) {
			SVertex &t = original->vert[v.original_neighbors[j]];

			Vec3 diff = v.pos - t.pos;
			double dist = diff.norm();
			double dist2 = dist*dist;
			double w = 1;
			if (average_power < 2) {
				double len = dist;
				if (len < 0.001*radius) len = radius*0.001;
				w = exp(dist2*iradius16) / pow(len, 2 - average_power);
			}
			else {
				w = exp(dist2*iradius16);
			}

			if (need_density) {
				w *= original_density[t.index];
			}
			if (t.is_fixed_original_weight) {
				w *= fix_original_weight;
			}
			average[i] += t.pos*w;
			average_weight_sum[i] += w;
		}
	}
}

void Skeletonization::computeRepulsionTerm(SMesh * _samples)
{
	double repulsion_power = para->getRepultionPower();
	double radius = para->getGridRadius();
	double radius2 = radius*radius;
	double iradius16 = para->getHGaussianPara() / radius2;

	for (int i = 0; i < samples->vert.size(); i++) {
		SVertex &v = samples->vert[i];

		if (v.is_fixed_sample || v.is_skel_ignore) {
			repulsion_weight_sum[i] = 0.;
			continue;
		}
		for (int j = 0; j < v.neighboors.size(); j++) {
			SVertex &t = samples->vert[v.neighboors[j]];
			Vec3 diff = v.pos - t.pos;
			double dist = v.pos.cptEuclideanDistance(t.pos);
			double dist2 = dist*dist;
			if (dist <= 0.001*radius) {
				dist = radius*0.001;
			}
			double w = exp(dist2*iradius16);
			double rep = w*pow(1.0 / dist, repulsion_power);
			repulsion[i] += diff*rep;
			repulsion_weight_sum[i] += rep;
		}
	}
}
