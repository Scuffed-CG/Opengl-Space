#include "cg1_4dline.h"

CG1_4DLine::CG1_4DLine(const CG1_4DVector &point1, const CG1_4DVector &point2) : point1(point1), point2(point2)
{}

CG1_4DLine::CG1_4DLine()
{
    point0 = CG1_4DVector(0.0f, 0.0f, 0.0f);
    point1 = CG1_4DVector(0.0f, 0.0f, 0.0f);
}

const CG1_4DVector &CG1_4DLine::getPoint1() const
{
    return point1;
}

void CG1_4DLine::setPoint1(const CG1_4DVector &newPoint1)
{
    point1 = newPoint1;
}

const CG1_4DVector &CG1_4DLine::getPoint0() const
{
    return point0;
}

void CG1_4DLine::setPoint0(const CG1_4DVector &newPoint0)
{
    point0 = newPoint0;
}

double CG1_4DLine::GetY0() const{return point0.GetY();}
double CG1_4DLine::GetX0() const{return point0.GetX();}
double CG1_4DLine::GetZ0() const{return point0.GetZ();}

double CG1_4DLine::GetY1() const{return point1.GetY();}
double CG1_4DLine::GetX1() const{return point1.GetX();}
double CG1_4DLine::GetZ1() const{return point1.GetZ();}

void CG1_4DLine::SetY0(double newY0){
    point0.SetXYZ(point0.GetX(), newY0, point0.GetZ());
}
void CG1_4DLine::SetX0(double newX0){
    point0.SetXYZ(newX0, point0.GetY(), point0.GetZ());
}
void CG1_4DLine::SetZ0(double newZ0){
    point0.SetXYZ(point0.GetY(), point0.GetY(), newZ0);
}

void CG1_4DLine::SetY1(double newY1){
    point0.SetXYZ(point1.GetX(), newY1, point1.GetZ());
}
void CG1_4DLine::SetX(double newX1){
    point1.SetXYZ(newX1, point1.GetY(), point1.GetZ());
}
void CG1_4DLine::SetZ1(double newZ1){
    point1.SetXYZ(point1.GetY(), point1.GetY(), newZ1);
}
