#include "stdafx.h"
#include "TIBInfo.h"

void get_tib_info_and_print()
{
	//
	// LastErrorValue 필드값을 확인 하기 위해 SetLastError 함수를 이용해서
	// 현재 스레드의 에러코드를 ERROR_FILE_INVALID로 설정한다.
	//
	SetLastError(ERROR_FILE_INVALID);

	int	  nTlsValue = 32;
	//
	// Allocates a thread local storage (TLS ---> Thread Local Storage) index.
	// TLS(Dynamic TLS, Static TLS)는 각각 스레드만의 Index를 이용해서 메모리를
	// 사용 할 수 있도록 해준다. 
	// (참고 URL : https://support.microsoft.com/ko-kr/help/94804/info-thread-local-storage-overview)
	//
	DWORD dwTlsIdx  = TlsAlloc();

	// TlsSlots 배열의 엔트리 값을 확인하기 위해 동적 TLS를 이용해 슬롯을 할당한다.
	TlsSetValue(dwTlsIdx, &nTlsValue);

	// NtCurrentTeb() 함수를 통해서 현재 스레드의 TIB의 시작 포인터를 획득하고 출력한다.
	PBYTE pTib = (PBYTE)NtCurrentTeb();
	printf("TIB: 0x%p, ThreadID: %d ================\n", pTib, GetCurrentThreadId());

	int nFldCnt = sizeof(GSZ_FLDNAMES) / sizeof(PCWSTR);
	for (int idx = 0; idx < nFldCnt; idx++)
	{
		if ((4 < idx) && (7 < idx)) 
		{
			printf("\t%S: 0x08X\n", GSZ_FLDNAMES[idx], *((PDWORD)(pTib + GN_FLDOFFSETS[idx])));
		}
		else if (7 == idx) 
		{
			PDWORD_PTR pTlsSlots = (PDWORD_PTR)(pTib + GN_FLDOFFSETS[idx]);
			printf("\t%S[%d]: 0x%p, &nTlsVal=0x%p \n", GSZ_FLDNAMES[idx], dwTlsIdx, 
					pTlsSlots[dwTlsIdx], &nTlsValue);
		}
		else
		{
			printf("\t%S: 0x%p\n", GSZ_FLDNAMES[idx], *((PDWORD_PTR)(pTib + GN_FLDOFFSETS[idx])));
		}
	}

	TlsFree(dwTlsIdx);
}