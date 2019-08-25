#include"disMesh.h"
#include<qfile.h>
#include <QTextStream>
void DisMesh::GenDisF() {
	int col = (this->maxP[0] - this->minP[0]) / this->quaSize + 3;
	int row = (this->maxP[1] - this->minP[1]) / this->quaSize + 3;
	if (col < 4 || row < 4 || col*row>2e6) {
		return;
	}
	if (dfm != nullptr) {
		delete dfm;
	}
	dfm = new DisFieldMesh(minP, maxP, quaSize);
	if (vertP.size() > 0) {
		borderI.clear();
		dfm->GetDomainValue(vertP, borderSize);
		if (dfm->isGenDFM) {
			for (int i = 0; i < borderSize - 1; i++) {
				dis.push_back(0.0);
				borderI.push_back(LineIndex(i, i + 1));
			}
			dis.push_back(0.0);
			borderI.push_back(LineIndex(borderSize - 1, 0));
			for (int i = borderSize; i < vertP.size(); i++) {
				double disf = dfm->GetInfoVToW(vertP[i], TypeField::Distance);
				maxDis = disf > maxDis ? disf : maxDis;
				dis.push_back(disf);
			}
			for (int i = 0; i < vertP.size(); i++) {
				double colorValue = dis[i] / maxDis;
				color.push_back(Vec3(colorValue));
				vI.push_back(i);
			}
			QFile file("distance.txt");
			if (file.open(QIODevice::WriteOnly)) //打开方式：可读、二进制方式
			{
				QTextStream out(&file);
				for (int i = 0; i < dis.size(); i++)
				{
					out << vertP[i][0] << " " << vertP[i][1] << " " << dis[i] << endl;
					//按二进制写入时，须进行类型转换，这也是QFile使用上比较麻烦的地方
					//file.write(QString("%1\n").arg(dis[i]).toStdString().c_str()); //参数为char*，须转换
					//file.write(reinterpret_cast<char*>(&value), sizeof(value));
				}
			}
			file.close();
		}
	}
	isGenDisF = true;
}
DisMesh::DisMesh()
{
	this->maxP = Vec3(-1.0e5, -1.0e5, 0.0);
	this->minP = Vec3(1.0e5, 1.0e5, 0.0);
	this->quaSize = 0.0;
	this->borderSize = 0;
	this->isLoad = false;
	this->isGenDisF = false;
	dfm = nullptr;
}
bool DisMesh::loadTXT(const char*fn) {
	vertP.clear();
	color.clear();
	dis.clear();
	vI.clear();
	borderSize = 0;
	quaSize = 0.0;
	QFile f(fn);
	if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) { return false; }
	char st[512];
	bool isB = true;
	while (f.readLine(st, 511) != -1) {
		QString s = QString("%1").arg(st);
		s = s.trimmed();
		if (s.startsWith("border")) {
			isB = true;
		}
		else if (s.startsWith("Interior")) {
			isB = false;
		}
		else {
			char cp[512];
			sprintf(cp, "%s", s.toStdString().data());
			Vec3 vp;
			vp.v[2] = 0.0;
			sscanf(cp, "%lf %lf", &vp.v[0], &vp.v[1]);
			vertP.push_back(vp);
			if (isB) borderSize++;
			if (maxP[1] < vp.v[1]) {
				maxP[1] = vp.v[1];
			}
			if (maxP[0] < vp.v[0]) {
				maxP[0] = vp.v[0];
			}
			if (minP[1] > vp.v[1]) {
				minP[1] = vp.v[1];
			}
			if (minP[0] > vp.v[0]) {
				minP[0] = vp.v[0];
			}
		}
	}
	if (borderSize > 2) {
		int i = 0;
		if (i < borderSize - 1) {
			quaSize = (vertP[i] - vertP[i + 1]).norm() / 5.0;
			isLoad = true;
			return true;
		}
	}
	return false;
}
DisMesh::~DisMesh()
{
}
