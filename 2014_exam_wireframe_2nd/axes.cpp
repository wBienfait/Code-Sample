#include "axes.h"
#include <math.h>

TAxes::TAxes()
	: x( 1.0f, 0.0f, 0.0f )
	, y( 0.0f, 1.0f, 0.0f )
	, z( 0.0f, 0.0f, 1.0f )
{
}

TGfxVec3 TAxes::Multiply( const TGfxVec3 & tVector ) const
{
	return tVector.x * x + tVector.y * y + tVector.z * z;
}

TAxes TAxes::Multiply( const TAxes & tOther ) const
{
	TAxes tResult;
	tResult.x = tOther.Multiply( x );
	tResult.y = tOther.Multiply( y );
	tResult.z = tOther.Multiply( z );
	return tResult;
}

TAxes TAxes::CreateXRotation( float fAngle )
{
	TAxes tAxes;

	tAxes.y = TGfxVec3( 0.0f, cos( fAngle ), sin( fAngle ) );
	tAxes.z = TGfxVec3( 0.0f, -sin( fAngle ), cos( fAngle ) );

	return tAxes;
}

TAxes TAxes::CreateYRotation( float fAngle )
{
	TAxes tAxes;

	tAxes.x = TGfxVec3( cos( fAngle ), 0.0f, -sin( fAngle ) );
	tAxes.z = TGfxVec3( sin( fAngle ), 0.0f, cos( fAngle ) );

	return tAxes;
}

TAxes TAxes::CreateZRotation( float fAngle )
{
	TAxes tAxes;

	tAxes.x = TGfxVec3( cos( fAngle ), sin( fAngle ), 0.0f );
	tAxes.y = TGfxVec3( -sin( fAngle ), cos( fAngle ), 0.0f );

	return tAxes;
}
