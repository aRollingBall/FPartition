#include "mesh.h"
#include<qfile.h>
#include<qpair.h>
#include<qstring.h>
#include<stdio.h>
#include<qdebug.h>
#include<qmessagebox.h>
#include<qinputdialog.h>
#include <QDir>
bool Mesh::isEmpty() const
{
	return Mesh::face.size() > 0 ? false : true;
}
Mesh::Mesh()
{
	halfEdges = nullptr;
	halfVerts = nullptr;
	halfFaces = nullptr;
	quaGridNum = 0;
	isTAIA = false;
	isFourMesh = false;
	isFinish = false;
	isQuaSmooth = false;
	isGenQuaMesh = false;
	isGenQua = false;
	sf.reserve(SLFace);
	polePs.reserve(100);
	dirSLPointCount.reserve(SLCount);
	showSSLS.reserve(SLVert * 10);
	SLSimplify.reserve(SLCount);
	SLS.reserve(SLCount);
	QuaSLInfo.reserve(SLCount);
	dirSL.reserve(SLCount*SLVert);
	dirSV.reserve(SVertCount);
	maxPos = Vec3(-1e5, -1e5, -1e5);
	minPos = Vec3(1e5, 1e5, 1e5);
	disQuaSize = 1e5;
	isGenRegDivToS = false;
	maxDis = 0.0;
	isGenALOA = false;
	isGenScalarField = false;
	isGenAcrossField = false;
	isGenDirectField = false;
	isGenStreamLine = false;
	isGenSSL = false;
	isGenSingular = false;
	aloa_layerS = 0;
	isloadVTK = false;
	isloadTXT = false;
	isGenPole = false;
	disSL = false;
	isDisSL = false;
	vSL = false;
	uSL = false;
	isGenDF = false;
	isGenDifF = false;
	difSL = false;
	isDifSL = false;
	dfm = nullptr;
	tfm = nullptr;
	slt = u_sl;
	genSL = false;
	gridPrecision = 0.1;
	selectSLIndex[0] = -1;
	selectSLIndex[1] = -1;
	selectSLPI = 0;
	llsValue = 3.0;
	dimNum = 10;
	croBSize = 1.0;
	croLayNum = 1;
	crolpD = 1;
	dirBSize = 1.0;
	dirLayNum = 1;
	dirHSize = 1.0;
	dirlpD = 1;
	smoIterTimes = 0;
	quaMinAngle = 180.0;
	quaMinYkbA = 1.0;
	quaMinYkbB = 1.0;
	isNonSVBor = false;
}
void Mesh::clear() {
	if (halfVerts != nullptr && halfEdges != nullptr && halfFaces != nullptr) {
		for (int i = 0; i < vert.size(); i++) {
			delete halfVerts[i];
			halfVerts[i] = nullptr;
		}
		for (int i = 0; i < face.size(); i++) {
			delete halfFaces[i];
			halfFaces[i] = nullptr;
		}
		for (int i = 0; i < face.size() * 3; i++) {
			delete halfEdges[i];
			halfEdges[i] = nullptr;
		}
	}
	for (auto iter = dirSV.begin(); iter != dirSV.end();) {
		delete *iter;
		iter = dirSV.erase(iter);
	}
	for (auto iter = SLS.begin(); iter != SLS.end();) {
		delete *iter;
		iter = SLS.erase(iter);
	}
	for (auto iter = QuaSLInfo.begin(); iter != QuaSLInfo.end();) {
		delete *iter;
		iter = QuaSLInfo.erase(iter);
	}
	for (auto iter = SLSimplify.begin(); iter != SLSimplify.end();) {
		delete *iter;
		iter = SLSimplify.erase(iter);
	}
	std::map<int, mark_1>().swap(BECI);
	std::map<std::vector<int>, slType_Secand>().swap(slType);
	std::vector<QuaIndex>().swap(careQuaIndex);
	std::vector<vertAloa>().swap(careQuaMeshP);
	std::vector<LineIndex>().swap(careQuaBorderInlet);
	std::vector<LineIndex>().swap(careQuaBorderOutlet);
	std::vector<LineIndex>().swap(careQuaBorderWall0);
	std::vector<LineIndex>().swap(careQuaBorderWall1);
	std::vector<quadraPInfo>().swap(careQuaInfo);
	std::vector<Quadra>().swap(quadraInfo);
	std::vector<PCPoint>().swap(quaCareSLs);
	std::vector<fourNode>().swap(fNode);
	std::vector<PCPoint>().swap(quaSLs);
	std::vector<SLPoint>().swap(dirSL);
	std::vector<SFace>().swap(sf);
	std::map<int, std::map<int, std::map<int, pdStr>>>().swap(osf);
	std::vector<LineIndex>().swap(dirBVEdgeIndex);
	std::vector<polePoint>().swap(polePs);
	std::vector<int>().swap(dirSLPointCount);
	std::vector <PCPoint>().swap(showSSLS);
	std::vector <Vert>().swap(vert);
	std::vector <Face>().swap(face);
	std::vector <Vec3>().swap(acrossF);
	std::vector <Vec3>().swap(directF);
	std::vector <Vec3>().swap(directHandleF);
	std::vector <LineVs>().swap(bVEdge);
	std::vector <BELine>().swap(bVSEdge);
	maxPos = Vec3(-1e5, -1e5, 0.0);
	minPos = Vec3(1e5, 1e5, 0.0);
	disQuaSize = 1e5;
	isGenRegDivToS = false;
	maxDis = 0.0;
	isGenALOA = false;
	isGenScalarField = false;
	isGenAcrossField = false;
	isGenDirectField = false;
	isGenStreamLine = false;
	isGenSSL = false;
	isGenSingular = false;
	aloa_layerS = 0;
	isloadVTK = false;
	isloadTXT = false;
	isGenPole = false;
	disSL = false;
	isDisSL = false;
	vSL = false;
	uSL = false;
	isGenDF = false;
	isGenDifF = false;
	difSL = false;
	isDifSL = false;
	slt = u_sl;
	genSL = false;
	isGenQua = false;
	isGenQuaMesh = false;
	isFinish = false;
	isQuaSmooth = false;
	gridPrecision = 0.1;
	isFourMesh = false;
	isTAIA = false;
	quaGridNum = 0;
	llsValue = 3.0;
	selectSLIndex[0] = -1;
	selectSLIndex[1] = -1;
	selectSLPI = 0;
	dimNum = 10;
	croBSize = 1.0;
	croLayNum = 1;
	dirBSize = 1.0;
	dirLayNum = 1;
	dirHSize = 1.0;
	dirlpD = 1;
	crolpD = 1;
	smoIterTimes = 0;
	quaMinAngle = 180.0;
	quaMinYkbA = 1.0;
	quaMinYkbB = 1.0;
	isNonSVBor = false;
}
void Mesh::loadVTK1(const char*fn) {
	face.clear();
	vert.clear();
	int vertCount = 0;
	maxPointValue = -1e5;
	minPointValue = 1e5;
	maxPos = Vec3(-1.0e9, -1.0e9, 0.0);
	minPos = Vec3(1.0e9, 1.0e9, 0.0);
	double tempPointValue = 1e5;
	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) { isloadVTK = false; return; }
	std::vector<Vec3> pos;
	std::vector<double> values;
	std::vector<Vec3> vecs;
	char st[512];
	bool points, cells, cell_types, point_data, point_v;
	points = cells = cell_types = point_data = point_v = false;
	pos.clear();
	values.clear();
	vecs.clear();
	while (f.readLine(st, 511) != -1) {
		QString s = QString("%1").arg(st);
		s = s.trimmed();
		if (s.startsWith("t")) {
			/*char pp[512];
			sprintf(pp, "%s", s.toStdString().data());
			int  pCount;
			sscanf(pp, "POINTS %d float", &pCount);
			pCount += 100;
			pos.reserve(pCount);
			values.reserve(pCount);
			vecs.reserve(pCount);
			vert.reserve(pCount);
			acrossF.reserve(4 * pCount);
			directF.reserve(3 * pCount);
			directHandleF.reserve(2 * pCount);*/
			points = true;
			cells = cell_types = point_data = point_v = false;
			continue;
		}
		if (s.startsWith("face")) {
			/*char pp[512];
			sprintf(pp, "%s", s.toStdString().data());
			int fCount, t;
			sscanf(pp, "CELLS %d %d", &fCount, &t);
			fCount += 100;
			face.reserve(fCount);*/
			cells = true;
			points = cell_types = point_data = point_v = false;
			continue;
		}
		if (s.startsWith("face")) {
			cell_types = true;
			points = cells = point_data = point_v = false;
			continue;
		}
		if (s.startsWith("#")) {
			point_data = false;
			points = cells = cell_types = point_v = false;
			continue;
		}
		if (s.startsWith("NORMALS node_normals")) {
			point_v = false;
			points = cells = cell_types = point_data = false;
			continue;
		}
		if (points) {
			char pp[512];
			sprintf(pp, "%s", s.toStdString().data());
			Vec3 v;
			sscanf(pp, "v %lf %lf %lf", &(v.v[0]), &(v.v[1]), &(v.v[2]));
			v[2] = 0.0;
			if (maxPos.v[0] < v.v[0]) {
				maxPos.v[0] = v.v[0];
			}
			if (maxPos.v[1] < v.v[1]) {
				maxPos.v[1] = v.v[1];
			}
			if (minPos.v[0] > v.v[0]) {
				minPos.v[0] = v.v[0];
			}
			if (minPos.v[1] > v.v[1]) {
				minPos.v[1] = v.v[1];
			}
			pos.push_back(v);
		}
		if (cells) {
			char cp[512];
			sprintf(cp, "%s", s.toStdString().data());
			Face ff;
			sscanf(cp, "f %d %d %d", &ff.index[0], &ff.index[1], &ff.index[2]);
			ff.index[0] -= 1;
			ff.index[1] -= 1;
			ff.index[2] -= 1;
			face.push_back(ff);
		}
		if (point_data) {
			char vp[512];
			sprintf(vp, "%s", s.toStdString().data());
			if (vp[0] != 'S' && vp[0] != 'L') {
				double v;
				sscanf(vp, "%lf", &v);
				if (v > maxPointValue) {
					maxPointValue = v;
				}
				if (v < minPointValue) {
					//防止过小
					tempPointValue = minPointValue;
					minPointValue = v;
				}
				else {
					if (v < tempPointValue) {
						tempPointValue = v;
					}
				}
				values.push_back(v);
			}
		}
		if (point_v) {
			char pv[512];
			sprintf(pv, "%s", s.toStdString().data());
			Vec3 v;
			sscanf(pv, "%lf %lf %lf", &(v.v[0]), &(v.v[1]), &(v.v[2]));
			vecs.push_back(v);
		}
	}
	//记录模型尺寸
	boxCenter = (minPos + maxPos) / 2.0;//得到最小容纳3D模型的立方体
	boxSize = (minPos - maxPos).norm();
	//minPointValue = tempPointValue;
	//colorSLT.reserve(vert.size());
	std::vector<Vec3>::iterator iter1 = pos.begin();
	//std::vector<double>::iterator iter2 = values.begin();
	//std::vector<Vec3>::iterator vecIter = vecs.begin();
	bool isVecs = false;
	halfVerts = new HE_vert*[pos.size()];
	halfEdges = new HE_edge*[face.size() * 3];
	halfFaces = new HE_face*[face.size()];
	for (; iter1 != pos.end(); ++iter1) {
		Vert v;
		v.pos = *iter1;
		//v.uValue = *iter2;
		vert.push_back(v);
		//根据角度得到矢量
		if (isVecs) {
			halfVerts[vertCount++] = new HE_vert(*iter1, vecs[vertCount]);
		}
		else {
			halfVerts[vertCount++] = new HE_vert(*iter1, Vec3(1.0, 0.0, 0.0));
		}
	}
	f.close();
	isloadVTK = true;
	//isloadVTK = !(pos.size() == 0 || values.size() == 0 || face.size() == 0 || vert.size() == 0);
}
void Mesh::loadTXT(const char*fn) {
	singularTXT.clear();
	face.clear();
	vert.clear();
	maxPointValue = -1e5;
	minPointValue = 1e5;
	maxPos = Vec3(-1e5, -1e5, 0.0);
	minPos = Vec3(1e5, 1e5, 0.0);
	double tempPointValue = 1.0e9;
	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) { isloadTXT = false; return; }
	std::vector<Vec3> pos;
	//pos.reserve(vertsCount);
	std::vector<double> values;
	//values.reserve(vertsCount);
	std::vector<Vec3> vecs;
	//values.reserve(vertsCount);
	char st[512];
	bool points, cells, cell_types, point_data, point_v;
	points = cells = cell_types = point_data = point_v = false;
	pos.clear();
	values.clear();
	vecs.clear();
	while (f.readLine(st, 511) != -1) {
		QString s = QString("%1").arg(st);
		s = s.trimmed();
		if (s.startsWith("POINTS")) {
			points = true;
			cells = cell_types = false;
			continue;
		}
		if (s.startsWith("CELLS")) {
			cells = true;
			points = cell_types = false;
			continue;
		}
		if (s.startsWith("SINGULAR")) {
			cell_types = true;
			points = cells = false;
			continue;
		}
		if (points) {
			char pp[512];
			sprintf(pp, "%s", s.toStdString().data());
			int pI;
			double v[2];
			double u;
			double n[2];
			sscanf(pp, "%d %lf %lf %lf %lf %lf", &pI, &(v[0]), &(v[1]), &u, &(n[0]), &(n[1]));
			//在读取数据时，存储的是八位精度，后两位随机添加
			//v = v*(-1.0);
			if (maxPos.v[0] < v[0]) {
				maxPos.v[0] = v[0];
			}
			if (maxPos.v[1] < v[1]) {
				maxPos.v[1] = v[1];
			}
			if (minPos.v[0] > v[0]) {
				minPos.v[0] = v[0];
			}
			if (minPos.v[1] > v[1]) {
				minPos.v[1] = v[1];
			}
			if (u > maxPointValue) {
				maxPointValue = u;
			}
			if (u < minPointValue) {
				//防止过小
				tempPointValue = minPointValue;
				minPointValue = u;
			}
			else {
				if (u < tempPointValue) {
					tempPointValue = u;
				}
			}
			pos.push_back(Vec3(v[0], v[1], 0.0));
			values.push_back(u);
			vecs.push_back(Vec3(n[0], n[1], 0.0));
		}
		if (cells) {
			char cp[512];
			sprintf(cp, "%s", s.toStdString().data());
			Face ff;
			sscanf(cp, "%d %d %d", &ff.index[0], &ff.index[1], &ff.index[2]);
			ff.index[0] -= 1;
			ff.index[1] -= 1;
			ff.index[2] -= 1;
			face.push_back(ff);
		}
		if (cell_types) {
			char vp[512];
			sprintf(vp, "%s", s.toStdString().data());
			int v;
			sscanf(vp, "%d", &v);
			singularTXT.push_back(v - 1);
		}
	}
	int vertCount = 0;
	//记录模型尺寸
	boxCenter = (minPos + maxPos) / 2.0;//得到最小容纳3D模型的立方体
	boxSize = (minPos - maxPos).norm();
	std::vector<Vec3>::iterator iter1 = pos.begin();
	std::vector<double>::iterator iter2 = values.begin();
	std::vector<Vec3>::iterator vecIter = vecs.begin();
	for (; iter1 != pos.end() && iter2 != values.end() && vecIter != vecs.end(); ++iter1, ++iter2, ++vecIter) {
		Vert v;
		v.pos = *iter1;
		v.uValue = *iter2;
		vert.push_back(v);
		halfVerts[vertCount++] = new HE_vert(*iter1, *vecIter);
		//vDif.push_back(4.0);	//默认为奇点边界4.0
		//vDis.push_back(-1.0);
		//vDiw.push_back(*iter2);
	}
	f.close();
	//color3.insert(color3.begin(), color2.begin(), color2.end());
	//color4.insert(color4.begin(), color2.begin(), color2.end());
	isloadTXT = !(pos.size() == 0 || values.size() == 0 || vecs.size() == 0 || face.size() == 0 || vert.size() == 0);
}
void Mesh::loadVTK(const char* fn)
{
	face.clear();
	vert.clear();
	int vertCount = 0;
	maxPointValue = -1e5;
	minPointValue = 1e5;
	maxPos = Vec3(-1.0e9, -1.0e9, 0.0);
	minPos = Vec3(1.0e9, 1.0e9, 0.0);
	double tempPointValue = 1e5;
	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) { isloadVTK = false; return; }
	std::vector<Vec3> pos;
	std::vector<double> values;
	std::vector<Vec3> vecs;
	char st[512];
	bool points, cells, cell_types, point_data, point_v;
	points = cells = cell_types = point_data = point_v = false;
	pos.clear();
	values.clear();
	vecs.clear();
	while (f.readLine(st, 511) != -1) {
		QString s = QString("%1").arg(st);
		s = s.trimmed();
		if (s.startsWith("POINTS")) {
			char pp[512];
			sprintf(pp, "%s", s.toStdString().data());
			int  pCount;
			sscanf(pp, "POINTS %d float", &pCount);
			pCount += 100;
			pos.reserve(pCount);
			values.reserve(pCount);
			vecs.reserve(pCount);
			vert.reserve(pCount);
			acrossF.reserve(4 * pCount);
			directF.reserve(3 * pCount);
			directHandleF.reserve(2 * pCount);
			points = true;
			cells = cell_types = point_data = point_v = false;
			continue;
		}
		if (s.startsWith("CELLS")) {
			char pp[512];
			sprintf(pp, "%s", s.toStdString().data());
			int fCount, t;
			sscanf(pp, "CELLS %d %d", &fCount, &t);
			fCount += 100;
			face.reserve(fCount);
			cells = true;
			points = cell_types = point_data = point_v = false;
			continue;
		}
		if (s.startsWith("CELL_TYPES")) {
			cell_types = true;
			points = cells = point_data = point_v = false;
			continue;
		}
		if (s.startsWith("POINT_DATA")) {
			point_data = true;
			points = cells = cell_types = point_v = false;
			continue;
		}
		if (s.startsWith("NORMALS node_normals")) {
			point_v = true;
			points = cells = cell_types = point_data = false;
			continue;
		}
		if (points) {
			char pp[512];
			sprintf(pp, "%s", s.toStdString().data());
			Vec3 v;
			sscanf(pp, "%lf %lf %lf", &(v.v[0]), &(v.v[1]), &(v.v[2]));
			if (maxPos.v[0] < v.v[0]) {
				maxPos.v[0] = v.v[0];
			}
			if (maxPos.v[1] < v.v[1]) {
				maxPos.v[1] = v.v[1];
			}
			if (minPos.v[0] > v.v[0]) {
				minPos.v[0] = v.v[0];
			}
			if (minPos.v[1] > v.v[1]) {
				minPos.v[1] = v.v[1];
			}
			pos.push_back(v);
		}
		if (cells) {
			char cp[512];
			sprintf(cp, "%s", s.toStdString().data());
			Face ff;
			sscanf(cp, "3 %d %d %d", &ff.index[0], &ff.index[1], &ff.index[2]);
			face.push_back(ff);
		}
		if (point_data) {
			char vp[512];
			sprintf(vp, "%s", s.toStdString().data());
			if (vp[0] != 'S' && vp[0] != 'L') {
				double v;
				sscanf(vp, "%lf", &v);
				if (v > maxPointValue) {
					maxPointValue = v;
				}
				if (v < minPointValue) {
					//防止过小
					tempPointValue = minPointValue;
					minPointValue = v;
				}
				else {
					if (v < tempPointValue) {
						tempPointValue = v;
					}
				}
				values.push_back(v);
			}
		}
		if (point_v) {
			char pv[512];
			sprintf(pv, "%s", s.toStdString().data());
			Vec3 v;
			sscanf(pv, "%lf %lf %lf", &(v.v[0]), &(v.v[1]), &(v.v[2]));
			vecs.push_back(v);
		}
	}
	//记录模型尺寸
	boxCenter = (minPos + maxPos) / 2.0;//得到最小容纳3D模型的立方体
	boxSize = (minPos - maxPos).norm();
	//minPointValue = tempPointValue;
	//colorSLT.reserve(vert.size());
	std::vector<Vec3>::iterator iter1 = pos.begin();
	std::vector<double>::iterator iter2 = values.begin();
	//std::vector<Vec3>::iterator vecIter = vecs.begin();
	bool isVecs = vecs.size() == pos.size();
	halfVerts = new HE_vert*[pos.size()];
	halfEdges = new HE_edge*[face.size() * 3];
	halfFaces = new HE_face*[face.size()];
	for (; iter1 != pos.end() && iter2 != values.end(); ++iter1, ++iter2) {
		Vert v;
		v.pos = *iter1;
		v.uValue = *iter2;
		v.selectedColor = Vec3(0.0, 0.0, 0.0);
		vert.push_back(v);
		//根据角度得到矢量
		if (isVecs) {
			halfVerts[vertCount++] = new HE_vert(*iter1, vecs[vertCount]);
		}
		else {
			halfVerts[vertCount++] = new HE_vert(*iter1, angleToNorm((*iter2) / 180.0*Q_PI));
		}
	}
	f.close();
	isloadVTK = !(pos.size() == 0 || values.size() == 0 || face.size() == 0 || vert.size() == 0);
}
void Mesh::loadPartR(const char*fn) {

}
void Mesh::GenHalfStruct() {
	int face_count = 0;
	int edge_count = 0;
	std::set<int> halfPair;
	std::map<orderLineIndex, int> findEdgePair;
	for (auto iter = face.begin(); iter != face.end(); ++iter) {
		halfEdges[edge_count++] = new HE_edge(edge_count, iter->index[1], face_count, edge_count + 1, edge_count + 2);
		halfEdges[edge_count++] = new HE_edge(edge_count, iter->index[2], face_count, edge_count + 1, edge_count - 1);
		halfEdges[edge_count++] = new HE_edge(edge_count, iter->index[0], face_count, edge_count - 2, edge_count - 1);
		halfVerts[iter->index[0]]->edgeI = edge_count - 3;
		halfVerts[iter->index[1]]->edgeI = edge_count - 2;
		halfVerts[iter->index[2]]->edgeI = edge_count - 1;
		halfFaces[face_count++] = new HE_face(face_count, edge_count - 3);
		for (int i = 0; i < 3; i++) {
			orderLineIndex olI(iter->index[i], iter->index[(i + 1) % 3]);
			if (findEdgePair.find(olI) != findEdgePair.end()) {
				int eI = findEdgePair[olI];
				halfEdges[eI]->pairI = edge_count - 3 + i;
				halfEdges[edge_count - 3 + i]->pairI = eI;
				findEdgePair.erase(olI);
			}
			else {
				findEdgePair[olI] = edge_count - 3 + i;
			}
		}
	}
	int borderCount = 0;
	for (int i = 0; i < edge_count; i++) {
		if (halfEdges[i]->pairI == -1) {
			borderCount++;
		}
	}
	borderCount += 100;
	bVEdge.reserve(borderCount);
	bVSEdge.reserve(borderCount * 2);
	dirBVEdgeIndex.reserve(borderCount);
}
int Mesh::getBFrontPTodNextPI(int dirPI) {
	if (halfVerts[dirPI]->isBoundary) {
		int curEI = halfEdges[halfVerts[dirPI]->edgeI]->nextI;
		while (halfEdges[halfEdges[curEI]->nextI]->pairI != -1) {
			curEI = halfEdges[halfEdges[halfEdges[curEI]->nextI]->pairI]->nextI;
		}
		return halfEdges[curEI]->vertI;
	}
	else {
		return -1;
	}
}
double Mesh::getWeight(int vertI) {
	switch (slt)
	{
	case v_sl:
		if (fabs(maxPointValue - minPointValue - 3.0) < 1e-5) {
			return  vert[vertI].uValue;	//对应范围【1.0~4.0】,边界1.0,奇点4.0
		}
		else {
			return fabs(3.0*(maxPointValue - vert[vertI].uValue) / (maxPointValue - minPointValue)) + 1.0;	//对应范围【0.0~1.0】
		}
		break;
	case dis_sl:
		return fabs(maxDis) < 1e-5 ? 4.0 : 3.0*vert[vertI].disValue / maxDis + 1.0;
		break;
	case dif_sl:
		return vert[vertI].difValue;
		break;
	case u_sl:
		return isloadVTK ? 4.0 : 1.0;
		break;
	default:
		return 4.0;
		break;
	}
}
void Mesh::getWeight(double value, double& para1, double& para2) {
	para1 = (maxPointValue - value) / (maxPointValue - minPointValue);
	para2 = 1.25*(value - minPointValue) / (maxPointValue - minPointValue) - 0.25;
}
static bool cocircle(Vec3 p1, Vec3 p2, Vec3 p3, Vec3& q, double& r) {
	double x12 = p2[0] - p1[0];
	double y12 = p2[1] - p1[1];
	double x13 = p3[0] - p1[0];
	double y13 = p3[1] - p1[1];
	double z2 = x12*(p1[0] + p2[0]) + y12*(p1[1] + p2[1]);
	double z3 = x13*(p1[0] + p3[0]) + y13*(p1[1] + p3[1]);
	double d = 2.0*(x12*(p3[1] - p2[1]) - y12*(p3[0] - p2[0]));
	if (fabs(d) < 1e-7) //共线，圆不存在   
		return false;
	q[0] = (y13*z2 - y12*z3) / d;
	q[1] = (x12*z3 - x13*z2) / d;
	r = (q - p1).norm();
	return true;
}
static double mult(Vec3 a, Vec3 b, Vec3 c)
{
	return (a.v[0] - c.v[0])*(b.v[1] - c.v[1]) - (b.v[0] - c.v[0])*(a.v[1] - c.v[1]);
}
//判断两线段是否相交，若相交，返回交点
static bool isCross(Vec3 pos1, Vec3 pos2, Vec3 pos3, Vec3 pos4, Vec3& pos) {
	pos = Vec3();
	if ((pos1 - pos3).norm() < 1e-5 || (pos1 - pos4).norm() < 1e-5) {
		pos = pos1;
		return true;
	}
	if ((pos2 - pos3).norm() < 1e-5 || (pos2 - pos4).norm() < 1e-5) {
		pos = pos2;
		return true;
	}
	double posS1 = mult(pos1, pos2, pos3);
	double posS2 = mult(pos1, pos2, pos4);
	double posS3 = mult(pos3, pos4, pos1);
	if (posS1*posS2 >= 0.0 || posS3*(posS3 + posS1 - posS2) >= 0)
	{
		return false;
	}
	else {
		double temp = posS3 / (posS2 - posS1);
		double dx = temp*(pos2.v[0] - pos1.v[0]);
		double dy = temp*(pos2.v[1] - pos1.v[1]);
		pos.v[0] = pos1.v[0] + dx;
		pos.v[1] = pos1.v[1] + dy;
		return true;
	}
}
//计算第三个点在前两个点连线的比率（当比率为0，则第三个点为第一个点）
static double getRotate(Vec3 v1, Vec3 v2, Vec3 v3) {
	if (fabs(v2.v[1] - v1.v[1]) < 1.0e-9) {
		if (fabs(v2.v[0] - v1.v[0]) > 1.0e-9) {
			return  (v3.v[0] - v1.v[0]) / (v2.v[0] - v1.v[0]);
		}
		else {
			return -1;
		}
	}
	else {
		return  (v3.v[1] - v1.v[1]) / (v2.v[1] - v1.v[1]);
	}
}
//通过三个向量判断是否包含
static bool calParToP(Vec3 v1, Vec3 v2, Vec3 v3) {
	double t1, t2;
	t1 = t2 = -1.0;
	if (fabs(v1.v[0]) > interDim && fabs(v1.v[1]) > interDim && fabs(v1.v[0] * v2.v[1] - v1.v[1] * v2.v[0]) > interDim) {
		t2 = (v3.v[1] - v1.v[1] * v3.v[0] / v1.v[0]) / (v2.v[1] - v1.v[1] * v2.v[0] / v1.v[0]);
		t1 = (v3.v[0] - t2*v2.v[0]) / v1.v[0];
	}
	else if (fabs(v1.v[0]) < interDim && fabs(v2.v[0]) > interDim && fabs(v1.v[1]) > interDim) {
		t2 = v3.v[0] / v2.v[0];
		t1 = (v3.v[1] - t2*v2.v[1]) / v1.v[1];
	}
	else if (fabs(v1.v[1]) > interDim && fabs(v1.v[0]) > interDim && fabs(v2.v[0] * v1.v[1] - v2.v[1] * v1.v[0]) > interDim) {
		t2 = (v3.v[0] - v1.v[0] * v3.v[1] / v1.v[1]) / (v2.v[0] - v1.v[0] * v2.v[1] / v1.v[1]);
		t1 = (v3.v[1] - t2*v2.v[1]) / v1.v[1];
	}
	else if (fabs(v1.v[1]) < interDim && fabs(v2.v[1]) > interDim && fabs(v1.v[0]) > interDim) {
		t2 = v3.v[1] / v2.v[1];
		t1 = (v3.v[0] - t2*v2.v[0]) / v1.v[0];
	}
	else {
		//qDebug("Insufficient conditions!");
	}
	if (t1 > 0.0 && t2 > 0.0) {
		return true;
	}
	else {
		return false;
	}
}
//判断三角形上某点向量是否指向三角形,第四个参数为判断向量，第一个和第三个为相邻向量
static bool dirTri(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 v4) {
	if (calParToP(v1, v2, v4) || calParToP(v2, v3, v4)) {
		return true;
	}
	else {
		return false;
	}
}
//求某点投影到直线的点，附带三角形判断
static Vec3 getFootPoint(Vec3 pos, Vec3 pos1, Vec3 pos2) {
	Vec3 rValue(interDim*2.0);
	if ((pos - pos1).norm() > interDim && (pos - pos2).norm() > interDim && (pos1 - pos2).norm() > interDim &&
		fabs((pos1[1] - pos[1])*(pos2[0] - pos[0]) - (pos2[1] - pos[1])*(pos1[0] - pos[0])) > interDim) {
		Vec3 n = pos1 - pos2;
		rValue[0] = (n[0] * (n[0] * pos[0] + n[1] * pos[1]) - n[1] * (n[0] * pos2[1] - n[1] * pos2[0])) / (n[0] * n[0] + n[1] * n[1]);
		if (fabs(n[0]) > interDim) {
			rValue[1] = n[1] * (rValue[0] - pos2[0]) / n[0] + pos2[1];
		}
		else {
			rValue[1] = pos[1];
		}
	}
	return rValue;
}
static double angledif(Vec3 v1, Vec3 v2) {
	return (v1*v2) / (v1.norm()*v2.norm());
}
//通过给定一个角度，寻找与一个角度和其另外三个差值为90度相近的角度
static double findAngleToAngle(double angle1, double angle2) {
	//第一个角度为指定的角度，第二个角度为待判断角度
	double minDif1 = fabs(angle1 - angle2);
	double minDif = fabs(minDif1 - 2.0*Q_PI) > minDif1 ? minDif1 : fabs(minDif1 - 2.0*Q_PI);

	//double minDif = (angledif(angleToNorm(angle1), angleToNorm(angle2)));
	double getAngle = angle2;
	double iter = angle2;
	for (int i = 1; i < 4; i++) {
		iter += 0.5*Q_PI;
		if (iter - 2.0*Q_PI > 1e-6) {
			iter -= 2.0*Q_PI;
		}
		//double isMin =(angledif(angleToNorm(iter), angleToNorm(angle1)));
		double isMin = fabs(iter - angle1) > fabs(fabs(iter - angle1) - 2.0*Q_PI) ? fabs(fabs(iter - angle1) - 2.0*Q_PI) : fabs(iter - angle1);
		if (isMin < minDif) {
			minDif = isMin;
			getAngle = iter;
		}
	}
	return getAngle;
}
void Mesh::findEdgePointPlus(int edgeI, Vec3 startPos, SLPoint* endPos, int &endPosNum) {
	HE_edge *sEdge = halfEdges[edgeI];
	int sEdgePairVI = halfEdges[halfEdges[sEdge->nextI]->nextI]->vertI;//以防反向半边不存在
	Vec3 n1 = halfVerts[sEdgePairVI]->vec;
	Vec3 n2 = halfVerts[sEdge->vertI]->vec;
	Vec3 pos1 = halfVerts[sEdgePairVI]->pos;
	Vec3 pos2 = halfVerts[sEdge->vertI]->pos;
	Vec3 nc1 = angleToNorm(n1.angleP() / 4.0)*n1.norm();
	Vec3 nc2 = angleToNorm(findAngleToAngle(n1.angleP() / 4.0, n2.angleP() / 4.0))*n2.norm();
	//由于存在向量场中向量模较小导致数据精确度不够，在此调整
	//方向向量的最小可达1e-5
	double dataAccur = 1.0e3;
	double tA[2], tB[2], tC[2];
	//当垂直
	tA[0] = (pos2[0] - pos1[0])*(nc2[0] - nc1[0])*dataAccur + (pos2[1] - pos1[1])*(nc2[1] - nc1[1])*dataAccur;
	tB[0] = (nc2[0] - nc1[0])*(pos1[0] - startPos[0])*dataAccur + (nc2[1] - nc1[1])*(pos1[1] - startPos[1])*dataAccur + (pos2[0] - pos1[0])*nc1[0] * dataAccur + (pos2[1] - pos1[1])*nc1[1] * dataAccur;
	tC[0] = (pos1[0] - startPos[0])*nc1[0] * dataAccur + (pos1[1] - startPos[1])*nc1[1] * dataAccur;
	//当平行
	tA[1] = (pos2[0] - pos1[0])*(nc2[1] - nc1[1])*dataAccur - (pos2[1] - pos1[1])*(nc2[0] - nc1[0])*dataAccur;
	tB[1] = (nc2[1] - nc1[1])*(pos1[0] - startPos[0])*dataAccur - (nc2[0] - nc1[0])*(pos1[1] - startPos[1])*dataAccur + (pos2[0] - pos1[0])*nc1[1] * dataAccur - (pos2[1] - pos1[1])*nc1[0] * dataAccur;
	tC[1] = (pos1[0] - startPos[0])*nc1[1] * dataAccur - (pos1[1] - startPos[1])*nc1[0] * dataAccur;
	for (int i = 0; i < 2; i++) {
		if (fabs(tA[i]) < 1e-5) {
			if (fabs(tB[i]) > 1e-5) {
				double st = (-1.0)*tC[i] / tB[i];
				if (st >= 0.0 && st <= 1.0) {
					Vec3 pos3 = pos1*(1.0 - st) + pos2* st;
					endPos[endPosNum].pos = pos3;
					endPos[endPosNum].edgeI = sEdge->index;
					endPos[endPosNum++].vec = pos3 - startPos;//方向明确
				}
			}
		}
		else {
			double deltaT = tB[i] * tB[i] - 4.0* tA[i] * tC[i];
			if (deltaT > 1e-7) {
				double st[2];
				st[0] = 0.5*(sqrt(fabs(deltaT)) - tB[i]) / tA[i];
				st[1] = 0.5*((-1.0)*sqrt(fabs(deltaT)) - tB[i]) / tA[i];
				for (int i = 0; i < 2; i++) {
					if (st[i] >= 0.0&&st[i] <= 1.0) {
						Vec3 pos3 = pos1*(1.0 - st[i]) + pos2*st[i];
						endPos[endPosNum].pos = pos3;
						endPos[endPosNum].edgeI = sEdge->index;
						endPos[endPosNum++].vec = pos3 - startPos;
					}
				}
			}
			else if (deltaT <= 1e-7&& deltaT >= 0.0) {
				double st = -0.5*tB[i] / tA[i];
				if (st >= 0.0 && st <= 1.0) {
					////add
					//Vec3 tempasdf = (pos1*(1.0 - st) + pos2*st - startPos).normalized();
					//Vec3 temppasdf = (nc1*(1.0 - st) + nc2*st).normalized();
					////end
					Vec3 pos3 = pos1*(1.0 - st) + pos2* st;
					endPos[endPosNum].pos = pos3;
					endPos[endPosNum].edgeI = sEdge->index;
					endPos[endPosNum++].vec = pos3 - startPos;
				}
			}
		}
	}
}
static double pointsAngle(Vec3 v1, Vec3 v2) {
	if (atan2(v1[1], v1[0]) - atan2(v2[1], v2[0]) < (-1.0)*Q_PI) {
		return atan2(v2[1], v2[0]) - (atan2(v1[1], v1[0]) + 2.0 * Q_PI);
	}
	else if (atan2(v1[1], v1[0]) - atan2(v2[1], v2[0]) > Q_PI) {
		return (atan2(v2[1], v2[0]) + 2.0 * Q_PI) - atan2(v1[1], v1[0]);
	}
	else {
		return atan2(v2[1], v2[0]) - atan2(v1[1], v1[0]);
	}
}
//通过给定三个点坐标和一个矢量以及对应的下一个点
static void calPoint(Vec3 v1, Vec3 v2, Vec3 v3, Vec3 n, Vec3 &endPos)
{
	//17.12.22 
	double dend = n[0] * (v2[1] - v1[1]) - n[1] * (v2[0] - v1[0]);
	if (fabs(dend) < 1e-6) {
		qDebug("being given");
		endPos = Vec3((v1 + v2) / 2.0);
	}
	else {
		double tempT = (n[1] * (v1[0] - v3[0]) - n[0] * (v1[1] - v3[1])) / dend;
		if (tempT >= 0.0&&tempT <= 1.0) {
			endPos = Vec3(v1*(1.0 - tempT) + v2*tempT);
		}
		else {
			qDebug("being given");
			endPos = Vec3((v1 + v2) / 2.0);
		}
	}
}
//寻找两线段交点
static bool findInterP(Vec3 v1, Vec3 v2, Vec3 u1, Vec3 u2, Vec3& target) {
	Vec3 v1v2 = v2 - v1;
	Vec3 v1u1 = u1 - v1;
	Vec3 v1u2 = u2 - v1;
	Vec3 v2u1 = u1 - v2;
	Vec3 v2u2 = u2 - v2;
	Vec3 u1u2 = u2 - u1;
	if (calParToP(v1u1, v1u2, v1v2) && calParToP(v2u1, v2u2, v1v2*(-1.0))
		&& calParToP(v1u1*(-1.0), v2u1*(-1.0), u1u2) && calParToP(v1u2*(-1.0), v2u2*(-1.0), u1u2*(-1.0))) {
		/*Vec3 temp1, temp2;
		calPoint(u1, u2, v1, v1v2, temp1);
		calPoint(v1, v2, u1, u1u2, temp2);
		if ((temp1 - temp2).norm() < 1e-5) {
		target = temp1;
		return true;
		}*/
		/*if (fabs((v1 - target).normalized()*(v2 - target).normalized() + 1.0) < 1e-5&&fabs((u1 - target).normalized()*(u2 - target).normalized() + 1.0) < 1e-5)*/
		//return true;
		calPoint(u1, u2, v1, v1v2, target);
		return true;
	}
	return false;
}
static int getLineSegmentIntersectionType(Vec3 v1, Vec3 v2, Vec3 u1, Vec3 u2) {
	Vec3 v2_v1 = v2 - v1; //流线
	Vec3 u2_u1 = u2 - u1; //网格边
	Vec3 v2_u1 = v2 - u1;
	Vec3 v2_u2 = v2 - u2;
	Vec3 u2_v1 = u2 - v1;
	Vec3 u2_v2 = u2 - v2;
	/*if ((v1.v[0] > v2.v[0] ? v1.v[0] : v2.v[0]) < (u1.v[0] < u2.v[0] ? u1.v[0] : u2.v[0]) ||
		(v1.v[1] > v2.v[1] ? v1.v[1] : v2.v[1]) < (u1.v[1] < u2.v[1] ? u1.v[1] : u2.v[1]) ||
		(u1.v[0] > u2.v[0] ? u1.v[0] : u2.v[0]) < (v1.v[0] < v2.v[0] ? v1.v[0] : v2.v[0]) ||
		(u1.v[1] > u2.v[1] ? u1.v[1] : u2.v[1]) < (v1.v[1] < v1.v[1] ? v1.v[1] : v1.v[1]))
	{
		qDebug("--1");
		return -1;
	}*/
	if (v2_v1.cross_2d(v2_u1)*v2_v1.cross_2d(v2_u2) < 0.0
		&& u2_u1.cross_2d(u2_v1)*u2_u1.cross_2d(u2_v2) < 0.0) {
		//交叉相交
		return 0;
	}
	if (v2_v1.cross_2d(v2_u1) > 0.0 && v2_v1.cross_2d(v2_u2) < 0.0
		&& u2_u1.cross_2d(u2_v1) == 0.0 && u2_u1.cross_2d(u2_v2) > 0.0) {
		return 1;
	}
	if (v2_v1.cross_2d(v2_u1) < 0.0 && v2_v1.cross_2d(v2_u2) > 0.0
		&& u2_u1.cross_2d(u2_v1) > 0.0 && u2_u1.cross_2d(u2_v2) == 0.0) {
		return 2;
	}
	if (v2_v1.cross_2d(v2_u1) == 0 && v2_v1.cross_2d(v2_u2) < 0.0
		&& u2_u1.cross_2d(u2_v1) < 0 && u2_u1.cross_2d(u2_v2) > 0.0) {
		return 3; // 半边终止于流线的情况
	}
	if (v2_v1.cross_2d(v2_u1) > 0 && v2_v1.cross_2d(v2_u2) == 0.0
		&& u2_u1.cross_2d(u2_v1) < 0 && u2_u1.cross_2d(u2_v2) > 0.0) {
		// 半边起始于流线的情况
		return 4;
	}
	if (v2_v1.cross_2d(v2_u1) < 0 && v2_v1.cross_2d(v2_u2) == 0.0
		&& u2_u1.cross_2d(u2_v2) == 0 && u2_u1.cross_2d(u2_v1) < 0.0
		) {
		//从网格顶点射出 流线出射点=半边出射点
		return 5;
	}
	if (v2_v1.cross_2d(v2_u1) == 0.0 && v2_v1.cross_2d(v2_u2) > 0.0
		&& u2_u1.cross_2d(u2_v2) == 0.0 && u2_u1.cross_2d(u2_v1) > 0.0
		) {
		//从网格顶点射出 流线出射点=半边入射点
		return 6;
	}
	if (v2_v1.cross_2d(v2_u1) == 0.0 && v2_v1.cross_2d(v2_u2) < 0.0
		&& u2_u1.cross_2d(u2_v1) == 0.0 && u2_u1.cross_2d(u2_v2) > 0.0) {
		// 流线终点=半边终点
		return 7;
	}

	if (v2_v1.cross_2d(v2_u1) > 0.0 && v2_v1.cross_2d(v2_u2) == 0.0
		&& u2_u1.cross_2d(u2_v2) > 0.0 && u2_u1.cross_2d(u2_v1) == 0.0) {
		// 流线终点=半边出射点
		return 8;
	}

	return -1;
}
//寻找三个整数的中值
static int findMidToT(int s1, int s2, int s3) {
	return s1 >= s2 ? (s2 >= s3 ? s2 : (s1 >= s3 ? s3 : s1)) : (s1 >= s3 ? s1 : (s2 >= s3 ? s3 : s2));
}
//流线的延伸
SLPoint Mesh::StreamLinePoint(const SLPoint *startPos) {
	SLPoint endPos;
	Vec3 spp = startPos->pos;
	Vec3 spv = startPos->vec.normalized();
	double spvn = startPos->vec.norm();
	int spvI = startPos->vertI;
	if (spvI != -1) {
		if (!halfVerts[spvI]->isBoundary) {
			double vAngleP1 = halfVerts[spvI]->vec.angleP();
			double pointAngle1 = findAngleToAngle(spv.angleP(), vAngleP1 / getWeight(spvI));
			Vec3 startPosDirVec = angleToNorm(pointAngle1);
			//Vec3 startPosDirVec = spv;
			int veI = halfVerts[spvI]->edgeI;
			int curVEI1 = veI;
			Vec3 startPosToV;
			do {
				Vec3 np1 = halfVerts[halfEdges[curVEI1]->vertI]->pos;
				Vec3 np2 = halfVerts[halfEdges[halfEdges[curVEI1]->nextI]->vertI]->pos;
				if (fabs((np1 - spp).normalized()*startPosDirVec - 1.0) < interDim) {
					Vec3 hhcvv = halfVerts[halfEdges[curVEI1]->vertI]->vec;
					double hhcvvA = hhcvv.angleP();
					double pointAngle2 = findAngleToAngle(spv.angleP(), hhcvvA / getWeight(spvI));
					startPosToV = angleToNorm(pointAngle2)*hhcvv.norm() + startPosDirVec*spvn;
					break;
				}
				if (calParToP(np1 - spp, np2 - spp, startPosDirVec)) {
					Vec3 asdf1 = np1 - spp;
					Vec3 asdf2 = np2 - spp;
					Vec3 auxPointP;
					Vec3 auxPointV;
					calPoint(np1, np2, spp, startPosDirVec, auxPointP);
					double ratioxy = getRotate(np1, np2, auxPointP);
					if (ratioxy > interDim && ratioxy < 1.0) {
						Vec3 npv1 = halfVerts[halfEdges[curVEI1]->vertI]->vec;
						Vec3 npv2 = halfVerts[halfEdges[halfEdges[curVEI1]->nextI]->vertI]->vec;
						double pointAngle2 = findAngleToAngle(spv.angleP(), npv1.angleP() / getWeight(spvI));
						double pointAngle3 = findAngleToAngle(spv.angleP(), npv2.angleP() / getWeight(spvI));
						auxPointV = angleToNorm(pointAngle2)*npv1.norm() *(1.0 - ratioxy) + angleToNorm(pointAngle3)*npv2.norm()*ratioxy;
						if (auxPointV.norm() > interDim) {
							startPosToV = auxPointV + startPosDirVec*spvn;
							break;
						}
					}
				}
				curVEI1 = halfEdges[halfEdges[curVEI1]->frontI]->pairI;
			} while (curVEI1 != veI && curVEI1 != -1);
			//方向找到，寻找下一个点
			if (startPosToV.norm() > interDim) {
				int curVEI2 = veI;
				do {
					Vec3 np2 = halfVerts[halfEdges[curVEI2]->vertI]->pos;
					Vec3 np3 = halfVerts[halfEdges[halfEdges[curVEI2]->nextI]->vertI]->pos;
					if (fabs((np2 - spp).normalized()*startPosToV.normalized() - 1.0) < interDim) {
						endPos.edgeI = -1;
						endPos.pos = np2;
						endPos.vec = startPosToV;
						endPos.vertI = halfEdges[curVEI2]->vertI;
						return endPos;
					}
					if (calParToP(np3 - spp, np2 - spp, startPosToV)) {
						calPoint(np3, np2, spp, startPosToV, endPos.pos);
						endPos.edgeI = halfEdges[curVEI2]->nextI;
						endPos.vec = startPosToV;
						endPos.vertI = -1;
						return endPos;
					}
					curVEI2 = halfEdges[halfEdges[curVEI2]->frontI]->pairI;
				} while (curVEI2 != veI && curVEI2 != -1);
			}
		}
	}
	else if (startPos->edgeI != -1) {
		int speI = halfEdges[startPos->edgeI]->pairI;
		if (speI != -1 && halfEdges[speI]->pairI != -1) {
			int vI[4] = { halfEdges[speI]->vertI ,halfEdges[halfEdges[speI]->nextI]->vertI ,
				halfEdges[halfEdges[speI]->pairI]->vertI ,halfEdges[halfEdges[halfEdges[speI]->pairI]->nextI]->vertI };
			Vec3 vv[4], pv[4], startPosToV[4];
			double vvn[4], pointAngle[4];
			for (int i = 0; i < 4; i++) {
				vv[i] = halfVerts[vI[i]]->vec;
				vvn[i] = vv[i].norm();
				pv[i] = halfVerts[vI[i]]->pos;
				pointAngle[i] = findAngleToAngle(spv.angleP(), vv[i].angleP() / getWeight(vI[i]));
				startPosToV[i] = (pv[i] - spp).normalized();
			}
			double ratio = 0.0;
			ratio = getRotate(pv[0], pv[2], spp);
			double weightr1 = ratio*vv[2].norm() + (1.0 - ratio)*vv[0].norm();
			Vec3 startPosDirV = angleToNorm(pointAngle[2])*vvn[2] * ratio + angleToNorm(pointAngle[0])*vvn[0] * (1.0 - ratio);
			double startPosDir = findAngleToAngle(spv.angleP(), startPosDirV.angleP());
			Vec3 startPosDirVec = angleToNorm(startPosDir);
			//确定下个点所在边							
			Vec3 auxPointV;
			for (int i = 0; i < 4; i++) {
				if (fabs(startPosToV[i] * startPosDirVec - 1.0) < 1.0e-8) {
					auxPointV = angleToNorm(pointAngle[i])*vvn[i];
					break;
				}
			}
			int arrOrderErg[4][2] = { { 0,1 },{ 1,2 },{ 0,3 },{ 3,2 } };
			if (auxPointV.norm() < interDim) {
				Vec3 auxPoint;		//辅助点	
				double ratioxy;
				for (int i = 0; i < 4; i++) {
					int index1 = arrOrderErg[i][0];
					int index2 = arrOrderErg[i][1];
					if (calParToP(startPosToV[index1], startPosToV[index2], startPosDirVec)) {
						//下个点落在该条边上
						calPoint(pv[index1], pv[index2], spp, startPosDirVec, auxPoint);
						//在此处，如果精度低导致的出错，很大可能是流线可能经过顶点了
						ratioxy = getRotate(pv[index1], pv[index2], auxPoint);
						auxPointV = angleToNorm(pointAngle[index1])*vvn[index1] * (1.0 - ratioxy) + angleToNorm(pointAngle[index2])*vvn[index2] * ratioxy;
						break;
					}
				}
			}
			if (auxPointV.norm() < interDim) {
				qDebug("no vector found(5)");
				return SLPoint();
			}
			if (auxPointV.norm() > interDim) {
				//pointAngle4 = findAngleToAngle(startPos->vec.angle(), pointAngle4);
				Vec3 startPosToV5 = auxPointV + startPosDirVec*startPosDirV.norm();
				if (fabs(startPosToV5.norm()) > interDim) {
					//确定下一个点位置
					Vec3 finalPos;		//下一个点										
					Vec3 startPosToV5N = startPosToV5.normalized();
					for (int i = 0; i < 4; i++) {
						if (fabs(startPosToV[i] * startPosToV5N - 1.0) < interDim) {
							endPos.pos = pv[i];
							endPos.vec = startPosToV5;
							endPos.edgeI = -1;
							endPos.vertI = vI[i];
							return endPos;
						}
					}
					if (calParToP(startPosToV[0], startPosToV[1], startPosToV5N)) {
						endPos.edgeI = halfEdges[speI]->nextI; //找到反向边的索引
						calPoint(pv[0], pv[1], spp, startPosToV5, finalPos);
						//确定半边
					}
					else if (calParToP(startPosToV[2], startPosToV[1], startPosToV5N)) {
						endPos.edgeI = halfEdges[halfEdges[speI]->nextI]->nextI; //找到反向边的索引
						calPoint(pv[2], pv[1], spp, startPosToV5, finalPos);
					}
					else if (calParToP(startPosToV[0], startPosToV[3], startPosToV5N)) {
						endPos.edgeI = halfEdges[halfEdges[halfEdges[speI]->pairI]->nextI]->nextI; //找到反向边的索引
						calPoint(pv[0], pv[3], spp, startPosToV5, finalPos);
						//确定半边
					}
					else if (calParToP(startPosToV[2], startPosToV[3], startPosToV5N)) {
						endPos.edgeI = halfEdges[halfEdges[speI]->pairI]->nextI; //找到反向边的索引
						calPoint(pv[2], pv[3], spp, startPosToV5, finalPos);
					}
					else {
						//出错
						return SLPoint();
					}
					endPos.pos = finalPos;
					endPos.vec = startPosToV5;
					return endPos;
				}
				else {
					//出错
					qDebug("being given");
					return SLPoint();
				}
			}
			else {
				qDebug("being given");
				return SLPoint();
			}
		}
	}
	return SLPoint();
}
//流线的获得
bool Mesh::getStreamLine(int singularI, int streamLineI, const SLPoint *edgeP, int& slpcT) {
	int streamLinePointCount = dirSL.size();
	dirSL.push_back(SLPoint(dirSV[singularI]->pos, -2, dirSV[singularI]->vertI, edgeP->vec, streamLineI));
	if (edgeP->edgeI < 0 && edgeP->vertI < 0) {
		dirSL.push_back(*edgeP);
		slpcT = 2;
		return true;
	}
	SLPoint nextPoint = *edgeP;
	while (nextPoint.vertI >= 0 || (nextPoint.edgeI >= 0 && halfEdges[nextPoint.edgeI]->pairI >= 0))
	{
		nextPoint.slI = streamLineI;
		dirSL.push_back(nextPoint);
		if (nextPoint.vertI != -1 && halfVerts[nextPoint.vertI]->isBoundary) {
			int bhhnevtc = BECI[halfVerts[nextPoint.vertI]->type].count;
			BECI[halfVerts[nextPoint.vertI]->type].count = bhhnevtc + 1;
			BECI[halfVerts[nextPoint.vertI]->type].vI[bhhnevtc % 3] = -1;
			break;
		}
		bool isEnd = false;
#if 1
		for (int j = 0; j < dirSV.size(); j++) {
			//结束条件：当流线进入任意奇点领域皆为终止。注意，此时还不存在faceI==-3的奇点
			//if (dirSV[j]->faceI >= -1) {			
			if (nextPoint.edgeI >= 0) {
				int hnepI = halfEdges[nextPoint.edgeI]->pairI;
				if (hnepI >= 0) {
					for (auto iter = dirSV[j]->bFaceI.begin(); iter != dirSV[j]->bFaceI.end(); ++iter) {
						if ((*iter) == halfEdges[hnepI]->faceI) {
							isEnd = true;
							break;
						}
					}
				}
				else {
					qDebug("error!-getStreamLine-1");
				}
			}
			else if (nextPoint.vertI >= 0) {
				for (auto iter = dirSV[j]->bFaceI.begin(); iter != dirSV[j]->bFaceI.end(); ++iter) {
					for (int k = 0; k < 3; k++) {
						if (face[*iter].index[k] == nextPoint.vertI) {
							isEnd = true;
							break;
						}
					}
					if (isEnd)break;
				}
			}
			if (isEnd) {
				isEnd = false;
				//该流线进入另一个奇点，不存储经过的面（有点懒）						
				if (dirSV[j]->svt == singularType::SV_THR || dirSV[j]->svt == singularType::SV_FIV) {
					isEnd = true;
				}
				else if (dirSV[singularI]->svt == singularType::SV_BOR) {
					Vec3 tarV = nextPoint.vec.normalized();
					Vec3 v1 = (nextPoint.pos - dirSL[dirSL.size() - 2].pos).normalized();
					Vec3 v2 = (dirSV[j]->pos - dirSL[dirSL.size() - 2].pos).normalized();
					Vec3 parV = (v1*((v1*v2)*v2.norm()) - v2).normalized();
					for (int jj = 0; jj < dirSV[j]->slc; jj++) {
						if (dirSV[j]->dir[jj].vec.normalized()*tarV < -0.866) {
							isEnd = true;
							break;
						}
					}
				}
				/*if (isFEnd) {
					dirSL.push_back(SLPoint(dirSV[j]->pos, j, -1, Vec3(), streamLineI));
				}
				if (!isEnd) { isEnd = true; break; }
				else { isEnd = false; }*/
			}
			if (isEnd) {
				dirSL.push_back(SLPoint(dirSV[j]->pos, j, -1, Vec3(), streamLineI));
				break;
			}
			//}
		}
#endif // 1
		if (!isEnd) {
			nextPoint = StreamLinePoint(&nextPoint);
#if 1
			if (nextPoint.edgeI >= 0) {
				int hnef = halfEdges[nextPoint.edgeI]->faceI;
				if (osf.find(hnef) != osf.end()) {
					if (osf[hnef].find(streamLineI) != osf[hnef].end()) {
						bool isf = false;
						for (auto iter = osf[hnef][streamLineI].begin(); iter != osf[hnef][streamLineI].end(); ++iter) {
							if (iter->second.d*nextPoint.vec.normalized() > 0.96593 &&  dirSL.size() - iter->second.pI > 3) {
								isf = true;
								break;
							}
						}
						if (isf) {
							nextPoint.vertI = -2;
							break;
						}
						else {
							osf[hnef][streamLineI][osf[hnef][streamLineI].size()].d = nextPoint.vec.normalized();
							osf[hnef][streamLineI][osf[hnef][streamLineI].size()].d = dirSL.size();
						}
					}
					else {
						std::map<int, pdStr> tm;
						tm[0].d = nextPoint.vec.normalized();
						tm[0].pI = dirSL.size();
						osf[hnef][streamLineI] = tm;
					}
				}
				else {
					std::map<int, pdStr> tm;
					tm[0].d = nextPoint.vec.normalized();
					tm[0].pI = dirSL.size();
					std::map<int, std::map<int, pdStr>> mtm;
					mtm[streamLineI] = tm;
					osf[hnef] = mtm;
				}
			}
#else
			if (dirSL.size() - streamLinePointCount > 3000) {
				nextPoint.vertI = -2;
				break;
			}
#endif // 0
			if (nextPoint.isEmpty()) {
				slpcT = -1;
				nextPoint.vertI = -3;
				return false;
			}
		}
		else {
			break;
		}
	}
	if (nextPoint.vertI == -1 && halfEdges[nextPoint.edgeI]->pairI == -1) {
		//当流线与边界相交，且不在端点上
		nextPoint.slI = streamLineI;
		dirSL.push_back(nextPoint);
		//add 18.5.15
		if (nextPoint.edgeI >= 0) {
			int bhhnevtc = BECI[halfVerts[halfEdges[nextPoint.edgeI]->vertI]->type].count;
			BECI[halfVerts[halfEdges[nextPoint.edgeI]->vertI]->type].count = bhhnevtc + 1;
			BECI[halfVerts[halfEdges[nextPoint.edgeI]->vertI]->type].vI[bhhnevtc % 3] = -1;
		}
		//end
	}
	else if (nextPoint.vertI == -2) {
		nextPoint.slI = streamLineI;
		dirSL.push_back(nextPoint);
	}
	slpcT = dirSL.size() - streamLinePointCount;
	return nextPoint.vertI == -2;
}
bool Mesh::fixBAngle(int vertI) {
	int curEdgeI = halfEdges[halfVerts[vertI]->edgeI]->nextI;
	do {
		if (fabs(CountTriAngle(halfEdges[curEdgeI]->faceI) - 2.0*Q_PI) < 1.0e-5) {
			return true;
		}
		curEdgeI = halfEdges[halfEdges[curEdgeI]->nextI]->pairI;
		if (curEdgeI == -1) {
			break;
		}
		curEdgeI = halfEdges[curEdgeI]->nextI;
	} while (true);
	if (halfEdges[halfVerts[vertI]->edgeI]->pairI != -1) {
		curEdgeI = halfEdges[halfEdges[halfVerts[vertI]->edgeI]->pairI]->frontI;
		do {
			if (fabs(CountTriAngle(halfEdges[curEdgeI]->faceI) - 2.0*Q_PI) < 1.0e-5) {
				return true;
			}
			curEdgeI = halfEdges[halfEdges[curEdgeI]->frontI]->pairI;
			if (curEdgeI == -1) {
				break;
			}
			curEdgeI = halfEdges[curEdgeI]->frontI;
		} while (true);
	}
	return false;
}
double Mesh::CountTriAngle(int faceI) {
	int sEdgeI = halfFaces[faceI]->edgeI;
	int vertITemp[3] = { halfEdges[sEdgeI]->vertI,halfEdges[halfEdges[sEdgeI]->nextI]->vertI,halfEdges[halfEdges[sEdgeI]->frontI]->vertI };
	double rotateA = 0.0;
	for (int j = 0; j < 3; j++) {
		Vec3 v1 = halfVerts[vertITemp[j]]->vec;
		Vec3 v2 = halfVerts[vertITemp[(j + 1) % 3]]->vec;
		if (fabs(v1.normalized()*v2.normalized() + 1.0) < interDim) {
			return 2.0*Q_PI;
		}
		rotateA += pointsAngle(v1, v2);
	}
	return fabs(rotateA);
}
void Mesh::getBorderSingular(int edgeI) {
	int curEdgeI = halfEdges[edgeI]->frontI;
	Vec3 orginVPos = halfVerts[halfEdges[curEdgeI]->vertI]->pos;
	Vec3 startVPos = halfVerts[halfEdges[edgeI]->vertI]->pos;
	int frontPI = halfEdges[halfEdges[edgeI]->nextI]->vertI;
	Vec3 secandVPos = halfVerts[frontPI]->pos;
	double rotateAngle = 0.0;
	//add
	int iter = 0;
	do {
		rotateAngle += acos((startVPos - orginVPos).normalized()*(secandVPos - orginVPos).normalized());
		startVPos = secandVPos;
		++iter;
		if (iter == 10) {
			qDebug("error!");
		}
		if (halfEdges[curEdgeI]->pairI != -1) {
			int hhhcpnI = halfEdges[halfEdges[curEdgeI]->pairI]->nextI;
			secandVPos = halfVerts[halfEdges[hhhcpnI]->vertI]->pos;
			curEdgeI = halfEdges[hhhcpnI]->nextI;
			frontPI = halfEdges[hhhcpnI]->vertI;
		}
		else {
			break;
		}
	} while (true);
	if (fabs(rotateAngle - Q_PI) > Q_PI / 9.0) {
		//纠正方向场		
		int hcvI = halfEdges[curEdgeI]->vertI;
		if (fixBAngle(hcvI)) {
			halfVerts[hcvI]->vec = halfVerts[hcvI]->vec *-1.0;
		}
		if (!isNonSVBor) {
			int judgeFaceI = 0;
			if (fabs(rotateAngle - Q_PI / 2.0) < Q_PI / 4.0) {
				judgeFaceI = -4;
			}
			else if (rotateAngle > Q_PI / 4.0 || 1 == 1) {
				judgeFaceI = -2;
			}
			if (judgeFaceI < 0) {
				Singular *singular = new Singular();
				singular->pos = halfVerts[hcvI]->pos;
				singular->vertI = hcvI;
				singular->faceI = judgeFaceI;
				singular->svt = judgeFaceI == -2 ? singularType::SV_BOR : singularType::SV_COR;
				int cEdgeI = halfEdges[edgeI]->nextI;
				do {
					singular->bFaceI.push_back(halfEdges[cEdgeI]->faceI);
					cEdgeI = halfEdges[halfEdges[cEdgeI]->nextI]->pairI;
					if (cEdgeI != -1) {
						cEdgeI = halfEdges[cEdgeI]->nextI;
					}
					else {
						break;
					}
				} while (true);
				if (!createSV(singular)) {
					qDebug("error!-getBorderSingular");
				}
			}
		}
	}
	halfVerts[halfEdges[curEdgeI]->vertI]->edgeI = edgeI;//使边界点保存边界边
}
//一层领域迭代
void Mesh::aloa() {
	if (vert.size() > 0) {
		int aloaSC = dirBVEdgeIndex.size();
		std::set<int> aloaSetS;
		std::set<int> aloaSetE;
		for (int i = 0; i < aloaSC; i++) {
			int vI = dirBVEdgeIndex[i][0];
			aloaSetS.insert(vI);
			//aloaArrayS[i] = dirBVEdgeIndex[i][0];
			vert[i].disValue = 0.0;
			halfVerts[vI]->aloaN = 0;
			vert[vI].aloaColor = Vec3(1.0, 0.0, 0.0);
		}
		int layerS = 1;
		Vec3 showNetColor = Vec3(1.0, 0.0, 0.0);
		while (aloaSC > 0) {
			int curAloaC = 0;
			if (layerS % 3 == 0) {
				showNetColor = Vec3(1.0, 0.0, 0.0);
			}
			else if (layerS % 3 == 1) {
				showNetColor = Vec3(0.0, 1.0, 0.0);
			}
			else {
				showNetColor = Vec3(0.0, 0.0, 1.0);
			}
			for (auto iter = aloaSetS.begin(); iter != aloaSetS.end(); ++iter) {
				if (*iter >= 0) {
					int startEI1 = halfVerts[*iter]->edgeI;
					if (startEI1 != -1) {
						int curStartEI1 = startEI1;
						int startEI2 = halfEdges[halfEdges[startEI1]->nextI]->nextI;
						if (layerS > 1) {
							//第一层另外处理
							while (startEI1 != -1) {
								int curHSN = halfEdges[startEI1]->nextI;
								int hcvI = halfEdges[curHSN]->vertI;
								if (halfVerts[hcvI]->aloaN < 0) {
									aloaSetE.insert(hcvI);
									vert[hcvI].aloaColor = showNetColor;
									halfVerts[hcvI]->aloaN = layerS;
									++curAloaC;
								}
								startEI1 = halfEdges[halfEdges[curHSN]->nextI]->pairI;
								if (startEI1 == curStartEI1) {
									break;
								}
							}
						}
						else {
							while (startEI1 != -1) {
								int curHSN = halfEdges[startEI1]->nextI;
								int hcvI = halfEdges[curHSN]->vertI;
								if (halfVerts[hcvI]->aloaN < 0) {
									aloaSetE.insert(hcvI);
									vert[hcvI].aloaColor = showNetColor;
									halfVerts[hcvI]->aloaN = layerS;
									++curAloaC;
								}
								startEI1 = halfEdges[halfEdges[curHSN]->nextI]->pairI;
								if (startEI1 == curStartEI1) {
									break;
								}
							}
							int curStartEI2 = startEI2;
							while (startEI2 != -1) {
								int curHSN = halfEdges[startEI2]->nextI;
								int hcvI = halfEdges[curHSN]->vertI;
								if (halfVerts[hcvI]->aloaN < 0) {
									aloaSetE.insert(hcvI);
									vert[hcvI].aloaColor = showNetColor;
									halfVerts[hcvI]->aloaN = layerS;
									++curAloaC;
								}
								startEI2 = halfEdges[curHSN]->pairI;
								if (startEI2 == curStartEI2) {
									break;
								}
							}
						}
					}
				}
			}
			aloaSC = curAloaC;
			aloaSetS.swap(aloaSetE);
			++layerS;
		}
		aloa_layerS = layerS - 1;
		isGenALOA = true;
	}
	else {
		isGenALOA = false;
	}
}
//标量场
void Mesh::scalarFiled() {
	if (vert.size() > 0) {
		double disV = maxPointValue - minPointValue;
		double scalarV = 0.0;
		for (int i = 0; i < vert.size(); i++) {
			scalarV = fabs((vert[i].uValue - minPointValue) / disV);
			vert[i].normColor = Vec3(scalarV, scalarV, scalarV);
		}
		isGenScalarField = true;
	}
	else {
		isGenScalarField = false;
	}
}
//生成交叉场
void Mesh::acrossField() {
	acrossF.clear();
	if (vert.size() > 0) {
		for (int i = 0; i < vert.size(); i++) {
			if (abs(halfVerts[i]->aloaN) % croLayNum == 0 && i % crolpD == 0) {
				double disVp = halfVerts[i]->vec.norm() * croBSize;
				Vec3 vPos1 = halfVerts[halfEdges[halfVerts[i]->edgeI]->vertI]->pos;
				Vec3 vPos2 = halfVerts[i]->pos;
				//double disVp = (vPos1 - vPos2).norm() / 3.0;
				double angle1 = halfVerts[i]->vec.angleP() / 4.0;
				double angle2 = fabs(angle1 > 3.0*Q_PI / 2.0 ? angle1 - 3.0*Q_PI / 2.0 : angle1 + Q_PI / 2.0);
				Vec3 vn1 = angleToNorm(angle1)*disVp;
				Vec3 vn2 = angleToNorm(angle2)*disVp;
				acrossF.push_back(vn1 + vPos2);
				acrossF.push_back(vn1*(-1.0) + vPos2);
				acrossF.push_back(vn2 + vPos2);
				acrossF.push_back(vn2*(-1.0) + vPos2);
			}
		}
		isGenAcrossField = true;
	}
	else {
		isGenAcrossField = false;
	}
}
//方向场
void Mesh::directField() {
	directF.clear();
	directHandleF.clear();
	if (vert.size() > 0) {
		for (int i = 0; i < vert.size(); i++) {
			if (abs(halfVerts[i]->aloaN) % dirLayNum == 0 && i % dirlpD == 0) {
				Vec3 vPos2 = halfVerts[i]->pos;
				Vec3 vn1 = halfVerts[i]->vec*dirBSize;// .normalized();
				double vn1n = vn1.norm();
				double vnf = halfVerts[i]->vec.angleP();
				Vec3 vn2 = angleToNorm(vnf > 3.0*Q_PI / 2.0 ? vnf - 3.0*Q_PI / 2.0 : vnf + Q_PI / 2.0);
				Vec3 vn3 = (vn1.normalized() + vn2).normalized();
				Vec3 vn4 = (vn1.normalized() - vn2).normalized();
				Vec3 endP = vn1 + vPos2;
				directF.push_back((vn3*0.7071*vn1n + vPos2)*dirHSize + endP*(1.0 - dirHSize));
				directF.push_back((vn4*0.7071*vn1n + vPos2)*dirHSize + endP*(1.0 - dirHSize));
				directF.push_back(endP);
				directHandleF.push_back(vPos2);
				directHandleF.push_back(endP);
			}
		}
		isGenDirectField = true;
	}
	else {
		isGenDirectField = false;
	}
}
//建立矩形区域及应用
void Mesh::getDFSL() {
	int col = (this->maxPos[0] - this->minPos[0]) / this->getDisQuaSize() + 3;
	int row = (this->maxPos[1] - this->minPos[1]) / this->getDisQuaSize() + 3;
	if (col < 4 || row < 4 || col*row>2e6) {
		return;
	}
	if (dfm != nullptr) {
		delete dfm;
	}
	dfm = new DisFieldMesh(minPos, maxPos, getDisQuaSize());
	if (bVEdge.size() > 0) {
		dfm->GetDomainValue(bVEdge);
		if (dfm->isGenDFM) {
			for (int i = 0; i < vert.size(); i++) {
				if (!halfVerts[i]->isBoundary) {
					double disf = dfm->GetInfoVToW(halfVerts[i]->pos, TypeField::Distance);
					maxDis = disf > maxDis ? disf : maxDis;
					vert[i].disValue = disf;
				}
				else {
					vert[i].disValue = 0.0;
				}
			}
			disSL = true;
			//color3.clear();
			//color3.insert(color3.begin(), color2.begin(), color2.end());
			for (int i = 0; i < vert.size(); i++) {
				double colorValue = vert[i].disValue / maxDis;
				vert[i].disColor = Vec3(colorValue);
			}
		}
		else {
			disSL = false;
		}
	}
	isDisSL = false;
	isGenDF = dfm->isGenDFM;
}
void Mesh::getTFSL() {
	if (dfm != nullptr) {
		this->setDisQuaSize(this->getDifQuaSize()*100.0);
		int col = (this->maxPos[0] - this->minPos[0]) / this->getDifQuaSize() + 3;
		int row = (this->maxPos[1] - this->minPos[1]) / this->getDifQuaSize() + 3;
		if (col < 4 || row < 4 || col*row>2e6) {
			return;
		}
		if (tfm != nullptr) {
			delete tfm;
		}
		tfm = new ThermalField(minPos, maxPos, getDifQuaSize(), dfm);
		if (bVSEdge.size() > 0) {
			tfm->GetDomainValue(bVSEdge);
			if (tfm->isGenTF) {
				for (int i = 0; i < vert.size(); i++) {
					//if (!halfVerts[i]->isBoundary) {
					double difm = tfm->GetInfoVToW(halfVerts[i]->pos, TypeField::Thermal);
					//maxDis = disf > maxDis ? disf : maxDis;
					//halfVerts[i]->dis = disf;
					vert[i].difValue = difm;
					if (difm < -1.0) {
						difm = -1.0;
					}
					else if (difm > 1.0) {
						//difm = 1.0;
					}
					if (!halfVerts[i]->isBoundary) {
						halfVerts[i]->vec = angleToNorm((difm));
					}
					//	}
				}
				difSL = true;
				//color4.clear();
				//color4.insert(color4.begin(), color2.begin(), color2.end());
				for (int i = 0; i < vert.size(); i++) {
					vert[i].difColor = Vec3(vert[i].difValue / (2.0*Q_PI));
				}
			}
			else {
				difSL = false;
			}
		}
		isDifSL = false;
		isGenDifF = tfm->isGenTF;
	}
}
void Mesh::remarkPSF(SLPoint* skp, int slI) {
#if 0
	if (skp->edgeI >= 0 && skp->vertI < 0) {
		int hhfI = halfEdges[skp->edgeI]->faceI;
		if (halfFaces[hhfI]->sfaceI == -1) {
			halfFaces[hhfI]->sfaceI = sf.size();
			sf.push_back(SFace(hhfI, slI, skp->vec));
		}
		else {
			sf[halfFaces[hhfI]->sfaceI].addSLI(slI, skp->vec);
		}
	}
	else if (skp->edgeI < 0 && skp->vertI >= 0 && !halfVerts[skp->vertI]->isBoundary) {
		int hsveI = halfVerts[skp->vertI]->edgeI;
		int ohsveI = hsveI;
		do {
			int hhfI = halfEdges[hsveI]->faceI;
			if (halfFaces[hhfI]->sfaceI == -1) {
				halfFaces[hhfI]->sfaceI = sf.size();
				sf.push_back(SFace(hhfI, slI, skp->vec));
			}
			else {
				sf[halfFaces[hhfI]->sfaceI].addSLI(slI, skp->vec);
			}
			hsveI = halfEdges[halfEdges[hsveI]->frontI]->pairI;
		} while (hsveI != -1 && ohsveI != hsveI);
	}
#else
	slI++;
	if (skp->edgeI >= 0 && skp->vertI < 0) {
		int hhfI = halfEdges[skp->edgeI]->faceI;
		if (halfFaces[hhfI]->sfaceI == -1) {
			halfFaces[hhfI]->sfaceI = sf.size();
			sf.push_back(SFace(hhfI, slI, skp->vec));
		}
		else {
			if (sf[halfFaces[hhfI]->sfaceI].slIs.find(slI) != sf[halfFaces[hhfI]->sfaceI].slIs.end()) {
				Vec3 curDir = sf[halfFaces[hhfI]->sfaceI].slIs.find(slI)->second;
				if (fabs(curDir.normalized()*skp->vec.normalized()) > 0.86) {
					sf[halfFaces[hhfI]->sfaceI].addSLI(slI, skp->vec);
				}
				else {
					sf[halfFaces[hhfI]->sfaceI].addSLI(-slI, skp->vec);
				}
			}
			else {
				sf[halfFaces[hhfI]->sfaceI].addSLI(slI, skp->vec);
			}
		}
	}
	else if (skp->edgeI < 0 && skp->vertI >= 0) {

		//&& !halfVerts[skp->vertI]->isBoundary
		int hsveI = halfVerts[skp->vertI]->edgeI;
		int ohsveI = hsveI;
		do {
			int hhfI = halfEdges[hsveI]->faceI;
			if (halfFaces[hhfI]->sfaceI == -1) {
				halfFaces[hhfI]->sfaceI = sf.size();
				sf.push_back(SFace(hhfI, slI, skp->vec));
			}
			else {
				if (sf[halfFaces[hhfI]->sfaceI].slIs.find(slI) != sf[halfFaces[hhfI]->sfaceI].slIs.end()) {
					Vec3 curDir = sf[halfFaces[hhfI]->sfaceI].slIs.find(slI)->second;
					if (fabs(curDir.normalized()*skp->vec.normalized()) > 0.86) {
						sf[halfFaces[hhfI]->sfaceI].addSLI(slI, skp->vec);
					}
					else {
						sf[halfFaces[hhfI]->sfaceI].addSLI(-slI, skp->vec);
					}
				}
				else {
					sf[halfFaces[hhfI]->sfaceI].addSLI(slI, skp->vec);
				}
			}
			hsveI = halfEdges[halfEdges[hsveI]->frontI]->pairI;
		} while (hsveI != -1 && ohsveI != hsveI);
	}
#endif
}
//不用简化的流线
void Mesh::reduceOSL(int slI, int& slPI) {
	if (SLS[slI]->endSingularIndex < 0) {
		int newSlSpI = slPI;
		int j;
		int slsSVI = SLS[slI]->startSingularIndex;
		for (j = SLS[slI]->startSLPointIndex; j < SLS[slI]->endSLPointIndex; j++, slPI++) {
			showSSLS.push_back(PCPoint(dirSL[j].pos, Vec3(0.5, 0.5, 0.5)));
			remarkPSF(&dirSL[j], SLSimplify.size());
		}
		showSSLS.push_back(PCPoint(dirSL[j].pos, Vec3(0.5, 0.5, 1.5)));
		Singular* singular = new Singular();
		singular->pos = dirSL[j].pos;
		singular->edgeI = dirSL[j].edgeI;
		singular->vertI = dirSL[j].vertI;
		singular->faceI = -3;
		singular->svt = singularType::SV_THR_BOR;
		singular->streamLines[0] = SLSimplify.size();
		if (!createSV(singular)) {
			qDebug("error!-SimplifyStreamLine");
		}
		//dirSV.push_back(singular);
		int ei = dirSV.size() - 1;
		int si = SLS[slI]->startSingularIndex;
		//colorSSL.push_back(colorArrays[SLS[slI]->startSingularIndex % 6]);
		double disSL = 0.0;
		for (int j = newSlSpI; j < slPI; j++) {
			disSL += (showSSLS[j].pos - showSSLS[j + 1].pos).norm();
		}
		SLSimplify.push_back(new StreamLine(si, ei, newSlSpI, slPI, false, disSL, false, true));
		for (int j = 0; j < 5; j++) {
			if (dirSV[si]->streamLines[j] == slI) {
				dirSV[si]->streamLines[j] = SLSimplify.size() - 1;
				break;
				//此处一个小技巧，改变的流线信息可能不是为需要修改的
				//由于修改流线索引后，在后续查询中可能会产生冲突，但这样处理可以规避
			}
		}
		slPI++;
	}
}
//给定的两条流线必须符合情况，由方法auxReduceSL给出
void Mesh::reduceSL(int slI1, int slI2, int& slPI) {
	SLS[slI1]->isShow = false;
	SLS[slI2]->isShow = false;
	if (SLS[slI1]->slt != SLS[slI2]->slt) {
		slI1 = slI1;
	}
	if (SLS[slI1]->slt.size() > 1) {
		SLS[slI1]->slt.erase(0);
	}
	int colI = *SLS[slI1]->slt.begin();
	double oedis = 0.0;
	double oedisT = 0.0;
	int newSlSpI = slPI;
	int endSLPI = 0;
	int endSLPIT = 0;
	int slsSVI = SLS[slI1]->startSingularIndex;
	//基于给定的两条流线能够实现简化的情况下
	if (SLS[slI1]->startSingularIndex == SLS[slI2]->startSingularIndex) {
		if (SLS[slI1]->endSingularIndex == SLS[slI2]->endSingularIndex && SLS[slI1]->endSingularIndex != -1) {
			endSLPI = SLS[slI1]->endSLPointIndex;
			endSLPIT = SLS[slI2]->endSLPointIndex;
		}
		else if (SLS[slI2]->startSingularIndex == SLS[slI2]->endSingularIndex) {
			endSLPI = getPIFsvIToslI(dirSL[SLS[slI2]->startSLPointIndex].pos, slI1, -(SLS[slI1]->startSLPointIndex + SLS[slI2]->slpc / 2)) + SLS[slI1]->startSLPointIndex;
			endSLPIT = SLS[slI2]->endSLPointIndex;
		}
		else {
			if (SLS[slI1]->slpc > 5 && SLS[slI2]->slpc > 5) {
				endSLPI = getPIFsvIToslI(dirSL[SLS[slI2]->startSLPointIndex].pos, slI1, -(SLS[slI1]->startSLPointIndex + 5)) + SLS[slI1]->startSLPointIndex;
				endSLPIT = getPIFsvIToslI(dirSL[SLS[slI1]->startSLPointIndex].pos, slI2, -(SLS[slI2]->startSLPointIndex + 5)) + SLS[slI2]->startSLPointIndex;
			}
		}
	}
	else {
		double dis1, dis2;
		int sp1 = SLS[slI1]->endSLPointIndex;
		int sp2 = SLS[slI2]->endSLPointIndex;
		while (true)
		{
			endSLPI = getPIFsvIToslI(dirSL[SLS[slI2]->startSLPointIndex].pos, slI1, sp1) + SLS[slI1]->startSLPointIndex;
			endSLPIT = getPIFsvIToslI(dirSL[SLS[slI1]->startSLPointIndex].pos, slI2, sp2) + SLS[slI2]->startSLPointIndex;
			dis1 = dis2 = 0.0;
			for (int i = SLS[slI1]->startSLPointIndex; i < endSLPI; i++) {
				dis1 += (dirSL[i].pos - dirSL[i + 1].pos).norm();
			}
			for (int i = SLS[slI2]->startSLPointIndex; i < endSLPIT; i++) {
				dis2 += (dirSL[i].pos - dirSL[i + 1].pos).norm();
			}
			if (dis1 / dis2 > 3.0) {
				sp1 = (sp1 + SLS[slI1]->startSLPointIndex) / 2;
			}
			else if (dis1 / dis2 < 0.333) {
				sp2 = (sp2 + SLS[slI2]->startSLPointIndex) / 2;
			}
			else {
				break;
			}
		}
	}
	int gapNum = endSLPI - SLS[slI1]->startSLPointIndex;
	for (int i = SLS[slI1]->startSLPointIndex; i < endSLPI; i++) {
		oedis += (dirSL[i].pos - dirSL[i + 1].pos).norm();
	}
	for (int i = SLS[slI2]->startSLPointIndex; i < endSLPIT; i++) {
		oedisT += (dirSL[i].pos - dirSL[i + 1].pos).norm();
	}
	showSSLS.push_back(PCPoint(dirSL[SLS[slI1]->startSLPointIndex].pos, slArrayColor[colI % 24]));
	++slPI;
	int auxSLp2 = endSLPIT - 1;
	double auxSLpDis2 = oedisT - (dirSL[endSLPIT].pos - dirSL[auxSLp2].pos).norm();
	double auxSLpDis3 = oedisT;
	//double auxSLpDis4 = auxSLpDis2 - (dirSL[auxSLp2 - 1].pos - dirSL[auxSLp2].pos).norm();
	double curDis = 0.0;
	for (int i = SLS[slI1]->startSLPointIndex + 1; i < endSLPI; ++i, ++slPI) {
		curDis += (dirSL[i].pos - dirSL[i - 1].pos).norm();
		double paras = curDis / oedis;
		double tempDis = (1.0 - paras)*oedisT;
		while (tempDis < auxSLpDis3 && tempDis < auxSLpDis2) {
			auxSLp2--;
			auxSLpDis3 = auxSLpDis2;
			auxSLpDis2 = auxSLpDis2 - (dirSL[auxSLp2].pos - dirSL[auxSLp2 + 1].pos).norm();
		}
		while (tempDis > auxSLpDis2 && tempDis > auxSLpDis3) {
			auxSLp2++;
			auxSLpDis2 = auxSLpDis3;
			auxSLpDis3 = auxSLpDis3 + (dirSL[auxSLp2].pos - dirSL[auxSLp2 + 1].pos).norm();
		}
		double ttemp = (tempDis - auxSLpDis2) / (auxSLpDis3 - auxSLpDis2);
		Vec3 newPv = dirSL[auxSLp2].pos*(1.0 - ttemp) + dirSL[auxSLp2 + 1].pos*ttemp;
		showSSLS.push_back(PCPoint(dirSL[i].pos*(1.0 - paras) + newPv*paras, slArrayColor[colI % 24]));
	}
	//最后一个点直接存储
	showSSLS.push_back(PCPoint(dirSL[SLS[slI2]->startSLPointIndex].pos, slArrayColor[colI % 24]));
	for (int j = SLS[slI1]->startSLPointIndex; j <= SLS[slI1]->endSLPointIndex; j++) {
		remarkPSF(&dirSL[j], SLSimplify.size());//记录流点所在三角形的大致位置
	}
	for (int k = SLS[slI2]->startSLPointIndex; k <= SLS[slI2]->endSLPointIndex; k++) {
		remarkPSF(&dirSL[k], SLSimplify.size());
	}
	double disSL = 0.0;
	for (int k = newSlSpI; k < slPI; k++) {
		disSL += (showSSLS[k].pos - showSSLS[k + 1].pos).norm();
	}
	SLSimplify.push_back(new StreamLine(SLS[slI1]->startSingularIndex, SLS[slI2]->startSingularIndex, newSlSpI, slPI, false, disSL, false, true));
	for (int k = 0; k < 5; k++) {
		if (dirSV[SLS[slI1]->startSingularIndex]->streamLines[k] == slI1) {
			dirSV[SLS[slI1]->startSingularIndex]->streamLines[k] = SLSimplify.size() - 1;
			break;
		}
	}
	for (int k = 0; k < 5; k++) {
		if (dirSV[SLS[slI2]->startSingularIndex]->streamLines[k] == slI2) {
			dirSV[SLS[slI2]->startSingularIndex]->streamLines[k] = SLSimplify.size() - 1;
			break;
		}
	}
	slPI++;
}
int Mesh::getSLIFsvIToslI(int svI, int slI, int tsvI, Vec3 pd) {
	if (svI < 0 || svI >= dirSV.size() || slI < 0 || slI >= SLS.size()) {
		return -1;
	}
	int dir = tsvI == SLS[slI]->startSingularIndex ? -1 : 1;
	int curPI = getPIFsvIToslI(dirSV[svI]->pos, slI) + SLS[slI]->startSLPointIndex;
	Vec3 jdgv = (dirSL[curPI + dir].pos - dirSL[curPI].pos).normalized();
	if (jdgv*pd < 0.259) {
		return -2;
	}
	int selSLI = -1;
	double minja = -1.0;
	for (int i = 0; i < 5; i++) {
		if (dirSV[svI]->streamLines[i] >= 0) {
			int sdssI = SLS[dirSV[svI]->streamLines[i]]->startSLPointIndex;
			Vec3 compV = (dirSL[sdssI + 1].pos - dirSL[sdssI].pos).normalized();
			if (compV*jdgv > minja) {
				selSLI = dirSV[svI]->streamLines[i];
				minja = compV*jdgv;
			}
		}
	}
	return selSLI;
}
//计算奇点svI到流线slI最近的流点（slI）
int Mesh::getPIFsvIToslI(Vec3 pos, int slI) {
	double disT = 1.0e9;
	int slPcI = -1;
	for (int i = SLS[slI]->startSLPointIndex; i <= SLS[slI]->endSLPointIndex; i++) {
		double disPoP = (dirSL[i].pos - pos).norm();
		if (disPoP < disT) {
			disT = disPoP;
			slPcI = i;
		}
	}
	return slPcI - SLS[slI]->startSLPointIndex;
}
//找出奇点处符合条件的流线索引
int Mesh::getSLIFsvv(int svI, Vec3 vec) {
	int reV = -1;
	if (svI >= 0 && svI<dirSV.size() && vec.norm() > interDim) {
		double aj = -1.0;
		for (int i = 0; i < 5; i++) {
			if (dirSV[svI]->streamLines[i] >= 0) {
				int sdssI = SLS[dirSV[svI]->streamLines[i]]->startSLPointIndex;
				Vec3 dirj = (dirSL[sdssI + 1].pos - dirSL[sdssI].pos).normalized();
				if (dirj*vec > aj) {
					reV = dirSV[svI]->streamLines[i];
					aj = dirj*vec;
				}
			}
		}
	}
	return reV;
}
//处理打转问题
//输入流线索引。输出该流线上流线点的索引
void Mesh::getPIFsvIToslI(int slI, int reSLI[]) {
	int ospI = SLS[slI]->startSLPointIndex;
	int svI = SLS[slI]->startSingularIndex;
	Vec3 jv = (dirSL[ospI].pos - dirSL[ospI + 1].pos).normalized();
	int reI = 0;
	if (dirSV[SLS[slI]->startSingularIndex]->svt == singularType::SV_THR) {
		for (int i = 0; i < 3 && reI < 2; i++) {
			if (dirSV[svI]->streamLines[i] != slI) {
				reSLI[reI++] = dirSV[svI]->streamLines[i];
			}
		}
	}
	else if (dirSV[SLS[slI]->startSingularIndex]->svt == singularType::SV_FIV) {
		for (int i = 0; i < 5 && reI < 2; i++) {
			if (dirSV[svI]->streamLines[i] != slI) {
				if ((dirSL[SLS[dirSV[svI]->streamLines[i]]->startSLPointIndex + 1].pos - dirSL[ospI].pos).normalized()*jv > 0.0) {
					reSLI[reI++] = dirSV[svI]->streamLines[i];
				}
			}
		}
	}
}
int Mesh::getPIFsvIToslI(Vec3 pos, int slI, int sp) {
	double disT = 1.0e9;
	int slPcI = -1;
	if (sp >= 0) {
		for (int i = SLS[slI]->startSLPointIndex; i <= sp; i++) {
			double disPoP = (dirSL[i].pos - pos).norm();
			if (disPoP < disT) {
				disT = disPoP;
				slPcI = i;
			}
		}
	}
	else {
		for (int i = -sp; i <= SLS[slI]->endSLPointIndex; i++) {
			double disPoP = (dirSL[i].pos - pos).norm();
			if (disPoP < disT) {
				disT = disPoP;
				slPcI = i;
			}
		}
	}
	return slPcI - SLS[slI]->startSLPointIndex;
}
bool Mesh::ReMatchFit(int slI1, int slI2, double spec) {
	int slsp1 = SLS[slI1]->startSLPointIndex;
	int slsp2 = SLS[slI2]->startSLPointIndex;
	int slep1 = getPIFsvIToslI(dirSL[slsp2].pos, slI1) + slsp1;
	int slep2 = getPIFsvIToslI(dirSL[slsp1].pos, slI2) + slsp2;
	double sdss1 = SLS[slI1]->distanceSL / SLS[slI1]->slpc;
	double sdss2 = SLS[slI2]->distanceSL / SLS[slI2]->slpc;
	//double rit = sdss1 / sdss2;
	double disStandard = 0.0;
	/*if (rit > 2.0 || rit < 0.5) {
	disStandard = (sdss1 > sdss2 ? sdss1 : sdss2)*6.0;
	}
	else {
	disStandard = (sdss1 + sdss2)*3.0;
	}*/
	if (llsValue > 0.0 && spec > 0.0) {
		disStandard = (sdss1 + sdss2)*llsValue*spec;
	}
	else {
		disStandard = (sdss1 + sdss2)*3.0;
		llsValue = 3.0;
	}
	return (SLS[slI1]->isShow && SLS[slI2]->isShow && slep2 - slsp2 > 2 && slep1 - slsp1 > 2 &&
		(dirSL[slsp1].pos - dirSL[slep1].pos).normalized()*(dirSL[slsp2].pos - dirSL[slep2].pos).normalized() < -0.866 &&
		(dirSL[slsp1].pos - dirSL[(slep1 + slsp1) / 2].pos).normalized()*(dirSL[slep2].pos - dirSL[(slep2 + slsp2) / 2].pos).normalized() > 0.866 &&
		(dirSL[slsp1].pos - dirSL[slep2].pos).norm() < disStandard && (dirSL[slsp2].pos - dirSL[slep1].pos).norm() < disStandard &&
		(((dirSL[slep2 - 1].pos - dirSL[slep2].pos).normalized() * (dirSL[slsp1 + 1].pos - dirSL[slsp1].pos).normalized() > 0.0 &&
		(dirSL[slsp2 + 1].pos - dirSL[slsp2].pos).normalized() *(dirSL[slep1 - 1].pos - dirSL[slep1].pos).normalized() > 0.0) ||
			(SLS[slI1]->startSingularIndex == SLS[slI2]->endSingularIndex || SLS[slI1]->endSingularIndex == SLS[slI2]->startSingularIndex)));
}
//判断由相似度给出的流线是否符合条件
int Mesh::MatchFit(int slI1, int slI2) {
	int si[2] = { SLS[slI2]->startSingularIndex ,SLS[slI2]->endSingularIndex };
	for (int j = 0; j < 2; j++) {
		if (si[j] >= 0) {
			for (int i = 0; i < 5; i++) {
				if (dirSV[si[j]]->streamLines[i] != slI2 && dirSV[si[j]]->streamLines[i] >= 0 && ReMatchFit(slI1, dirSV[si[j]]->streamLines[i], 1.0)) {
					return dirSV[si[j]]->streamLines[i];
				}
			}
		}
	}
	return -1;
}
void Mesh::auxInterReduceSL(int slI, int& slPI) {
	int svI = SLS[slI]->startSingularIndex;
	int evI = SLS[slI]->endSingularIndex;
	for (int i = 0; i < SLS.size(); i++) {
		if (slI != i && SLS[i]->isShow) {
			if (SLS[i]->startSingularIndex == evI && SLS[i]->endSingularIndex == svI) {
				if (JudgeComDirSL(slI, i) == 1) {
					reduceSL(slI, i, slPI);
				}
			}
		}
	}
}
void Mesh::auxBorReduceSL(int slI, int& slPI) {
	if (SLS[slI]->endSingularIndex == -1) {
		reduceOSL(slI, slPI);
	}
	else {
		qDebug("auxBorReduceSL-ssl-error!");
	}
}
void Mesh::auxReReduceSL(int slI, int& slPI) {
	std::vector<int> reSL;
	for (int i = 0; i < SLS.size(); i++) {
		if (slI != i && SLS[i]->isShow) {
			if (ReMatchFit(slI, i, 1.0)) {
				reSL.push_back(i);
			}
		}
	}
	if (reSL.size() == 0) {
		qDebug("auxReReduceSL-error!");
	}
	int minSlpc = 1e9;
	int minIndex = -1;
	for (auto iter = reSL.begin(); iter != reSL.end(); ++iter) {
		int disVpc = getPIFsvIToslI(dirSL[SLS[*iter]->startSLPointIndex].pos, slI);
		if (disVpc < minSlpc) {
			minSlpc = disVpc;
			minIndex = *iter;
		}
	}
	if (minIndex >= 0 && minSlpc != 1e9) {
		reduceSL(slI, minIndex, slPI);
	}
}
void Mesh::auxReduceSL(int slI, int& slPI) {
	std::map<int, int> reSL;
	for (int i = 0; i < SLS.size(); ++i) {
		if (slI != i && SLS[i]->isShow) {
			int act = JudgeComDirSL(slI, i);
			if (act > 0) {
				reSL[i] = act;
			}
			else if (act == 0) {
				//此处考虑的相近流线是该流线的起始点在被找流线中找到相近点的						
				if (ReMatchFit(slI, i, 1.0)) {
					reSL[i] = act;
				}
			}
		}
	}
	if (reSL.size() == 0) {
		return;
	}
	int minSlpc = 1e9;
	int minIndex = -1;
	for (auto iter = reSL.begin(); iter != reSL.end(); ++iter) {
		if (iter->second == 0 || iter->second == 2) {
			int disVpC = getPIFsvIToslI(dirSV[SLS[iter->first]->startSingularIndex]->pos, slI);
			if (disVpC < minSlpc) {
				minSlpc = disVpC;
				minIndex = iter->first;
			}
		}
		else if (iter->second == 1) {
			if (SLS[slI]->slpc < minSlpc) {
				minSlpc = SLS[slI]->slpc;
				minIndex = iter->first;
			}
		}
		else {
			qDebug("error!-auxReduceSL");
			return;
		}
	}
	if (minIndex >= 0 && minSlpc != 1e9) {
		reduceSL(slI, minIndex, slPI);
	}
}
//寻找大致四边区域边界（一类曲线）
//在流线尾奇点延伸出的五条流线中，寻找与该流线相似的那条
int Mesh::auxJudgeRSLI(int slI) {
	int svI = SLS[slI]->endSingularIndex;
	if (svI < 0)
		return-1;
	int pI = SLS[slI]->endSLPointIndex;
	Vec3 judgeV;
	if (SLS[slI]->slpc == 2) {
		judgeV = (dirSL[pI].pos - dirSL[pI - 1].pos).normalized();
	}
	else {
		judgeV = (dirSL[pI - 1].pos - dirSL[pI - 2].pos).normalized();
	}
	int reV = -1;
	double minA = -1.0;
	Vec3 op = dirSV[svI]->pos;
	for (int i = 0; i < 5; i++) {
		int dssi = dirSV[svI]->streamLines[i];
		if (dssi >= 0) {
			double am = (op - dirSL[SLS[dssi]->startSLPointIndex + 1].pos).normalized()*judgeV;
			if (am > minA) {
				minA = am;
				reV = dssi;
			}
		}
	}
	return reV;
}
bool Mesh::judgeQuaDomainSL(int v1, int v2, int v3, int v4, Vec3 poss[]) {
	for (uint32_t i = 0; i < dirSV.size(); i++) {
		if (i != v1 && i != v2 && i != v3 && i != v4) {
			for (uint8_t j = 0; j < 4; j++) {
				poss[j] = poss[j] - dirSV[i]->pos;
			}
			if (quaDomainJudge(poss)) {
				return true;
			}
		}
	}
	return false;
}
//奇点附近流线判断
//前提条件：奇点中存在流线更够与流线slI进行简化
bool Mesh::auxRSLSVLToFSLI(int slI1, int slI2) {
	if (slI1 >= 0 && slI1 < SLS.size() && slI2 >= 0 && slI2 < SLS.size()) {
		int opI = getPIFsvIToslI(dirSV[SLS[slI1]->startSingularIndex]->pos, slI2) + SLS[slI2]->startSLPointIndex;
		if (opI == SLS[slI2]->startSLPointIndex) {
			++opI;
		}
		Vec3 oop = (dirSL[opI - 1].pos - dirSL[opI].pos).normalized();
		Vec3 top = dirSV[SLS[slI1]->startSingularIndex]->pos - dirSL[opI].pos;
		Vec3 asdf = oop*((oop*top.normalized())*top.norm());
		Vec3 op = (oop*((oop*top.normalized())*top.norm()) - top).normalized();
		if (op.norm() < interDim)return true;
		Vec3 jp = (dirSL[SLS[slI1]->startSLPointIndex + 1].pos - dirSL[SLS[slI1]->startSLPointIndex].pos).normalized();
		//double jv = op*jp;
		for (int i = 0; i < 5; i++) {
			int tslI = dirSV[SLS[slI1]->startSingularIndex]->streamLines[i];
			if (tslI >= 0 && tslI != slI1) {
				Vec3 cp = (dirSL[SLS[tslI]->startSLPointIndex + 1].pos - dirSL[SLS[tslI]->startSLPointIndex].pos).normalized();
				//double jup = op*cp;
				if (/*jup > jv  &&*/ cp*op < 0.5 && calParToP(op, jp, cp)) {
					return false;
				}
			}
		}
		return true;
	}
	else {
		return false;
	}
}
void Mesh::auxReduceSLID(std::set<int>& slArray, int ci) {
	bool isGenFit = false;
	for (auto iter = slArray.begin(); iter != slArray.end(); ++iter) {
		SLS[*iter]->isShow = false;
		for (int i = SLS[*iter]->startSLPointIndex; i <= SLS[*iter]->endSLPointIndex; i++) {
			dirSL[i].color = slArrayColor[ci % 24];// Vec3(0.0, 1.0, 0.0);
		}
		if (SLS[*iter]->endSingularIndex != -1 || SLS[*iter]->isN) {
			int svI = SLS[*iter]->startSingularIndex;
			int evI = SLS[*iter]->endSingularIndex;
			std::set<int> reSL;
			double minPc = 1.0e9;
			int minPI = -1;
			singularType nslts = dirSV[svI]->svt;
			singularType nslte = SV_UND;
			if (evI >= 0 && evI < dirSV.size()) {
				nslte = dirSV[evI]->svt;
			}
			for (auto titer = slArray.begin(); titer != slArray.end(); ++titer) {
				if (*iter != *titer) {
					int tsvI = SLS[*titer]->startSingularIndex;
					int tevI = SLS[*titer]->endSingularIndex;
					//&& SLS[*iter]->distanceSL > SLS[*titer]->distanceSL
					if ((svI == tevI && auxJudgeRSLI(*titer) == *iter && SLS[*titer]->distanceSL < SLS[*iter]->distanceSL) ||
						(evI == tsvI && auxJudgeRSLI(*iter) == *titer && (SLS[*titer]->distanceSL > SLS[*iter]->distanceSL))) {
						if (minPc > SLS[*titer]->distanceSL) {
							minPc = SLS[*titer]->distanceSL;
							reSL.erase(minPI);
							reSL.insert(*titer);
							minPI = *titer;
						}
					}
					else {
						int slsp1 = SLS[*iter]->startSLPointIndex;
						int slsp2 = SLS[*titer]->startSLPointIndex;
						Vec3 opv1 = (dirSL[slsp1 + 1].pos - dirSL[slsp1].pos).normalized();
						Vec3 opv2 = (dirSL[slsp2 + 1].pos - dirSL[slsp2].pos).normalized();
						int slep1 = getPIFsvIToslI(dirSL[slsp2].pos, *iter) + slsp1;
						int slep2 = getPIFsvIToslI(dirSL[slsp1].pos, *titer) + slsp2;
						singularType slts = dirSV[tsvI]->svt;
						singularType slte = SV_UND;
						if (tevI >= 0 && tevI < dirSV.size()) {
							slte = dirSV[tevI]->svt;
						}
						if (slep1 > slsp1 && (evI == tsvI || slts == singularType::SV_BOR || nslte == singularType::SV_BOR || slep1 < SLS[*iter]->endSLPointIndex)
							&& slep2 > slsp2 && (svI == tevI || slte == singularType::SV_BOR || nslts == singularType::SV_BOR || slep2 < SLS[*titer]->endSLPointIndex)) {
							//add
							double diss = (dirSL[slsp1].pos - dirSL[slep2].pos).norm();
							double disst = 0.0;
							for (int i = slsp1; i < slep1; i++) {
								disst += (dirSL[i].pos - dirSL[i + 1].pos).norm();
							}
							if (diss > disst) {
								continue;
							}
							//end
							Vec3 jpv1, jpv2;
							if (slep1 - slsp1 > 1) {
								jpv1 = (dirSL[slep1 - 2].pos - dirSL[slep1 - 1].pos).normalized();
							}
							else {
								jpv1 = (dirSL[slep1 - 1].pos - dirSL[slep1].pos).normalized();
							}
							if (slep2 - slsp2 > 1) {
								jpv2 = (dirSL[slep2 - 2].pos - dirSL[slep2 - 1].pos).normalized();
							}
							else {
								jpv2 = (dirSL[slep2 - 1].pos - dirSL[slep2].pos).normalized();
							}
							if (jpv1*opv2 > 0.5 && jpv2*opv1 > 0.5) {
								int slep1s = (slep1 + slsp1) / 2;
								int slep2s = getPIFsvIToslI(dirSL[slep1s].pos, *titer) + slsp2;
								//int slep2s = (slep2 + slsp2) / 2;
								if (slep2s < 1 || slep2s >= dirSL.size())continue;
								jpv1 = (dirSL[slep1s].pos - dirSL[slep1s + 1].pos).normalized();
								jpv2 = (dirSL[slep2s].pos - dirSL[slep2s - 1].pos).normalized();
								if (jpv1*jpv2 > 0.5 || slep1 - slsp1 < 5 || slep2 - slsp2 < 5) {
									double tdis1, tdis2;
									tdis1 = tdis2 = 0.0;
									bool isfit = true;
									int curslp1 = slep1, curslp2 = slep2;
									for (int i = slsp1 + 1; i < slep1; ++i) {
										int curpI = getPIFsvIToslI(dirSL[i].pos, *titer) + slsp2;
										if (curpI > curslp2 || curpI < 0) {
											isfit = false;
											break;
										}
										else {
											curslp2 = curpI;
										}
									}
									if (isfit) {
										for (int i = slsp2 + 1; i < slep2; ++i) {
											int curpI = getPIFsvIToslI(dirSL[i].pos, *iter) + slsp1;
											if (curpI > curslp1 || curpI < 0) {
												isfit = false;
												break;
											}
											curslp1 = curpI;
										}
									}
									if (isfit && auxRSLSVLToFSLI(*iter, *titer)) {
										reSL.insert(*titer);
									}
								}
							}
						}
						/*else if (svI == 16) {
							reSL.insert(*titer);
						}*/
					}
				}
			}
			double minDisls1 = 1.0e9;
			double minDisls2 = 1.0e9;
			double minSlpd = 1.0e9;
			int minIndex = -1;
			int nearSLPI = -1;
			for (auto itert = reSL.begin(); itert != reSL.end(); ++itert) {
				Vec3 pts = dirSL[SLS[*itert]->startSLPointIndex].pos;
				int disVpc;
				if (SLS[*itert]->startSingularIndex == SLS[*itert]->endSingularIndex && SLS[*itert]->startSingularIndex == SLS[*iter]->startSingularIndex) {
					disVpc = SLS[*iter]->endSLPointIndex;	//无奈之举
				}
				else {
					disVpc = getPIFsvIToslI(pts, *iter) + SLS[*iter]->startSLPointIndex;
				}
				Vec3 ps = dirSL[SLS[*iter]->startSLPointIndex].pos;
				int disVpct;
				if (SLS[*iter]->startSingularIndex == SLS[*iter]->endSingularIndex) {
					disVpct = SLS[*itert]->startSLPointIndex;
				}
				else {
					disVpct = getPIFsvIToslI(ps, *itert) + SLS[*itert]->startSLPointIndex;
				}
				double curDis = 0.0;
				for (int i = SLS[*iter]->startSLPointIndex; i < disVpc; ++i) {
					curDis += (dirSL[i].pos - dirSL[i + 1].pos).norm();
				}
				//bool asdf = ((dirSL[disVpc].pos - pts).norm() < minDisls1 && (dirSL[disVpct].pos - ps).norm() < minDisls2);
				if (curDis < minSlpd) {//|| (asdf && abs(disVpc - nearSLPI) < 4)) {
					minSlpd = curDis;
					minIndex = *itert;
					minDisls1 = (dirSL[disVpc].pos - pts).norm();
					minDisls2 = (dirSL[disVpct].pos - ps).norm();
					nearSLPI = disVpc;
				}
			}
			if (minIndex >= 0) {
				SLS[*iter]->objSSL = minIndex;
				SLS[*iter]->slt.insert(ci);
				SLS[minIndex]->objSSL = *iter;
				SLS[minIndex]->slt.insert(ci);
				SLS[minIndex]->isShow = false;
			}
		}
	}
}
void Mesh::auxReduceSLI(int slI, std::set<int>& slArray) {
	std::set<int> iterSLArray;
	iterSLArray.insert(slI);
	bool isCon = true;
	while (isCon)
	{
		int iterslarraySize = iterSLArray.size();
		isCon = false;
		std::set<int> titerSLArray;
		for (auto iter = iterSLArray.begin(); iter != iterSLArray.end(); ++iter) {
			//SLS[*iter]->isShow = false;
			if (*iter == 4) {
				slI = slI;
			}
			if (SLS[*iter]->endSingularIndex >= 0) {
				int resl = auxJudgeRSLI(*iter);
				if (resl >= 0 && SLS[resl]->isShow) {
					titerSLArray.insert(resl);
					isCon = true;
				}
			}
			if (SLS[*iter]->isN) {
				int reSLI[2];
				getPIFsvIToslI(*iter, reSLI);
				for (int i = 0; i < 2; i++) {
					if (reSLI[i] < SLS.size() && reSLI[i] >= 0) {
						titerSLArray.insert(reSLI[i]);
						isCon = true;
					}
				}
			}
			for (int i = 0; i < SLS.size(); i++) {
				if (SLS[i]->endSingularIndex == SLS[*iter]->startSingularIndex) {
					int resl = auxJudgeRSLI(i);
					if (resl == *iter) {
						titerSLArray.insert(i);
						isCon = true;
					}
				}
			}
		}
		iterSLArray.insert(titerSLArray.begin(), titerSLArray.end());
		titerSLArray.clear();
		for (auto iter1 = iterSLArray.begin(); iter1 != iterSLArray.end(); ++iter1) {
			int svI = SLS[*iter1]->startSingularIndex;
			int evI = SLS[*iter1]->endSingularIndex;
			//add
			if (*iter1 == 32) {
				svI = svI;
			}
			//end
			if (SLS[*iter1]->isN) {
				for (auto iter2 = iterSLArray.begin(); iter2 != iterSLArray.end(); ++iter2) {
					if (*iter2 != *iter1) {
						if (SLS[*iter2]->startSingularIndex != svI) {
							int slpI = getPIFsvIToslI(dirSV[SLS[*iter2]->startSingularIndex]->pos, *iter1) + SLS[*iter1]->startSLPointIndex;
							if (slpI < dirSL.size() - 1) {
								Vec3 vj = (dirSL[slpI].pos - dirSL[slpI + 1].pos).normalized();
								int slIt1 = getSLIFsvv(SLS[*iter2]->startSingularIndex, vj);
								int slIt2 = getSLIFsvv(SLS[*iter2]->startSingularIndex, vj*-1.0);
								if (slIt1 >= 0 && slIt1 < SLS.size()) {
									titerSLArray.insert(slIt1);
									isCon = true;
								}
								if (slIt2 >= 0 && slIt2 < SLS.size()) {
									titerSLArray.insert(slIt2);
									isCon = true;
								}
							}
						}
					}
				}
			}
			else if (evI != -1 /*&& evI != svI*/) {
				for (auto iter2 = iterSLArray.begin(); iter2 != iterSLArray.end(); ++iter2) {
					int asvI = SLS[*iter2]->startSingularIndex;
					int aevI = SLS[*iter2]->endSingularIndex;
					if ((asvI != svI && aevI != evI) || (asvI != evI && aevI != svI)) {
						double dist1, dist2;
						dist1 = SLS[*iter1]->distanceSL;
						dist2 = SLS[*iter2]->distanceSL;
						int curSLI1 = *iter1;
						int curSLI2 = *iter2;
						int taevI = -1;
						int tsvI = -1;
						int tsvp = -1;
						Vec3 slv1, slv2;
						if (asvI == evI && aevI != svI && auxJudgeRSLI(curSLI1) == curSLI2) {
							taevI = aevI;
							tsvI = svI;
							tsvp = asvI;
						}
						else if (asvI != evI && aevI == svI &&auxJudgeRSLI(curSLI2) == curSLI1) {
							taevI = asvI;
							tsvI = evI;
							tsvp = aevI;
						}
						else if (evI == aevI && svI != asvI && evI != -1 && auxJudgeRSLI(curSLI1) == auxJudgeRSLI(curSLI2)) {
							taevI = asvI;
							tsvI = svI;
							tsvp = evI;
						}
						if ((taevI != -1 || tsvI != -1)) {
							int pI1, pI2;
							SLS[curSLI1]->getSLDir(tsvp, pI1, pI2);
							Vec3 slVJudge1 = (dirSL[pI1].pos - dirSL[pI2].pos).normalized();
							SLS[curSLI2]->getSLDir(tsvp, pI1, pI2);
							Vec3 slVJudge2 = (dirSL[pI1].pos - dirSL[pI2].pos).normalized();
							if (slVJudge1*slVJudge2 < 0.259) {
								continue;
							}
							std::vector<int> saveFSLI;
							int maSLI = *iter1;
							while (taevI != tsvI) {
								if (dist1 > dist2) {
									int selSLI = -1;
									if (taevI < 0) {
										selSLI = -2;
									}
									else {
										SLS[curSLI2]->getSLDir(taevI, pI1, pI2);
										Vec3 pd = (dirSL[pI1].pos - dirSL[pI2].pos).normalized();
										selSLI = getSLIFsvIToslI(taevI, curSLI1, tsvI, pd);
									}
									if (selSLI == -2) {
										bool maj = false;
										while (true)
										{
											int masvI = SLS[maSLI]->startSingularIndex;
											for (int mai = 0; mai < SLS.size(); ++mai) {
												if (mai != maSLI && SLS[mai]->endSingularIndex == masvI && SLS[mai]->startSingularIndex != SLS[maSLI]->endSingularIndex) {
													int majsI = auxJudgeRSLI(mai);
													titerSLArray.erase(selSLI);
													if (majsI == maSLI) {
														if (maSLI == curSLI1) {
															dist1 += SLS[mai]->distanceSL - SLS[curSLI1]->distanceSL;
															curSLI1 = mai;
															tsvI = SLS[curSLI1]->startSingularIndex;
															selSLI = curSLI1;
														}
														else {
															dist2 += SLS[mai]->distanceSL - SLS[curSLI2]->distanceSL;
															curSLI2 = mai;
															taevI = SLS[curSLI2]->startSingularIndex;
															selSLI = curSLI2;
														}
														titerSLArray.insert(mai);
														isCon = true;
														maj = true;
														//maSLI = mai;	//不需要修改
														break;
													}
												}
											}
											if (maj)break;
											if (!maj && saveFSLI.size() > 1) {
												double tdis = dist2;
												dist2 = dist1;
												dist1 = tdis;
												break;
												/*titerSLArray.erase(selSLI);
												maSLI = saveFSLI[saveFSLI.size() - 1];
												dist2 -= SLS[curSLI2]->distanceSL;
												saveFSLI.pop_back();
												maSLI = saveFSLI[saveFSLI.size() - 1];
												selSLI = maSLI;
												curSLI2 = maSLI;
												taevI = SLS[selSLI]->endSingularIndex;*/
											}
											else {
												selSLI = -2;
												break;
											}
										}
										if (selSLI == -2) {
											break;
										}
										else {
											continue;
										}
									}
									if (selSLI < 0) {
										break;
									}
									else
										if (selSLI >= 0) {
											if (iterSLArray.find(selSLI) == iterSLArray.end()) {
												if (selSLI == 12) {
													selSLI = 12;
												}
												saveFSLI.push_back(selSLI);
												titerSLArray.insert(selSLI);
												isCon = true;
											}
										}
									taevI = SLS[selSLI]->endSingularIndex;
									dist2 += SLS[selSLI]->distanceSL;
									curSLI2 = selSLI;
									maSLI = curSLI2;
								}
								else {
									int auxTemp = taevI;
									taevI = tsvI;
									tsvI = auxTemp;
									auxTemp = curSLI1;
									curSLI1 = curSLI2;
									curSLI2 = auxTemp;
									double disTemp = dist1;
									dist1 = dist2;
									dist2 = disTemp;
								}
							}
						}
					}
				}
			}
			else if (evI == svI) {
				int aslI = auxJudgeRSLI(*iter1);
				//此处属于自交状态
			}
		}
		iterSLArray.insert(titerSLArray.begin(), titerSLArray.end());
		if (iterSLArray.size() == iterslarraySize) {
			break;
		}
	}
	int asdf = -2;
	if (iterSLArray.find(13) != iterSLArray.end()) {
		asdf = -1;
	}
	slArray.clear();
	slArray.insert(iterSLArray.begin(), iterSLArray.end());
}
//判断两条流线是否伪共线
int Mesh::JudgeComDirSL(int slI1, int slI2) {
	double dim = 0.866;
	Vec3 nj[2], njh[2];
	int reValue = 0;
	if (SLS[slI1]->startSingularIndex == SLS[slI2]->startSingularIndex) {
		if (SLS[slI1]->endSingularIndex == SLS[slI2]->endSingularIndex && SLS[slI1]->endSingularIndex >= 0) {
			njh[1] = (dirSL[SLS[slI1]->startSLPointIndex + int(SLS[slI1]->slpc / 2)].pos - dirSL[SLS[slI1]->startSLPointIndex].pos).normalized();
			njh[0] = (dirSL[SLS[slI2]->startSLPointIndex + int(SLS[slI2]->slpc / 2)].pos - dirSL[SLS[slI2]->endSLPointIndex].pos).normalized();
			if (njh[0] * njh[1] > dim) {
				return 1;
			}
			else {
				return -1;
			}
		}
		else {
			if (SLS[slI2]->slpc > 5 && SLS[slI1]->slpc > 5) {
				double judgeDis1 = (dirSL[SLS[slI2]->startSLPointIndex + 5].pos - dirSL[SLS[slI2]->startSLPointIndex].pos).norm();
				double judgeDis2 = (dirSL[SLS[slI1]->startSLPointIndex + 5].pos - dirSL[SLS[slI1]->startSLPointIndex].pos).norm();
				int judgePI1 = getPIFsvIToslI(dirSL[SLS[slI1]->startSLPointIndex].pos, slI2, -(SLS[slI2]->startSLPointIndex + 5));
				int judgePI2 = getPIFsvIToslI(dirSL[SLS[slI2]->startSLPointIndex].pos, slI1, -(SLS[slI1]->startSLPointIndex + 5));
				if ((dirSL[judgePI1 + SLS[slI2]->startSLPointIndex].pos - dirSL[SLS[slI1]->startSLPointIndex].pos).norm() < judgeDis1 &&
					(dirSL[judgePI2 + SLS[slI1]->startSLPointIndex].pos - dirSL[SLS[slI2]->startSLPointIndex].pos).norm() < judgeDis2) {
					double area1, area2;
					area1 = area2 = 0.0;
					int endI = SLS[slI1]->startSLPointIndex + judgePI2;
					for (int i = SLS[slI1]->startSLPointIndex; i < endI; i++) {
						area1 += dirSL[i].pos.Outer_product(dirSL[i + 1].pos);
					}
					area1 += dirSL[endI].pos.Outer_product(dirSL[SLS[slI1]->startSLPointIndex].pos);
					endI = SLS[slI2]->startSLPointIndex + judgePI1;
					for (int i = SLS[slI2]->startSLPointIndex; i < endI; i++) {
						area2 += dirSL[i].pos.Outer_product(dirSL[i + 1].pos);
					}
					area2 += dirSL[endI].pos.Outer_product(dirSL[SLS[slI2]->startSLPointIndex].pos);
					if (area1*area2 < 0.0) {
						//此处使用一半的面积作为类极值
						if (fabs(area1 + area2) < fabs(area1) / 2.0 && fabs(area1 + area2) < fabs(area2) / 2.0) {
							return 1;
						}
					}
				}
			}
			//两结束端点在其他处
			return -1;
		}
	}
	else if (SLS[slI1]->startSingularIndex == SLS[slI2]->endSingularIndex && (SLS[slI1]->endSingularIndex == SLS[slI2]->startSingularIndex)) {
		nj[1] = Vec3(1.0, 0.0, 0.0);
		nj[0] = Vec3(1.0, 0.0, 0.0);
		njh[1] = (dirSL[SLS[slI1]->startSLPointIndex + int(SLS[slI1]->slpc / 2)].pos - dirSL[SLS[slI1]->startSLPointIndex].pos).normalized();
		njh[0] = (dirSL[SLS[slI2]->startSLPointIndex + int(SLS[slI2]->slpc / 2)].pos - dirSL[SLS[slI2]->endSLPointIndex].pos).normalized();
		reValue = 1;
	}
	else if (SLS[slI1]->startSingularIndex == SLS[slI2]->endSingularIndex) {
		if (SLS[slI2]->slpc < 5 || (SLS[slI2]->distanceSL < SLS[slI1]->distanceSL && SLS[slI1]->slpc >= SLS[slI2]->slpc)) {
			nj[1] = (dirSL[SLS[slI1]->startSLPointIndex + SLS[slI2]->slpc].pos - dirSL[SLS[slI1]->startSLPointIndex].pos).normalized();
			nj[0] = (dirSL[SLS[slI2]->startSLPointIndex].pos - dirSL[SLS[slI2]->endSLPointIndex].pos).normalized();
			njh[1] = (dirSL[SLS[slI1]->startSLPointIndex + int(SLS[slI2]->slpc / 2)].pos - dirSL[SLS[slI1]->startSLPointIndex].pos).normalized();
			njh[0] = (dirSL[SLS[slI2]->startSLPointIndex + int(SLS[slI2]->slpc / 2)].pos - dirSL[SLS[slI2]->endSLPointIndex].pos).normalized();
			reValue = 2;
		}
		else {
			reValue = -1;
		}
	}
	else if (SLS[slI1]->endSingularIndex == SLS[slI2]->startSingularIndex) {
		if (SLS[slI2]->distanceSL > SLS[slI1]->distanceSL) {
			int slspseI[4] = { SLS[slI1]->startSLPointIndex ,SLS[slI1]->endSLPointIndex ,SLS[slI2]->startSLPointIndex ,SLS[slI2]->endSLPointIndex };
			Vec3 incidentTemp = SLS[slI1]->slpc > 2 ? (dirSL[slspseI[1] - 1].pos - dirSL[slspseI[1] - 2].pos).normalized() : (dirSL[slspseI[1]].pos - dirSL[slspseI[0]].pos).normalized();
			Vec3 outgoTemp = SLS[slI2]->slpc > 2 ? (dirSL[slspseI[2]].pos - dirSL[slspseI[2] + 1].pos).normalized() : (dirSL[slspseI[2]].pos - dirSL[slspseI[3]].pos).normalized();
			if (incidentTemp*outgoTemp > 0.09 && SLS[slI1]->slpc / 2 < SLS[slI2]->slpc) {
				nj[1] = (dirSL[slspseI[1]].pos - dirSL[slspseI[0]].pos).normalized();
				int slpcI = getPIFsvIToslI(dirSL[slspseI[0]].pos, slI2);
				nj[0] = (dirSL[slspseI[2]].pos - dirSL[slpcI + slspseI[2]].pos).normalized();
				njh[1] = (dirSL[slspseI[0] + int(SLS[slI1]->slpc / 2)].pos - dirSL[slspseI[0]].pos).normalized();
				njh[0] = (dirSL[slspseI[2] + int(SLS[slI1]->slpc / 2)].pos - dirSL[slspseI[2] + slpcI].pos).normalized();
				reValue = 1;
			}
			else {
				reValue = -1;
			}
		}
		else {
			reValue = -1;
		}
	}
	if (reValue > 0 && SLS[slI2]->slpc >= 5 && (nj[0] * nj[1] < dim || njh[0] * njh[1] < dim)) {
		reValue = -1;
	}
	return reValue;
}
void Mesh::ReduceSLToArrayClear() {
	std::vector<SFace>().swap(sf);
	for (int i = 0; i < face.size(); i++) {
		halfFaces[i]->sfaceI = -1;
	}
	for (auto iter = SLSimplify.begin(); iter != SLSimplify.end();) {
		delete *iter;
		iter = SLSimplify.erase(iter);
	}
	std::vector <PCPoint>().swap(showSSLS);
	dirSV.clear();
	for (auto iter = dirSV.begin(); iter != dirSV.end();) {
		delete *iter;
		iter = dirSV.erase(iter);
	}
	for (auto iter = auxDirSV.begin(); iter != auxDirSV.end(); iter++) {
		Singular* sv = new Singular();
		sv->edgeI = (*iter)->edgeI;
		for (int i = 0; i < (*iter)->bFaceI.size(); i++) {
			sv->bFaceI.push_back((*iter)->bFaceI[i]);
		}
		//sv->bFaceI.assign((*iter)->bFaceI.begin(), (*iter)->bFaceI.end());
		sv->faceI = (*iter)->faceI;
		sv->setColor();
		sv->pos = (*iter)->pos;
		sv->svt = (*iter)->svt;
		for (int i = 0; i < 5; i++) {
			sv->streamLines[i] = (*iter)->streamLines[i];
		}
		sv->vertI = (*iter)->vertI;
		dirSV.push_back(sv);
	}
	for (int i = 0; i < SLS.size(); i++) {
		SLS[i]->isShow = true;
		SLS[i]->isSelect = false;
		//SLS[i]->slt = SLS[i]->startSingularIndex;
		//SLS[i]->objSSL = -1;
		double dis = 0.0;
		for (int j = SLS[i]->startSLPointIndex; j < SLS[i]->endSLPointIndex; j++) {
			dis += (dirSL[j].pos - dirSL[j + 1].pos).norm();
		}
		SLS[i]->distanceSL = dis;
	}
	/*for (int i = 0; i < dirSL.size(); i++) {
		dirSL[i].color = Vec3(0.0, 0.0, 1.0);
	}*/
	isGenSSL = false;
	isGenQua = false;
	isFourMesh = false;
	isFinish = false;
	isQuaSmooth = false;
	selectClear();
	selectSLPI = 0;
}
//参数记录对应流线简化对象（anyone）
//更新流线参数reduceObjSLI
void Mesh::ReduceSLToArrayI() {
	ReduceSLToArrayClear();
	int slPI = 0;
	for (int i = 0; i < SLS.size(); i++) {
		if (SLS[i]->isShow) {
			if (SLS[i]->objSSL >= 0 && SLS[SLS[i]->objSSL]->objSSL == i) {
				reduceSL(i, SLS[i]->objSSL, slPI);
			}
			else if (SLS[i]->endSingularIndex < 0) {
				auxBorReduceSL(i, slPI);
			}
		}
	}
	isGenSSL = true;
}
void Mesh::ReduceSLToArray() {
	ReduceSLToArrayClear();
	int slPI = 0;
	//进行两次遍历：存在第一次遍历无法确定的情况	
	//单流（存在间接单流）
	for (int i = 0; i < SLS.size(); i++) {
		if (SLS[i]->isShow) {
			auxReduceSL(i, slPI);
		}
	}
	//互不流
	for (int i = 0; i < SLS.size(); i++) {
		if (SLS[i]->isShow && SLS[i]->endSingularIndex != -1) {
			auxReReduceSL(i, slPI);
		}
	}
	for (int i = 0; i < SLS.size(); i++) {
		if (SLS[i]->isShow) {
			auxBorReduceSL(i, slPI);
		}
	}
	//isGenSingular = false;
	isGenSSL = true;
	//writeReduceSLTxt();
}
void Mesh::selecTtraps() {
	for (int i = 0; i < SLS.size(); i++) {
		if (SLS[i]->isShow) {
			if (SLS[i]->endSingularIndex == -1) {
				reduceOSL(i, selectSLPI);
			}
		}
	}
	//isGenSingular = false;
	isGenSSL = true;
}
//elext sim
void Mesh::selectSimSL() {
	if (selectSLIndex[0] >= 0 && selectSLIndex[0] < SLS.size() && selectSLIndex[1] >= 0 && selectSLIndex[1] < SLS.size()) {
		reduceSL(selectSLIndex[0], selectSLIndex[1], selectSLPI);
		for (int i = 0; i < 2; i++) {
			for (int j = SLS[selectSLIndex[i]]->startSLPointIndex; j <= SLS[selectSLIndex[i]]->endSLPointIndex; j++) {
				dirSL[j].color = Vec3(0.5, 0.5, 0.5);
			}
			dirSV[SLS[selectSLIndex[i]]->startSingularIndex]->setColor();
		}
		selectSLIndex[0] = -1;
		selectSLIndex[1] = -1;
	}
}
bool Mesh::isSelectOK() {
	return selectSLIndex[0] >= 0 && selectSLIndex[0] < SLS.size() && selectSLIndex[1] >= 0 && selectSLIndex[1] < SLS.size() &&
		SLS[selectSLIndex[0]]->isShow && SLS[selectSLIndex[1]]->isShow;
}
//ene
int Mesh::getSSLC() {
	return SLSimplify.size();
}
//new
void Mesh::findSL() {
	StreamLineClear();
	for (int i = 0; i < singularTXT.size(); i++) {
		int hseI = halfVerts[singularTXT[i]]->edgeI;
		int nextEI1 = hseI;
		int nextEI2 = hseI;
		Vec3 op = halfVerts[singularTXT[i]]->pos;
		Vec3 np = halfVerts[singularTXT[i]]->vec;
		SLPoint slp;
		do {
			Vec3 p1 = halfVerts[halfEdges[nextEI2]->vertI]->pos;
			double asdf = (p1 - op).normalized()*np.normalized();
			if (fabs((p1 - op).normalized()*np.normalized() - 1.0) < interDim) {
				slp.pos = p1;
				slp.vec = np;
				slp.vertI = halfEdges[nextEI2]->vertI;
				break;
			}
			nextEI2 = halfEdges[halfEdges[nextEI2]->frontI]->pairI;
		} while (nextEI2 != -1 && nextEI2 != hseI);
		if (slp.vertI < 0) {
			do {
				int hhnnI = halfEdges[nextEI1]->nextI;
				Vec3 p1 = halfVerts[halfEdges[nextEI1]->vertI]->pos;
				Vec3 p2 = halfVerts[halfEdges[hhnnI]->vertI]->pos;
				if (calParToP(p1 - op, p2 - op, np)) {
					slp.edgeI = hhnnI;
					calPoint(p1, p2, op, np, slp.pos);
					slp.vec = np;
					break;
				}
				nextEI1 = halfEdges[halfEdges[nextEI1]->frontI]->pairI;
			} while (nextEI1 != -1 && nextEI1 != hseI);
		}
		int slI = dirSLPointCount.size();
		if (slp.edgeI == -2 && slp.vertI == -1) {
			genSL = false;
			return;
		}
		//若问题域中没有奇点，为不报错，再此处添加一个虚奇点
		if (dirSV.size() == 0) {
			dirSV.push_back(new Singular());
		}
		int gslC;
		getStreamLine(0, slI, &slp, gslC);
		if (gslC > 0) {
			dirSLPointCount.push_back(gslC);
		}
		else {
			return;
		}
	}
	int dirSLPointI = 0;
	QFile file("streamline.txt");
	int cou = dirSL.size();
	if (file.open(QIODevice::WriteOnly))
	{
		QTextStream out(&file);
		for (int i = 0; i < dirSLPointCount.size(); i++) {
			out << "---" << i << "---" << endl;
			out << halfVerts[singularTXT[i]]->pos[0] << " " << halfVerts[singularTXT[i]]->pos[1] << endl;
			for (int j = 0; j < dirSLPointCount[i]; j++) {
				if (j != 0)
					out << dirSL[cou].pos[0] << " " << dirSL[cou].pos[1] << endl;
				cou++;
			}
		}
	}
	file.close();
	genSL = true;
}
//end
QString Mesh::getSLInfoFP(Vec3 pos) {
	int selV = -1;
	int selSLI = getSelSLFP(pos, selV);
	if (selSLI >= 0 && isGenStreamLine) {
		return QString("SL:%1   selV:%2   SV:%3   EV:%4").arg(selSLI).arg(selV).arg(SLS[selSLI]->startSingularIndex).arg(SLS[selSLI]->endSingularIndex);
	}
	else {
		return "error-getSLInfoFP";
	}
}
void Mesh::StreamLineClear() {
	isGenStreamLine = false;
	dirSL.clear();
	SLS.clear();
	dirSLPointCount.clear();
	std::map<int, std::map<int, std::map<int, pdStr>>>().swap(osf);
}
void Mesh::getStreamLines() {
	StreamLineClear();
#pragma region 流线			
	for (int i = 0; i < dirSV.size(); i++) {
		if (dirSV[i]->slc > 0) {
			double bgsvS = 0.0;
			for (int j = 0; j < dirSV[i]->slc; j++) {
				bgsvS += (dirSV[i]->dir[j].pos - dirSV[i]->pos).norm();
			}
			dirSV[i]->bgSize = bgsvS / dirSV[i]->slc;
		}
	}
	for (int i = 0; i < dirSV.size(); i++) {
		for (int j = 0; j < dirSV[i]->slc; j++)
		{
			int slI = dirSLPointCount.size();
			dirSV[i]->streamLines[j] = slI;
			int gslC;
			bool isNSL = getStreamLine(i, slI, &dirSV[i]->dir[j], gslC);
			if (gslC > 1) {
				dirSLPointCount.push_back(gslC);
			}
			else {
				isGenStreamLine = false;
				return;
			}
			int dirSLCount = dirSL.size();
			int	endSingularIndex = dirSL[dirSLCount - 1].vec.norm() < interDim ? dirSL[dirSLCount - 1].edgeI : -1;
			int startSLPointIndex = dirSLCount - dirSLPointCount[slI];
			int endSLPointIndex = dirSLCount - 1;
			double slDistance = 0.0;
			for (int k = startSLPointIndex; k < endSLPointIndex; k++) {
				slDistance += (dirSL[k].pos - dirSL[k + 1].pos).norm();
			}
			SLS.push_back(new StreamLine(i, endSingularIndex, startSLPointIndex, endSLPointIndex, false, slDistance, false, isNSL));
			slsDistance(SLS.size() - 1);
		}
	}
	isGenStreamLine = true;
#pragma region 18.5.15 边界曲线
	for (auto iter1 = BECI.begin(); iter1 != BECI.end(); ++iter1) {
		if (iter1->second.count < 3) {
			//这里似乎只能是0或大于等于3
			for (int i = 0; i < 3; i++) {
				if (iter1->second.vI[i] >= 0) {
					createBESV(iter1->second.vI[i]);
				}
			}
		}
	}
	std::map<int, std::map<int, std::map<int, pdStr>>>().swap(osf);
#pragma endregion
#pragma region 对流线进行分类及简化对象的指定
	int j = 1;
	for (int i = 0; i < SLS.size(); i++) {
		if (SLS[i]->isN && SLS[i]->isShow) {
			std::set<int> slArray;
			auxReduceSLI(i, slArray);
			if (slArray.size() > 1) {
				auxReduceSLID(slArray, j++);
			}
		}
	}
	for (int i = 0; i < SLS.size(); i++) {
		if (SLS[i]->isShow) {
			std::set<int> slArray;
			auxReduceSLI(i, slArray);
			if (slArray.find(13) != slArray.end()) {
				i = i;
			}
			if (slArray.size() > 1) {
				auxReduceSLID(slArray, j++);
			}
		}
	}
#pragma endregion
#pragma region 18.6.14 复制一份奇点
	auxDirSV.clear();
	for (auto iter = auxDirSV.begin(); iter != auxDirSV.end();) {
		delete *iter;
		iter = auxDirSV.erase(iter);
	}
	for (auto iter = dirSV.begin(); iter != dirSV.end(); ++iter) {
		Singular* sv = new Singular();
		sv->edgeI = (*iter)->edgeI;
		for (int i = 0; i < (*iter)->bFaceI.size(); i++) {
			sv->bFaceI.push_back((*iter)->bFaceI[i]);
		}
		sv->faceI = (*iter)->faceI;
		sv->setColor();
		sv->pos = (*iter)->pos;
		for (int i = 0; i < 5; i++) {
			sv->streamLines[i] = (*iter)->streamLines[i];
		}
		sv->vertI = (*iter)->vertI;
		sv->svt = (*iter)->svt;
		sv->slc = (*iter)->slc;
		for (int i = 0; i < (*iter)->slc; i++) {
			sv->dir[i] = (*iter)->dir[i];
		}
		auxDirSV.push_back(sv);
	}
#pragma endregion		
}
void Mesh::slsDistance(int slI) {
	double dis = 0.0;
	if (slI > -1 && slI < SLS.size()) {
		for (int i = SLS[slI]->startSLPointIndex; i < SLS[slI]->endSLPointIndex; i++) {
			dis += (dirSL[i].pos - dirSL[i + 1].pos).norm();
		}
		SLS[slI]->distanceSL = dis;
	}
}
void Mesh::createBESV(int vI) {
	int slI = dirSLPointCount.size();
	Vec3 hifP = halfVerts[vI]->pos;
	Singular* singular = new Singular();
	singular->pos = hifP;
	singular->edgeI = -1;
	singular->vertI = vI;
	singular->faceI = -3;
	singular->slc = 1;
	singular->svt = singularType::SV_BOR;
	singular->bFaceI.push_back(halfEdges[halfVerts[vI]->edgeI]->faceI);
	singular->bFaceI.push_back(halfEdges[halfVerts[getBFrontPTodNextPI(vI)]->edgeI]->faceI);
	singular->streamLines[0] = slI;
	//初始点的获得
	int gifEI = getBFrontPTodNextPI(vI);
	Vec3 hgifP = halfVerts[gifEI]->pos;
	Vec3 hhhifevP = halfVerts[halfEdges[halfVerts[vI]->edgeI]->vertI]->pos;
	double hhaA = (hhhifevP - hgifP).angleP();
	double dir_judgment = hhaA > 3.0*Q_PI / 2.0 ? hhaA - 3.0*Q_PI / 2.0 : hhaA + Q_PI / 2.0;
	Vec3 nnn = ((hifP - hhhifevP).normalized() + (hifP - hgifP).normalized()).normalized();
	nnn = angleToNorm(dir_judgment)*nnn > 0.0 ? nnn : nnn*-1.0;
	//建立一个初始流点	
	SLPoint  ePoint;
	int cur_EI = halfVerts[gifEI]->edgeI;
	do {
		int hhhgenvI = halfEdges[halfEdges[cur_EI]->nextI]->vertI;
		int hhcfvI = halfEdges[halfEdges[cur_EI]->frontI]->vertI;
		if (fabs((halfVerts[hhcfvI]->pos - hifP).normalized()*nnn - 1.0) < 1e-7) {
			ePoint.pos = halfVerts[hhcfvI]->pos;
			ePoint.vec = nnn;
			ePoint.vertI = hhcfvI;
			break;
		}
		else if (calParToP(halfVerts[hhcfvI]->pos - hifP, halfVerts[hhhgenvI]->pos - hifP, nnn)) {
			ePoint.edgeI = halfEdges[cur_EI]->frontI;
			calPoint(halfVerts[hhcfvI]->pos, halfVerts[hhhgenvI]->pos, hifP, nnn, ePoint.pos);
			//EdgePoint[endPosNum].pos = originPoint;
			ePoint.vec = nnn;
			break;
		}
		cur_EI = halfEdges[halfEdges[cur_EI]->nextI]->pairI;
	} while (cur_EI != -1);
	singular->dir[0] = ePoint;
	//end
	if (!createSV(singular)) {
		qDebug("error!-createBESV");
	}
	int gslC;
	bool isNSL = getStreamLine(dirSV.size() - 1, slI, &ePoint, gslC);
	if (gslC > 0) {
		dirSLPointCount.push_back(gslC);
	}
	else {
		isGenStreamLine = false;
		return;
	}
	//dirSLPointCount[slI] = getStreamLine(i, slI, &EdgePoint[j]);
	int dirSLCount = dirSL.size();
	int	endSingularIndex = dirSL[dirSLCount - 1].vec.norm() < 1e-7 ? dirSL[dirSLCount - 1].edgeI : -1;
	int startSLPointIndex = dirSLCount - dirSLPointCount[slI];
	int endSLPointIndex = dirSLCount - 1;
	double disSL = 0.0;
	for (int k = startSLPointIndex; k < endSLPointIndex; k++) {
		disSL += (dirSL[k].pos - dirSL[k + 1].pos).norm();
	}
	SLS.push_back(new StreamLine(dirSV.size() - 1, endSingularIndex, startSLPointIndex, endSLPointIndex, false, disSL, false, isNSL));
	slsDistance(SLS.size() - 1);
}
//select
int Mesh::getSelectSLI(Vec3 pos, int& selI) {
	selI = -1;
	double minDis = 1.0e9;
	for (int i = 0; i < dirSV.size(); i++) {
		if ((pos - dirSV[i]->pos).norm() < minDis) {
			minDis = (pos - dirSV[i]->pos).norm();
			selI = i;
		}
	}
	int selSLI = -1;
	if (selI >= 0) {
		Vec3 judgeVec = (pos - dirSV[selI]->pos).normalized();
		for (int i = 0; i < 5; i++) {
			int slI = dirSV[selI]->streamLines[i];
			if (slI >= 0) {
				if (QuaSLInfo[slI]->startSingularIndex == selI) {
					if ((quaSLs[QuaSLInfo[slI]->startSLPointIndex + 1].pos - dirSV[selI]->pos).normalized()*judgeVec > 0.9) {
						selSLI = slI;
						break;
					}
				}
				else if (QuaSLInfo[slI]->endSingularIndex == selI) {
					if ((quaSLs[QuaSLInfo[slI]->endSLPointIndex - 1].pos - dirSV[selI]->pos).normalized()*judgeVec > 0.9) {
						selSLI = slI;
						break;
					}
				}
				else {
					qDebug("error!_selectStreamLine");
				}
			}
		}
	}
	return selSLI;
}

void Mesh::setVertTagType(StreamLine sl,const HE_edge &he)
{
	int startSingularType =dirSV[sl.startSingularIndex]->svt;
	int endSingularType = dirSV[sl.endSingularIndex]->svt;
	
	int type=0;
	if (startSingularType == SV_BOR) {
		if (endSingularType == SV_BOR) {
			type = 4;
		}
		else if (endSingularType == SV_THR_BOR) {
			type = 10;
		}
		else if (endSingularType == SV_THR) {
			type = 6;
			
		}
		else if (endSingularType == SV_FIV) {
			type = 7;
		}
	}
	else if (startSingularType == SV_THR) {
		if (endSingularType == SV_BOR ) {
			type = 6;
		}
		else if (endSingularType == SV_THR_BOR) {
			type = 8;
		}
		else if (endSingularType == SV_THR) {
			type = 2;
		}
		else if (endSingularType == SV_FIV) {
			type = 5;
		}
	}
	else if (startSingularType == SV_FIV) {
		if (endSingularType == SV_BOR ) {
			type = 7;
		}
		else if (endSingularType == SV_THR_BOR) {
			type = 9;
		}
		else if (endSingularType == SV_THR) {
			type = 5;
		}
		else if (endSingularType == SV_FIV) {
			type = 3;
		}
	}
	HE_vert *v0 = halfVerts[he.vertI];
	HE_vert *v1 = halfVerts[halfEdges[he.nextI]->vertI];
	HE_vert *v2 = halfVerts[halfEdges[he.frontI]->vertI];
	if (type == 2) {
		v0->vType == 2 || v0->vType == 0 ? v0->vType = 2 : v0->vType = 11;
		v1->vType == 2 || v1->vType == 0 ? v1->vType = 2 : v1->vType = 11;
		v2->vType == 2 || v2->vType == 0 ? v2->vType = 2 : v2->vType = 11;
	}
	else if (type == 3) {
		v0->vType == 3 || v0->vType == 0 ? v0->vType = 3 : v0->vType = 11;
		v1->vType == 3 || v1->vType == 0 ? v1->vType = 3 : v1->vType = 11;
		v2->vType == 3 || v2->vType == 0 ? v2->vType = 3 : v2->vType = 11;
	}
	else if (type == 4) {
		v0->vType == 4 || v0->vType == 0 ? v0->vType = 4 : v0->vType = 11;
		v1->vType == 4 || v1->vType == 0 ? v1->vType = 4 : v1->vType = 11;
		v2->vType == 4 || v2->vType == 0 ? v2->vType = 4 : v2->vType = 11;
	}
	else if (type == 5) {
		v0->vType == 5 || v0->vType == 0 ? v0->vType = 5 : v0->vType = 11;
		v1->vType == 5 || v1->vType == 0 ? v1->vType = 5 : v1->vType = 11;
		v2->vType == 5 || v2->vType == 0 ? v2->vType = 5 : v2->vType = 11;
	}
	else if (type == 6) {
		v0->vType == 6 || v0->vType == 0 ? v0->vType = 6 : v0->vType = 11;
		v1->vType == 6 || v1->vType == 0 ? v1->vType = 6 : v1->vType = 11;
		v2->vType == 6 || v2->vType == 0 ? v2->vType = 6 : v2->vType = 11;
	}
	else if (type == 7) {
		v0->vType == 7 || v0->vType == 0 ? v0->vType = 7 : v0->vType = 11;
		v1->vType == 7 || v1->vType == 0 ? v1->vType = 7 : v1->vType = 11;
		v2->vType == 7 || v2->vType == 0 ? v2->vType = 7 : v2->vType = 11;
	}
	else if (type == 8) {
		v0->vType == 8 || v0->vType == 0 ? v0->vType = 8 : v0->vType = 11;
		v1->vType == 8 || v1->vType == 0 ? v1->vType = 8 : v1->vType = 11;
		v2->vType == 8 || v2->vType == 0 ? v2->vType = 8 : v2->vType = 11;
	}
	else if (type == 9) {
		v0->vType == 9 || v0->vType == 0 ? v0->vType = 9 : v0->vType = 11;
		v1->vType == 9 || v1->vType == 0 ? v1->vType = 9 : v1->vType = 11;
		v2->vType == 9 || v2->vType == 0 ? v2->vType = 9 : v2->vType = 11;
	}
	else if (type == 10) {
		v0->vType == 10 || v0->vType == 0 ? v0->vType = 10 : v0->vType = 11;
		v1->vType == 10 || v1->vType == 0 ? v1->vType = 10 : v1->vType = 11;
		v2->vType == 10 || v2->vType == 0 ? v2->vType = 10 : v2->vType = 11;
	}
}

void Mesh::getFacesStreamLineGoThrough()
{
	int streamPointIndex = 0; //SLSimplify[0]->slpc;// + SLSimplify[1]->slpc + SLSimplify[2]->slpc;
	for (int i = 0; i < SLSimplify.size(); ++i) {
		Singular startSingular;
		PCPoint currentPoint;
		startSingular = *dirSV[SLSimplify[i]->startSingularIndex];
		currentPoint.pos = startSingular.pos;
	    HE_face curFace(-1, -1);
	    HE_edge curHe(-1, -1, -1, -1, -1);
		if (startSingular.faceI != -1) {
			if (startSingular.faceI!= -2) {
	           curFace = *halfFaces[startSingular.faceI];
			   curHe = *halfEdges[halfFaces[startSingular.faceI]->edgeI];
			}
			else {
				// 第一段流线从顶点射出，需要遍历顶点邻域面找到流入的面
				HE_edge he(*halfEdges[halfVerts[startSingular.vertI]->edgeI]);
				curHe = he;

				do { // 遍历点的邻域面
					if (calParToP(currentPoint.pos - halfVerts[curHe.vertI]->pos,
						currentPoint.pos - halfVerts[halfEdges[curHe.nextI]->vertI]->pos,
						currentPoint.pos - showSSLS[streamPointIndex + 1].pos)) {
						curFace = *halfFaces[curHe.faceI];
						curFace.is_selected = true;
						break;
					}
					// 找下一个面
					if (curHe.pairI != -1) {
						curHe = *halfEdges[halfEdges[curHe.pairI]->nextI];
					}
					else break;
				} while (he.index != curHe.index);
				if (curFace.index == -1) {
					HE_edge he_f(*halfEdges[he.frontI]);
					curHe = he_f;
					do { // 遍历点的邻域面
						if (calParToP(currentPoint.pos - halfVerts[halfEdges[curHe.frontI]->vertI]->pos,
							currentPoint.pos - halfVerts[halfEdges[curHe.nextI]->vertI]->pos,
							currentPoint.pos - showSSLS[streamPointIndex + 1].pos)) { // 判断流线是否在两条半边之间
							curFace = *halfFaces[curHe.faceI];
							curFace.is_selected = true;
							break;
						}
						// 找下一个面
						if (curHe.pairI != -1) {
							curHe = *halfEdges[halfEdges[curHe.pairI]->frontI];
						}
						else break;
					} while (he_f.index != curHe.index);
				}
			}
		}
		else if (startSingular.edgeI != -1) {
			curFace = *halfFaces[halfEdges[startSingular.edgeI]->faceI];
			curHe = *halfEdges[startSingular.edgeI];
		}

		else if (startSingular.vertI != -1) {
			// 第一段流线从顶点射出，需要遍历顶点邻域面找到流入的面
			HE_edge he(*halfEdges[halfVerts[startSingular.vertI]->edgeI]);
			curHe = he;

			do { // 遍历点的邻域面
				if (calParToP(currentPoint.pos - halfVerts[curHe.vertI]->pos,
					currentPoint.pos - halfVerts[halfEdges[curHe.nextI]->vertI]->pos,
					currentPoint.pos - showSSLS[streamPointIndex + 1].pos)) {
					curFace = *halfFaces[curHe.faceI];
					curFace.is_selected = true;
					break;
				}
				// 找下一个面
				if (curHe.pairI != -1) {
					curHe = *halfEdges[halfEdges[curHe.pairI]->nextI];
				}
			} while (he.index != curHe.index);
			if (curFace.index != -1) {
				HE_edge he_f(*halfEdges[he.frontI]);
				curHe = he_f;
				do { // 遍历点的邻域面
					if (calParToP(currentPoint.pos - halfVerts[halfEdges[curHe.frontI]->vertI]->pos,
						currentPoint.pos - halfVerts[halfEdges[curHe.nextI]->vertI]->pos,
						currentPoint.pos - showSSLS[streamPointIndex + 1].pos)) { // 判断流线是否在两条半边之间
						curFace = *halfFaces[curHe.faceI];
						curFace.is_selected = true;
						break;
					}
					// 找下一个面
					if (curHe.pairI != -1) {
						curHe = *halfEdges[halfEdges[curHe.pairI]->frontI];
					}
				} while (he_f.index != curHe.index);
			}
		}

		setVertTagType(*SLSimplify[i],curHe);
		for (int j = 1; j < SLSimplify[i]->slpc; ++j) {
			Vec3 u1;
			Vec3 u2;
			int r = getNextFace(curFace, curHe, showSSLS[j + streamPointIndex].pos, currentPoint.pos, u1, u2, *SLSimplify[i]);
			halfFaces[curFace.index]->is_selected = true;
			//判断下一条流线段在哪个面
			if (r == 7) {  //上一条流线段流入顶点终止，需要用下一段流线判断流入面
				HE_edge he_t(curHe);
				if (j + 1 < SLSimplify[i]->slpc) {
					curHe.flag = 7;
					if (curHe.pairI != -1) {
						curHe = *halfEdges[halfEdges[curHe.pairI]->frontI];
						do { // 遍历点的邻域面
							if (calParToP(showSSLS[j + streamPointIndex].pos - halfVerts[halfEdges[curHe.frontI]->vertI]->pos,
								showSSLS[j + streamPointIndex].pos - halfVerts[halfEdges[curHe.nextI]->vertI]->pos,
								showSSLS[j + streamPointIndex].pos - showSSLS[j + streamPointIndex + 1].pos)) { // 判断流线是否在两条半边之间
								curFace = *halfFaces[curHe.faceI];
								curFace.is_selected = true;
								//he_t = curHe;
								break;
							}
							// 找下一个面
							if (curHe.pairI != -1) {
								curHe = *halfEdges[halfEdges[curHe.pairI]->frontI];
							}
						} while (he_t.index != curHe.index);
					}
				}
			}
			if (r == 8) {
				HE_edge he_t(curHe);
				if (j + 1 < SLSimplify[i]->slpc) {
					curHe.flag = 8;
					if (curHe.pairI != -1) {
						curHe = *halfEdges[halfEdges[curHe.pairI]->nextI];
						do { // 遍历点的邻域面
							if (calParToP(showSSLS[j + streamPointIndex].pos - halfVerts[curHe.vertI]->pos,
								showSSLS[j + streamPointIndex].pos - halfVerts[halfEdges[curHe.nextI]->vertI]->pos,
								showSSLS[j + streamPointIndex].pos - showSSLS[j + streamPointIndex + 1].pos)) {
								curFace = *halfFaces[curHe.faceI];
								curFace.is_selected = true;
								//he_t = curHe;
								break;
							}
							// 找下一个面
							if (curHe.pairI != -1) {
								curHe = *halfEdges[halfEdges[curHe.pairI]->nextI];
							}
						} while (he_t.index != curHe.index);
					}
				}
			}
			currentPoint = showSSLS[j + streamPointIndex];
		}
		streamPointIndex += SLSimplify[i]->slpc;
	}
}
int Mesh::getNextFace(HE_face &curFace, HE_edge &curHe, Vec3 &v1, Vec3 &v2, Vec3 &u1, Vec3 &u2, const StreamLine &sl) {
	selectedFaces.push_back(curFace);
	setVertTagType(sl, curHe);
	halfFaces[curFace.index]->is_selected = true;
	// 只找一段流线经过的所有面
	int r = -1;
	HE_edge he(curHe);
	do {  // 循环三条半边
		u1 = halfVerts[curHe.vertI]->pos;
		u2 = halfVerts[halfEdges[curHe.frontI]->vertI]->pos;

		r = getLineSegmentIntersectionType(v1, v2, u1, u2);
		if (curHe.flag != -1) {
			curHe = *halfEdges[curHe.frontI];
			continue;
		}
		else {
			if (r == -1) {
				curHe = *halfEdges[curHe.frontI];
				continue;
			}
			else  if (r == 0) {
				//交叉相交
				if (curHe.flag == 0) { // 不处理已考虑过的半边
					curHe = *halfEdges[curHe.frontI];
					continue;
				}
				if (curHe.pairI == -1) return r;
				curHe.flag = 0;
				he = *halfEdges[curHe.pairI];// 交叉了肯定有对边
				he.flag = 0;
				curHe = he;
				curFace = *halfFaces[curHe.faceI];
				break;
			}
			else if (r == 1) {// 流线终止于边 v1在u2_u1上  
				if (curHe.pairI != -1) {
					curHe = *halfEdges[curHe.pairI];
					curFace = *halfFaces[curHe.faceI];
					curHe.flag = 2;
				}
				break;
			}
			else if (r == 2) {
				//流线起始于边 v2在u2_u1上
				if (curHe.flag == 2) {
					curHe = *halfEdges[curHe.frontI];
					continue;
				}
				curHe.flag = 2;
				curHe = *halfEdges[curHe.frontI];
			}
			else if (r == 3) {
				if (curHe.flag == 3 || curHe.flag == 4) {  // 如果上次遍历过的就不考虑
					curHe = *halfEdges[curHe.frontI];
					continue;
				}
				curHe.flag = 3;
				//半边终止于流线的情况
				HE_vert hv(*halfVerts[curHe.vertI]);
				if (curHe.pairI != -1) {
					curHe = *halfEdges[halfEdges[curHe.pairI]->frontI];
				}
				do { // 遍历点的邻域面
					if (calParToP(hv.pos - halfVerts[halfEdges[curHe.frontI]->vertI]->pos,
						hv.pos - halfVerts[halfEdges[curHe.nextI]->vertI]->pos,
						hv.pos - u1)) { // 判断流线是否在两条半边之间
						curFace = *halfFaces[curHe.faceI];
						curHe.flag = 4;
						halfEdges[curHe.nextI]->flag = 3;
						//halfEdges[curHe.nextI]->flag = 2;
						he = curHe;
						break;
					}

					// 找下一个面
					if (curHe.pairI != -1) {
						curHe = *halfEdges[halfEdges[curHe.pairI]->frontI];
					}
				} while (he.index != curHe.index);


			}
			else if (r == 4) {
				if (curHe.flag == 3 || curHe.flag == 4) {  // 如果上次遍历过的就不考虑
					curHe = *halfEdges[curHe.frontI];
					continue;
				}
				curHe.flag = 4;
				//半边起始于流线的情况
				HE_vert hv(*halfVerts[halfEdges[curHe.frontI]->vertI]); // 要绕的顶点
				if (curHe.pairI != -1) {
					curHe = *halfEdges[halfEdges[curHe.pairI]->nextI];
				}
				do { // 遍历点的邻域面
					if (calParToP(hv.pos - halfVerts[curHe.vertI]->pos,
						hv.pos - halfVerts[halfEdges[curHe.frontI]->vertI]->pos,
						hv.pos - u1)) {
						curFace = *halfFaces[curHe.faceI];
						curHe.flag = 3;
						halfEdges[curHe.frontI]->flag = 4;
						//halfEdges[curHe.frontI]->flag = 2;
						he = curHe;
						break;
					}
					// 找下一个面
					if (curHe.pairI != -1) {
						curHe = *halfEdges[halfEdges[curHe.pairI]->nextI];
					}
				} while (he.index != curHe.index);

			}
			else if (r == 5) {
				//从网格顶点射出
				if (curHe.flag == 5) {
					curHe = *halfEdges[curHe.frontI];
					continue;
				}
				curHe.flag = 5;
				curHe = *halfEdges[curHe.frontI];
			}
			else if (r == 6) {
				//从网格顶点射出
				if (curHe.flag == 6) {
					curHe = *halfEdges[curHe.frontI];
					continue;
				}
				curHe.flag = 6;
				curHe = *halfEdges[curHe.frontI];
			}
			
			else if (r == 7 || r == 8) {
				// 流入网格顶点
				//需要利用下一段流线判断
				break;
			}
			else return r;
		}


	} while (he.index != curHe.index);

	if (r == 0 || r == 2 || r == 3 || r == 4)
		r = getNextFace(curFace, curHe, v1, v2, u1, u2,sl);
	return r;
}
int Mesh::getSelSLFP(Vec3 pos, int& selI) {
	selI = -1;
	double minDis = 1.0e9;
	for (int i = 0; i < dirSV.size(); i++) {
		if ((pos - dirSV[i]->pos).norm() < minDis) {
			minDis = (pos - dirSV[i]->pos).norm();
			selI = i;
		}
	}
	if (selI >= 0 && selI < dirSV.size()) {
		int selSLI = -1;
		Vec3 judgeVec = (pos - dirSV[selI]->pos).normalized();
		for (int i = 0; i < 5; i++) {
			int slI = dirSV[selI]->streamLines[i];
			if (slI >= 0) {
				if (SLS[slI]->startSingularIndex == selI) {
					if ((dirSL[SLS[slI]->startSLPointIndex + 1].pos - dirSV[selI]->pos).normalized()*judgeVec > 0.9) {
						selSLI = slI;
						break;
					}
				}
				else if (SLS[slI]->endSingularIndex == selI) {
					if ((dirSL[SLS[slI]->endSLPointIndex - 1].pos - dirSV[selI]->pos).normalized()*judgeVec > 0.9) {
						selSLI = slI;
						break;
					}
				}
				else {
					qDebug("error!_selectStreamLine");
				}
			}
		}
		return selSLI;
	}
	return -1;
}
void Mesh::selectStreamLine(Vec3 pos) {
	int selI = -1;
	int selSLI = getSelSLFP(pos, selI);
	if (selSLI >= 0 && SLS[selSLI]->isShow) {
		int reCSL = -1;
		SLS[selSLI]->isSelect = !SLS[selSLI]->isSelect;
		if (SLS[selSLI]->isSelect) {
			for (int i = SLS[selSLI]->startSLPointIndex; i <= SLS[selSLI]->endSLPointIndex; i++) {
				dirSL[i].color = colorArrays1[selI % 6];
				dirSV[selI]->color = Vec3(1.0, 0.0, 1.0);
			}
			if (selectSLIndex[0] < 0) {
				selectSLIndex[0] = selSLI;
			}
			else if (selectSLIndex[1] < 0) {
				selectSLIndex[1] = selSLI;
			}
			else {
				reCSL = selectSLIndex[0];
				selectSLIndex[0] = selSLI;
			}
		}
		else {
			reCSL = selSLI;
			if (selectSLIndex[0] == selSLI) {
				selectSLIndex[0] = -1;
			}
			else if (selectSLIndex[1] == selSLI) {
				selectSLIndex[1] = -1;
			}
		}
		if (reCSL >= 0) {
			for (int i = SLS[reCSL]->startSLPointIndex; i <= SLS[reCSL]->endSLPointIndex; i++) {
				dirSL[i].color = Vec3(0.0, 0.0, 1.0);
			}
			SLS[reCSL]->isSelect = false;
			int svI = SLS[reCSL]->startSingularIndex;
			bool isO = false;
			for (int i = 0; i < 5; i++) {
				if (dirSV[svI]->streamLines[i] >= 0 && SLS[dirSV[svI]->streamLines[i]]->isSelect) {
					isO = true;
				}
			}
			if (!isO) {
				dirSV[svI]->setColor();
			}
		}
	}
}
void Mesh::selectClear() {
	for (int i = 0; i < dirSV.size(); i++) {
		dirSV[i]->isSelect = false;
		setColorSV(i, false);
	}
}
QString Mesh::setQBTSL(Vec3 pos, QuaBoundaryType qbt) {
	int selI;
	int selSLI = getSelectSLI(pos, selI);
	if (selSLI >= 0) {
		QuaSLInfo[selSLI]->qbt = qbt;
		for (int i = QuaSLInfo[selSLI]->startSLPointIndex; i <= QuaSLInfo[selSLI]->endSLPointIndex; i++) {
			quaSLs[i].color = qbtColor[(int)qbt];
		}
		return ("slI: " + QString::number(selSLI) + "      slPC: " + QString::number(QuaSLInfo[selSLI]->slpc) + "     svS: " + QString::number(QuaSLInfo[selSLI]->startSingularIndex) + "      svE: " + QString::number(QuaSLInfo[selSLI]->endSingularIndex));
	}
	return "error!";
}
int Mesh::setQBTSL(Vec3 sPos, Vec3 ePos) {
	Vec3 minP, maxP;
	if (sPos[0] < ePos[0]) {
		minP[0] = sPos[0];
		maxP[0] = ePos[0];
	}
	else {
		minP[0] = ePos[0];
		maxP[0] = sPos[0];
	}
	if (sPos[1] < ePos[1]) {
		minP[1] = sPos[1];
		maxP[1] = ePos[1];
	}
	else {
		minP[1] = ePos[1];
		maxP[1] = sPos[1];
	}
	bool isOK;
	QString text = QInputDialog::getText(NULL, "Input Dialog",
		"inlet-outlet-wall0-wall1",
		QLineEdit::Normal,
		"",
		&isOK);
	if (isOK) {
		QuaBoundaryType qbtT = getQBTFQS(text);
		if (qbtT != QuaBoundaryType::INDOOR) {
			int selSLNum = 0;
			for (int i = 0; i < QuaSLInfo.size(); i++) {
				Vec3 sv = dirSV[QuaSLInfo[i]->startSingularIndex]->pos;
				Vec3 ev = dirSV[QuaSLInfo[i]->endSingularIndex]->pos;
				if (isPoPMid(minP, maxP, sv) && isPoPMid(minP, maxP, ev)) {
					QuaSLInfo[i]->qbt = qbtT;
					for (int ii = QuaSLInfo[i]->startSLPointIndex; ii <= QuaSLInfo[i]->endSLPointIndex; ii++) {
						quaSLs[i].color = qbtColor[(int)qbtT];
					}
					selSLNum++;
				}
			}
			return selSLNum;
		}
	}
	return -1;
}
int Mesh::setPGQC(Vec3 pos) {
	int selI;
	int selSLI = getSelectSLI(pos, selI);
	if (selSLI >= 0 && selI >= 0 && isGenQua) {
		bool isOK;
		QString text = QInputDialog::getText(NULL, "Input segments",
			">0,0.0~1.0",
			QLineEdit::Normal,
			"",
			&isOK);
		if (isOK) {
			int slC = text.section(',', 0, 0).toInt();
			double slSegR = text.section(',', 1, 1).toDouble();
			if (slC > 0 && slSegR >= 0.0 && slSegR < 1.0) {
				QuaSLInfo[selSLI]->bpc = slC;
				QuaSLInfo[selSLI]->selSVI = selI;
				QuaSLInfo[selSLI]->selSLSegR = slSegR;
				return selI;
			}
			else {
				QuaSLInfo[selSLI]->bpc = -1;
				QuaSLInfo[selSLI]->selSVI = -2;
				QuaSLInfo[selSLI]->selSLSegR = 1.0;;
				return -1;
			}
		}
		else {
			return -1;
		}
	}
	else {
		return -1;
	}
}
//end
void Mesh::setColorSV(int svI, bool is) {
	if (svI >= 0 && svI < dirSV.size()) {
		if (is) {
			dirSV[svI]->color = dirSV[svI]->color / 2.0;
		}
		else {
			dirSV[svI]->setColor();
		}
	}
}
void Mesh::getPolePoint() {
	if (vert.size() > 0) {
		polePs.clear();
		for (int i = 0; i < vert.size(); i++) {
			int curEdgeI = halfVerts[i]->edgeI;
			double pValue = vert[i].uValue;
			double firstValue = vert[halfEdges[halfVerts[i]->edgeI]->vertI].uValue;
			bool isAllMin = pValue < firstValue ? true : false;
			bool isS = true;
			while (halfEdges[curEdgeI]->pairI != -1 && halfEdges[halfEdges[curEdgeI]->pairI]->nextI != halfVerts[i]->edgeI) {
				int nextEdgeI = halfEdges[halfEdges[curEdgeI]->pairI]->nextI;
				if (isAllMin) {
					if (vert[halfEdges[nextEdgeI]->vertI].uValue < pValue) {
						isS = false;
						break;
					}
				}
				else {
					if (vert[halfEdges[nextEdgeI]->vertI].uValue > pValue) {
						isS = false;
						break;
					}
				}
				curEdgeI = nextEdgeI;
			}
			if (isS) {
				polePs.push_back(polePoint(vert[i].pos, isAllMin ? Vec3(0.0, 1.0, 1.0) : Vec3(1.0, 0.0, 1.0)));
			}
		}
		isGenPole = true;
	}
	else {
		isGenPole = false;
	}
}
QString Mesh::setBCV(Vec3 pos) {
	int vertI = -1;
	double ar = 1.0e9;
	for (int i = 0; i < vert.size(); ++i) {
		if (halfVerts[i]->isBoundary) {
			if ((vert[i].pos - pos).norm() < ar) {
				vertI = i;
				ar = (vert[i].pos - pos).norm();
			}
		}
	}
	if (vertI >= 0) {
		Singular *singular = new Singular();
		singular->pos = halfVerts[vertI]->pos;
		singular->vertI = vertI;
		singular->faceI = -3;
		singular->svt = singularType::SV_THR_BOR;
		//halfVerts[vertI]->edgeI
		int cEdgeI = halfVerts[vertI]->edgeI;
		while (-1 != halfEdges[cEdgeI]->pairI) {
			cEdgeI = halfEdges[halfEdges[cEdgeI]->pairI]->nextI;
		}
		cEdgeI = halfEdges[cEdgeI]->frontI;
		do {
			singular->bFaceI.push_back(halfEdges[cEdgeI]->faceI);
			if (-1 != halfEdges[cEdgeI]->pairI) {
				cEdgeI = halfEdges[halfEdges[cEdgeI]->pairI]->frontI;
			}
			else {
				break;
			}
		} while (true);
		if (!createSV(singular)) {
			qDebug("error!-getBorderSingular");
		}
	}
	return QString("VertI:%1   btype:%2").arg(vertI).arg(vertI >= 0 ? halfVerts[vertI]->type : -1);
}
bool Mesh::createSV(Singular* sv) {
	for (auto iter = dirSV.begin(); iter < dirSV.end(); iter++) {
		if (((*iter)->pos - sv->pos).norm() < interDim) {
			return false;
		}
	}
	sv->setColor();
	dirSV.push_back(sv);
	return true;
}
void Mesh::findSingular() {
	if (vert.size() > 0 && halfEdges != nullptr) {
		for (int i = 0; i < face.size() * 3; i++) {
			if (halfEdges[i]->pairI == -1)
				getBorderSingular(i);	//需要提前处理
		}
		for (int i = 0; i < face.size(); i++) {
			int edgeTemp[3] = { halfFaces[i]->edgeI ,halfEdges[halfFaces[i]->edgeI]->nextI ,halfEdges[halfFaces[i]->edgeI]->frontI };
			int edge_to_vertI[3] = { -1 };
			double rotateA = 0.0;
			for (int j = 0; j < 3; j++) {
				edge_to_vertI[j] = halfEdges[edgeTemp[j]]->vertI;
				rotateA += pointsAngle(halfVerts[halfEdges[edgeTemp[j]]->vertI]->vec, halfVerts[halfEdges[edgeTemp[(j + 1) % 3]]->vertI]->vec);
			}
			bool isExistSV = false;
			for (int j = 0; j < 3; j++) {
				if (halfVerts[edge_to_vertI[j]]->vec.norm() < interDim) {
					Singular *singular = new Singular();
					singular->svt = rotateA > 0.0 ? singularType::SV_THR : singularType::SV_FIV;
					singular->pos = halfVerts[edge_to_vertI[j]]->pos;
					singular->vertI = edge_to_vertI[j];
					if (!halfVerts[edge_to_vertI[j]]->isBoundary) {
						int heeI = halfVerts[edge_to_vertI[j]]->edgeI;
						int nextEI = heeI;
						do {
							singular->bFaceI.push_back(halfEdges[nextEI]->faceI);
							nextEI = halfEdges[halfEdges[nextEI]->frontI]->pairI;
						} while (heeI != nextEI);
					}
					singular->bFaceI.push_back(halfEdges[halfVerts[edge_to_vertI[j]]->edgeI]->faceI);
					if (!createSV(singular)) {
						qDebug("error!-GenRegDiv");
					}
					//dirSV.push_back(singular);
					isExistSV = true;
					break;
				}

			}
			if (fabs(fabs(rotateA) - 2.0*Q_PI) < 1e-5 && !isExistSV) {
#pragma region 奇点				
				Vec3 norm1, norm2, norm3;
				Vec3 normA3[3];
				double afx, afy, afz;
				norm1 = halfVerts[edge_to_vertI[0]]->vec;
				norm2 = halfVerts[edge_to_vertI[0]]->vec - halfVerts[edge_to_vertI[1]]->vec;
				norm3 = halfVerts[edge_to_vertI[0]]->vec - halfVerts[edge_to_vertI[2]]->vec;
				if (fabs(norm2.v[1]) > interDim) {
					if (fabs(norm3.v[0] - norm2.v[0] * norm3.v[1] / norm2.v[1]) > interDim) {
						double abc1, abc2;
						abc1 = norm1.v[0] - norm1.v[1] * norm2.v[0] / norm2.v[1];
						abc2 = norm3.v[0] - norm3.v[1] * norm2.v[0] / norm2.v[1];
						afz = abc1 / abc2;
						afy = (norm1.v[1] - afz*norm3.v[1]) / norm2.v[1];
						afx = 1.0 - afz - afy;
					}
				}
				else {
					if (fabs(norm3.v[1]) > interDim) {
						if (fabs(norm2.v[0]) > interDim) {
							afz = norm1.v[1] / norm3.v[1];
							afy = (norm1.v[0] - (norm3.v[0] * norm1.v[1] / norm3.v[1])) / norm2.v[0];
							afx = 1.0 - afz - afy;
						}
					}
				}
				if ((afz > 0 || fabs(afz) < interDim) && (afx > 0 || fabs(afx) < interDim) && (afy > 0 || fabs(afy) < interDim) && fabs(1.0 - afz - afx - afy) < interDim) {
					if (fabs(norm1.v[0] - norm2.v[0] * afy - norm3.v[0] * afz) < interDim && fabs(norm1.v[1] - norm2.v[1] * afy - norm3.v[1] * afz) < interDim) {
						Singular *singular = new Singular();
						singular->svt = rotateA > 0.0 ? singularType::SV_THR : singularType::SV_FIV;
						singular->pos.v[0] = afx*halfVerts[edge_to_vertI[0]]->pos.v[0] + afy*halfVerts[edge_to_vertI[1]]->pos.v[0] + afz*halfVerts[edge_to_vertI[2]]->pos.v[0];
						singular->pos.v[1] = afx*halfVerts[edge_to_vertI[0]]->pos.v[1] + afy*halfVerts[edge_to_vertI[1]]->pos.v[1] + afz*halfVerts[edge_to_vertI[2]]->pos.v[1];
						bool isFaceOrEdge = false;
						if (fabs(afz) < interDim) {
							singular->edgeI = edgeTemp[1];
							singular->bFaceI.push_back(halfEdges[edgeTemp[1]]->faceI);
							singular->bFaceI.push_back(halfEdges[halfEdges[edgeTemp[1]]->pairI]->faceI);
						}
						else if (fabs(afy) < interDim) {
							singular->edgeI = edgeTemp[0];
							singular->bFaceI.push_back(halfEdges[edgeTemp[0]]->faceI);
							singular->bFaceI.push_back(halfEdges[halfEdges[edgeTemp[0]]->pairI]->faceI);
						}
						else if (fabs(afx) < interDim) {
							singular->edgeI = edgeTemp[2];
							singular->bFaceI.push_back(halfEdges[edgeTemp[2]]->faceI);
							singular->bFaceI.push_back(halfEdges[halfEdges[edgeTemp[2]]->pairI]->faceI);
						}
						else if (!isFaceOrEdge) {
							singular->faceI = i;
							singular->bFaceI.push_back(i);
						}
						if (!createSV(singular)) {
							qDebug("error!-GenRegDiv");
						}
						//createSV(sFaceI, sEdgeI, -1, sPos, sBf);
						//dirSV.push_back(singular);
						//dirSV[singularVertCount++] = singular;
					}
				}
				else {
					qDebug("error!");
				}
#pragma endregion		
			}
		}
#pragma region 边界类别
		for (int i = 0; i < vert.size(); i++) {
			if (halfVerts[i]->isBoundary && halfVerts[i]->type == -1) {
				int iterI = i;
				std::vector<int> bVI_midV;
				bVI_midV.clear();
				do {
					bVI_midV.push_back(iterI);
					halfVerts[iterI]->type = BECI.size();
					iterI = halfEdges[halfVerts[iterI]->edgeI]->vertI;
					//test
				/*	if ((!halfVerts[iterI]->isBoundary || halfVerts[iterI]->type >= 0) && iterI != i) {
						qDebug("2770 error!");
						break;
					}*/
					//end
				} while (iterI != i);
				mark_1 mark_obj;
				mark_obj.count = 0;
				mark_obj.vI[0] = i;
				mark_obj.vI[1] = bVI_midV[bVI_midV.size() / 3];
				mark_obj.vI[2] = bVI_midV[2 * bVI_midV.size() / 3];
				BECI[BECI.size()] = mark_obj;
			}
		}
#pragma endregion	
		isGenSingular = true;
#pragma region 奇点附近的流点
		for (int i = 0; i < dirSV.size(); i++) {
			if (dirSV[i]->vertI >= 0) {
				if (halfVerts[dirSV[i]->vertI]->isBoundary) {
					int bhhnevtc = BECI[halfVerts[dirSV[i]->vertI]->type].count;
					BECI[halfVerts[dirSV[i]->vertI]->type].count = bhhnevtc + 1;
					BECI[halfVerts[dirSV[i]->vertI]->type].vI[bhhnevtc % 3] = -1;
				}
			}
			//end:18.5.15
			int streamLinePoint = 0;
			SLPoint EdgePoint[10];
			int endPosNum = 0;
			if (dirSV[i]->faceI >= 0 && dirSV[i]->vertI == -1 && dirSV[i]->edgeI == -1) {
				int dirFaceI = dirSV[i]->faceI;
				int edgeTemp[3] = { 0 };
				edgeTemp[0] = halfFaces[dirFaceI]->edgeI;
				edgeTemp[1] = halfEdges[edgeTemp[0]]->nextI;
				edgeTemp[2] = halfEdges[edgeTemp[1]]->nextI;
				int curPosNum = 0;
				for (int j = 0; j < 3; j++) {
					findEdgePointPlus(edgeTemp[j], dirSV[i]->pos, EdgePoint, endPosNum);
					if (endPosNum - curPosNum > 2 && halfEdges[edgeTemp[j]]->pairI != -1) {
						endPosNum = curPosNum;
						int hep = halfEdges[edgeTemp[j]]->pairI;
						findEdgePointPlus(halfEdges[hep]->nextI, dirSV[i]->pos, EdgePoint, endPosNum);
						findEdgePointPlus(halfEdges[halfEdges[hep]->nextI]->nextI, dirSV[i]->pos, EdgePoint, endPosNum);
					}
					curPosNum = endPosNum;
				}
				if (endPosNum > dirSV[i]->svt != singularType::SV_UND) {
					if (dirSV[i]->svt == singularType::SV_THR || dirSV[i]->svt == singularType::SV_FIV) {
						double judgeAEP = cos(Q_PI / int(dirSV[i]->svt));
						bool judgeSV[10] = { false };
						for (int j = 0; j < endPosNum; j++) {
							Vec3 epep = (EdgePoint[j].pos - dirSV[i]->pos).normalized();
							for (int k = 0; k < endPosNum; k++) {
								if (k != j) {
									if (epep * (EdgePoint[k].pos - dirSV[i]->pos).normalized() > judgeAEP) {
										judgeSV[j] = true;
										break;
									}
								}
							}
						}
						SLPoint EdgePointT[10];
						for (int j = 0; j < endPosNum; j++) {
							EdgePointT[j] = EdgePoint[j];
						}
						bool isExitEP = false;
						int iterEP = endPosNum;
						endPosNum = 0;
						for (int j = 0; j < iterEP; j++) {
							if (judgeSV[j]) {
								if (!isExitEP) {
									isExitEP = true;
									EdgePoint[endPosNum++] = EdgePointT[j];
								}
							}
							else {
								EdgePoint[endPosNum++] = EdgePointT[j];
							}
						}
					}
				}
			}
			else if (dirSV[i]->edgeI >= 0 && dirSV[i]->faceI == -1 && dirSV[i]->vertI == -1) {
				//由所在面包含	
				int edgeTemp[4] = { 0 };
				edgeTemp[0] = halfEdges[dirSV[i]->edgeI]->nextI;
				edgeTemp[1] = halfEdges[edgeTemp[0]]->nextI;
				edgeTemp[2] = halfEdges[halfEdges[dirSV[i]->edgeI]->pairI]->nextI;
				edgeTemp[3] = halfEdges[edgeTemp[2]]->nextI;
				int curPosNum = 0;
				for (int j = 0; j < 4; j++) {
					findEdgePointPlus(edgeTemp[j], dirSV[i]->pos, EdgePoint, endPosNum);
					if (endPosNum - curPosNum > 2 && halfEdges[edgeTemp[j]]->pairI != -1) {
						endPosNum = curPosNum;
						int hep = halfEdges[edgeTemp[j]]->pairI;
						findEdgePointPlus(halfEdges[hep]->nextI, dirSV[i]->pos, EdgePoint, endPosNum);
						findEdgePointPlus(halfEdges[halfEdges[hep]->nextI]->nextI, dirSV[i]->pos, EdgePoint, endPosNum);
					}
					curPosNum = endPosNum;
				}
			}
			else if (dirSV[i]->vertI >= 0 && dirSV[i]->faceI != -4 && dirSV[i]->edgeI == -1) {
				//找到初始半边以及最后半边
				int startHalfEdgeIto = halfVerts[dirSV[i]->vertI]->edgeI;
				int endHalfEdgeIto = halfVerts[getBFrontPTodNextPI(dirSV[i]->vertI)]->edgeI;
				Vec3 dirn1 = (halfVerts[halfEdges[startHalfEdgeIto]->vertI]->pos - dirSV[i]->pos).normalized();
				Vec3 dirn2 = (halfVerts[halfEdges[halfEdges[endHalfEdgeIto]->frontI]->vertI]->pos - dirSV[i]->pos).normalized();
				double dirnf1 = dirn1.angleP();
				double dirnf2 = dirn2.angleP();
				double ratoteA = dirnf1 > dirnf2 ? dirnf2 - dirnf1 + Q_PI*2.0 : dirnf2 - dirnf1;
				Vec3 vn = halfVerts[dirSV[i]->vertI]->vec;
				double crossFn = vn.angleP() / 4.0;
				Vec3 crossF[4];
				for (int ii = 0; ii < 4; ii++) {
					Vec3 cfTemp = angleToNorm(crossFn);
					//夹角大于六十度
					if (acos(cfTemp*dirn1) > Q_PI / 4.0 && acos(cfTemp*dirn2) > Q_PI / 4.0) {
						if (ratoteA > Q_PI) {
							if (!calParToP(dirn1, dirn2, cfTemp)) {
								crossF[ii] = cfTemp;
							}
							else {
								crossF[ii] = Vec3();
							}
						}
						else {
							if (calParToP(dirn1, dirn2, cfTemp)) {
								crossF[ii] = cfTemp;
							}
							else {
								crossF[ii] = Vec3();
							}
						}
					}
					else {
						crossF[ii] = Vec3();
					}
					crossFn = crossFn + Q_PI / 2.0;
					crossFn = crossFn > Q_PI*2.0 ? crossFn - Q_PI*2.0 : crossFn;
				}
				int tempEdgeI = halfEdges[startHalfEdgeIto]->nextI;
				Vec3 firstPoint = halfVerts[halfEdges[startHalfEdgeIto]->vertI]->pos;
				Vec3 secandPoint = firstPoint;
				Vec3 originPoint = dirSV[i]->pos;
				do {
					firstPoint = secandPoint;
					secandPoint = halfVerts[halfEdges[tempEdgeI]->vertI]->pos;
					for (int ii = 0; ii < 4; ii++) {
						if (crossF[ii].norm() > 1.0e-5) {
							if (calParToP(firstPoint - originPoint, secandPoint - originPoint, crossF[ii])) {
								EdgePoint[endPosNum].edgeI = tempEdgeI;
								calPoint(firstPoint, secandPoint, originPoint, crossF[ii], EdgePoint[endPosNum].pos);
								//EdgePoint[endPosNum].pos = originPoint;
								EdgePoint[endPosNum++].vec = crossF[ii];
								crossF[ii] = Vec3();
							}
						}
					}
					if (halfEdges[tempEdgeI]->nextI != endHalfEdgeIto && halfEdges[halfEdges[tempEdgeI]->nextI]->pairI != -1) {
						tempEdgeI = halfEdges[halfEdges[halfEdges[tempEdgeI]->nextI]->pairI]->nextI;
					}
					else {
						break;
					}
				} while (true);
			}
			else if (dirSV[i]->vertI >= 0 && dirSV[i]->faceI == -1 && dirSV[i]->edgeI == -1) {
				//18.5.16
				int startSV_to_E = halfVerts[dirSV[i]->vertI]->edgeI;
				int curSV_to_E = startSV_to_E;
				do {
					findEdgePointPlus(halfEdges[curSV_to_E]->nextI, dirSV[i]->pos, EdgePoint, endPosNum);
					curSV_to_E = halfEdges[halfEdges[curSV_to_E]->frontI]->pairI;
				} while (curSV_to_E != startSV_to_E);
			}
			else {
				if (dirSV[i]->faceI != -4) {
					qDebug("being given");
				}
			}
			for (int j = 0; j < endPosNum && endPosNum < 6; j++) {
				dirSV[i]->slc = endPosNum;
				dirSV[i]->dir[j] = EdgePoint[j];
			}
		}
#pragma endregion

	}
	else {
		isGenSingular = false;
	}
}
QString Mesh::getSVCount() {
	int fiveValueSV = 0;
	int threeValueSV = 0;
	int fourValueSV = 0;
	int borderValueSV = 0;
	int slBValueSV = 0;
	int cornerValueSV = 0;
	for (int i = 0; i < dirSV.size(); i++) {
		switch (dirSV[i]->svt)
		{
		case singularType::SV_COR:
			cornerValueSV++;
			break;
		case singularType::SV_BOR:
			borderValueSV++;
			break;
		case singularType::SV_THR:
			threeValueSV++;
			break;
		case singularType::SV_FOU:
			fourValueSV++;
			break;
		case singularType::SV_FIV:
			fiveValueSV++;
			break;
		case singularType::SV_THR_BOR:
			slBValueSV++;
			break;
		default:
			break;
		}
	}
	return "three:" + QString::number(threeValueSV) + " | five:" + QString::number(fiveValueSV) + " | border:" + QString::number(borderValueSV) + "\n" + "Corner:" + QString::number(cornerValueSV) + " | four:" + QString::number(fourValueSV) + " | SLB:" + QString::number(slBValueSV);
}
//区域分解
bool Mesh::GenRegDiv() {
	if (halfEdges != nullptr) {
		double qSize = 1e5;
		bVEdge.clear();
		bVSEdge.clear();
		dirSV.clear();
		/*bEdges.reserve(bVEdge.size());
		bEdges.clear();*/
#pragma region 边界点
		for (int i = 0; i < face.size(); i++) {
			int edgeTemp[3] = { halfFaces[i]->edgeI ,halfEdges[halfFaces[i]->edgeI]->nextI,halfEdges[halfFaces[i]->edgeI]->frontI };
			int heI[3][2] = { { -1 ,-1 },{ -1,-1 },{ -1,-1 } };
			for (int j = 0; j < 3; j++) {
				if (halfEdges[edgeTemp[j]]->pairI == -1) {

					heI[j][0] = edgeTemp[j];
					heI[j][1] = edgeTemp[(j + 2) % 3];
					halfVerts[halfEdges[edgeTemp[j]]->vertI]->isBoundary = true;
					halfVerts[halfEdges[edgeTemp[(j + 2) % 3]]->vertI]->isBoundary = true;
				}
			}
			for (int j = 0; j < 3; j++) {
				if (heI[j][0] != -1 && heI[j][1] != -1) {
					int fv = heI[j][0];
					int sev = heI[j][1];
					dirBVEdgeIndex.push_back(LineIndex(halfEdges[fv]->vertI, halfEdges[sev]->vertI));
					LineVs ls;
					int indexI[2] = { halfEdges[fv]->vertI ,halfEdges[sev]->vertI };
					ls.v[0] = halfVerts[indexI[0]]->pos;
					ls.v[1] = halfVerts[indexI[1]]->pos;
					bVEdge.push_back(ls);
					bVSEdge.push_back(BELine(ls, (halfVerts[indexI[0]]->vec.angleP()), (halfVerts[indexI[1]]->vec.angleP())));
					//getBorderSingular(fv);
					double edgeLenS = (halfVerts[halfEdges[fv]->vertI]->pos - halfVerts[halfEdges[sev]->vertI]->pos).norm();
					qSize = qSize > edgeLenS ? edgeLenS : qSize;
				}
			}
		}
#pragma endregion		
		setDisQuaSize(qSize*2.0);
		setDifQuaSize(qSize*2.0);
		return true;
	}
	else {
		return false;
	}
}
void Mesh::setDisQuaSize(double queSize) {
	this->disQuaSize = queSize;
}
double Mesh::getDisQuaSize() {
	return this->disQuaSize;
}
void Mesh::saveSawtoothQua(QString st) {
	if (dfm != nullptr && dfm->qface.size() > 0) {

		for (int i = 0; i < vert.size(); i++) {
			if (!halfVerts[i]->isBoundary) {
				dfm->GetInfoFace(halfVerts[i]->pos);
			}
		}

		std::vector<int> vtov;
		std::map<int, int> vtos;
		int vc, fc, qc;
		vc = fc = qc = 0;
		for (auto iter = dfm->qface.begin(); iter != dfm->qface.end(); ++iter) {
			if (dfm->qft[qc] /*&& !dfm->bqft[qc]*/) {
				for (int i = 0; i < 4; ++i) {
					if (vtos.find(iter->cornerPI[i]) == vtos.end()) {
						vtov.push_back(iter->cornerPI[i]);
						vtos[iter->cornerPI[i]] = vc++;
					}
				}
				fc++;
			}
			qc++;
		}
		QFile fp(st);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeSSLVTK) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return;
		}
		QTextStream stream(&fp);
		stream << "# vtk DataFile Version 2.0" << endl;
		stream << "model boundary" << endl;
		stream << "ASCII" << endl;
		stream << "DATASET UNSTRUCTURED_GRID" << endl;
		stream << "POINTS " << vtos.size() << " double" << endl;
		for (int i = 0; i < vtov.size(); ++i) {
			stream << dfm->qvert[vtov[i]].pos[0] << " " << dfm->qvert[vtov[i]].pos[1] << " 0.0" << endl;
		}
		stream << "CELLS " << fc << " " << fc * 5 << endl;
		qc = 0;
		for (auto iter = dfm->qface.begin(); iter != dfm->qface.end(); ++iter) {
			if (dfm->qft[qc] /*&& !dfm->bqft[qc]*/) {
				stream << "4 " << vtos[iter->cornerPI[0]] << " " << vtos[iter->cornerPI[1]] << " " << vtos[iter->cornerPI[2]] << " " << vtos[iter->cornerPI[3]] << endl;
			}
			qc++;
		}
		stream << "CELL_TYPES " << fc << endl;
		for (int i = 0; i < fc; ++i) {
			stream << "7" << endl;
		}
		fp.close();
	}
}
void Mesh::setDifQuaSize(double queSize) {
	this->difQuaSize = queSize;
}
double Mesh::getDifQuaSize() {
	return this->difQuaSize;
}
bool Mesh::dimInterP(Vec3 tempP) {
	bool isExist = false;
	for (auto iter = fNode.begin(); iter != fNode.end(); ++iter) {
		if ((iter->pos - tempP).norm() < 1.0e-7) {
			isExist = true;
			break;
		}
	}
	return isExist;
}
//findQuaSFValue
void Mesh::findQuaSFValue() {
	fNode.clear();
#if 1
	for (int i = 0; i < sf.size(); i++) {
		if (sf[i].slIs.size() >= 2) {
			if (sf[i].slIs.find(0) != sf[i].slIs.end()) {
				i = i;
			}
			int j = 0;
			std::map<int, Vec3> newSlI;
			newSlI.insert(sf[i].slIs.begin(), sf[i].slIs.end());
			for (auto iter = sf[i].slIs.begin(); iter != sf[i].slIs.end(); iter++, j++) {
				int fiter = abs(iter->first) - 1;
				/*if (iter->first < 0) {
					fiter = fiter;
				}*/
				int jj = 0;
				for (auto iter1 = newSlI.begin(); iter1 != newSlI.end() && jj < j; ++iter1, ++jj) {
					int fiter1 = abs(iter1->first) - 1;
					double isnisn = iter1->second.normalized()*iter->second.normalized();
					if (fabs(isnisn) < 0.5) {
						int sfI = sf[i].faceI;
						Vec3 auxInterP = (halfVerts[face[sfI].index[0]]->pos + halfVerts[face[sfI].index[1]]->pos + halfVerts[face[sfI].index[2]]->pos) / 3.0;
						Vec3 interP;
						int lVS[2];
						if (findSLInterPToAuxP(fiter1, fiter, iter1->second, iter->second, auxInterP, interP, lVS) && !dimInterP(interP)) {
							fNode.push_back(fourNode(interP, fiter1, fiter, lVS, sf[i].faceI));
							//流线上的节点排序							
							int sffIs[2] = { fiter1,fiter };
							for (int k = 0; k < 2; k++) {
								if (SLSimplify[sffIs[k]]->interV.size() == 0) {
									SLSimplify[sffIs[k]]->interV.push_back(fNode.size() - 1);
								}
								else {
									std::vector<int> orderInterV;
									orderInterV.clear();
									bool isf = false;
									bool isff = false;
									double minInterPDis = 1.0e7;
									int interVI = -1;
									int kk = 0;
									//todo:处理简化后的流线出现多处自交情况
									std::map<int, bool> fnodeInfo;
									for (auto iter1 = SLSimplify[sffIs[k]]->interV.begin(); iter1 != SLSimplify[sffIs[k]]->interV.end(); ++iter1) {
										fnodeInfo[*iter1] = false;
									}
									for (auto iter1 = SLSimplify[sffIs[k]]->interV.begin(); iter1 != SLSimplify[sffIs[k]]->interV.end(); ++iter1, ++kk) {
										if (!isf) {
											if (fNode[*iter1].streamLine1 == sffIs[k] && fNode[*iter1].streamLine1 == fNode[*iter1].streamLine2) {
												if (!fnodeInfo[*iter1]) {
													bool uequSL = fNode[*iter1].psl[0] < fNode[*iter1].psl[1];
													if ((uequSL && fNode[*iter1].psl[0] > lVS[k]) ||
														(!uequSL && fNode[*iter1].psl[1] > lVS[k])) {
														interVI = kk;
														isf = true;
														break;
													}
												}
												else {
													bool uequSL = fNode[*iter1].psl[0] > fNode[*iter1].psl[1];
													if ((uequSL && fNode[*iter1].psl[0] > lVS[k]) ||
														(!uequSL && fNode[*iter1].psl[1] > lVS[k])) {
														interVI = kk;
														isf = true;
														break;
													}
												}
												fnodeInfo[*iter1] = true;
											}
											else if (((fNode[*iter1].streamLine1 == sffIs[k] && fNode[*iter1].psl[0] > lVS[k]) ||
												(fNode[*iter1].streamLine2 == sffIs[k] && fNode[*iter1].psl[1] > lVS[k])) && !isff) {
												interVI = kk;
												isf = true;
												break;
											}
											else if ((fNode[*iter1].streamLine1 == sffIs[k] && fNode[*iter1].psl[0] == lVS[k]) ||
												(fNode[*iter1].streamLine2 == sffIs[k] && fNode[*iter1].psl[1] == lVS[k])) {
												Vec3 temp = (fNode[*iter1].pos - interP).normalized()*(fNode[*iter1].pos - showSSLS[lVS[k]].pos).normalized();
												if ((fNode[*iter1].pos - interP).normalized()*(fNode[*iter1].pos - showSSLS[lVS[k]].pos).normalized() < -0.5) {
													isff = true;
													continue;
												}
												else {
													isff = true;
													interVI = kk;
													break;
												}
											}
											else if (isff) {
												interVI = kk;
												break;
											}
										}
									}
									kk = 0;
									for (auto iter1 = SLSimplify[sffIs[k]]->interV.begin(); iter1 != SLSimplify[sffIs[k]]->interV.end(); ++iter1, ++kk) {
										if (interVI == kk) {
											orderInterV.push_back(fNode.size() - 1);
										}
										orderInterV.push_back(*iter1);
									}
									if ((!isf && !isff) || (isff && interVI == -1)) {
										orderInterV.push_back(fNode.size() - 1);
									}
									SLSimplify[sffIs[k]]->interV.clear();
									SLSimplify[sffIs[k]]->interV.insert(SLSimplify[sffIs[k]]->interV.begin(), orderInterV.begin(), orderInterV.end());
								}
							}
						}
					}
				}
			}
		}
	}
#endif
	QuaDimSL();
}
//参数一和二为流线索引，参数三为辅助点，参数四为交点，参数五对应参数一二流线交点的前一个流点的索引
bool Mesh::findSLInterPToAuxP(int slI1, int slI2, Vec3 v1, Vec3 v2, Vec3 auxP, Vec3& interP, int lVS[2]) {
	if (SLSimplify.size() > slI1 && SLSimplify.size() > slI2 && SLSimplify[slI1]->slpc > 1 && SLSimplify[slI2]->slpc > 1) {
		int minls1DisI1t = SLSimplify[slI1]->startSLPointIndex + 1;
		int minls2DisI1t = SLSimplify[slI2]->startSLPointIndex + 1;
		double minls1Dis = (showSSLS[minls1DisI1t].pos - auxP).norm();
		double minls2Dis = (showSSLS[minls2DisI1t].pos - auxP).norm();
		for (int i = SLSimplify[slI1]->startSLPointIndex + 1; i < SLSimplify[slI1]->endSLPointIndex; i++) {
			if (fabs((showSSLS[i].pos - showSSLS[i + 1].pos).normalized()*v1.normalized()) > 0.86 && (showSSLS[i].pos - auxP).norm() < minls1Dis) {
				minls1DisI1t = i;
				minls1Dis = (showSSLS[i].pos - auxP).norm();
			}
		}
		for (int j = SLSimplify[slI2]->startSLPointIndex + 1; j < SLSimplify[slI2]->endSLPointIndex; j++) {
			if (fabs((showSSLS[j].pos - showSSLS[j + 1].pos).normalized()*v2.normalized()) > 0.86 && (showSSLS[j].pos - auxP).norm() < minls2Dis) {
				minls2DisI1t = j;
				minls2Dis = (showSSLS[j].pos - auxP).norm();
			}
		}
		int dNum = 2;
		std::vector<int> minls1DisII;
		std::vector<int> minls2DisI2;
		while (dNum < dimNum) {
			minls1DisII.clear();
			minls2DisI2.clear();
			for (int k = -dNum; k <= dNum; k++) {
				int mk = minls1DisI1t + k;
				if (mk > SLSimplify[slI1]->endSLPointIndex) {
					minls1DisII.push_back(SLSimplify[slI1]->endSLPointIndex);
				}
				else if (mk < SLSimplify[slI1]->startSLPointIndex) {
					minls1DisII.push_back(SLSimplify[slI1]->startSLPointIndex);
				}
				else {
					minls1DisII.push_back(mk);
				}
				mk = minls2DisI1t + k;
				if (mk > SLSimplify[slI2]->endSLPointIndex) {
					minls2DisI2.push_back(SLSimplify[slI2]->endSLPointIndex);
				}
				else if (mk < SLSimplify[slI2]->startSLPointIndex) {
					minls2DisI2.push_back(SLSimplify[slI2]->startSLPointIndex);
				}
				else {
					minls2DisI2.push_back(mk);
				}
			}
			//找到线段	
			for (int i = 0; i < minls1DisII.size() - 1; i++) {
				for (int j = 0; j < minls2DisI2.size() - 1; j++) {
					if (minls1DisII[i] >= SLSimplify[slI1]->startSLPointIndex && minls1DisII[i + 1] <= SLSimplify[slI1]->endSLPointIndex
						&& minls2DisI2[j] >= SLSimplify[slI2]->startSLPointIndex && minls2DisI2[j + 1] <= SLSimplify[slI2]->endSLPointIndex) {
						if (findInterP(showSSLS[minls1DisII[i]].pos, showSSLS[minls1DisII[i + 1]].pos, showSSLS[minls2DisI2[j]].pos, showSSLS[minls2DisI2[j + 1]].pos, interP)) {
							//interP = interP;
							lVS[0] = minls1DisII[i] > minls1DisII[i + 1] ? minls1DisII[i + 1] : minls1DisII[i];
							lVS[1] = minls2DisI2[j] > minls2DisI2[j + 1] ? minls2DisI2[j + 1] : minls2DisI2[j];
							return true;
						}
					}

				}
			}
			++dNum;
		}
	}
	return false;
}
//参数一二为流线的始末两奇点，参数三四为流点始末两点
void Mesh::GenQuaSL(int ssI, int esI, int spI, int epI, bool isB, bool dr) {
	StreamLine* sl = new StreamLine(ssI, esI, spI, epI, isB, 0.0, dr, true);
	QuaSLInfo.push_back(sl);
}
void Mesh::QuaDimSL() {
	quaSLs.clear();
	QuaSLInfo.clear();
	int k = 0;			//全局流线数量索引
	for (auto iter = SLSimplify.begin(); iter != SLSimplify.end(); ++iter, ++k) {
		if (k == 21) {
			k = 21;
		}
		if ((*iter)->interV.size() > 0) {
			//分割成interV.size()+1流线段
			int slssI = (*iter)->startSingularIndex;
			int slseI = (*iter)->endSingularIndex;
			int slspI = (*iter)->startSLPointIndex;
			//showQuaSL.push_back(showSSL[(*iter)->startSLPointIndex]);
			//修改奇点记录的流线信息
			//bool asdf = false;
			for (int i = 0; i < 5; i++) {
				if (dirSV[slssI]->streamLines[i] == k) {
					dirSV[slssI]->streamLines[i] = QuaSLInfo.size();
					//asdf = true;
					break;
				}
			}
			//todo:处理简化后的流线出现多处自交情况
			std::map<int, bool> uequSLInfo;
			for (int i = 0; i < (*iter)->interV.size(); i++) {
				uequSLInfo[(*iter)->interV[i]] = false;
			}
			for (int i = 0; i < (*iter)->interV.size(); i++) {
				int fiipI;
				if (fNode[(*iter)->interV[i]].streamLine1 == fNode[(*iter)->interV[i]].streamLine2) {
					if (!uequSLInfo[(*iter)->interV[i]]) {
						fiipI = fNode[(*iter)->interV[i]].psl[0] > fNode[(*iter)->interV[i]].psl[1] ? fNode[(*iter)->interV[i]].psl[1] : fNode[(*iter)->interV[i]].psl[0];
						uequSLInfo[(*iter)->interV[i]] = true;
					}
					else {
						fiipI = fNode[(*iter)->interV[i]].psl[0] < fNode[(*iter)->interV[i]].psl[1] ? fNode[(*iter)->interV[i]].psl[1] : fNode[(*iter)->interV[i]].psl[0];
					}
				}
				else {
					fiipI = fNode[(*iter)->interV[i]].streamLine1 == k ? fNode[(*iter)->interV[i]].psl[0] : fNode[(*iter)->interV[i]].psl[1];
				}
				int fiisI = fNode[(*iter)->interV[i]].singularI;
				if (fiisI != -1) {
					//已经建立奇点
					for (int i = 1; i < 4; i++) {
						if (dirSV[fiisI]->streamLines[i] == -1) {
							dirSV[fiisI]->streamLines[i] = QuaSLInfo.size();
							dirSV[fiisI]->streamLines[i + 1] = QuaSLInfo.size() + 1;
							slseI = fiisI;
							break;
						}
					}
				}
				else {
					//建立新奇点
					Singular* sv = new Singular();
					sv->pos = fNode[(*iter)->interV[i]].pos;
					sv->streamLines[0] = QuaSLInfo.size();
					sv->streamLines[1] = QuaSLInfo.size() + 1;//流线进出，形成两条流线
					sv->faceI = fNode[(*iter)->interV[i]].faceI;
					sv->svt = singularType::SV_FOU;
					if (!createSV(sv)) {
						qDebug("error!-QuaDimSL");
					}
					fNode[(*iter)->interV[i]].singularI = dirSV.size() - 1;
					slseI = dirSV.size() - 1;
				}
				int curQSLC = i == 0 ? quaSLs.size() : quaSLs.size() - 1;
				/*if (fiipI - slspI < 0) {
				break;
				}*/
				//GenQuaSL(slssI, slseI, curQSLC, curQSLC + fiipI - slspI + 1);
				for (int i = slspI; i <= fiipI; i++) {
					quaSLs.push_back(showSSLS[i].pos);
				}
				quaSLs.push_back(fNode[(*iter)->interV[i]].pos);
				GenQuaSL(slssI, slseI, curQSLC, quaSLs.size() - 1, false, false);
				quaSLs.push_back(fNode[(*iter)->interV[i]].pos);
				slspI = fiipI + 1;
				slssI = slseI;
			}
			GenQuaSL(slssI, (*iter)->endSingularIndex, quaSLs.size() - 1, quaSLs.size() - 1 + (*iter)->endSLPointIndex - slspI + 1, false, false);
			for (int i = slspI; i <= (*iter)->endSLPointIndex; i++) {
				quaSLs.push_back(showSSLS[i].pos);
			}
			for (int i = 0; i < 5; i++) {
				if (dirSV[(*iter)->endSingularIndex]->streamLines[i] == k) {
					dirSV[(*iter)->endSingularIndex]->streamLines[i] = QuaSLInfo.size() - 1;
					break;
				}
			}
		}
		else {
			GenQuaSL((*iter)->startSingularIndex, (*iter)->endSingularIndex, quaSLs.size(), quaSLs.size() - 1 + (*iter)->slpc, false, false);
			for (int i = (*iter)->startSLPointIndex; i <= (*iter)->endSLPointIndex; i++) {
				quaSLs.push_back(showSSLS[i].pos);
			}
			bool dissk = false;
			bool diesk = false;
			for (int j = 0; j < 5; j++) {
				if (dirSV[(*iter)->startSingularIndex]->streamLines[j] == k && !dissk) {
					dirSV[(*iter)->startSingularIndex]->streamLines[j] = QuaSLInfo.size() - 1;
					dissk = true;
				}
				if (dirSV[(*iter)->endSingularIndex]->streamLines[j] == k && !diesk) {
					dirSV[(*iter)->endSingularIndex]->streamLines[j] = QuaSLInfo.size() - 1;
					diesk = true;
				}
			}
		}
	}
	//建立边界划分流线
	for (int i = 0; i < dirSV.size(); i++) {
		if (dirSV[i]->faceI == -3 || dirSV[i]->faceI == -4 || dirSV[i]->faceI == -2) {
			//该奇点为边界分区节点
			//通过寻找流线式方法，来寻找划分流线				
			if (dirSV[i]->vertI >= 0) {
				findBSL(i, dirSV[i]->vertI, true);			//逆时针
				findBSL(i, dirSV[i]->vertI, false);			//顺时针				
			}
			else if (dirSV[i]->edgeI >= 0) {
				findBSL(i, halfEdges[dirSV[i]->edgeI]->vertI, true);
				findBSL(i, halfEdges[halfEdges[dirSV[i]->edgeI]->frontI]->vertI, false);
			}
			else {
				//出错
				qDebug("program error!");
			}
		}
	}
	//end
	//绘制分解问题域	
	for (int i = 0; i < QuaSLInfo.size(); i++) {
		int j = QuaSLInfo[i]->startSLPointIndex;
		for (; j < QuaSLInfo[i]->endSLPointIndex; j++) {
			quaSLs[j].color = j % 2 == 0 ? quaSLColor1 : quaSLColor2;
		}
		quaSLs[j].color = j % 2 == 0 ? quaSLColor1 : quaSLColor2;
	}
	//isGenSingular = false;
	isGenQua = true;
	//暂时放着
	preGenQuaStatec();
	//isFourMesh = arrSLOrder() >= 0;
}
void Mesh::auxFindBSL(int svI1, int svI2, bool dir, int slspI, int slepI) {
	quaSLs.push_back(dirSV[svI2]->pos);
	GenQuaSL(svI1, svI2, slspI, slepI, true, dir);
	if (dir) {
		dirSV[svI2]->streamLines[4] = QuaSLInfo.size() - 1;
		dirSV[svI1]->streamLines[3] = QuaSLInfo.size() - 1;
	}
	else {
		dirSV[svI2]->streamLines[3] = QuaSLInfo.size() - 1;
		dirSV[svI1]->streamLines[4] = QuaSLInfo.size() - 1;
	}
}
//vertI：起点位置
//dir：流线方向
void Mesh::findBSL(int svI, int vertI, bool dir) {
	if (((dirSV[svI]->faceI == -3 || dirSV[svI]->faceI == -4 || dirSV[svI]->faceI == -2) && ((dir && dirSV[svI]->streamLines[3] == -1) || (!dir && dirSV[svI]->streamLines[4] == -1)))) {
		int slCount = quaSLs.size();
		quaSLs.push_back(dirSV[svI]->pos);
		//处理细节处						
		if (dirSV[svI]->edgeI >= 0) {
			double minDisVToP = 1.0e7;
			int targetSVI = -1;
			Vec3 dSVSP = dirSV[svI]->pos;
			Vec3 hfvP = halfVerts[vertI]->pos;
			for (int i = 0; i < dirSV.size(); i++) {
				if (i != svI && dirSV[i]->faceI < -1 && (dirSV[i]->edgeI == dirSV[svI]->edgeI || dirSV[i]->vertI == vertI)) {
					Vec3 dSVIP = dirSV[i]->pos;
					if (minDisVToP > (dSVSP - dSVIP).norm() && (dSVIP - dSVSP).normalized()*(dSVIP - hfvP).normalized() < -0.5)
					{
						targetSVI = i;
						minDisVToP = (dSVSP - dSVIP).norm();
					}
				}
			}
			if (targetSVI >= 0) {
				auxFindBSL(svI, targetSVI, dir, slCount, slCount + 1);
				return;
			}
		}
		//处理起始情况		
		//end
		if (dirSV[svI]->vertI != vertI) {
			quaSLs.push_back(halfVerts[vertI]->pos);
		}
		int curPI = vertI;
		do {
			//线监测边内情况，在考虑下一个点		
			for (int i = 0; i < dirSV.size(); i++) {
				if ((i == svI && quaSLs.size() - slCount > 3) || i != svI) {
					if (dirSV[i]->faceI == -3 && dirSV[i]->edgeI >= 0) {
						if (i == 93) {
							i = 93;
						}
						if ((dir && dirSV[i]->edgeI == halfVerts[curPI]->edgeI) || (!dir && dirSV[i]->edgeI == halfVerts[getBFrontPTodNextPI(curPI)]->edgeI)) {
							Vec3 dSVSP = halfVerts[curPI]->pos;
							double minDisVToP = 1.0e7;
							int targetSVI = -1;
							for (int j = 0; j < dirSV.size(); j++) {
								if (dirSV[j]->edgeI == dirSV[i]->edgeI) {
									if (minDisVToP > (dirSV[j]->pos - dSVSP).norm()) {
										targetSVI = j;
										minDisVToP = (dirSV[j]->pos - dSVSP).norm();
									}
								}
							}
							if (targetSVI != -1) {
								if (quaSLs.size() - slCount > 1 && ((quaSLs[quaSLs.size() - 1].pos - dirSV[targetSVI]->pos).norm() < 1.0e-6 ||
									(quaSLs[quaSLs.size() - 2].pos - dirSV[targetSVI]->pos).normalized()*(quaSLs[quaSLs.size() - 1].pos - dirSV[targetSVI]->pos).normalized() < 0.0))
									quaSLs.pop_back();
								auxFindBSL(svI, targetSVI, dir, slCount, slCount == quaSLs.size() ? quaSLs.size() + 1 : quaSLs.size());
								return;
							}
						}
					}
				}
			}
			curPI = dir ? halfEdges[halfVerts[curPI]->edgeI]->vertI : getBFrontPTodNextPI(curPI);
			for (int i = 0; i < dirSV.size(); i++) {
				if ((i == svI && quaSLs.size() - slCount > 3) || i != svI) {
					if (dirSV[i]->vertI >= 0 && dirSV[i]->vertI == curPI) {
						if ((quaSLs[quaSLs.size() - 1].pos - dirSV[i]->pos).norm() < 1.0e-6 ||
							((int)quaSLs.size() - slCount > 1 && (quaSLs[quaSLs.size() - 2].pos - dirSV[i]->pos).normalized()*(quaSLs[quaSLs.size() - 1].pos - dirSV[i]->pos).normalized() < 0.0))
							quaSLs.pop_back();
						auxFindBSL(svI, i, dir, slCount, quaSLs.size());
						if ((quaSLs[quaSLs.size() - 1].pos - quaSLs[quaSLs.size() - 2].pos).norm() < 1e-4) {
							qDebug("error!(2967)");
						}
						return;
					}
				}
			}
			quaSLs.push_back(halfVerts[curPI]->pos);
			if (curPI == vertI) {
				break;
			}
		} while (curPI != vertI);
		GenQuaSL(svI, svI, slCount, quaSLs.size() - 1, true, dir);
	}
}
void Mesh::findSLToSV(int sv1, int sv2, int slIArray[]) {
	slIArray[0] = -1;
	slIArray[1] = -1;
	if (sv1 >= 0 && sv1 < dirSV.size() && sv2 >= 0 && sv2 < dirSV.size()) {
		int i = 0, k = 0;
		for (auto iter = QuaSLInfo.begin(); iter != QuaSLInfo.end(); ++iter, ++k) {
			if (((*iter)->startSingularIndex == sv1 && (*iter)->endSingularIndex == sv2) ||
				((*iter)->startSingularIndex == sv2) && (*iter)->endSingularIndex == sv1) {
				slIArray[i++] = k;
			}
		}
	}
}
void Mesh::findErrorId(int slI) {
	if (slI >= 0 && slI < QuaSLInfo.size()) {
		for (int i = QuaSLInfo[slI]->startSLPointIndex; i <= QuaSLInfo[slI]->endSLPointIndex; i++) {
			quaSLs[i].color = quaESLColor;
		}
	}
}
bool Mesh::JudgeTAIA(double gp, bool is) {
	if (isFourMesh) {
		if (is) {
			for (int i = 0; i < QuaSLInfo.size(); i++) {
				//更新流线长度
				double distanceSl = 0.0;
				for (int j = QuaSLInfo[i]->auxStartSLPI; j < QuaSLInfo[i]->auxEndSLPI; j++) {
					distanceSl += (quaCareSLs[j + 1].pos - quaCareSLs[j].pos).norm();
				}
				QuaSLInfo[i]->refreshDisSLInfo(distanceSl);
			}
		}
		std::vector<int> typeSLSplc;
		for (auto iter = slType.begin(); iter != slType.end(); ++iter) {
			int qiissC = 0;
			if (iter->second.sI > 0) {
				qiissC = iter->second.sI;
			}
			else
			{
				qiissC = int(iter->second.dis / gp);
				qiissC = (qiissC < 2 ? 2 : qiissC) + 1;
			}
			typeSLSplc.push_back(qiissC);
		}
		int quaNum = 0;
		for (int i = 0; i < quadraInfo.size(); i++) {
			quaNum += (typeSLSplc[QuaSLInfo[quadraInfo[i].slI[0]]->typeSL] - 1) * (typeSLSplc[QuaSLInfo[quadraInfo[i].slI[1]]->typeSL] - 1);
		}
		if (quaNum < 5e6) {
			quaGridNum = quaNum;
			careQuaIndex.reserve(quaNum);
			careQuaMeshP.reserve(quaNum);
			careQuaInfo.reserve(quaNum);
			return true;
		}
		else {
			return false;
		}
	}
	return false;
}
void Mesh::genQuaMesh() {
	//粗四边形是正常的
	isGenQuaMesh = false;
	if (isTAIA) {
		std::vector<PCPoint>().swap(quaCareSLs);
		quaCareSLs.reserve(quaSLs.size());
		for (auto iter1 = slType.begin(); iter1 != slType.end(); ++iter1) {
			updateLikeSLInfo(iter1->first);
			int qiissC = 1;
			if (iter1->second.sI > 0) {
				qiissC = iter1->second.sI;
			}
			else {
				qiissC = int(iter1->second.dis / gridPrecision) + 1;
				qiissC += (iter1->second).dis - (qiissC - 1)*gridPrecision > gridPrecision / 2.0 ? 1.0 : 0.0;
			}
			qiissC = qiissC < 2 ? 2 : qiissC;
			for (auto iter2 = (*iter1).first.begin(); iter2 != (*iter1).first.end(); ++iter2) {
				int startSLPI = quaCareSLs.size();
				standardSL(*iter2, qiissC, QuaSLInfo[*iter2]->selSVI >= 0);
				QuaSLInfo[*iter2]->refreshSLPInfo(startSLPI, quaCareSLs.size() - 1);
				int i = startSLPI;
				for (; i < quaCareSLs.size() - 1; i++) {
					quaCareSLs[i].color = i % 2 == 0 ? quaSSLColor1 : quaSSLColor2;
				}
				quaCareSLs[i].color = i % 2 == 0 ? quaSSLColor1 : quaSSLColor2;
			}
		}
		isGenQuaMesh = true;
		GenCareQua();
	}
}
void Mesh::updateLikeSLInfo(std::vector<int> sla) {
	//补充类曲线信息
	int slTypeSelSVI = -1;
	int iter3_selI = -1;
	for (int iter = 0; iter < sla.size(); iter++) {
		if (slTypeSelSVI < 0 && QuaSLInfo[sla[iter]]->selSVI >= 0) {
			slTypeSelSVI = QuaSLInfo[sla[iter]]->selSVI;
			iter3_selI = iter;
		}
		else if (QuaSLInfo[sla[iter]]->selSVI == -2) {
			slTypeSelSVI = -2;
			break;
		}
	}
	if (slTypeSelSVI == -2) {
		for (iter3_selI = 0; iter3_selI < sla.size(); iter3_selI++) {
			QuaSLInfo[sla[iter3_selI]]->selSVI = -1;
			QuaSLInfo[sla[iter3_selI]]->bpc = -1;
			QuaSLInfo[sla[iter3_selI]]->selSLSegR = 1.0;
		}
	}
	else if (slTypeSelSVI >= 0 && iter3_selI >= 0) {
		int slaSLI = sla[iter3_selI];
		double segSLR = QuaSLInfo[slaSLI]->selSLSegR;
		int qsnf[2] = { QuaSLInfo[slaSLI]->nearFaceI[0],QuaSLInfo[slaSLI]->nearFaceI[1] };
		for (int i = 0; i < 2; i++) {
			int curSSVI = slTypeSelSVI;
			int curSSSI = slaSLI;
			int curQsnf = qsnf[i];
			while (qsnf[i] >= 0 && qsnf[i] < quadraInfo.size()) {
				int objSSSI = -1;
				for (int j = 0; j < 4; j++) {
					if (quadraInfo[qsnf[i]].slI[j] == curSSSI) {
						objSSSI = quadraInfo[qsnf[i]].slI[(j + 2) % 4];
						int judgeSL[2];
						int auxSelSVI = QuaSLInfo[objSSSI]->startSingularIndex;
						findSLToSV(curSSVI, auxSelSVI, judgeSL);
						if (judgeSL[0] < 0 && judgeSL[1] < 0) {
							auxSelSVI = QuaSLInfo[objSSSI]->endSingularIndex;
							findSLToSV(curSSVI, auxSelSVI, judgeSL);
						}
						if (judgeSL[0] >= 0 || judgeSL[1] >= 0) {
							QuaSLInfo[objSSSI]->selSVI = auxSelSVI;
							QuaSLInfo[objSSSI]->selSLSegR = segSLR;
							curSSSI = objSSSI;
							curSSVI = auxSelSVI;
							qsnf[i] = QuaSLInfo[objSSSI]->nearFaceI[0] == qsnf[i] ? QuaSLInfo[objSSSI]->nearFaceI[1] : QuaSLInfo[objSSSI]->nearFaceI[0];
						}
						else {
							qsnf[i] = -1;
						}
						break;
					}
				}
				if (objSSSI < 0 || curQsnf == qsnf[i]) {
					break;
				}
			}
		}
	}
}
bool Mesh::IsBorderSL(int slI) {
	return slI >= 0 && slI < QuaSLInfo.size() && QuaSLInfo[slI]->isBorder;
}
bool Mesh::IsBorderSL(int slsI[], int len) {
	for (int i = 0; i < len; i++) {
		if (!IsBorderSL(slsI[i]))
			return false;
	}
	return true;
}
void Mesh::getSLDir(int slI, int dirValue[]) {
	if (QuaSLInfo[slI]->slpc > 3) {
		int returnValue = QuaSLInfo[slI]->startSLPointIndex;
		double valueTemp = 0.0;
		do {
			++returnValue;
			if (returnValue > quaSLs.size() - 1) {
				break;
			}
			valueTemp = (quaSLs[returnValue + 1].pos - quaSLs[returnValue].pos).norm();
		} while (valueTemp < 1.0e-6);
		dirValue[0] = returnValue;
		returnValue = QuaSLInfo[slI]->endSLPointIndex;
		valueTemp = 0.0;
		do {
			--returnValue;
			if (returnValue <= 0) {
				break;
			}
			valueTemp = (quaSLs[returnValue - 1].pos - quaSLs[returnValue].pos).norm();
		} while (valueTemp < 1.0e-6);
		dirValue[1] = returnValue;
	}
	else {
		dirValue[0] = QuaSLInfo[slI]->startSLPointIndex;
		dirValue[1] = QuaSLInfo[slI]->endSLPointIndex;
	}
}
bool Mesh::findOpSLI(int slI, int opSLI[]) {
	opSLI[0] = -1;
	opSLI[1] = -1;
	int quaSLInfoC = QuaSLInfo.size();
	if (slI >= 0 && slI < QuaSLInfo.size()) {
		int i = slI;
		int svse[2] = { QuaSLInfo[i]->startSingularIndex ,QuaSLInfo[i]->endSingularIndex };
		//为了过滤干扰项				
		int getSLDirValue[2];
		getSLDir(slI, getSLDirValue);
		Vec3 dirSLN[2] = { (quaSLs[getSLDirValue[0] + 1].pos - quaSLs[getSLDirValue[0]].pos).normalized(),(quaSLs[getSLDirValue[1] - 1].pos - quaSLs[getSLDirValue[1]].pos).normalized() };
		if (svse[0] >= 0 && svse[1] >= 0 && svse[0] < dirSV.size() && svse[1] < dirSV.size()) {
			bool isQua = false;			//false为无四边形				
			std::vector<std::pair<int, int>> aloaV[2];
			for (int k = 0; k < 2; k++) {
				std::vector<ToOrderDIV> maxValueT;
				for (int j = 0; j < 5; j++) {
					int dssI = dirSV[svse[k]]->streamLines[j];
					if (dssI != i && dssI != -1) {
						Vec3 dir;
						getSLDir(dssI, getSLDirValue);
						if (QuaSLInfo[dssI]->startSingularIndex == svse[k]) {
							dir = (quaSLs[getSLDirValue[0] + 1].pos - quaSLs[getSLDirValue[0]].pos).normalized();
						}
						else {
							dir = (quaSLs[getSLDirValue[1] - 1].pos - quaSLs[getSLDirValue[1]].pos).normalized();
						}
						maxValueT.push_back(ToOrderDIV(dir* dirSLN[k], dssI, dir));
					}
				}
				std::sort(maxValueT.begin(), maxValueT.end(), TCompToOrderDown);
				if (maxValueT.size() == 0) {
					findErrorId(i);
					return false;
				}
				else {
					int maxValueTIndex[2] = { maxValueT[0].sI,-1 };
					if (maxValueT.size() == 4 && !calParToP(maxValueT[0].dir, maxValueT[1].dir, dirSLN[k]) && maxValueT[0].dir*maxValueT[1].dir > -0.866) {
						//其中-0.866意义在于防止判断的方向接近相反，从而破坏判断的合理性
						maxValueTIndex[1] = maxValueT[2].sI;
					}
					else if (maxValueT.size() > 1) {
						maxValueTIndex[1] = maxValueT[1].sI;
					}
					for (int j = 0; j < 2; j++) {
						if (maxValueTIndex[j] >= 0) {
							if (QuaSLInfo[maxValueTIndex[j]]->startSingularIndex == svse[k]) {
								aloaV[k].push_back(std::pair<int, int>(QuaSLInfo[maxValueTIndex[j]]->endSingularIndex, maxValueTIndex[j]));
							}
							else {
								aloaV[k].push_back(std::pair<int, int>(QuaSLInfo[maxValueTIndex[j]]->startSingularIndex, maxValueTIndex[j]));
							}
						}
					}
				}
			}
			if (aloaV[0].size() == 0 || aloaV[1].size() == 0) {
				findErrorId(i);
				return false;
			}
			else {
				int opSLII = 0;
				for (auto iter1 = aloaV[0].begin(); iter1 != aloaV[0].end(); ++iter1) {
					for (auto iter2 = aloaV[1].begin(); iter2 != aloaV[1].end(); ++iter2) {
						int fsTir[2][2] = { -1,-1,-1,-1 };
						findSLToSV(svse[0], iter2->first, fsTir[0]);
						findSLToSV(svse[1], iter1->first, fsTir[1]);
						for (int j = 0; j < 2; j++) {
							for (int jj = 0; jj < 2; jj++) {
								if (fsTir[j][jj] >= 0) {
									//三角区域
									if (QuaSLInfo[i]->isBorder && QuaSLInfo[j == 0 ? iter2->second : iter1->second]->isBorder && QuaSLInfo[fsTir[j][jj]]->isBorder) {
										continue;
									}
									findErrorId(i);
									findErrorId(j == 0 ? iter2->second : iter1->second);
									findErrorId(fsTir[j][jj]);
									return false;
								}
							}
						}
						int fsIArray[2] = { -1,-1 };
						findSLToSV((*iter1).first, (*iter2).first, fsIArray);
						int fsI = -1;
						if (fsIArray[1] >= 0) {
							//此时获得两条流线索引
							//19.5.16
							//在建立边界去向上必须有三个点后，以下情况不存在
							if (QuaSLInfo[fsIArray[0]]->isBorder && QuaSLInfo[fsIArray[1]]->isBorder) {
								Vec3 spi1 = quaSLs[QuaSLInfo[fsIArray[0]]->slpc / 2 + QuaSLInfo[fsIArray[0]]->startSLPointIndex].pos;
								Vec3 spi2 = quaSLs[QuaSLInfo[fsIArray[1]]->slpc / 2 + QuaSLInfo[fsIArray[1]]->startSLPointIndex].pos;
								Vec3 jpi = quaSLs[QuaSLInfo[slI]->slpc / 2 + QuaSLInfo[slI]->startSLPointIndex].pos;
								if ((jpi - spi1).norm() < (jpi - spi2).norm()) {
									fsI = fsIArray[0];
								}
								else {
									fsI = fsIArray[1];
								}
							}
						}
						else {
							fsI = fsIArray[0];
						}
						if (fsI >= 0) {
							bool isExist = false;
							for (int j = 0; j < opSLII; j++) {
								if (opSLI[j] == fsI) {
									isExist = true;
									break;
								}
							}
							if (!isExist) {
								if (opSLII > 1) {
									//if (i != 271)
									findErrorId(i);
									return false;
								}
								else {
									bool isOrderOrB = true;
									int slI_quaB[4] = { slI,iter2->second,fsI,iter1->second };
									if (IsBorderSL(slI_quaB, 4)) {
										if (isOrderOrB) {
											double areaBQua = 0.0;
											for (int qsIsI = 0; qsIsI < 4; qsIsI++) {
												if (dirSV[QuaSLInfo[slI_quaB[qsIsI]]->startSingularIndex]->faceI != -4 || dirSV[QuaSLInfo[slI_quaB[qsIsI]]->endSingularIndex]->faceI != -4) {
													isOrderOrB = false;
													break;
												}
												if (QuaSLInfo[slI_quaB[qsIsI]]->dir) {
													for (int slnpcI = QuaSLInfo[slI_quaB[qsIsI]]->startSLPointIndex; slnpcI < QuaSLInfo[slI_quaB[qsIsI]]->endSLPointIndex; slnpcI++) {
														areaBQua += quaSLs[slnpcI].pos.Outer_product(quaSLs[slnpcI + 1].pos);
													}
												}
												else {
													for (int slnpcI = QuaSLInfo[slI_quaB[qsIsI]]->endSLPointIndex; slnpcI > QuaSLInfo[slI_quaB[qsIsI]]->startSLPointIndex; slnpcI--) {
														areaBQua += quaSLs[slnpcI].pos.Outer_product(quaSLs[slnpcI - 1].pos);
													}
												}
											}
											if (areaBQua < 0.0) {
												isOrderOrB = false;
											}
										}
									}
									else {
										//过滤边界情况：出现如下情况是边界曲线只有两个点时
										int quaSl_iter1 = -1;
										int quaslbc = 0;
										for (int quaSL_iter2 = 0; quaSL_iter2 < 4; quaSL_iter2++) {
											if (QuaSLInfo[slI_quaB[quaSL_iter2]]->isBorder) {
												quaSl_iter1 = quaSL_iter2;
												++quaslbc;
											}
										}
										if (quaSl_iter1 >= 0 && quaslbc > 1) {
											int judgeHeadVI = QuaSLInfo[slI_quaB[quaSl_iter1]]->dir ? QuaSLInfo[slI_quaB[quaSl_iter1]]->startSingularIndex : QuaSLInfo[slI_quaB[quaSl_iter1]]->endSingularIndex;
											int s_judgeHeadVI = QuaSLInfo[slI_quaB[quaSl_iter1]]->endSingularIndex == judgeHeadVI ? QuaSLInfo[slI_quaB[quaSl_iter1]]->startSingularIndex : QuaSLInfo[slI_quaB[quaSl_iter1]]->endSingularIndex;
											do {
												int sqi_1_4 = slI_quaB[(quaSl_iter1++) % 4];
												if (QuaSLInfo[sqi_1_4]->isBorder) {
													if (QuaSLInfo[sqi_1_4]->dir) {
														if (judgeHeadVI != QuaSLInfo[sqi_1_4]->startSingularIndex) {
															isOrderOrB = false;
															break;
														}
														else {
															judgeHeadVI = QuaSLInfo[sqi_1_4]->endSingularIndex;
														}
													}
													else {
														if (judgeHeadVI != QuaSLInfo[sqi_1_4]->endSingularIndex) {
															isOrderOrB = false;
															break;
														}
														else {
															judgeHeadVI = QuaSLInfo[sqi_1_4]->startSingularIndex;
														}
													}
												}
												else {
													judgeHeadVI = QuaSLInfo[sqi_1_4]->startSingularIndex != judgeHeadVI ? QuaSLInfo[sqi_1_4]->startSingularIndex : QuaSLInfo[sqi_1_4]->endSingularIndex;
												}
											} while (s_judgeHeadVI != judgeHeadVI);
										}
									}
									if (isOrderOrB) {
										opSLI[opSLII++] = fsI;
										GenRoughQua(svse[0], svse[1], iter2->first, iter1->first, slI, iter2->second, fsI, iter1->second);
									}
								}
							}
							isQua = true;
						}
					}
				}
			}
			if (!isQua) {
				//不存在四边形
				findErrorId(i);
				return false;
			}
		}
	}
	if (!(opSLI[0] >= 0 && opSLI[0] < QuaSLInfo.size())) {
		findErrorId(slI);
		return false;
	}
	if (opSLI[1] < 0 && (dirSV[QuaSLInfo[slI]->endSingularIndex]->faceI >= -1 || dirSV[QuaSLInfo[slI]->startSingularIndex]->faceI >= -1)) {
		findErrorId(slI);
		return false;
	}
	return opSLI[0] >= 0 && opSLI[0] < QuaSLInfo.size();
}
void Mesh::preGenQuaStatec() {
	isFinish = false;
	isQuaSmooth = false;
	isFourMesh = arrSLOrder() >= 0;
	if (!isFourMesh) {
		return;
	}
	isTAIA = JudgeTAIA(gridPrecision, false);
	double gp = gridPrecision;
	while (!isTAIA)
	{
		if (JudgeTAIA(gp*5.0, false)) {
			gridPrecision = gp* 5.0;
			isTAIA = true;
		}
		else {
			gp *= 5.0;
		}
	}
}
//容器：第一个参数容器为属于该类的流线，第二个参数为该类的流线数（包括首尾端点）
int Mesh::arrSLOrder() {
	int slTypeI = 0;
	quadraInfo.clear();
	quadraInfo.reserve(QuaSLInfo.size());
	int quaSLInfoC = QuaSLInfo.size();
	slType.clear();
	std::vector<ToOrderDI> slTypeS;
	std::vector<std::vector<int>> slSet;
	std::vector<double> slSetToPC;
	for (int i = 0; i < quaSLInfoC; i++) {
		//更新流线长度
		double distanceSl = 0.0;
		for (int j = QuaSLInfo[i]->startSLPointIndex; j < QuaSLInfo[i]->endSLPointIndex; j++) {
			distanceSl += (quaSLs[j + 1].pos - quaSLs[j].pos).norm();
		}
		QuaSLInfo[i]->refreshDisSLInfo(distanceSl);
		QuaSLInfo[i]->typeSL = -1;
	}
	for (int i = 0; i < quaSLInfoC; i++) {
		//利用流线两端点进行局部寻找对边		
		if (QuaSLInfo[i]->typeSL == -1) {
			QuaSLInfo[i]->typeSL = slTypeI;
			slTypeS.clear();
			slTypeS.push_back(ToOrderDI(QuaSLInfo[i]->distanceSL, i));
			int opSLI[2];
			if (findOpSLI(i, opSLI)) {
				for (int j = 0; j < 2; j++) {
					int tempOpSLI = i;
					while (opSLI[j] != -1 && QuaSLInfo[opSLI[j]]->typeSL == -1) {
						//int qotI1 = QuaSLInfo[opSLI[j]]->typeSL;
						QuaSLInfo[opSLI[j]]->typeSL = slTypeI;
						//slTypeS.push_back(ToOrder(QuaSLInfo[i]->slpc, opSLI[j]));
						slTypeS.push_back(ToOrderDI(QuaSLInfo[opSLI[j]]->distanceSL, opSLI[j]));
						int opSLII[2];
						if (findOpSLI(opSLI[j], opSLII)) {
							if (opSLII[0] == tempOpSLI) {
								tempOpSLI = opSLI[j];
								opSLI[j] = opSLII[1];
							}
							else {
								tempOpSLI = opSLI[j];
								opSLI[j] = opSLII[0];
							}
						}
						else {
							qDebug("error!(3422)");
							return -1;
						}
					}
				}
				if (opSLI[0] >= 0 && opSLI[1] >= 0 && QuaSLInfo[opSLI[0]]->typeSL != QuaSLInfo[opSLI[1]]->typeSL) {
					qDebug("error!(3290)");
					return -1;
				}
			}
			else {
				qDebug("error!(3272)");
				return -1;
			}
			if (slTypeS.size() == 0) {
				qDebug("error!(3318)");
				return -1;
			}
			else {
				//需要在此为记录的流线按段数大小排序：依次增大
				std::sort(slTypeS.begin(), slTypeS.end(), compToOrderDUp);
				std::vector<int> tempSSLI;
				for (int k = 0; k < slTypeS.size(); k++) {
					tempSSLI.push_back(slTypeS[k].sI);
				}
				slSet.push_back(tempSSLI);
				slSetToPC.push_back(slTypeS[slTypeS.size() - 1].dis);
			}
			++slTypeI;
		}
	}
	for (int i = 0; i < slSet.size(); i++) {
		int bslnp = -1;
		for (int j = 0; j < slSet[i].size(); j++) {
			if (QuaSLInfo[slSet[i][j]]->bpc > 0) {
				bslnp = QuaSLInfo[slSet[i][j]]->bpc;
			}
			if (QuaSLInfo[slSet[i][j]]->selSVI == -2) {
				bslnp = -1;
				break;
			}
		}
		slType.insert(std::pair<std::vector<int>, slType_Secand>(slSet[i], slType_Secand(slSetToPC[i], bslnp)));
	}
	return slTypeI;	//类别数
}
void Mesh::standardSL(int slI, int slC, bool isOk) {
	//slC：组成流线的流点数，段数为slC-1
	if (slI >= 0 && slI < QuaSLInfo.size() && slC >= 2) {
		double curSLSegDis = 0.0;
		double IsoSLSegDis = 0.0;
		std::vector<double> slSegDis;
		if (!isOk) {
			IsoSLSegDis = QuaSLInfo[slI]->distanceSL / (slC - 1);	//等距
			for (int i = 0; i < slC - 1; i++) {
				slSegDis.push_back(IsoSLSegDis);
			}
		}
		else {
			//需做成数组
			IsoSLSegDis = QuaSLInfo[slI]->distanceSL / ((slC - 1)*slC / 2);
			int sln1, sln2;
			double isoDis = QuaSLInfo[slI]->selSLSegR*QuaSLInfo[slI]->distanceSL;
			double difDis = QuaSLInfo[slI]->distanceSL - isoDis;
			if (fabs(isoDis) < 1.0e-5) {
				sln2 = slC - 1;
				sln1 = 0;
			}
			else if (fabs(difDis) < 1.0e-5) {
				sln1 = slC - 1;
				sln2 = 0;
				IsoSLSegDis = QuaSLInfo[slI]->distanceSL / sln1;
			}
			else {
				double paraDis1 = 2.0*difDis / isoDis;
				sln2 = int((sqrt((3.0 + 2.0*paraDis1)*(3.0 + 2.0*paraDis1) + 8.0*(slC - 1)*paraDis1) - (3.0 + 2.0*paraDis1)) / 2.0);
				//sln2 = int((sqrt((1.0 + paraDis1)*(1.0 + paraDis1) + 4.0*paraDis1*(slC - 1)) - 1.0 - paraDis1) / 2.0);
				sln1 = slC - 1 - sln2;
				if (sln1 <= 0) {
					sln2 = slC - 1;
					sln1 = 0;
				}
				else if (sln1 >= slC - 1) {
					sln1 = slC - 1;
					sln2 = 0;
					IsoSLSegDis = QuaSLInfo[slI]->distanceSL / sln1;
				}
				else {
					IsoSLSegDis = isoDis / sln1;
				}
			}
			double asdfDis = 0.0;
			for (int i = 0; i < sln1; i++) {
				slSegDis.push_back(IsoSLSegDis);
				asdfDis += IsoSLSegDis;
			}
			//difDis = QuaSLInfo[slI]->distanceSL - IsoSLSegDis*sln1;
			//IsoSLSegDis = difDis / ((sln2 + 1)*sln2 / 2);
			for (int j = 1; j < sln2; j++) {
				//slSegDis.push_back(IsoSLSegDis*j);
				slSegDis.push_back(IsoSLSegDis*(j + 1) / 2.0);
				asdfDis += IsoSLSegDis*(j + 1) / 2.0;
			}
			slSegDis.push_back(QuaSLInfo[slI]->distanceSL - asdfDis);
			//asdfDis = asdfDis;
		}
		double auxIsoSLSegDis = IsoSLSegDis;
		double ccurSLSegDis = 0.0;					//当前行进距离		
		double leftSLSegDis = 0.0;
		int curSQMSLC = quaCareSLs.size();
		quaCareSLs.push_back(quaSLs[QuaSLInfo[slI]->startSLPointIndex].pos);
		int iterNum = 0;
		bool isSeq = QuaSLInfo[slI]->startSingularIndex == QuaSLInfo[slI]->selSVI;
		if (isOk && !isSeq) {
			iterNum = slC - 2;
			//IsoSLSegDis = auxIsoSLSegDis*iterNum;
		}
		for (int i = QuaSLInfo[slI]->startSLPointIndex + 1; i <= QuaSLInfo[slI]->endSLPointIndex && quaCareSLs.size() - curSQMSLC + 1 < slC; i++) {
			double ssnd = (quaSLs[i].pos - quaSLs[i - 1].pos).norm();
			ccurSLSegDis += ssnd;
			bool isCon = false;
			while (iterNum < slC - 1 && iterNum >= 0 && ccurSLSegDis > slSegDis[iterNum] && quaCareSLs.size() - curSQMSLC + 1 < slC) {
				double denNum = ccurSLSegDis - leftSLSegDis;
				if (fabs(denNum) < 1e-5) {
					quaCareSLs.push_back(isCon ? quaCareSLs[quaCareSLs.size() - 1].pos : quaSLs[i].pos);
				}
				else {
					double ratioDis = (slSegDis[iterNum] - leftSLSegDis) / denNum;
					quaCareSLs.push_back((isCon ? quaCareSLs[quaCareSLs.size() - 1].pos : quaSLs[i - 1].pos) * (1.0 - ratioDis) + quaSLs[i].pos * ratioDis);
				}
				ccurSLSegDis -= slSegDis[iterNum];
				if (isOk) {
					iterNum += isSeq ? 1 : -1;
					//IsoSLSegDis = auxIsoSLSegDis* iterNum;
				}
				else {
					iterNum++;
				}
				leftSLSegDis = 0.0;
				isCon = true;
			}
			if (!isCon) {
				leftSLSegDis += ssnd;
			}
			else {
				leftSLSegDis = ccurSLSegDis;
			}
		}
		quaCareSLs.push_back(quaSLs[QuaSLInfo[slI]->endSLPointIndex].pos);
	}
}
void Mesh::GenRoughQua(int slp1, int slp2, int slp3, int slp4, int slI1, int slI2, int slI3, int slI4) {
	//参数一至四为顺序流点索引，参数五至八为对应流点间的流线索引
	//若是且未记录该四边形，则记录，否则返回false
	//函数使用位置，位于粗四边形边的对边寻找位置
	//在记录四边形结构时，存储顺序为逆时针或顺时针，并且标明依据存储节点的流线“正反”
	//在使用该函数的最大约束是默认该粗四边形符合条件
	for (auto iter = quadraInfo.begin(); iter != quadraInfo.end(); ++iter) {
		int i = 0;
		for (; i < 5; i++) {
			if (i < 4 && slp1 == iter->slP[i]) {
				break;
			}
		}
		if (i == 4) {
			continue;
		}
		else {
			if ((slp2 == iter->slP[(i + 1) % 4] && slp3 == iter->slP[(i + 2) % 4] && slp4 == iter->slP[(i + 3) % 4]) ||
				(slp2 == iter->slP[(i + 3) % 4] && slp3 == iter->slP[(i + 2) % 4] && slp4 == iter->slP[(i + 1) % 4])) {
				//顺序存在逆反
				return;
			}
		}
	}
	int faceI = quadraInfo.size();
	QuaSLInfo[slI1]->setNearFaceI(faceI);
	QuaSLInfo[slI2]->setNearFaceI(faceI);
	QuaSLInfo[slI3]->setNearFaceI(faceI);
	QuaSLInfo[slI4]->setNearFaceI(faceI);
	quadraInfo.push_back(Quadra(slI1, slI2, slI3, slI4, slp1, slp2, slp3, slp4));
}
void Mesh::GenCareQua() {
	if (quadraInfo.size() > 0 && isGenQuaMesh) {
		isFinish = false;
		isQuaSmooth = false;
		std::vector<quadraPInfo>().swap(careQuaInfo);
		std::vector<vertAloa>().swap(careQuaMeshP);
		std::vector<QuaIndex>().swap(careQuaIndex);
		std::vector<LineIndex>().swap(careQuaBorderInlet);
		std::vector<LineIndex>().swap(careQuaBorderOutlet);
		std::vector<LineIndex>().swap(careQuaBorderWall0);
		std::vector<LineIndex>().swap(careQuaBorderWall1);
		quaMinAngle = 180.0;
		quaMinYkbA = 1.0;
		quaMinYkbB = 1.0;
		smoIterTimes = 0;
		//由于需要建立细四边形的拓扑结构，因此，需要记录“已经找到的点”，即流线上的点
		std::vector<int> quaslPI;
		quaslPI.clear();
		quaslPI.reserve(quaCareSLs.size());
		for (int i = 0; i < quaCareSLs.size(); i++) {
			quaslPI.push_back(-1);
		}
		//先将节点添加
		for (int i = 0; i < dirSV.size(); i++) {
			int vertTypeP = dirSV[i]->faceI == -3 ? 1 : dirSV[i]->faceI == -2 || dirSV[i]->faceI == -4 ? 2 : 0;
			careQuaMeshP.push_back(vertAloa(dirSV[i]->pos, vertTypeP, -1));
			//careQuaMeshColor.push_back(careQuaColor);
		}
		//添加节点索引
		for (int i = 0; i < QuaSLInfo.size(); i++) {
			quaslPI[QuaSLInfo[i]->auxStartSLPI] = QuaSLInfo[i]->startSingularIndex;
			quaslPI[QuaSLInfo[i]->auxEndSLPI] = QuaSLInfo[i]->endSingularIndex;
		}
		int quaBlc = 0;	//四边形网格边界段数
		int likeQuadraInfo = 0;	//为了给类四边形填色		
		for (auto iter = quadraInfo.begin(); iter != quadraInfo.end(); ++iter, ++likeQuadraInfo) {
			int cols = QuaSLInfo[iter->slI[0]]->aslpc - 1;
			int rows = QuaSLInfo[iter->slI[1]]->aslpc - 1;
			int qisI[4][2];
			bool isBSL[4];
			for (int i = 0; i < 4; i++) {
				int ii = iter->slI[i];
				isBSL[i] = QuaSLInfo[ii]->isBorder;
				qisI[i][0] = QuaSLInfo[ii]->auxStartSLPI;
				qisI[i][1] = QuaSLInfo[ii]->auxEndSLPI;
				//记录边界数
				if (isBSL[i]) {
					quaBlc += QuaSLInfo[ii]->aslpc - 1;
				}
			}
			int cqpc = careQuaMeshP.size();
			bool dirG[4];	//流线走向
			std::vector<int> colrowPI[4];	//记录流线流点索引，该索引与细分四边形时相对应
			for (int i = 0; i < 4; i++) {
				bool bqissIis = QuaSLInfo[iter->slI[i]]->startSingularIndex == iter->slP[i];
				dirG[i] = bqissIis;
				//建立流线点的索引
				int j = qisI[i][0];
				if (isBSL[i]) {
					//由于是边界，所以只存在一个单元（四边形）
					int curCQMPI = careQuaMeshP.size();
					int startCQPI, endCQPI;
					if ((i < 2 && bqissIis) || (i >= 2 && !bqissIis)) {
						startCQPI = QuaSLInfo[iter->slI[i]]->startSingularIndex;
						endCQPI = QuaSLInfo[iter->slI[i]]->endSingularIndex;
					}
					else {
						endCQPI = QuaSLInfo[iter->slI[i]]->startSingularIndex;
						startCQPI = QuaSLInfo[iter->slI[i]]->endSingularIndex;
					}
					//QuaBoundaryType qbts = QuaSLInfo[iter->slI[i]]->qbt;
					QuaSLInfo[iter->slI[i]]->auxQBT.init(startCQPI, endCQPI, curCQMPI, curCQMPI + qisI[i][1] - qisI[i][0] - 1);
				}
				if (quaslPI[j + 1] != -1) {
					//+1目的：判断该流线是否已经被标记过
					if ((i < 2 && bqissIis) || (i >= 2 && !bqissIis)) {
						for (int k = j; k <= qisI[i][1]; k++) {
							colrowPI[i].push_back(quaslPI[k]);
						}
					}
					else {
						for (int k = qisI[i][1]; k >= j; k--) {
							colrowPI[i].push_back(quaslPI[k]);
						}
					}
				}
				else {
					if ((i < 2 && bqissIis) || (i >= 2 && !bqissIis)) {
						colrowPI[i].push_back(quaslPI[j]);
						for (j++; j < qisI[i][1]; j++) {
							colrowPI[i].push_back(cqpc);
							quaslPI[j] = cqpc++;
							careQuaMeshP.push_back(vertAloa(quaCareSLs[j].pos, isBSL[i] ? 1 : 0, -2));
							//careQuaMeshColor.push_back(careQuaColor);
						}
						colrowPI[i].push_back(quaslPI[qisI[i][1]]);
					}
					else {
						int k = qisI[i][1];
						colrowPI[i].push_back(quaslPI[k]);
						for (k--; k > j; k--) {
							colrowPI[i].push_back(cqpc);
							quaslPI[k] = cqpc++;
							careQuaMeshP.push_back(vertAloa(quaCareSLs[k].pos, isBSL[i] ? 1 : 0, -2));
							//careQuaMeshColor.push_back(careQuaColor);
						}
						colrowPI[i].push_back(quaslPI[j]);
					}
				}
			}
			//分离出四条流线，并按照一定规则“排序”，正正负负
			std::vector<Vec3> colP[2];
			std::vector<Vec3> rowP[2];
			for (int i = 0; i < cols + 1; i++) {
				colP[0].push_back(quaCareSLs[dirG[0] ? qisI[0][0] + i : qisI[0][1] - i].pos);
				colP[1].push_back(quaCareSLs[dirG[2] ? qisI[2][1] - i : qisI[2][0] + i].pos);
			}
			for (int i = 0; i < rows + 1; i++) {
				rowP[0].push_back(quaCareSLs[dirG[1] ? qisI[1][0] + i : qisI[1][1] - i].pos);
				rowP[1].push_back(quaCareSLs[dirG[3] ? qisI[3][1] - i : qisI[3][0] + i].pos);
			}
			bool orderQuaInfo = false; //true：可用、false：不可用
			bool orderUQuaInfo = true;	//true：逆时针、false：顺时针
										//通过逐点方法求解可知，可预先知道已存在的三个点索引	
			if (cols == 1 || rows == 1) {
				cols = cols;
			}
			for (int i = 1; i <= cols; i++) {
				int cQMPI[4] = { colrowPI[3][0],colrowPI[0][i],-1,colrowPI[3][1] };
				//int cQMPI[3] = { colrowPI[3][0],colrowPI[0][i],colrowPI[3][1] };
				if (i < cols) {
					for (int j = 1; j < rows; j++) {
						double t = j*1.0 / rows;
						double s = i*1.0 / cols;
						Vec3 lc = colP[0][i] * (1.0 - t) + colP[1][i] * t;
						Vec3 lr = rowP[0][j] * s + rowP[1][j] * (1.0 - s);
						Vec3 bcr = colP[0][0] * (1.0 - s)*(1.0 - t) + colP[0][cols] * s*(1.0 - t) + colP[1][0] * (1.0 - s)*t + colP[1][cols] * s*t;
						Vec3 tarP = lc + lr - bcr;
						cQMPI[2] = careQuaMeshP.size();;
						//careQuaIndex.push_back(QuaIndex(cQMPI));
						careQuaMeshP.push_back(vertAloa(tarP, 0, likeQuadraInfo));
						if (!orderQuaInfo) {
							double sumA = 0;
							for (int k = 0; k < 4; k++) {
								sumA += careQuaMeshP[cQMPI[k]].pos.Outer_product(careQuaMeshP[cQMPI[(k + 1) % 4]].pos);
							}
							orderUQuaInfo = sumA > 0.0 ? true : false;
							orderQuaInfo = true;
						}
						GenPoPAloa(cQMPI, orderUQuaInfo);
						//careQuaMeshColor.push_back(careQuaColor);
						//更新参考数组
						colrowPI[3][j - 1] = cQMPI[1];
						colrowPI[3][j] = cQMPI[2];
						//更新模板索引
						cQMPI[0] = cQMPI[3];
						cQMPI[1] = cQMPI[2];
						cQMPI[3] = colrowPI[3][j + 1];
					}
					cQMPI[2] = colrowPI[2][i];
					if (!orderQuaInfo) {
						double sumA = 0;
						for (int k = 0; k < 4; k++) {
							sumA += careQuaMeshP[cQMPI[k]].pos.Outer_product(careQuaMeshP[cQMPI[(k + 1) % 4]].pos);
						}
						orderUQuaInfo = sumA > 0.0 ? true : false;
						orderQuaInfo = true;
					}
					GenPoPAloa(cQMPI, orderUQuaInfo);
					colrowPI[3][rows] = cQMPI[2];
					if (rows == 1) {
						colrowPI[3][0] = cQMPI[1];
					}
				}
				else {
					for (int j = 1; j <= rows; j++) {
						cQMPI[2] = colrowPI[1][j];
						if (!orderQuaInfo) {
							double sumA = 0;
							for (int k = 0; k < 4; k++) {
								sumA += careQuaMeshP[cQMPI[k]].pos.Outer_product(careQuaMeshP[cQMPI[(k + 1) % 4]].pos);
							}
							orderUQuaInfo = sumA > 0.0 ? true : false;
							orderQuaInfo = true;
						}
						GenPoPAloa(cQMPI, orderUQuaInfo);
						cQMPI[0] = cQMPI[3];
						cQMPI[1] = cQMPI[2];
						if (j != rows)		//防止访问越界
							cQMPI[3] = colrowPI[3][j + 1];
					}
				}
			}
		}
		setLikeCareQuaStruct();
		for (int i = 0; i < careQuaMeshP.size(); i++) {
			if (careQuaMeshP[i].faceI >= 0) {
				careQuaMeshP[i].color = colorArrayCareQua[abs(quadraInfo[careQuaMeshP[i].faceI].color) % 6];
			}
		}

		isFinish = true;
		setQBT();
		//smoothMesh();
	}
	else {
		isFinish = false;
	}
}
void Mesh::setQBT() {
	if (isFinish) {
		std::vector<LineIndex>().swap(careQuaBorderInlet);
		std::vector<LineIndex>().swap(careQuaBorderOutlet);
		std::vector<LineIndex>().swap(careQuaBorderWall0);
		std::vector<LineIndex>().swap(careQuaBorderWall1);
		for (int i = 0; i < QuaSLInfo.size(); i++) {
			if (QuaSLInfo[i]->qbt != INDOOR) {
				int startPI = QuaSLInfo[i]->auxQBT.auxQBTSLSPI;
				int endPI = QuaSLInfo[i]->auxQBT.auxQBTSLEPI;
				int slSPI = QuaSLInfo[i]->auxQBT.slpSI;
				int slEPI = QuaSLInfo[i]->auxQBT.slpEI;
				std::vector<LineIndex> tempQBT;
				for (int j = slSPI; j < slEPI; j++) {
					tempQBT.push_back(LineIndex(startPI, slSPI));
					startPI = slSPI++;
				}
				tempQBT.push_back(LineIndex(startPI, endPI));
				switch (QuaSLInfo[i]->qbt)
				{
				case INLET:
					careQuaBorderInlet.insert(careQuaBorderInlet.end(), tempQBT.begin(), tempQBT.end());
					break;
				case OUTLET:
					careQuaBorderOutlet.insert(careQuaBorderOutlet.end(), tempQBT.begin(), tempQBT.end());
					break;
				case WALL0:
					careQuaBorderWall0.insert(careQuaBorderWall0.end(), tempQBT.begin(), tempQBT.end());
					break;
				case WALL1:
					careQuaBorderWall1.insert(careQuaBorderWall1.end(), tempQBT.begin(), tempQBT.end());
					break;
				default:
					break;
				}
			}
		}
		//new
		for (int i = 0; i < careQuaBorderInlet.size(); i++) {
			careQuaMeshP[careQuaBorderInlet[i][0]].color = qbtColor[0];
			careQuaMeshP[careQuaBorderInlet[i][1]].color = qbtColor[0];
		}
		for (int i = 0; i < careQuaBorderOutlet.size(); i++) {
			careQuaMeshP[careQuaBorderOutlet[i][0]].color = qbtColor[1];
			careQuaMeshP[careQuaBorderOutlet[i][1]].color = qbtColor[1];
		}
		for (int i = 0; i < careQuaBorderWall0.size(); i++) {
			careQuaMeshP[careQuaBorderWall0[i][0]].color = qbtColor[2];
			careQuaMeshP[careQuaBorderWall0[i][1]].color = qbtColor[2];
		}
		for (int i = 0; i < careQuaBorderWall1.size(); i++) {
			careQuaMeshP[careQuaBorderWall1[i][0]].color = qbtColor[3];
			careQuaMeshP[careQuaBorderWall1[i][1]].color = qbtColor[3];
		}
	}
}
void Mesh::GenPoPAloa(int svI[], bool order) {
	double jabMin = 2.0;
	double jabMax = -1.0;
	double angle[4];
	double jabBMin = 1.0e9;
	double jabBMax = 1.0e-9;
	Vec3 cqp[4];
	for (int i = 0; i < 4; i++) {
		Vec3 op = careQuaMeshP[svI[i]].pos;
		cqp[i] = op;
		Vec3 dir1 = careQuaMeshP[svI[(i + 1) % 4]].pos - op;
		Vec3 dir2 = careQuaMeshP[svI[(i + 3) % 4]].pos - op;
		angle[i] = acos(dir1.normalized()*dir2.normalized()) * 180.0 / Q_PI;
		double temp = dir1.norm()*dir2.norm()* sin(acos(dir1.normalized() *dir2.normalized()));
		if (temp > jabMax) {
			jabMax = temp;
		}
		if (temp < jabMin) {
			jabMin = temp;
		}
		if (dir1.norm() < jabBMin) {
			jabBMin = dir1.norm();
		}
		if (dir2.norm() < jabBMin) {
			jabBMin = dir2.norm();
		}
		if (dir1.norm() > jabBMax) {
			jabBMax = dir1.norm();
		}
		if (dir2.norm() > jabBMax) {
			jabBMax = dir2.norm();
		}
	}
	qsort(angle, 4, sizeof(angle[0]), cmp);


	double ykb_scale = v_quad_scaled_jacobian(cqp);
	careQuaInfo.push_back(quadraPInfo(ykb_scale, jabBMin / jabBMax, angle));

	careQuaIndex.push_back(order ? QuaIndex(svI) : QuaIndex(svI[3], svI[2], svI[1], svI[0]));
	if (quaMinYkbA > ykb_scale) {
		quaMinYkbA = ykb_scale;
	}
	if (quaMinYkbB > jabBMin / jabBMax) {
		quaMinYkbB = jabBMin / jabBMax;
	}
	if (quaMinAngle > angle[0]) {
		quaMinAngle = angle[0];
	}
	for (int i = 0; i < 4; i++) {
		careQuaMeshP[svI[i]].addVaI(svI[(i + 1) % 4]);
		careQuaMeshP[svI[i]].addVlI(svI[(i + 2) % 4]);
		careQuaMeshP[svI[i]].addVaI(svI[(i + 3) % 4]);
	}
}
int Mesh::setLikeCareQuaColor(int faceI) {
	int returnValue = 0;
	int nearFaceI[4];
	for (int i = 0; i < 4; i++) {
		if (QuaSLInfo[quadraInfo[faceI].slI[i]]->nearFaceI[0] == faceI) {
			nearFaceI[i] = QuaSLInfo[quadraInfo[faceI].slI[i]]->nearFaceI[1];
		}
		else {
			nearFaceI[i] = QuaSLInfo[quadraInfo[faceI].slI[i]]->nearFaceI[0];
		}
	}
	while (true) {
		int curRV = returnValue;
		for (int i = 0; i < 4; i++) {
			if (nearFaceI[i] >= 0 && quadraInfo[nearFaceI[i]].color == returnValue) {
				++returnValue;
				break;
			}
		}
		if (curRV == returnValue) break;
	}
	return returnValue;
}
void Mesh::setLikeCareQuaStruct() {
	if (quadraInfo.size() > 0) {
		for (int i = 0; i < quadraInfo.size(); i++) {
			quadraInfo[i].color = setLikeCareQuaColor(i);
		}
	}
}
int Mesh::smoothMesh() {
	if (isFinish) {
		//end
		//手动输入迭代次数
		bool isOK;
		QString text = QInputDialog::getText(NULL, "Input Dialog",
			"iter number:(int)",
			QLineEdit::Normal,
			"",
			&isOK);
		//end
		int timesIter = text.toInt();
		if (timesIter <= 0 || timesIter > 100) {
			timesIter = 10;
		}
		//int timesIter = 100;
		int times = 0;
		while (times++ < timesIter) {
			for (int i = 0; i < careQuaMeshP.size(); i++) {
				if (careQuaMeshP[i].vertType == 0) {
					Vec3 tempCQPV1;
					Vec3 tempCQPV2;
					if (careQuaMeshP[i].napt != careQuaMeshP[i].nlpt) {
						break;
					}
					for (int j = 0; j < careQuaMeshP[i].napt; j++) {
						tempCQPV1 += careQuaMeshP[careQuaMeshP[i].vaI[j]].npos;
						tempCQPV2 += careQuaMeshP[careQuaMeshP[i].vlI[j]].npos;
					}
					if (careQuaMeshP[i].napt != 4 || 1 == 1) {
						careQuaMeshP[i].npos = tempCQPV1 / careQuaMeshP[i].napt;
					}
					else {
						careQuaMeshP[i].npos = tempCQPV1*0.5 - tempCQPV2*0.25;
					}
				}
				else if (careQuaMeshP[i].vertType == 1) {
					Vec3 tempCQPV;
					int cpvt[5] = { -1,-1,-1,-1,-1 };
					int cpvtC = 0;
					for (int j = 0; j < careQuaMeshP[i].napt; j++) {
						if (careQuaMeshP[careQuaMeshP[i].vaI[j]].vertType == 1 || careQuaMeshP[careQuaMeshP[i].vaI[j]].vertType == 2) {
							cpvt[cpvtC++] = careQuaMeshP[i].vaI[j];
						}
						tempCQPV += careQuaMeshP[careQuaMeshP[i].vaI[j]].npos;
					}
					if (careQuaMeshP[i].napt > 3) {
						//非边界情况
						//careQuaMeshP[i].pos = tempCQPV / careQuaMeshP[i].npt;
						careQuaMeshP[i].npos = (careQuaMeshP[i].npos + tempCQPV / careQuaMeshP[i].napt)*0.5;
						if (cpvtC == 2) {
							double disvt[3] = { (careQuaMeshP[i].npos - careQuaMeshP[cpvt[0]].npos).norm() ,
								(careQuaMeshP[i].npos - careQuaMeshP[cpvt[1]].npos).norm() ,
								(careQuaMeshP[cpvt[0]].npos - careQuaMeshP[cpvt[1]].npos).norm() };
							//careQuaMeshP[i].pos = careQuaMeshP[cpvt[0]].pos*(1.0 - disvt[0] / disvt[2]) + careQuaMeshP[cpvt[1]].pos*(1.0 - disvt[1] / disvt[2]);
						}
					}
					else if (careQuaMeshP[i].napt == 3) {
						if (cpvtC == 2) {
							Vec3 centerP;
							double cr;
							if (cocircle(careQuaMeshP[cpvt[0]].npos, careQuaMeshP[cpvt[1]].npos, careQuaMeshP[i].npos, centerP, cr)) {
								careQuaMeshP[i].npos = ((careQuaMeshP[cpvt[0]].npos + careQuaMeshP[cpvt[1]].npos) *0.5 - centerP).normalized()*cr + centerP;
							}
							else {
								careQuaMeshP[i].npos = (careQuaMeshP[cpvt[0]].npos + careQuaMeshP[cpvt[1]].npos) *0.5;
							}
						}
						else {
							isQuaSmooth = false;
							qDebug("error!(3854)");
							return -1;
						}
					}
					else {
						isQuaSmooth = false;
						qDebug("error!(3858)");
						return -1;
					}
				}
			}
		}
		smoIterTimes += timesIter;
		//更新四边形质量	
		quaMinAngle = 180.0;
		quaMinYkbA = 1.0;
		quaMinYkbB = 1.0;

		for (int j = 0; j < careQuaIndex.size(); j++) {
			double jabMin = 2.0;
			double jabMax = -1.0;
			double angle[4];
			double jabBMin = 1.0e9;
			double jabBMax = 1.0e-9;
			Vec3 cqp[4];
			for (int i = 0; i < 4; i++) {
				Vec3 op = careQuaMeshP[careQuaIndex[j][i]].npos;
				cqp[i] = op;
				Vec3 dir1 = careQuaMeshP[careQuaIndex[j][(i + 1) % 4]].npos - op;
				Vec3 dir2 = careQuaMeshP[careQuaIndex[j][(i + 3) % 4]].npos - op;
				angle[i] = acos(dir1.normalized()*dir2.normalized()) * 180.0 / Q_PI;
				double temp = dir1.norm()*dir2.norm()* sin(acos(dir1.normalized() *dir2.normalized()));
				if (temp > jabMax) {
					jabMax = temp;
				}
				if (temp < jabMin) {
					jabMin = temp;
				}
				if (dir1.norm() < jabBMin) {
					jabBMin = dir1.norm();
				}
				if (dir2.norm() < jabBMin) {
					jabBMin = dir2.norm();
				}
				if (dir1.norm() > jabBMax) {
					jabBMax = dir1.norm();
				}
				if (dir2.norm() > jabBMax) {
					jabBMax = dir2.norm();
				}
			}
			qsort(angle, 4, sizeof(angle[0]), cmp);
			double ykb_scale = v_quad_scaled_jacobian(cqp);
			careQuaInfo[j].update(ykb_scale, jabBMin / jabBMax, angle);


			if (quaMinAngle > angle[0]) {
				quaMinAngle = angle[0];
			}
			if (quaMinYkbA > ykb_scale) {
				quaMinYkbA = ykb_scale;
			}
			if (quaMinYkbB > jabBMin / jabBMax) {
				quaMinYkbB = jabBMin / jabBMax;

			}
		}
		//end
		isQuaSmooth = true;
	}
	else {
		isQuaSmooth = false;
	}
	return smoIterTimes;
}
void Mesh::resetSmoothMesh() {
	for (int i = 0; i < careQuaMeshP.size(); i++) {
		careQuaMeshP[i].npos = careQuaMeshP[i].pos;
	}
	quaMinAngle = 180.0;
	quaMinYkbA = 1.0;
	quaMinYkbB = 1.0;
	for (int j = 0; j < careQuaIndex.size(); j++) {
		double jabMin = 2.0;
		double jabMax = -1.0;
		double angle[4];
		double jabBMin = 1.0e9;
		double jabBMax = 1.0e-9;
		Vec3 cqp[4];
		for (int i = 0; i < 4; i++) {
			Vec3 op = careQuaMeshP[careQuaIndex[j][i]].pos;
			cqp[i] = op;
			Vec3 dir1 = careQuaMeshP[careQuaIndex[j][(i + 1) % 4]].pos - op;
			Vec3 dir2 = careQuaMeshP[careQuaIndex[j][(i + 3) % 4]].pos - op;
			angle[i] = acos(dir1.normalized()*dir2.normalized()) * 180.0 / Q_PI;
			double temp = dir1.norm()*dir2.norm()* sin(acos(dir1.normalized() *dir2.normalized()));
			if (temp > jabMax) {
				jabMax = temp;
			}
			if (temp < jabMin) {
				jabMin = temp;
			}
			if (dir1.norm() < jabBMin) {
				jabBMin = dir1.norm();
			}
			if (dir2.norm() < jabBMin) {
				jabBMin = dir2.norm();
			}
			if (dir1.norm() > jabBMax) {
				jabBMax = dir1.norm();
			}
			if (dir2.norm() > jabBMax) {
				jabBMax = dir2.norm();
			}
		}
		qsort(angle, 4, sizeof(angle[0]), cmp);

		double ykb_scale = v_quad_scaled_jacobian(cqp);
		careQuaInfo[j].update(ykb_scale, jabBMin / jabBMax, angle);


		if (quaMinAngle > angle[0]) {
			quaMinAngle = angle[0];
		}
		if (quaMinYkbA > ykb_scale) {
			quaMinYkbA = ykb_scale;
		}
		if (quaMinYkbB > jabBMin / jabBMax) {
			quaMinYkbB = jabBMin / jabBMax;
		}
	}
	smoIterTimes = 0;
}
void Mesh::writeTxt(int quaBlcT, QString path) {
	if (careQuaMeshP.size() > 0 && careQuaIndex.size() > 0) {
		FILE* fp = NULL;
		if ((fp = fopen("qua.txt", "w")) == 0) {
			fprintf(stderr, "can't open output file %s\n", "qua.txt");
			return;
		}
		fprintf(fp, "%d\t%d\t%d\t%d\n", (int)careQuaMeshP.size(), (int)careQuaIndex.size(), quaBlcT, 5);
		for (int i = 0; i < careQuaMeshP.size(); i++) {
			fprintf(fp, "%d\t%f\t%f\n", i + 1, careQuaMeshP[i].pos[0], careQuaMeshP[i].pos[1]);
		}
		for (int i = 0; i < careQuaIndex.size(); i++) {
			fprintf(fp, "%d\t%d\t%d\t%d\t%d\n", i + 1, careQuaIndex[i].v[0] + 1, careQuaIndex[i].v[1] + 1, careQuaIndex[i].v[2] + 1, careQuaIndex[i].v[3] + 1);
		}
		int quaBlcC = 1;
		for (int i = 0; i < careQuaIndex.size(); i++) {
			for (int j = 0; j < 4; j++) {
				if (careQuaMeshP[careQuaIndex[i].v[j]].vertType > 0 && careQuaMeshP[careQuaIndex[i].v[(j + 1) % 4]].vertType > 0) {
					/*int slTypeN;
					if (careQuaMeshP[careQuaIndex[i].v[j]].pos.v[0] < -6.999 && careQuaMeshP[careQuaIndex[i].v[(j + 1) % 4]].pos.v[0] < -6.999) {
					slTypeN = 1;
					}
					else if (careQuaMeshP[careQuaIndex[i].v[j]].pos.v[0] < -6.999 && careQuaMeshP[careQuaIndex[i].v[(j + 1) % 4]].pos.v[0] < -6.999) {
					slTypeN = 1;
					}
					else if (careQuaMeshP[careQuaIndex[i].v[j]].pos.v[0] < -6.999 && careQuaMeshP[careQuaIndex[i].v[(j + 1) % 4]].pos.v[0] < -6.999) {
					slTypeN = 1;
					}
					else  if (careQuaMeshP[careQuaIndex[i].v[j]].pos.v[0] < -6.999 && careQuaMeshP[careQuaIndex[i].v[(j + 1) % 4]].pos.v[0] < -6.999) {
					slTypeN = 1;
					}*/
					//fprintf(fp, "%d\t%d\t%d\t\n", quaBlcC, careQuaIndex[i].v[j], careQuaIndex[i].v[(j + 1) % 4], slTypeN);
					quaBlcC++;
				}
			}
		}
		fclose(fp);
		fp = NULL;
	}
}
bool Mesh::writeReduceSLTxt(QString path) {
	if (isGenSSL && dirBVEdgeIndex.size() > 0) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeQuaZx) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream << (int)dirBVEdgeIndex.size() * 2 << " " << (int)dirBVEdgeIndex.size() << "\n";
		stream << "POINTS\n";
		for (int i = 0; i < dirBVEdgeIndex.size(); i++) {
			stream << vert[dirBVEdgeIndex[i][0]].pos[0] << " " << vert[dirBVEdgeIndex[i][0]].pos[1] << "\n";
			stream << vert[dirBVEdgeIndex[i][1]].pos[0] << " " << vert[dirBVEdgeIndex[i][1]].pos[1] << "\n";
		}
		stream << "LINES\n";
		int curPIndex = 1;
		for (int i = 0; i < dirBVEdgeIndex.size(); i++) {
			stream << curPIndex << " " << curPIndex + 1 << "\n";
			curPIndex += 2;
		}
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeQuaZx(QString path) {
	if (careQuaIndex.size() && careQuaMeshP.size() > 0) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeQuaZx) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream << (int)careQuaMeshP.size() << " " << (int)careQuaIndex.size() << "\n";
		stream << "POINTS\n";
		for (int i = 0; i < careQuaMeshP.size(); i++) {
			stream << careQuaMeshP[i].pos[0] << " " << careQuaMeshP[i].pos[1] << "\n";
		}
		stream << "QUA\n";
		for (int i = 0; i < careQuaIndex.size(); i++) {
			stream << careQuaIndex[i].v[0] << " " << careQuaIndex[i].v[1] << " " << careQuaIndex[i].v[2] << " " << careQuaIndex[i].v[3] << "\n";
		}
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeLikeQua(QString path) {
	Vec3 scale = (maxPos - minPos) / 2.0;
	double w = scale[0] > scale[1] ? scale[0] : scale[1];
	if (quaCareSLs.size() > 0 && QuaSLInfo.size() > 0 && quadraInfo.size() > 0) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeLikeQua) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream << (int)quaCareSLs.size() << " " << (int)QuaSLInfo.size() << " " << (int)quadraInfo.size() << "\n";
		for (int i = 0; i < quaCareSLs.size(); i++) {
			stream << quaCareSLs[i].pos[0] << " " << quaCareSLs[i].pos[1] << "\n";
		}
		for (int i = 0; i < QuaSLInfo.size(); i++) {
			stream << QuaSLInfo[i]->auxStartSLPI << " " << QuaSLInfo[i]->auxEndSLPI << "\n";
		}
		for (int i = 0; i < quadraInfo.size(); i++) {
			stream << quadraInfo[i].slI[0] << " " << quadraInfo[i].slI[1] << " " << quadraInfo[i].slI[2] << " " << quadraInfo[i].slI[3] << "\n";
		}
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
void Mesh::writeVTK(QString path) {
	if (vert.size() > 0 && face.size() > 0 && isGenRegDivToS) {
		FILE* fp = NULL;
		if ((fp = fopen("demo.vtk", "w")) == 0) {
			fprintf(stderr, "can't open output file %s\n", "qua.txt");
			return;
		}
		fprintf(fp, "# vtk DataFile Version 2.0\n");
		fprintf(fp, "Differential method to modify the direction field\n");
		fprintf(fp, "ASCII\n");
		fprintf(fp, "DATASET UNSTRUCTURED_GRID\n");
		fprintf(fp, "POINTS %d double\n", (int)vert.size());
		for (int i = 0; i < (int)vert.size(); i++) {
			fprintf(fp, "%f %f %.1f\n", vert[i].pos[0], vert[i].pos[1], vert[i].pos[2]);
		}
		fprintf(fp, "CELLS %d %d\n", (int)face.size(), (int)face.size() * 4);
		for (int i = 0; i < (int)face.size(); i++) {
			fprintf(fp, "3 %d %d %d\n", face[i].index[0], face[i].index[1], face[i].index[2]);
		}
		fprintf(fp, "CELL_TYPES %d\n", (int)face.size());
		for (int i = 0; i < (int)face.size(); i++) {
			fprintf(fp, "5\n");
		}
		fprintf(fp, "POINT_DATA %d\n", (int)vert.size());
		fprintf(fp, "SCALARS grad float\n");
		fprintf(fp, "LOOKUP_TABLE default\n");
		for (int i = 0; i < (int)vert.size(); i++) {
			fprintf(fp, "%f\n", halfVerts[i]->vec.angleP());
			//fprintf(fp, "0.1\n");
		}
		fprintf(fp, "NORMALS node_normals float\n");
		for (int i = 0; i < (int)vert.size(); i++) {
			Vec3 temp = halfVerts[i]->vec.normalized();
			fprintf(fp, "%f %f %f\n", temp[0], temp[1], temp[2]);
		}
		fclose(fp);
		fp = NULL;
	}
}
bool Mesh::writeVTKQJOC(QString path) {
	if (careQuaMeshP.size() > 0 && careQuaIndex.size() > 0 && careQuaInfo.size() > 0) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeLikeQua) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream << "# vtk DataFile Version 2.0" << endl;
		stream << "Quadrilateral quality inspection" << endl;
		stream << "ASCII" << endl;
		stream << "DATASET UNSTRUCTURED_GRID" << endl;
		stream << "POINTS " << careQuaMeshP.size() << " double" << endl;
		for (int i = 0; i < careQuaMeshP.size(); i++) {
			stream << careQuaMeshP[i].npos[0] << " " << careQuaMeshP[i].npos[1] << " 0.0" << endl;
		}
		stream << "CELLS " << careQuaIndex.size() << " " << careQuaIndex.size() * 5 << endl;
		for (int i = 0; i < careQuaIndex.size(); i++) {
			stream << "4 " << careQuaIndex[i][0] << " " << careQuaIndex[i][1] << " " << careQuaIndex[i][2] << " " << careQuaIndex[i][3] << endl;
		}
		stream << "CELL_TYPES " << careQuaIndex.size() << endl;
		for (int i = 0; i < careQuaIndex.size(); i++) {
			stream << "7" << endl;
		}
		stream << "CELL_DATA " << careQuaIndex.size() << endl;
		stream << "SCALARS ykb double 1" << endl;
		stream << "LOOKUP_TABLE default" << endl;
		for (int i = 0; i < careQuaInfo.size(); i++) {
			stream << careQuaInfo[i].ykbB << endl;
		}
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeVTKQMINA(QString path) {
	if (careQuaMeshP.size() > 0 && careQuaIndex.size() > 0 && careQuaInfo.size() > 0) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeLikeQua) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream << "# vtk DataFile Version 2.0" << endl;
		stream << "Quadrilateral quality inspection" << endl;
		stream << "ASCII" << endl;
		stream << "DATASET UNSTRUCTURED_GRID" << endl;
		stream << "POINTS " << careQuaMeshP.size() << " double" << endl;
		for (int i = 0; i < careQuaMeshP.size(); i++) {
			stream << careQuaMeshP[i].npos[0] << " " << careQuaMeshP[i].npos[1] << " 0.0" << endl;
		}
		stream << "CELLS " << careQuaIndex.size() << " " << careQuaIndex.size() * 5 << endl;
		for (int i = 0; i < careQuaIndex.size(); i++) {
			stream << "4 " << careQuaIndex[i][0] << " " << careQuaIndex[i][1] << " " << careQuaIndex[i][2] << " " << careQuaIndex[i][3] << endl;
		}
		stream << "CELL_TYPES " << careQuaIndex.size() << endl;
		for (int i = 0; i < careQuaIndex.size(); i++) {
			stream << "7" << endl;
		}
		stream << "CELL_DATA " << careQuaIndex.size() << endl;
		stream << "SCALARS min-angle double 1" << endl;
		stream << "LOOKUP_TABLE default" << endl;
		for (int i = 0; i < careQuaInfo.size(); i++) {
			stream << careQuaInfo[i].angle[0] << endl;
		}
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeVTKB(QString path) {
	if (QuaSLInfo.size() > 0 && quaCareSLs.size() > 0) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeLikeQua) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		int pointsCount = 0;
		int cellCount = 0;
		std::vector<int> borderIndex;
		std::map<int, int> singIndex;
		for (int i = 0; i < QuaSLInfo.size(); i++) {
			if (QuaSLInfo[i]->isBorder) {
				borderIndex.push_back(i);
				singIndex[i] = ++pointsCount;
				pointsCount += QuaSLInfo[i]->aslpc - 1;
			}
		}
		stream << "# vtk DataFile Version 2.0" << endl;
		stream << "model boundary" << endl;
		stream << "ASCII" << endl;
		stream << "DATASET UNSTRUCTURED_GRID" << endl;
		stream << "POINTS " << pointsCount << " double" << endl;
		for (int i = 0; i < borderIndex.size(); i++) {
			for (int j = QuaSLInfo[borderIndex[i]]->auxStartSLPI; j <= QuaSLInfo[borderIndex[i]]->auxEndSLPI; j++) {
				stream << quaCareSLs[j].pos[0] << " " << quaCareSLs[j].pos[1] << " 0.0" << endl;
			}
		}
		for (int i = 0; i < borderIndex.size(); i++) {
			int startPI = singIndex[borderIndex[i]];
			int endPI = QuaSLInfo[borderIndex[i]]->auxEndSLPI - QuaSLInfo[borderIndex[i]]->auxStartSLPI + singIndex[borderIndex[i]];
			for (int j = startPI - 1; j < endPI - 1; j++) {
				cellCount++;
			}
		}
		stream << "CELLS " << cellCount << " " << cellCount * 3 << endl;
		for (int i = 0; i < borderIndex.size(); i++) {
			int startPI = singIndex[borderIndex[i]];
			int endPI = QuaSLInfo[borderIndex[i]]->auxEndSLPI - QuaSLInfo[borderIndex[i]]->auxStartSLPI + singIndex[borderIndex[i]];
			for (int j = startPI - 1; j < endPI - 1; j++) {
				stream << "2 " << j << " " << j + 1 << endl;
			}
		}
		stream << "CELL_TYPES " << cellCount << endl;
		for (int i = 0; i < borderIndex.size(); i++) {
			int startPI = singIndex[borderIndex[i]];
			int endPI = QuaSLInfo[borderIndex[i]]->auxEndSLPI - QuaSLInfo[borderIndex[i]]->auxStartSLPI + singIndex[borderIndex[i]];
			for (int j = startPI - 1; j < endPI - 1; j++) {
				stream << "3" << endl;
			}
		}
		fp.close();
		return true;
	}
	return false;
}
bool Mesh::writeSSLVTK(QString path) {
	if (SLSimplify.size() > 0 && showSSLS.size() > 0) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeSSLVTK) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream << "# vtk DataFile Version 2.0" << endl;
		stream << "model boundary" << endl;
		stream << "ASCII" << endl;
		stream << "DATASET UNSTRUCTURED_GRID" << endl;
		stream << "POINTS " << showSSLS.size() << " double" << endl;
		for (int i = 0; i < showSSLS.size(); i++) {
			stream << showSSLS[i].pos[0] << " " << showSSLS[i].pos[1] << " 0.0" << endl;
		}
		int cellCount = 0;
		for (int i = 0; i < SLSimplify.size(); i++) {
			cellCount += SLSimplify[i]->slpc - 1;
		}
		stream << "CELLS " << cellCount << " " << cellCount * 3 << endl;
		for (int i = 0; i < SLSimplify.size(); i++) {
			for (int j = SLSimplify[i]->startSLPointIndex; j < SLSimplify[i]->endSLPointIndex; j++) {
				stream << "2 " << j << " " << j + 1 << endl;
			}
		}
		stream << "CELL_TYPES " << cellCount << endl;
		for (int i = 0; i < SLSimplify.size(); i++) {
			for (int j = SLSimplify[i]->startSLPointIndex; j < SLSimplify[i]->endSLPointIndex; j++) {
				stream << "3 " << endl;
			}
		}
		fp.close();
		return true;
	}
	return false;
}
bool Mesh::writeSegBlcn(QString path) {
	if (isGenRegDivToS) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(segB.lcn) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		std::vector<int> isShowB;
		int npc = 0;
		for (int i = 0; i < vert.size(); i++) {
			if (halfVerts[i]->isBoundary) {
				isShowB.push_back(halfVerts[i]->edgeI);
				npc++;
			}
			else {
				isShowB.push_back(-1);
			}
		}
		stream << npc << endl;
		std::vector<int> bIndexs;
		for (int i = 0; i < vert.size(); i++) {
			int findPN = 0;
			if (isShowB[i] != -1) {
				int startP = i;
				isShowB[startP] = -1;
				do {
					startP = halfEdges[halfVerts[startP]->edgeI]->vertI;
					stream << halfVerts[startP]->pos[0] << " " << halfVerts[startP]->pos[1] << endl;
					isShowB[startP] = -1;
					findPN++;

				} while (startP != i);
				bIndexs.push_back(findPN);
			}
		}
		int curI = 0;
		for (int i = 0; i < bIndexs.size(); i++) {
			stream << "LINE" << endl;
			int ccurI = curI;
			for (int j = 0; j < bIndexs[i] - 1; j++, curI++) {
				stream << curI << " " << curI + 1 << endl;
			}
			stream << curI++ << " " << ccurI << endl;
		}
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeGM2(QString path) {
	if (isGenRegDivToS) {
		std::map<int, int> bsI;
		int bsn, bts;
		genBoundaryType(bsI, bsn, bts);
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(gm2) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream << bsn << "\n";
		int cbsn = 1;
		for (auto iter = bsI.begin(); iter != bsI.end(); ++iter) {
			int curVI = (*iter).first;
			stream << cbsn++ << " " << vert[curVI].pos[0] << " " << vert[curVI].pos[1] << "\n";
			while (true)
			{
				int nextVI = halfEdges[halfVerts[curVI]->edgeI]->vertI;
				if (nextVI == (*iter).first) {
					break;
				}
				else {
					stream << cbsn++ << " " << vert[nextVI].pos[0] << " " << vert[nextVI].pos[1] << "\n";
					curVI = nextVI;
				}
			}
		}

		stream << bts * 2 << "\n";

		for (auto iter = bsI.begin(); iter != bsI.end(); ++iter) {
			stream << (*iter).second + 1 << "\n";
			int cbsn1 = 1;
			int curVI = (*iter).first;
			stream << cbsn1++ << " " << vert[curVI].pos[0] << " " << vert[curVI].pos[1] << " " << halfVerts[curVI]->bType << "\n";
			bool isns = false;
			while (true)
			{
				int nextVI = halfEdges[halfVerts[curVI]->edgeI]->vertI;
				stream << cbsn1++ << " " << vert[nextVI].pos[0] << " " << vert[nextVI].pos[1] << " " << halfVerts[nextVI]->bType << "\n";
				/*if (!isns && cbsn1 == (*iter).second / 2 + 1) {
					isns = true;
					cbsn1 = 1;
					stream << (*iter).second - (*iter).second / 2 + 2 << "\n";
					stream << cbsn1++ << " " << vert[nextVI].pos[0] << " " << vert[nextVI].pos[1] << " " << halfVerts[nextVI]->bType << "\n";
				}*/
				if (nextVI == (*iter).first) {
					break;
				}
				else {
					curVI = nextVI;
				}
			}
			//stream << cbsn1 << " " << vert[(*iter).first].pos[0] << " " << vert[(*iter).first].pos[1] << " " << halfVerts[(*iter).first]->bType << "\n";
		}
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeGD2(QString path) {
	if (isGenQuaMesh) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(gm2) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream.setRealNumberNotation(QTextStream::ScientificNotation);
		stream.setFieldAlignment(QTextStream::AlignRight);
		stream << qSetFieldWidth(6);
		stream << careQuaIndex.size() << " " << careQuaMeshP.size() << endl;
		//stream << qSetFieldWidth(15);
		for (int i = 0; i < careQuaMeshP.size(); i++) {
			stream << qSetFieldWidth(6) << i + 1 << " " << qSetFieldWidth(15) << careQuaMeshP[i].pos[0] << " " << careQuaMeshP[i].pos[1] << endl;
		}
		stream << qSetFieldWidth(6);
		for (int i = 0; i < careQuaIndex.size(); i++) {
			stream << i + 1 << " " << "4" << " " << careQuaIndex[i][0] << " " << careQuaIndex[i][1] << " " << careQuaIndex[i][2] << " " << careQuaIndex[i][3] << " 0 0" << endl;
		}
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeToCGNS(QString path) {
	if (isGenQuaMesh) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(ToCGNS) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream.setRealNumberNotation(QTextStream::ScientificNotation);
		stream << careQuaIndex.size() << " " << careQuaMeshP.size() << endl;
		//stream << qSetFieldWidth(15);
		stream << "p " << careQuaMeshP.size() << endl;
		for (int i = 0; i < careQuaMeshP.size(); i++) {
			stream << i + 1 << " " << careQuaMeshP[i].npos[0] << " " << careQuaMeshP[i].npos[1] << endl;
		}
		stream << "c " << careQuaIndex.size() << endl;
		for (int i = 0; i < careQuaIndex.size(); i++) {
			stream << i + 1 << " " << careQuaIndex[i][0] << " " << careQuaIndex[i][1] << " " << careQuaIndex[i][2] << " " << careQuaIndex[i][3] << endl;
		}
		stream << "i " << careQuaBorderInlet.size() << endl;
		for (int i = 0; i < careQuaBorderInlet.size(); i++) {
			stream << i + 1 << " " << careQuaBorderInlet[i][0] << " " << careQuaBorderInlet[i][1] << endl;
		}
		stream << "o " << careQuaBorderOutlet.size() << endl;
		for (int i = 0; i < careQuaBorderOutlet.size(); i++) {
			stream << i + 1 << " " << careQuaBorderOutlet[i][0] << " " << careQuaBorderOutlet[i][1] << endl;
		}
		stream << "w " << careQuaBorderWall0.size() << endl;
		for (int i = 0; i < careQuaBorderWall0.size(); i++) {
			stream << i + 1 << " " << careQuaBorderWall0[i][0] << " " << careQuaBorderWall0[i][1] << endl;
		}
		stream << "m " << careQuaBorderWall1.size() << endl;
		for (int i = 0; i < careQuaBorderWall1.size(); i++) {
			stream << i + 1 << " " << careQuaBorderWall1[i][0] << " " << careQuaBorderWall1[i][1] << endl;
		}
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeToExcel(QString path) {
	if (careQuaInfo.size() > 0) {
		int allAngle[30];
		int minAngle[15];
		int tykb[20];
		for (int i = 0; i < 30; i++) {
			allAngle[i] = 0;
		}
		for (int i = 0; i < 15; i++) {
			minAngle[i] = 0;
		}
		for (int i = 0; i < 20; i++) {
			tykb[i] = 0;
		}
		double minAngles = 180.0;
		double minJac = 1.0;
		for (int i = 0; i < careQuaInfo.size(); i++) {
			minAngle[int(careQuaInfo[i].angle[0] / 6.0)] ++;
			if (careQuaInfo[i].angle[0] < minAngles) {
				minAngles = careQuaInfo[i].angle[0];
			}
			for (int j = 0; j < 4; j++) {
				allAngle[int(careQuaInfo[i].angle[j] / 6.0)]++;
			}
			tykb[int(careQuaInfo[i].ykbA / 0.05)]++;
			if (careQuaInfo[i].ykbA < minJac) {
				minJac = careQuaInfo[i].ykbA;
			}
		}
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(ToCGNS) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream.setRealNumberNotation(QTextStream::ScientificNotation);
		stream << "all angles:" << endl;
		for (int i = 0; i < 30; i++) {
			stream << allAngle[i] << endl;
		}

		stream << "min angle:" << endl;
		for (int i = 0; i < 15; i++) {
			stream << minAngle[i] << endl;
		}
		stream << "Jacobian:" << endl;
		for (int i = 0; i < 20; i++) {
			stream << tykb[i] << endl;
		}
		stream << "min angle value:" << minAngles << endl;
		stream << "min Jac value:" << minJac << endl;
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeToPAT3(QString path) {
	if (dfm != nullptr) {
		QFile fp(path);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeTo3D) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		stream << "# PAT File Generated by GLArea" << endl;
		stream << "# Verts:" << vert.size() << endl;
		stream << "# Edges:" << face.size() << endl;
		for (int i = 0; i < vert.size(); i++) {
			stream << "v " << vert[i].pos[0] << " " << vert[i].pos[1] << " " << vert[i].disValue << endl;
		}
		for (int i = 0; i < face.size(); i++) {
			stream << "f " << face[i].index[0] << " " << face[i].index[1] << " " << face[i].index[2] << endl;
		}
		fp.close();
		return true;
	}
	else {
		return false;
	}
}
bool Mesh::writeToCFL(QStringList path) {
	if (sf.size() != 0) {
		QDir dir;

		if(!dir.exists(path[0]))
			dir.mkdir(path[0]);

		QFile fp(path[0]+"/"+path[1]);
		if (!fp.open(QIODevice::WriteOnly | QIODevice::Text)) {
			QMessageBox::warning(NULL, "warning", "can't open output(writeToCFL) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
			return false;
		}
		QTextStream stream(&fp);
		int pc = vert.size();
		for (int i = 0; i < pc; ++i) {
			/*if (halfVerts[i]->vType != 0) {
				halfVerts[i]->vType = 1;
			}*/
			if (halfVerts[i]->isBoundary) {
				if (halfVerts[i]->vType != 0) {
					halfVerts[i]->vType = 11;
				}
				else
					halfVerts[i]->vType = 1;
			}
		}
		Vec3 tv;
		double max_fabs = 0.0;
		for (int i = 0; i < pc; ++i) {
			if (max_fabs < fabs(vert[i].pos[0])) {
				max_fabs = fabs(vert[i].pos[0]);
			}
			if (max_fabs < fabs(vert[i].pos[1])) {
				max_fabs = fabs(vert[i].pos[1]);
			}
		}
		double rate = 0.999999 / max_fabs;
		double out_x = 0.0;
		double out_y = 0.0;
		// double out_z = 0.0;
		for (int i = 0; i < pc; ++i) {
			tv = angleToNorm(halfVerts[i]->vec.angleP() / 4.)*halfVerts[i]->vec.norm();
			double norm = tv.norm();
			if (fabs(norm - 0.1) < 0.0000009)
				norm = 0.100000;
			norm *= 10;
			out_x= vert[i].pos[0] * rate;
			
			out_y = vert[i].pos[1] * rate;
				// out_z = vert[i].pos[2] * rate;
			
			stream << qSetRealNumberPrecision(6) << fixed<< out_x << "  " << out_y << "  " << vert[i].pos[2] <<"  " << norm<< "  " << halfVerts[i]->vec[0]*10 << "  " << halfVerts[i]->vec[1] * 10 << "  " << halfVerts[i]->vType << endl;
		}
		fp.close();
		qDebug()<< path[0] + "/" + path[1]<<endl;
		return true;
	}
	else {
		return false;
	}
}
void Mesh::showNormClass()
{
	int pc = vert.size();
	int a[10] = { 0 };
	for (int i = 0; i < pc; ++i) {
		Vec3 tv = angleToNorm(halfVerts[i]->vec.angleP() / 4.)*halfVerts[i]->vec.norm();
		double norm = tv.norm();
		if (fabs(norm - 0.1) < 0.0000009)
			norm = 0.100000;
		norm *= 10;
		if (norm >= 0 && norm <= 0.1) {
			a[0]++;
		}
		else if (norm > 0.1 && norm <= 0.2) {
			a[1]++;
		}
		else if (norm > 0.2 && norm <= 0.3) {
			a[2]++;
		}
		else if (norm > 0.3 && norm <= 0.4) {
			a[3]++;
		}
		else if (norm > 0.4 && norm <= 0.5) {
			a[4]++;
		}
		else if (norm > 0.5 && norm <= 0.6) {
			a[5]++;
		}
		else if (norm > 0.6 && norm <= 0.7) {
			a[6]++;
		}
		else if (norm > 0.7 && norm <= 0.8) {
			a[7]++;
		}
		else if (norm > 0.8 && norm <= 0.9) {
			a[8]++;
		}
		else if (norm > 0.9 && norm <= 1.0) {
			a[9]++;
		}
	}
	qDebug() << "0-0.1:" << a[0] << "0.1-0.2:" << a[1] << "0.2-0.3:" << a[2] << "0.3-0.4:" << a[3] << "0.4-0.5:" << a[4]
		<< "0.5-0.6:"<<a[5] << "0.6-0.7:"<<a[6]<<"0.7-0.8:"<<a[7] << "0.8-0.9:"<<a[8] << "0.9-1.0:"<<a[9];
}
bool Mesh::highLightCrossField(QString filePath)
{
	QFile fp(filePath);
	if (!fp.open(QIODevice::ReadOnly | QIODevice::Text)) {
		//QMessageBox::warning(NULL, "warning", "can't open output(writeToCFL) file!", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
		return false;
	}
	QTextStream in(&fp);
	QString line = in.readLine();
	int i = 0;
	while (!line.isNull()) {
		if (i < vert.size()) {
			if (line.toInt() != 0 && line.toInt() != 1) {
				if (halfVerts[i] != nullptr) {
					PCPoint temp;
					temp.pos = halfVerts[i]->pos;
					temp.color = Vec3(1.0, 0.0, 0.0);
					highLightedCrossField.push_back(temp);
				}
			}
			line = in.readLine();
		}
		i++;
	}
	return true;
}
void Mesh::setLLSSpec(double spec) {
	if (spec > 0.0) {
		llsValue = spec;
	}
}
double Mesh::getLLSSpec() {
	return llsValue;
}
bool Mesh::setCroSize(int cln, int clp, double cbs) {
	if (cln == 0 || fabs(cbs) < interDim || clp == 0 || (cln - this->croLayNum == 0 && clp - this->crolpD == 0 && fabs(cbs - this->croBSize) < interDim)) {
		return false;
	}
	else {
		this->croLayNum = cln;
		this->croBSize = cbs;
		this->crolpD = clp;
		return true;
	}
}
bool Mesh::setDirSize(int dln, int dlp, double dbs, double dhs) {
	if (dln == 0 || dlp == 0 || fabs(dbs) < interDim || fabs(dhs) < interDim || (dln - this->dirLayNum == 0 && dlp - this->dirlpD == 0 && fabs(dbs - this->dirBSize) < interDim && fabs(dhs - this->dirHSize) < interDim)) {
		return false;
	}
	else {
		this->dirLayNum = dln;
		this->dirBSize = dbs;
		this->dirHSize = dhs;
		this->dirlpD = dlp;
		return true;
	}
}
bool Mesh::genBoundaryType(std::map<int, int>& bsI, int& bsn, int& bts) {
	if (isGenRegDivToS) {
		int curBtype = 0;
		bsI.clear();
		bsn = 1;
		for (int i = 0; i < vert.size(); i++) {
			if (halfVerts[i]->isBoundary && halfVerts[i]->bType == -1) {
				int curBsn = bsn;
				halfVerts[i]->bType = bsn++;
				int curVI = i;
				while (true)
				{
					int nextVI = halfEdges[halfVerts[curVI]->edgeI]->vertI;
					if (nextVI == i) {
						break;
					}
					else {
						halfVerts[nextVI]->bType = bsn++;
						curVI = nextVI;
					}
				}
				curBtype++;
				bsI.insert(std::pair<int, int>(i, bsn - curBsn));
			}
		}
		bsn--;
		bts = curBtype;
		return true;
	}
	else {
		return false;
	}
}
