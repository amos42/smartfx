/**
 * @file    navi.h
 * @brief
 *
 * <B><I>
 * Copyright 2007 by DSC Development Center, Samsung Techwin, Co., LTD.,
 * 145-3 Sangdaewon 1, Jungwon, Sungnam, Kyungki, Korea.
 * All rights reserved.
 *
 * This software is the confidential and proprietary information
 * of Samsung Techwin Co., LTD. ("Confidential Information"). You
 * shall not disclose such Confidential Information and shall use
 * it only in accordance with the terms of the license agreement
 * you entered into with Samsung.</I></B>
 */
#ifndef _NAVI_H_
#define _NAVI_H_

enum {
    EVENT_UP_KEY,
    EVENT_DOWN_KEY,
    EVENT_LEFT_KEY,
    EVENT_RIGHT_KEY,
    EVENT_JUMP_LEFT_KEY,
    EVENT_JUMP_RIGHT_KEY,
    EVENT_TELE_KEY,
    EVENT_WIDE_KEY,
    EVENT_EFFECT_KEY,
    EVENT_ENTER_KEY,
    EVENT_ADJDEL_KEY,

} NAVI_EVENT;

void get_display_info( unsigned char **addr, int *width, int *height );

#endif /* _NAVI_H_ */
