#include<qdebug.h>
#include"DisFieldMesh.h"
//targetP:目标点；bp1:目标线一端点；bp2:目标线另一端点
//返回目标点距离目标线的最短距离
void DisFieldMesh::getMinDisV(int targetPI, LineVs vs) {
	Vec3 targetP = qvert[targetPI].pos;
	if ((targetP - vs[0]).norm() < 1e-4 || (targetP - vs[1]).norm() < 1e-4) {
		qvertInto[targetPI].value = 0.0f;
		return;
	}
	Vec3 n1 = targetP - vs[0];
	Vec3 n2 = targetP - vs[1];
	Vec3 n3 = vs[0] - vs[1];
	if (n1*(n3*(-1.0)) < 0.0) {
		if (qvertInto[targetPI].value > n1.norm()) {
			qvertInto[targetPI].value = n1.norm();
		}
	}
	else if (n2*n3 < 0.0) {
		if (qvertInto[targetPI].value > n2.norm()) {
			qvertInto[targetPI].value = n2.norm();
		}
	}
	else {
		double valueTemp = abs(n2[0] * n3[1] - n2[1] * n3[0]) / n3.norm();
		if (qvertInto[targetPI].value > valueTemp) {
			qvertInto[targetPI].value = valueTemp;
		}
	}
}
//traiB:二维区域边界，由于可能存在多条边界
//todo:得到四边形区域的边界
void DisFieldMesh::GetDomainValue(std::vector<LineVs>& traiB) {
	if (this->isGenQuaMesh) {
		auto iteTraiB1 = traiB.begin();
		auto iteTraiB2 = traiB.end();
		std::set<int> faceI;
		for (; iteTraiB1 != iteTraiB2; iteTraiB1++) {
			faceI.clear();
			getQuaEAloaF(*iteTraiB1, faceI);//获得一层领域面
			auto iterFaceI1 = faceI.begin();
			auto iterFaceI2 = faceI.end();
			for (; iterFaceI1 != iterFaceI2; iterFaceI1++) {
				for (int i = 0; i < 4; i++) {
					if (*iterFaceI1 > qface.size() - 1 || *iterFaceI1 < 0) {
						qDebug("array out of reference!");
						isGenDFM = false;
						return;
					}
					int vertI = qface[*iterFaceI1].cornerPI[i];
					if (vertI >= 0 && vertI < disRow*disCol) {
						qvertInto[vertI].isBoundary = true;
						qvert[vertI].color = Vec3(0.0, 1.0, 0.0);
						getMinDisV(vertI, *iteTraiB1);
					}
					else {
						qDebug("array out of reference!");
						isGenDFM = false;
						return;
					}
				}
			}
		}
		faceI.clear();
		this->isGenDFM = true;

		//add

		//end

		for (int i = 0; i < 3; i++) {
			FSMP();
		}
		//double temp = (this->maxPoint - this->minPoint).norm() / 2.0;
		//this->disValue = temp > this->disValue ? this->disValue : temp;
		for (int i = 0; i < qvertInto.size(); i++) {
			if (!qvertInto[i].isBoundary)
				qvert[i].color = SelectColor(qvertInto[i].value);
		}
	}
	else {
		this->isGenDFM = false;
	}
	this->setGenDomainValueState(this->isGenDFM);
}
void  DisFieldMesh::GetDomainValue(std::vector<Vec3>& traiB, const int len) {
	if (this->isGenQuaMesh) {
		std::set<int> faceI;
		//std::vector<LineVs> tb;
		for (int i = 0; i < len; i++) {
			LineVs vs;
			if (i < len - 1) {
				vs = LineVs(traiB[i], traiB[i + 1]);
			}
			else {
				vs = LineVs(traiB[len - 1], traiB[0]);
			}
			faceI.clear();
			getQuaEAloaF(vs, faceI);
			auto iterFaceI1 = faceI.begin();
			auto iterFaceI2 = faceI.end();
			for (; iterFaceI1 != iterFaceI2; iterFaceI1++) {
				if (*iterFaceI1 > qface.size() - 1 || *iterFaceI1 < 0) {
					qDebug("array out of reference!");
					isGenDFM = false;
					return;
				}
				for (int i = 0; i < 4; i++) {
					int vertI = qface[*iterFaceI1].cornerPI[i];
					if (vertI >= 0 && vertI < disRow*disCol) {
						qvertInto[vertI].isBoundary = true;
						qvert[vertI].color = Vec3(0.0, 1.0, 0.0);
						getMinDisV(vertI, vs);
					}
					else {
						qDebug("array out of reference!");
						isGenDFM = false;
						return;
					}
				}
			}
		}
		faceI.clear();
		this->isGenDFM = true;
		for (int i = 0; i < 3; i++) {
			FSMP();
		}
		for (int i = 0; i < qvertInto.size(); i++) {
			qvertInto[i].value = qvertInto[i].value / disValue;
			if (!qvertInto[i].isBoundary)
				qvert[i].color = SelectColor(qvertInto[i].value);
		}
	}
	else {
		this->isGenDFM = false;
	}
	this->setGenDomainValueState(this->isGenDFM);
}

Vec3 DisFieldMesh::SelectColor(double vc) {
	//double selectItem = vc / this->disValue;
	return Vec3(vc, vc, vc);
	/*if (selectItem < 1.0 / 3.0) {
		return Vec3(selectItem*3.0, 1.0, 1.0);
	}
	else if (selectItem < 2.0 / 3.0) {
		return Vec3(0.0, (selectItem - 1.0 / 3.0)*3.0, 1.0);
	}
	else {
		return Vec3(1.0, 0.0, (selectItem - 2.0 / 3.0)*3.0);
	}*/
}
void DisFieldMesh::auxFun(int vertI, FS_Dir Dir) {
	do {
		double vertX = qvert[vertI].pos[0];
		if (!(vertI / this->disCol <= 0 || vertI / this->disCol >= this->disRow - 1 || vertI%this->disCol == 0 || vertI%this->disCol == this->disCol - 1)) {
			if (!qvertInto[vertI].isBoundary) {
				double valueLeft = qvertInto[vertI - 1].value;
				double valueRight = qvertInto[vertI + 1].value;
				double valueUp = qvertInto[vertI + this->disCol].value;
				double valueDown = qvertInto[vertI - this->disCol].value;
				double a = std::min(valueLeft, valueRight);
				double b = std::min(valueUp, valueDown);
				double valueP = a + this->specifcQua;
				if (valueP > b) {
					valueP = (a + b + sqrt(abs(2 * this->specifcQua*this->specifcQua - (a - b)*(a - b)))) / 2;
				}
				qvertInto[vertI].value = std::min(qvertInto[vertI].value, valueP);
				this->disValue = std::max(qvertInto[vertI].value, disValue);
			}
		}
		else {
			if (qvertInto[vertI].value > 1e4) {
				qvertInto[vertI].value = 0.0;//矩形边界设为边界
			}
		}
		switch (Dir)
		{
		case left_up:
			vertI = vertI - 1 + this->disCol;
			break;
		case left_down:
			vertI = vertI - 1 - this->disCol;
			break;
		case right_up:
			vertI = vertI + 1 + this->disCol;
			break;
		case right_down:
			vertI = vertI + 1 - this->disCol;
			break;
		default:
			return;
			break;
		}
		if (vertI < this->disCol*this->disRow && vertI >= 0) {
			if (Dir == FS_Dir::left_down || Dir == FS_Dir::left_up) {
				if (qvert[vertI].pos[0] > vertX) {
					break;
				}
			}
			if (Dir == FS_Dir::right_down || Dir == FS_Dir::right_up) {
				if (qvert[vertI].pos[0] < vertX) {
					break;
				}
			}
		}
		else {
			break;
		}
	} while (true);
}
void DisFieldMesh::FSMP() {
	//四个顶点方向遍历	
	//add
	int counts = 0;
	//end
	for (int i = 0; i < this->disRow; i++) {
		auxFun(i*this->disCol, FS_Dir::right_down);
	}
	for (int i = 1; i < this->disCol; i++) {
		auxFun(i + this->disCol*(this->disRow - 1), FS_Dir::right_down);
	}
	this->disValue = 0.0; //第一次遍历得到的值为最大值

	for (int i = this->disCol - 1; i >= 0; i--) {
		auxFun(i + this->disCol*(this->disRow - 1), FS_Dir::right_down);
	}
	for (int i = this->disRow - 2; i >= 0; i--) {
		auxFun(i*this->disCol, FS_Dir::right_down);
	}
	this->disValue = 0.0; //第一次遍历得到的值为最大值

	for (int i = this->disRow - 1; i >= 0; i--) {
		auxFun(i*this->disCol, FS_Dir::right_up);
	}
	for (int i = 1; i < this->disCol; i++) {
		auxFun(i, FS_Dir::right_up);
	}
	this->disValue = 0.0; //第一次遍历得到的值为最大值

	for (int i = this->disCol - 1; i <= 0; i++) {
		auxFun(i, FS_Dir::right_up);
	}
	for (int i = 1; i < this->disRow; i++) {
		auxFun(i*this->disCol, FS_Dir::right_up);
	}
	//auxFun()

}
//void QuaMesh::FSM() {
//	auto iterVb = qvertInto.begin();
//	auto iterVe = qvertInto.end();
//	for (; iterVb != iterVe; iterVb++) {
//		int iterI = (*iterVb).index;
//		if (iterI / this->col <= 0 || iterI / this->col >= this->row - 1 || iterI%this->col == 0 || iterI%this->col == this->col - 1) {
//			continue;
//		}
//		else {
//			if (!qvertInto[iterI].isBoundary) {
//				//边界已知
//				double valueLeft = qvertInto[iterI - 1].value;
//				double valueRight = qvertInto[iterI + 1].value;
//				double valueUp = qvertInto[iterI + this->col].value;
//				double valueDown = qvertInto[iterI - this->col].value;
//				double a = valueLeft < valueRight ? valueLeft : valueRight;
//				double b = valueUp < valueDown ? valueUp : valueDown;
//				double valueP = a + this->specifcQua;
//				if (valueP > b) {
//					valueP = (a + b + sqrt(2 * this->specifcQua*this->specifcQua - (a - b)*(a - b))) / 2;
//				}
//				qvertInto[iterI].value = qvertInto[iterI].value < valueP ? qvertInto[iterI].value : valueP;
//				this->disValue = qvertInto[iterI].value > disValue ? qvertInto[iterI].value : disValue;
//			}
//		}
//	}
//}
//todo:求三角形面积
double getArea(double e1, double e2, double e3) {
	double len = (e1 + e2 + e3) / 2.0;
	double sArea = sqrt(len*(len - e1)*(len - e2)*(len - e3));
	return sArea < 1e-3 ? (-1.0)*e2 / e1 : sArea;
}
//判断某个点是否在四边形内,并求出其位置
//bp[4]:左上，左下，右上，右下
bool isExistInter(Vec3 targetP, Vec3 bp[4]) {
	double en[4];
	for (int i = 0; i < 4; i++) {
		en[i] = (bp[(i + 1) % 4] - bp[i]).norm();
	}
	//判断目标点是否在四边形四个顶点上
	double tn[4];
	for (int i = 0; i < 4; i++) {
		tn[i] = (targetP - bp[i]).norm();
		if (tn[i] < 1e-4) {
			//当目标点在矩形顶点上
			return true;
		}
	}
	double sA = en[0] * en[1];//矩形面积
	double ss[4];
	for (int i = 0; i < 4; i++) {
		ss[i] = getArea(en[i], tn[i], tn[(i + 1) % 4]);
		if (ss[i] < 0.0) {
			//当目标顶点在矩形边上

			return true;
		}
	}
	return abs(ss[0] + ss[1] + ss[2] + ss[3] - sA) < 1e-4;
}
//double DisFieldMesh::GetDisPToM(Vec3 targetP) {
//	int x = int((targetP[0] - this->minPoint[0]) / this->specifcQua);
//	int y = int((targetP[1] - this->minPoint[1]) / this->specifcQua);
//	int tarFI = x + y*(this->col - 1);
//	double leftDisValue = abs(targetP[0] - this->minPoint[0] - x*this->specifcQua);
//	double rightDisValue = abs((x + 1)*this->specifcQua + this->minPoint[0] - targetP[0]);
//	double upDisValue = abs((y + 1)*this->specifcQua + this->minPoint[1] - targetP[1]);
//	double dowmDisValue = abs(targetP[1] - this->minPoint[1] - y*this->specifcQua);
//	if (leftDisValue < this->specifcQua && rightDisValue < this->specifcQua && upDisValue < this->specifcQua && dowmDisValue < this->specifcQua) {
//		double temp = qvertInto[qface[tarFI].cornerPI[0]].value*(rightDisValue + dowmDisValue) / (2.0*this->specifcQua) +
//			qvertInto[qface[tarFI].cornerPI[1]].value*(rightDisValue + upDisValue) / (2.0*this->specifcQua) +
//			qvertInto[qface[tarFI].cornerPI[2]].value*(leftDisValue + upDisValue) / (2.0*this->specifcQua) +
//			qvertInto[qface[tarFI].cornerPI[3]].value*(leftDisValue + dowmDisValue) / (2.0*this->specifcQua);
//		if (temp > 1000) {
//			if (1);
//		}
//		return temp;
//	}
//	else {
//		return -1.0;
//	}
//}
DisFieldMesh::~DisFieldMesh()
{
}