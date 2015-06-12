/*
	Reference: Geometric Modeling - Michael E Mortenson
	
	Geometric Modeling routines.  In some cases these are thin
	wrappers over graphicc routines.  On others, they are
	specific to this modeling library.
*/
#pragma once

#ifndef GEOMODELING_API_H
#define GEOMODELING_API_H

#include "graphicc.h"

#ifdef __cplusplus
extern "C" {
#endif

// ARCL
// ARCLNS
// ARCNTR
// ARCPC
// AREA
// ATG
// ATGS
// ATGSOL

// BF		Compute blending functions of a pc curve
// BFU
// BFUU
// BLNDC
// BLOCK
// BN
// BSEGBS
// BSEGBZ
// BSEGPC
// BSPLNC
// BSPLNS
// BZCRV
// BZSRF

// CDU
// CNTCRV
// CONSRF
// CRTCRV
// CRVINV
// CRVSRF
// CRV2BS
// CRV2BZ
// CRV2PC
// CUBIC
// CYLSRF
// C4SRF

// DETVAL
void DETVAL(const mat3 &DI, real &VAL);

// EFGLMN

// FPTOG

// GTAS
// GTASOL
// GTOFP
// G16P

// INVCRV
// INVSRF

// LITE
// LNCRV
// LNLN
// LNSRF

// MATINV	Calculate inverse of 4x4 matrix
void MATINV(const mat4 &MI, const mat4 &MO);

// MATRNS - Calculate transpose of 4x4 matrix
void MATRNS(const mat4 &MI, mat4 &MO);

// MIDCRV
// MINDCC
// MINDCS
// MINDPC
// MINDPS
// NORPLN
// ORBIT
// PARPLN
// PATH
// PCCRCL
// PCLASC
// PCLASS
// PCLIN
// PCNC
// PCRSOL
// PCRV
// PCRVA
// PDSURF
// PERPLN
// PLN
// PLNBCS
// PLNBSC
// PLNBSS
// PLNBZC
// PLNBZS
// PLNLN
// PLNPC
// PLNPLN
// PNC
// POCOS
// POICRV
// PPCRV
// PPSRF

// PROJPT
// PRSPRJ
// PSOLID
// PSRF
// PSRFA
// PSRF16
// PSRSOL
// PTGSOL
// PTNCRV
// PUM
// P16G

// RFLCT
// RFLMAG
// ROTAXS
// ROTORG
// RPRCRV
// RPSRF
// RULSRF
// SCALE
// SETOP
// SILOET
// SPCRV
// SPHSRF
// SPLNPC

// SPROD - Dot product
void SPROD(const real3 &V1, const real3 &V2, real &S);

// SRFINV
// SRFREV
// SUBSRF
// TANPLN
// THNET
// THP
// TNBCRV
// TPTP
// TRANSH
// TRNBZC
// TRNCRV
// TRNPC
// TVCRV
// TVSRF
// TWSTV
// UNV		
// VANG		Compute the angle between two vectors
void VANG(const real3 &V1, const real3 &V2, real &A);

// VMAG		Compute the magnitude of a vector
void VMAG(const real3 V, real MAG);

// VPROD	Compute the vector product of two vectors (cross product)
void VPROD(const real3 &V1, const real3 &V2, real3 &V3);

// VSUM	- Compute the sum of two vectors
void VSUM(const real3 &V1, const real3 &V2, real3 &V3);

// VUNIT - Compute unit vector of a vector
void VUNIT(const real3 &V, real3 &U);

#ifdef __cplusplus
}
#endif

#endif
