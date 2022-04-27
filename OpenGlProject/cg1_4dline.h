#ifndef CG1_4DLINE_H
#define CG1_4DLINE_H

#include "cg1_4dvector.h"

class CG1_4DLine
{
public:
    CG1_4DLine();
    CG1_4DLine(CG1_4DVector point0, CG1_4DVector point1);
    CG1_4DLine(const CG1_4DVector &point0, const CG1_4DVector &point1);

    const CG1_4DVector &getPoint1() const;
    void setPoint1(const CG1_4DVector &newPoint1);

    const CG1_4DVector &getPoint0() const;
    void setPoint0(const CG1_4DVector &newPoint0);

    double GetY0() const;
    double GetX0() const;
    double GetZ0() const;

    double GetY1() const;
    double GetX1() const;
    double GetZ1() const;

    void SetY0(double newY0);
    void SetX0(double newX0);
    void SetZ0(double newZ0);

    void SetY1(double newY1);
    void SetX1(double newX1);
    void SetZ1(double newZ1);

private:
    CG1_4DVector point0;
    CG1_4DVector point1;

};

#endif // CG1_4DLINE_H
