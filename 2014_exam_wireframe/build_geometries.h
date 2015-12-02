#ifndef BUILD_GEOMETRIES_H_INCLUDED
#define BUILD_GEOMETRIES_H_INCLUDED

class TGeometry;

void BuildBox( TGeometry & tGeometry );
void BuildSphere( TGeometry & tGeometry, int iSegments, int iDoubleMeridianCount, int iParallelsCount );
void BuildPlane( TGeometry & tGeometry, int iTileCountX, int iTileCountZ );
void BuildDoughnut( TGeometry & tGeometry, int iSegments, float fRadius );
void BuildPyramid( TGeometry & tGeometry, int iSegments );
void BuildHelicoid( TGeometry & tGeometry, int iSegments, int iTwist );

#endif
