
extern void init_uiapi_system_oem(void);
extern void init_uiapi_debug_oem(void);
extern void init_uiapi_mem_oem(void);
extern void init_uiapi_gdi_oem(void);
extern void init_uiapi_font_oem(void);
extern void init_uiapi_file_oem(void);
extern void init_uiapi_res_oem(void);
extern void init_uiapi_sound_oem(void);
extern void init_uiapi_str_oem(void);
extern void init_uiapi_proc_oem(void);
extern void init_uiapi_timer_oem(void);
extern void init_uiapi_key_oem(void);
extern void init_uiapi_touch_oem(void);
extern void init_oic_ime_oem(void);
extern void init_oic_touch_oem(void);


void OEM_InitUIAPIfunc(void)
{
	init_uiapi_system_oem();
	init_uiapi_debug_oem();
	init_uiapi_mem_oem();
	init_uiapi_gdi_oem();
	init_uiapi_font_oem();
	init_uiapi_file_oem();
	init_uiapi_res_oem();
	init_uiapi_sound_oem();
	init_uiapi_str_oem();
	init_uiapi_proc_oem();
	init_uiapi_timer_oem();
	init_uiapi_key_oem();
	init_uiapi_touch_oem();
	init_uiapi_ime_oem();
	init_uiapi_oic_oem();
}

