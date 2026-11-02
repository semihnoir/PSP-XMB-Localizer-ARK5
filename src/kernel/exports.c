#include <pspmoduleexport.h>
#define NULL ((void *) 0)

extern void module_start;
extern void module_info;
static const unsigned int __syslib_exports[4] __attribute__((section(".rodata.sceResident"))) = {
	0xD632ACDB,
	0xF01D73A7,
	(unsigned int) &module_start,
	(unsigned int) &module_info,
};

extern void LLibClearCaches;
extern void LLibPatchExports;
extern void LLibFindModuleByName;
extern void LLibFindProc;
static const unsigned int __LocalizerLib_exports[8] __attribute__((section(".rodata.sceResident"))) = {
	0xB3169E4E,
	0x94F32CE0,
	0xEEA61CB7,
	0xBE2A6AC6,
	(unsigned int) &LLibClearCaches,
	(unsigned int) &LLibPatchExports,
	(unsigned int) &LLibFindModuleByName,
	(unsigned int) &LLibFindProc,
};

const struct _PspLibraryEntry __library_exports[2] __attribute__((section(".lib.ent"), used)) = {
	{ NULL, 0x0000, 0x8000, 4, 1, 1, &__syslib_exports },
	{ "LocalizerLib", 0x0000, 0x4001, 4, 0, 4, &__LocalizerLib_exports },
};
