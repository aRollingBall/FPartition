#include"ThermalField.h"
#include<qdebug.h>
void ThermalField::initialBoundaryValue() {
	for (int i = 0; i < this->difRow; i++) {
		for (int j = 0; j < this->difCol; j++) {
			if (i == 0 || i == this->difRow - 1 || j == 0 || j == this->difCol - 1) {
				qvertInto[i*difCol + j].value = 1.0;
				qvertInto[i*difCol + j].isBoundary = true;
			}
			else {
				qvertInto[i*difCol + j].value = -1.0;
				qvertInto[i*difCol + j].isBoundary = false;
			}
		}
	}
}
void ThermalField::GetDomainValue(std::vector<BELine> &traiB) {
	if (this->isGenQuaMesh) {
		initialBoundaryValue();
		auto iteTraiB1 = traiB.begin();
		auto iteTraiB2 = traiB.end();
		std::set<int> faceI;
		//int lenTB = 0;
		for (; iteTraiB1 != iteTraiB2; iteTraiB1++) {
			faceI.clear();
			getQuaEAloaF(*iteTraiB1->v, faceI);//获得一层领域面
			auto iterFaceI1 = faceI.begin();
			auto iterFaceI2 = faceI.end();
			for (; iterFaceI1 != iterFaceI2; iterFaceI1++) {
				for (int i = 0; i < 4; i++) {
					if (*iterFaceI1 > qface.size() - 1 || *iterFaceI1 < 0) {
						qDebug("array out of reference!");
						isGenTF = false;
						return;
					}
					int vertI = qface[*iterFaceI1].cornerPI[i];
					if (vertI >= 0 && vertI < difRow*difCol) {
						qvertInto[vertI].isBoundary = true;
						if (fabs(iteTraiB1->val[0] - iteTraiB1->val[1]) > 1.0e-5) {
							qDebug("error!");
						}
						if (fabs(qvertInto[vertI].value) < 1.0e-5) {
							qvertInto[vertI].value = (angleToNorm(iteTraiB1->val[0]) + angleToNorm(iteTraiB1->val[1])).angleP();
						}
						else {
							qvertInto[vertI].value = (angleToNorm(qvertInto[vertI].value) + (angleToNorm(iteTraiB1->val[0]) + angleToNorm(iteTraiB1->val[1])).normalized()).angleP();
						}

						//qvertInto[vertI].value = ((*iteTraiB1)[0] - (*iteTraiB1)[1]).norm() < 1e-5 ? 4.0 : 1.0;
					}
					else {
						qDebug("array out of reference!");
						isGenTF = false;
						return;
					}
				}
			}
		}
		faceI.clear();
		/*for (int i = 0; i < 1000; i++) {
			FSMP();
		}*/
		if (DifScan()) {
			for (int i = 0; i < qvertInto.size(); i++) {
				if (!qvertInto[i].isBoundary) {
					qvert[i].color = Vec3(qvertInto[i].value / (2.0*Q_PI));
				}
				else {
					qvert[i].color = Vec3(0.0, 0.0, 0.0);
				}
			}
			this->isGenTF = true;
		}
		else {
			this->isGenTF = false;
		}
	}
	else {
		this->isGenTF = false;
	}
	this->setGenDomainValueState(this->isGenTF);
}

bool calStrL(double& a, double& b, double& c, Vec3 point1, Vec3 point2) {
	double p1 = fabs(point1[0] - point2[0]);
	double p2 = fabs(point1[1] - point2[1]);
	if (p1 < 1e-5 && p2 > 1e-5) {
		a = 1.0;
		b = 0.0;
		c = (-1.0) * point1[0];
	}
	else if (p1 > 1e-5 && p2 < 1e-5) {
		a = 0.0;
		b = 1.0;
		c = (-1.0) * point1[1];
	}
	else if (p1 > 1e-5 && p2 > 1e-5) {
		a = p2 / p1;
		b = -1.0;
		c = point1[1] - a*point1[0];
	}
	else {
		return false;
	}
	return true;
}
Vec3 ThermalField::SelectColor(double vc) {
	return Vec3(vc, vc, vc);
}
void ThermalField::auxFun(int vertI, FS_Dir dir) {
	do {
		double vertX = qvert[vertI].pos[0];
		if (!(vertI / this->difCol <= 0 || vertI / this->difCol >= this->difRow - 1 || vertI%this->difCol == 0 || vertI%this->difCol == this->difCol - 1)) {
			if (!qvertInto[vertI].isBoundary) {
				double oldValue[4] = { qvertInto[vertI - 1].value ,qvertInto[vertI + 1].value ,qvertInto[vertI - this->difCol].value ,qvertInto[vertI + this->difCol].value };
				//qvertInto[i] = 0;
				Vec3 temp;
				for (int j = 0; j < 4; j++) {
					if (oldValue[j] >= 0.0) {
						temp += angleToNorm(oldValue[j]);
						temp = temp.normalized();
					}
					//newQVertInto[i] += oldValue[j];
				}
				qvertInto[vertI].value = temp.angleP();
			}
		}
		else {
			if (qvertInto[vertI].value > 1e4) {
				qvertInto[vertI].value = 0.0;//矩形边界设为边界
			}
		}
		switch (dir)
		{
		case left_up:
			vertI = vertI - 1 + this->difCol;
			break;
		case left_down:
			vertI = vertI - 1 - this->difCol;
			break;
		case right_up:
			vertI = vertI + 1 + this->difCol;
			break;
		case right_down:
			vertI = vertI + 1 - this->difCol;
			break;
		default:
			return;
			break;
		}
		if (vertI < this->difCol*this->difRow && vertI >= 0) {
			if (dir == FS_Dir::left_down || dir == FS_Dir::left_up) {
				if (qvert[vertI].pos[0] > vertX) {
					break;
				}
			}
			if (dir == FS_Dir::right_down || dir == FS_Dir::right_up) {
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
void ThermalField::FSMP() {
	for (int i = 0; i < this->difRow; i++) {
		auxFun(i*this->difCol, FS_Dir::right_down);
	}
	for (int i = 1; i < this->difCol; i++) {
		auxFun(i + this->difCol*(this->difRow - 1), FS_Dir::right_down);
	}
	//this->disValue = 0.0; //第一次遍历得到的值为最大值

	for (int i = this->difCol - 1; i >= 0; i--) {
		auxFun(i + this->difCol*(this->difRow - 1), FS_Dir::right_down);
	}
	for (int i = this->difRow - 2; i >= 0; i--) {
		auxFun(i*this->difCol, FS_Dir::right_down);
	}
	//this->disValue = 0.0; //第一次遍历得到的值为最大值

	for (int i = this->difRow - 1; i >= 0; i--) {
		auxFun(i*this->difCol, FS_Dir::right_up);
	}
	for (int i = 1; i < this->difCol; i++) {
		auxFun(i, FS_Dir::right_up);
	}
	//this->disValue = 0.0; //第一次遍历得到的值为最大值

	for (int i = this->difCol - 1; i <= 0; i++) {
		auxFun(i, FS_Dir::right_up);
	}
	for (int i = 1; i < this->difRow; i++) {
		auxFun(i*this->difCol, FS_Dir::right_up);
	}
}
bool ThermalField::DifScan() {
	double thre = 1.0e-7;
	int i, j, iteration = 0;
	double eps = 0.0, maxeps = 0.0;
	std::vector<double> oldQVertInto;
	oldQVertInto.reserve(this->difRow*this->difCol);
	std::vector<double> newQVertInto;
	newQVertInto.reserve(this->difRow*this->difCol);
	for (int k = 0; k < this->difRow*this->difCol; k++) {
		oldQVertInto.push_back(qvertInto[k].value);
		newQVertInto.push_back(qvertInto[k].value);
	}
	int iter = 0;
	do {
		++iter;
		maxeps = 0.0;
		oldQVertInto.assign(newQVertInto.begin(), newQVertInto.end());
		for (i = 0; i < qvertInto.size(); i++) {
			if (!qvertInto[i].isBoundary) {
				double oldValue[4] = { oldQVertInto[i - 1] ,oldQVertInto[i + 1] ,oldQVertInto[i - this->difCol] ,oldQVertInto[i + this->difCol] };
				double oldValueB[4] = { oldQVertInto[i - this->difCol - 1],oldQVertInto[i - this->difCol + 1], oldQVertInto[i + this->difCol - 1] ,oldQVertInto[i + this->difCol + 1] };
				Vec3 paraValue[4] = { qvert[i - 1].pos,qvert[i + 1].pos,qvert[i - this->difCol].pos,qvert[i + this->difCol].pos };
				Vec3 paraValueB[4] = { qvert[i - this->difCol - 1].pos ,qvert[i - this->difCol + 1].pos, qvert[i + this->difCol - 1].pos ,qvert[i + this->difCol + 1].pos };
				double paraSv[8] = { 0.0 };
				double paraSum = 0.0;
				for (int j = 0; j < 4; j++) {
					double tempV = disField->GetInfoVToW(paraValue[j], TypeField::Distance);
					double tempVB = disField->GetInfoVToW(paraValueB[j], TypeField::Distance);
					/*if (tempV<0.0 || tempV>1.0) {
						tempV = tempV;
					}*/
					/*paraSv[j] = cos(tempV*Q_PI)*cos(tempV*Q_PI)*cos(tempV*Q_PI)*cos(tempV*Q_PI) + 0.01;
					paraSv[j + 4] = cos(tempVB*Q_PI)*cos(tempVB*Q_PI)*cos(tempVB*Q_PI)*cos(tempVB*Q_PI) + 0.01;*/
					/*paraSv[j] = fabs(cos(tempV*Q_PI)*cos(tempV*Q_PI)*cos(tempV*Q_PI)) + 0.1;
					paraSv[j + 4] = fabs(cos(tempVB*Q_PI)*cos(tempVB*Q_PI)*cos(tempVB*Q_PI)) + 0.1;*/
					/*paraSv[j] = cos(tempV*Q_PI)*cos(tempV*Q_PI) + 0.1;
					paraSv[j + 4] = cos(tempVB*Q_PI)*cos(tempVB*Q_PI) + 0.1;*/
					//paraSv[j] = fabs(cos(tempV*Q_PI) + 1.0) / 2.0 + 1.0;
					/*paraSv[j] = 1.01 - fabs(cos((tempV - 0.5)*Q_PI));
					paraSv[j + 4] = 1.01 - fabs(cos((tempVB - 0.5)*Q_PI));*/
					paraSv[j] = 1.0 - fabs(sin(tempV*Q_PI));
					paraSv[j] = pow(paraSv[j], 4) + 0.1;
					paraSv[j + 4] = 1.0 - fabs(sin(tempVB*Q_PI));
					paraSv[j + 4] = pow(paraSv[j + 4], 4) + 0.1;
					//paraSv[j] =6.0*fabs((tempV - 0.5)*(tempV - 0.5)*(tempV - 0.5)*(tempV - 0.5)) + 1.0;
					/*paraSv[j] = (1.0 - tempV)*(1.0 - tempV) + 0.001;
					paraSv[j + 4] = (1.0 - tempVB)*(1.0 - tempVB) + 0.001;*/

					paraSum += paraSv[j] + paraSv[j + 4];

				}
				//newQVertInto[i] = 0;
				Vec3 temp;
				for (int j = 0; j < 4; j++) {
					//if (oldValue[j] >= 0.0) {
					//temp += angleToNorm(oldValue[j])*(paraSv[j] / paraSum);
					temp += angleToNorm(oldValue[j])*4.0*(paraSv[j] / paraSum) + angleToNorm(oldValueB[j])*(paraSv[j + 4] / paraSum)*0.0;
					//temp += angleToNorm(oldValue[j])*4.0 + angleToNorm(oldValueB[j])*1.0;
					//temp = temp.normalized();
				//}
				//newQVertInto[i] += oldValue[j];
				}

				newQVertInto[i] = temp.angleP();
				//newQVertInto[i] /= 4.0;
				maxeps += fabs(newQVertInto[i] - oldQVertInto[i]);
				oldQVertInto[i] = newQVertInto[i];
				//else {
				//eps = fabs(newQVertInto[i] - oldQVertInto[i]);
				////	}
				//if (fabs(eps) > maxeps) {
				//	maxeps = fabs(eps);
				//}
			}
		}
	} while (maxeps > thre);
	for (int k = 0; k < this->difRow*this->difCol; k++) {
		if (fabs(newQVertInto[k]) > 1.0) {
			qDebug("error!");
		}
		qvertInto[k].value = newQVertInto[k];
	}
	return true;
}
ThermalField::~ThermalField()
{
}