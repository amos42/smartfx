#ifndef __COMM_DEF_H_
#define __COMM_DEF_H_


#ifdef __cplusplus
extern "C" {
#endif
	

#define MAX_READ_BUFFER 4096
#define MAX_WRITE_BUFFER 4096

//#define OVERLAPPED_MODE 1

/*
	����GPS����� ����ϴ� WIndows Mobile������ stat.cbInQue�� ������������
	��� �´�.

	stat.cbInQue�� �����ϰ� ��ü ���۸� �д´�.
*/

#define	USE_PREREAD_BUFFER 0   // �̰� ���� PC�������� GPS�� ������ �۵��Ѵ�.


#ifdef __cplusplus
}
#endif


#endif
