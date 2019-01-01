/**
  @file UI_Types.h
  @brief UI엔진에서 사용하는 기본 Type 정의
  @remark UI엔진에서 사용하는 기본 Type 정의
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see AT_Types.h
*/
#ifndef __UI_TYPES_H_
#define __UI_TYPES_H_


#ifndef __AT_TYPES_H_
#include "AT_Types.h"
#endif
#ifndef __AT_REAL_H_
#include "AT_Real.h"
#endif


#ifdef __cplusplus
  extern "C" {
#endif


/**
@brief Margin(여백) 타입
@remark Anchor나 Layout 마진을 위해 주로 사용된다.
*/
typedef struct tagatMARGIN {
    atLONG	nTop;			/**< 위 여백 */
    atLONG	nBottom;		/**< 아래 여백 */
    atLONG	nLeft;			/**< 왼쪽 여백 */
    atLONG	nRight;			/**< 오른쪽 여백 */
} atMARGIN;

/** @brief Margin 설정 매크로 */
#define SET_MARGIN(rt, t,b,l,r) 		{(rt).nTop = (t); (rt).nBottom = (b); (rt).nLeft = (l); (rt).nRight = (r);}


#ifdef __cplusplus
}
#endif


#endif
