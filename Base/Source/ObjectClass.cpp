#include "ObjectClass.h"

CObjectClass::CObjectClass()
{
	Rotate.SetToIdentity();
	Translate.SetZero();
	Scale.SetZero();
}

CObjectClass::CObjectClass(Vector3 Translate, Vector3 Scale, Mtx44 Rotate, Vector3 MinBound, Vector3 MaxBound, int ID)
{
	this->Translate = Translate;
	this->Scale = Scale;
	this->Rotate = Rotate;
	this->ID = ID;
	BoundCheck.setBound(MinBound, MaxBound);
}

CObjectClass::~CObjectClass()
{

}

Mtx44 CObjectClass::getRotate()
{
	return Rotate;
}

Vector3 CObjectClass::getTranslate()
{
	return Translate;
}

Vector3 CObjectClass::getScale()
{
	return Scale;
}

Vector3 CObjectClass::getBoundMax()
{
	return BoundCheck.getBoundMax();
}

Vector3 CObjectClass::getBoundMin()
{
	return BoundCheck.getBoundMin();
}

void CObjectClass::setRotate(Mtx44 R)
{
	Rotate = R;
}

void CObjectClass::setTranslate(Vector3 T)
{
	Translate = T;
}

void CObjectClass::setScale(Vector3 S)
{
	Scale = S;
}

void CObjectClass::setBound(Vector3 Min, Vector3 Max)
{
	BoundCheck.setBound(Min,Max);
}

void CObjectClass::setID(int ID)
{
	this->ID = ID;
}