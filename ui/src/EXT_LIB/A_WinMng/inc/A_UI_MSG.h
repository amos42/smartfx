/**
  @file A_UI_MSG.h
  @brief SoftKey에 관한 API
  @remark SoftKey에 관한 API 모음
  @author Amos
  @date 2010-04-19 오전 11:41:00
  @see 
*/
#ifndef __A_UI_MSG_H_
#define __A_UI_MSG_H_


#include "UI_Types.h"
#include "UI_OpDef.h"


#ifdef __cplusplus
extern "C" {
#endif


typedef enum {
	atUIMSG_NULL = 0,				
	atUIMSG_DUMMY = atUIMSG_NULL,
		
	atUIMSG_INIT,					/**< 개체 생성시, 초기화 과정에서 호출되는 이벤트. 이 이벤트가 호출되는 시점에서, 개체는 액세스 가능하지만 개체의 추가적인 자원들은 액세스가 제한된다. 특히 개체 생성/소멸에 관한 프로세스를 방해하는 동작을 하면 오동작할 위험이 높다. */
	atUIMSG_RELEASE,				/**< 개체 소멸시, 소멸 동작에 앞서 호출되는 이벤트. 이 이벤트가 호출되는 시점에서, 아직 개체는 액세스 가능하다. */

	atUIMSG_PRECREATE,				/**< 개체 생성이 진행되기 전에 전처리를 위해 발생하는 커맨드 메시지(내부용) */
	atUIMSG_CREATE,				/**< 개체 생성을 위한 커맨드 메시지(내부용) */
	atUIMSG_REQ_DESTROY,				/**< 개체 파괴에 앞서 호출되는 이벤트. 개체 소멸을 허락할 것인지를 결정할 수 있다.(내부용) */
	atUIMSG_PREDESTROY,				/**< 개체 소멸에 앞서 전처리를 위해 발생하는 커맨드 메시지(내부용) */
	atUIMSG_DESTROY,				/**< 개체 소멸을 위한 커맨드 메시지(내부용) */
	
	atUIMSG_REGIST,				/**< 개체가 매니저에 등록되는 순간 발생하는 이벤트. 개체의 각종 추가적인 자원들을 사용할 수 있게 된다. */
	atUIMSG_SHOW,				/**< 개체가 화면에 표시되는 순간 발생하는 이벤트 */
	atUIMSG_HIDE,				/**< 개체가 Hide 되는 순간 발생하는 이벤트 */
	atUIMSG_CHANGEPOS,				/**< 개체의 위치를 변경하는 순간 발생하는 이벤트 */
	atUIMSG_PRERESIZE,				/**< 개체의 크기가 변경되기 전, 전처리를 위해 발생하는 이벤트 */
	atUIMSG_RESIZE,				/**< 개체의 크기가 변경되는 순간 발생하는 이벤트 */
	atUIMSG_CHANGELAYOUT,				/**< 개체의 레이아웃이 변경되는 순간 발생하는 이벤트 */

	atUIMSG_PREPAINT,				/**< 개체가 Paint 되기 전에 발생하는 이벤트 */
	atUIMSG_PAINTLAYOUT,				/**< 개체의 Layout 영역을 그려야 할 때 발생하는 이벤트 */
	atUIMSG_PAINT,				/**< 개체가 Paint 되는 순간 발생하는 이벤트. 사용자의 출력은 주로 이곳에서 이루어진다. */
	atUIMSG_PAINTCHILD,				/**< 개체가 Paint 되는 순간 발생하는 이벤트. 사용자의 출력은 주로 이곳에서 이루어진다. */
	atUIMSG_PAINTOBJS,				/**< 개체에 포함된 Child 개체들을 그려야 할 때 발생하는 이벤트 */
	atUIMSG_POSTPAINT,				/**< 개체의 Paint가 끝나는 순간 발생하는 이벤트 */

	atUIMSG_PAUSE,				/**< 개체를 잠시 멈추는 순간 발생하는 이벤트  */
	atUIMSG_RESUME,				/**< Pause된 개체들을 다시 살리는 순간 발생하는 이벤트  */

	atUIMSG_GOTFOCUS,				/**< 개체가 포커스를 갖는 순간 발생하는 이벤트. */
	atUIMSG_LOSTFOCUS,				/**< 개체가 포커스를 잃는 순간 발생하는 이벤트. */

	atUIMSG_TIMER,				/**< 개체 타이머가 발생시켜 주는 이벤트 */
	atUIMSG_SYSNOTIFY,				/**< 시스템에 전달해 주는 이벤트 */

	atUIMSG_COMMAND,				/**< Child 개체가 Owner 개체에게 전달해 주는 이벤트 */
	atUIMSG_COMMAND_EX,				/**< Child 개체가 Owner 개체에게 전달해 주는 이벤트. 추가적으로 확장된 정보를 보내줄 때 사용. */
	atUIMSG_CHANGE,				/**< Child 개체가 모종의 변화를 일으킬 때 Owner 개체에게 전달해 주는 이벤트 */

	atUIMSG_KEYDOWN,				/**< 키를 누르는 순간 발생하는 이벤트 */
	atUIMSG_KEYPRESS,				/**< 키를 누르고 있는 동안 지속적으로 일정 간격으로 발생하는 이벤트. KeyDown과 동시에 기본적으로 1개는 발생한다. */
	atUIMSG_LONGKEYDOWN,				/**< 키를 길게 누르고 있을 때 발생하는 이벤트. 지속적으로 발생하지 않고, 단 한번만 발생한다. */
	atUIMSG_KEYUP,				/**< 누르고 있던 키를 떼는 순간 발생하는 이벤트 */
	atUIMSG_KEYCHAR,				/**< ASCII 코드를 갖는 키가 눌렸을 때 발생하는 이벤트 */
	atUIMSG_SYSKEYDOWN,				/**< 특수한 시스템 키를 눌렀을 때 발생하는 이벤트. 모든 플랫폼에 공통으로 적용되지 않는다. */

	atUIMSG_PREMOUSEDOWN,				/**< 마우스, 혹은 터치 스크린을 누르는 순간 전처리를 위해 발생하는 이벤트. 이벤트 적용 대상 개체를 찾아내는데 사용한다. */
	atUIMSG_MOUSEDOWN,				/**< 마우스, 혹은 터치 스크린을 누르는 순간 발생하는 이벤트 */
	atUIMSG_MOUSEUP,				/**< 마우스 혹은 터치 스크린을 떼는 순간 발생하는 이벤트 */
	atUIMSG_MOUSEMOVE,				/**< 마우스를 움직일 때 발생하는 이벤트 */
	atUIMSG_MOUSECLICK,				/**< 마우스 Down과 Up이 동일한 개체의 영역에서 나타났을 때 발생하는 이벤트 */
	atUIMSG_PREMOUSELONGCLICK,				/**< 마우스, 혹은 터치 스크린을 길게 누르고 있으면 발생하는 이벤트. 이벤트 적용 대상 개체를 찾아내는데 사용한다. */
	atUIMSG_MOUSELONGCLICK,				/**< 마우스, 혹은 터치 스크린을 길게 누르고 있으면 발생하는 이벤트 */

	atUIMSG_MAX,
	
	atUIMSG_USERDEF = 256				/**<  */
} atUIMSG;


#ifdef __cplusplus
}
#endif


#endif // __A_UI_MSG_H_

