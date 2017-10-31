// study.debugger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

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

typedef struct tagST
{
	CHAR  a;
	CHAR  b;
	INT*  c;
	INT64 d;
	INT	  cbName;
	WCHAR name[1];
} ST;

#pragma pack(push, 1)
typedef struct tagST2
{
	CHAR  a;
	CHAR  b;
	INT*  c;
	INT64 d;
	INT	  cbName;
	WCHAR name[1];
} ST2;
#pragma pack(pop)

void struct_byte_alignment();
void field_offset_example();

int main()
{
	// Zero. ����ü ����Ʈ ����
	struct_byte_alignment();
	// First. FIELD_OFFSET�� ���� �ϱ� ���� �ڵ�
	field_offset_example();

	system("pause");
    return 0;
}

void field_offset_example()
{
	// ST�� ���� ����ü�� �ִٰ� ������ �� ��°�� �ִ� d�� �������� �ùٸ��� ���ϱ� ���� ���
	INT offset = sizeof(CHAR) + sizeof(CHAR) + sizeof(INT*);
	printf("\n================================\n");
	printf("ST Member d offset(%d) \n", offset);
	// ���� ���� �ڵ带 �ۼ� �ߴٸ� Ʋ�ȴ�.�����Ϸ� ����ü ��� ���İ��� ���� ����� �ٸ���
	// ������ �����̴�. ���� �޸𸮿� ����ü�� ������ ���� �Ҵ� �ȴ�. �޸� �Ҵ� ���� ������
	// struct_byte_alignment() �Լ� ������ ���� �Ѵ�.
	
	//
	offset = FIELD_OFFSET(ST, d);
	printf("ST Member d offset(%d) \n", offset);
	printf("================================\n");
	
	// FIELD_OFFSET Ȱ�� : "name"�̶�� ������ L"some string"�̶�� ���ڿ��� ���� �غ���.
	const WCHAR* pszName = L"some string";
	INT pszNameLength = wcslen(pszName);
	
	// ù��° ���
	{
		// ����ü ��ü�� ũ�⿡ ���� ���ڿ��� ũ�⸦ ���ؼ� �޸𸮸� �Ҵ��Ѵ�.
		// ����ü�� name[1]�� �̹� ���ԵǾ� �����Ƿ� WCHAR 1�� ��ŭ�� �ٽ� ���־�� �Ѵ�.
		ST* p = (ST*)malloc(sizeof(ST) + (pszNameLength * sizeof(WCHAR)) - sizeof(WCHAR));
		memcpy(p->name, pszName, pszNameLength * sizeof(WCHAR));
		p->cbName = pszNameLength * sizeof(WCHAR);
	}
	// �ι�° ���
	{
		// ó������ name�� �����±����� �� �� ���ڿ� ���̸� �����ش�.
		// ù��° ���ó�� �ߺ��� WCHAR ��ŭ�� �ٽ� ���� �ʿ䰡 ����.
		ST* p = (ST*)malloc(FIELD_OFFSET(ST, name) + pszNameLength * sizeof(WCHAR));
		memcpy(p->name, pszName, pszNameLength * sizeof(WCHAR));
		p->cbName = pszNameLength * sizeof(WCHAR);
	}
	// ����° ���
	{
		// FIELD_OFFSET�� �׻� ����� �̸��� ���� �ִ� ���� �ƴϴ�.
		// �Ʒ�ó�� �迭�� �ε����� ������ ����ϴ� �͵� �����ϴ�.
		// �� �� cb�� �ƴ϶� cch�� �ְ� �ִ� �Ϳ� �����ؾ� �Ѵ�.
		ST* p = (ST*)malloc(FIELD_OFFSET(ST, name[pszNameLength]));
		memcpy(p->name, pszName, pszNameLength * sizeof(WCHAR));
		p->cbName = pszNameLength * sizeof(WCHAR);
	}

	return;
}

void struct_byte_alignment()
{
	// MSDN(https://msdn.microsoft.com/en-us/library/aa273913(v=vs.60).aspx)�� ������
	// ����ü ��ŷ�� ����Ʈ ũ�Ⱚ�� 8�̴�. 32��Ʈ �ü�������� 4����Ʈ�̰� 64��Ʈ �ü��
	// ������ 8����Ʈ��� �����ϴ� ����� �� ������, ����Ʈ ��ŷ ũ��� �����Ϸ��� �����Ѵ�.
	// MSVC���� ����Ʈ ��ŷ�� 8����Ʈ�� ���� ������(32��Ʈ �ü������ ����) �⺻ Ÿ�� ��
	// ���� ū Ÿ���� 8����Ʈ �̱� �����̴�.
	// ����ü�� ������� �ڽ��� ũ���� ����� �޸𸮿� �Ҵ� �� �� ���� ����. 
	// char�� 1�� ���, short�� 2�� ��� int�� 4�� ���, double�� 8�� ����� �޸� ���� 
	// �ּҿ� ��ġ �ϰ� ���� �� �츮�� �ش� �����Ͱ� ���ĵǾ� �ִٰ� ���Ѵ�.

	// struct(ST)�� ����ü�� �޸𸮿� ������ ���� �Ҵ�ȴ�. 
	// char�� 1�� �����, int�� 4�� ����� double�� 8�� ����� ���� �Ǿ��� �ö󰣴�. 
	// �̷��� �Ҵ� �� �� �ִ� ������ 2����Ʈ �е� �����̴�.char�� 1�� ����� ��� 
	// �ּҿ��� �ö󰡵� �ǹǷ� padding�� ���� ���� �ʾƵ� ��� ����� �׻� �ڽ��� ���ϴ�
	// �ּҿ� �ö󰡰� �ȴ�. 8�� ��ŷ�Ѵٴ� ���� ����ü�� ũ�⸦ 8�� ����� ���߰ڴٴ� ��
	// �� �ƴ϶�, ũ�Ⱑ 8���� ū ����� ���� ���� ������ ���� �Ѵٴ� ���� ���Ѵ�. ��, ũ�Ⱑ 8����
	// ���� Ÿ�Կ� ���ؼ��� �����Ϸ��� �õ��ϸ�, �̰��� �ٸ� ����, ������ �޸� �ּҸ� �ִ� 8�� �����
	// �����Ѵٴ� ���� �ȴ�. (���� ��α� : http://www.benjaminlog.com)

	// 0  a(1)  1  b(1)  2   padding(2)   4           c(4)                                       
	// |--------|--------|----------------|--------------------------------|
	// 8							d(8) 						     
	// |----------------------------------------------------------------|
	// 16          c(4)					20		d(2)
	// |--------------------------------|----------------|
	
	ST st;
	
	printf("================================\n");
	printf("ST.a 		 : type(char)  align(%d) address(0x%X) offset(%d) \n", __alignof(char), &st.a, FIELD_OFFSET(ST, a));
	printf("ST.b 		 : type(char)  align(%d) address(0x%X) offset(%d) \n", __alignof(char), &st.b, FIELD_OFFSET(ST, b));
	printf("ST.c 		 : type(INT)   align(%d) address(0x%X) offset(%d) \n", __alignof(INT*), &st.c, FIELD_OFFSET(ST, c));
	printf("ST.d 		 : type(INT64) align(%d) address(0x%X) offset(%d) \n", __alignof(INT64), &st.d, FIELD_OFFSET(ST, d));
	printf("ST.cbName	 : type(INT)   align(%d) address(0x%X) offset(%d) \n", __alignof(INT), &st.cbName, FIELD_OFFSET(ST, cbName));
	printf("ST.name		 : type(WCHAR) align(%d) address(0x%X) offset(%d) \n", __alignof(WCHAR), &st.name, FIELD_OFFSET(ST, name));
	printf("================================\n");


	// struct(ST2)�� ����ü�� ����ü ��ŷ ����� 1�� �����Ͽ� �����Ͱ� �ִ� 8�� ����� ����
	// ���� ���� ���� Ȯ�� �� �� �ִ�. �ٸ�, x86ȣȯ ��Ű��ó���� �������� �������α׷��� ���� ���
	// ������ �Ǿ����� �����Ƿ� CPU�� �޸𸮿� �����Ϸ��� �õ��ϸ鼭 ������ �������Եȴ�.

	ST2 st2;
	printf("================================\n");
	printf("ST.a 		 : type(char)  align(%d) address(0x%X) offset(%d) \n", __alignof(char), &st2.a, FIELD_OFFSET(ST2, a));
	printf("ST.b 		 : type(char)  align(%d) address(0x%X) offset(%d) \n", __alignof(char), &st2.b, FIELD_OFFSET(ST2, b));
	printf("ST.c 		 : type(INT*)  align(%d) address(0x%X) offset(%d) \n", __alignof(INT*), &st2.c, FIELD_OFFSET(ST2, c));
	printf("ST.d 		 : type(INT64) align(%d) address(0x%X) offset(%d) \n", __alignof(INT64), &st2.d, FIELD_OFFSET(ST2, d));
	printf("ST.cbName	 : type(INT)   align(%d) address(0x%X) offset(%d) \n", __alignof(INT), &st2.cbName, FIELD_OFFSET(ST2, cbName));
	printf("ST.name		 : type(WCHAR) align(%d) address(0x%X) offset(%d) \n", __alignof(WCHAR), &st2.name, FIELD_OFFSET(ST2, name));
	printf("================================\n");

}