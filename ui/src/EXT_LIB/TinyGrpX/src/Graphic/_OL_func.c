/*#############################################################################
-------------------------------------------------------------------------------

                                  _OL_Func.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : Line function definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "GrpX.h"
#include "A_Math.h"
#include "_OL_func.h"


#define INT_HALF(a) ((a) >> 1)

#ifdef __FIXEDPTR_
#define REAL_HALF(a) INT_HALF(a)
#else
#define REAL_HALF(a) ((a) / 2)
#endif


atREAL __atGRPX_GetOutlinePtrs( atINT kiL, atINT kiT, atINT kiR, atINT kiB, atINT nWidth, atPOINT pkPtOut[], atBOOL isSExt, atBOOL isEExt )
{
	atREAL R1, R2;
	atREAL CosR1, SinR1;
	atREAL wCosR1, wSinR1;
	atREAL CosR2, SinR2;
	atREAL wCosR2, wSinR2;
	atINT kw = kiR - kiL;
	atINT kh = kiB - kiT;

//	if( (kiT == kiB) && (kiL == kiR) ) return INTTOREAL(360);

	if( kh == (0) ){
	  R1 = (kw >= (0))? INTTOREAL(0) : INTTOREAL(180);
	} else if( kw == (0) ){
	  R1 = (kh > (0))? INTTOREAL(90) : INTTOREAL(270);
	} else {
	  R1 = ATAN2( INTTOREAL(kw), INTTOREAL(kh) );
    }

	R2 = R1 + INTTOREAL(90);

	CosR1 = COS( R1 );
	SinR1 = SIN( R1 );
	CosR2 = COS( R2 );
	SinR2 = SIN( R2 );

/*
	if( nWidth > 1 ){
		wCosR1 = ((nWidth-1) * CosR1);
		wSinR1 = ((nWidth-1) * SinR1);
		wCosR2 = ((nWidth-1) * CosR2);
		wSinR2 = ((nWidth-1) * SinR2);
	}else{
		wCosR1 = REAL_HALF(CosR1);
		wSinR1 = REAL_HALF(SinR1);
		wCosR2 = REAL_HALF(CosR2);
		wSinR2 = REAL_HALF(SinR2);
	}
*/


	wCosR2 = nWidth * CosR2;
	wSinR2 = nWidth * SinR2;

    if( isSExt || isSExt ){
		wCosR1 = nWidth * CosR1;
		wSinR1 = nWidth * SinR1;

		if( isSExt ){
			kiL -= REALTOINT(REAL_HALF( wCosR1 ));
			kiT -= REALTOINT(REAL_HALF( wSinR1 ));
		}
		if( isEExt ){
			kiR += REALTOINT(REAL_HALF( wCosR1 ));
  			kiB += REALTOINT(REAL_HALF( wSinR1 ));
		}
	}

	pkPtOut[0].nX = kiL + REALTOINT(REAL_HALF(wCosR2));
	pkPtOut[0].nY = kiT + REALTOINT(REAL_HALF(wSinR2));
	pkPtOut[1].nX = pkPtOut[0].nX - REALTOINT(wCosR2);
	pkPtOut[1].nY = pkPtOut[0].nY - REALTOINT(wSinR2);
	pkPtOut[3].nX = kiR + REALTOINT(REAL_HALF(wCosR2));
	pkPtOut[3].nY = kiB + REALTOINT(REAL_HALF(wSinR2));
	pkPtOut[2].nX = pkPtOut[3].nX - REALTOINT(wCosR2);
	pkPtOut[2].nY = pkPtOut[3].nY - REALTOINT(wSinR2);

	return R1;
}


atREAL __atGRPX_GetOutlinePtrs_REAL( atREAL kiL, atREAL kiT, atREAL kiR, atREAL kiB, atREAL rWidth, atRPOINT pkPtOut[], atBOOL isSExt, atBOOL isEExt )
{
	atREAL R1, R2;
	atREAL CosR1, SinR1;
	atREAL wCosR1, wSinR1;
	atREAL CosR2, SinR2;
	atREAL wCosR2, wSinR2;
	atREAL kw = kiR - kiL;
	atREAL kh = kiB - kiT;

//	if( (kiT == kiB) && (kiL == kiR) ) return INTTOREAL(360);

	if( kh == INTTOREAL(0) ){
	  R1 = (kw >= INTTOREAL(0))? INTTOREAL(0) : INTTOREAL(180);
	} else if( kw == INTTOREAL(0) ){
	  R1 = (kh > INTTOREAL(0))? INTTOREAL(90) : INTTOREAL(270);
	} else {
	  R1 = ATAN2( kw, kh );
    }

	R2 = R1 + INTTOREAL(90);

	CosR1 = COS( R1 );
	SinR1 = SIN( R1 );
	CosR2 = COS( R2 );
	SinR2 = SIN( R2 );

/*
	if( nWidth > 1 ){
		wCosR1 = ((nWidth-1) * CosR1);
		wSinR1 = ((nWidth-1) * SinR1);
		wCosR2 = ((nWidth-1) * CosR2);
		wSinR2 = ((nWidth-1) * SinR2);
	}else{
		wCosR1 = REAL_HALF(CosR1);
		wSinR1 = REAL_HALF(SinR1);
		wCosR2 = REAL_HALF(CosR2);
		wSinR2 = REAL_HALF(SinR2);
	}
*/


	wCosR1 = REALMUL(rWidth, CosR1);
	wSinR1 = REALMUL(rWidth, SinR1);
	wCosR2 = REALMUL(rWidth, CosR2);
	wSinR2 = REALMUL(rWidth, SinR2);

    if( isSExt ){
	    kiL -= REAL_HALF(wCosR1);
	    kiT -= REAL_HALF(wSinR1);
    }
    if( isEExt ){
	    kiR += REAL_HALF(wCosR1);
  	    kiB += REAL_HALF(wSinR1);
    }

	pkPtOut[0].rX = kiL + REAL_HALF(wCosR2);
	pkPtOut[0].rY = kiT + REAL_HALF(wSinR2);
	pkPtOut[1].rX = pkPtOut[0].rX - wCosR2;
	pkPtOut[1].rY = pkPtOut[0].rY - wSinR2;
	pkPtOut[3].rX = kiR + REAL_HALF(wCosR2);
	pkPtOut[3].rY = kiB + REAL_HALF(wSinR2);
	pkPtOut[2].rX = pkPtOut[3].rX - wCosR2;
	pkPtOut[2].rY = pkPtOut[3].rY - wSinR2;

	return R1;
}
