#include "AT_Types.h"
#include "_AB_Int.h"
#include "_AA_Func.h"
#include "_BRE_func_samsung.h"


atINT alpha_table[151] = {255	,251	,248	,244	,241	,238	,235	,231	,228	,225	,
						222	,218	,215	,212	,209	,206	,203	,200	,197	,194	,
						191	,188	,185	,182	,179	,177	,174	,171	,168	,165	,
						163	,160	,157	,155	,152	,149	,147	,144	,142	,139	,
						137	,134	,132	,129	,127	,124	,122	,120	,117	,115	,
						113	,111	,108	,106	,104	,102	,100	,98	,95	,93	,
						91	,89	,87	,85	,83	,81	,79	,78	,76	,74	,
						72	,70	,68	,67	,65	,63	,62	,60	,58	,57	,
						55	,53	,52	,50	,49	,47	,46	,44	,43	,42	,
						40	,39	,38	,36	,35	,34	,33	,31	,30	,29	,
						28	,27	,26	,25	,23	,22	,21	,20	,19	,19	,
						18	,17	,16	,15	,14	,13	,13	,12	,11	,10	,
						10	,9	,8	,8	,7	,7	,6	,5	,5	,4	,
						4	,4	,3	,3	,2	,2	,2	,1	,1	,1	,
						1	,0	,0	,0	,0	,0	,0	,0	,0	,0	,0};

atINT round_table_1[1] = 
{	255};
atINT round_table_2[4] = {
183,	183,
183,	183}; 
atINT round_table_3[9] = 
{	132,	255,	132,
243,	255,	243,
135,	255,	135};
atINT round_table_4[16] = {
78,		219,	219,	78,
219,	255,	255,	219,
219,	255,	255,	219,
78,		219,	219,	78}; 
atINT round_table_5[25] = 
{	27,		177,	249,	177,	27,
186,	255,	255,	255,	192,
243,	255,	255,	255,	249,
186,	255,	255,	255,	192,
30,		183,	255,	183,	30};
atINT round_table_6[36]	= {
9,		153,	237,	237,	153,	9,
138,	255,	255,	255,	255,	138,
243,	255,	255,	255,	255,	243,
243,	255,	255,	255,	255,	243,
138,	255,	255,	255,	255,	138,
9,		153,	255,	255,	153,	9};
atINT round_table_7[49] = 
{	0,		72,		198,	249,	198,	72,		0,
75,		255,	255,	255,	255,	255,	84,
210,	255,	255,	255,	255,	255,	210,
249,	255,	255,	255,	255,	255,	249,
210,	255,	255,	255,	255,	255,	210,
78,		255,	255,	255,	255,	255,	84,
0,		81,		198,	255,	198,	72,		0};
atINT round_table_8[64] = {
0,		33,		177,	237,	237,	177,	33,		0,
42,		240,	255,	255,	255,	255,	240,	42,
177,	255,	255,	255,	255,	255,	255,	177,
243,	255,	255,	255,	255,	255,	255,	243,
243,	255,	255,	255,	255,	255,	255,	243,
177,	255,	255,	255,	255,	255,	255,	177,
42,		240,	255,	255,	255,	255,	240,	42,
0,		33,		177,	237,	237,	177,	33,		0};
atINT round_table_9[81] = 
{	0,		3,		114,	204,	252,	204,	114,	3,		0,
3,		192,	255,	255,	255,	255,	255,	192,	3,
123,	255,	255,	255,	255,	255,	255,	255,	123,
222,	255,	255,	255,	255,	255,	255,	255,	222,
249,	255,	255,	255,	255,	255,	255,	255,	249,
222,	255,	255,	255,	255,	255,	255,	255,	132,
132,	255,	255,	255,	255,	255,	255,	255,	255,
9,		198,	255,	255,	255,	255,	255,	201,	9,
0,		3,		117,	207,	255,	207,	117,	3,		0};
atINT round_table_10[100] = {
0,		0,		81,		183,	237,	237,	183,	81,		0,		0,
0,		132,	255,	255,	255,	255,	255,	255,	132,	0,
63,		255,	255,	255,	255,	255,	255,	255,	255,	63,
195,	255,	255,	255,	255,	255,	255,	255,	255,	195,
243,	255,	255,	255,	255,	255,	255,	255,	255,	243,
243,	255,	255,	255,	255,	255,	255,	255,	255,	243,
195,	255,	255,	255,	255,	255,	255,	255,	255,	195,
63,		255,	255,	255,	255,	255,	255,	255,	255,	63,
0,		132,	255,	255,	255,	255,	255,	255,	132,	0,
0,		0,		81,		183,	237,	237,	183,	81,		0,		0};
atINT round_table_11[121] = 
{	0,		0,		30,		150,	225,	255,	225,	153,	30,		0,		0,
0,		69,		243,	255,	255,	255,	255,	255,	243,	69,		0,
30,		240,	255,	255,	255,	255,	255,	255,	255,	240,	30,
147,	255,	255,	255,	255,	255,	255,	255,	255,	255,	147,
228,	255,	255,	255,	255,	255,	255,	255,	255,	255,	228,
249,	255,	255,	255,	255,	255,	255,	255,	255,	255,	249,
228,	255,	255,	255,	255,	255,	255,	255,	255,	255,	228,
150,	255,	255,	255,	255,	255,	255,	255,	255,	255,	150,
33,		243,	255,	255,	255,	255,	255,	255,	255,	243,	33,
0,		69,		243,	255,	255,	255,	255,	255,	243,	69,		0,
0,		0,		33,		150,	225,	255,	225,	153,	30,		0,		0};
atINT round_table_12[144] = {
0,		0,		9,		114,	210,	255,	255,	210,	114,	9,		0,		0,
0,		27,		210,	255,	255,	255,	255,	255,	255,	210,	27,		0,
6,		210,	255,	255,	255,	255,	255,	255,	255,	255,	210,	6,
114,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	114,
207,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	207,
249,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	249,
249,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	249,
207,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	207,
114,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	114,
6,		210,	255,	255,	255,	255,	255,	255,	255,	255,	210,	6,
0,		27,		210,	255,	255,	255,	255,	255,	255,	210,	27,		0,
0,		0,		9,		114,	210,	255,	255,	210,	114,	9,		0,		0};
atINT round_table_13[169] = 
{	0,		0,		0,		45,		171,	228,	255,	228,	171,	45,		0,		0,		0,
0,		6,		123,	249,	255,	255,	255,	255,	255,	249,	123,	6,		0,
0,		147,	255,	255,	255,	255,	255,	255,	255,	255,	255,	147,	0,
60,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	60,
165,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	165,
228,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	228,
249,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	249,
228,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	228,
165,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	165,
60,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	60,
0,		147,	255,	255,	255,	255,	255,	255,	255,	255,	255,	147,	0,
0,		6,		132,	249,	255,	255,	255,	255,	255,	249,	123,	6,		0,
0,		0,		0,		45,		171,	228,	255,	228,	171,	45,		0,		0,		0};
atINT round_table_14[196] = {
0,		0,		0,		24,		135,	216,	255,	255,	216,	135,	24,		0,		0,		0,
0,		0,		90,		243,	255,	255,	255,	255,	255,	255,	243,	90,		0,		0,
0,		87,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	87,		0,
24,		243,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	243,	24,
132,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	132,
207,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	207,
249,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	249,
249,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	249,
207,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	207,
132,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	132,
24,		243,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	243,	24,
0,		87,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
0,		0,		90,		243,	255,	255,	255,	255,	255,	255,	243,	90,		0,		0,
0,		0,		0,		24,		135,	216,	255,	255,	216,	135,	24,		0,		0,		0};
atINT round_table_15[225] = 
{	0,		0,		0,		0,		75,		150,	201,	249,	201,	150,	75,		0,		0,		0,		0,
0,		0,		21,		174,	255,	255,	255,	255,	255,	255,	255,	174,	21,		0,		0,
0,		24,		231,	255,	255,	255,	255,	255,	255,	255,	255,	255,	231,	24,		0,
0,		174,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	174,	0,
87,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	87,
180,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	180,
231,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	231,
249,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	249,
231,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	231,
180,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	180,
87,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	87,
0,		174,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	174,	0,
0,		24,		231,	255,	255,	255,	255,	255,	255,	255,	255,	255,	231,	24,		0,
0,		0,		21,		174,	255,	255,	255,	255,	255,	255,	255,	174,	21,		0,		0,
0,		0,		0,		0,		75,		150,	201,	249,	201,	150,	75,		0,		0,		0,		0};
atINT round_table_16[256] = {
0,		0,		0,		0,		39,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,
255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255};
atINT round_table_17[289] = 
{	0,	0,		0,		0,		12,		108,	183,	225,	255,	225,	183,	108,	12,		0,		0,		0,		0,
0,		0,		0,		81,		231,	255,	255,	255,	255,	255,	255,	255,	231,	81,		0,		0,		0,
0,		0,		120,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	120,	0,		0,
0,		87,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	87,		0,
12,		228,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	228,	12,
105,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	105,
186,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	186,
231,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	231,
249,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	249,
231,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	231,
186,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	186,
105,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	105,
12,		228,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	228,	12,
0,		87,		255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	87,		0,
0,		0,		120,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	255,	120,	0,		0,
0,		0,		0,		81,		231,	255,	255,	255,	255,	255,	255,	255,	231,	81,		0,		0,		0,
0,		0,		0,		0,		12,		108,	183,	225,	255,	225,	183,	108,	12,		0,		0,		0,		0};

//한번에 3픽셀
atVOID	atGRPX_DrawPixelBre( atGRPX_PIXEL* addr, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance)
{
	atGRPX_PIXEL *ptr;
	atGRPX_PIXEL *ptr_inc_1;
	atGRPX_PIXEL *ptr_inc_2;
	atLONG cl, r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[ABS(diff_distance)];
	atLONG nAlpha1 = alpha_table[ABS(invD2du - diff_distance)];
	atLONG nAlpha2 = alpha_table[ABS(invD2du + diff_distance)];
	
	ptr = (atGRPX_PIXEL*)addr;
	
	if( nAlpha0 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha0;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr, r1,g1,b1, cl,r2,g2,b2, nAlpha0,nAlpha_temp);
	}
	
	//픽셀 포인트 설정
	ptr_inc_1 = ptr + inc;
	ptr_inc_2 = ptr - inc;
	
	//inc
	if( nAlpha1 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_inc_1, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha1;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr_inc_1, r1,g1,b1, cl,r2,g2,b2, nAlpha1,nAlpha_temp);
	}
	
	//-inc
	if( nAlpha2 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_inc_2, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha2;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr_inc_2, r1,g1,b1, cl,r2,g2,b2, nAlpha2,nAlpha_temp);
	}
	
}
//atGRPX_DrawPixelBre의 미러 버젼(한번에 6픽셀)
atVOID	atGRPX_DrawPixelBreMir( atGRPX_PIXEL* addr_s, atGRPX_PIXEL* addr_e, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance)
{
	atGRPX_PIXEL *ptr_s;
	atGRPX_PIXEL *ptr_e;
	atGRPX_PIXEL *ptr_s_temp;
	atGRPX_PIXEL *ptr_e_temp;
	
	atLONG cl, r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[ABS(diff_distance)];
	atLONG nAlpha1 = alpha_table[ABS(invD2du - diff_distance)];
	atLONG nAlpha2 = alpha_table[ABS(invD2du + diff_distance)];

	// RGB 컬러 값 추출
	r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
	
	
	//가운데
	ptr_s = (atGRPX_PIXEL*)addr_s;
	ptr_e = (atGRPX_PIXEL*)addr_e;
	
	if( nAlpha0 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_s, (atGRPX_PIXEL)Color);
		__ALPHA_PIXEL_50P(ptr_e, (atGRPX_PIXEL)Color);
	} else {
 		nAlpha_temp = 0xFF - nAlpha0;
		__ALPHA_PIXEL_I(ptr_s, r1,g1,b1, cl,r2,g2,b2, nAlpha0,nAlpha_temp);
		__ALPHA_PIXEL_I(ptr_e, r1,g1,b1, cl,r2,g2,b2, nAlpha0,nAlpha_temp);
	}
	
	//inc
	ptr_s_temp = ptr_s + inc;
	ptr_e_temp = ptr_e - inc;
	
	if( nAlpha1 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_s_temp, (atGRPX_PIXEL)Color);
		__ALPHA_PIXEL_50P(ptr_e_temp, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha1;
		__ALPHA_PIXEL_I(ptr_s_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha1,nAlpha_temp);
		__ALPHA_PIXEL_I(ptr_e_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha1,nAlpha_temp);
	}
	
	r2 = GET_RED(*ptr_s_temp); g2 = GET_GREEN(*ptr_s_temp); b2 = GET_BLUE(*ptr_s_temp);

	//-inc
	ptr_s_temp = ptr_s - inc;
	ptr_e_temp = ptr_e + inc;
	
	if( nAlpha2 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_s_temp, (atGRPX_PIXEL)Color);
		__ALPHA_PIXEL_50P(ptr_e_temp, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha2;
		__ALPHA_PIXEL_I(ptr_s_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha2,nAlpha_temp);
		__ALPHA_PIXEL_I(ptr_e_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha2,nAlpha_temp);
	}	
}


//가로
atVOID	atGRPX_DrawPixelBreH( atGRPX_CANVAS *lpCanvas, atLONG draw_x, atLONG draw_y, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance)
{
	atRECT *clip;
	atGRPX_PIXEL *ptr;
	atGRPX_PIXEL *ptr_inc_1;
	atGRPX_PIXEL *ptr_inc_2;
	atLONG cl, r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[diff_distance];
	atLONG nAlpha1 = alpha_table[invD2du - diff_distance];
	atLONG nAlpha2 = alpha_table[invD2du + diff_distance];
	
	
	//	if( lpCanvas == atNULL ) return;
	
	//clipping
	clip = atGRPX_GetClipRect( lpCanvas );
	if( draw_x < clip->nStartX || draw_x > clip->nEndX || draw_y < clip->nStartY || draw_y > clip->nEndY ) return;
	
	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, draw_x, draw_y );
	
	
	//중간 픽셀 출력
	*(ptr) = (atGRPX_PIXEL)Color;
	
	//픽셀 포인트 설정
	ptr_inc_1 = ptr + inc;
	ptr_inc_2 = ptr - inc;
	
	//inc
	if( nAlpha1 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_inc_1, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha1;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr_inc_1, r1,g1,b1, cl,r2,g2,b2, nAlpha1,nAlpha_temp);
	}
	
	//-inc
	if( nAlpha2 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_inc_2, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha2;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr_inc_2, r1,g1,b1, cl,r2,g2,b2, nAlpha2,nAlpha_temp);
	}
	
}

//세로
atVOID	atGRPX_DrawPixelBreV( atGRPX_CANVAS *lpCanvas, atLONG draw_x, atLONG draw_y, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance)
{
	atRECT *clip;
	atGRPX_PIXEL *ptr;
	atGRPX_PIXEL *ptr_inc_1;
	atGRPX_PIXEL *ptr_inc_2;
	atLONG cl, r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[diff_distance];
	atLONG nAlpha1 = alpha_table[invD2du - diff_distance];
	atLONG nAlpha2 = alpha_table[invD2du + diff_distance];
	
	if( lpCanvas == atNULL ) return;
	
	//clipping
	clip = atGRPX_GetClipRect( lpCanvas );
	if( draw_x < clip->nStartX || draw_x > clip->nEndX || draw_y < clip->nStartY || draw_y > clip->nEndY ) return;
	
	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, draw_x, draw_y );
	
	//중간 픽셀 출력
	*(ptr) = (atGRPX_PIXEL)Color;
	//inc
	
	//픽셀 포인트 설정
	ptr_inc_1 = ptr + (inc * lpCanvas->nVMemWidth);
	ptr_inc_2 = ptr - (inc * lpCanvas->nVMemWidth);
	
	//inc
	if( nAlpha1 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_inc_1, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha1;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr_inc_1, r1,g1,b1, cl,r2,g2,b2, nAlpha1,nAlpha_temp);
	}
	
	//-inc
	if( nAlpha2 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_inc_2, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha2;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr_inc_2, r1,g1,b1, cl,r2,g2,b2, nAlpha2,nAlpha_temp);
	}
	
}

// 한픽셀만 그리는 함수(좌표)
atVOID	atGRPX_DrawPixelBrePixel( atGRPX_CANVAS *lpCanvas, atLONG draw_x, atLONG draw_y, atGRPX_COLOR Color, atLONG diff_distance, atRECT* clip)
{
	atGRPX_PIXEL *ptr;
	atLONG cl, r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[ABS(diff_distance)];
	
	if( lpCanvas == atNULL ) return;
		
	if( draw_x < clip->nStartX || draw_x > clip->nEndX || draw_y < clip->nStartY || draw_y > clip->nEndY )
	{
		return;
	}
	
	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, draw_x, draw_y );
	
	//중간 픽셀 출력
	if( nAlpha0 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha0;
		r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
		__ALPHA_PIXEL_I(ptr, r1,g1,b1, cl,r2,g2,b2, nAlpha0,nAlpha_temp);
	}
	
}
// 한 픽셀만 찍는다. 단 알파값을 계산할 배경색을 같이 넣어 준다.(배경 = BkColor, 그릴 색 = Color)
atVOID	atGRPX_DrawPixelBrePixel_2( atGRPX_CANVAS *lpCanvas, atLONG draw_x, atLONG draw_y, atGRPX_COLOR Color, atGRPX_COLOR BkColor, atLONG diff_distance, atRECT* clip)
{
	atGRPX_PIXEL *ptr;
	atLONG r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[ABS(diff_distance)];
	
	if( lpCanvas == atNULL ) return;
	
	if( draw_x < clip->nStartX || draw_x > clip->nEndX || draw_y < clip->nStartY || draw_y > clip->nEndY ) return;
	
	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, draw_x, draw_y );
	
	//중간 픽셀 출력
	nAlpha_temp = 0xFF - nAlpha0;
	r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
	r2 = GET_RED(BkColor); g2 = GET_GREEN(BkColor); b2 = GET_BLUE(BkColor);
	__ALPHA_PIXEL_I_2(ptr, r1,g1,b1, r2,g2,b2, nAlpha0);
	
}

// 한 픽셀만 그리는 함수(주소 이용)
atVOID	atGRPX_DrawPixelBrePixelMem( atGRPX_PIXEL* addr_s, atLONG inc, atGRPX_COLOR Color, atLONG diff_distance)
{
	atGRPX_PIXEL *ptr_s;
	
	atLONG cl, r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[ABS(diff_distance)];
	
	// RGB 컬러 값 추출
	r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
	
	//가운데
	ptr_s = (atGRPX_PIXEL*)addr_s;
	
	if( nAlpha0 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_s, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha0;
		__ALPHA_PIXEL_I(ptr_s, r1,g1,b1, cl,r2,g2,b2, nAlpha0,nAlpha_temp);
	}
}

// atGRPX_DrawPixelBre의 미러 버젼을 이용하여 항상 방향의 좌측만 알파를 먹임... Edge에 사용.(한번에 4픽셀)
atVOID	atGRPX_DrawPixelBreMirEdgeMem( atGRPX_PIXEL* addr_s, atGRPX_PIXEL* addr_e, atLONG inc, atGRPX_COLOR Color, atGRPX_COLOR BkColor, atLONG mid_alpha, atLONG out_alpha_1, atLONG out_alpha_2)
{
	atGRPX_PIXEL *ptr_s;
	atGRPX_PIXEL *ptr_e;
	atGRPX_PIXEL *ptr_s_temp;
	atGRPX_PIXEL *ptr_e_temp;
	
	atLONG cl, r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[ABS(mid_alpha)];
	atLONG nAlpha1 = alpha_table[ABS(out_alpha_1)];
	atLONG nAlpha2 = alpha_table[ABS(out_alpha_2)];
	
	// RGB 컬러 값 추출
	r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
	
	//가운데
	ptr_s = (atGRPX_PIXEL*)addr_s;
	ptr_e = (atGRPX_PIXEL*)addr_e;

	ptr_s_temp = ptr_s + inc;
	ptr_e_temp = ptr_e + inc;
	
	if( nAlpha0 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_s_temp, (atGRPX_PIXEL)Color);
		__ALPHA_PIXEL_50P(ptr_e_temp, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha0;
		__ALPHA_PIXEL_I(ptr_s_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha0,nAlpha_temp);
		__ALPHA_PIXEL_I(ptr_e_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha0,nAlpha_temp);
	}

	ptr_s_temp = ptr_s + inc + inc;
	ptr_e_temp = ptr_e + inc + inc;
	
	if( nAlpha1 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_s_temp, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha1;
		__ALPHA_PIXEL_I(ptr_s_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha1,nAlpha_temp);
	}

	if( nAlpha2 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_e_temp, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha2;
		__ALPHA_PIXEL_I(ptr_e_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha2,nAlpha_temp);
	}
}
// atGRPX_DrawPixelBre의 이용하여 항상 방향의 좌측만 알파를 먹임... Edge에 사용.(한번에 2픽셀)
atVOID	atGRPX_DrawPixelBreEdgeMem( atGRPX_PIXEL* addr_s, atLONG inc, atGRPX_COLOR Color, atGRPX_COLOR BkColor, atLONG mid_alpha, atLONG out_alpha_1)
{
	atGRPX_PIXEL *ptr_s;
	atGRPX_PIXEL *ptr_s_temp;
		
	atLONG cl, r1, g1, b1, r2, g2, b2;
	atLONG nAlpha_temp;
	
	atLONG nAlpha0 = alpha_table[ABS(mid_alpha)];
	atLONG nAlpha1 = alpha_table[ABS(out_alpha_1)];
		
	// RGB 컬러 값 추출
	r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
	
	//가운데
	ptr_s = (atGRPX_PIXEL*)addr_s;
		
	ptr_s_temp = ptr_s + inc;
		
	if( nAlpha0 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_s_temp, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha0;
		__ALPHA_PIXEL_I(ptr_s_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha0,nAlpha_temp);
	}
	
	ptr_s_temp = ptr_s + inc + inc;
		
	if( nAlpha1 == (0xFF / 2) ){
		__ALPHA_PIXEL_50P(ptr_s_temp, (atGRPX_PIXEL)Color);
	} else {
		nAlpha_temp = 0xFF - nAlpha1;
		__ALPHA_PIXEL_I(ptr_s_temp, r1,g1,b1, cl,r2,g2,b2, nAlpha1,nAlpha_temp);
	}
}

// atGRPX_DrawPixelBre의 미러 버젼을 이용하여 항상 방향의 좌측만 알파를 먹임... Edge에 사용.(한번에 4픽셀)
atVOID	atGRPX_DrawPixelBreMirEdgePt( atGRPX_CANVAS *lpCanvas, atLONG draws_x, atLONG draws_y, atLONG drawe_x, atLONG drawe_y, atLONG inc, atGRPX_COLOR Color, atLONG invD2du, atLONG diff_distance, atBOOL inverse, atLONG uincr, atLONG vincr)
{

}

atVOID	atGRPX_DrawBreLineWidthAAIn( atGRPX_CANVAS *lpCanvas, atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atRPOINT Pt_3, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth )
{
	//삼각형 두개를 그려야 함.....
	atGRPX_DrawBreTriangleEdgeAA(lpCanvas, Pt_0, Pt_1, Pt_2, Color, OutColor, nWidth);
	atGRPX_DrawBreTriangleEdgeAA(lpCanvas, Pt_2, Pt_3, Pt_0, Color, OutColor, nWidth);
}

atVOID	atGRPX_DrawBreLineWidthAAOut( atGRPX_CANVAS *lpCanvas, atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atRPOINT Pt_3, atPOINT pkPtOut[], atGRPX_COLOR Color, atGRPX_COLOR BkColor, atINT nWidth )
{
	// 일반 변수
	atPOINT pt0, pt1, pt2, pt3;
	atRECT clip_shrink;
	atINT shrink;
	atRECT *clip;
	
	// Bresenham 변수
	
	// 점들을 모두 SHORT화 한다.
	pkPtOut[0].nX = pt0.nX = REALTOINT(Pt_0.rX);
	pkPtOut[0].nY = pt0.nY = REALTOINT(Pt_0.rY);
	pkPtOut[1].nX = pt1.nX = REALTOINT(Pt_1.rX);
	pkPtOut[1].nY = pt1.nY = REALTOINT(Pt_1.rY);
	pkPtOut[2].nX = pt2.nX = REALTOINT(Pt_2.rX);
	pkPtOut[2].nY = pt2.nY = REALTOINT(Pt_2.rY);
	pkPtOut[3].nX = pt3.nX = REALTOINT(Pt_3.rX);
	pkPtOut[3].nY = pt3.nY = REALTOINT(Pt_3.rY);
		
	clip = atGRPX_GetClipRect( lpCanvas );
	
	shrink = 2;
	clip_shrink.nStartX = clip->nStartX+shrink;
	clip_shrink.nStartY = clip->nStartX+shrink;
	clip_shrink.nEndX = clip->nEndX-shrink;
	clip_shrink.nEndY = clip->nEndX-shrink;
	
	//Pt_1->Pt_2 Line
	if((pt1.nX < clip->nStartX && pt2.nX < clip->nStartX) || (pt1.nX > clip->nEndX && pt2.nX > clip->nEndX) ||
		(pt1.nY < clip->nStartY && pt2.nY < clip->nStartY) || (pt1.nY > clip->nEndY && pt2.nY > clip->nEndY))
	{
		// 완전 밖인 경우
	}
	else
	{
		if((pt1.nX >= clip_shrink.nStartX && pt2.nX >= clip_shrink.nStartX) && (pt1.nX <= clip_shrink.nEndX && pt2.nX <= clip_shrink.nEndX) &&
		(pt1.nY >= clip_shrink.nStartY && pt2.nY >= clip_shrink.nStartY) && (pt1.nY <= clip_shrink.nEndY && pt2.nY <= clip_shrink.nEndY))
		{
			//화면 안에 있는 경우 -> 메모리 접근하여 그린다.
			atGRPX_DrawBreEdgeAAMem(lpCanvas, pkPtOut[1], pkPtOut[2],  &(pkPtOut[1]), &(pkPtOut[2]), Color, BkColor);
		}
		else
		{
			// 일부분이 밖인 경우
			atGRPX_DrawBreEdgeAAPt(lpCanvas, pkPtOut[1], pkPtOut[2],  &(pkPtOut[1]), &(pkPtOut[2]), Color, BkColor);
		}
	}
	
	//Pt_3->Pt_0 Line
	
	if((pt3.nX < clip->nStartX && pt0.nX < clip->nStartX) || (pt3.nX > clip->nEndX && pt0.nX > clip->nEndX) ||
		(pt3.nY < clip->nStartY && pt0.nY < clip->nStartY) || (pt3.nY > clip->nEndY && pt0.nY > clip->nEndY))
	{
		// 완전 밖인 경우	
	}
	else
	{
		if((pt3.nX >= clip_shrink.nStartX && pt0.nX >= clip_shrink.nStartX) && (pt3.nX <= clip_shrink.nEndX && pt0.nX <= clip_shrink.nEndX) &&
		(pt3.nY >= clip_shrink.nStartY && pt0.nY >= clip_shrink.nStartY) && (pt3.nY <= clip_shrink.nEndY && pt0.nY <= clip_shrink.nEndY))
		{
			//화면 안에 있는 경우 -> 메모리 접근하여 그린다.
			atGRPX_DrawBreEdgeAAMem(lpCanvas, pkPtOut[3], pkPtOut[0],  &(pkPtOut[3]), &(pkPtOut[0]), Color, BkColor);
		}
		else
		{
			//일부분이 밖인 경우
			atGRPX_DrawBreEdgeAAPt(lpCanvas, pkPtOut[3], pkPtOut[0],  &(pkPtOut[3]), &(pkPtOut[0]), Color, BkColor);
		}
	}
	
}

//도로 출력시 Out Line을 그리는 경우 (화면안에 있는 경우 Clipping 하지 않는다.)
atVOID	atGRPX_DrawBreEdgeAAMem(atGRPX_CANVAS* lpCanvas, atPOINT Pt_0, atPOINT Pt_1, atPOINT* pkPtOut_0, atPOINT* pkPtOut_1, atGRPX_COLOR ExParam1, atGRPX_COLOR ExParam2)
{

	atLONG dx;
    atLONG dy;
	atLONG drawu_s;
	atLONG drawv_s;
	atLONG drawu_e;
	atLONG drawv_e;
	atGRPX_PIXEL Color = (atGRPX_PIXEL)ExParam1;
	atGRPX_PIXEL BkColor = (atGRPX_PIXEL)ExParam2;
	atLONG du;
	atLONG dv;
	atLONG u;
	atLONG v;
	
	atLONG uincr_mem;		// 메모리 주소
	atLONG vincr_mem;
	atLONG uincr;			//Screen 좌표
	atLONG vincr;
	
	atLONG uend;
    atLONG d;				
    atLONG incrS;			
    atLONG incrD;			
    atLONG twovdu;			

    atREAL invD;			
    atREAL invD2du;		

	atBOOL inverse;
	atLONG alpha_1 = 0;
	atLONG alpha_2 = 0;

	atREAL diff_distance = 0;
	
	atGRPX_PIXEL* addr_s; // 픽셀을 출력할 버퍼의 위치
	atGRPX_PIXEL* addr_e; // 픽셀을 출력할 버퍼의 위치
	
	atBOOL isOdd = atTRUE;
	atBOOL edgeflag = atTRUE;
	
	atLONG alpha_var_1 = 0; // 알파값 매개 변수
	atLONG alpha_var_2 = 0;

	dx = Pt_1.nX - Pt_0.nX;
	dy = Pt_1.nY - Pt_0.nY;

	if(dx == 0 && dy == 0)
	{
		return;
	}

    if (ABS(dx) > ABS(dy))
    {
		du = ABS(dx);
		dv = ABS(dy);
		u = Pt_1.nX;
		v = Pt_1.nY;
		drawu_s = Pt_0.nX;
		drawv_s = Pt_0.nY;
		drawu_e = Pt_1.nX;
		drawv_e = Pt_1.nY;
		uincr_mem = 1;
		vincr_mem = lpCanvas->nVMemWidth;
		uincr = 1;
		vincr = 1;
		if (dx < 0)
		{
			uincr_mem = -uincr_mem;
			uincr = -uincr;
		}
		if (dy < 0)
		{
			vincr_mem = -vincr_mem;
			vincr = -vincr;
		}
		inverse = atFALSE;
    }
    else
    {
		du = ABS(dy);
		dv = ABS(dx);
		u = Pt_1.nY;
		v = Pt_1.nX;
		drawu_s = Pt_0.nY;
		drawv_s = Pt_0.nX;
		drawu_e = Pt_1.nY;
		drawv_e = Pt_1.nX;
		uincr_mem = lpCanvas->nVMemWidth;
		vincr_mem = 1;
		uincr = 1;
		vincr = 1;
		if (dy < 0)
		{
			uincr_mem = -uincr_mem;
			uincr = -uincr;
		}
		if (dx < 0)
		{
			vincr_mem = -vincr_mem;
			vincr = -vincr ;
		}
		inverse = atTRUE;
    }
    
	if(du % 2)
	{
		isOdd = atTRUE;
	}
	else
	{
		isOdd = atFALSE;
	}

	uend = u + ((du%2)?(du>>1)+1:(du>>1));
    d = (2 * dv) - du;	    /* Initial value as in Bresenham's */
    incrS = 2 * dv;	/* Δd for straight increments */
    incrD = 2 * (dv - du);	/* Δd for diagonal increments */
    twovdu = 0;	/* Numerator of distance; starts at 0 */
  
	//for AA
	invD = REALDIV(INTTOREAL(1),INTTOREAL(2 * IntSqrt((unsigned long)(du*du) + (unsigned long)(dv*dv))));
	invD2du = REALMUL(INTTOREAL(2),REALMUL(INTTOREAL(du),invD));   /* Precomputed constant */ //코사인 
    
	addr_s = atGRPX_GetFrameBufferPtrXY( lpCanvas, Pt_0.nX, Pt_0.nY );
	addr_e = atGRPX_GetFrameBufferPtrXY( lpCanvas, Pt_1.nX, Pt_1.nY );

	//For Detect Left Edge
	if((uincr + vincr) == 0)
	{
		edgeflag = atTRUE;
	}
	else
	{
		edgeflag = atFALSE;
	}

	if(inverse)
	{
		if(edgeflag)
		{
			pkPtOut_0->nX = Pt_0.nX - vincr;
			pkPtOut_0->nY = Pt_0.nY;
			pkPtOut_1->nX = Pt_1.nX - vincr;
			pkPtOut_1->nY = Pt_1.nY;
		}
		else
		{
			pkPtOut_0->nX = Pt_0.nX + vincr;
			pkPtOut_0->nY = Pt_0.nY;
			pkPtOut_1->nX = Pt_1.nX + vincr;
			pkPtOut_1->nY = Pt_1.nY;
		}
	}
	else
	{
		if(edgeflag)
		{
			pkPtOut_0->nX = Pt_0.nX;
			pkPtOut_0->nY = Pt_0.nY + vincr;
			pkPtOut_1->nX = Pt_1.nX;
			pkPtOut_1->nY = Pt_1.nY + vincr;
		}
		else
		{
			pkPtOut_0->nX = Pt_0.nX;
			pkPtOut_0->nY = Pt_0.nY - vincr;
			pkPtOut_1->nX = Pt_1.nX;
			pkPtOut_1->nY = Pt_1.nY - vincr;
		}
	}

	do
    {
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));

		if(inverse)
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBreMirEdgeMem( addr_s, addr_e, -vincr_mem, Color, BkColor, alpha_var_2, alpha_var_1 + alpha_var_2, alpha_var_1 - alpha_var_2);
			}
			else
			{
				atGRPX_DrawPixelBreMirEdgeMem( addr_s, addr_e, vincr_mem, Color, BkColor, alpha_var_2, alpha_var_1 - alpha_var_2, alpha_var_1 + alpha_var_2);
			}			
		}
		else
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBreMirEdgeMem( addr_s, addr_e, vincr_mem, Color, BkColor, alpha_var_2, alpha_var_1 - alpha_var_2, alpha_var_1 + alpha_var_2);
			}
			else
			{
				atGRPX_DrawPixelBreMirEdgeMem( addr_s, addr_e, -vincr_mem, Color, BkColor, alpha_var_2, alpha_var_1 + alpha_var_2, alpha_var_1 - alpha_var_2);
			}
		}

		if (d < 0)
		{
			/* choose straight (u direction) */
			twovdu = d + du;
			d = d + incrS;
		}
		else
		{
			/* choose diagonal (u+v direction) */
			twovdu = d - du;
			d = d + incrD;
			v = v+1;
			addr_s = addr_s + vincr_mem;
			addr_e = addr_e - vincr_mem;
			drawv_s = drawv_s + vincr;																	
			drawv_e = drawv_e - vincr;
		}
		u = u+1;
		addr_s = addr_s + uincr_mem;
		addr_e = addr_e - uincr_mem;
		drawu_s = drawu_s + uincr;																		
		drawu_e = drawu_e - uincr;
    } while (u < uend);

	if(drawu_s == drawu_e)
	{
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));
		
		if(inverse)
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBreMirEdgeMem( addr_s, addr_e, -vincr_mem, Color, BkColor, alpha_var_2, alpha_var_1 + alpha_var_2, alpha_var_1 - alpha_var_2);
			}
			else
			{
				atGRPX_DrawPixelBreMirEdgeMem( addr_s, addr_e, vincr_mem, Color, BkColor, alpha_var_2, alpha_var_1 - alpha_var_2, alpha_var_1 + alpha_var_2);
			}			
		}
		else
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBreMirEdgeMem( addr_s, addr_e, vincr_mem, Color, BkColor, alpha_var_2, alpha_var_1 - alpha_var_2, alpha_var_1 + alpha_var_2);
			}
			else
			{
				atGRPX_DrawPixelBreMirEdgeMem( addr_s, addr_e, -vincr_mem, Color, BkColor, alpha_var_2, alpha_var_1 + alpha_var_2, alpha_var_1 - alpha_var_2);
			}
		}
	}
}

// 도로의 외각 라인을 그림.(포인트를 이용)
atVOID	atGRPX_DrawBreEdgeAAPt(atGRPX_CANVAS* lpCanvas, atPOINT Pt_0, atPOINT Pt_1, atPOINT* pkPtOut_0, atPOINT* pkPtOut_1, atGRPX_COLOR ExParam1, atGRPX_COLOR ExParam2)
{

	atLONG dx = Pt_1.nX-Pt_0.nX;
    atLONG dy = Pt_1.nY-Pt_0.nY;
	atLONG drawu_s = 0;
	atLONG drawv_s = 0;
	atLONG drawu_e = 0;
	atLONG drawv_e = 0;
	atGRPX_PIXEL Color = (atGRPX_PIXEL)ExParam1;
	atGRPX_PIXEL BkColor = (atGRPX_PIXEL)ExParam2;
	atLONG du = ABS(dx);
	atLONG dv = ABS(dy);
	atLONG u = Pt_1.nX;
	atLONG v = Pt_1.nY;
	
	atLONG uincr;			//Screen 좌표
	atLONG vincr;
	
	atLONG uend;
    atLONG d;				/* Initial value as in Bresenham's */
    atLONG incrS;			/* Δd for straight increments */
    atLONG incrD;			/* Δd for diagonal increments */
    atLONG twovdu;			/* Numerator of distance; starts at 0 */

    atREAL invD;			/* Precomputed inverse denominator */
    atREAL invD2du;		/* Precomputed constant */

	atBOOL inverse;
	atLONG alpha_1 = 0;
	atLONG alpha_2 = 0;

	atREAL diff_distance = 0;
	
	atBOOL isOdd = atTRUE;
	atBOOL edgeflag = atTRUE;
	atRECT* clip;

	atLONG alpha_var_1 = 0; // 알파값 매개 변수
	atLONG alpha_var_2 = 0;

	if(dx == 0 && dy == 0)
	{
		pkPtOut_0->nX = Pt_0.nX;
		pkPtOut_0->nY = Pt_0.nY;
		pkPtOut_1->nX = Pt_1.nX;
		pkPtOut_1->nY = Pt_1.nY;

		return;
	}
	
    /* By switching to (u,v), we combine all eight octants */
    if (ABS(dx) > ABS(dy))
    {
	/* Note: If this were actual C, these integers would be lost
	* at the closing brace.  That's not what I mean to do.  Do what
		* I mean. */
		du = ABS(dx);
		dv = ABS(dy);
		u = Pt_1.nX;
		v = Pt_1.nY;
		drawu_s = Pt_0.nX;
		drawv_s = Pt_0.nY;
		drawu_e = Pt_1.nX;
		drawv_e = Pt_1.nY;
		uincr = 1;
		vincr = 1;
		if (dx < 0)
		{
			uincr = -uincr;
		}
		if (dy < 0)
		{
			vincr = -vincr;
		}
		inverse = atFALSE;
    }
    else
    {
		du = ABS(dy);
		dv = ABS(dx);
		u = Pt_1.nY;
		v = Pt_1.nX;
		drawu_s = Pt_0.nY;
		drawv_s = Pt_0.nX;
		drawu_e = Pt_1.nY;
		drawv_e = Pt_1.nX;
		uincr = 1;
		vincr = 1;
		if (dy < 0)
		{
			uincr = -uincr;
		}
		if (dx < 0)
		{
			vincr = -vincr ;
		}
		inverse = atTRUE;
    }
    
	if(du % 2)
	{
		isOdd = atTRUE;
	}
	else
	{
		isOdd = atFALSE;
	}

	uend = u + ((du%2)?(du>>1)+1:(du>>1));
    d = (2 * dv) - du;	    /* Initial value as in Bresenham's */
    incrS = 2 * dv;	/* Δd for straight increments */
    incrD = 2 * (dv - du);	/* Δd for diagonal increments */
    twovdu = 0;	/* Numerator of distance; starts at 0 */
  
	//for AA
	invD = REALDIV(INTTOREAL(1),INTTOREAL(2 * IntSqrt((unsigned long)(du*du) + (unsigned long)(dv*dv))));
	invD2du = REALMUL(INTTOREAL(2),REALMUL(INTTOREAL(du),invD));   /* Precomputed constant */ //코사인 
    
	//For Detect Left Edge
	if((uincr + vincr) == 0)
	{
		edgeflag = atTRUE;
	}
	else
	{
		edgeflag = atFALSE;
	}
	
	clip = atGRPX_GetClipRect( lpCanvas );

	if(inverse)
	{
		if(edgeflag)
		{
			pkPtOut_0->nX = drawv_s - vincr;
			pkPtOut_0->nY = drawu_s;
			pkPtOut_1->nX = drawv_e - vincr;
			pkPtOut_1->nY = drawu_e;
		}
		else
		{
			pkPtOut_0->nX = drawv_s + vincr;
			pkPtOut_0->nY = drawu_s;
			pkPtOut_1->nX = drawv_e + vincr;
			pkPtOut_1->nY = drawu_e;
		}
	}
	else
	{
		if(edgeflag)
		{
			pkPtOut_0->nX = drawu_s;
			pkPtOut_0->nY = drawv_s + vincr;
			pkPtOut_1->nX = drawu_e;
			pkPtOut_1->nY = drawv_e + vincr;
		}
		else
		{
			pkPtOut_0->nX = drawu_s;
			pkPtOut_0->nY = drawv_s - vincr;
			pkPtOut_1->nX = drawu_e;
			pkPtOut_1->nY = drawv_e - vincr;
		}
	}

	do
    {
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));

		if(inverse)
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s - vincr, drawu_s, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s - vincr - vincr, drawu_s, Color, alpha_var_1 + alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e - vincr, drawu_e, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e - vincr - vincr, drawu_e, Color, alpha_var_1 - alpha_var_2, clip);
			}
			else
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s + vincr, drawu_s, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s + vincr + vincr, drawu_s, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e + vincr, drawu_e, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e + vincr + vincr, drawu_e, Color, alpha_var_1 + alpha_var_2, clip);
			}
		}
		else
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s + vincr, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s + vincr + vincr, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e + vincr, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e + vincr + vincr, Color, alpha_var_1 + alpha_var_2, clip);
			}
			else
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s - vincr, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s - vincr - vincr, Color, alpha_var_1 + alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e - vincr, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e - vincr - vincr, Color, alpha_var_1 - alpha_var_2, clip);
			}
		}

		if (d < 0)
		{
			/* choose straight (u direction) */
			twovdu = d + du;
			d = d + incrS;
		}
		else
		{
			/* choose diagonal (u+v direction) */
			twovdu = d - du;
			d = d + incrD;
			v = v+1;
			drawv_s = drawv_s + vincr;
			drawv_e = drawv_e - vincr;
		}
		u = u+1;
		drawu_s = drawu_s + uincr;
		drawu_e = drawu_e - uincr;
    } while (u < uend);

	if(drawu_s == drawu_e)
	{
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));
		
		if(inverse)
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e - vincr, drawu_e, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e - vincr - vincr, drawu_e, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s - vincr, drawu_s, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s - vincr - vincr, drawu_s, Color, alpha_var_1 + alpha_var_2, clip);
			}
			else
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s + vincr, drawu_s, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_s + vincr + vincr, drawu_s, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e + vincr, drawu_e, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawv_e + vincr + vincr, drawu_e, Color, alpha_var_1 + alpha_var_2, clip);
			}
		}
		else
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s + vincr, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s + vincr + vincr, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e + vincr, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e + vincr + vincr, Color, alpha_var_1 + alpha_var_2, clip);
			}
			else
			{
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e - vincr, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_e, drawv_e - vincr - vincr, Color, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s - vincr, Color, alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel(lpCanvas, drawu_s, drawv_s - vincr - vincr, Color, alpha_var_1 + alpha_var_2, clip);
			}
		}
	}
}

atVOID	atGRPX_GetNextRoundPoint(atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atRPOINT Pt_3, atPOINT pkPtOut[], atINT nWidth)
{
	// 일반 변수
	atPOINT pt0, pt1, pt2, pt3;
	
	// Bresenham 변수
	atLONG dx;
    atLONG dy;
	atLONG drawu_s = 0;
	atLONG drawv_s = 0;
	atLONG drawu_e = 0;
	atLONG drawv_e = 0;
	atLONG du;
	atLONG dv;
	atLONG u;
	atLONG v;
	
	atLONG uincr;			//Screen 좌표
	atLONG vincr;
	
	atBOOL inverse;
	atBOOL edgeflag = atTRUE;

	
	// 점들을 모두 SHORT화 한다.
	pkPtOut[0].nX = pt0.nX = REALTOINT(Pt_0.rX);
	pkPtOut[0].nY = pt0.nY = REALTOINT(Pt_0.rY);
	pkPtOut[1].nX = pt1.nX = REALTOINT(Pt_1.rX);
	pkPtOut[1].nY = pt1.nY = REALTOINT(Pt_1.rY);
	pkPtOut[2].nX = pt2.nX = REALTOINT(Pt_2.rX);
	pkPtOut[2].nY = pt2.nY = REALTOINT(Pt_2.rY);
	pkPtOut[3].nX = pt3.nX = REALTOINT(Pt_3.rX);
	pkPtOut[3].nY = pt3.nY = REALTOINT(Pt_3.rY);

	// 첫 점.... 
	dx = pt2.nX-pt1.nX;
    dy = pt2.nY-pt1.nY;

	if(dx == 0 && dy == 0)
	{
		return;
	}

    if (ABS(dx) > ABS(dy))
    {
		du = ABS(dx);
		dv = ABS(dy);
		u = pt2.nX;
		v = pt2.nY;
		drawu_s = pt1.nX;
		drawv_s = pt1.nY;
		drawu_e = pt2.nX;
		drawv_e = pt2.nY;
		uincr = 1;
		vincr = 1;
		if (dx < 0)
		{
			uincr = -uincr;
		}
		if (dy < 0)
		{
			vincr = -vincr;
		}
		inverse = atFALSE;
    }
    else
    {
		du = ABS(dy);
		dv = ABS(dx);
		u = pt2.nY;
		v = pt2.nX;
		drawu_s = pt1.nY;
		drawv_s = pt1.nX;
		drawu_e = pt2.nY;
		drawv_e = pt2.nX;
		uincr = 1;
		vincr = 1;
		if (dy < 0)
		{
			uincr = -uincr;
		}
		if (dx < 0)
		{
			vincr = -vincr ;
		}
		inverse = atTRUE;
    }
    
	//For Detect Left Edge
	if((uincr + vincr) == 0)
	{
		edgeflag = atTRUE;
	}
	else
	{
		edgeflag = atFALSE;
	}
	
	if(inverse)
	{
		if(edgeflag)
		{
			pkPtOut[1].nX = drawv_s - vincr;
			pkPtOut[1].nY = drawu_s;
			pkPtOut[2].nX = drawv_e - vincr;
			pkPtOut[2].nY = drawu_e;
		}
		else
		{
			pkPtOut[1].nX = drawv_s + vincr;
			pkPtOut[1].nY = drawu_s;
			pkPtOut[2].nX = drawv_e + vincr;
			pkPtOut[2].nY = drawu_e;
		}
	}
	else
	{
		if(edgeflag)
		{
			pkPtOut[1].nX = drawu_s;
			pkPtOut[1].nY = drawv_s + vincr;
			pkPtOut[2].nX = drawu_e;
			pkPtOut[2].nY = drawv_e + vincr;
		}
		else
		{
			pkPtOut[1].nX = drawu_s;
			pkPtOut[1].nY = drawv_s - vincr;
			pkPtOut[2].nX = drawu_e;
			pkPtOut[2].nY = drawv_e - vincr;
		}
	}

	// 두번째 
	dx = pt0.nX-pt3.nX;
    dy = pt0.nY-pt3.nY;
	
    if (ABS(dx) > ABS(dy))
    {
		du = ABS(dx);
		dv = ABS(dy);
		u = pt0.nX;
		v = pt0.nY;
		drawu_s = pt3.nX;
		drawv_s = pt3.nY;
		drawu_e = pt0.nX;
		drawv_e = pt0.nY;
		uincr = 1;
		vincr = 1;
		if (dx < 0)
		{
			uincr = -uincr;
		}
		if (dy < 0)
		{
			vincr = -vincr;
		}
		inverse = atFALSE;
    }
    else
    {
		du = ABS(dy);
		dv = ABS(dx);
		u = pt0.nY;
		v = pt0.nX;
		drawu_s = pt3.nY;
		drawv_s = pt3.nX;
		drawu_e = pt0.nY;
		drawv_e = pt0.nX;
		uincr = 1;
		vincr = 1;
		if (dy < 0)
		{
			uincr = -uincr;
		}
		if (dx < 0)
		{
			vincr = -vincr ;
		}
		inverse = atTRUE;
    }
    
	//For Detect Left Edge
	if((uincr + vincr) == 0)
	{
		edgeflag = atTRUE;
	}
	else
	{
		edgeflag = atFALSE;
	}
	
	if(inverse)
	{
		if(edgeflag)
		{
			pkPtOut[3].nX = drawv_s - vincr;
			pkPtOut[3].nY = drawu_s;
			pkPtOut[0].nX = drawv_e - vincr;
			pkPtOut[0].nY = drawu_e;
		}
		else
		{
			pkPtOut[3].nX = drawv_s + vincr;
			pkPtOut[3].nY = drawu_s;
			pkPtOut[0].nX = drawv_e + vincr;
			pkPtOut[0].nY = drawu_e;
		}
	}
	else
	{
		if(edgeflag)
		{
			pkPtOut[3].nX = drawu_s;
			pkPtOut[3].nY = drawv_s + vincr;
			pkPtOut[0].nX = drawu_e;
			pkPtOut[0].nY = drawv_e + vincr;
		}
		else
		{
			pkPtOut[3].nX = drawu_s;
			pkPtOut[3].nY = drawv_s - vincr;
			pkPtOut[0].nX = drawu_e;
			pkPtOut[0].nY = drawv_e - vincr;
		}
	}

}

// 지정된 배경색과 AA하여 라인을 출력 한다.(한줄만....)
atVOID	atGRPX_DrawBreTriangleEdgeLineAA(atGRPX_CANVAS* lpCanvas, atLONG StartX, atLONG StartY, atLONG EndX, atLONG EndY, atGRPX_COLOR ExParam1, atGRPX_COLOR ExParam2)
{

	atLONG dx = EndX-StartX;
    atLONG dy = EndY-StartY;
	atLONG drawu_s = 0;
	atLONG drawv_s = 0;
	atLONG drawu_e = 0;
	atLONG drawv_e = 0;
	atGRPX_PIXEL Color = (atGRPX_PIXEL)ExParam2;
	atGRPX_PIXEL BkColor = (atGRPX_PIXEL)ExParam1;
	atLONG du = ABS(dx);
	atLONG dv = ABS(dy);
	atLONG u;
	atLONG v;
	
	atLONG uincr;			//Screen 좌표
	atLONG vincr;
	
	atLONG uend;
    atLONG d;				/* Initial value as in Bresenham's */
    atLONG incrS;			/* Δd for straight increments */
    atLONG incrD;			/* Δd for diagonal increments */
    atLONG twovdu;			/* Numerator of distance; starts at 0 */

    atREAL invD;			/* Precomputed inverse denominator */
    atREAL invD2du;		/* Precomputed constant */

	atBOOL inverse;
	atLONG alpha_1 = 0;
	atLONG alpha_2 = 0;

	atREAL diff_distance = 0;
	
	atBOOL isOdd = atTRUE;
	atBOOL edgeflag = atTRUE;
	atRECT* clip;

	atLONG alpha_var_1 = 0; // 알파값 매개 변수
	atLONG alpha_var_2 = 0;
	atREAL rV1 = 0;

	if(dx == 0 && dy == 0)
	{
		return;
	}
	
    /* By switching to (u,v), we combine all eight octants */
    if (ABS(dx) > ABS(dy))
    {
	/* Note: If this were actual C, these integers would be lost
	* at the closing brace.  That's not what I mean to do.  Do what
		* I mean. */
		du = ABS(dx);
		dv = ABS(dy);
		u = EndX;
		v = EndY;
		drawu_s = StartX;
		drawv_s = StartY;
		drawu_e = EndX;
		drawv_e = EndY;
		uincr = 1;
		vincr = 1;
		if (dx < 0)
		{
			uincr = -uincr;
		}
		if (dy < 0)
		{
			vincr = -vincr;
		}
		inverse = atFALSE;
    }
    else
    {
		du = ABS(dy);
		dv = ABS(dx);
		u = EndY;
		v = EndX;
		drawu_s = StartY;
		drawv_s = StartX;
		drawu_e = EndY;
		drawv_e = EndX;
		uincr = 1;
		vincr = 1;
		if (dy < 0)
		{
			uincr = -uincr;
		}
		if (dx < 0)
		{
			vincr = -vincr ;
		}
		inverse = atTRUE;
    }
    
	if(du % 2)
	{
		isOdd = atTRUE;
	}
	else
	{
		isOdd = atFALSE;
	}

	uend = u + ((du%2)?(du>>1)+1:(du>>1));
    d = (2 * dv) - du;	    /* Initial value as in Bresenham's */
    incrS = 2 * dv;	/* Δd for straight increments */
    incrD = 2 * (dv - du);	/* Δd for diagonal increments */
    twovdu = 0;	/* Numerator of distance; starts at 0 */
  
	//check variable devide by zero
	rV1 = INTTOREAL(2 * IntSqrt((unsigned long)(du*du) + (unsigned long)(dv*dv)));
	if (rV1 == 0)
	{
		return;
	}

	//for AA
	invD = REALDIV(INTTOREAL(1),INTTOREAL(2 * IntSqrt((unsigned long)(du*du) + (unsigned long)(dv*dv))));
	invD2du = REALMUL(INTTOREAL(2),REALMUL(INTTOREAL(du),invD));   /* Precomputed constant */ //코사인 
    
	//For Detect Left Edge
	if((uincr + vincr) == 0)
	{
		edgeflag = atTRUE;
	}
	else
	{
		edgeflag = atFALSE;
	}
	clip = atGRPX_GetClipRect( lpCanvas );

	do
    {
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));

		if(inverse)
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawv_s, drawu_s, Color, BkColor, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawv_e, drawu_e, Color, BkColor, alpha_var_1 + alpha_var_2, clip);

			}
			else
			{
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawv_s, drawu_s, Color, BkColor, alpha_var_1 + alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawv_e, drawu_e, Color, BkColor, alpha_var_1 - alpha_var_2, clip);

			}
		}
		else
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawu_s, drawv_s, Color, BkColor, alpha_var_1 + alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawu_e, drawv_e, Color, BkColor, alpha_var_1 - alpha_var_2, clip);

			}
			else
			{
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawu_s, drawv_s, Color, BkColor, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawu_e, drawv_e, Color, BkColor, alpha_var_1 + alpha_var_2, clip);

			}
		}

		if (d < 0)
		{
			/* choose straight (u direction) */
			twovdu = d + du;
			d = d + incrS;
		}
		else
		{
			/* choose diagonal (u+v direction) */
			twovdu = d - du;
			d = d + incrD;
			v = v+1;
			drawv_s = drawv_s + vincr;
			drawv_e = drawv_e - vincr;
		}
		u = u+1;
		drawu_s = drawu_s + uincr;
		drawu_e = drawu_e - uincr;
    } while (u < uend);

	if(drawu_s == drawu_e)
	{
		diff_distance = REALMUL(invD, INTTOREAL(twovdu));
		alpha_var_1 = REALTOINT(REALMUL(invD2du, INTTOREAL(100)));
		alpha_var_2 = REALTOINT(REALMUL(diff_distance,INTTOREAL(100)));
		
		if(inverse)
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawv_s, drawu_s, Color, BkColor, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawv_e, drawu_e, Color, BkColor, alpha_var_1 + alpha_var_2, clip);
			}
			else
			{
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawv_s, drawu_s, Color, BkColor, alpha_var_1 + alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawv_e, drawu_e, Color, BkColor, alpha_var_1 - alpha_var_2, clip);
			}
		}
		else
		{
			if(edgeflag)
			{
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawu_s, drawv_s, Color, BkColor, alpha_var_1 + alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawu_e, drawv_e, Color, BkColor, alpha_var_1 - alpha_var_2, clip);
			}
			else
			{
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawu_s, drawv_s, Color, BkColor, alpha_var_1 - alpha_var_2, clip);
				atGRPX_DrawPixelBrePixel_2(lpCanvas, drawu_e, drawv_e, Color, BkColor, alpha_var_1 + alpha_var_2, clip);
			}
		}
	}
}

atVOID	atGRPX_DrawBreLineWidthAAInEdge( atGRPX_CANVAS *lpCanvas, atRPOINT Pt_0, atRPOINT Pt_1, atRPOINT Pt_2, atRPOINT Pt_3, atGRPX_COLOR Color, atGRPX_COLOR OutColor, atINT nWidth )
{
	atGRPX_DrawBreTriangleEdgeLineAA(lpCanvas, REALTOINT(Pt_1.rX), REALTOINT(Pt_1.rY), REALTOINT(Pt_2.rX), REALTOINT(Pt_2.rY), Color, OutColor);
	atGRPX_DrawBreTriangleEdgeLineAA(lpCanvas, REALTOINT(Pt_3.rX), REALTOINT(Pt_3.rY), REALTOINT(Pt_0.rX), REALTOINT(Pt_0.rY), Color, OutColor);
}

// 임시로 여기에 둔다. 나중에 다른곳에 놔두자.
atVOID atGRPX_DrawHorizLine2( atGRPX_CANVAS *lpCanvas, atLONG StartX, atLONG StartY, atINT Length, atGRPX_COLOR Color, atRECT* clip )
{
    int i, t;
    atGRPX_PIXEL *p;
    atGRPX_PIXEL cl = COLOR_TO_PIXEL(Color);
	
    if( Length <= 0 ) return;
	
    if( (StartY < clip->nStartY) || (StartY > clip->nEndY) ) return;
	
    if( StartX > clip->nEndX ) return;
    if( StartX < clip->nStartX ){
		Length -= clip->nStartX - StartX;
		StartX = clip->nStartX;
    }
    t = StartX + Length - 1;
    if( t < clip->nStartX ) return;
    if( t > clip->nEndX ){
		Length -= t - clip->nEndX;
    }
	
    p = atGRPX_GetFrameBufferPtrXY( lpCanvas, StartX, StartY );
	
	for( i = 0; i < Length; i ++ )
		*p++ = cl;
}

atVOID	atGRPX_DrawCircleTableMem(atGRPX_CANVAS *lpCanvas, atPOINT ptTL, atINT nWidth, atGRPX_COLOR Color)
{
	atGRPX_PIXEL *ptr;
	atINT cl, r1, g1, b1, r2, g2, b2;
	atINT nAlpha_temp;
	atINT i,j,index;
	atINT endX = ptTL.nX + nWidth;
	atINT endY = ptTL.nY + nWidth;
	atINT* round_table;
	
	switch(nWidth)
	{
	case 1:
		{
			round_table = round_table_1;
			break;
		}
	case 2:
		{
			round_table = round_table_2;
			break;
		}
	case 3:
		{
			round_table = round_table_3;
			break;
		}
	case 4:
		{
			round_table = round_table_4;
			break;
		}
	case 5:
		{
			round_table = round_table_5;
			break;
		}
	case 6:
		{
			round_table = round_table_6;
			break;
		}
	case 7:
		{
			round_table = round_table_7;
			break;
		}
	case 8:
		{
			round_table = round_table_8;
			break;
		}
	case 9:
		{
			round_table = round_table_9;
			break;
		}
	case 10:
		{
			round_table = round_table_10;
			break;
		}
	case 11:
		{
			round_table = round_table_11;
			break;
		}
	case 12:
		{
			round_table = round_table_12;
			break;
		}
	case 13:
		{
			round_table = round_table_13;
			break;
		}
	case 14:
		{
			round_table = round_table_14;
			break;
		}
	case 15:
		{
			round_table = round_table_15;
			break;
		}
	case 16:
		{
			round_table = round_table_16;
			break;
		}
	case 17:
		{
			round_table = round_table_17;
			break;
		}
	default:
		{
			round_table = round_table_1;		
		}
	}
	
	ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, ptTL.nX, ptTL.nY );
	index = 0;
	
	for(j = ptTL.nY; j < endY; j++)
	{
		for(i = ptTL.nX; i < endX; i++)
		{
			if(round_table[index] != 0xFF)
			{
				nAlpha_temp = 0xFF - round_table[index];
				r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
				__ALPHA_PIXEL_I(ptr, r1,g1,b1, cl,r2,g2,b2, round_table[index],nAlpha_temp);
			}
			else
			{
				*ptr = (atGRPX_PIXEL)Color;
			}
			ptr = ptr + 1;
			index++;
		}
		ptr = ptr +lpCanvas->nVMemWidth - nWidth;
	}
}
atVOID	atGRPX_DrawCircleTablePt(atGRPX_CANVAS *lpCanvas, atPOINT ptTL, atINT nWidth, atGRPX_COLOR Color, atRECT* clip)
{
	atGRPX_PIXEL *ptr;
	atINT r1, g1, b1, r2, g2, b2;
	atINT i,j,index;
	atINT endX = ptTL.nX + nWidth;
	atINT endY = ptTL.nY + nWidth;
	atINT* round_table;
	
	switch(nWidth)
	{
	case 1:
		{
			round_table = round_table_1;
			break;
		}
	case 2:
		{
			round_table = round_table_2;
			break;
		}
	case 3:
		{
			round_table = round_table_3;
			break;
		}
	case 4:
		{
			round_table = round_table_4;
			break;
		}
	case 5:
		{
			round_table = round_table_5;
			break;
		}
	case 6:
		{
			round_table = round_table_6;
			break;
		}
	case 7:
		{
			round_table = round_table_7;
			break;
		}
	case 8:
		{
			round_table = round_table_8;
			break;
		}
	case 9:
		{
			round_table = round_table_9;
			break;
		}
	case 10:
		{
			round_table = round_table_10;
			break;
		}
	case 11:
		{
			round_table = round_table_11;
			break;
		}
	case 12:
		{
			round_table = round_table_12;
			break;
		}
	case 13:
		{
			round_table = round_table_13;
			break;
		}
	case 14:
		{
			round_table = round_table_14;
			break;
		}
	case 15:
		{
			round_table = round_table_15;
			break;
		}
	case 16:
		{
			round_table = round_table_16;
			break;
		}
	case 17:
		{
			round_table = round_table_17;
			break;
		}
	default:
		{
			round_table = round_table_1;		
		}
	}
	
	index = 0;
	
	for(j = ptTL.nY; j < endY; j++)
	{
		for(i = ptTL.nX; i < endX; i++)
		{
			if( i >= clip->nStartX && i <= clip->nEndX && j >= clip->nStartY && j <= clip->nEndY)
			{
				if(round_table[index] != 0xFF)
				{
					ptr = atGRPX_GetFrameBufferPtrXY( lpCanvas, i, j );
					r1 = GET_RED(Color); g1 = GET_GREEN(Color); b1 = GET_BLUE(Color);
					r2 = GET_RED(*ptr); g2 = GET_GREEN(*ptr); b2 = GET_BLUE(*ptr);
					r2 = (atUCHAR)( (((r1 - r2) * (round_table[index]))>>8) + (r2));
					g2 = (atUCHAR)( (((g1 - g2) * (round_table[index]))>>8) + (g2));
					b2 = (atUCHAR)( (((b1 - b2) * (round_table[index]))>>8) + (b2));
					*ptr = MAKERGB(r2,g2,b2);
				}
				else
				{
					atGRPX_DrawPixel(lpCanvas, i, j, Color);
				}
			}
			index++;
		}
	}
}

