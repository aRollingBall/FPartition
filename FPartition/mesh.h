#pragma once
#include"DisFieldMesh.h"
#include"ThermalField.h"
#include"TriStruct.h"
#include"Singular.h"
#include"SLPoint.h"
#include"streamLine.h"
#include"StreamLineCrossFace.h"
#include"QuaMesh.h"
#include"qstring.h"
//#include"Fit.h"
#pragma region const
const int SVertCount = 100;		//�����������
const int SLCount = 500;		//�����������
const int SLVert = 3000;			//����������������
const int SLFace = 5000;			//������
const Vec3 quaSLColor1 = Vec3(0.0, 1.0, 0.0);
const Vec3 quaSLColor2 = Vec3(0.0, 0.0, 0.0);
const Vec3 quaESLColor = Vec3(1.0, 0.0, 0.0);
const Vec3 quaSSLColor1 = Vec3(1.0, 0.0, 0.0);
const Vec3 quaSSLColor2 = Vec3(0.0, 0.0, 1.0);
const Vec3 colorArraysls[6] = { Vec3(1.0,0.0,0.0),Vec3(0.0,1.0,0.0),Vec3(0.0,0.0,1.0),Vec3(1.0,1.0,0.0),Vec3(0.0,1.0,1.0),Vec3(1.0,0.0,1.0) };
const Vec3 colorArrays1[6] = { Vec3(1.0,0.0,0.0),Vec3(0.0,1.0,0.0),Vec3(0.5,0.5,0.5),Vec3(1.0,1.0,0.0),Vec3(0.0,1.0,1.0),Vec3(1.0,0.0,1.0) };
//const Vec3 colorArrays[6] = { Vec3(0.0,0.0,1.0),Vec3(0.0,0.0,1.0),Vec3(0.0,0.0,1.0),Vec3(0.0,0.0,1.0),Vec3(0.0,0.0,1.0),Vec3(0.0,0.0,1.0) };
//const Vec3 colorArraysls[6] = { Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0),Vec3(0.0,0.0,0.0) };
const Vec3 colorArrayCareQua[6] = { Vec3(1.0,0.0,0.0),Vec3(0.0,1.0,0.0),Vec3(0.0,0.0,1.0),Vec3(1.0,1.0,0.0),Vec3(0.0,1.0,1.0),Vec3(1.0,0.0,1.0) };
const Vec3 qbtColor[5] = { Vec3(1.0,1.0,0.0),Vec3(0.7,0.7,0.0),Vec3(0.0,1.0,0.0),Vec3(0.0,0.7,0.0) ,Vec3(0.0,0.0,0.0) };
const Vec3 slArrayColor[24] = { Vec3(1.0,1.0,0.0),Vec3(0.7,0.7,0.0),Vec3(0.5,0.5,0.0),Vec3(0.3,0.3,0.0),
								Vec3(0.0,1.0,1.0),Vec3(0.0,0.7,0.7),Vec3(0.0,0.5,0.5),Vec3(0.0,0.3,0.3),
								Vec3(1.0,0.0,1.0),Vec3(0.7,0.0,0.7),Vec3(0.5,0.0,0.5),Vec3(0.3,0.0,0.3),
								Vec3(1.0,0.0,0.0),Vec3(0.7,0.0,0.0),Vec3(0.5,0.0,0.0),Vec3(0.3,0.0,0.0),
								 Vec3(0.0,1.0,0.0),Vec3(0.0,0.7,0.0),Vec3(0.0,0.5,0.0),Vec3(0.0,0.3,0.0),
								 Vec3(0.0,0.0,1.0),Vec3(0.0,0.0,0.7),Vec3(0.0,0.0,0.5),Vec3(0.0,0.0,0.3) };
#pragma endregion
enum StreamLineType { v_sl, dis_sl, dif_sl, u_sl };
class Mesh
{
private:
#pragma region private variable
	
	int croLayNum;							//չʾ��ܳ��Ĳ���
	int crolpD;								//�������ݽ��̶ܳ�
	double croBSize;						//��ܳߴ�
	int dirLayNum;							//չʾ���򳡲���
	int dirlpD;								//�������ݽ��̶ܳ�
	double dirHSize;						//��ͷ�ߴ�
	double dirBSize;						//�����ߴ�
	std::map<int, mark_1> BECI;		//����һ���߽�����ĳ���������������������ߺ��������
	std::vector<LineVs> bVEdge;		//�߽���	
	std::vector<BELine> bVSEdge;	//�������ı߽���	
	std::vector<Singular* >auxDirSV;	//���ڸ������
	std::vector<int> dirSLPointCount;
	double llsValue;					//�򻯡����ȡ�
	int selectSLIndex[2];	//��¼��Ҫ�򻯵���������
	int selectSLPI;			//���߼򻯼�¼��
	std::map<std::vector<int>, slType_Secand> slType; //�ı�������
	std::vector<SFace> sf;	//�ı�������	
	std::map<int, std::map<int, std::map<int, pdStr>>> osf;
	//std::vector<SFace> osf;	//�ı�������	
	bool isTAIA;				//��¼����Ԥ��ϸ�ı��������Ƿ�ϸ�

	double maxPointValue;		//���Ȩ��ֵ
	double minPointValue;		//��СȨ��ֵ
	Vec3 maxPos;		//�������������Ϸ���
	Vec3 minPos;		//�������������·���
	double disQuaSize;		//������ι��
	double difQuaSize;		//��־�������
	double maxDis;		//��¼���㵽��Ӧ�����ߵ�������
#pragma endregion
#pragma region private function
	bool fixBAngle(int vertI);		//�޸ġ��յ㡱���ĽǶ�
	double CountTriAngle(int faceI);	//������������Ƭ�������㷽��ʸ��ת��
	bool createSV(Singular* sv);
	void createBESV(int vI);				//�����߽������������		
	//�ı�������ֽ�	
	void remarkPSF(SLPoint* skp, int slI);			//��¼���߾�������
	//void remarkPlf(SLPoint* skp, int slI);
	bool findSLInterPToAuxP(int slI1, int slI2, Vec3 v1, Vec3 v2, Vec3 auxP, Vec3& interP, int lVS[2]);		//�������߽���
	bool dimInterP(Vec3 tempP);		//�ж����߽����Ƿ����
	void GenQuaSL(int ssI, int esI, int spI, int epI, bool isB, bool dr);	//����QuaSLInfo����
	void findBSL(int svI, int vertI, bool dir);		//�����߽�����
	void auxFindBSL(int svI1, int svI2, bool dir, int slspI, int slepI);
	void slsDistance(int slI);				//����ԭʼ���ߵĳ���
	void StreamLineClear();					//���߳�ʼ��
	void setColorSV(int svI, bool is);		//���ó�ʼ������ɫ
	//���߼�
	bool judgeQuaDomainSL(int, int, int, int, Vec3[]);
	void selectClear();
	bool ReMatchFit(int slI1, int slI2, double spec);		//����ƥ��
	int MatchFit(int slI1, int slI2);						//����ֱ��ƥ��
	void reduceOSL(int slI, int& slPI);
	void reduceSL(int slI1, int slI2, int& slPI);
	int JudgeComDirSL(int slI1, int slI2);	//����һ��������������������	
	int getSLIFsvIToslI(int svI, int slI, int dir, Vec3 pd);
	int getPIFsvIToslI(Vec3 pos, int slI);	//���ݸ�����λ�ã��ҵ���Ӧ�����ϵ�λ��
	void getPIFsvIToslI(int slI, int reSLI[]);
	int getSLIFsvv(int svI, Vec3 vec);
	int getPIFsvIToslI(Vec3 pos, int slI, int sp);	//���ϲ�ͬ���Ǹ���Ѱ�����Ľ���λ��
	void auxInterReduceSL(int slI, int& slPI);
	void auxReduceSL(int slI, int& slPI);
	void auxReReduceSL(int slI, int& slPI);
	void auxReduceSLI(int slI, std::set<int>&);
	void auxReduceSLID(std::set<int>&, int);
	bool auxRSLSVLToFSLI(int slI1, int slI2);
	int auxJudgeRSLI(int slI);
	void auxBorReduceSL(int slI, int& slPI);
	//�ı���
	void getSLDir(int slI, int dirValue[]);	//trueΪ���߷���falseΪ���淽��
	bool findOpSLI(int slI, int opSLI[]);
	bool IsBorderSL(int slI);
	bool IsBorderSL(int slsI[], int len);
	void findSLToSV(int sv1, int sv2, int fsIArray[]);		//����һ�Ͷ�Ϊ�����������㣬������Ϊ�洢Ѱ�ҵ�������������������
	void findErrorId(int slI);				//��Ǵ�������		
	int arrSLOrder();						//������𣬷��������
	void standardSL(int slI, int slC, bool);		//�Ⱦ�����
	void GenPoPAloa(int svI[], bool order);
	void GenRoughQua(int slp1, int slp2, int slp3, int slp4, int slI1, int slI2, int slI3, int slI4);	//�������ı���
	int setLikeCareQuaColor(int faceI);
	void setLikeCareQuaStruct();			//ȷ�����ı��ε����˽ṹ	
	bool genBoundaryType(std::map<int, int>& bsI, int& bsn, int &bts);				//Ϊ�߽���
	void updateLikeSLInfo(std::vector<int> sla);
	//���ѡ��---��������
	int getSelectSLI(Vec3 pos, int&);
#pragma endregion
public:
#pragma region variable
	std::vector<Vert> vert;
	std::vector<Face> face;
	void setVertTagType(StreamLine sl, const HE_edge &he);
	void getFacesStreamLineGoThrough();
	int getNextFace(HE_face &curFace,HE_edge &curEdge, Vec3 &v1, Vec3 &v2, Vec3 &u1, Vec3 &u2,const StreamLine &sl);
	HE_edge **halfEdges;
	HE_vert **halfVerts;
	HE_face **halfFaces;
	std::vector<HE_face> selectedFaces;
	Vec3 boxCenter;
	double boxSize;
	bool isloadVTK;							//����ģ��
	bool isGenRegDivToS;					//�Ƿ�����������������ز��ṹ
	std::vector<LineIndex> dirBVEdgeIndex;	//ģ�ͱ߽��߶�����
	bool isGenALOA;							//���ɲ���
	int aloa_layerS;						//�������	
	bool isGenScalarField;					//�Ƿ����ɱ�����
	std::vector<Vec3> acrossF;				//���泡��ʶ��λ��		
	bool isGenAcrossField;					//�Ƿ����ɱ�ʶ
	std::vector<Vec3> directF;				//���򳡼�ͷλ��	
	std::vector<Vec3> directHandleF;		//���򳡼�ͷ��		
	bool isGenPole;							//�Ƿ����ɼ���
	std::vector<polePoint> polePs;			//������Ϣ
	bool isGenDirectField;					//�ж��Ƿ���
	std::vector<Singular*> dirSV;			//�������
	bool isNonSVBor;						//�߽����
	bool isGenSingular;						//�Ƿ��������
	StreamLineType slt;						//��ǰ����״̬	
	std::vector<SLPoint> dirSL;				//��ʼ����
	std::vector<StreamLine*> SLS;			//������Ϣ	
	bool uSL;								//ԭ����״̬
	bool vSL;								//����value������
	bool isGenDF;							//��������������Ƿ������˾���ֵ
	bool isDisSL;							//�Ƿ�����Ȩ����
	bool isGenDifF;							//��������������Ƿ������˲��ֵ
	bool isDifSL;							//�Ƿ����ɲ������	
	bool isGenStreamLine;					//�Ƿ���������	
	std::vector<StreamLine*> SLSimplify;	//�򻯺��������Ϣ	
	std::vector<PCPoint> showSSLS;			//��������
	bool isGenSSL;							//�Ƿ��	
	std::vector<StreamLine*> QuaSLInfo;		//�ı�������	
	int dimNum;								//Ѱ���ļ۽ڵ��ǵ����Ĵ���
	std::vector<fourNode> fNode;			//�ļ۽ڵ�
	bool isFourMesh;						//�ļ۽ڵ��Ƿ�����
	std::vector<PCPoint> quaSLs;			//���ı�������
	std::vector<PCPoint> quaCareSLs;		//����ı�������
	bool isGenQua;
	bool isGenQuaMesh;
	bool isFinish;							//
	bool isQuaSmooth;
	std::vector<Quadra> quadraInfo;		//����ϸ�ı�����������
	std::vector<LineIndex> careQuaBorderInlet;	//ϸ�ı�������߽�������
	std::vector<LineIndex> careQuaBorderOutlet;	//ϸ�ı�������߽�������
	std::vector<LineIndex> careQuaBorderWall0;	//ϸ�ı�������߽�������
	std::vector<LineIndex> careQuaBorderWall1;	//ϸ�ı�������߽�������
	std::vector<QuaIndex> careQuaIndex;		//ϸ�ı������񶥵�����
	std::vector<quadraPInfo> careQuaInfo;		//�ſ˱�����
	std::vector<vertAloa> careQuaMeshP;		//ϸ�ı������񶥵�
	double quaMinAngle;
	double quaMinYkbA;
	double quaMinYkbB;
	int smoIterTimes;
	double gridPrecision;					//ϸ�ı������񾫶�
	int quaGridNum;							//ϸ�ı�����������
	DisFieldMesh* dfm;						//���볡
	bool disSL;								//�Ƿ�������Ȩ����
	ThermalField* tfm;						//��ֳ�
	bool difSL;								//�Ƿ������ɲ������
#pragma endregion
#pragma region function
	bool isEmpty()const;
	Mesh();
	void clear();
	~Mesh() {}
	void loadPartR(const char*fn);
	void loadVTK1(const char*fn);
	void loadVTK(const char*fn);
	void GenHalfStruct();
	bool setCroSize(int, int, double);
	bool setDirSize(int, int, double, double);
	int getBFrontPTodNextPI(int dirPI);	//��ñ߽�����һ���߽��
	bool GenRegDiv();					//���򻮷ֳ�ʼ��
	void getPolePoint();				//��ü���
	//core SV
	void getBorderSingular(int edgeI);
	void findSingular();
	QString getSVCount();
	//core SL
	void findEdgePointPlus(int edgeI, Vec3 startPos, SLPoint* endPos, int &endPosNum);
	SLPoint StreamLinePoint(const SLPoint *startPos);
	bool getStreamLine(int singularI, int streamLineI, const SLPoint *edgeP, int& slpcT);
	void getStreamLines();				//��������
	QString getSLInfoFP(Vec3 pos);				//�鿴������Ϣ
	//aux sl
	void getWeight(double value, double& para1, double& para2);
	double getWeight(int vertI);
	//field
	void aloa();						//���������
	void scalarFiled();					//�����������ɣ�todo:Ϊ���Ӿ��Ա�
	void acrossField();					//���泡�ı�ʶ
	void directField();					//���򳡵ı�ʶ
	//sim SL	
	void ReduceSLToArrayClear();
	void ReduceSLToArray();				//ֱ�Ӽ�
	void ReduceSLToArrayI();			//���ڼ򻯶���	
	int getSSLC();						//�򻯺����������
	void setLLSSpec(double spec);		//���ü�����
	double getLLSSpec();
	//�ı�������		
	void QuaDimSL();					//����ֽ⴦��	
	void findQuaSFValue();
	bool JudgeTAIA(double gp, bool is);	//�ж��Ƿ��ʺ����ɾ���Ϊgp��ϸ�ı���
	void genQuaMesh();				//ȷ�����ı��κ����ϸ�ı�����������
	void GenCareQua();				//ϸ�ı�����������	
	int smoothMesh();						//��˳����
	void resetSmoothMesh();
	void preGenQuaStatec();					//�ı�����������Ԥ����
	int setPGQC(Vec3 pos);				//���ñ߽����
	void setQBT();
	//���볡
	void getDFSL();						//���ɾ��볡����
	void setDisQuaSize(double);			//���þ��볡��������ߴ�
	double getDisQuaSize();				//��þ��볡��������ߴ�
	void saveSawtoothQua(QString st);
	//��ֳ�
	void getTFSL();						//���ɲ�ֳ�����
	void setDifQuaSize(double);			//���ò�ֳ���������ߴ�
	double getDifQuaSize();				//��ò�ֳ���������ߴ�	
	//����ѡ��򻯴���
	int getSelSLFP(Vec3 pos, int& sv);
	void selectStreamLine(Vec3 pos);
	void selecTtraps();
	void selectSimSL();
	bool isSelectOK();
	//�߽����������趨
	QString setQBTSL(Vec3 pos, QuaBoundaryType qbt);
	int setQBTSL(Vec3 sPos, Vec3 ePos);
	//�趨�߽�Լ����
	QString setBCV(Vec3 pos);	
	//�ļ����
	void writeTxt(int quaBlcT, QString path);
	bool writeQuaZx(QString path);
	bool writeReduceSLTxt(QString path);
	bool writeLikeQua(QString path);
	bool writeSSLVTK(QString path);
	void writeVTK(QString path);
	bool writeVTKQJOC(QString path);
	bool writeVTKQMINA(QString path);
	bool writeVTKB(QString path);
	bool writeSegBlcn(QString path);
	bool writeGM2(QString path);
	bool writeGD2(QString path);
	bool writeToCGNS(QString path);
	bool writeToExcel(QString path);
	bool writeToPAT3(QString path);
	bool writeToCFL(QStringList path);

	void showNormClass();
	bool highLightCrossField(QString filePath);
	std::vector<PCPoint> highLightedCrossField;
#pragma endregion	
	std::vector<int> singularTXT;
	void loadTXT(const char*fn);
	bool isloadTXT;
	void findSL();
	bool genSL;
};




