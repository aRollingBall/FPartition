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
const int SVertCount = 100;		//奇点的最大数量
const int SLCount = 500;		//流线最多条数
const int SLVert = 3000;			//单条流线上最多点数
const int SLFace = 5000;			//流线面
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
	
	int croLayNum;							//展示标架场的层数
	int crolpD;								//单层数据紧密程度
	double croBSize;						//标架尺寸
	int dirLayNum;							//展示方向场层数
	int dirlpD;								//单层数据紧密程度
	double dirHSize;						//箭头尺寸
	double dirBSize;						//箭柄尺寸
	std::map<int, mark_1> BECI;		//参数一：边界曲线某点索引；参数二：该曲线含有奇点数
	std::vector<LineVs> bVEdge;		//边界线	
	std::vector<BELine> bVSEdge;	//包含奇点的边界线	
	std::vector<Singular* >auxDirSV;	//用于更新奇点
	std::vector<int> dirSLPointCount;
	double llsValue;					//简化“力度”
	int selectSLIndex[2];	//记录需要简化的两条流线
	int selectSLPI;			//流线简化记录数
	std::map<std::vector<int>, slType_Secand> slType; //四边形区域
	std::vector<SFace> sf;	//四边形区域	
	std::map<int, std::map<int, std::map<int, pdStr>>> osf;
	//std::vector<SFace> osf;	//四边形区域	
	bool isTAIA;				//记录生成预期细四边形数量是否合格

	double maxPointValue;		//最大权重值
	double minPointValue;		//最小权重值
	Vec3 maxPos;		//矩形网格最右上方点
	Vec3 minPos;		//矩形网格最左下方点
	double disQuaSize;		//距离矩形规格
	double difQuaSize;		//差分矩形网格
	double maxDis;		//记录个点到相应中轴线的最大距离
#pragma endregion
#pragma region private function
	bool fixBAngle(int vertI);		//修改“拐点”处的角度
	double CountTriAngle(int faceI);	//给点三角形面片索引计算方向矢量转角
	bool createSV(Singular* sv);
	void createBESV(int vI);				//建立边界曲线虚拟奇点		
	//四边形区域分解	
	void remarkPSF(SLPoint* skp, int slI);			//记录流线经过的面
	//void remarkPlf(SLPoint* skp, int slI);
	bool findSLInterPToAuxP(int slI1, int slI2, Vec3 v1, Vec3 v2, Vec3 auxP, Vec3& interP, int lVS[2]);		//计算流线交点
	bool dimInterP(Vec3 tempP);		//判断流线交点是否存在
	void GenQuaSL(int ssI, int esI, int spI, int epI, bool isB, bool dr);	//创建QuaSLInfo流线
	void findBSL(int svI, int vertI, bool dir);		//建立边界流线
	void auxFindBSL(int svI1, int svI2, bool dir, int slspI, int slepI);
	void slsDistance(int slI);				//计算原始流线的长度
	void StreamLineClear();					//流线初始化
	void setColorSV(int svI, bool is);		//设置初始流线颜色
	//流线简化
	bool judgeQuaDomainSL(int, int, int, int, Vec3[]);
	void selectClear();
	bool ReMatchFit(int slI1, int slI2, double spec);		//流线匹配
	int MatchFit(int slI1, int slI2);						//流线直接匹配
	void reduceOSL(int slI, int& slPI);
	void reduceSL(int slI1, int slI2, int& slPI);
	int JudgeComDirSL(int slI1, int slI2);	//参数一：流出，参数二：流入	
	int getSLIFsvIToslI(int svI, int slI, int dir, Vec3 pd);
	int getPIFsvIToslI(Vec3 pos, int slI);	//根据给出的位置，找到对应流线上的位置
	void getPIFsvIToslI(int slI, int reSLI[]);
	int getSLIFsvv(int svI, Vec3 vec);
	int getPIFsvIToslI(Vec3 pos, int slI, int sp);	//与上不同的是给出寻找最大的结束位置
	void auxInterReduceSL(int slI, int& slPI);
	void auxReduceSL(int slI, int& slPI);
	void auxReReduceSL(int slI, int& slPI);
	void auxReduceSLI(int slI, std::set<int>&);
	void auxReduceSLID(std::set<int>&, int);
	bool auxRSLSVLToFSLI(int slI1, int slI2);
	int auxJudgeRSLI(int slI);
	void auxBorReduceSL(int slI, int& slPI);
	//四边形
	void getSLDir(int slI, int dirValue[]);	//true为流线方向；false为其逆方向
	bool findOpSLI(int slI, int opSLI[]);
	bool IsBorderSL(int slI);
	bool IsBorderSL(int slsI[], int len);
	void findSLToSV(int sv1, int sv2, int fsIArray[]);		//参数一和二为输入的两个奇点，参数三为存储寻找到连接两奇点的流线索引
	void findErrorId(int slI);				//标记错误流线		
	int arrSLOrder();						//流线类别，返回类别数
	void standardSL(int slI, int slC, bool);		//等距流线
	void GenPoPAloa(int svI[], bool order);
	void GenRoughQua(int slp1, int slp2, int slp3, int slp4, int slI1, int slI2, int slI3, int slI4);	//生成类四边形
	int setLikeCareQuaColor(int faceI);
	void setLikeCareQuaStruct();			//确定类四边形的拓扑结构	
	bool genBoundaryType(std::map<int, int>& bsI, int& bsn, int &bts);				//为边界编号
	void updateLikeSLInfo(std::vector<int> sla);
	//鼠标选择---辅助方法
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
	bool isloadVTK;							//加载模型
	bool isGenRegDivToS;					//是否生成了三角网格的拓补结构
	std::vector<LineIndex> dirBVEdgeIndex;	//模型边界线段索引
	bool isGenALOA;							//生成层域
	int aloa_layerS;						//领域层数	
	bool isGenScalarField;					//是否生成标量场
	std::vector<Vec3> acrossF;				//交叉场标识的位置		
	bool isGenAcrossField;					//是否生成标识
	std::vector<Vec3> directF;				//方向场箭头位置	
	std::vector<Vec3> directHandleF;		//方向场箭头柄		
	bool isGenPole;							//是否生成极点
	std::vector<polePoint> polePs;			//极点信息
	bool isGenDirectField;					//判断是否方向场
	std::vector<Singular*> dirSV;			//奇点数组
	bool isNonSVBor;						//边界奇点
	bool isGenSingular;						//是否生成奇点
	StreamLineType slt;						//当前流线状态	
	std::vector<SLPoint> dirSL;				//初始流点
	std::vector<StreamLine*> SLS;			//流线信息	
	bool uSL;								//原流线状态
	bool vSL;								//基于value的流线
	bool isGenDF;							//三角网格各顶点是否生成了距离值
	bool isDisSL;							//是否生成权流线
	bool isGenDifF;							//三角网格各顶点是否生成了差分值
	bool isDifSL;							//是否生成差分流线	
	bool isGenStreamLine;					//是否生成流线	
	std::vector<StreamLine*> SLSimplify;	//简化后的流线信息	
	std::vector<PCPoint> showSSLS;			//流点数据
	bool isGenSSL;							//是否简化	
	std::vector<StreamLine*> QuaSLInfo;		//四边形区域	
	int dimNum;								//寻找四价节点是迭代的次数
	std::vector<fourNode> fNode;			//四价节点
	bool isFourMesh;						//四价节点是否生成
	std::vector<PCPoint> quaSLs;			//类四边形流线
	std::vector<PCPoint> quaCareSLs;		//类等四边形流线
	bool isGenQua;
	bool isGenQuaMesh;
	bool isFinish;							//
	bool isQuaSmooth;
	std::vector<Quadra> quadraInfo;		//用于细四边形网格生成
	std::vector<LineIndex> careQuaBorderInlet;	//细四边形网格边界线索引
	std::vector<LineIndex> careQuaBorderOutlet;	//细四边形网格边界线索引
	std::vector<LineIndex> careQuaBorderWall0;	//细四边形网格边界线索引
	std::vector<LineIndex> careQuaBorderWall1;	//细四边形网格边界线索引
	std::vector<QuaIndex> careQuaIndex;		//细四边形网格顶点索引
	std::vector<quadraPInfo> careQuaInfo;		//雅克比质量
	std::vector<vertAloa> careQuaMeshP;		//细四边形网格顶点
	double quaMinAngle;
	double quaMinYkbA;
	double quaMinYkbB;
	int smoIterTimes;
	double gridPrecision;					//细四边形网格精度
	int quaGridNum;							//细四边形网格数量
	DisFieldMesh* dfm;						//距离场
	bool disSL;								//是否能生成权流线
	ThermalField* tfm;						//差分场
	bool difSL;								//是否能生成差分流线
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
	int getBFrontPTodNextPI(int dirPI);	//获得边界点的下一个边界点
	bool GenRegDiv();					//区域划分初始化
	void getPolePoint();				//获得极点
	//core SV
	void getBorderSingular(int edgeI);
	void findSingular();
	QString getSVCount();
	//core SL
	void findEdgePointPlus(int edgeI, Vec3 startPos, SLPoint* endPos, int &endPosNum);
	SLPoint StreamLinePoint(const SLPoint *startPos);
	bool getStreamLine(int singularI, int streamLineI, const SLPoint *edgeP, int& slpcT);
	void getStreamLines();				//生成流线
	QString getSLInfoFP(Vec3 pos);				//查看流线信息
	//aux sl
	void getWeight(double value, double& para1, double& para2);
	double getWeight(int vertI);
	//field
	void aloa();						//层域的生成
	void scalarFiled();					//标量场的生成，todo:为了视觉对比
	void acrossField();					//交叉场的标识
	void directField();					//方向场的标识
	//sim SL	
	void ReduceSLToArrayClear();
	void ReduceSLToArray();				//直接简化
	void ReduceSLToArrayI();			//存在简化对象	
	int getSSLC();						//简化后的流线条数
	void setLLSSpec(double spec);		//设置简化力度
	double getLLSSpec();
	//四边形区域		
	void QuaDimSL();					//区域分解处理	
	void findQuaSFValue();
	bool JudgeTAIA(double gp, bool is);	//判断是否适合生成精度为gp的细四边形
	void genQuaMesh();				//确定类四边形后进行细四边形网格生成
	void GenCareQua();				//细四边形网格生成	
	int smoothMesh();						//光顺网格
	void resetSmoothMesh();
	void preGenQuaStatec();					//四边形网格生成预处理
	int setPGQC(Vec3 pos);				//设置边界段数
	void setQBT();
	//距离场
	void getDFSL();						//生成距离场流线
	void setDisQuaSize(double);			//设置距离场矩形网格尺寸
	double getDisQuaSize();				//获得距离场矩形网格尺寸
	void saveSawtoothQua(QString st);
	//差分场
	void getTFSL();						//生成差分场流线
	void setDifQuaSize(double);			//设置差分场矩形网格尺寸
	double getDifQuaSize();				//获得差分场矩形网格尺寸	
	//流线选择简化处理
	int getSelSLFP(Vec3 pos, int& sv);
	void selectStreamLine(Vec3 pos);
	void selecTtraps();
	void selectSimSL();
	bool isSelectOK();
	//边界流线类型设定
	QString setQBTSL(Vec3 pos, QuaBoundaryType qbt);
	int setQBTSL(Vec3 sPos, Vec3 ePos);
	//设定边界约束点
	QString setBCV(Vec3 pos);	
	//文件输出
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




