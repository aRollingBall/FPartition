#pragma once
#include <QtOpenGL/QGLWidget>
#include<QtOpenGL\qgl.h>
#include<QtOpenGL\qglshaderprogram.h>
#include<QtOpenGL>
#include"mesh.h"
#include"disMesh.h"
#include "ANN\ANN.h"
class QFile;
struct Mesh;
class GLArea :public QGLWidget
{
	Q_OBJECT
public:
	GLArea(QWidget* parent = Q_NULLPTR);

	//bool is_pick_face;
	ANNkd_tree* kdTree;
	void renderSelectedFace();
	void showCustomModel(bool b);
	bool isHighLightCF;
	void highLightCF();
	//int find_face_using_selected_point();
	//int find_vertex_using_selected_point();
	//void buildIndex();
	//std::vector<Face> selectedFace;
	//std::vector<Vert> selectedVertex;
	//std::vector<int> selectedFaceIdx;
	//bool check_in_triangle_face(std::vector<Vec3> tri, Vec3 p);
	//void pick_face();
	bool showPickedFace;
	void getFacesStreamLineGoThrough(bool b);
	void loadShowDisMesh(bool on);
	bool loadDisMesh(QString fileName, QString name);
	bool loadVTKMesh(QString filename, QString name);
	bool loadTXTMesh(QString filename, QString name);
	bool loadShaders(QString vertFileName, QString fragFileName, showColor ct);
	Vec3 loadHalfEdge();
	bool showWireframe;
	bool showDivReg;
	bool showSingular;
	bool showDoubtP;
	bool select_dimension;
	bool showALOA;
	bool showQuaMesh;
	bool showQuaSSL;
	bool isShowQuaSSL;
	bool isSetSpec;
	//model
	bool showModel;
	//流线
	bool showStreamLine;
	bool showDisSL;
	bool showValueSL;
	bool showDifSL;
	bool showSLSimplify;
	//距离场	
	bool showDisQuaMesh;
	bool showDisField;
	//差分场
	bool showDifField;
	bool showDifQuaMesh;
	//交叉场
	bool showAcrossField;
	//方向场
	bool showDirectField;
	//标量场
	bool showScalarField;
	//距离面
	bool showDisMesh;
	//建立四边拓扑
	bool showSLT;
	//四边形生成预处理
	bool preGenQud;
	//分解问题域
	bool showQuaSL;
	//细四边形网格
	bool showCareQuaMesh;
	bool careQuaSmooth;
	//流线简化操作
	bool simState;
	bool drawPState;
	//为简化流线信息
	bool slInfoState;
	//添加约束点 （边界点）
	bool setCVP;
	//显示颜色
	showColor slColr;
	showColor sslColor;
	showColor quaColor;
	//边界流线类型设置启用状态
	bool quasSLBTState;
	//temo
	double time;
signals:
	void notifyMeshInfo(bool, int, int);
	void notifySingularInfo(bool, QString);
	void notifyALOAInfo(bool, int);
	void notifyDoubtPInfo(bool, int);
	void notifyDisQuaMeshInfo(bool, int, int, Vec3, Vec3, double);
	void notifyDifQuaMeshInfo(bool, int, int, Vec3, Vec3, double);
	//void setSpecState(bool);
	void updateDisSpecState(bool);
	void setDisBgMeshState(bool);
	void setSLState(bool, bool, bool, bool, bool, bool, bool, bool, bool, int, int);
	void setGSBState(bool, bool, bool);//grid,sv,b,qua
	void setDisSLAble(bool);
	void updateDifSpecState(bool);
	void setDifSLAble(bool);
	void setDifBgMeshState(bool);
	void setMeshShowState();
	void setQuaMeshGPState(bool, double);
	void notifyQuaMeshNum(bool, QString);
	void setLLSState(double);
	void setCoord(Vec3, double, bool);
	void setDrawState(bool);
	void ToSetFourDim(int);
	void setQuaSmoothState();
public:
	void setWireframeMode(bool on);
	void setDivReg(bool on);
	void setDivSingular(bool on);
	void setNonSVBor(bool on);
	void setDivStreamLine(bool on);
	void setDivSLSimplify(bool on);
	void setDivDoubtP(bool on);
	void setALOA(bool on);
	void setDisQuaMesh(bool on);
	void setDisQuaMeshSpec(double);
	void genDisQua(QString st);
	void setDisField(bool on);
	void setDivDisSL(bool on);
	void setDivVSL(bool on);
	void setDivDifSL(bool on);
	void setDifQuaMesh(bool on);
	void setDifField(bool on);
	void setDifQuaMeshSpec(double);
	void setAcrossField(bool on);
	void setDirectField(bool on);
	void setScalarField(bool on);
	void setQuaMeshSpec(double);
	void setCreateFourValueMesh(bool on);
	void setSTL1(bool on);
	void setSTL2(bool on);
	void setLLSSpec(double);
	void setDrawPState(bool);
	void setCroSize(int, int, double, bool);
	void setDirSize(int, int, double, double, bool);
	void outputFiles(QString, QStringList);
	void setCareQuaSmo(bool);
	void smoothCareQua();
	void smoothCareQuaReset();
	//new
	void showTempSL();
	void setQMesh(bool on);
	void setSimState(bool on);
	void setSimCoord(QString s);
	void setSimSL();
	bool Pick(const int & x, const int &y, Vec3 &pp);
	void setFourDim(int);
	void setShowColor(showColor, showColor);
	void setLineSize(double sw);
	void setPreGenQua();

	void getNormClass();
	void highLightCrossField(QString filePath);
private:
	void auxFun();
	void initializeGL();
	void paintGL();
	void resizeGL(int x, int y);
	void renderOMesh();
	void renderMesh(const Mesh& m);
	void centerMesh(const Mesh& m);
	void renderReg(const Mesh& m);
	void renderSingular(const Mesh &m);
	void renderStreamLine(const Mesh &m);
	void renderSSL(const Mesh &m);
	void renderDoubtP(const Mesh& m);
	void renderDisQuaMesh(const Mesh& m);
	void renderDifQuaMesh(const Mesh& m);
	void renderAcrossField(const Mesh& m);
	void renderHDirectField(const Mesh& m);
	void renderBDirectField(const Mesh& m);
	void renderDisMeshField(const DisMesh& m);
	void renderDisMeshBorder(const DisMesh& m);
	void renderDisMeshQua(const DisMesh& m);
	void enableWireframe(bool on, QGLShaderProgram&);
	void renderQuaSL(const Mesh& m);
	void renderQuaSSL(const Mesh& m);
	void renderCareQuaMesh(const Mesh& m);
	void renderCoord();
	void renderFourSV();

	/* mouse+keyboard callbacks */
	void mouseReleaseEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent*e);
	void mouseMoveEvent(QMouseEvent*e);
	void wheelEvent(QWheelEvent*e);
	void keyPressEvent(QKeyEvent *e);
	void Init();
	void InitSLState();
	/* shaders 着色器*/
	QGLShaderProgram shaderProgramBorder;
	QGLShaderProgram shaderProgramSSL;
	QGLShaderProgram shaderProgramCroB;
	QGLShaderProgram shaderProgramDirHC;
	QGLShaderProgram shaderProgramDirBC;
	QGLShaderProgram shaderProgramB;
	QGLShaderProgram shaderProgram;
	QGLShaderProgram shaderProgramModel;
	QGLShaderProgram wireShaderProgram;
	QGLShaderProgram customShaderProgram;
	QGLShaderProgram *currentShader;

	double eyeDist, eyePhi, eyeTheta;
	Vec3 centerPos;
	QPointF lastMousePos;
	void resetTrackball();
	void setTrackballView();

	int widthPix() const;
	int heightPix() const;

	Mesh mesh;
	DisMesh disMesh;
	int saveMeshCount;

	PCPoint curSimPos;
	double simCoordSpe;

	double sslw;

	QuaBoundaryType curQBT;
};


