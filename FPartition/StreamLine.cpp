#include"streamLine.h"
StreamLine::StreamLine(int startSI, int endSI, int startSLPI, int endSLPI, bool isB, double disSL, bool dr, bool is)
	:isShow(true), isf(false), typeSL(-1), startSingularIndex(startSI), endSingularIndex(endSI), distanceSL(disSL), isSelect(false),
	startSLPointIndex(startSLPI), endSLPointIndex(endSLPI), isBorder(isB), dir(dr), auxEndSLPI(endSLPI), auxStartSLPI(startSLPI), isN(is)
{
	interV.clear();
	this->slpc = endSLPI - startSLPI + 1 > 0 ? endSLPI - startSLPI + 1 : 0;
	this->nearFaceI[0] = -1;
	this->nearFaceI[1] = -1;
	this->qbt = QuaBoundaryType::INDOOR;
	this->bpc = -1;
	this->selSVI = -1;
	this->aslpc = -1;
	this->selSLSegR = 1.0;
	this->objSSL = -1;
	this->slt.insert(0);
	//this->auxQBT = SLQBT();
}
bool StreamLine::setNearFaceI(int faceI) {
	if (nearFaceI[0] >= 0) {
		if (nearFaceI[1] >= 0) {
			return false;
		}
		else {
			nearFaceI[1] = faceI;
		}
	}
	else {
		nearFaceI[0] = faceI;
	}
	return true;
}
bool StreamLine::refreshEndInfo(int newEndSingularIndex, int newEndSLPointIndex) {
	if (newEndSingularIndex < 0 || newEndSLPointIndex < 0) {
		return false;
	}
	this->endSingularIndex = newEndSingularIndex;
	this->endSLPointIndex = newEndSLPointIndex;
	return true;
}
bool StreamLine::refreshStartInfo(int newStartSingularIndex, int newStartSLPointIndex) {
	if (newStartSingularIndex < 0 || newStartSLPointIndex < 0) {
		return false;
	}
	this->startSingularIndex = newStartSingularIndex;
	this->startSLPointIndex = newStartSLPointIndex;
	return true;
}
bool StreamLine::refreshSLPInfo(int startSLPI, int endSLPI) {
	if (startSLPI < 0 || endSLPI < 0 || startSLPI >= endSLPI) {
		return false;
	}
	this->auxStartSLPI = startSLPI;
	this->auxEndSLPI = endSLPI;
	this->aslpc = endSLPI - startSLPI + 1 > 0 ? endSLPI - startSLPI + 1 : 0;
	return true;
}
bool StreamLine::refreshDisSLInfo(double disSL) {
	if (disSL < 0.0) {
		return false;
	}
	this->distanceSL = disSL;
	return true;
}
void StreamLine::getSLDir(int svI, int& pI1, int& pI2) {
	if (svI == this->startSingularIndex) {
		pI1 = startSLPointIndex;
		pI2 = startSLPointIndex + 1;
	}
	else {
		if (this->slpc == 2) {
			pI1 = endSLPointIndex;
			pI2 = startSLPointIndex;
		}
		else {
			pI1 = endSLPointIndex - 1;
			pI2 = endSLPointIndex - 2;
		}
	}
}