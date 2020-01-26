#pragma once
#include <QtWidgets/QWidget>
#include "ui_FPartition.h"

class FPartition : public QWidget
{
	Q_OBJECT

public:
	FPartition(QWidget *parent = Q_NULLPTR);
	//基础操作		
	void pickDataFile(QString);
	void showDisMesh(bool is);
	void pickDataset(QString);
	void isShow(bool is);
	void showSingular(bool is);
	void setNonSVBor(bool is);
	void setSVState(bool g, bool v, bool b);
	void showDoubtP(bool is);
	void showALOA(bool is);
	void showBorder(bool is);
	//流线CheckBox	
	void showTSL2(bool is);
	void showTSL1(bool is);
	void showQSL(bool is);
	void showSSL(bool is);
	void showStreamLine(bool is);
	void showDisStreamLine(bool is);
	void showValueStreamLine(bool is);
	void showDifStreamLine(bool is);
	void setSLState(bool, bool, bool, bool, bool, bool, bool, bool, bool, int, int);
	void setMeshShowState();
	//消息展示
	void displayModelInfo(bool, int, int);
	void displayALOAInfo(bool, int);
	void displayPolePInfo(bool, int);
	void displaySingularInfo(bool, QString);
	void displayDisQuaMeshInfo(bool, int, int, Vec3, Vec3, double);
	void displayDifQuaMeshInfo(bool, int, int, Vec3, Vec3, double);
	//距离场
	void showDisQuaMesh(bool is);
	void showplayDisField(bool is);
	void setDisBgMeshState(bool is);
	void setDisFieldState(bool is);
	void setDisSLAble(bool is);
	void addDisQuaSpec();
	void redDisQuaSpec();
	void showRefreshDisQuaMesh();
	void saveDisQua();
	//差分场
	void showDifQuaMesh(bool is);
	void showplayDifField(bool is);
	void setDifBgMeshState(bool is);
	void setDifFieldState(bool is);
	void setDifSLAble(bool is);
	void addDifQuaSpec();
	void redDifQuaSpec();
	void showRefreshDifQuaMesh();
	//交叉场
	void showAcrossField(bool is);
	void setCroSize();
	//方向场
	void showDirectField(bool is);
	void setDirSize();
	//标量场
	void showScalarField(bool is);
	//四边形网格生成	
	void setFourDim();
	void getFourDim(int);
	void showQMesh(bool is);
	void addQuaMeshSpec();
	void redQuaMeshSpec();
	void showRefreshQuaMesh();
	void setQuaMeshGPState(bool, double);
	void showQuaMeshInfo(bool, QString);
	void quaSmooth(bool);
	void quaSmoothbtn();
	void quaSmoothReset();
	void setQuaSmoothState();
	//简化流线限度
	void setLLSSpec();
	void setLLSState(double);
	//流线简化处理	
	void drawCoord(Vec3, double, bool);
	void setCoordSpec();
	void setSimSL();
	void setSelState(bool);
	void setDrawState(bool);
	//显示设置
	void setShowColor();
	void setLineSize();
	void setCrosssColor();
	void setDirectColorH();
	void setDirectColorB();
	void setBorderColor();
	void setMeshColor();
	//输出
	void outputFile(QString st);

	void getFacesStreamLineGoThrough(bool b);
	void showLearningResults();
	void batch();
	void outputIntersection();
	void generateLines();
private:
	Ui::FPartitionClass ui;
	QString dataPath;
	QString shaderPath;
	QString picDataPath;
	//初始化
	void fillBoxes();
	QStringList listFiles(QString base, QString ext);
	void helpF();
	//着色器
	QString vertexShaderFilename() const;
	QString fragmentShaderFilename() const;
};