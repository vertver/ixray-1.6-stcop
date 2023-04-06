#include "stdafx.h"
#pragma hdrstop

#include "cpuid.h"

#include <intrin.h>

/***
*
* int _cpuid (_p_info *pinfo)
* 
* Entry:
*
*   pinfo: pointer to _p_info, NULL is not allowed!
*
* Exit:
*
*   Returns int with capablity bit set.
*
****************************************************/
int _cpuid(_processor_info *pinfo) {
	_processor_info& processorInfo = *pinfo;
	ZeroMemory(&processorInfo, sizeof(_processor_info));

	int cpinfo[4];

	// Detect vender
	__cpuid(cpinfo, 0);
	memcpy(processorInfo.v_name, &(cpinfo[1]), sizeof(int));
	memcpy(processorInfo.v_name + sizeof(int), &(cpinfo[3]), sizeof(int));
	memcpy(processorInfo.v_name + 2 * sizeof(int), &(cpinfo[2]), sizeof(int));

	// Detect model
	__cpuid(cpinfo, 0x80000002);
	memcpy(processorInfo.model_name, cpinfo, sizeof(cpinfo));
	__cpuid(cpinfo, 0x80000003);
	memcpy(processorInfo.model_name + 16, cpinfo, sizeof(cpinfo));
	__cpuid(cpinfo, 0x80000004);
	memcpy(processorInfo.model_name + 32, cpinfo, sizeof(cpinfo));

	// Detect main features
	__cpuid(cpinfo, 1);
	processorInfo.stepping = cpinfo[0] & 0xf;
	processorInfo.model =
		(u8)  ((cpinfo[0] >> 4)  & 0xf) |
		((u8) ((cpinfo[0] >> 16) & 0xf) << 4);
	processorInfo.family =
		(u8)  ((cpinfo[0] >> 8)  & 0xf) |
		((u8) ((cpinfo[0] >> 20) & 0xff) << 4);

	if (cpinfo[3] & (1 << 23))
		processorInfo.feature |= _CPU_FEATURE_MMX;
	if (cpinfo[3] & (1 << 25))
		processorInfo.feature |= _CPU_FEATURE_SSE;
	if (cpinfo[3] & (1 << 26))
		processorInfo.feature |= _CPU_FEATURE_SSE2;
	if (cpinfo[2] & 0x1)
		processorInfo.feature |= _CPU_FEATURE_SSE3;
	if (cpinfo[2] & (1 << 19))
		processorInfo.feature |= _CPU_FEATURE_SSE4_1;
	if (cpinfo[2] & (1 << 20))
		processorInfo.feature |= _CPU_FEATURE_SSE4_2;
	if (cpinfo[2] & (1 << 9))
		processorInfo.feature |= _CPU_FEATURE_SSSE3;
	if (cpinfo[2] & (1 << 3))
		processorInfo.feature |= _CPU_FEATURE_MWAIT;

	// Detect 3DNow! support
	__cpuid(cpinfo, 0x80000001);
	if (cpinfo[3] & (1 << 31))
		processorInfo.feature |= _CPU_FEATURE_3DNOW;

	return processorInfo.feature;
}
