#pragma once
#include<math.h>
#include<qstring.h>
#include"global.h"
const double Q_PI = 3.14159265358979323846;
const double interDim = 1.0e-9;
typedef unsigned int uint;
enum showColor
{
	pure_c, s_black, s_blue, s_colour, dirB_color, dirH_color, croB_color, border_color, model_color, sl_color,custom_shader
};
struct Vec3 {
	double v[3];
	Vec3(double x) { v[0] = x; v[1] = x; v[2] = x; }
	Vec3(double x, double y, double z) { v[0] = x; v[1] = y; v[2] = z; }
	Vec3() { v[0] = 0.0; v[1] = 0.0; v[2] = 0.0; }
	double operator[](int i) const { return v[i]; }
	double& operator[](int i) { return v[i]; }
	void operator += (const Vec3& b) { v[0] += b[0]; v[1] += b[1]; v[2] += b[2]; }
	void operator -= (const Vec3& b) { v[0] -= b[0]; v[1] -= b[1]; v[2] -= b[2]; }
	Vec3 operator + (const Vec3& b) const { return Vec3(v[0] + b[0], v[1] + b[1], v[2] + b[2]); }
	Vec3 operator - (const Vec3& b) const { return Vec3(v[0] - b[0], v[1] - b[1], v[2] - b[2]); }
	Vec3 operator * (double b)  const { return Vec3(v[0] * b, v[1] * b, v[2] * b); }
	Vec3 operator / (double b)  const { return Vec3(v[0] / b, v[1] / b, v[2] / b); }
	double operator * (const Vec3& b) const { return v[0] * b[0] + v[1] * b[1] + v[2] * b[2]; }
	Vec3 operator / (const Vec3& b) const { return Vec3(v[0] / b[0], v[1] / b[1], v[2] / b[2]); }
	bool operator == (const Vec3& b) { return (b - *this).norm() < 1e-9; }
	void operator = (const Vec3& b) { v[0] = b[0]; v[1] = b[1]; v[2] = b[2]; }
	double Outer_product(const Vec3 b) {
		double area = this->norm()*b.norm()*sin(acos(this->normalized()*b.normalized()));	//少了1/2,这里为了计算简单
		return v[0] * b[1] - v[1] * b[0] > 0.0 ? area : -1.0*area;
	}
	double norm() const { return sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]); }
	Vec3 normalized() const { return this->norm() < 1e-9 ? Vec3(0.0, 0.0, 0.0) : (*this) / ((*this).norm()); }
	double angleP() const { return atan2(v[1], v[0]) > 0.0 ? atan2(v[1], v[0]) : atan2(v[1], v[0]) + 2.0*Q_PI; }
	Vec3 cross(const Vec3& b) {
		return Vec3(v[1] * b.v[2] - v[2] * b.v[1],
			v[2] * b.v[0] -v[0] * b.v[2],
			v[0] * b.v[1] - v[1] * b.v[0]
		);
	}
	double cross_2d(const Vec3&b) {
		return v[0] * b.v[1] - v[1] * b.v[0];
	}
};
struct pdStr
{
	uint32_t pI;
	Vec3 d;
};
struct LineVs
{
	Vec3 v[2];
	LineVs(Vec3 v1) { v[0] = v1; v[1] = v1; }
	LineVs(Vec3 v1, Vec3 v2) { v[0] = v1; v[1] = v2; }
	LineVs() { v[0] = Vec3(0.0, 0.0, 0.0); v[1] = Vec3(0.0, 0.0, 0.0); }
	Vec3 operator[](int i) const { return v[i]; }
	void operator = (const LineVs& vs) { v[0] = vs[0]; v[1] = vs[1]; }
	double norm() const { return (v[0] - v[1]).norm(); }
};
struct BELine
{
	Vec3 v[2];
	double val[2];
	BELine(LineVs ls, double val1, double val2) { v[0] = ls[0]; v[2] = ls[1]; val[0] = val1; val[1] = val2; }
};
struct LineIndex
{
	int v[2];
	LineIndex(int v1, int v2) { v[0] = v1; v[1] = v2; }
	//无参数构造方法仅用于流线简化
	LineIndex() { v[0] = -2; v[1] = -2; }
	int operator[](int i) const { return v[i]; }
	bool operator<(const LineIndex& vs)const { return ((v[0] < vs.v[0]) || (!(v[0] > vs.v[0]) && (v[1] < vs.v[1]))); }
	void operator = (const LineIndex& b) { v[0] = b[0]; v[1] = b[1]; }
};
struct careQMBI {
	LineIndex bI;
	int bType;
	QuaBoundaryType qbt;
	careQMBI(int bt, LineIndex& b, QuaBoundaryType& qb) :bType(bt), bI(b), qbt(qb) { }
};
struct orderLineIndex {
	int v[2];
	orderLineIndex(int v1, int v2) { if (v1 > v2) { v[0] = v1; v[1] = v2; } else { v[0] = v2; v[1] = v1; } }
	int operator[](int i) const { return v[i]; }
	bool operator<(const orderLineIndex& vs)const { return ((v[0] < vs.v[0]) || (!(v[0] > vs.v[0]) && (v[1] < vs.v[1]))); }
};
struct TriIndex
{
	int v[3];
	TriIndex(int v1, int v2, int v3) { v[0] = v1; v[1] = v2; v[2] = v3; }
	TriIndex(const int index[3]) { v[0] = index[0]; v[1] = index[1]; v[2] = index[3]; }
	int operator[](int i)const { return v[i]; }
	void operator=(const TriIndex& vs) { v[0] = vs[0]; v[1] = vs[1]; v[2] = vs[2]; }
};
struct ToOrder
{
	int slpc;
	int slI;
	ToOrder(int slpc, int slI) { this->slpc = slpc; this->slI = slI; }
};
typedef struct ToOrderDI {
	double dis;
	int sI;
	ToOrderDI(double dis, int slI) { this->dis = dis; this->sI = slI; }
}slType_Secand;
struct ToOrderDIV {
	double val;
	int sI;
	Vec3 dir;
	ToOrderDIV(double val, int slI, Vec3 dir) { this->val = val; this->dir = dir; this->sI = slI; }
};
static bool compToOrder(const ToOrder &a, const ToOrder &b)
{
	return a.slpc < b.slpc;
}
static bool compToOrderDUp(const ToOrderDI &a, const ToOrderDI &b)
{
	return a.dis < b.dis;
}
static bool TCompToOrderDown(const ToOrderDIV &a, const ToOrderDIV &b)
{
	return a.val > b.val;
}

static bool compToOrderDown(const ToOrderDI &a, const ToOrderDI &b)
{
	return a.dis > b.dis;
}
struct QuaIndex
{
	int v[4];
	QuaIndex(int v1, int v2, int v3, int v4) { v[0] = v1; v[1] = v2; v[2] = v3; v[3] = v4; }
	QuaIndex(int vv[]) { v[0] = vv[0], v[1] = vv[1], v[2] = vv[2], v[3] = vv[3]; }
	int operator[](int i)const { return v[i]; }
	void operator=(const QuaIndex& vs) { v[0] = vs.v[0]; v[1] = vs.v[1]; v[2] = vs.v[2]; v[3] = vs.v[3]; }
};
//用于边界曲线
struct mark_1
{
	int vI[3];			//边界曲线上两点
	int count;			//奇点数量
};
static Vec3 angleToNorm(double angle) {
	if (fabs(angle + 2.0*Q_PI) < interDim) {
		return Vec3(1.0, 0.0, 0.0);
	}
	else if (angle < -2.0*Q_PI) {
		return angleToNorm(angle + 2.0*Q_PI);
	}
	else if (fabs(angle + 1.5*Q_PI) < interDim) {
		return Vec3(0.0, 1.0, 0.0);
	}
	else if (angle < -1.5*Q_PI) {
		return Vec3(1.0, tan(angle), 0.0).normalized();
	}
	else if (fabs(angle + 1.0*Q_PI) < interDim) {
		return Vec3(-1.0, 0.0, 0.0);
	}
	else if (angle < -1.0*Q_PI) {
		return Vec3(-1.0, (-1.0)*tan(angle), 0.0).normalized();
	}
	else if (fabs(angle + 0.5*Q_PI) < interDim) {
		return Vec3(0.0, -1.0, 0.0);
	}
	else if (angle < -0.5*Q_PI) {
		return Vec3(-1.0, (-1.0)*tan(angle), 0.0).normalized();
	}
	else if (fabs(angle) < interDim) {
		return Vec3(1.0, 0.0, 0.0);
	}
	else if (angle < 0.0) {
		return Vec3(1.0, tan(angle), 0.0).normalized();
	}
	else if (fabs(angle - 0.5*Q_PI) < interDim) {
		return Vec3(0.0, 1.0, 0.0);
	}
	else if (angle < 0.5*Q_PI) {
		return Vec3(1.0, tan(angle), 0.0).normalized();
	}
	else if (fabs(angle - Q_PI) < interDim) {
		return Vec3(-1.0, 0.0, 0.0);
	}
	else if (angle < Q_PI) {
		return Vec3(-1.0, (-1.0)* tan(angle), 0.0).normalized();
	}
	else if (fabs(angle - 1.5*Q_PI) < interDim) {
		return Vec3(0.0, -1.0, 0.0);
	}
	else if (angle < 1.5*Q_PI) {
		return Vec3(-1.0, (-1.0)*tan(angle), 0.0).normalized();
	}
	else if (fabs(angle - 2.0*Q_PI) < interDim) {
		return Vec3(1.0, 0.0, 0.0);
	}
	else if (angle < 2.0*Q_PI) {
		return Vec3(1.0, tan(angle), 0.0).normalized();
	}
	else {
		return angleToNorm(angle - 2.0*Q_PI);
	}
}
static double dulPow(double value, int i) {
	if (i == 0) {
		return 0.0;
	}
	else if (i > 0) {
		double reValue = value;
		int iter = 1;
		while (i > iter++) {
			reValue *= value;
		}
		return reValue;
	}
	else {
		return 1.0 / dulPow(value, -i);
	}
}
static bool isPoPMid(Vec3 s, Vec3 e, Vec3 p) {
	return (p[0] > s[0] && e[0] < p[0] && p[1]>s[1] && p[1] < e[1]);
}
static QuaBoundaryType getQBTFQS(QString text) {
	if (text == "inlet") {
		return QuaBoundaryType::INLET;
	}
	else if (text == "outlet") {
		return QuaBoundaryType::OUTLET;
	}
	else if (text == "wall0") {
		return QuaBoundaryType::WALL0;
	}
	else if (text == "wall1") {
		return QuaBoundaryType::WALL1;
	}
	else {
		return INDOOR;
	}
}
static int cmp(const void *a, const void *b)
{
	return *(double *)a > *(double *)b ? 1 : -1;
}
static double v_quad_scaled_jacobian(Vec3 p[]) {
	double corner_areas[4], length[4];;
	Vec3 edges[4];
	for (int i = 0; i < 4; i++) {
		edges[i] = p[(i + 1) % 4] - p[i];
		length[i] = edges[i].norm();
	}
	double corner_normals[4];
	for (int i = 0; i < 4; i++) {
		corner_normals[i] = edges[(i + 3) % 4][0] * edges[i % 4][1] - edges[(i + 3) % 4][1] * edges[i][0];
	}
	/*Vec3 principal_axes[2];
	principal_axes[0] = edges[0] - edges[2];
	principal_axes[1] = edges[1] - edges[3];
	double unit_center_normal = principal_axes[0][0] * principal_axes[1][1] - principal_axes[0][1] * principal_axes[1][0];
	for (int i = 0; i < 4; i++) {
		corner_areas[i] = unit_center_normal*corner_normals[i];
	}*/
	double scaled_jac[4];
	scaled_jac[0] = corner_normals[0] / (length[0] * length[3]);
	scaled_jac[1] = corner_normals[1] / (length[1] * length[0]);
	scaled_jac[2] = corner_normals[2] / (length[2] * length[1]);
	scaled_jac[3] = corner_normals[3] / (length[3] * length[2]);
	for (int i = 0; i < 4; i++) {
		scaled_jac[i] = fabs(scaled_jac[i]);
	}
	qsort(scaled_jac, 4, sizeof(scaled_jac[0]), cmp);
	return scaled_jac[0];
}
static bool quaDomainJudge(Vec3 d[]) {
	bool seq = d[3].Outer_product(d[0]) > 0;
	if (seq) {
		return d[0].Outer_product(d[1]) > 0 && d[1].Outer_product(d[2]) > 0 && d[2].Outer_product(d[3]) > 0;
	}
	else {
		return d[0].Outer_product(d[1]) < 0 && d[1].Outer_product(d[2]) < 0 && d[2].Outer_product(d[3]) < 0;
	}
}