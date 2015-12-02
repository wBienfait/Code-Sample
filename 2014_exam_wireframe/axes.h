#ifndef AXES_H_INCLUDED
#define AXES_H_INCLUDED

#include "flib_vec3.h"

struct TAxes
{
	TAxes();

	TGfxVec3 x;
	TGfxVec3 y;
	TGfxVec3 z;

	TGfxVec3 Multiply( const TGfxVec3 & tVector ) const;
	TAxes Multiply( const TAxes & tOther ) const;

	static TAxes CreateXRotation( float fAngle );
	static TAxes CreateYRotation( float fAngle );
	static TAxes CreateZRotation( float fAngle );
};

#endif
