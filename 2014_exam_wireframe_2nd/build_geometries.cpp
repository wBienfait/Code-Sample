#include "build_geometries.h"
#include "geometry.h"
#include "flib.h"

#include "extended_math.h"



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

void BuildOrbit(TGeometry & tGeometry, int iTwistAngle, int iSegments, int iCircleCount)
{
	const float fAngle = static_cast<float>((2*M_PI)/iSegments); 
	const float fMovRadius = 1.0f/iCircleCount;
	const float fMovAngle = static_cast<float>(iTwistAngle/iCircleCount/180.0f*M_PI);

	for (int a = 0; a<= iCircleCount; a++)
	{
		const float fRadius = 1.0f-(a*fMovRadius);
		const float fVertAngle = (a+1)*fMovAngle;

		for (int i = 0; i< iSegments; i++)
		{
			const float fActAngle = i*fAngle;
			const float fNextAngle = (i+1)*fAngle;

			const float fPosY = sinf(fActAngle)*fRadius;
			const float fSize = cosf(fActAngle)*fRadius;
			const float fPosY2 = sinf(fNextAngle)*fRadius;
			const float fSize2 = cosf(fNextAngle)*fRadius;


			tGeometry.AddLine( TGfxVec3( cosf(fVertAngle)*fSize, sinf(fVertAngle)*fSize, fPosY) , TGfxVec3( cosf(fVertAngle)*fSize2, sinf(fVertAngle)*fSize2,  fPosY2) );
		}
	}
}

void BuildHoneycomb(TGeometry & tGeometry, int iRows, int iTurns)
{
	const float fRowSize = 2.0f/iRows;
	const float fBigAngles = static_cast<float>((2*M_PI)/ iTurns);
	const float fSmallAngles = static_cast<float>((2*M_PI)/(4*iTurns));

	int iCount = 0;
	for (int i = 0; i < iRows; i++)
	{
		const float fActRow = -1 + (i*fRowSize);
		const float fNextRow = -1+((i+1)*fRowSize);

		if(iCount == 0)
		{
			for(int a = 0; a <= iTurns; a++)
			{
				float fAngles = a*fBigAngles;
				float fAngles2 = fAngles + fSmallAngles;
				tGeometry.AddLine( TGfxVec3( cosf(fAngles), fActRow, sinf(fAngles) ), TGfxVec3( cosf(fAngles2), fNextRow, sinf(fAngles2) ) );
				fAngles = fAngles2;
				fAngles2+= fSmallAngles;
				tGeometry.AddLine( TGfxVec3( cosf(fAngles), fNextRow, sinf(fAngles) ), TGfxVec3( cosf(fAngles2), fNextRow, sinf(fAngles2) ) );
				fAngles = fAngles2;
				fAngles2+= fSmallAngles;
				tGeometry.AddLine( TGfxVec3( cosf(fAngles), fNextRow, sinf(fAngles) ), TGfxVec3( cosf(fAngles2), fActRow, sinf(fAngles2) ) );

			}
			iCount = 2;
		}
		else
		{
			for(int a = 0; a <= iTurns; a++)
			{
				float fAngles = a*fBigAngles + 2*fSmallAngles;
				float fAngles2 = fAngles + fSmallAngles;
				tGeometry.AddLine( TGfxVec3( cosf(fAngles), fActRow, sinf(fAngles) ), TGfxVec3( cosf(fAngles2), fNextRow, sinf(fAngles2) ) );
				fAngles = fAngles2;
				fAngles2+= fSmallAngles;
				tGeometry.AddLine( TGfxVec3( cosf(fAngles), fNextRow, sinf(fAngles) ), TGfxVec3( cosf(fAngles2), fNextRow, sinf(fAngles2) ) );
				fAngles = fAngles2;
				fAngles2+= fSmallAngles;
				tGeometry.AddLine( TGfxVec3( cosf(fAngles), fNextRow, sinf(fAngles) ), TGfxVec3( cosf(fAngles2), fActRow, sinf(fAngles2) ) );

			}
		}
		iCount--;
	}
}

void BuildHelix(TGeometry & tGeometry, float fTurnCount, int iTwistCount, float fRadius, int iSegmentsPerTurn, int iLinkStep)
{
	const float fTurnSize = 2.0f/fTurnCount;
	const float fSegmentSize = fTurnSize/iSegmentsPerTurn;
	const float fAngle = static_cast<float>((2*M_PI)/iSegmentsPerTurn);
	const float fScale = 1-fRadius;
	const float fRotate = static_cast<float>(iSegmentsPerTurn)/iTwistCount;
	const float fRotateAngle = static_cast<float>((2*M_PI)/ fRotate);

	int iCount = 0;
	for (int i = 0; i<=(iSegmentsPerTurn*fTurnCount); i++)
	{
		const float fHelixAngle = i*fRotateAngle;
		GfxDbgPrintf("%f\n", fHelixAngle);

		
		const float fActAngle = i*fAngle;
		const float fNextAngle = (i+1)*fAngle;
		const float fVertSize = -1+(i*fSegmentSize);


		iCount++;

		const float fPosY = sinf(fActAngle)*2*fRadius;
		const float fSize = cosf(fActAngle)*2*fRadius;
		const float fPosY2 = sinf(fNextAngle)*2*fRadius;
		const float fSize2 = cosf(fNextAngle)*2*fRadius;

		TGfxVec3 tMoving1 = TGfxVec3( cosf(fActAngle), fVertSize, sinf(fActAngle) );
		TGfxVec3 tMoving2 = TGfxVec3( cosf(fActAngle)*fScale, fVertSize, sinf(fActAngle)*fScale );


		tGeometry.AddLine( TGfxVec3( cosf(fHelixAngle)*fSize, fPosY, sinf(fHelixAngle)*fSize) + tMoving1, TGfxVec3( cosf(fHelixAngle)*fSize2,  fPosY2, sinf(fHelixAngle)*fSize2) + tMoving1);
		tGeometry.AddLine( TGfxVec3( cosf(fHelixAngle)*fSize, sinf(fHelixAngle)*fSize, fPosY) + tMoving2, TGfxVec3( cosf(fHelixAngle)*fSize2, sinf(fHelixAngle)*fSize2,  fPosY2) + tMoving2);

		if(iCount == iLinkStep)
		{
			tGeometry.AddLine( TGfxVec3( cosf(fHelixAngle)*fSize, fPosY, sinf(fHelixAngle)*fSize) + tMoving1, TGfxVec3( cosf(fHelixAngle)*fSize, sinf(fHelixAngle)*fSize, fPosY) +tMoving2);
		iCount = 0;
		}
	}
}

void BuildBrickWall(TGeometry & tGeometry, int iRowCount, int iBrickCount, float fWallThickness)
{
	const float fRowSize = 2.0f/iRowCount;
	const float fBrickSize = (2.0f-fWallThickness) / iBrickCount;
	int iSigne = -1;

	for (int i = 0; i <= iRowCount; i++)
	{
		const float fY = -1 +(i * fRowSize);

		if(i!=0 && i!= iRowCount)
		{
			tGeometry.AddLine( TGfxVec3( 1, fY, 1 ), TGfxVec3( 1, fY, -1 ) );
			tGeometry.AddLine( TGfxVec3( 1, fY, -1 ), TGfxVec3( -1, fY, -1 ) );
			tGeometry.AddLine( TGfxVec3( -1, fY, -1 ), TGfxVec3( -1, fY, 1 ) );
			tGeometry.AddLine( TGfxVec3( -1, fY, 1 ), TGfxVec3( 1, fY, 1 ) );
		}

		const float fThickness = 1-(fWallThickness);

		tGeometry.AddLine( TGfxVec3( fThickness, fY, fThickness ), TGfxVec3( fThickness, fY, -fThickness ) );
	    tGeometry.AddLine( TGfxVec3( fThickness, fY, -fThickness ), TGfxVec3( -fThickness, fY, -fThickness ) );
	    tGeometry.AddLine( TGfxVec3( -fThickness, fY, -fThickness ), TGfxVec3( -fThickness, fY, fThickness ) );
	    tGeometry.AddLine( TGfxVec3( -fThickness, fY, fThickness ), TGfxVec3( fThickness, fY, fThickness ) );


		
		iSigne *= -1;

		for (int j = 0; j<= iBrickCount; j++)
	    {
			float fLenght = -1+(j * fBrickSize);
			const float fY2 = fY + fRowSize;
			fLenght *= iSigne;

			if(i!= iRowCount)
			{
				if(j!=0 && j!= iBrickCount)
				{
					tGeometry.AddLine( TGfxVec3( fLenght, fY, 1 ), TGfxVec3( fLenght, fY2, 1 ) );
					tGeometry.AddLine( TGfxVec3( -fLenght, fY, -1 ), TGfxVec3( -fLenght, fY2, -1 ) );
					tGeometry.AddLine( TGfxVec3( 1, fY, -fLenght ), TGfxVec3( 1, fY2, -fLenght ) );
					tGeometry.AddLine( TGfxVec3( -1, fY, fLenght ), TGfxVec3( -1, fY2, fLenght ) );
				}

				tGeometry.AddLine( TGfxVec3( fLenght, fY, fThickness ), TGfxVec3( fLenght, fY2, fThickness ) );
				tGeometry.AddLine( TGfxVec3( -fLenght, fY, -fThickness ), TGfxVec3( -fLenght, fY2, -fThickness ) );
				tGeometry.AddLine( TGfxVec3( fThickness, fY, -fLenght ), TGfxVec3( fThickness, fY2, -fLenght ) );
				tGeometry.AddLine( TGfxVec3( -fThickness, fY, fLenght ), TGfxVec3( -fThickness, fY2, fLenght ) );

				tGeometry.AddLine( TGfxVec3( fLenght, fY, 1 ), TGfxVec3( fLenght, fY, fThickness ) );
				tGeometry.AddLine( TGfxVec3( -fLenght, fY, -1 ), TGfxVec3( -fLenght, fY, -fThickness ) );
				tGeometry.AddLine( TGfxVec3( 1, fY, -fLenght ), TGfxVec3( fThickness, fY, -fLenght ) );
				tGeometry.AddLine( TGfxVec3( -1, fY, fLenght ), TGfxVec3( -fThickness, fY, fLenght ) );

				tGeometry.AddLine( TGfxVec3( fLenght, fY2, 1 ), TGfxVec3( fLenght, fY2, fThickness ) );
				tGeometry.AddLine( TGfxVec3( -fLenght, fY2, -1 ), TGfxVec3( -fLenght, fY2, -fThickness ) );
				tGeometry.AddLine( TGfxVec3( 1, fY2, -fLenght ), TGfxVec3( fThickness, fY2, -fLenght ) );
				tGeometry.AddLine( TGfxVec3( -1, fY2, fLenght ), TGfxVec3( -fThickness, fY2, fLenght ) );
			}
	    }
	}

	
}
