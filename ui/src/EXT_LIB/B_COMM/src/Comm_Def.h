#ifndef __COMM_DEF_H_
#define __COMM_DEF_H_


#ifdef __cplusplus
extern "C" {
#endif
	

#define MAX_READ_BUFFER 4096
#define MAX_WRITE_BUFFER 4096

//#define OVERLAPPED_MODE 1

/*
	퀄컴GPS모듈을 사용하는 WIndows Mobile에서는 stat.cbInQue이 비정상적으로
	들어 온다.

	stat.cbInQue를 무시하고 전체 버퍼를 읽는다.
*/

#define	USE_PREREAD_BUFFER 0   // 이것 사용시 PC버전에서 GPS가 비정상 작동한다.


#ifdef __cplusplus
}
#endif


#endif
