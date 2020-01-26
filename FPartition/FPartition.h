#pragma once
#include <QtWidgets/QWidget>
#include "ui_FPartition.h"

class FPartition : public QWidget
{
	Q_OBJECT

public:
	FPartition(QWidget *parent = Q_NULLPTR);
	//��������		
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
	//����CheckBox	
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
	//��Ϣչʾ
	void displayModelInfo(bool, int, int);
	void displayALOAInfo(bool, int);
	void displayPolePInfo(bool, int);
	void displaySingularInfo(bool, QString);
	void displayDisQuaMeshInfo(bool, int, int, Vec3, Vec3, double);
	void displayDifQuaMeshInfo(bool, int, int, Vec3, Vec3, double);
	//���볡
	void showDisQuaMesh(bool is);
	void showplayDisField(bool is);
	void setDisBgMeshState(bool is);
	void setDisFieldState(bool is);
	void setDisSLAble(bool is);
	void addDisQuaSpec();
	void redDisQuaSpec();
	void showRefreshDisQuaMesh();
	void saveDisQua();
	//��ֳ�
	void showDifQuaMesh(bool is);
	void showplayDifField(bool is);
	void setDifBgMeshState(bool is);
	void setDifFieldState(bool is);
	void setDifSLAble(bool is);
	void addDifQuaSpec();
	void redDifQuaSpec();
	void showRefreshDifQuaMesh();
	//���泡
	void showAcrossField(bool is);
	void setCroSize();
	//����
	void showDirectField(bool is);
	void setDirSize();
	//������
	void showScalarField(bool is);
	//�ı�����������	
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
	//�������޶�
	void setLLSSpec();
	void setLLSState(double);
	//���߼򻯴���	
	void drawCoord(Vec3, double, bool);
	void setCoordSpec();
	void setSimSL();
	void setSelState(bool);
	void setDrawState(bool);
	//��ʾ����
	void setShowColor();
	void setLineSize();
	void setCrosssColor();
	void setDirectColorH();
	void setDirectColorB();
	void setBorderColor();
	void setMeshColor();
	//���
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
	//��ʼ��
	void fillBoxes();
	QStringList listFiles(QString base, QString ext);
	void helpF();
	//��ɫ��
	QString vertexShaderFilename() const;
	QString fragmentShaderFilename() const;
};