#include "stdafx.h"
#include "TIBInfo.h"

void get_tib_info_and_print()
{
	//
	// LastErrorValue �ʵ尪�� Ȯ�� �ϱ� ���� SetLastError �Լ��� �̿��ؼ�
	// ���� �������� �����ڵ带 ERROR_FILE_INVALID�� �����Ѵ�.
	//
	SetLastError(ERROR_FILE_INVALID);

	int	  nTlsValue = 32;
	//
	// Allocates a thread local storage (TLS ---> Thread Local Storage) index.
	// TLS(Dynamic TLS, Static TLS)�� ���� �����常�� Index�� �̿��ؼ� �޸𸮸�
	// ��� �� �� �ֵ��� ���ش�. 
	// (���� URL : https://support.microsoft.com/ko-kr/help/94804/info-thread-local-storage-overview)
	//
	DWORD dwTlsIdx  = TlsAlloc();

	// TlsSlots �迭�� ��Ʈ�� ���� Ȯ���ϱ� ���� ���� TLS�� �̿��� ������ �Ҵ��Ѵ�.
	TlsSetValue(dwTlsIdx, &nTlsValue);

	// NtCurrentTeb() �Լ��� ���ؼ� ���� �������� TIB�� ���� �����͸� ȹ���ϰ� ����Ѵ�.
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