#include "cg1_4dvector.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/********************************************************************/

CG1_4DVector::CG1_4DVector()
{
    x = y = z = 0.0f;
    w = 1.0f;
}

/********************************************************************/

CG1_4DVector::CG1_4DVector(double X, double Y, double Z, double W)
{
    x = X;
    y = Y;
    z = Z;
    w = W;
}

/********************************************************************/

CG1_4DVector::~CG1_4DVector()
{

}

/********************************************************************/

CG1_4DVector &CG1_4DVector::operator=(const CG1_4DVector &v)	// TOEKENNINGS-OPERATOR
{
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;

    return *this;
}

/********************************************************************/

CG1_4DVector CG1_4DVector::operator-()	// NEGATIE
{
    CG1_4DVector Neg;
    Neg.SetXYZW(-x, -y, -z, w);

    return Neg;
}

/********************************************************************/

void CG1_4DVector::SetXYZ(double NewX, double NewY, double NewZ)
{
    x = NewX;
    y = NewY;
    z = NewZ;
    w = 1.0f;
}

/********************************************************************/

void CG1_4DVector::SetXYZW(double NewX, double NewY, double NewZ, double NewW)
{
    x = NewX;
    y = NewY;
    z = NewZ;
    w = NewW;
}

/********************************************************************/

double CG1_4DVector::GetX() const
{
    return x;
}

/********************************************************************/

double CG1_4DVector::GetY() const
{
    return y;
}

/********************************************************************/

double CG1_4DVector::GetZ() const
{
    return z;
}

/********************************************************************/

double CG1_4DVector::GetW() const
{
    return w;
}

/********************************************************************/

void CG1_4DVector::DivideByW()
{
    if(w)
    {
        x /= w;
        y /= w;
        z /= w;
        w = 1.0f;
    }
}
