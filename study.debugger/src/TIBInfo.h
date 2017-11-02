#pragma once

#ifdef _WIN64
#define OFF_ExceptionList			0x000
#define OFF_StackBase				0x008
#define OFF_StackLimit				0x010
#define OFF_Self					0x030

#define OFF_ProcessEnvironmentBlock 0x0060
#define OFF_LastErrorValue			0x0068
#define OFF_CurrentLocale			0x0108
#define OFF_TlsSlots				0x1480 // TLS 슬롯을 담기 위한 64개의 PVOID 배열
#define OFF_TlsExpansionSlots		0x1780 // 1024개의 확장 TLS 슬롯 버퍼를 위한 포인터
#else
#define OFF_ExceptionList			0x000
#define OFF_StackBase				0x004
#define OFF_StackLimit				0x008
#define OFF_Self					0x018

#define OFF_ProcessEnvironmentBlock 0x0030
#define OFF_LastErrorValue			0x0034
#define OFF_CurrentLocale			0x00c4
#define OFF_TlsSlots				0x0e10 // TLS 슬롯을 담기 위한 64개의 PVOID 배열
#define OFF_TlsExpansionSlots		0x0f6c // 1024개의 확장 TLS 슬롯 버퍼를 위한 포인터
#endif // __WIN64

/*
32비트에서 TLB의 실제 번지를 획득하는 인라인 어셈블리 코드다.
PBYTE pTib = NULL;
_asm
{
mov eax, fs:[0x018]
mov pTib, eax
}

64비트의 경우 C/C++ 컴파일러는 인라인 어셈블러를 지원하지 않기 때문에 "GS:[0X030]" 형식으로는
TIB의 포인터를 획득 할 수 없다.

PTEB NtCurrentTeb(void);
---> NtCurrentTeb함수는 인라인 함수로, "WinNT.h"에 다음과 같이 선언 되어 있다.

64bit 정의
__forceinline struct _TEB* NtCurrentTeb(VOID)
{
return (struct _TEB*) __readgsqword(FIELD_OFFSET(NT_TIB, Self));
}

32bit 정의
__forceinline struct _TEB* NtCurrentTeb(VOID)
{
return (struct _TEB*) (ULONG_PTR) __readfsdword(NT_TIB, Self);
}


*/

static const PCWSTR GSZ_FLDNAMES[] =
{
	L"ExceptionList", L"StackBase", L"StackLimit",
	L"Self", L"PEB", L"LastErrorValue",
	L"CurrentLocale", L"TlsSlots", L"TlsExpansionSlots"
};

static const INT GN_FLDOFFSETS[] =
{
	OFF_ExceptionList, OFF_StackBase, OFF_StackBase,
	OFF_Self, OFF_ProcessEnvironmentBlock, OFF_LastErrorValue,
	OFF_CurrentLocale, OFF_TlsSlots, OFF_TlsExpansionSlots
};

void get_tib_info_and_print();