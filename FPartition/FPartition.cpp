#include "FPartition.h"
#include<qdir.h>
#include<qmessagebox.h>
#include<qcolordialog.h>
#include <QStringList>
QString PAPER_TITLE("Regional division(QZX------DEMO) ");
static QStringList getFileNames(const QString &path);
static void GetAllFileFolder(QString dirPath, std::vector<QString> &folder);
FPartition::FPartition(QWidget *parent)
	: QWidget(parent)
{
	dataPath = "sources/";
	shaderPath = "shaders/";
	picDataPath = "sources/pictures/";
	if (!(QDir(dataPath).exists()) || !(QDir(shaderPath).exists())) {
		QMessageBox::critical(this, PAPER_TITLE,
			QString("<b>Please put me in an appropriate path</b>"
				"<br/>I need to find data and shaders path in:<br/><i>%1</i> and <i>%2</i>.<p>Exiting, see you later.</p> ").arg(dataPath).arg(shaderPath));
		exit(0);
	}
	setWindowTitle(PAPER_TITLE);
	setWindowIcon(QIcon(picDataPath + "little.ico"));
	ui.setupUi(this);
	//new流线简化操作	
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool)>(&GLArea::setDrawState), this, &FPartition::setDrawState);
	connect(ui.selSL, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::setSelState);
	connect(ui.glArea, static_cast<void(GLArea::*)(const Vec3, const double, const bool)>(&GLArea::setCoord), this, &FPartition::drawCoord);
	connect(ui.coordSpe, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setCoordSpec);
	connect(ui.simOK, &QPushButton::clicked, this, &FPartition::setSimSL);
	//四边形网格生成
	connect(ui.glArea, static_cast<void(GLArea::*)(const int)>(&GLArea::ToSetFourDim), this, &FPartition::getFourDim);
	connect(ui.iterNum, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setFourDim);
	connect(ui.QMesh, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showQMesh);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const double)>(&GLArea::setQuaMeshGPState), this, &FPartition::setQuaMeshGPState);
	connect(ui.quaGenOK, &QPushButton::clicked, this, &FPartition::showRefreshQuaMesh);
	connect(ui.quaValue_up, &QPushButton::clicked, this, &FPartition::addQuaMeshSpec);
	connect(ui.quaValue_down, &QPushButton::clicked, this, &FPartition::redQuaMeshSpec);
	connect(ui.quaSmo, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::quaSmooth);
	connect(ui.glArea, static_cast<void(GLArea::*)()>(&GLArea::setQuaSmoothState), this, &FPartition::setQuaSmoothState);
	connect(ui.quaSmoBtn, &QPushButton::clicked, this, &FPartition::quaSmoothbtn);
	connect(ui.quaSmoReset, &QPushButton::clicked, this, &FPartition::quaSmoothReset);
	//基础操作
	connect(ui.disMesh, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showDisMesh);
	connect(ui.disComboBox, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &FPartition::pickDataFile);
	connect(ui.comboBox, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &FPartition::pickDataset);
	connect(ui.show, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::isShow);
	connect(ui.showSingular, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showSingular);
	connect(ui.svNonBor, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::setNonSVBor);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const bool, const bool)>(&GLArea::setGSBState), this, &FPartition::setSVState);
	connect(ui.doubtP, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showDoubtP);
	connect(ui.aloa, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showALOA);
	connect(ui.border, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showBorder);
	connect(ui.glArea, static_cast<void(GLArea::*)()>(&GLArea::setMeshShowState), this, &FPartition::setMeshShowState);
	//流线	
	connect(ui.TSL2, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showTSL2);
	connect(ui.TSL1, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showTSL1);
	connect(ui.Qua, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showQSL);
	connect(ui.SSL, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showSSL);
	connect(ui.showStreamLine, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showStreamLine);
	connect(ui.slDis, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showDisStreamLine);
	connect(ui.slValue, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showValueStreamLine);
	connect(ui.slDif, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showDifStreamLine);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const bool, const bool, const bool, const bool, const bool, const bool, const bool, const bool, const int, const int)>(&GLArea::setSLState), this, &FPartition::setSLState);
	//数据信息
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const QString)>(&GLArea::notifyQuaMeshNum), this, &FPartition::showQuaMeshInfo);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const int, const int)>(&GLArea::notifyMeshInfo), this, &FPartition::displayModelInfo);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const int)>(&GLArea::notifyALOAInfo), this, &FPartition::displayALOAInfo);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const int)>(&GLArea::notifyDoubtPInfo), this, &FPartition::displayPolePInfo);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const QString)>(&GLArea::notifySingularInfo), this, &FPartition::displaySingularInfo);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const int, const int, const Vec3, const Vec3, const double)>(&GLArea::notifyDisQuaMeshInfo), this, &FPartition::displayDisQuaMeshInfo);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool, const int, const int, const Vec3, const Vec3, const double)>(&GLArea::notifyDifQuaMeshInfo), this, &FPartition::displayDifQuaMeshInfo);
	//距离场

	connect(ui.btnQuaS, &QPushButton::clicked, this, &FPartition::saveDisQua);
	connect(ui.disBgGrid, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showDisQuaMesh);
	connect(ui.disField, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showplayDisField);
	connect(ui.disOK, &QPushButton::clicked, this, &FPartition::showRefreshDisQuaMesh);
	connect(ui.disValue_up, &QPushButton::clicked, this, &FPartition::addDisQuaSpec);
	connect(ui.disValue_down, &QPushButton::clicked, this, &FPartition::redDisQuaSpec);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool)>(&GLArea::updateDisSpecState), this, &FPartition::setDisFieldState);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool)>(&GLArea::setDisSLAble), this, &FPartition::setDisSLAble);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool)>(&GLArea::setDisBgMeshState), this, &FPartition::setDisBgMeshState);
	//差分场
	connect(ui.difBgGrid, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showDifQuaMesh);
	connect(ui.difField, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showplayDifField);
	connect(ui.difOK, &QPushButton::clicked, this, &FPartition::showRefreshDifQuaMesh);
	connect(ui.difValue_up, &QPushButton::clicked, this, &FPartition::addDifQuaSpec);
	connect(ui.difValue_down, &QPushButton::clicked, this, &FPartition::redDifQuaSpec);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool)>(&GLArea::updateDifSpecState), this, &FPartition::setDifFieldState);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool)>(&GLArea::setDifSLAble), this, &FPartition::setDifSLAble);
	connect(ui.glArea, static_cast<void(GLArea::*)(const bool)>(&GLArea::setDifBgMeshState), this, &FPartition::setDifBgMeshState);
	//交叉场
	connect(ui.crossField, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showAcrossField);
	connect(ui.croLNum, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setCroSize);
	connect(ui.croBSize, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setCroSize);
	connect(ui.croLPd, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setCroSize);
	//方向场
	connect(ui.directField, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showDirectField);
	connect(ui.dirLNum, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setDirSize);
	connect(ui.dirBSize, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setDirSize);
	connect(ui.dirHSize, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setDirSize);
	connect(ui.dirLPd, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setDirSize);
	//标量场
	connect(ui.scalarField, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showScalarField);
	//流线简化限度
	connect(ui.lls, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setLLSSpec);
	connect(ui.glArea, static_cast<void(GLArea::*)(const double)>(&GLArea::setLLSState), this, &FPartition::setLLSState);
	//显示设置
	connect(ui.sslC1, static_cast<void(QRadioButton::*)(const bool)>(&QRadioButton::clicked), this, &FPartition::setShowColor);
	connect(ui.sslC2, static_cast<void(QRadioButton::*)(const bool)>(&QRadioButton::clicked), this, &FPartition::setShowColor);
	connect(ui.quaC1, static_cast<void(QRadioButton::*)(const bool)>(&QRadioButton::clicked), this, &FPartition::setShowColor);
	connect(ui.quaC2, static_cast<void(QRadioButton::*)(const bool)>(&QRadioButton::clicked), this, &FPartition::setShowColor);
	connect(ui.sslw, static_cast<void(QLineEdit::*)()>(&QLineEdit::returnPressed), this, &FPartition::setLineSize);
	connect(ui.croC, &QPushButton::clicked, this, &FPartition::setCrosssColor);
	connect(ui.dirHC, &QPushButton::clicked, this, &FPartition::setDirectColorH);
	connect(ui.dirBC, &QPushButton::clicked, this, &FPartition::setDirectColorB);
	connect(ui.borC, &QPushButton::clicked, this, &FPartition::setBorderColor);
	connect(ui.meshC, &QPushButton::clicked, this, &FPartition::setMeshColor);
	//文件输出
	connect(ui.outputFiles, static_cast<void(QComboBox::*)(const QString&)>(&QComboBox::currentIndexChanged), this, &FPartition::outputFile);
	// 面片选择
	//connect(ui.PF, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::setFacePick);
	connect(ui.AutoFacePick, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::setPickFaceShow);
	connect(ui.CustomCheck, static_cast<void(QCheckBox::*)(const bool)>(&QCheckBox::clicked), this, &FPartition::showCustomModel);

	connect(ui.Batch, &QPushButton::clicked, this, &FPartition::batch);
	connect(ui.NormClass, &QPushButton::clicked, this, &FPartition::normClass);
	connect(ui.HighLightCF, &QPushButton::clicked, this, &FPartition::highLightCrossField);
	//connect(ui.show, &QCheckBox::clicked(bool), this, &FPartition::isShow);
	//ui.comboBox->currentIndex = -1;	
	QRegExp rx("100|([0-9]{0,2}[.][0-9]{1,5})");
	QRegExpValidator *pReg = new QRegExpValidator(rx, this);
	//流线简化	
	ui.lls->setValidator(pReg);
	ui.lls->setEnabled(false);
	//距离场
	ui.disSpec->setValidator(pReg);
	ui.disSpec->setEnabled(false);
	//ui.spec->setValidator(new QDoubleValidator(0.0, 1.0, 5, this));
	ui.disValue_up->setIcon(QIcon(picDataPath + "up.ico"));
	ui.disValue_down->setIcon(QIcon(picDataPath + "down.ico"));
	ui.disOK->setIcon(QIcon(picDataPath + "refresh.ico"));
	ui.disValue_up->setEnabled(false);
	ui.disValue_down->setEnabled(false);
	ui.disOK->setEnabled(false);
	ui.slDis->setEnabled(false);
	//差分场
	ui.difSpec->setValidator(pReg);
	ui.difSpec->setEnabled(false);
	//ui.spec->setValidator(new QDoubleValidator(0.0, 1.0, 5, this));
	ui.difValue_up->setIcon(QIcon(picDataPath + "up.ico"));
	ui.difValue_down->setIcon(QIcon(picDataPath + "down.ico"));
	ui.difOK->setIcon(QIcon(picDataPath + "refresh.ico"));
	ui.difValue_up->setEnabled(false);
	ui.difValue_down->setEnabled(false);
	ui.difOK->setEnabled(false);
	ui.slDif->setEnabled(false);
	//交叉场
	//四边形网格
	ui.quaGP->setValidator(pReg);
	ui.quaGP->setEnabled(false);
	ui.quaValue_up->setIcon(QIcon(picDataPath + "up.ico"));
	ui.quaValue_down->setIcon(QIcon(picDataPath + "down.ico"));
	ui.quaGenOK->setIcon(QIcon(picDataPath + "refresh.ico"));
	ui.quaValue_up->setEnabled(false);
	ui.quaValue_down->setEnabled(false);
	ui.quaGenOK->setEnabled(false);

	//end
	/*ui.coordX->setValidator(pReg);
	ui.coordY->setValidator(pReg);*/
	ui.coordSpe->setValidator(pReg);
	ui.coordX->setEnabled(false);
	ui.coordY->setEnabled(false);
	ui.coordZ->setEnabled(false);
	ui.coordSpe->setAlignment(Qt::AlignCenter);
	ui.coordX->setAlignment(Qt::AlignCenter);
	ui.coordY->setAlignment(Qt::AlignCenter);
	ui.coordZ->setAlignment(Qt::AlignCenter);
	ui.coordSpe->setEnabled(false);
	QRegExp ry("^[1-9]\\d{0,1}$");
	QRegExpValidator *pyReg = new QRegExpValidator(ry, this);
	ui.iterNum->setValidator(pyReg);
	ui.iterNum->setAlignment(Qt::AlignCenter);
	ui.iterNum->setEnabled(false);
	//editline
	//ui.lls->setAlignment(Qt::AlignCenter);
	QRegExp rz("1|0.\\d{1,2}");
	QRegExpValidator *pzReg = new QRegExpValidator(rz, this);
	QRegExp rw("[0-9].\\d{1,2}");
	QRegExpValidator *pwReg = new QRegExpValidator(rw, this);
	ui.croLNum->setValidator(pyReg);
	ui.croLNum->setAlignment(Qt::AlignCenter);
	ui.croBSize->setValidator(pReg);
	ui.croBSize->setAlignment(Qt::AlignCenter);
	ui.croLPd->setValidator(pyReg);
	ui.croLPd->setAlignment(Qt::AlignCenter);
	ui.dirLNum->setValidator(pyReg);
	ui.dirLNum->setAlignment(Qt::AlignCenter);
	ui.dirHSize->setValidator(pwReg);
	ui.dirHSize->setAlignment(Qt::AlignCenter);
	ui.dirBSize->setValidator(pReg);
	ui.dirBSize->setAlignment(Qt::AlignCenter);
	ui.dirLPd->setValidator(pyReg);
	ui.dirLPd->setAlignment(Qt::AlignCenter);
	//add	
	ui.sslw->setValidator(pReg);
	ui.sslw->setAlignment(Qt::AlignCenter);

	ui.croC->setIcon(QIcon(picDataPath + "cro.ico"));
	ui.dirHC->setIcon(QIcon(picDataPath + "dirH.ico"));
	ui.dirBC->setIcon(QIcon(picDataPath + "dirB.ico"));
	fillBoxes();
}
static bool fixFileColorInfo(QString basename, Vec3 color);
#pragma region 文件输出
void FPartition::outputFile(QString st) {
	if (!st.isEmpty()) {
		QString basename = QFileInfo(st).baseName();
		QFileDialog fileDialog;
		QString fileName = fileDialog.getSaveFileName(this, tr("Open File"), "/", tr("Text File(*.txt *.zx *.gm2 *.gd2 *.acgns *.vtk *.lcn)"));
		QStringList filepath;
		filepath.append(fileName);
		if (fileName == "") {
			QMessageBox::warning(NULL, "warning", "fileName is empty!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
		else {
			ui.glArea->outputFiles(basename, filepath);
		}
	}
}
void FPartition::setPickFaceShow(bool b)
{
	ui.glArea->getFacesStreamLineGoThrough(b);
}

#pragma endregion

#pragma region 显示设置
void FPartition::setShowColor() {
	QRadioButton* pbtn1 = qobject_cast<QRadioButton*>(ui.c_sslBG->checkedButton());
	QRadioButton* pbtn2 = qobject_cast<QRadioButton*>(ui.c_quaBG->checkedButton());
	QString name1 = pbtn1->objectName();
	QString name2 = pbtn2->objectName();
	showColor c_ssl;
	showColor c_qua;
	if (!QString::compare(name1, "sslC2")) {
		c_ssl = s_colour;
	}
	else {
		c_ssl = pure_c;
		QColor c = QColorDialog::getColor(Qt::black);
		Vec3 color(c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0);
		if (fixFileColorInfo(shaderPath + "basic.frag.ssl.glsl", color)) {
			ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.ssl.glsl", pure_c);
		}
	}
	if (!QString::compare(name2, "quaC1")) {
		c_qua = s_black;
	}
	else {
		c_qua = s_colour;
	}
	ui.glArea->setShowColor(c_ssl, c_qua);
}
void FPartition::setLineSize() {
	ui.glArea->setLineSize(ui.sslw->text().toDouble());
}
void FPartition::setCrosssColor() {
	QColor c = QColorDialog::getColor(Qt::black);
	Vec3 color(c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0);
	if (fixFileColorInfo(shaderPath + "basic.frag.croC.glsl", color)) {
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.croC.glsl", croB_color);
	}
}
void FPartition::setDirectColorH() {
	QColor c = QColorDialog::getColor(Qt::black);
	Vec3 color(c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0);
	if (fixFileColorInfo(shaderPath + "basic.frag.dirHC.glsl", color)) {
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.dirHC.glsl", dirH_color);
	}
}
void FPartition::setDirectColorB() {
	QColor c = QColorDialog::getColor(Qt::black);
	Vec3 color(c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0);
	if (fixFileColorInfo(shaderPath + "basic.frag.dirBC.glsl", color)) {
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.dirBC.glsl", dirB_color);
	}
}
void FPartition::setBorderColor() {
	QColor c = QColorDialog::getColor(Qt::black);
	Vec3 color(c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0);
	if (fixFileColorInfo(shaderPath + "basic.frag.border.glsl", color)) {
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.border.glsl", border_color);
	}
}
void FPartition::setMeshColor() {
	QColor c = QColorDialog::getColor(Qt::black);
	Vec3 color(c.red() / 255.0, c.green() / 255.0, c.blue() / 255.0);
	if (fixFileColorInfo(shaderPath + "basic.frag.black.glsl", color)) {
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.black.glsl", s_black);
	}
}
#pragma endregion

#pragma region 流线简化操作
void FPartition::setDrawState(bool is) {
	ui.selSL->setCheckState(is ? Qt::Checked : Qt::Unchecked);
}
void FPartition::setSelState(bool is) {
	ui.glArea->setDrawPState(is);
}
void FPartition::drawCoord(Vec3 coord, double spec, bool is) {
	ui.coordX->setText(QString::number(coord[0], 'f', 6));
	ui.coordY->setText(QString::number(coord[1], 'f', 6));
	ui.coordZ->setText(QString::number(coord[2], 'f', 6));
	ui.coordSpe->setText(QString::number(spec, 'f', 6));
	ui.coordX->setEnabled(is);
	ui.coordY->setEnabled(is);
	ui.coordZ->setEnabled(is);
	ui.coordSpe->setEnabled(is);
}
void FPartition::setCoordSpec() {
	ui.glArea->setSimCoord(ui.coordSpe->text());
}
void FPartition::setSimSL() {
	ui.glArea->setSimSL();
}
#pragma endregion

#pragma region 简化流线限度
void FPartition::setLLSSpec() {
	ui.glArea->setLLSSpec(ui.lls->text().toDouble());
}
void FPartition::setLLSState(double spec) {
	ui.lls->setEnabled(true);
	ui.lls->setText(QString::number(spec, 'f', 8));
}
#pragma endregion
#pragma region 四边形网格生成
void FPartition::setFourDim() {
	ui.glArea->setFourDim(ui.iterNum->text().toInt());
}
void FPartition::getFourDim(int s) {
	ui.iterNum->setEnabled(true);
	ui.iterNum->setText(QString::number(s, 10));
}
void FPartition::showQMesh(bool is) {
	ui.glArea->setQMesh(is);
}
void FPartition::addQuaMeshSpec() {
	double tempValue = ui.quaGP->text().toDouble()*2.0;
	if (tempValue < 10.0) {
		ui.quaGP->setText(QString::number(tempValue, 'f', 8));
	}
}
void FPartition::redQuaMeshSpec() {
	double tempValue = ui.quaGP->text().toDouble() / 2.0;
	if (tempValue > 1e-9) {
		ui.quaGP->setText(QString::number(tempValue, 'f', 8));
	}
}
void FPartition::showRefreshQuaMesh() {
	ui.glArea->setQuaMeshSpec(ui.quaGP->text().toDouble());
}
void FPartition::setQuaMeshGPState(bool is, double spec) {
	if (is) {
		ui.quaGP->setText(QString::number(spec, 'f', 6));
	}
	else {
		ui.quaGP->setText(QString(""));
	}
	ui.quaGP->setEnabled(is);
	ui.quaValue_up->setEnabled(is);
	ui.quaValue_down->setEnabled(is);
	ui.quaGenOK->setEnabled(is);
}
void FPartition::quaSmooth(bool is) {
	ui.glArea->setCareQuaSmo(is);
}
void FPartition::quaSmoothbtn() {
	ui.glArea->smoothCareQua();
}
void FPartition::quaSmoothReset() {
	ui.glArea->smoothCareQuaReset();
}
void FPartition::setQuaSmoothState() {
	ui.quaSmo->setCheckState(Qt::Unchecked);
}
#pragma endregion
#pragma region 标量场
void FPartition::showScalarField(bool is) {
	ui.glArea->setScalarField(is);
}
#pragma endregion
#pragma region 方向场
void FPartition::showDirectField(bool is) {
	ui.glArea->setDirectField(is);
}
void FPartition::setDirSize() {
	QString dln = ui.dirLNum->text();
	QString dbs = ui.dirBSize->text();
	QString dhs = ui.dirHSize->text();
	QString dlp = ui.dirLPd->text();
	if (dln != "" && dbs != "" && dhs != "" && dlp != "") {
		ui.glArea->setDirSize(dln.toInt(), dlp.toInt(), dbs.toDouble(), dhs.toDouble(), ui.directField->checkState() == Qt::Checked);
	}
}
#pragma endregion
#pragma region 交叉场
void FPartition::showAcrossField(bool is) {
	ui.glArea->setAcrossField(is);
}
void FPartition::setCroSize() {
	QString cln = ui.croLNum->text();
	QString cbs = ui.croBSize->text();
	QString clp = ui.croLPd->text();
	if (cln != "" && cbs != "" && clp != "") {
		ui.glArea->setCroSize(cln.toInt(), clp.toInt(), cbs.toDouble(), ui.crossField->checkState() == Qt::Checked);
	}
}
#pragma endregion
#pragma region 差分场
void FPartition::showDifQuaMesh(bool is) {
	ui.glArea->setDifQuaMesh(is);
}
void FPartition::showplayDifField(bool is) {
	ui.glArea->setDifField(is);
}
void FPartition::setDifBgMeshState(bool is) {
	ui.difBgGrid->setCheckState(is ? Qt::Checked : Qt::Unchecked);
}
void FPartition::setDifFieldState(bool is) {
	bool on = is;;
	if (ui.difField->checkState() == Qt::Checked || ui.difBgGrid->checkState() == Qt::Checked) {
		on = true;
	}
	ui.difValue_up->setEnabled(on);
	ui.difValue_down->setEnabled(on);
	ui.difOK->setEnabled(on);
	ui.difSpec->setEnabled(on);
}
void FPartition::setDifSLAble(bool is) {
	ui.slDif->setEnabled(is);
}
void FPartition::addDifQuaSpec() {
	float tempValue = ui.difSpec->text().toFloat()*2.0;
	if (tempValue <= 100.0)
		ui.difSpec->setText(QString::number(tempValue, 'f', 5));
}
void FPartition::redDifQuaSpec() {
	float tempValue = ui.difSpec->text().toFloat() / 2.0;
	if (tempValue >= 1e-4)
		ui.difSpec->setText(QString::number(tempValue, 'f', 5));
}
void FPartition::showRefreshDifQuaMesh() {
	ui.glArea->setDifQuaMeshSpec(ui.difSpec->text().toFloat());
}
#pragma endregion
#pragma region 距离场
void FPartition::showDisQuaMesh(bool is) {
	ui.glArea->setDisQuaMesh(is);
}
void FPartition::showplayDisField(bool is) {
	ui.glArea->setDisField(is);
}
void FPartition::setDisBgMeshState(bool is) {
	ui.disBgGrid->setCheckState(is ? Qt::Checked : Qt::Unchecked);
}
void FPartition::setDisFieldState(bool is) {
	bool on = is;;
	if (ui.disField->checkState() == Qt::Checked || ui.disBgGrid->checkState() == Qt::Checked) {
		on = true;
	}
	ui.disValue_up->setEnabled(on);
	ui.disValue_down->setEnabled(on);
	ui.disOK->setEnabled(on);
	ui.disSpec->setEnabled(on);
}
void FPartition::setDisSLAble(bool is) {
	ui.slDis->setEnabled(is);
}
void FPartition::addDisQuaSpec() {
	float tempValue = ui.disSpec->text().toFloat()*2.0;
	if (tempValue <= 100.0)
		ui.disSpec->setText(QString::number(tempValue, 'f', 5));
}
void FPartition::redDisQuaSpec() {
	float tempValue = ui.disSpec->text().toFloat() / 2.0;
	if (tempValue >= 1e-4)
		ui.disSpec->setText(QString::number(tempValue, 'f', 5));
}
void FPartition::showRefreshDisQuaMesh() {
	ui.glArea->setDisQuaMeshSpec(ui.disSpec->text().toFloat());
}
void FPartition::saveDisQua() {
	QFileDialog fileDialog;
	QString fileName = fileDialog.getSaveFileName(this, tr("Open File"), "/", tr("Text File(*.txt *.zx *.gm2 *.gd2 *.acgns *.vtk *.lcn)"));
	if (fileName == "") {
		QMessageBox::warning(NULL, "warning", "fileName is empty!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return;
	}
	else {
		ui.glArea->genDisQua(fileName);
	}
}

#pragma endregion
#pragma region 消息展示
void FPartition::showQuaMeshInfo(bool on, QString num) {
	ui.quaMeshInfo->setText(on ? num : "");
}
void FPartition::displayModelInfo(bool on, int vert, int tri) {
	ui.modelInfo->setText(on ? QString("<font color='blue' size=4>tris:%1   verts:%2</font>").arg(tri).arg(vert) : "");
}
void FPartition::displayALOAInfo(bool on, int aloaCount) {
	ui.aloaInfo->setText(on ? QString("<font color='blue' size=4>Number of layers:%1</font>").arg(aloaCount) : "");
}
void FPartition::displayPolePInfo(bool on, int polePCount) {
	ui.poleInfo->setText(on ? QString("<font color='blue' size=4>Pole number:%1</font>").arg(polePCount) : "");
}
void FPartition::displaySingularInfo(bool on, QString st) {
	//ui.singularInfo->setText(on ? QString("three number:%1  five number:%2</font>").arg(thrSv).arg(fivSv) : "");
	//QString info = "three:" + QString::number(thrSv) + " | five" + QString::number(fivSv) + "\n" + "border:" + QString::number(bsv) + " | Corner:" + QString::number(csv) + " | inter:" + QString::number(isv);	
	ui.singularInfo->setText(on ? st : "");
}
void FPartition::displayDisQuaMeshInfo(bool on, int col, int row, Vec3 minP, Vec3 maxP, double specifQua) {
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
	ui.disQuaInfo->setText(on ? QString("<font color='blue' size=4>cols:%1 ; Rows:%2<br/>minP:(%3,%4,%5)<br/>maxP:(%6,%7,%8)<br/>size:%9</font>").arg(col).arg(row)
		.arg(minP[0], 0, 'g', 3).arg(minP[1], 0, 'g', 3).arg(minP[2], 0, 'g', 3)
		.arg(maxP[0], 0, 'g', 3).arg(maxP[1], 0, 'g', 3).arg(maxP[2], 0, 'g', 3)
		.arg(specifQua, 0, 'g', 4) : "");
	ui.disSpec->setText(QString::number(specifQua, 'f', 5));
}
void FPartition::displayDifQuaMeshInfo(bool on, int col, int row, Vec3 minP, Vec3 maxP, double specifQua) {
	QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));
	ui.difQuaInfo->setText(on ? QString("<font color='blue' size=4>cols:%1 ; Rows:%2<br/>minP:(%3,%4,%5)<br/>maxP:(%6,%7,%8)<br/>size:%9</font>").arg(col).arg(row)
		.arg(minP[0], 0, 'g', 3).arg(minP[1], 0, 'g', 3).arg(minP[2], 0, 'g', 3)
		.arg(maxP[0], 0, 'g', 3).arg(maxP[1], 0, 'g', 3).arg(maxP[2], 0, 'g', 3)
		.arg(specifQua, 0, 'g', 4) : "");
	ui.difSpec->setText(QString::number(specifQua, 'f', 5));
}
#pragma endregion
#pragma region 流线控制
void FPartition::showTSL2(bool is) {
	ui.glArea->setSTL2(is);
}
void FPartition::showTSL1(bool is) {
	ui.glArea->setSTL1(is);
}
void FPartition::showQSL(bool is) {
	ui.glArea->setCreateFourValueMesh(is);
}
void FPartition::showSSL(bool is) {
	ui.glArea->setDivSLSimplify(is);
	//ui.glArea->setLLSSpec(4.0);
}
void FPartition::showStreamLine(bool is) {
	ui.glArea->setDivStreamLine(is);
}
void FPartition::showDisStreamLine(bool is) {
	ui.glArea->setDivDisSL(is);
}
void FPartition::showValueStreamLine(bool is) {
	ui.glArea->setDivVSL(is);
}
void FPartition::showDifStreamLine(bool is) {
	ui.glArea->setDivDifSL(is);
}
void FPartition::setSLState(bool is1, bool is2, bool is3, bool is4, bool is5, bool is6, bool is7, bool is8, bool is9, int streamLineCount, int sSLC) {
	ui.QMesh->setCheckState(is9 ? Qt::Checked : Qt::Unchecked);
	ui.TSL2->setCheckState(is8 ? Qt::Checked : Qt::Unchecked);
	ui.TSL1->setCheckState(is7 ? Qt::Checked : Qt::Unchecked);
	ui.Qua->setCheckState(is6 ? Qt::Checked : Qt::Unchecked);
	ui.SSL->setCheckState(is5 ? Qt::Checked : Qt::Unchecked);
	ui.showStreamLine->setCheckState(is4 ? Qt::Checked : Qt::Unchecked);
	ui.slDis->setCheckState(is3 ? Qt::Checked : Qt::Unchecked);
	ui.slValue->setCheckState(is2 ? Qt::Checked : Qt::Unchecked);
	ui.slDif->setCheckState(is1 ? Qt::Checked : Qt::Unchecked);
	ui.streamLineInfo->setText(is1 || is2 || is3 || is4 || is5 || is7 || is8 ? QString("<font color='blue' size=4>RSL:%1   SSL:%2</font>").arg(streamLineCount).arg(sSLC) : "");
}
#pragma endregion
#pragma region 基本操作
void FPartition::setMeshShowState() {
	ui.border->setCheckState(Qt::Checked);
	ui.show->setCheckState(Qt::Unchecked);
}
void FPartition::showDisMesh(bool is) {
	ui.glArea->loadShowDisMesh(false);
}
void FPartition::pickDataFile(QString st) {
	if (!st.isEmpty()) {
		QString basename = QFileInfo(st).baseName();
		//ui.glArea->chooseMesh(ataPath + st + ".vtk", basename);
		if (ui.glArea->loadDisMesh(dataPath + st + ".txt", basename)) {
			//ui.glArea->loadHalfEdge();
		}
	}
}
void FPartition::pickDataset(QString st) {
	if (!st.isEmpty()) {
		QString basename = QFileInfo(st).baseName();
		//ui.glArea->chooseMesh(ataPath + st + ".vtk", basename);
		setWindowTitle(PAPER_TITLE + st);
		if (ui.glArea->loadVTKMesh(dataPath + st + ".vtk", basename)) {
			Vec3 coord = ui.glArea->loadHalfEdge();
			ui.coordX->setText(QString::number(coord[0], 'f', 8));
			ui.coordY->setText(QString::number(coord[1], 'f', 8));
			ui.coordZ->setText(QString::number(coord[2], 'f', 8));
		}
		else if (ui.glArea->loadTXTMesh(dataPath + st + ".txt", basename)) {
			Vec3 coord = ui.glArea->loadHalfEdge();
			ui.coordX->setText(QString::number(coord[0], 'f', 8));
			ui.coordY->setText(QString::number(coord[1], 'f', 8));
			ui.coordZ->setText(QString::number(coord[2], 'f', 8));
		}
		else {
			setWindowTitle(PAPER_TITLE);
		}
		ui.glArea->showWireframe = true;
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.glsl", s_colour);
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.black.glsl", s_black);
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.blue.glsl", s_blue);
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.border.glsl", border_color);
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.croC.glsl", croB_color);
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.dirBC.glsl", dirB_color);
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.dirHC.glsl", dirH_color);
		ui.glArea->loadShaders(shaderPath + "basic.vert.glsl", shaderPath + "basic.frag.ssl.glsl", pure_c);
		ui.glArea->loadShaders(shaderPath + "basic.vert.model.glsl", shaderPath + "basic.frag.model.glsl", model_color);
		ui.glArea->loadShaders(shaderPath + "custom.vert.glsl", shaderPath + "custom.frag.glsl", custom_shader);
		ui.outputFiles->setCurrentIndex(-1);
	}
	else {
		setWindowTitle(PAPER_TITLE);
	}
	ui.show->setCheckState(Qt::Checked);
	ui.glArea->setWireframeMode(true);
	helpF();
}
void FPartition::showCustomModel(bool b) {
	ui.glArea->showCustomModel(b);
}
void FPartition::isShow(bool is) {
	ui.glArea->setWireframeMode(is);
}
void FPartition::showSingular(bool is) {
	ui.glArea->setDivSingular(is);
}
void FPartition::setNonSVBor(bool is) {
	ui.glArea->setNonSVBor(is);
}
void FPartition::setSVState(bool g, bool v, bool b) {
	ui.show->setCheckState(g ? Qt::Checked : Qt::Unchecked);
	ui.showSingular->setCheckState(v ? Qt::Checked : Qt::Unchecked);
	ui.border->setCheckState(b ? Qt::Checked : Qt::Unchecked);

}
void FPartition::showDoubtP(bool is) {
	ui.glArea->setDivDoubtP(is);
}
void FPartition::showALOA(bool is) {
	ui.glArea->setALOA(is);
}
void FPartition::showBorder(bool is) {
	ui.glArea->setDivReg(is);
}
#pragma endregion
#pragma region 着色器
QString FPartition::vertexShaderFilename() const {
	return shaderPath + "basic.vert.glsl";
}
QString FPartition::fragmentShaderFilename() const {

	return shaderPath + "basic.frag.glsl";
}
#pragma endregion
#pragma region 初始化
void FPartition::helpF() {
	ui.glArea->setDivSLSimplify(false);	//简化流线
	ui.glArea->setDivStreamLine(false);	//流线
	ui.border->setCheckState(Qt::Unchecked);
	ui.glArea->setDivReg(false);			//边界
	ui.showStreamLine->setCheckState(Qt::Unchecked);
	ui.glArea->setSTL1(false);
	ui.TSL1->setCheckState(Qt::Unchecked);
	ui.glArea->setSTL2(false);
	ui.TSL2->setCheckState(Qt::Unchecked);
	ui.glArea->setCreateFourValueMesh(false);
	ui.Qua->setCheckState(Qt::Unchecked);
	ui.glArea->setDivSLSimplify(false);
	ui.SSL->setCheckState(Qt::Unchecked);
	ui.glArea->setDivSingular(false);		//奇点
	ui.showSingular->setCheckState(Qt::Unchecked);
	ui.glArea->setDivDoubtP(false);		//极点
	ui.doubtP->setCheckState(Qt::Unchecked);
	ui.glArea->setALOA(false);				//层域
	ui.aloa->setCheckState(Qt::Unchecked);
	ui.glArea->setDisQuaMesh(false);			//距离场四边形
	ui.disBgGrid->setCheckState(Qt::Unchecked);
	ui.glArea->setDisField(false);
	ui.disField->setCheckState(Qt::Unchecked);
	ui.glArea->setDivDisSL(false);
	ui.slDis->setCheckState(Qt::Unchecked);
	ui.glArea->setDifQuaMesh(false);			//差分场四边形
	ui.difBgGrid->setCheckState(Qt::Unchecked);
	ui.glArea->setDifField(false);
	ui.difField->setCheckState(Qt::Unchecked);
	ui.glArea->setDivDifSL(false);
	ui.slDif->setCheckState(Qt::Unchecked);
	ui.glArea->setDivVSL(false);
	ui.slValue->setCheckState(Qt::Unchecked);
	ui.glArea->setAcrossField(false);
	ui.crossField->setCheckState(Qt::Unchecked);
	ui.glArea->setDirectField(false);
	ui.directField->setCheckState(Qt::Unchecked);
	ui.glArea->setScalarField(false);
	ui.scalarField->setCheckState(Qt::Unchecked);
	ui.disValue_up->setEnabled(false);
	ui.disValue_down->setEnabled(false);
	ui.disOK->setEnabled(false);
	ui.disSpec->setEnabled(false);

	ui.difValue_up->setEnabled(false);
	ui.difValue_down->setEnabled(false);
	ui.difOK->setEnabled(false);
	ui.difSpec->setEnabled(false);
	ui.quaGP->clear();
	ui.quaGP->setEnabled(false);
	ui.quaGenOK->setEnabled(false);
	ui.quaValue_down->setEnabled(false);
	ui.quaValue_up->setEnabled(false);
	ui.quaMeshInfo->setText(QString(""));

	ui.lls->setText(QString(""));
	ui.lls->setEnabled(false);

	ui.coordX->setText(QString(""));
	ui.coordY->setText(QString(""));
	ui.coordZ->setText(QString(""));
	ui.coordSpe->setText(QString(""));
	ui.iterNum->setEnabled(false);
	ui.iterNum->setText(QString(""));

	ui.croBSize->setText(QString(""));
	ui.croLNum->setText(QString(""));
	ui.croLPd->setText(QString(""));

	ui.dirBSize->setText(QString(""));
	ui.dirHSize->setText(QString(""));
	ui.dirLNum->setText(QString(""));
	ui.dirLPd->setText(QString(""));
	if (ui.sslC1->isChecked()) {
		ui.sslC2->setChecked(true);
	}
	if (ui.quaC1->isChecked()) {
		ui.quaC2->setChecked(true);
	}
	ui.sslw->setText(QString(""));
	ui.quaSmo->setCheckState(Qt::Unchecked);

	ui.singularInfo->setText("");
}
void FPartition::fillBoxes() {

	QStringList files3 = listFiles("", "vtk");//获得资源列表	
	for (int i = 0; i < files3.size(); i++) {
		files3[i] = QFileInfo(files3[i]).baseName();
	}
	files3.removeDuplicates();
	for (int i = 0; i < files3.size(); i++) {
		ui.comboBox->addItem(files3[i]);//datasetpick控件添加选项
	}
	//此处txt用于临时
	QStringList files1 = listFiles("", "txt");//获得资源列表	
	for (int i = 0; i < files1.size(); i++) {
		files1[i] = QFileInfo(files1[i]).baseName();
	}
	files1.removeDuplicates();//去掉重复项目		
	for (int i = 0; i < files1.size(); i++) {
		ui.comboBox->addItem(files1[i]);//datasetpick控件添加选项
	}

	ui.comboBox->setCurrentIndex(0);//初始化选择空

	QStringList files2 = listFiles("", "txt");//获得资源列表
	for (int i = 0; i < files2.size(); i++) {
		files2[i] = QFileInfo(files2[i]).baseName();
	}
	files2.removeDuplicates();//去掉重复项目
	for (int i = 0; i < files2.size(); i++) {
		ui.disComboBox->addItem(files2[i]);//datasetpick控件添加选项
	}
	ui.disComboBox->setCurrentIndex(0);//初始化选择空
}
QStringList FPartition::listFiles(QString basename, QString extension) {
	QStringList masks, res;
	masks << (basename + "." + extension);
	masks << (basename + "*." + extension);
	qDebug("looking in '%s' for %s...", dataPath.toStdString().data(), masks.join(',').toStdString().data());
	QDirIterator dirite(dataPath, masks, QDir::Files, QDirIterator::NoIteratorFlags);

	while (dirite.hasNext()) {
		QString fn = dirite.next();
		qDebug("Found '%s'", fn.toStdString().data());
		res.push_back(fn);
	}
	res.sort();
	return res;
}

void FPartition::batch() {
	std::vector<QString> folders;
	GetAllFileFolder("D:\\Projects\\fieldGen", folders);
	std::vector<QStringList> files;
	for (int i = 0; i < folders.size(); ++i) {
		if(folders[i].contains(QString("aug_")))
		    files.push_back(getFileNames(folders[i]));
	}
	
	for (int i = 0; i < files.size(); ++i) {
		QStringList sl = files[i];
		for (int j = 0; j < sl.size(); ++j) {
			QString tmp = sl.at(j);
			QStringList outFilePath;
			QString out1 = folders[i]+"_out" ;
			QString out2 = tmp.split(".")[0] + ".txt";
			outFilePath.append(out1);
			outFilePath.append(out2);
			if (ui.glArea->loadVTKMesh(folders[i] +"/"+tmp, tmp)) {
				Vec3 coord = ui.glArea->loadHalfEdge();
				ui.glArea->setDivSingular(true);
				ui.glArea->setDivStreamLine(true);
				ui.glArea->setDivSLSimplify(true);
				ui.glArea->setLLSSpec(4.0);
				ui.glArea->getFacesStreamLineGoThrough(true);
				ui.glArea->outputFiles("CFL", outFilePath);
			}
		}
		
	}
	
}

void FPartition::normClass()
{
	ui.glArea->getNormClass();
}

void FPartition::highLightCrossField()
{
	QString fileName = QFileDialog::getOpenFileName(this, "Open File", "", "Text File(*.txt)");
	ui.glArea->highLightCrossField(fileName);
}

static QStringList getFileNames(const QString &path)
{
	QDir dir(path);
	QStringList nameFilters;
	nameFilters << "*.vtk";
	QStringList files = dir.entryList(nameFilters, QDir::Files | QDir::Readable, QDir::Name);
	return files;
}
static void GetAllFileFolder(QString dirPath, std::vector<QString> &folder)

{

	QDir dir(dirPath);

	dir.setFilter(QDir::Dirs);

	foreach(QFileInfo fullDir, dir.entryInfoList())

	{

		if (fullDir.fileName() == "." || fullDir.fileName() == "..") continue;

		folder.push_back(fullDir.absoluteFilePath());

		// GetAllFileFolder(fullDir.absoluteFilePath(), folder);

	}

}

static bool fixFileColorInfo(QString basename, Vec3 color) {
	QString strAll;
	QStringList strList;
	QFile qf(basename);
	if (qf.open(QIODevice::ReadOnly | QIODevice::Text)) {
		QTextStream stream(&qf);
		strAll = stream.readAll();
	}
	else {
		QMessageBox::warning(NULL, "warning", "can't open fixFile(fixFileColorInfo) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return false;
	}
	qf.close();
	QFile writeFile(basename);
	if (writeFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
		QTextStream stream(&writeFile);
		strList = strAll.split("\n");
		for (int i = 0; i < strList.count(); i++) {
			if (i == strList.count() - 1) {
				stream << strList.at(i);
			}
			else {
				stream << strList.at(i) << '\n';
			}
			if (strList.at(i).contains("{")) {
				QString tempStr = strList.at(++i);
				tempStr.replace(0, tempStr.length(), "	vec3 color = vec3(" + QString::number(color[0]) + "," + QString::number(color[1]) + "," + QString::number(color[2]) + ");");
				stream << tempStr << '\n';
			}
		}
	}
	writeFile.close();
	return true;
}
#pragma endregion


