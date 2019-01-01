/**
  @file UI_Types.h
  @brief UI�������� ����ϴ� �⺻ Type ����
  @remark UI�������� ����ϴ� �⺻ Type ����
  @author Amos
  @date 2010-04-19 ���� 11:41:00
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
@brief Margin(����) Ÿ��
@remark Anchor�� Layout ������ ���� �ַ� ���ȴ�.
*/
typedef struct tagatMARGIN {
    atLONG	nTop;			/**< �� ���� */
    atLONG	nBottom;		/**< �Ʒ� ���� */
    atLONG	nLeft;			/**< ���� ���� */
    atLONG	nRight;			/**< ������ ���� */
} atMARGIN;

/** @brief Margin ���� ��ũ�� */
#define SET_MARGIN(rt, t,b,l,r) 		{(rt).nTop = (t); (rt).nBottom = (b); (rt).nLeft = (l); (rt).nRight = (r);}


#ifdef __cplusplus
}
#endif


#endif
