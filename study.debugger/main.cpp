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
	// Zero. 구조체 바이트 정렬
	struct_byte_alignment();
	// First. FIELD_OFFSET를 이해 하기 위한 코드
	field_offset_example();

	system("pause");
    return 0;
}

void field_offset_example()
{
	// ST와 같은 구조체가 있다고 했을때 네 번째에 있는 d의 오프셋을 올바르게 구하기 위한 방법
	INT offset = sizeof(CHAR) + sizeof(CHAR) + sizeof(INT*);
	printf("\n================================\n");
	printf("ST Member d offset(%d) \n", offset);
	// 위와 같이 코드를 작성 했다면 틀렸다.컴파일러 구조체 멤버 정렬값에 따라서 결과가 다르게
	// 나오기 때문이다. 실제 메모리에 구조체는 다음과 같이 할당 된다. 메모리 할당 관련 내용은
	// struct_byte_alignment() 함수 내용을 참고 한다.
	
	//
	offset = FIELD_OFFSET(ST, d);
	printf("ST Member d offset(%d) \n", offset);
	printf("================================\n");
	
	// FIELD_OFFSET 활용 : "name"이라는 변수에 L"some string"이라는 문자열을 복사 해본다.
	const WCHAR* pszName = L"some string";
	INT pszNameLength = wcslen(pszName);
	
	// 첫번째 방법
	{
		// 구조체 전체의 크기에 가변 문자열의 크기를 더해서 메모리를 할당한다.
		// 구조체에 name[1]이 이미 포함되어 있으므로 WCHAR 1개 만큼을 다시 빼주어야 한다.
		ST* p = (ST*)malloc(sizeof(ST) + (pszNameLength * sizeof(WCHAR)) - sizeof(WCHAR));
		memcpy(p->name, pszName, pszNameLength * sizeof(WCHAR));
		p->cbName = pszNameLength * sizeof(WCHAR);
	}
	// 두번째 방법
	{
		// 처음부터 name의 오프셋까지만 얻어낸 뒤 문자열 길이를 더해준다.
		// 첫번째 방법처럼 중복된 WCHAR 만큼을 다시 빼줄 필요가 없다.
		ST* p = (ST*)malloc(FIELD_OFFSET(ST, name) + pszNameLength * sizeof(WCHAR));
		memcpy(p->name, pszName, pszNameLength * sizeof(WCHAR));
		p->cbName = pszNameLength * sizeof(WCHAR);
	}
	// 세번째 방법
	{
		// FIELD_OFFSET에 항상 멤버의 이름만 쓸수 있는 것은 아니다.
		// 아래처럼 배열의 인덱스에 변수를 명시하는 것도 가능하다.
		// 이 때 cb가 아니라 cch를 넣고 있는 것에 유의해야 한다.
		ST* p = (ST*)malloc(FIELD_OFFSET(ST, name[pszNameLength]));
		memcpy(p->name, pszName, pszNameLength * sizeof(WCHAR));
		p->cbName = pszNameLength * sizeof(WCHAR);
	}

	return;
}

void struct_byte_alignment()
{
	// MSDN(https://msdn.microsoft.com/en-us/library/aa273913(v=vs.60).aspx)에 따르면
	// 구조체 패킹의 디폴트 크기값은 8이다. 32비트 운영체제에서는 4바이트이고 64비트 운영체제
	// 에서는 8바이트라고 주장하는 사람들 도 있지만, 디폴트 패킹 크기는 컴파일러가 결정한다.
	// MSVC에서 디폴트 패킹을 8바이트로 정한 이유는(32비트 운영체제에서 조차) 기본 타입 중
	// 가장 큰 타입이 8바이트 이기 때문이다.
	// 구조체의 멤버들은 자신의 크기의 배수로 메모리에 할당 되 는 것이 좋다. 
	// char는 1의 배수, short은 2의 배수 int는 4의 배수, double은 8의 배수의 메모리 번지 
	// 주소에 위치 하고 있을 때 우리는 해당 데이터가 정렬되어 있다고 말한다.

	// struct(ST)의 구조체는 메모리에 다음과 같이 할당된다. 
	// char는 1의 배수에, int는 4의 배수에 double은 8의 배수에 정렬 되어져 올라간다. 
	// 이렇게 할당 될 수 있는 이유는 2바이트 패딩 때문이다.char는 1의 배수인 어느 
	// 주소에나 올라가도 되므로 padding을 집어 넣지 않아도 모든 멤버가 항상 자신이 원하는
	// 주소에 올라가게 된다. 8로 패킹한다는 것은 구조체의 크기를 8의 배수로 맞추겠다는 것
	// 이 아니라, 크기가 8보다 큰 멤버가 있을 때는 정렬을 포기 한다는 것을 뜻한다. 즉, 크기가 8보다
	// 작은 타입에 대해서만 정렬하려고 시도하며, 이것은 다른 말로, 변수의 메모리 주소를 최대 8의 배수로
	// 정렬한다는 뜻이 된다. (참고 블로그 : http://www.benjaminlog.com)

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


	// struct(ST2)의 구조체는 구조체 패킹 사이즈를 1로 변경하여 데이터가 최대 8의 배수로 정렬
	// 되지 않은 것을 확인 할 수 있다. 다만, x86호환 아키텍처에서 윈도우즈 응용프로그램을 만들 경우
	// 정렬이 되어있지 않으므로 CPU가 메모리에 접근하려고 시도하면서 성능이 떨어지게된다.

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