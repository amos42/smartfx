#include "bsp_key.h"

#define KEY_ST_PORT								D_K_ST_GPIO_Port
#define KEY_ST_PIN									D_K_ST_Pin


#define KEY_RE_PORT								D_K_RE_GPIO_Port
#define KEY_RE_PIN									D_K_RE_Pin

#define KEY_PWR_KEY_PORT				D_PWR_KEY_GPIO_Port
#define KEY_PWR_KEY_PIN					D_PWR_KEY_Pin
							   
							   
#define KEY_STS_PUSH				0		// key를 눌렀을 때 key 값
#define KEY_STS_RELEASE		1		// key를 안눌렀을 때 key 값
							   
//static uint8_t flag_key = 0;
static uint32_t key_st_cnt = 0;

uint8_t key_st_get(void)
{
	static uint8_t key = KEY_NONE;
	static uint8_t key_prev = KEY_STS_RELEASE, key_cur = KEY_STS_RELEASE;	 // 초기 key값이 high면 1

	key_cur = HAL_GPIO_ReadPin(KEY_ST_PORT, KEY_ST_PIN);
	
	if (key_prev == KEY_STS_RELEASE && key_cur == KEY_STS_RELEASE) // none
	{
		key = KEY_NONE;
		if (key_st_cnt < 0xffffffff) 
		{
			key_st_cnt++;
		}
	} 
	else if (key_prev == KEY_STS_RELEASE && key_cur == KEY_STS_PUSH) // push
	{
		key = KEY_PUSH;
		key_st_cnt = 0;
	}
	else if (key_prev == KEY_STS_PUSH && key_cur == KEY_STS_RELEASE) // release	
	{
		key = KEY_RELEASE;
		key_st_cnt = 0;
	}
	else if (key_prev == KEY_STS_PUSH && key_cur == KEY_STS_PUSH) // hold
	{
		key = KEY_HOLD;

		if (key_st_cnt < 0xffffffff) 
		{
			key_st_cnt++;
		}
	}

	key_prev = key_cur;

	return key;
}

uint32_t key_st_count(void)
{
	return key_st_cnt;
}

/////////////////////////////////
static uint32_t key_re_cnt = 0;

uint8_t key_re_get(void)
{
	static uint8_t key = KEY_NONE;
	static uint8_t key_prev = KEY_STS_RELEASE, key_cur = KEY_STS_RELEASE;	 // 초기 key값이 high면 1

	key_cur = HAL_GPIO_ReadPin(KEY_RE_PORT, KEY_RE_PIN);
	
	if (key_prev == KEY_STS_RELEASE && key_cur == KEY_STS_RELEASE) // none
	{
		key = KEY_NONE;
		if (key_re_cnt < 0xffffffff) 
		{
			key_re_cnt++;
		}
	} 
	else if (key_prev == KEY_STS_RELEASE && key_cur == KEY_STS_PUSH) // push
	{
		key = KEY_PUSH;
		key_re_cnt = 0;
	}
	else if (key_prev == KEY_STS_PUSH && key_cur == KEY_STS_RELEASE) // release	
	{
		key = KEY_RELEASE;
		key_re_cnt = 0;
	}
	else if (key_prev == KEY_STS_PUSH && key_cur == KEY_STS_PUSH) // hold
	{
		key = KEY_HOLD;

		if (key_re_cnt < 0xffffffff) 
		{
			key_re_cnt++;
		}
	}

	key_prev = key_cur;

	return key;
}

uint32_t key_re_count(void)
{
	return key_re_cnt;
}

/////////////////////////////////

#define KEY_PWR_RELEASE		0
#define KEY_PWR_PUSH			1

static uint32_t key_pwr_key_cnt = 0;

uint8_t key_pwr_key_get(void)
{
	static uint8_t key = KEY_NONE;
	static uint8_t key_prev = KEY_PWR_RELEASE, key_cur = KEY_PWR_RELEASE;	 // 초기 key값이 high면 1

	key_cur = HAL_GPIO_ReadPin(KEY_PWR_KEY_PORT, KEY_PWR_KEY_PIN);
	
	//printf("k:%d\r\n", key_cur);
	
	if (key_prev == KEY_PWR_RELEASE && key_cur == KEY_PWR_RELEASE) // none
	{
		key = KEY_NONE;
		if (key_pwr_key_cnt < 0xffffffff) 
		{
			key_pwr_key_cnt++;
		}
	} 
	else if (key_prev == KEY_PWR_RELEASE && key_cur == KEY_PWR_PUSH) // push
	{
		key = KEY_PUSH;
		key_pwr_key_cnt = 0;
	}
	else if (key_prev == KEY_PWR_PUSH && key_cur == KEY_PWR_RELEASE) // release	
	{
		key = KEY_RELEASE;
		key_pwr_key_cnt = 0;
	}
	else if (key_prev == KEY_PWR_PUSH && key_cur == KEY_PWR_PUSH) // hold
	{
		key = KEY_HOLD;

		if (key_pwr_key_cnt < 0xffffffff) 
		{
			key_pwr_key_cnt++;
		}
	}

	key_prev = key_cur;

	return key;
}

uint32_t key_pwr_key_count(void)
{
	return key_pwr_key_cnt;
}

