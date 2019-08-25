#include<GL\glew.h>
#include <QMetaEnum>
#include "GLArea.h"
#include<qtimer.h>
#include<time.h>
#include<sys\timeb.h>
#define WHITE_BACKGROUND 1
#define ATTR_POINTER(NAME,NUM)
#define glEnableVertexAttribArray GLEW_GET_FUN(__glewEnableVertexAttribArray)

GLArea::GLArea(QWidget * parent) :QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::AlphaChannel), parent)
{
	resetTrackball();
	Init();
}
void GLArea::Init() {
	showWireframe = false;
	showDivReg = false;
	showSingular = false;
	showStreamLine = false;
	showSLSimplify = false;
	showDoubtP = false;
	select_dimension = false;
	showALOA = false;
	showDisQuaMesh = false;
	showDisField = false;
	showDisSL = false;
	showValueSL = false;
	showDifSL = false;
	showDifField = false;
	showDifQuaMesh = false;
	showAcrossField = false;
	showDirectField = false;
	showScalarField = false;
	showDisMesh = false;
	showSLT = false;
	showQuaMesh = false;
	showQuaSSL = false;
	showCareQuaMesh = false;
	isShowQuaSSL = false;
	isSetSpec = false;
	simState = false;
	drawPState = false;
	careQuaSmooth = false;
	quasSLBTState = false;
	preGenQud = false;
	showModel = false;
	slInfoState = false;
	setCVP = false;
	saveMeshCount = 0;
	simCoordSpe = 1.0;
	sslColor = s_colour;
	quaColor = s_colour;
	curQBT = QuaBoundaryType::INLET;
	sslw = 4.0;
	time = 0.0;

	//is_pick_face = false;
	showPickedFace = false;
	kdTree = NULL;
}
void GLArea::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);//表示要清除颜色缓冲以及深度缓冲，
	setTrackballView();
	centerMesh(mesh);

	if (showSLT) {
		enableWireframe(true, wireShaderProgram);
		renderFourSV();
		enableWireframe(false, shaderProgramModel);
	}

	if (showDisField || showALOA || showDifField || showScalarField) {
		enableWireframe(true, wireShaderProgram);
		renderMesh(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showSingular) {
		enableWireframe(true, wireShaderProgram);
		renderSingular(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showStreamLine || showDisSL || showValueSL || showDifSL) {
		if (sslColor != s_colour) {
			enableWireframe(true, shaderProgramSSL);
		}
		else {
			enableWireframe(true, wireShaderProgram);
		}
		renderStreamLine(mesh);

		enableWireframe(false, shaderProgramModel);
	}
	if (showSLSimplify) {
		if (sslColor == s_colour) {
			enableWireframe(true, wireShaderProgram);
		}
		else {
			enableWireframe(true, shaderProgramSSL);
		}
		renderSSL(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showDoubtP) {
		enableWireframe(true, wireShaderProgram);
		renderDoubtP(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showDirectField) {
		enableWireframe(true, shaderProgramDirHC);
		renderHDirectField(mesh);
		enableWireframe(false, shaderProgramModel);
		enableWireframe(true, shaderProgramDirBC);
		renderBDirectField(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showAcrossField) {
		enableWireframe(true, shaderProgramCroB);
		renderAcrossField(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showDisQuaMesh) {
		enableWireframe(true, wireShaderProgram);
		renderDisQuaMesh(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showDifQuaMesh) {
		enableWireframe(true, wireShaderProgram);
		renderDifQuaMesh(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showDisMesh) {
		enableWireframe(true, wireShaderProgram);
		renderDisMeshField(disMesh);
		renderDisMeshBorder(disMesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showQuaSL) {
		enableWireframe(true, wireShaderProgram);
		renderQuaSL(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showQuaSSL) {
		enableWireframe(true, wireShaderProgram);
		renderQuaSSL(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showCareQuaMesh) {
		if (mesh.careQuaIndex.size() > 45e5) {

		}
		else {
			if (quaColor == s_colour) {
				enableWireframe(true, wireShaderProgram);
			}
			else {
				enableWireframe(true, shaderProgram);
			}
			renderCareQuaMesh(mesh);
			enableWireframe(false, shaderProgramModel);
		}
	}
	if (simState || quasSLBTState || preGenQud || slInfoState || setCVP) {
		enableWireframe(true, wireShaderProgram);
		renderCoord();
		enableWireframe(false, shaderProgramModel);
	}
	if (showDivReg) {
		enableWireframe(true, shaderProgramBorder);
		renderReg(mesh);
		enableWireframe(false, shaderProgramModel);
	}
	if (showWireframe) {
		enableWireframe(true, shaderProgram);
		renderOMesh();
		enableWireframe(false, shaderProgramModel);
	}

	/*if (is_pick_face) {
		enableWireframe(false, wireShaderProgram);
	    renderSelectedFace();
	}*/
	if (isHighLightCF) {
		enableWireframe(true, wireShaderProgram);
		highLightCF();
		enableWireframe(false, shaderProgramModel);
	}
	if (showPickedFace) {
		enableWireframe(false,customShaderProgram);
		renderSelectedFace();
	}
	if (showModel) {
		renderOMesh();
	}

}
void GLArea::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
	update();
}

void GLArea::renderSelectedFace() {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	GLuint loc = currentShader->uniformLocation("flag");

	if (mesh.vert.size() > 0 && mesh.face.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vert), mesh.vert[0].pos.v);
		glEnableVertexAttribArray(locOf_pos);
		for (int i = 0; i < mesh.face.size(); ++i) {
			if (mesh.halfFaces[i]->is_selected) {
			
				glUniform1iARB(loc, 1);
			}
			else glUniform1iARB(loc, 0);
			glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, &(mesh.face[i].index));
		}
	}
}
void GLArea::showCustomModel(bool b)
{
	showWireframe = !b;
	showPickedFace=b;

	update();
}
void GLArea::highLightCF()
{
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");

	if (mesh.highLightedCrossField.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), mesh.highLightedCrossField[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), mesh.highLightedCrossField[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glPointSize(5.0);
		glDrawArrays(GL_POINTS, 0, mesh.highLightedCrossField.size());
	}
}
#pragma region 图形绘制
//流线交点
void GLArea::renderFourSV() { 
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (mesh.fNode.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(fourNode), mesh.fNode[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(fourNode), mesh.fNode[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glPointSize(5.0);
		glDrawArrays(GL_POINTS, 0, mesh.fNode.size());
	}
}
//鼠标焦点
void GLArea::renderCoord() {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), curSimPos.pos.v);
	glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), curSimPos.color.v);
	glEnableVertexAttribArray(locOf_pos);
	glEnableVertexAttribArray(locOf_color);
	glPointSize(10.0);
	glDrawArrays(GL_POINTS, 0, 1);
}
//细四边形网格
void GLArea::renderCareQuaMesh(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.careQuaMeshP.size() > 0) {
		if (careQuaSmooth) {
			glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(vertAloa), m.careQuaMeshP[0].npos.v);
		}
		else {
			glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(vertAloa), m.careQuaMeshP[0].pos.v);
		}
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(vertAloa), m.careQuaMeshP[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glDrawElements(GL_QUADS, (int)m.careQuaIndex.size() * 4, GL_UNSIGNED_INT, m.careQuaIndex[0].v);
	}
}
//分解问题域的规范化*
void GLArea::renderQuaSSL(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.quaCareSLs.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), m.quaCareSLs[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), m.quaCareSLs[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glLineWidth(3.0);
		for (int i = 0; i < m.QuaSLInfo.size(); i++) {
			glDrawArrays(GL_LINE_STRIP, m.QuaSLInfo[i]->auxStartSLPI, m.QuaSLInfo[i]->auxEndSLPI - m.QuaSLInfo[i]->auxStartSLPI + 1);
		}
		//glDrawElements(GL_LINES, (int)m.showQuaSSLIndex.size() * 2, GL_UNSIGNED_INT, m.showQuaSSLIndex[0].v);
		glLineWidth(2.0);
	}
}
//分解问题域
void GLArea::renderQuaSL(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.quaSLs.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), m.quaSLs[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), m.quaSLs[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glLineWidth(3.0);
		for (int i = 0; i < m.QuaSLInfo.size(); i++) {
			glDrawArrays(GL_LINE_STRIP, m.QuaSLInfo[i]->startSLPointIndex, m.QuaSLInfo[i]->endSLPointIndex - m.QuaSLInfo[i]->startSLPointIndex + 1);
		}
		//glDrawElements(GL_LINES, (int)m.showQuaSLIndex.size() * 2, GL_UNSIGNED_INT, m.showQuaSLIndex[0].v);
		glLineWidth(2.0);
	}
}
//离散点四边形距离场
void GLArea::renderDisMeshQua(const DisMesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.vertP.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(QuaV), (*m.dfm).qvert[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(QuaV), (*m.dfm).qvert[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glDrawElements(GL_QUADS, (int)(*m.dfm).qface.size() * 4, GL_UNSIGNED_INT, &((*m.dfm).qface[0].cornerPI));
	}
}
//离散点距离场边界
void GLArea::renderDisMeshBorder(const DisMesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.vertP.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vec3), m.vertP[0].v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), m.color[0].v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glDrawElements(GL_LINES, (int)m.borderI.size() * 2, GL_UNSIGNED_INT, &m.borderI[0].v);
	}
}
//离散点距离场
void GLArea::renderDisMeshField(const DisMesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.vertP.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vec3), m.vertP[0].v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, 3 * sizeof(double), m.color[0].v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glDrawElements(GL_POINTS, (int)m.vertP.size(), GL_UNSIGNED_INT, &m.vI[0]);
	}
}
//简化后的流线
void GLArea::renderSSL(const Mesh &m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.showSSLS.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), m.showSSLS[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(PCPoint), m.showSSLS[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glLineWidth(sslw);
		for (int i = 0; i < m.SLSimplify.size(); i++) {
			glDrawArrays(GL_LINE_STRIP, m.SLSimplify[i]->startSLPointIndex, m.SLSimplify[i]->slpc);
		}
	}
}
//方向场
void GLArea::renderHDirectField(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	if (m.directF.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vec3), m.directF[0].v);
		glEnableVertexAttribArray(locOf_pos);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDrawArrays(GL_TRIANGLES, 0., m.directF.size());
	}
}
void GLArea::renderBDirectField(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	if (m.directF.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vec3), m.directHandleF[0].v);
		glEnableVertexAttribArray(locOf_pos);
		glDrawArrays(GL_LINES, 0., m.directHandleF.size());
	}
}
//交叉场
void GLArea::renderAcrossField(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	if (m.acrossF.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vec3), m.acrossF[0].v);
		glEnableVertexAttribArray(locOf_pos);
		glLineWidth(3.0);
		glDrawArrays(GL_LINES, 0, m.acrossF.size());
	}
}
//差分四边形网格
void GLArea::renderDifQuaMesh(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.tfm != nullptr) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(QuaV), (*m.tfm).qvert[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(QuaV), (*m.tfm).qvert[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glDrawElements(GL_QUADS, (int)(*m.tfm).qface.size() * 4, GL_UNSIGNED_INT, &((*m.tfm).qface[0].cornerPI));
	}
}
//距离四边形网格
void GLArea::renderDisQuaMesh(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.dfm != nullptr) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(QuaV), (*m.dfm).qvert[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(QuaV), (*m.dfm).qvert[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glDrawElements(GL_QUADS, (int)(*m.dfm).qface.size() * 4, GL_UNSIGNED_INT, &((*m.dfm).qface[0].cornerPI));
	}
}
void GLArea::enableWireframe(bool on, QGLShaderProgram& shader) {
	if (on) {
		glLineWidth(2.);
		//只有在RGBA模式下，才可以使用混合功能，颜色索引模式下是无法使用混合功能的。
		//禁用反走样
		//glEnable(GL_LINE_SMOOTH);//启用了反走样处理得到不同的效果。启用线段的反走样处理必须使用
		glDisable(GL_BLEND);//关闭OpenGL的混合功能
							//glEnable(GL_BLEND);//使用OpenGL的混合功能
		glBlendFunc(GL_ONE, GL_ZERO);//前者表示源因子，后者表示目标因子
									 //表示使用源颜色的alpha值来作为因子。表示用1.0减去源颜色的alpha值来作为因子。
									 //glEnable(GL_POLYGON_OFFSET_LINE);
									 //glEnable(GL_POLYGON_OFFSET_FILL);//启用多边形偏移
									 //glPolygonOffset(-1, -1);//计算偏移值
									 //glPolygonOffset(3,3);


									 //glEnable(GL_POLYGON_OFFSET_LINE);
									 //glPolygonOffset(-1,-1);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		currentShader = &shader;
	}
	else {
		glDisable(GL_BLEND);//关闭OpenGL的混合功能
		glBlendFunc(GL_ONE, GL_ZERO);


		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);//设置当前的多边形光栅化方法
		glDisable(GL_POLYGON_OFFSET_FILL);

		currentShader = &shader;
	}
}
//极点
void GLArea::renderDoubtP(const Mesh& m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.polePs.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(polePoint), m.polePs[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(polePoint), m.polePs[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glPointSize(10.0);
		glDrawArrays(GL_POINTS, 0, m.polePs.size());
	}
}
//流线
void GLArea::renderStreamLine(const Mesh &m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.dirSL.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(SLPoint), mesh.dirSL[0].pos.v);
		glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(SLPoint), mesh.dirSL[0].color.v);
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glLineWidth(3.0);
		for (int i = 0; i < m.SLS.size(); i++) {
			glDrawArrays(GL_LINE_STRIP, m.SLS[i]->startSLPointIndex, m.SLS[i]->slpc);
		}
	}
}
//奇点
void GLArea::renderSingular(const Mesh &m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (m.dirSV.size() > 0) {
		glPointSize(5.0);
		for (int i = 0; i < mesh.dirSV.size(); i++) {
			//if (mesh.dirSV[i]->faceI >= 0) {
			glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Singular), mesh.dirSV[i]->pos.v);
			glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(Singular), mesh.dirSV[i]->color.v);
			glEnableVertexAttribArray(locOf_pos);
			glEnableVertexAttribArray(locOf_color);
			glDrawArrays(GL_POINTS, 0, 1);
			//}
		}
	}
}
//边界->中轴线
void GLArea::renderReg(const Mesh &m) {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	if (m.vert.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vert), m.vert[0].pos.v);
		glEnableVertexAttribArray(locOf_pos);
		glLineWidth(3.0);
		glDrawElements(GL_LINES, (int)m.dirBVEdgeIndex.size() * 2, GL_UNSIGNED_INT, m.dirBVEdgeIndex[0].v);
	}
}
//网格
void GLArea::renderOMesh() {
	currentShader->bind();
	int locOf_pos = currentShader->attributeLocation("pos");
	if (mesh.vert.size() > 0 && mesh.face.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vert), mesh.vert[0].pos.v);
		glEnableVertexAttribArray(locOf_pos);
		glDrawElements(GL_TRIANGLES, (int)mesh.face.size() * 3, GL_UNSIGNED_INT, &(mesh.face[0].index));
	}
}
void GLArea::renderMesh(const Mesh& m) {
	currentShader->bind();
	//int locOf_norm = currentShader->attributeLocation("norm");
	int locOf_pos = currentShader->attributeLocation("pos");
	int locOf_color = currentShader->attributeLocation("c_color");
	if (mesh.vert.size() > 0 && mesh.face.size() > 0) {
		glVertexAttribPointerARB(locOf_pos, 3, GL_DOUBLE, GL_FALSE, sizeof(Vert), mesh.vert[0].pos.v);
		if (showALOA) {
			glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(Vert), &mesh.vert[0].aloaColor.v);
		}
		else if (showDifField) {
			glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(Vert), &mesh.vert[0].difColor.v);
		}
		else if (showDisField) {
			glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(Vert), &mesh.vert[0].disColor.v);
		}
		else if (showScalarField) {
			glVertexAttribPointerARB(locOf_color, 3, GL_DOUBLE, GL_FALSE, sizeof(Vert), &mesh.vert[0].normColor.v);
		}
		glEnableVertexAttribArray(locOf_pos);
		glEnableVertexAttribArray(locOf_color);
		glDrawElements(GL_TRIANGLES, (int)mesh.face.size() * 3, GL_UNSIGNED_INT, &(mesh.face[0].index));
	}
}
#pragma endregion
void GLArea::centerMesh(const Mesh &m) {
	double k = 1.0 / m.boxSize;
	glScalef(k, k, k);
	glTranslatef(-m.boxCenter.v[0], -m.boxCenter.v[1] - 1.0, -m.boxCenter.v[2]);
}
//窗口坐标转模型坐标
bool GLArea::Pick(const int & x, const int &y, Vec3 &pp)
{
	GLint viewport[4];
	GLdouble mvmatrix[16], projmatrix[16];
	GLfloat winx, winy, winz;
	glPushMatrix();
	//glScalef(0.1, 0.1, 0.1);  
	glGetIntegerv(GL_VIEWPORT, viewport);   /* 获取三个矩阵 */
	glGetDoublev(GL_MODELVIEW_MATRIX, mvmatrix);
	glGetDoublev(GL_PROJECTION_MATRIX, projmatrix);

	glPopMatrix();

	winx = x;
	winy = viewport[3] - y;

	glReadPixels((int)winx, (int)winy, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winz);   /* 获取深度 */

	GLfloat depthrange[2] = { 0,0 };
	glGetFloatv(GL_DEPTH_RANGE, depthrange);
	if (winz == depthrange[1]) {
		return false;
	}
	gluUnProject(winx, winy, winz, mvmatrix, projmatrix, viewport, &pp[0], &pp[1], &pp[2]); /* 获取三维坐标 */
	return true;
}
#pragma region 鼠标和键盘操作
void GLArea::mouseReleaseEvent(QMouseEvent* e) {
	/*if (quasSLBTState) {
	Vec3 endP;
	Vec3 startP;
	if (Pick(e->pos().x(), e->pos().y(), endP) && Pick(lastMousePos.x(), lastMousePos.y(), startP)) {
	endP[1] -= 1.0;
	startP[1] -= 1.0;
	if ((startP - endP).norm() > interDim) {
	QMessageBox::about(this, "border-type", "<font color='red'>" + QString::number(mesh.setQBTSL(startP, endP)) + "</font>");
	}
	}
	}*/
}
void GLArea::mousePressEvent(QMouseEvent * e)
{
	lastMousePos = e->pos();
	e->accept();
	if (simState && mesh.isGenStreamLine && !mesh.isGenSSL && showStreamLine) {
		if (Pick(e->pos().x(), e->pos().y(), curSimPos.pos)) {
			//curSimPos.pos -= mesh.boxCenter;
			curSimPos.pos[1] -= 1.0;
			mesh.selectStreamLine(curSimPos.pos);
			emit setCoord(curSimPos.pos, simCoordSpe, simState);
		}
	}
	else if (quasSLBTState && mesh.isFourMesh && showQuaSL) {
		if (Pick(e->pos().x(), e->pos().y(), curSimPos.pos)) {
			//curSimPos.pos -= mesh.boxCenter;
			curSimPos.pos[1] -= 1.0;
			QMessageBox::about(this, "border-info", "<font color='red'>" + mesh.setQBTSL(curSimPos.pos, curQBT) + "</font>");
			emit setCoord(curSimPos.pos, simCoordSpe, quasSLBTState);
		}
	}
	else if (preGenQud && mesh.isGenQua && showQuaSL) {
		if (Pick(e->pos().x(), e->pos().y(), curSimPos.pos)) {
			//curSimPos.pos -= mesh.boxCenter;
			curSimPos.pos[1] -= 1.0;
			QMessageBox::about(this, "border-segment", "<font color='red'>" + QString::number(mesh.setPGQC(curSimPos.pos)) + "</font>");
			emit setCoord(curSimPos.pos, simCoordSpe, preGenQud);
		}
	}
	else if (slInfoState && mesh.isGenStreamLine && showStreamLine) {
		if (Pick(e->pos().x(), e->pos().y(), curSimPos.pos)) {
			//curSimPos.pos -= mesh.boxCenter;
			curSimPos.pos[1] -= 1.0;
			QMessageBox::about(this, "border-segment", "<font color='red'>" + mesh.getSLInfoFP(curSimPos.pos) + "</font>");
			emit setCoord(curSimPos.pos, simCoordSpe, slInfoState);
		}
	}
	else if (setCVP && mesh.isloadVTK && !mesh.isGenSingular) {
		if (Pick(e->pos().x(), e->pos().y(), curSimPos.pos)) {
			curSimPos.pos[1] -= 1.0;
			QMessageBox::about(this, "border-singular", "<font color='red'>" + mesh.setBCV(curSimPos.pos) + "</font>");
			emit setCoord(curSimPos.pos, simCoordSpe, setCVP);
		}
	}
	/*else if (is_pick_face == true) {
		if (Pick(e->pos().x(), e->pos().y(), curSimPos.pos)) {
			curSimPos.pos[1] -= 1.0;
			pick_face();
		}
	}*/
	setFocus();
	update();
}
void GLArea::mouseMoveEvent(QMouseEvent * e)
{
	QPointF dp = e->pos() - lastMousePos;
	lastMousePos = e->pos();
	if (!select_dimension) {
		if (drawPState) {
			curSimPos.pos[0] += dp.rx() / 500.0;
			curSimPos.pos[1] -= dp.ry() / 500.0;
			emit setCoord(curSimPos.pos, simCoordSpe, simState);
		}
		else {
			eyePhi += dp.x()*eyeDist;
			eyeTheta -= dp.y()*eyeDist;
		}
	}
	else {
		if (e->buttons() & Qt::LeftButton) {
			eyePhi += dp.x()*eyeDist;
			eyeTheta += dp.y()*eyeDist;
		}
		else if (e->buttons() & Qt::RightButton) {
			centerPos.v[0] += dp.rx() / 1500.0;
			centerPos.v[1] -= dp.ry() / 1500.0;
		}
	}
	update();
}
void GLArea::wheelEvent(QWheelEvent * e)
{
	/* wheel ==> zoom */
	if (e->delta() > 0) eyeDist *= 1.01f;
	else eyeDist /= 1.01f;
	if (!select_dimension) {
		/*if (eyeDist > 11.0) eyeDist = 11.0;
		if (eyeDist < 0.001f) eyeDist = 0.001f;*/
	}
	else {
		if (eyeDist > 5) eyeDist = 5;
		if (eyeDist < 0.5f) eyeDist = 0.5f;
	}
	update();
}
void GLArea::keyPressEvent(QKeyEvent * e)
{
	if (e->key() == Qt::Key_Space) {
		select_dimension = !select_dimension;
		resetTrackball();
		update();
	}
	else if ((e->modifiers() == Qt::ControlModifier) && (e->key() == Qt::Key_A))
	{
		QMessageBox::aboutQt(NULL, "aboutQt");
	}
	else if (e->key() == Qt::Key_T && mesh.isloadTXT) {
		if (!mesh.genSL && !mesh.isloadVTK)
			showTempSL();
		else {
			showStreamLine = !showStreamLine;
		}
		update();
	}
	else if (e->key() == Qt::Key_L && mesh.isloadVTK && mesh.isGenSSL) {
		showSLT = !showSLT;
		setCreateFourValueMesh(showSLT);
	}
	else if (e->key() == Qt::Key_P && mesh.isGenQua && showQuaSL) {
		if (e->modifiers() == Qt::ControlModifier) {
			preGenQud = !preGenQud;
			if (preGenQud) {
				simState = false;
				quasSLBTState = false;
				slInfoState = false;
				setCVP = false;
			}
		}
		else {
			setPreGenQua();
			/*setQMesh(true);
			careQuaSmooth = true;*/
		}
	}
	else if (e->key() == Qt::Key_E && mesh.isGenStreamLine && showStreamLine) {
		slInfoState = !slInfoState;
		if (slInfoState) {
			preGenQud = false;
			quasSLBTState = false;
			simState = false;
			setCVP = false;
		}
	}
	else if (e->key() == Qt::Key_R && mesh.isGenQua) {
		showQuaSL = !showQuaSL;
		setSTL1(showQuaSL);
	}
	else if (e->key() == Qt::Key_Q && mesh.isFinish) {
		//四边形网格生成
		showQuaSSL = !showQuaSSL;
		setSTL2(showQuaSSL);
	}
	else if (e->key() == Qt::Key_F && mesh.isFourMesh) {
		showCareQuaMesh = !showCareQuaMesh;
		setQMesh(showCareQuaMesh);
	}
	else if (simState && (e->key() == Qt::Key_Up || e->key() == Qt::Key_Down || e->key() == Qt::Key_Left || e->key() == Qt::Key_Right)) {
		switch (e->key())
		{
		case Qt::Key_Up:
			curSimPos.pos[1] += simCoordSpe;
			break;
		case Qt::Key_Down:
			curSimPos.pos[1] -= simCoordSpe;
			break;
		case Qt::Key_Left:
			curSimPos.pos[0] -= simCoordSpe;
			break;
		case Qt::Key_Right:
			curSimPos.pos[0] += simCoordSpe;
			break;
		default:
			break;
		}
		emit setCoord(curSimPos.pos, simCoordSpe, simState);
	}
	else if (e->key() == Qt::Key_W && mesh.isGenStreamLine && showStreamLine) {
		simState = !simState;
		if (simState) {
			quasSLBTState = false;
			preGenQud = false;
			slInfoState = false;
			setCVP = false;
		}
		if (simState && mesh.isGenStreamLine) {
			mesh.ReduceSLToArrayClear();
			showSLSimplify = showCareQuaMesh = showQuaSSL = showQuaSL = showSLT = false;
			setDivSingular(true);
			showStreamLine = true;
			setQuaMeshGPState(false, 0.0);
			emit setSLState(showDifSL, showValueSL, showDisSL, showStreamLine, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
		}
		emit setCoord(curSimPos.pos, simCoordSpe, simState);
	}
	else if (e->key() == Qt::Key_A && simState && mesh.isGenStreamLine) {
		mesh.selectStreamLine(curSimPos.pos);
	}
	else if (e->key() == Qt::Key_Y  && mesh.isSelectOK() && !mesh.isGenSSL) {
		mesh.selectSimSL();
	}
	else if (e->key() == Qt::Key_D) {
		drawPState = !drawPState;
		setDrawPState(drawPState);
	}
	else if (e->key() == Qt::Key_U && mesh.isFourMesh && showQuaSL) {
		if (e->modifiers() == Qt::ControlModifier) {
			bool isOK = false;
			QString text = QInputDialog::getText(NULL, "Input Dialog",
				"inlet-outlet-wall0-wall1",
				QLineEdit::Normal,
				"",
				&isOK);
			if (isOK) {
				curQBT = getQBTFQS(text);
				QMessageBox::about(this, "border-Type", "<font color='red'>" + QString::number(int(curQBT)) + "</font>");
			}
		}
		quasSLBTState = !quasSLBTState;
		if (quasSLBTState) {
			simState = false;
			preGenQud = false;
			slInfoState = false;
			setCVP = false;
		}
		emit setCoord(curSimPos.pos, simCoordSpe, simState);
	}
	else if (e->key() == Qt::Key_M) {
		showModel = !showModel;
	}
	else if (mesh.isloadTXT || mesh.isloadVTK) {
		switch (e->key())
		{
		case Qt::Key_S:
			showStreamLine = !showStreamLine;
			setDivStreamLine(showStreamLine);
			emit notifyMeshInfo(showSLT || showWireframe || showDisField || showALOA, (int)mesh.vert.size(), (int)mesh.face.size());
			break;
		case Qt::Key_C:
			showSLSimplify = !showSLSimplify;
			setDivSLSimplify(showSLSimplify);
			break;
		case Qt::Key_B:
			showDivReg = !showDivReg;
			setDivReg(showDivReg);
			break;
		case Qt::Key_V:
			if (e->modifiers() == Qt::ControlModifier) {
				setCVP = !setCVP;
				if (setCVP) {
					simState = false;
					preGenQud = false;
					slInfoState = false;
					quasSLBTState = false;
				}
			}
			else {
				showSingular = !showSingular;
				setDivSingular(showSingular);
			}
			break;
		case Qt::Key_G:
			showWireframe = !showWireframe;
			setWireframeMode(showWireframe);
			break;
		default:
			break;
		}
		emit setGSBState(showWireframe, showSingular, showDivReg);
	}
	update();
}
#pragma endregion
//着色器
bool GLArea::loadShaders(QString vertFileName, QString fragFileName, showColor ct) {
	makeCurrent();
	switch (ct)
	{
	case s_black:
		shaderProgram.removeAllShaders();
		shaderProgram.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		shaderProgram.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		shaderProgram.link();
		shaderProgram.bind();
		break;
	case s_colour:
		wireShaderProgram.removeAllShaders();
		//将shader写好在文件中，并为该函数指明文件的路径，实现添加着色器。
		wireShaderProgram.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		wireShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		wireShaderProgram.link();
		wireShaderProgram.bind();
		break;
	case s_blue:
		shaderProgramB.removeAllShaders();
		shaderProgramB.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		shaderProgramB.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		shaderProgramB.link();
		shaderProgramB.bind();
		break;
	case croB_color:
		shaderProgramCroB.removeAllShaders();
		shaderProgramCroB.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		shaderProgramCroB.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		shaderProgramCroB.link();
		shaderProgramCroB.bind();
		break;
	case dirB_color:
		shaderProgramDirBC.removeAllShaders();
		shaderProgramDirBC.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		shaderProgramDirBC.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		shaderProgramDirBC.link();
		shaderProgramDirBC.bind();
		break;
	case dirH_color:
		shaderProgramDirHC.removeAllShaders();
		shaderProgramDirHC.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		shaderProgramDirHC.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		shaderProgramDirHC.link();
		shaderProgramDirHC.bind();
		break;
	case pure_c:
		shaderProgramSSL.removeAllShaders();
		shaderProgramSSL.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		shaderProgramSSL.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		shaderProgramSSL.link();
		shaderProgramSSL.bind();
		break;
	case border_color:
		shaderProgramBorder.removeAllShaders();
		shaderProgramBorder.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		shaderProgramBorder.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		shaderProgramBorder.link();
		shaderProgramBorder.bind();
		break;
	case model_color:
		shaderProgramModel.removeAllShaders();
		shaderProgramModel.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		shaderProgramModel.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		shaderProgramModel.link();
		shaderProgramModel.bind();
		break;
	case custom_shader:
		customShaderProgram.removeAllShaders();
		customShaderProgram.addShaderFromSourceFile(QGLShader::Vertex, vertFileName);
		customShaderProgram.addShaderFromSourceFile(QGLShader::Fragment, fragFileName);
		customShaderProgram.link();
		customShaderProgram.bind();
	default:
		break;
	}
	update();
	return true;
}
//加载模型
bool GLArea::loadDisMesh(QString fileName, QString name) {
	bool isLoadTXT = disMesh.loadTXT(fileName.toLocal8Bit().data());
	update();
	return isLoadTXT;
}
bool GLArea::loadTXTMesh(QString filename, QString name) {
	mesh.clear();
	Init();
	qDebug("Loading mesh '%s'", filename.toLocal8Bit().data());
	mesh.loadTXT(filename.toLocal8Bit().data());
	if (!mesh.isloadTXT) {
		return false;
	}
	update();
	return true;
}
bool GLArea::loadVTKMesh(QString filename, QString name) {
	mesh.clear();
	Init();
	qDebug("Loading mesh '%s'", filename.toLocal8Bit().data());
	mesh.loadVTK(filename.toLocal8Bit().data());
	if (!mesh.isloadVTK) {
		return false;
	}
	update();
	return true;
}
//建立半边结构
Vec3 GLArea::loadHalfEdge() {
	mesh.GenHalfStruct();
	mesh.isGenRegDivToS = mesh.GenRegDiv();
	update();
	curSimPos.pos = mesh.boxCenter;
	return mesh.boxCenter;
}
//窗口初始化
void GLArea::initializeGL() {
	glewInit();
#if WHITE_BACKGROUND
	glClearColor(1, 1, 1, 1);
#else
	glClearColor(1, 1, 1, 1);
#endif
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_MULTISAMPLE);

	enableWireframe(true, wireShaderProgram);

	QString res;
	qDebug(res.toLocal8Bit().data());
}
int GLArea::widthPix() const {
	return width() * windowHandle()->devicePixelRatio();
}
int GLArea::heightPix() const {
	return height() * windowHandle()->devicePixelRatio();
}
void GLArea::resetTrackball() {
	eyeDist = 2.0;
	eyePhi = 0.0;
	eyeTheta = 0.0;
	centerPos = Vec3();
	curSimPos.pos = mesh.boxCenter;
}
// substitute gluPerspective (to avoid a dependency)
static void myGluPerspective(GLdouble fovx, GLdouble aspect, GLdouble zNear, GLdouble zFar)
{
	GLdouble xMin, xMax, yMin, yMax;

	xMax = zNear * tan(fovx * 3.1415926535 / 360.0);
	xMin = -xMax;

	yMin = xMin / aspect;
	yMax = xMax / aspect;

	GLdouble m[16] = {
		(2.0 * zNear) / (xMax - xMin), 0, 0, 0,

		0, (2.0 * zNear) / (yMax - yMin), 0, 0,

		(xMax + xMin) / (xMax - xMin),
		(yMax + yMin) / (yMax - yMin),
		-(zFar + zNear) / (zFar - zNear),
		-1,

		0, 0, -(2.0 * zFar * zNear) / (zFar - zNear), 0
	};

	glMultMatrixd(m);
}
void GLArea::setTrackballView() {
	// projection matrix
	glMatrixMode(GL_PROJECTION); //对投影矩阵应用随后的矩阵操作.
	glLoadIdentity();//该函数的功能是重置当前指定的矩阵为单位矩阵.等于是之前的矩阵变换带来的影响到此为止了！
	int w = widthPix(), h = heightPix();
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);//它负责把视景体截取的图像按照怎样的高和宽显示到屏幕上。
	myGluPerspective(30, w / (double)h, 0.001, 1000);
	glMatrixMode(GL_MODELVIEW);//模型视图  GL_TEXTURE 纹理.	
							   // view matrix
	glLoadIdentity();
	if (!select_dimension) {
		glTranslatef(eyePhi / 1500.0, eyeTheta / 1500.0, -eyeDist);
	}
	else {
		glTranslatef(centerPos.v[0], centerPos.v[1], -eyeDist);
		glRotatef(eyeTheta, 1, 0, 0);
		glRotatef(eyePhi, 0, 1, 0);
		//if (flipView) glRotatef(180, 0, 0, 1);  // small hack, some mesh is upside down
	}
}
//模型数据信息
void GLArea::setWireframeMode(bool on) {
	showWireframe = on;
	if (mesh.isloadVTK || mesh.isloadTXT) {
		emit notifyMeshInfo(showSLT || showWireframe || showDisField || showALOA, (int)mesh.vert.size(), (int)mesh.face.size());
	}
	else {
		showWireframe = false;
	}
	update();
}
//模型网格边界显示设置
void GLArea::setDivReg(bool on) {
	showDivReg = on;
	update();
}
//奇点生成及显示
void GLArea::setDivSingular(bool on) {
	showSingular = on;
	if (on && !mesh.isGenSingular) {
		mesh.findSingular();
		if (!mesh.isGenSingular) {
			showSingular = false;
		}
		else {
			showWireframe = false;
			showDivReg = true;
			emit setMeshShowState();
		}
	}
	emit notifySingularInfo(showSingular, mesh.getSVCount());
	update();
}
//流线生成及显示
void GLArea::setDivStreamLine(bool on) {
	showStreamLine = on;
	if (on && !mesh.uSL && !mesh.isloadTXT && mesh.isGenSingular) {
		mesh.slt = u_sl;
		auxFun();
		//mesh.StreamLineClear();
		QTime time;
		time.start();
		mesh.getStreamLines();
		qDebug() << "geographic division:" << time.elapsed() / 1000.0 << "s";
		if (!mesh.isGenStreamLine) {
			showStreamLine = false;
			mesh.uSL = false;
		}
		else {
			showWireframe = false;
			showDivReg = true;
			emit setMeshShowState();
			mesh.isGenSSL = false;
			showSLSimplify = false;
		}
	}
	emit setSLState(false, false, false, showStreamLine && mesh.isGenStreamLine, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
//流线结构简化及显示
void GLArea::setDivSLSimplify(bool on) {
	if (mesh.isGenStreamLine) {
		if (on && !mesh.isGenSSL) {
			//SimplifyStreamLine
			QTime time;
			time.start();
			//mesh.ReduceSLToArray();
			mesh.ReduceSLToArrayI();
			qDebug() << "DivSLSimplify: " << time.elapsed() / 1000.0 << "s";
			if (!mesh.isGenSSL) {
				showSLSimplify = false;
			}
			showCareQuaMesh = showQuaSSL = showQuaSL = showSLT = false;
		}
		if (on && mesh.isGenSSL && !showQuaSL && !showQuaSSL) {
			InitSLState();
			emit setLLSState(mesh.getLLSSpec());
		}
		showSLSimplify = on;
	}
	else {
		showSLSimplify = false;
	}
	emit setSLState(showDifSL, showValueSL, showDisSL, showStreamLine, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
//极点生成及显示
void GLArea::setDivDoubtP(bool on) {
	showDoubtP = on;
	if (on && !mesh.isGenPole) {
		mesh.getPolePoint();
		if (!mesh.isGenPole) {
			showDoubtP = false;
		}
		else {
			showWireframe = false;
			showDivReg = true;
			emit setMeshShowState();
		}
	}
	emit notifyDoubtPInfo(on, mesh.polePs.size());
	update();
}
//层域生成及显示
void GLArea::setALOA(bool on) {
	showALOA = on;
	if (on && !mesh.isGenALOA) {
		mesh.aloa();
		if (!mesh.isGenALOA || mesh.aloa_layerS < 0) {
			showALOA = false;
		}
	}
	emit notifyMeshInfo(showSLT || showWireframe || showDisField || showALOA, (int)mesh.vert.size(), (int)mesh.face.size());
	emit notifyALOAInfo(on, mesh.aloa_layerS);
	update();
}
#pragma region 基于背景矩形网格获取场场信息
//距离场四边网格生成及显示
void GLArea::setDisQuaMesh(bool on) {
	showDisQuaMesh = on;
	if (on && mesh.isGenRegDivToS && !mesh.isGenDF) {
		mesh.getDFSL();
		if (!mesh.isGenDF) {
			showDisQuaMesh = false;
			QMessageBox::about(this, "Qua", "<font color='red'>Quadrilateral mesh</font> generation error");
		}
	}
	//emit setSpecState(mesh.isGenDF && (showDisField || showQuaA));
	emit updateDisSpecState(mesh.isGenDF);
	//emit setDisBgMeshState(showQuaA);
	int col = 0, row = 0;
	Vec3 minP, maxP;
	double specifQua = 0.0;
	if (mesh.isGenDF)
		mesh.dfm->getQuaMeshInfo(col, row, minP, maxP, specifQua);
	emit notifyDisQuaMeshInfo(showDisQuaMesh, col, row, minP, maxP, specifQua);
	emit setDisSLAble(mesh.disSL);
	update();
}
//距离场数据精度调整
void GLArea::setDisQuaMeshSpec(double spec) {
	if (abs(spec - mesh.getDisQuaSize()) > 1e-5) {
		mesh.setDisQuaSize(spec);
		mesh.isGenDF = false;
		//setQuaMesh(true);
		if (mesh.isGenRegDivToS) {
			mesh.getDFSL();
			if (!mesh.isGenDF) {
				QMessageBox::about(this, "Qua", "<font color='red'>Quadrilateral mesh</font> generation error");
			}
		}
		//emit setSpecState(true);
		//emit updateDisSpecState();
		int col = 0, row = 0;
		Vec3 minP, maxP;
		//double specifQua = 0.0;
		//if (mesh.isGenDF)
		mesh.dfm->getQuaMeshInfo(col, row, minP, maxP);
		emit notifyDisQuaMeshInfo(true, col, row, minP, maxP, mesh.getDisQuaSize());
		update();
	}
}
//输出锯齿网格
void GLArea::genDisQua(QString st) {
	if (mesh.isGenDF) {
		mesh.saveSawtoothQua(st);
	}
}
//距离场显示
void GLArea::setDisField(bool on) {
	showDisField = on;
	if (mesh.isloadVTK) {
		emit notifyMeshInfo(showSLT || showWireframe || showDisField || showALOA || showDifSL, (int)mesh.vert.size(), (int)mesh.face.size());
	}
	else {
		showDisField = false;
	}
	//emit setSpecState(mesh.isGenDF && (showDisField || showQuaA));
	emit updateDisSpecState(mesh.isGenDF);
	update();
}
//void GLArea::buildIndex()
//{
//	if (mesh.vert.size() == 0)
//		return;
//
//	unsigned nv = mesh.vert.size();
//	ANNpointArray dataPts = annAllocPts(nv, 2); // 顶点个数
//	int count = 0;
//	for (int i = 0; i<mesh.vert.size(); ++i)
//	{
//		dataPts[count][0] = mesh.vert[i].pos[0];
//		dataPts[count][1] = mesh.vert[i].pos[1];
//		//dataPts[count][2] = mesh.vert[i].pos[2];
//		++count;
//	}
//	if (kdTree)
//		delete kdTree;
//	kdTree = new ANNkd_tree(dataPts, nv, 2);
//}
//int GLArea::find_vertex_using_selected_point()
//{
//	ANNpoint tp = annAllocPt(3);
//	tp[0] = curSimPos.pos[0];
//	tp[1] = curSimPos.pos[1];
//	tp[2] = curSimPos.pos[2];
//	ANNidxArray nnIdx = new ANNidx[1];
//	ANNdistArray dists = new ANNdist[1];
//	kdTree->annkSearch(tp, 1, nnIdx, dists);
//	return nnIdx[0];
//}
//int GLArea::find_face_using_selected_point()
//{
//	int rv = find_vertex_using_selected_point();
//	HE_vert *hv = mesh.halfVerts[rv];
//	HE_edge *he = mesh.halfEdges[hv->edgeI];
//
//	HE_face *hf = mesh.halfFaces[he->faceI];
//	HE_edge he_t(*he);
//	//Mesh::VertexFaceIter vf_it = mesh.vf_iter(mesh.vertex_handle(rv));
//	int desiredFace = -1; //double minLen = 10*radius();
//	std::vector<Vec3> tri_p(3);
//	int tri_count = 0;
//	Vec3 resultP(curSimPos.pos[0], curSimPos.pos[1], curSimPos.pos[2]);
//	while (1) {
//		tri_count = 0;
//		HE_edge he_tt(he_t);
//		while (tri_count<3) {
//			tri_p[tri_count] = mesh.halfVerts[he_tt.vertI]->pos;
//			he_tt = *mesh.halfEdges[he_tt.nextI];
//			/*if (he_tt->index == he_t->index) {
//			break;
//			}*/
//			tri_count++;
//		}
//		if (check_in_triangle_face(tri_p, resultP)) {
//			desiredFace = hf->index;
//			break;
//		}
//		if (he_t.pairI != -1) {
//			if (mesh.halfEdges[mesh.halfEdges[he_t.pairI]->nextI]->index != he->index) {
//				he_t = *mesh.halfEdges[mesh.halfEdges[he_t.pairI]->nextI];
//				hf = mesh.halfFaces[he_t.faceI];
//			}
//			else break;
//		}
//		else {
//			if (he->pairI != -1) {
//				break;
//			}
//			if (mesh.halfEdges[he_t.frontI]->pairI != -1) {
//				he_t = *mesh.halfEdges[mesh.halfEdges[he_t.frontI]->pairI];
//				hf = mesh.halfFaces[he_t.faceI];
//			}
//			else break;
//		}
//	}
//
//	if (desiredFace < 0)
//	{
//		for (int i = 0; i<mesh.face.size(); ++i)
//		{
//			tri_count = 0;
//			hf = mesh.halfFaces[i];
//			he_t = *mesh.halfEdges[hf->edgeI];
//			HE_edge he_tt(he_t);
//			while (tri_count<3) {
//				tri_p[tri_count] = mesh.halfVerts[he_tt.vertI]->pos;
//				he_tt = *mesh.halfEdges[he_tt.nextI];
//				tri_count++;
//			}
//			if (check_in_triangle_face(tri_p, resultP)) {
//				desiredFace = hf->index;
//				break;
//			}
//		}
//	}
//
//	return  desiredFace;
//}

//void GLArea::pick_face()
//{
//	int desiredFace = find_face_using_selected_point();
//	if (desiredFace < 0) return;
//	//desiredFace = 9289;
//	//lastestFace = desiredFace;
//	qDebug("Select fase  %d\n", desiredFace);
//
//	std::vector<int>::iterator it;
//	if ((it = std::find(selectedFaceIdx.begin(), selectedFaceIdx.end(), desiredFace)) == selectedFaceIdx.end())
//	{
//		selectedFaceIdx.push_back(desiredFace);
//	/*	mesh.vert[mesh.face[desiredFace].index[0]].selectedColor = Vec3(1.0,0.0,0.0);
//		mesh.vert[mesh.face[desiredFace].index[1]].selectedColor = Vec3(1.0, 0.0, 0.0);
//		mesh.vert[mesh.face[desiredFace].index[2]].selectedColor = Vec3(1.0, 0.0, 0.0);*/
//		mesh.halfFaces[desiredFace]->is_selected=true;
//	}
//	else
//	{
//		mesh.halfFaces[desiredFace]->is_selected = false;
//		/*mesh.vert[mesh.face[*it].index[0]].selectedColor = Vec3(0.0, 0.0, 0.0);
//		mesh.vert[mesh.face[*it].index[1]].selectedColor = Vec3(0.0, 0.0, 0.0);
//		mesh.vert[mesh.face[*it].index[2]].selectedColor = Vec3(0.0, 0.0, 0.0);*/
//		selectedFaceIdx.erase(it);
//		
//
//	}
//	//selectedFace.clear();
//	//selectedVertex.clear();
//
//	update();
//}
//bool GLArea::check_in_triangle_face(std::vector<Vec3> tri, Vec3 p)
//{
//	Vec3 v1 = tri[1] - tri[0];
//	Vec3 v2 = tri[2] - tri[0];
//	p[2] = 0.0;
//	Vec3 n = v1.cross(v2);
//	double face_area = n.norm(); //三角形的面积
//	n = n.normalized();
//	double all_area = 0;
//	for (unsigned i = 0; i < tri.size(); ++i)
//	{
//		unsigned next_i = (i + 1) % tri.size();
//		unsigned prev_i = (i + tri.size() - 1) % tri.size();
//		v1 = tri[next_i] - p;
//		v2 = tri[prev_i] - p;
//		double area = abs((v1.cross(v2))*n);
//		all_area += area;
//		if (area < 0)
//		{
//			return false;
//		}
//	}
//	if (std::abs(all_area - face_area) < 1e-8) {
//		return true;
//	}
//	else {
//		return false;
//	}
//}

void GLArea::getFacesStreamLineGoThrough(bool b)
{
	if (b) {
		mesh.getFacesStreamLineGoThrough();
	}
	showPickedFace = b;
}

void GLArea::loadShowDisMesh(bool on) {
	showDisMesh = on;
	if (on && disMesh.isLoad && !disMesh.isGenDisF) {
		disMesh.GenDisF();
		showDisMesh = disMesh.isGenDisF;
	}
	update();
}
void GLArea::setDifQuaMesh(bool on) {
	showDifQuaMesh = on;
	if (on && mesh.isGenRegDivToS && !mesh.isGenDifF) {
		mesh.getTFSL();
		if (!mesh.isGenDifF) {
			showDifQuaMesh = false;
			QMessageBox::about(this, "Qua", "<font color='red'>Quadrilateral mesh</font> generation error");
		}
	}
	//emit setSpecState(mesh.isGenDF && (showDisField || showQuaA));
	emit updateDifSpecState(mesh.isGenDifF);
	//emit setDisBgMeshState(showQuaA);
	int col = 0, row = 0;
	Vec3 minP, maxP;
	double specifQua = 0.0;
	if (mesh.isGenDifF)
		mesh.tfm->getQuaMeshInfo(col, row, minP, maxP, specifQua);
	emit notifyDifQuaMeshInfo(mesh.isGenDifF, col, row, minP, maxP, specifQua);
	emit setDifSLAble(mesh.difSL);
	update();
}
void GLArea::setDifField(bool on) {
	showDifField = on;
	if (mesh.isloadVTK) {
		emit notifyMeshInfo(showSLT || showWireframe || showDisField || showALOA || showDifSL, (int)mesh.vert.size(), (int)mesh.face.size());
	}
	else {
		showDifField = false;
	}
	//emit setSpecState(mesh.isGenDF && (showDisField || showQuaA));
	emit updateDifSpecState(mesh.isGenDifF);
	update();
}
void GLArea::setDifQuaMeshSpec(double spec) {
	if (fabs(spec - mesh.getDifQuaSize()) > 1.0e-5) {
		mesh.setDifQuaSize(spec);
		mesh.isGenDifF = false;
		//setQuaMesh(true);
		if (mesh.isGenRegDivToS) {
			mesh.isGenDirectField = false;
			mesh.getTFSL();
			if (!mesh.isGenDifF) {
				QMessageBox::about(this, "Qua", "<font color='red'>Quadrilateral mesh</font> generation error");
			}
		}
		//emit setSpecState(true);
		//emit updateDisSpecState();
		int col = 0, row = 0;
		Vec3 minP, maxP;

		//if (mesh.isGenDifF)
		mesh.tfm->getQuaMeshInfo(col, row, minP, maxP);
		emit notifyDifQuaMeshInfo(true, col, row, minP, maxP, mesh.getDifQuaSize());
		update();
	}
}
#pragma endregion
//流线生成及显示（多种类型）
void GLArea::setDivDisSL(bool on) {
	showDisSL = on;
	if (on && mesh.disSL && !mesh.isDisSL) {
		mesh.slt = dis_sl;
		auxFun();
		//mesh.StreamLineClear();
		mesh.getStreamLines();
		if (!mesh.isGenStreamLine) {
			showDisSL = false;
		}
		else {
			mesh.isGenSSL = false;
			showSLSimplify = false;
		}
	}
	emit setSLState(false, false, showDisSL && mesh.isGenStreamLine, false, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
void GLArea::setDivVSL(bool on) {
	showValueSL = on;
	if (on && !mesh.vSL) {
		mesh.slt = v_sl;
		auxFun();
		//mesh.StreamLineClear();
		mesh.getStreamLines();
		if (!mesh.isGenStreamLine) {
			showValueSL = false;
			mesh.vSL = false;
		}
		else {
			mesh.isGenSSL = false;
			showSLSimplify = false;
		}
	}
	emit setSLState(false, showValueSL && mesh.isGenStreamLine, false, false, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
void GLArea::setDivDifSL(bool on) {
	showDifSL = on;
	if (on && !mesh.isDifSL && mesh.difSL) {
		mesh.slt = dif_sl;
		auxFun();
		//mesh.getTFSL();
		//mesh.isDSL = true;
		//mesh.StreamLineClear();
		mesh.getStreamLines();
		if (!mesh.isGenStreamLine) {
			//showWeightSL = false;
			showDifSL = false;
		}
		else {
			mesh.isGenSSL = false;
			showSLSimplify = false;
		}
	}
	emit setSLState(showDifSL && mesh.isGenStreamLine, false, false, false, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
//标架场生成及显示
void GLArea::setAcrossField(bool on) {
	showAcrossField = on;
	if (on && !mesh.isGenAcrossField) {
		QTime time;
		time.start();
		mesh.acrossField();
		qDebug() << "cross field:" << time.elapsed() / 1000.0 << "s";
		if (!mesh.isGenAcrossField || mesh.acrossF.size() == 0) {
			showAcrossField = false;
		}
	}
	update();
}
void GLArea::setCroSize(int cln, int clp, double cbs, bool on) {
	if (mesh.setCroSize(cln, clp, cbs)) {
		mesh.isGenAcrossField = false;
		setAcrossField(on);
	}
}
//方向场生成及显示
void GLArea::setDirectField(bool on) {
	showDirectField = on;
	if (on && !mesh.isGenDirectField) {
		QTime time;
		time.start();
		mesh.directField();
		qDebug() << "direct field:" << time.elapsed() / 1000.0 << "s";
		if (!mesh.isGenDirectField || mesh.directF.size() == 0) {
			showDirectField = false;
		}
	}
	update();
}
void GLArea::setDirSize(int dln, int dlp, double dbs, double dhs, bool on) {
	if (mesh.setDirSize(dln, dlp, dbs, dhs)) {
		mesh.isGenDirectField = false;
		setDirectField(on);
	}
}
//标量场显示
void GLArea::setScalarField(bool on) {
	showScalarField = on;
	if (on && !mesh.isGenScalarField) {
		mesh.scalarFiled();
		if (!mesh.isGenScalarField) {
			showScalarField = false;
		}
	}
	update();
}
//状态设置
void GLArea::auxFun() {
	mesh.isDifSL = false;
	mesh.isDisSL = false;
	mesh.vSL = false;
	mesh.uSL = false;
	showStreamLine = false;
	showDisSL = false;
	showValueSL = false;
	showDifSL = false;
	switch (mesh.slt)
	{
	case v_sl:
		mesh.vSL = true;
		showValueSL = true;
		break;
	case dis_sl:
		mesh.isDisSL = true;
		showDisSL = true;
		break;
	case dif_sl:
		mesh.isDifSL = true;
		showDifSL = true;
		break;
	case u_sl:
		mesh.uSL = true;
		showStreamLine = true;
		break;
	default:
		qDebug("variable error!");
		break;
	}
}
//new（无关）
void GLArea::showTempSL() {
	showStreamLine = true;
	mesh.findSL();
	if (!mesh.genSL) {
		showStreamLine = false;
	}
}
//end
//建立流线拓扑结构后，
//流线交点
void GLArea::setFourDim(int i) {
	if (i > 3 && i < 100) {
		mesh.dimNum = i;
	}
}
void GLArea::setCreateFourValueMesh(bool on) {
	showSLT = on;
	if (mesh.isGenSSL) {
		if (on && !mesh.isGenQua) {
			mesh.findQuaSFValue();
			emit ToSetFourDim(mesh.dimNum);
			showSLT = mesh.isGenQua;
			showSingular = false;
			setSTL1(true);
			setDivSLSimplify(false);
		}
	}
	else {
		showSLT = false;
	}
	emit notifyMeshInfo(showSLT || showWireframe || showDisField || showALOA, (int)mesh.vert.size(), (int)mesh.face.size());
	emit setSLState(showDifSL, showValueSL, showDisSL, showStreamLine, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
//离线
void GLArea::setSTL1(bool on) {
	if (mesh.isGenQua) {
		if (on && !showQuaSSL) {
			InitSLState();
		}
		showQuaSL = on;
	}
	else {
		showQuaSL = false;
	}
	emit setSLState(showDifSL, showValueSL, showDisSL, showStreamLine, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
//设置流线段数，需结合按键事件操作
void GLArea::setPreGenQua() {
	mesh.preGenQuaStatec();
	setQMesh(false);
	showCareQuaMesh = false;
	showQuaSSL = false;
	preGenQud = false;
}
void GLArea::getNormClass()
{
	mesh.showNormClass();
}
void GLArea::highLightCrossField(QString filePath)
{
	if (mesh.highLightCrossField(filePath)) {
		isHighLightCF = true;
	}
	else {
		isHighLightCF = false;
		qDebug() << "cannot highlight CF" << endl;
	}
	update();
}
//设置边界流线类型（用于数值计算）
void GLArea::setNonSVBor(bool on) {
	mesh.isNonSVBor = on;
	update();
}
//等间隔流线
void GLArea::setSTL2(bool on) {
	if (mesh.isFinish) {
		if (on) {
			InitSLState();
		}
		showQuaSSL = on;
	}
	else {
		showQuaSSL = false;
	}
	emit setSLState(showDifSL, showValueSL, showDisSL, showStreamLine, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
//四边网格生成
void GLArea::setQuaMeshSpec(double spec) {
	if (fabs(spec - mesh.gridPrecision) > 1e-9 && mesh.isFourMesh && isSetSpec) {
		if (mesh.JudgeTAIA(spec, true)) {
			mesh.gridPrecision = spec;
			QTime time;
			time.start();
			mesh.genQuaMesh();
			qDebug() << " quadrilateral mesh:" << time.elapsed() / 1000.0 << "s";
			if (!mesh.isFinish) {
				showCareQuaMesh = false;
			}
			QString quaInfo = "point:" + QString::number(mesh.careQuaMeshP.size()) + " cell:" + QString::number(mesh.careQuaIndex.size()) + " area:" + QString::number(mesh.quadraInfo.size()) + "\n" + "Jacobian:" + QString::number(mesh.quaMinYkbA) + " | " + QString::number(mesh.quaMinYkbB) + "\n" + "min angle:" + QString::number(mesh.quaMinAngle);
			emit notifyQuaMeshNum(showCareQuaMesh, quaInfo);
		}
		else {
			QMessageBox::about(this, "Qua", "<font color='red'>Quadrilateral mesh</font> Inaccurate accuracy");
		}
		update();
	}
}
void GLArea::setQMesh(bool on) {
	showCareQuaMesh = on;
	if (on) {
		quasSLBTState = false;
	}
	if (mesh.isFourMesh) {
		if (on && !mesh.isFinish) {
			QTime time;
			time.start();
			mesh.genQuaMesh();
			qDebug() << " quadrilateral mesh:" << time.elapsed() / 1000.0 << "s";
			isSetSpec = mesh.isFinish;
			if (isSetSpec) {
				showCareQuaMesh = on;
				showSLT = false;
			}
		}
		if (on && mesh.isFinish) {
			InitSLState();
			mesh.setQBT();
		}
	}
	else {
		showCareQuaMesh = false;
	}
	if (showCareQuaMesh) {
		setQuaMeshGPState(true, mesh.gridPrecision);
	}
	QString quaInfo = "point:" + QString::number(mesh.careQuaMeshP.size()) + " cell:" + QString::number(mesh.careQuaIndex.size()) + " area:" + QString::number(mesh.quadraInfo.size()) + "\n" + "Jacobian:" + QString::number(mesh.quaMinYkbA) + " | " + QString::number(mesh.quaMinYkbB) + "\n" + "min angle:" + QString::number(mesh.quaMinAngle);
	emit notifyQuaMeshNum(showCareQuaMesh, quaInfo);
	emit setSLState(showDifSL, showValueSL, showDisSL, showStreamLine, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
//四边网格光顺
void GLArea::setCareQuaSmo(bool on) {
	careQuaSmooth = on;
	if (on && !mesh.isFinish) {
		careQuaSmooth = false;
		QMessageBox::about(this, "Quadrilateral mesh smoothing", "<font color='red'>Please make a quadrilateral mesh</font>");
		emit setQuaSmoothState();
	}
	update();
}
void GLArea::smoothCareQua() {
	if (mesh.isFinish) {
		QMessageBox::about(this, "Quadrilateral mesh smoothing", "<font color='blue'>" + QString::number(mesh.smoothMesh()) + "</font>");
		QString quaInfo = "point:" + QString::number(mesh.careQuaMeshP.size()) + " cell:" + QString::number(mesh.careQuaIndex.size()) + " area:" + QString::number(mesh.quadraInfo.size()) + "\n" + "Jacobian:" + QString::number(mesh.quaMinYkbA) + " | " + QString::number(mesh.quaMinYkbB) + "\n" + "min angle:" + QString::number(mesh.quaMinAngle);
		emit notifyQuaMeshNum(showCareQuaMesh, quaInfo);
	}
	else {
		QMessageBox::about(this, "Quadrilateral mesh smoothing", "<font color='red'>Please make a quadrilateral mesh</font>");
	}
	update();
}
void GLArea::smoothCareQuaReset() {
	if (mesh.isFinish) {
		mesh.resetSmoothMesh();
		QMessageBox::about(this, "Quadrilateral mesh smooth reset", "<font color='blue'>Reset successful!</font>");
		QString quaInfo = "point:" + QString::number(mesh.careQuaMeshP.size()) + " cell:" + QString::number(mesh.careQuaIndex.size()) + " area:" + QString::number(mesh.quadraInfo.size()) + "\n" + "Jacobian:" + QString::number(mesh.quaMinYkbA) + " | " + QString::number(mesh.quaMinYkbB) + "\n" + "min angle:" + QString::number(mesh.quaMinAngle);
		emit notifyQuaMeshNum(showCareQuaMesh, quaInfo);
	}
	else {
		QMessageBox::about(this, "Quadrilateral mesh smooth reset", "<font color='red'>Please make a quadrilateral mesh</font>");
	}
	update();
}
//初始化状态
void GLArea::InitSLState() {
	showQuaSSL = showQuaSL = showSLSimplify = showStreamLine = showDisSL = showValueSL = showDifSL = false;
}
//流线简化力度设置
void GLArea::setLLSSpec(double spec) {
	if (fabs(spec - mesh.getLLSSpec()) > 1.0e-5) {
		mesh.setLLSSpec(spec);
		if (mesh.isGenStreamLine) {
			QTime time;
			time.start();
			mesh.ReduceSLToArray();
			qDebug() << " auto sll:" << time.elapsed() / 1000.0 << "s";
			if (mesh.isGenSSL) {
				showSLSimplify = true;
			}
			showCareQuaMesh = showQuaSSL = showQuaSL = showSLT = false;
			setQuaMeshGPState(false, 0.0);
		}
		update();
	}
}
void GLArea::setSimState(bool on) {
	simState = on;
	update();
}
void GLArea::setSimCoord(QString s) {
	simCoordSpe = s.toDouble();
	update();
}
//流线结构简化-手动操作（结合按键事件）
void GLArea::setSimSL() {
	if (mesh.isGenStreamLine) {
		if (!mesh.isGenSSL) {
			QTime time;
			time.start();
			mesh.selecTtraps();
			qDebug() << "hand sll:" << time.elapsed() / 1000.0 << "s";
			mesh.isGenSSL = true;
			showSLSimplify = showCareQuaMesh = showQuaSSL = showQuaSL = showSLT = false;
		}
		showSLSimplify = !showSLSimplify;
		if (showSLSimplify && mesh.isGenSSL && !showQuaSL && !showQuaSSL) {
			InitSLState();
			showSLSimplify = true;
			emit setLLSState(mesh.getLLSSpec());
		}
	}
	else {
		showSLSimplify = false;
	}
	emit setSLState(showDifSL, showValueSL, showDisSL, showStreamLine, showSLSimplify, showSLT, showQuaSL, showQuaSSL, showCareQuaMesh, mesh.SLS.size(), mesh.getSSLC());
	update();
}
void GLArea::setDrawPState(bool is) {
	drawPState = is && simState;
	emit setDrawState(drawPState);
	update();
}
//显示设置
void GLArea::setShowColor(showColor slC, showColor quaC) {
	sslColor = slC;
	quaColor = quaC;
	update();
}
void GLArea::setLineSize(double sw) {
	if (fabs(sw - sslw) > interDim && sw > 0.0) {
		sslw = sw;
	}
	update();
}
//文件输出
void GLArea::outputFiles(QString st, QStringList path) {
	if (st == "LikeQua") {
		if (!mesh.writeLikeQua(path[0])) {
			QMessageBox::warning(NULL, "warning", "writeLikeQua Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "CareQua") {
		if (!mesh.writeQuaZx(path[0])) {
			QMessageBox::warning(NULL, "warning", "writeQuaZx Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "BorderSL") {
		if (!mesh.writeReduceSLTxt(path[0])) {
			QMessageBox::warning(NULL, "warning", "BorderSL Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "border-GM2") {
		if (!mesh.writeGM2(path[0])) {
			QMessageBox::warning(NULL, "warning", "gm2 Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "care-GD2") {
		if (!mesh.writeGD2(path[0])) {
			QMessageBox::warning(NULL, "warning", "gd2 Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "CGNS-AUX") {
		if (!mesh.writeToCGNS(path[0])) {
			QMessageBox::warning(NULL, "warning", "cgns Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "Qua-ToExcel") {
		if (!mesh.writeToExcel(path[0])) {
			QMessageBox::warning(NULL, "warning", "qua-ToExcel Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "Qua-JOC-vtk") {
		if (!mesh.writeVTKQJOC(path[0])) {
			QMessageBox::warning(NULL, "warning", "Qua-JOC-vtk Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "Qua-MINA-vtk") {
		if (!mesh.writeVTKQMINA(path[0])) {
			QMessageBox::warning(NULL, "warning", "Qua-MINA-vtk Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "segB-lcn") {
		if (!mesh.writeSegBlcn(path[0])) {
			QMessageBox::warning(NULL, "warning", "segB-lcn Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "BorderLine-VTK") {
		if (!mesh.writeVTKB(path[0])) {
			QMessageBox::warning(NULL, "warning", "BorderLine-VTK Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "ssl-VTK") {
		if (!mesh.writeSSLVTK(path[0])) {
			QMessageBox::warning(NULL, "warning", "ssl-VTK Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "PAT3") {
		if (!mesh.writeToPAT3(path[0])) {
			QMessageBox::warning(NULL, "warning", "PAT3 Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else if (st == "CFL") {
		if (!mesh.writeToCFL(path)) {
			QMessageBox::warning(NULL, "warning", "CFL Build failed!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		}
	}
	else {
		QMessageBox::warning(NULL, "warning", "unknown mistake!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	}
}

