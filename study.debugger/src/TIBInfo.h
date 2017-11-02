#pragma once

#ifdef _WIN64
#define OFF_ExceptionList			0x000
#define OFF_StackBase				0x008
#define OFF_StackLimit				0x010
#define OFF_Self					0x030

#define OFF_ProcessEnvironmentBlock 0x0060
#define OFF_LastErrorValue			0x0068
#define OFF_CurrentLocale			0x0108
#define OFF_TlsSlots				0x1480 // TLS ������ ��� ���� 64���� PVOID �迭
#define OFF_TlsExpansionSlots		0x1780 // 1024���� Ȯ�� TLS ���� ���۸� ���� ������
#else
#define OFF_ExceptionList			0x000
#define OFF_StackBase				0x004
#define OFF_StackLimit				0x008
#define OFF_Self					0x018

#define OFF_ProcessEnvironmentBlock 0x0030
#define OFF_LastErrorValue			0x0034
#define OFF_CurrentLocale			0x00c4
#define OFF_TlsSlots				0x0e10 // TLS ������ ��� ���� 64���� PVOID �迭
#define OFF_TlsExpansionSlots		0x0f6c // 1024���� Ȯ�� TLS ���� ���۸� ���� ������
#endif // __WIN64

/*
32��Ʈ���� TLB�� ���� ������ ȹ���ϴ� �ζ��� ����� �ڵ��.
PBYTE pTib = NULL;
_asm
{
mov eax, fs:[0x018]
mov pTib, eax
}

64��Ʈ�� ��� C/C++ �����Ϸ��� �ζ��� ������� �������� �ʱ� ������ "GS:[0X030]" �������δ�
TIB�� �����͸� ȹ�� �� �� ����.

PTEB NtCurrentTeb(void);
---> NtCurrentTeb�Լ��� �ζ��� �Լ���, "WinNT.h"�� ������ ���� ���� �Ǿ� �ִ�.

64bit ����
__forceinline struct _TEB* NtCurrentTeb(VOID)
{
return (struct _TEB*) __readgsqword(FIELD_OFFSET(NT_TIB, Self));
}

32bit ����
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