#include <pspkernel.h>
#include <psploadcore.h>

//Newer SDKs only have SceModule (same layout as the old SceModule2)
typedef SceModule SceModule2;

#include <systemctrl.h>
#include <systemctrl_se.h>

#include <string.h>
#include <stdio.h>

#include "localize.h" //bin2c -> localizeruser.prx

#define JAL_OPCODE	0x0C000000
#define J_OPCODE    0x08000000
#define MAKE_CALL(a, f) _sw(JAL_OPCODE | (((u32)(f) >> 2)  & 0x03ffffff), a)
#define MAKE_JUMP(a, f) _sw(J_OPCODE | (((u32)(f) & 0x0ffffffc) >> 2), a)
#define PSP_FIRMWARE(f) ((((f >> 8) & 0xF) << 24) | (((f >> 4) & 0xF) << 16) | ((f & 0xF) << 8) | 0x10)

#include "nidpatch.h"
#include "../structs.h"

PSP_MODULE_INFO("LocalizerKernel", 0x1007, 1, 0);
PSP_MAIN_THREAD_ATTR(0);
PSP_HEAP_SIZE_KB(128);

//Optional debug log (build with -DLOC_DEBUG): ms0:/seplugins/localizer_k.log
#ifdef LOC_DEBUG
static int klog_first = 1;
static void klog(const char *name, u32 value)
{
	static const char hex[] = "0123456789ABCDEF";
	char line[64];
	int i, n = 0;
	for(i = 0; name[i] && i < 40; i++) line[n++] = name[i];
	line[n++] = '=';
	for(i = 28; i >= 0; i -= 4) line[n++] = hex[(value >> i) & 0xF];
	line[n++] = '\r'; line[n++] = '\n';
	SceUID fd = sceIoOpen("ms0:/seplugins/localizer_k.log", PSP_O_WRONLY | PSP_O_CREAT | (klog_first ? PSP_O_TRUNC : PSP_O_APPEND), 0777);
	if(fd < 0) fd = sceIoOpen("ef0:/seplugins/localizer_k.log", PSP_O_WRONLY | PSP_O_CREAT | (klog_first ? PSP_O_TRUNC : PSP_O_APPEND), 0777);
	if(fd >= 0) { sceIoWrite(fd, line, n); sceIoClose(fd); klog_first = 0; }
}
#else
#define klog(name, value) ((void)0)
#endif

//On 6.61 / ARK-5 the user module must not be loaded right away: loading it while the plugin loader is
//still running crashes the console. The (now removed) file logging used to provide this delay by accident.
#define LOAD_DELAY_US 50000

int MainThread(SceSize args, void *argp)
{
	sceKernelDelayThread(LOAD_DELAY_US);
	klog("main_thread", 0);
	//Register order is (size, buffer) for this NID, same as the original build. Newer SDK headers name the args the other way round.
	SceUID thid = sceKernelLoadModuleBuffer((void*)size_localize, (SceSize)localize, 0, NULL);
	klog("loaded_uid", thid);
	if(thid >= 0)
	{
		int status = 0;
		int ret = sceKernelStartModule(thid, 0, NULL, &status, NULL);
		(void)ret;
		klog("start_ret", ret);
		klog("start_status", status);
	}
	sceKernelExitDeleteThread(0);
	return 0;
}

int module_start(SceSize args, void *argp)
{
	klog("kernel_module_start", 0);
	sceKernelDelayThread(LOAD_DELAY_US);
	SceUID thid = sceKernelCreateThread("loc_kernel", MainThread, 0x18, 0x4000, 0, NULL);
	klog("thread_created", thid);
	if(thid >= 0) sceKernelStartThread(thid, 0, NULL);
	return 0;
}
