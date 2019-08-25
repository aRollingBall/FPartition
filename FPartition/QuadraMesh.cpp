#include"QuadraMesh.h"
#include<qfile.h>
#include<qmessagebox.h>
#include<qdebug.h>
void QuadraMesh::genQuadraMesh() {
	int col = (this->maxPoint[0] - this->minPoint[0]) / this->specifcQua + 3;
	int row = (this->maxPoint[1] - this->minPoint[1]) / this->specifcQua + 3;
	if (col < 4 || row < 4 || col*row>2e6) {
		isGenQuaMesh = false;
		return;
	}
	this->minPoint[0] -= 1.0* this->specifcQua;
	this->minPoint[1] -= 1.0* this->specifcQua;
	this->col = col;
	this->row = row;
	this->qvert.clear();
	this->qface.clear();
	this->qvertInto.clear();
	this->qvert.reserve(col*row);
	this->qface.reserve((col - 1)*(row - 1));
	this->qvertInto.reserve(col*row);
	int vertCount = 0;
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			QuaV vq;
			vq.pos = Vec3(this->minPoint[0] + j*this->specifcQua, this->minPoint[1] + i*this->specifcQua, 0.0);
			vq.color = Vec3(1.0, 0.0, 0.0);
			qvert.push_back(vq);
			QuaVInto qv;
			qv.dirI[0] = i == row - 1 ? -1 : vertCount + col;
			qv.dirI[1] = j == 0 ? -1 : vertCount - 1;
			qv.dirI[2] = i == 0 ? -1 : vertCount - col;
			qv.dirI[3] = j == col - 1 ? -1 : vertCount + 1;
			vertCount++;
			if (i == 0 || j == 0 || j == col - 1 || i == row - 1) {
				qv.value = 0.0;
			}
			else {
				qv.value = 1e5;   //给定大值
			}
			qv.isBoundary = false;
			qvertInto.push_back(qv);
			if (i > 0 && j > 0) {
				QuaF qf;
				qf.cornerPI[0] = vertCount - 2;
				qf.cornerPI[1] = vertCount - 2 - col;
				qf.cornerPI[2] = vertCount - 1 - col;
				qf.cornerPI[3] = vertCount - 1;		//前面vertNum已减一
				qface.push_back(qf);
			}
		}
	}
	qft.resize(qface.size(), false);
	bqft.resize(qface.size(), false);
	isGenQuaMesh = true;
}
void QuadraMesh::getQuaEAloaF(LineVs vs, std::set<int> &faceI) {
	getQuaVAloaF(vs[0], faceI);
	getQuaVAloaF(vs[1], faceI);
	if (faceI.size() < 2) {
		return;
	}
	auto iterFaceI1 = faceI.begin();
	auto iterFaceI2 = faceI.end();
	int maxRowC = -1;
	int maxColC = -1;
	int minRowC = this->row;
	int minColC = this->col;
	for (; iterFaceI1 != iterFaceI2; iterFaceI1++) {
		int arc = (*iterFaceI1) / (this->col - 1);
		if (arc > maxRowC) {
			maxRowC = arc;
		}
		if (arc < minRowC) {
			minRowC = arc;
		}
		int acc = (*iterFaceI1) % (this->col - 1);
		if (acc > maxColC) {
			maxColC = acc;
		}
		if (acc < minColC) {
			minColC = acc;
		}
	}
	for (int i = minRowC; i <= maxRowC; i++) {
		for (int j = minColC; j <= maxColC; j++) {
			int fI = i*(col - 1) + j;
			qft[fI] = true;
			bqft[fI] = true;
			faceI.insert(fI);
		}
	}
}
//得到指定点的一层领域面
void QuadraMesh::getQuaVAloaF(Vec3 targetP, std::set<int> &faceI) {
	int x = int((targetP[0] - this->minPoint[0]) / this->specifcQua);
	int y = int((targetP[1] - this->minPoint[1]) / this->specifcQua);
	int tarFI = x + y*(this->col - 1);
	if (x >= 0 && x < this->col && y >= 0 && y < this->row && tarFI >= 0 && tarFI < (col - 1)*(row - 1)) {
		faceI.insert(tarFI);
		if ((qvert[qface[tarFI].cornerPI[0]].pos - targetP).norm() < 1e-4) {
			faceI.insert(tarFI - 1);
			faceI.insert(tarFI + this->col - 1);
			faceI.insert(tarFI + this->col - 2);
		}
		else if ((qvert[qface[tarFI].cornerPI[1]].pos - targetP).norm() < 1e-4) {
			faceI.insert(tarFI - 1);
			faceI.insert(tarFI - this->col);
			faceI.insert(tarFI - this->col + 1);
		}
		else if ((qvert[qface[tarFI].cornerPI[2]].pos - targetP).norm() < 1e-4) {
			faceI.insert(tarFI + 1);
			faceI.insert(tarFI - this->col + 1);
			faceI.insert(tarFI - this->col + 2);
		}
		else if ((qvert[qface[tarFI].cornerPI[3]].pos - targetP).norm() < 1e-4) {
			faceI.insert(tarFI + 1);
			faceI.insert(tarFI + this->col - 1);
			faceI.insert(tarFI + this->col);
		}
		else if (abs((targetP[0] - this->minPoint[0]) - this->specifcQua*double(x)) < 1e-4) {
			int traFIT1 = int((targetP[0] - this->minPoint[0] + this->specifcQua / 2.0) / this->specifcQua) + y*(this->col - 1);;
			int traFIT2 = int((targetP[0] - this->minPoint[0] - this->specifcQua / 2.0) / this->specifcQua) + y*(this->col - 1);;
			faceI.insert(traFIT1);
			faceI.insert(traFIT2);
		}
		else if (abs((targetP[1] - this->minPoint[1]) - this->specifcQua*double(y)) < 1e-4) {
			int traFIT1 = x + (this->col - 1)* int((targetP[1] - this->minPoint[1] + this->specifcQua / 2.0) / this->specifcQua);
			int traFIT2 = x + (this->col - 1)* int((targetP[1] - this->minPoint[1] - this->specifcQua / 2.0) / this->specifcQua);
			faceI.insert(traFIT1);
			faceI.insert(traFIT2);
		}
	}
}
void QuadraMesh::GetInfoFace(Vec3 targetP) {
	if (!isGenDomainValue) { return; }
	int x = int((targetP[0] - this->minPoint[0]) / this->specifcQua);
	int y = int((targetP[1] - this->minPoint[1]) / this->specifcQua);
	int tarFI = x + y*(this->col - 1);
	if (tarFI > qface.size() - 1) {
		return;
	}
	if (qft[tarFI]) {
		return;
	}
	qft[tarFI] = true;

	std::vector<bool> mark;
	mark.resize(qft.size(), false);
	std::set<int> sfI;
	sfI.insert(tarFI);
	int cfI, tcol, trow;
	while (sfI.size() != 0) {
		for (auto iter = sfI.begin(); iter != sfI.end();) {
			qft[*iter] = true;
			mark[*iter] = true;
			trow = *iter / (col - 1);
			tcol = *iter % (col - 1);
			if (tcol - 1 >= 0 && !bqft[*iter - 1] && !mark[*iter - 1]) {
				sfI.insert(*iter - 1);
			}
			if (tcol + 1 < col - 1 && !bqft[*iter + 1] && !mark[*iter + 1]) {
				sfI.insert(*iter + 1);
			}
			if (trow - 1 >= 0 && !bqft[(trow - 1)*(col - 1) + tcol] && !mark[(trow - 1)*(col - 1) + tcol]) {
				sfI.insert((trow - 1)*(col - 1) + tcol);
			}
			if (trow + 1 < row - 1 && !bqft[(trow + 1)*(col - 1) + tcol] && !mark[(trow + 1)*(col - 1) + tcol]) {
				sfI.insert((trow + 1)*(col - 1) + tcol);
			}
			iter = sfI.erase(iter);
		}
	}
}
//得到指定点的场域信息
double QuadraMesh::GetInfoVToW(Vec3 targetP, TypeField type) {
	if (!isGenDomainValue) { return -1.0; }
	int x = int((targetP[0] - this->minPoint[0]) / this->specifcQua);
	int y = int((targetP[1] - this->minPoint[1]) / this->specifcQua);
	int tarFI = x + y*(this->col - 1);
	if (tarFI > qface.size() - 1) {
		return 0.0;//根据需求不同而不同，默认为0.0
	}
	double leftDisValue = abs(targetP[0] - this->minPoint[0] - x*this->specifcQua);
	double rightDisValue = abs((x + 1)*this->specifcQua + this->minPoint[0] - targetP[0]);
	double upDisValue = abs((y + 1)*this->specifcQua + this->minPoint[1] - targetP[1]);
	double dowmDisValue = abs(targetP[1] - this->minPoint[1] - y*this->specifcQua);
	//if (leftDisValue < this->specifcQua && rightDisValue < this->specifcQua && upDisValue < this->specifcQua && dowmDisValue < this->specifcQua) {

	/*if (fabs(temp / (4.0*this->specifcQua)) > 1.0) {
		qDebug("error!");
	}*/
	if (type == TypeField::Thermal) {
		Vec3 temp = angleToNorm(qvertInto[qface[tarFI].cornerPI[0]].value)*(rightDisValue + dowmDisValue) +
			angleToNorm(qvertInto[qface[tarFI].cornerPI[1]].value)*(rightDisValue + upDisValue) +
			angleToNorm(qvertInto[qface[tarFI].cornerPI[2]].value)*(leftDisValue + upDisValue) +
			angleToNorm(qvertInto[qface[tarFI].cornerPI[3]].value)*(leftDisValue + dowmDisValue);
		return (temp / (4.0*this->specifcQua)).angleP();
	}
	else if (type == TypeField::Distance) {
		double temp = qvertInto[qface[tarFI].cornerPI[0]].value*(rightDisValue + dowmDisValue) +
			qvertInto[qface[tarFI].cornerPI[1]].value*(rightDisValue + upDisValue) +
			qvertInto[qface[tarFI].cornerPI[2]].value*(leftDisValue + upDisValue) +
			qvertInto[qface[tarFI].cornerPI[3]].value*(leftDisValue + dowmDisValue);
		return temp / (4.0*this->specifcQua);
	}
	else {
		qDebug("error!-GetInfoVToW");
		return 0.0;
	}
	/*}
	else {
		return -1.0;
	}*/

}
void QuadraMesh::getQuaMeshInfo(int& col, int& row, Vec3& minP, Vec3& maxP)
{
	col = this->col;
	row = this->row;
	minP = this->minPoint;
	maxP = this->maxPoint;
	//specifcQua = this->specifcQua;
}
void QuadraMesh::getQuaMeshInfo(int& col, int& row, Vec3& minP, Vec3& maxP, double& specifcQua) {
	col = this->col;
	row = this->row;
	minP = this->minPoint;
	maxP = this->maxPoint;
	specifcQua = this->specifcQua;
}
