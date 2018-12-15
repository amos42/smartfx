
extern void init_sysapi_system_oem(void);
extern void init_sysapi_sysutil_oem(void);
extern void init_sysapi_debug_oem(void);
extern void init_sysapi_mem_oem(void);
extern void init_sysapi_scr_oem(void);
extern void init_sysapi_file_oem(void);
extern void init_sysapi_sound_oem(void);
extern void init_sysapi_str_oem(void);
extern void init_sysapi_task_oem(void);
extern void init_sysapi_timer_oem(void);
extern void init_sysapi_key_oem(void);
extern void init_sysapi_touch_oem(void);
extern void init_sysapi_gps_oem(void);


void OEM_InitSYSAPIfunc(void)
{
	init_sysapi_system_oem();
	init_sysapi_sysutil_oem();
	init_sysapi_debug_oem();
	init_sysapi_mem_oem();
	init_sysapi_scr_oem();
	init_sysapi_file_oem();
	init_sysapi_sound_oem();
	init_sysapi_str_oem();
	init_sysapi_task_oem();
	init_sysapi_timer_oem();
	init_sysapi_key_oem();
	init_sysapi_touch_oem();
	init_sysapi_gps_oem();
}

