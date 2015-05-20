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
	CObjectClass(Vector3 Translate, Vector3 Scale, Mtx44 Rotate, float size, int ID); 
	~CObjectClass();

	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_QUAD,
		GEO_TEXT,
		GEO_SKYPLANE,
		GEO_TERRAIN,
		GEO_POND,
		GEO_BULLET,
		GEO_OIL_DRUM,
		GEO_SPRITE_ANIMATION,
		GEO_SHACK,
		/*GEO_SPHERE,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,
		GEO_LEFT,
		GEO_RIGHT,
		GEO_TOP,
		GEO_BOTTOM,
		GEO_FRONT,
		GEO_BACK,*/
		NUM_GEOMETRY,
	};

	void Init(Vector3 Translate, Vector3 Scale, Mtx44 Rotate, float size, int ID);

	Mtx44 getRotate(void);
	
	Vector3 getTranslate(void);
	Vector3 getScale(void);
	Vector3 getBoundMax(void);
	Vector3 getBoundMin(void);

	int getID();

	void setRotate(Mtx44 R);
	void setTranslate(Vector3 T);
	void setScale(Vector3 S);
	void setBound(int size);
	void setID(int ID);
	
};

#endif