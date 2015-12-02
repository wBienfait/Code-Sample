#ifndef BUILD_GEOMETRIES_H_INCLUDED
#define BUILD_GEOMETRIES_H_INCLUDED

class TGeometry;

void BuildBox(TGeometry & tGeometry);
void BuildOrbit(TGeometry & tGeometry, int iTwistAngle, int iSegments, int iCircleCount);
void BuildHoneycomb(TGeometry & tGeometry, int iRows, int iColumns);
void BuildHelix(TGeometry & tGeometry, float fTurnCount, int iTwistCount, float fRadius, int iSegmentsPerTurn, int iLinkStep);
void BuildBrickWall(TGeometry & tGeometry, int iRowCount, int iBrickCount, float fWallThickness);
#endif
