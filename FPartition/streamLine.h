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
	//todo���������߽ṹ��
	std::set<int> slt;
	int objSSL;	//���ڴ洢�򻯶���
	bool isN;	//��¼�����Ƿ�������ֹ	
	//todo����ʾ���ߵ���������ı�����������
	int typeSL;
	QuaBoundaryType qbt;
	int selSVI;	//���ڱ߽��ݶ���������	//-1:�����Ⱦ����ɣ�-2:�ָ��Ⱦ����ɣ�>=0:�ֶ����÷ֲ�
	double selSLSegR;	//���߻��ֱ�������Χ0-1��plan->��ֵ 0���Ȳ1���Ⱦ�
	int bpc;	//�߽����������Ա߽磩
	int nearFaceI[2];
	bool isSelect;

	int slpc;	//startSLPointIndex
	int aslpc;	//auxStartSLPI
	bool isShow;

	//todo���ж�ͼ���Ƿ��ɴ��ı������
	bool isf;
	int startSingularIndex;
	int endSingularIndex;
	int startSLPointIndex;
	int endSLPointIndex;
	int auxStartSLPI;
	int auxEndSLPI;
	SLQBT auxQBT;
	bool isBorder;
	bool dir;	//��߽���أ�false����ʾesI->ssI��true����ʾssI->esI
	double distanceSL;
	std::vector<int> interV;	//�洢����ʽ�Ǿ�������ʸ������Խ����ô��Խ��ǰ	
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