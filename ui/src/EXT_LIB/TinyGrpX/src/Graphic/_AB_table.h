#ifndef ___AB_TABLE_H_
#define ___AB_TABLE_H_

#include "GrpX.h"

#ifdef atGRPX_USES_ALPHABLEND

extern unsigned long blend565tab[33][2][256];

#define __ALPHA_BLEND565(dst, src, rA, rInvA, nA, nInvA, sum) \
{ \
	nA = REALTOINT(rA * 32); \
	nInvA = REALTOINT(rInvA * 32); \
	sum = *(dst); \
    sum = blend565tab[nA][0][src & 0xff] + blend565tab[nA][1][src >> 8] + blend565tab[nInvA][0][sum & 0xff] + blend565tab[nInvA][1][sum >> 8]; \
	sum &= 0xF81F07E0; \
	*(dst) = (atUSHORT)((sum & 0xffff) + (sum >> 16)); \
}

#endif

#endif
