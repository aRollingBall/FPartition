#pragma once
#include<vector>
#include<set>
#include"global.h"
struct SLQBT
{
	int auxQBTSLSPI;
	int auxQBTSLEPI;
	int slpSI;
	int slpEI;
	SLQBT() { auxQBTSLEPI = 0; auxQBTSLSPI = 0; slpEI = 0; slpSI = 0; }
	void init(int auxs, int auxe, int sI, int eI) { auxQBTSLSPI = auxs; auxQBTSLEPI = auxe; slpSI = sI; slpEI = eI; }
};
class StreamLine
{
public:
	//todo：用于流线结构简化
	std::set<int> slt;
	int objSSL;	//用于存储简化对象
	bool isN;	//记录流线是否意外终止	
	//todo：表示流线的类别，用于四边形网格生成
	int typeSL;
	QuaBoundaryType qbt;
	int selSVI;	//用于边界梯度网格生成	//-1:正常等距生成；-2:恢复等距生成；>=0:手动设置分布
	double selSLSegR;	//流线划分比例，范围0-1，plan->极值 0：等差，1：等距
	int bpc;	//边界段数（仅针对边界）
	int nearFaceI[2];
	bool isSelect;

	int slpc;	//startSLPointIndex
	int aslpc;	//auxStartSLPI
	bool isShow;

	//todo：判断图形是否由粗四边形组成
	bool isf;
	int startSingularIndex;
	int endSingularIndex;
	int startSLPointIndex;
	int endSLPointIndex;
	int auxStartSLPI;
	int auxEndSLPI;
	SLQBT auxQBT;
	bool isBorder;
	bool dir;	//与边界相关，false：表示esI->ssI，true：表示ssI->esI
	double distanceSL;
	std::vector<int> interV;	//存储的形式是距离奇点的矢量距离越近那么就越排前	
	StreamLine(int startSI, int endSI, int startSLPI, int endSLPI, bool isB, double disSL, bool dr, bool isN);
	bool refreshEndInfo(int newEndSingularIndex, int newEndSLPointIndex);
	bool refreshStartInfo(int newStartSingularIndex, int newStartSLPointIndex);
	bool refreshSLPInfo(int startSLPI, int endSLPI);
	bool refreshDisSLInfo(double disSL);
	bool regreshReduceObjSLI(int objSLi);
	bool setNearFaceI(int faceI);
	void getSLDir(int svI, int& pI1, int& pI2);
	~StreamLine() {};
};