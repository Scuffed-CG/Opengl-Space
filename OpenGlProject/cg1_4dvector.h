#ifndef CG1_4DVECTOR_H
#define CG1_4DVECTOR_H


class CG1_4DVector
{
public:
    void DivideByW();
    CG1_4DVector();
    CG1_4DVector(double X, double Y, double Z, double W = 1);
    virtual ~CG1_4DVector();

    double GetY() const;
    double GetX() const;
    double GetZ() const;
    double GetW() const;

    void SetXYZ(double NewX, double NewY, double NewZ);
    void SetXYZW(double NewX, double NewY, double NewZ, double NewW);

    CG1_4DVector& operator=(const CG1_4DVector &v);
    CG1_4DVector operator-();

private:
    double x;
    double y;
    double z;
    double w;
};

#endif // CG1_4DVECTOR_H
