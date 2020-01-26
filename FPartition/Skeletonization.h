#pragma once
#include "Skeleton.h"
#include "StreamLinePointCloud.h"
#include "Parameter.h"
class Skeletonization
{
public:
	Skeletonization() {};
	~Skeletonization();

	void run();
	void setInput();
	void clear();

	void setFirstIterate();
	int getIterateNum() { return nTimeIterated; }
	double getErrorX() { return error_x; }

private:
	void runAutoWlopOneStep();
	void runStep0_WLOPIterationAndBranchGrowing();

	void runStep1_DetectFeaturePoints();
	void runStep2_SearchNewBranches();
	void runStep3_UpdateRadius();

	/*for step 0*/
	void updateBridgeHeadFollowSamples(Branch &branch);
	void updateOneBranchFollowSamples(Branch &branch);
	void updateAllBrachesFollowSamples();

	void dealWithBridgesForTail(Branch &branch);
	void dealWithBridgesForOneBranch(Branch &branch);
	void dealWithBridgesForAllBranch();
	bool isBridgeTailHealthy(Branch &branch);

	/*for step 1*/
	void removeTooClosePoints();
	void eigenThresholdIdentification();
	void eigenConfidenceSmoothing();

	/*for step 2*/
	void searchNewBranches();
	Branch searchOneBranchFromIndex(int begin_idx);
	Branch searchOneBranchFromDirection(int begin_idx, Vec3 head_direction);

	void growBridgeTailUntilStop(Branch &branch);
	void growOneBranchByBridge(Branch &branch);
	void growAllBranches();

	/*for step 3*/
	//Merge branches
	bool mergeNearEndsGroupFromP(Vec3 p0);
	void mergeNearEndsGroup();
	bool isPosVisited(vector<Vec3> &visited_pts, Vec3 p, double dist_threshold);

	//connect two branch with similar angle
	enum CONNECT_TYPE{H0_H1,H0_T1,T0_T1,UNKNOWN};
	void compareTwoCurvesEnds(Curve &c0, Curve &c1, double &min_dist, double &angle, CONNECT_TYPE &C_Type);
	double getRealAngleOfTwoCurve(Curve &c0, Curve &c1, CONNECT_TYPE C_Type);
	double getMaxAngleOfTwoPair(vector<Vec3> &dir0, vector<Vec3> &dir1);
	bool isTwoCurvesTheSame(Curve &c0, Curve &c1);

	Branch mergeTwoBranches(Branch &branch0, Branch &branch1, CONNECT_TYPE C_Type = UNKNOWN);
	Curve combineTwoCurvesInOrder(Curve &c0, Curve &c1);
	Curve reverseOneCurve(Curve c0);

	// do some clean up before increase radius
	void cleanPointsNearBranches();
	void labelFixOriginal();
	void rememberBridgeEnds();

	// increase radius
	void increaseRadius();

	// if only a few moving point left,stop the process
	int getMovingPointsNum();

	void finalProcess();

	// reconnect before refinement
	void reconnectSkeleton();

	//segment-refinement
	void runAllSegment();
	void segmentOneCurve(Curve &c);

	double getNodeAngle(Curve &c, int idx);
	void smoothNode(Curve &c, int idx);
	void smoothCurve(Curve &c, double angle_threshold, int max_time);
	void subdivisionNode(Curve &c, int idx);
	void subdivisionCurve(Curve &c, double stop_segment_length);

private:
	void input(SMesh *_samples,SMesh *_original);
	void initVertexes();

	double wlopIterate();
	void computeAverageTerm(SMesh *samples,SMesh *original);
	void computeRepulsionTerm(SMesh *samples);
	void computeDensity(bool isOriginal, double radius);

private:
	ParameterSet *para;
	SMesh *samples;
	SMesh *original;
	Skeleton *skeleton;

	//Box3f box;
	int nTimeIterated;
	double error_x;

	vector<double> samples_density;
	vector<double> original_density;
	vector<Vec3> repulsion;
	vector<double> repulsion_weight_sum;

	vector<Vec3> average;
	vector<double> average_weight_sum;

	bool is_skeleton_locked;

	double iterate_error;
	int iterate_time_in_one_stage;
};