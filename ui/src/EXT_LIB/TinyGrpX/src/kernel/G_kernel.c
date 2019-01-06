/*#############################################################################
-------------------------------------------------------------------------------

                                  G_Kernel.c
                           (* =================== *)


 Version           : 1.0.0
 Dependent Modules :
 Author            : Ju, Gyeong-min ( Amos )
 Create            : 2001/04/25    01:47:09am
 Update            : 2003/04/29    00:00:00am
 C Compiler        : ADS 1.01, SDT 2.51, Borland C/C++ 5.0, Visual C/C++ 6.0
 Assembler         : ADS 1.01, SDT 2.51, Borland TASM 3.1, MASM 6.0
 Description       : G_Kernel related fucntion definition

-------------------------------------------------------------------------------
#############################################################################*/

#include "TinyGrpX.h"
#include <string.h>


atINT atTGRPX_GetInstanceSize(NOARGS)
{
	return sizeof(atGRPXMNG);
}


atBOOL  atTGRPX_Initialize( atLPGRPXMNG lpGrpX, atINT nWidth, atINT nHeight, atINT nBPP, atVOID *pFrameBufPtr, atINT VMemWidth )
{
	if( lpGrpX == atNULL ) return atFALSE;

	lpGrpX->ScrInfo.nFrameWidth = nWidth;
	lpGrpX->ScrInfo.nFrameHeight = nHeight;
	lpGrpX->ScrInfo.lpFrameBufferPtr = pFrameBufPtr;
	lpGrpX->ScrInfo.nBPP = nBPP;
	lpGrpX->ScrInfo.nVMemWidth = VMemWidth;

	lpGrpX->lpTempBuf = atNULL;

	return atTRUE;
}


atBOOL  atTGRPX_SetFrameBuffer( atLPGRPXMNG lpGrpX, atLPVOID pFrameBufPtr )
{
	if( lpGrpX == atNULL ) return atFALSE;

	lpGrpX->ScrInfo.lpFrameBufferPtr = pFrameBufPtr;

	return atTRUE;
}


atBOOL  atTGRPX_SetScreenResolution( atLPGRPXMNG lpGrpX, atINT nWidth, atINT nHeight, atINT VMemWidth )
{
	if( lpGrpX == atNULL ) return atFALSE;

	lpGrpX->ScrInfo.nFrameWidth = nWidth;
	lpGrpX->ScrInfo.nFrameHeight = nHeight;
	lpGrpX->ScrInfo.nVMemWidth = VMemWidth;

	return atTRUE;
}


void  atTGRPX_GetScreenInfo( atLPGRPXMNG lpGrpX, atGRPXSCRINFO *lpScrInfo )
{
	*lpScrInfo = lpGrpX->ScrInfo;
}


atTGRPX_COLOR atTGRPX_GetRGB( atBYTE r, atBYTE g, atBYTE b )
{
	return MAKERGB( r,g,b );
}

atBYTE atTGRPX_GetRValue( atTGRPX_COLOR nColor )
{
	return GET_RED( nColor );
}

atBYTE atTGRPX_GetGValue( atTGRPX_COLOR nColor )
{
	return GET_GREEN( nColor );
}

atBYTE atTGRPX_GetBValue( atTGRPX_COLOR nColor )
{
	return GET_BLUE( nColor );
}
