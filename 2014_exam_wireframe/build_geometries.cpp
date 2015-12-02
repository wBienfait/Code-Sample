#include "build_geometries.h"
#include "geometry.h"
#include "flib.h"
#include <math.h>

const float SIZE_CUBE = 2.0f;
const float RAY_SPHERE = 1.0f;
const float SCALE = 100.0f;//SpriteSetScale dans main.cpp pas magic number
const float SEGMENT_TORUS = 10.0f;

void BuildBox( TGeometry & tGeometry )
{
	tGeometry.AddLine( TGfxVec3( 1, 1, 1 ), TGfxVec3( -1, 1, 1 ) );
	tGeometry.AddLine( TGfxVec3( -1, 1, 1 ), TGfxVec3( -1, -1, 1 ) );
	tGeometry.AddLine( TGfxVec3( -1, -1, 1 ), TGfxVec3( 1, -1, 1 ) );
	tGeometry.AddLine( TGfxVec3( 1, -1, 1 ), TGfxVec3( 1, 1, 1 ) );

	tGeometry.AddLine( TGfxVec3( 1, 1, -1 ), TGfxVec3( -1, 1, -1 ) );
	tGeometry.AddLine( TGfxVec3( -1, 1, -1 ), TGfxVec3( -1, -1, -1 ) );
	tGeometry.AddLine( TGfxVec3( -1, -1, -1 ), TGfxVec3( 1, -1, -1 ) );
	tGeometry.AddLine( TGfxVec3( 1, -1, -1 ), TGfxVec3( 1, 1, -1 ) );

	tGeometry.AddLine( TGfxVec3( 1, 1, 1 ), TGfxVec3( 1, 1, -1 ) );
	tGeometry.AddLine( TGfxVec3( -1, 1, 1 ), TGfxVec3( -1, 1, -1 ) );
	tGeometry.AddLine( TGfxVec3( -1, -1, 1 ), TGfxVec3( -1, -1, -1 ) );
	tGeometry.AddLine( TGfxVec3( 1, -1, 1 ), TGfxVec3( 1, -1, -1 ) );
}

void BuildSphere( TGeometry & tGeometry, int iSegments, int iDoubleMeridianCount, int iParallelsCount )
{
	//float fRay = 1.0f;
	//float fPerimetre = 2*M_PI*(fRay*SCALE);
	float fAngleMov = static_cast<float>((2*M_PI)/iSegments);

	for (int i = 0; i<iSegments; i++)
	{
		//tGeometry.AddLine( TGfxVec3( cosf(i*fAngleMov), 0, sinf(i*fAngleMov) ), TGfxVec3( cosf((i+1)*fAngleMov), 0, sinf((i+1)*fAngleMov)) );
	}

	float fParallelleAngle = static_cast<float>(M_PI/iParallelsCount);

	for (int a = 0; a<iParallelsCount; a++)
	{
		float fParaRay = cosf(a*fParallelleAngle);
		float fPosY = sinf(a*fParallelleAngle);

		//for (int i = 0; i<iSegments; i++)
		//{
		//	tGeometry.AddLine( TGfxVec3( cosf(i*fAngleMov)*fParaRay, fPosY, sinf(i*fAngleMov)*fParaRay ), TGfxVec3( cosf((i+1)*fAngleMov)*fParaRay, fPosY, sinf((i+1)*fAngleMov)*fParaRay) );
		//}
		//for (int i = 0; i<iSegments; i++)
		//{
		//	tGeometry.AddLine( TGfxVec3( cosf(i*fAngleMov)*fParaRay, -fPosY, sinf(i*fAngleMov)*fParaRay ), TGfxVec3( cosf((i+1)*fAngleMov)*fParaRay, -fPosY, sinf((i+1)*fAngleMov)*fParaRay) );
		//}
	}

	for (int i = 0; i<iDoubleMeridianCount; i++)
	{
		float fMeridiantMov = static_cast<float>(M_PI/iDoubleMeridianCount);
		for (int a = 0; a<iSegments; a++)
		{
			float fPosY = sinf(fAngleMov*a);
			float fSize = cos(fAngleMov*a);
			float fPosY2 = sinf(fAngleMov*(a+1));
			float fSize2 = cos(fAngleMov*(a+1));

			tGeometry.AddLine( TGfxVec3( cosf(i*fMeridiantMov)*fSize, fPosY, sinf(i*fMeridiantMov)*fSize) , TGfxVec3( cosf(i*fMeridiantMov)*fSize2,  fPosY2, sinf(i*fMeridiantMov)*fSize2) );
		}
	}

}

void BuildPlane( TGeometry & tGeometry, int iTileCountX, int iTileCountZ )
{
	tGeometry.AddLine( TGfxVec3( 1, 0, 1 ), TGfxVec3( -1, 0, 1 ) );
	tGeometry.AddLine( TGfxVec3( -1, 0, 1 ), TGfxVec3( -1, 0, -1 ) );
	tGeometry.AddLine( TGfxVec3( -1, 0, -1 ), TGfxVec3( 1, 0, -1 ) );
	tGeometry.AddLine( TGfxVec3( 1, 0, -1 ), TGfxVec3( 1, 0, 1 ) );

	float fXCoo = (SIZE_CUBE/static_cast<float>(iTileCountX));
	for (int i = 0; i<iTileCountX; i++)
	{
		
		tGeometry.AddLine( TGfxVec3( (i*fXCoo)-1, 0, 1 ), TGfxVec3( (i*fXCoo)-1, 0, -1 ) );
	}
	float fZCoo = (SIZE_CUBE/static_cast<float>(iTileCountZ));
	for (int i = 0; i<iTileCountZ; i++)
	{
		
		tGeometry.AddLine( TGfxVec3( 1, 0, (i*fZCoo)-1 ), TGfxVec3( -1, 0, (i*fZCoo)-1 ) );
	}
}

void BuildDoughnut( TGeometry & tGeometry, int iSegments, float fRadius )
{
	float fAngleMov = 2*M_PI/SEGMENT_TORUS;
	for (int i = 0; i<iSegments; i++)
	{
		float fRadiusMov = 2*M_PI/iSegments;
	    for (int a = 0; a<SEGMENT_TORUS; a++)
	    {
	    	float fPosY = sinf(fAngleMov*a);
	    	float fSize = cos(fAngleMov*a);
	    	float fPosY2 = sinf(fAngleMov*(a+1));
	    	float fSize2 = cos(fAngleMov*(a+1));

	    
			TGfxVec3 tRadiusMov = TGfxVec3(cosf(i*fRadiusMov)*fRadius, 0, sinf(i*fRadiusMov)*fRadius);
			TGfxVec3 tPosition = tRadiusMov + TGfxVec3( cosf(i*fRadiusMov)*fSize*fRadius, fPosY*fRadius, sinf(i*fRadiusMov)*fSize*fRadius);
			TGfxVec3 tPosition2 = tRadiusMov + TGfxVec3( cosf(i*fRadiusMov)*fSize2*fRadius, fPosY2*fRadius, sinf(i*fRadiusMov)*fSize2*fRadius);

			TGfxVec3 tNextRadiusMov = TGfxVec3(cosf((i+1)*fRadiusMov)*fRadius, 0, sinf((i+1)*fRadiusMov)*fRadius);
			TGfxVec3 tPositionNext = tNextRadiusMov + TGfxVec3( cosf((i+1)*fRadiusMov)*fSize*fRadius, fPosY*fRadius, sinf((i+1)*fRadiusMov)*fSize*fRadius);
	    
			tGeometry.AddLine( tPosition , tPosition2 );
			tGeometry.AddLine( tPosition , tPositionNext );

	    }
	}
}

void BuildPyramid( TGeometry & tGeometry, int iSegments )
{
	float fHauteur = SIZE_CUBE/iSegments;
	for (int i = 0; i<iSegments; i++)
	{
		float fDiff = (SIZE_CUBE/(iSegments+1))/2;
		float fLargeur = (SIZE_CUBE - i*fDiff)-1;

	tGeometry.AddLine( TGfxVec3( fLargeur, (i*fHauteur)-1, fLargeur ), TGfxVec3( fLargeur, ((i+1)*fHauteur)-1, fLargeur ) );
	tGeometry.AddLine( TGfxVec3( fLargeur, ((i+1)*fHauteur)-1, fLargeur ), TGfxVec3( -fLargeur, ((i+1)*fHauteur)-1, fLargeur ) );
	tGeometry.AddLine( TGfxVec3( -fLargeur, ((i+1)*fHauteur)-1, fLargeur ), TGfxVec3( -fLargeur, ((i)*fHauteur)-1, fLargeur ) );
	tGeometry.AddLine( TGfxVec3( -fLargeur, ((i)*fHauteur)-1, fLargeur ), TGfxVec3( fLargeur, (i*fHauteur)-1, fLargeur ) );

	tGeometry.AddLine( TGfxVec3( fLargeur, (i*fHauteur)-1, fLargeur ), TGfxVec3( fLargeur, (i*fHauteur)-1, -fLargeur ) );
	tGeometry.AddLine( TGfxVec3( -fLargeur, (i*fHauteur)-1, fLargeur ), TGfxVec3( -fLargeur, (i*fHauteur)-1, -fLargeur ) );
	tGeometry.AddLine( TGfxVec3( fLargeur, ((i+1)*fHauteur)-1, -fLargeur ), TGfxVec3( fLargeur, ((i+1)*fHauteur)-1, fLargeur ) );
	tGeometry.AddLine( TGfxVec3( -fLargeur, ((i+1)*fHauteur)-1, -fLargeur ), TGfxVec3( -fLargeur, ((i+1)*fHauteur)-1, fLargeur ) );

	tGeometry.AddLine( TGfxVec3( fLargeur, (i*fHauteur)-1, -fLargeur ), TGfxVec3( fLargeur, ((i+1)*fHauteur)-1, -fLargeur ) );
	tGeometry.AddLine( TGfxVec3( fLargeur, ((i+1)*fHauteur)-1, -fLargeur ), TGfxVec3( -fLargeur, ((i+1)*fHauteur)-1, -fLargeur ) );
	tGeometry.AddLine( TGfxVec3( -fLargeur, ((i+1)*fHauteur)-1, -fLargeur ), TGfxVec3( -fLargeur, ((i)*fHauteur)-1, -fLargeur ) );
	tGeometry.AddLine( TGfxVec3( -fLargeur, ((i)*fHauteur)-1, -fLargeur ), TGfxVec3( fLargeur, (i*fHauteur)-1, -fLargeur ) );
	}

}

void BuildHelicoid( TGeometry & tGeometry, int iSegments, int iTwist )
{
	float fHauteur = SIZE_CUBE/static_cast<float>(iSegments);
	float fMovAngle = Math_fRadian(iTwist);
	fMovAngle /= static_cast<float>(iSegments);

	for (int i = 0; i<iSegments; i++)
	{
		float fAngle = fMovAngle*i;
		tGeometry.AddLine( TGfxVec3( 0, (i*fHauteur)-1, 0 ), TGfxVec3(0, ((i+1)*fHauteur)-1, 0 ) );
		tGeometry.AddLine( TGfxVec3( 0, ((i+1)*fHauteur)-1, 0 ), TGfxVec3( cosf(fAngle), ((i+1)*fHauteur)-1, sinf(fAngle) ) );
		tGeometry.AddLine( TGfxVec3( cosf(fAngle), ((i+1)*fHauteur)-1, sinf(fAngle) ), TGfxVec3( cosf(fAngle), ((i)*fHauteur)-1, sinf(fAngle)) );
		tGeometry.AddLine( TGfxVec3( cosf(fAngle), ((i)*fHauteur)-1, sinf(fAngle)), TGfxVec3( 0, (i*fHauteur)-1, 0 ) );

		tGeometry.AddLine( TGfxVec3( cosf(fAngle), ((i+1)*fHauteur)-1, sinf(fAngle) ), TGfxVec3( cosf(fAngle+fMovAngle), ((i+1)*fHauteur)-1, sinf(fAngle+fMovAngle)) );
		tGeometry.AddLine( TGfxVec3( cosf(fAngle), ((i)*fHauteur)-1, sinf(fAngle) ), TGfxVec3( cosf(fAngle+fMovAngle), ((i)*fHauteur)-1, sinf(fAngle+fMovAngle)) );
		tGeometry.AddLine( TGfxVec3( cosf(fAngle+fMovAngle), ((i+1)*fHauteur)-1, sinf(fAngle+fMovAngle) ), TGfxVec3( cosf(fAngle+fMovAngle), ((i)*fHauteur)-1, sinf(fAngle+fMovAngle)) );
		tGeometry.AddLine( TGfxVec3( cosf(fAngle+fMovAngle), ((i)*fHauteur)-1, sinf(fAngle+fMovAngle) ), TGfxVec3( 0, (i*fHauteur)-1, 0 ) );
	}
}
