#include "artt.h"

AMatrix & AMatrix::scaleXYZ(const AVector &v)
{
	m[0] *= v.x; m[3] *= v.x; m[6] *= v.x;
	m[1] *= v.y; m[4] *= v.y; m[7] *= v.y;
	m[2] *= v.z; m[5] *= v.z; m[8] *= v.z;

	return *this;
}

AVector operator*(const AVector &v, const AMatrix &m)
{
	return AVector(
		(v.x*m.m[0]) + (v.y*m.m[3]) + (v.z*m.m[6]),
		(v.x*m.m[1]) + (v.y*m.m[4]) + (v.z*m.m[7]),
		(v.x*m.m[2]) + (v.y*m.m[5]) + (v.z*m.m[8]));
}

AMatrix operator*(const AMatrix &a, const AMatrix &b)
{
	AMatrix r;
	r.m[0] = (a.m[0] * b.m[0]) + (a.m[1] * b.m[3]) + (a.m[2] * b.m[6]);
	r.m[1] = (a.m[0] * b.m[1]) + (a.m[1] * b.m[4]) + (a.m[2] * b.m[7]);
	r.m[2] = (a.m[0] * b.m[2]) + (a.m[1] * b.m[5]) + (a.m[2] * b.m[8]);
	r.m[3] = (a.m[3] * b.m[0]) + (a.m[4] * b.m[3]) + (a.m[5] * b.m[6]);
	r.m[4] = (a.m[3] * b.m[1]) + (a.m[4] * b.m[4]) + (a.m[5] * b.m[7]);
	r.m[5] = (a.m[3] * b.m[2]) + (a.m[4] * b.m[5]) + (a.m[5] * b.m[8]);
	r.m[6] = (a.m[6] * b.m[0]) + (a.m[7] * b.m[3]) + (a.m[8] * b.m[6]);
	r.m[7] = (a.m[6] * b.m[1]) + (a.m[7] * b.m[4]) + (a.m[8] * b.m[7]);
	r.m[8] = (a.m[6] * b.m[2]) + (a.m[7] * b.m[5]) + (a.m[8] * b.m[8]);

	return r;
}


/*
*/

void ACamera::CalcViewMatrix()
{
	AVector rdir(-dir.x, -dir.y, -dir.z);

	// Combine all three rotations all at once
	m.m[0] = cos(rdir.y) * cos(rdir.z);
	m.m[1] = cos(rdir.y)*sin(rdir.z);
	m.m[2] = -sin(rdir.y);

	m.m[3] = (sin(rdir.x)*sin(rdir.y)*cos(rdir.z)) - (cos(rdir.x)*sin(rdir.z));
	m.m[4] = (sin(rdir.x)*sin(rdir.y)*sin(rdir.z)) + (cos(rdir.x)*cos(rdir.z));
	m.m[5] = sin(rdir.x)*cos(rdir.y);

	m.m[6] = (cos(rdir.x)*sin(rdir.y)*cos(rdir.z)) + (sin(rdir.x)*sin(rdir.z));
	m.m[7] = (cos(rdir.x)*sin(rdir.y)*sin(rdir.z)) - (sin(rdir.x)*cos(rdir.z));
	m.m[8] = cos(rdir.x)*cos(rdir.y);

	// scale by viewport apsect ratio and field of view
	AVector vscale(1.0*viewportAspect, 1.0, fov / FOV_90);
	m.scaleXYZ(vscale);
}

void ACamera::worldToView(const AVector &ptsrc, AVector &ptdest)
{
	ptdest = (ptsrc - loc) * m;		// translate, then rotate/scale;
}

bool ACamera::project(const AVector &ptsrc, APolyVertex &pvdest)
{
	// if behind camera, then reject it
	if (ptsrc.z >= 0) {
		return false;
	}

	pvdest.x = ((ptsrc.x / -ptsrc.z)*viewportW) + (viewportW / 2) + 0.5;
	pvdest.y = viewportH - (((ptsrc.y / -ptsrc.z)*viewportH) + (viewportH / 2) + 0.5);

	return true;
}

bool ACamera::isFacing(APolyVertex &v1, APolyVertex &v2, APolyVertex &v3)
{
	// Compute z-compontent of cross product
	// if < 0, then polygon is facing 'forward' (toward camera)
	float x1 = v3.x - v2.x;
	float x2 = v1.x - v2.x;
	float y1 = v3.y = v2.y;
	float y2 = v1.y - v2.y;

	return ((x1*y2 - y1*x2) < 0);
}


/*
	APolygon
*/
int APolygon::findTopmostVertex() const
{
	real ymin(32767);
	int vmin;

	for (int i = 0; i < numVerts; i++){
		if (pVerts[vertIndex[i]].y < ymin)
		{
			ymin = pVerts[vertIndex[i]].y;
			vmin = i;
		}
	}

	return vmin;
}

struct APolyDda {
	short vertIndex;
	short vertNext;
	real x;
	real dx;
	short ybeg;
	short yend;
	// for texture mapping
	//ATmapCoord tc;
	//ATmapCoord tcDelta;

	void setupPolyDda(const APolygon &poly, short ivert, int dir){
		vertIndex = ivert;
		vertNext = ivert + dir;
		if (vertNext < 0) {
			vertNext = poly.numVerts - 1;
		}
		else if (vertNext == poly.numVerts){
			vertNext = 0;
		}

		// set starting/ending ypos and current xpos
		ybeg = yend;
		yend = round(poly.pVerts[poly.vertIndex[vertNext]].y);
		x = poly.pVerts[poly.vertIndex[vertIndex]].x;

		// Calculate fractional number of pixels to step in x (dx)
		real xdelta = poly.pVerts[poly.vertIndex[vertNext]].x -
			poly.pVerts[poly.vertIndex[vertIndex]].x;
		int ydelta = yend - ybeg;
		if (ydelta > 0) {
			dx = xdelta / ydelta;
		}
		else {
			dx = 0;
		}
	}
};


/*
	ACanvas
*/

ACanvas::ACanvas(pb_rgba *pbm)
{
	bm = pbm;
	setClipRect(pbm->frame);
	colorValue = CBlack;
}



void ACanvas::drawFlatConvexPolygon(const APolygon &poly)
{
	// find topmost vertex of the polygon
	int vmin = poly.findTopmostVertex();

	// set starting line
	APolyDda ldda, rdda;
	int y = int(poly.pVerts[poly.vertIndex[vmin]].y);
	ldda.yend = rdda.yend = y;

	// setup polygon scanner for left side, starting from top
	ldda.setupPolyDda(poly, vmin, +1);

	// setup polygon scanner for right side, starting from top
	rdda.setupPolyDda(poly, vmin, -1);

	setColor(poly.colorNative);

	while (true)
	{
		if (y >= ldda.yend)
		{
			if (y >= rdda.yend)
			{
				if (ldda.vertNext == rdda.vertNext)	{ // if same vertex, then done
					break;
				}

				int vnext = rdda.vertNext - 1;

				if (vnext < 0) {
					vnext = poly.numVerts - 1;
				}

				if (vnext == ldda.vertNext)
				{
					break;
				}
			}
			ldda.setupPolyDda(poly, ldda.vertNext, +1);	// reset left side
		}

		// check for right dda hitting end of polygon side
		// if so, reset scanner
		if (y >= rdda.yend) {
			rdda.setupPolyDda(poly, rdda.vertNext, -1);
		}

		// fill span between two line-drawers, advance drawers when
		// hit vertices
		if (y >= clipRect.y) {
			drawHorizontalLine(y, round(ldda.x), round(rdda.x));
		}

		ldda.x += ldda.dx;
		rdda.x += rdda.dx;

		// Advance y position.  Exit if run off its bottom
		y += 1;
		if (y >= clipRect.y + clipRect.height)
		{
			break;
		}
	}
}
