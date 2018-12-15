typedef uint32_t (*FUNC_TYPE)(uint32_t, void *);	 

typdef struct {
	FUNC_TYPE	tsk[3];
	FUNC_TYPE	tmr[3];
	FUNC_TYPE	get_tick;
	uint8_t *param;
	uint16_t *video_buf;
	uint32_t szbuf;
	uint8_t *mem;
	uint32_t szmem;
} OS_Manager;



