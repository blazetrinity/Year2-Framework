/******************************************************************************/
/*!
\file	ObjectClass.h
\author Malcolm Lim
\par	email: Malcolm_Lim\@nyp.edu.sg
\brief
Class to define a Scene Object
*/
/******************************************************************************/

#ifndef OBJECTCLASS_H
#define OBJECTCLASS_H

#include "Vector3.h"
#include "Mtx44.h"
#include "Bound.h"

/******************************************************************************/
/*!
		Class CObjectClass:
\brief	Defines a Scene Object and its methods
*/
/******************************************************************************/
class CObjectClass
{
private:
	Mtx44 Rotate;
	Vector3 Translate;
	Vector3 Scale;
	CBound BoundCheck;
	int ID;

public:
	CObjectClass();
	CObjectClass(Vector3 Translate, Vector3 Scale, Mtx44 Rotate, Vector3 MinBound, Vector3 MaxBound, int ID); 
	~CObjectClass();

	Mtx44 getRotate(void);
	
	Vector3 getTranslate(void);
	Vector3 getScale(void);
	Vector3 getBoundMax(void);
	Vector3 getBoundMin(void);

	int getID();

	void setRotate(Mtx44 R);
	void setTranslate(Vector3 T);
	void setScale(Vector3 S);
	void setBound(Vector3 Min, Vector3 Max);
	void setID(int ID);
	
};

#endif