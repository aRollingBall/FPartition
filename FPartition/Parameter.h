#pragma once
#include <qlist.h>
#include <limits>
using namespace std;
class ParameterSet
{
public:
	ParameterSet();
	ParameterSet(const ParameterSet &ps);
	double getStopAndGrowError() { return stop_and_grow_error; }
	double getMaxIterateTime() { return max_iterate_time; }
	double getMaxStopRadius() { return max_stop_radius; }
	double getGridRadius() { return grid_radius; }
	double getAcceptBranchSize() { return accept_branch_size; }
	double getFollowSampleRadius() { return follow_sample_radius; }
	double getCombineTooCloseThreshold() { return combine_too_close_threshold; }
	double getFollowSampleMaxAngle() { return follow_sample_max_angle; }
	double getBranchesMergeMaxDist() { return branches_merge_max_dist; }
	double getDoubleMaxNum() { return (numeric_limits<double>::max)(); }
	double getSaveBridgeAngle() { return save_bridge_angle; }
	double getBadBridgeAngle() { return bad_bridge_angle; }
	double getGrowSearchRadius() { return grow_search_radius; }
	double getGrowAcceptSigma() { return grow_accept_sigma; }
	double getBridgeHeadAcceptAngle() { return bridge_head_accept_angle; }
	double getCombineSimilarAngle() { return combine_similar_angle; }
	double getCleanNearBranchesDist() { return clean_near_branches_dist; }
	double getRepulsionMu() { return repulsion_mu; }
	double getRepulsionMu2() { return repulsion_mu2; }
	double getAveragePower() { return average_power; }
	double getFixOriginalWeight() { return fix_original_weight; }
	double getHGaussianPara() { return h_gaussian_para; }
	double getRepultionPower() { return repulsion_power; }
	bool getUseBridgeGroupMergeStrategy() { return use_bridge_group_merge_strategy; }
	bool getUseCleanPointsWhenFollowingStrategy() { return use_clean_points_when_following_strategy; }
	bool getNeedComputeDensity() { return need_compute_density; }
	bool getRunAnisotropicLOP() { return run_anisotropic_LOP; }
	double setInitRadius(double para) { init_radius = para; }
	double setCurrentMovementError(double para) { current_movement_error=para; }
	bool isEmpty() const;
public:
	QList<ParameterSet*> paramList;
private:
	double stop_and_grow_error;
	double max_iterate_time;
	double max_stop_radius;
	double grid_radius;
	double accept_branch_size;
	double init_radius;
	double current_movement_error;
	double follow_sample_radius;
	double combine_too_close_threshold;
	double follow_sample_max_angle;
	double branches_merge_max_dist;
	double double_max_num;
	double save_bridge_angle;
	double bad_bridge_angle;
	double grow_search_radius;
	double grow_accept_sigma;
	double bridge_head_accept_angle;
	double combine_similar_angle;
	double clean_near_branches_dist;
	double repulsion_mu;
	double repulsion_mu2;
	double average_power;
	double fix_original_weight;
	double h_gaussian_para;
	double repulsion_power;
	bool use_bridge_group_merge_strategy;
	bool use_clean_points_when_following_strategy;
	bool need_compute_density;
	bool run_anisotropic_LOP;
};