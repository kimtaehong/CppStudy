#include <stdafx.h>

#include <winsock2.h>
#include <iphlpapi.h>
#include <lm.h>
#include <Msi.h>
#pragma comment(lib, "netapi32.lib")
#pragma comment(lib, "IPHLPAPI.lib")
#pragma comment(lib, "Msi.lib")

///
///@brief ���� ��ǻ���� ������ ���� ������ ȹ�� �Ѵ�.
///@remark Active Directory ȯ�濡���� ACL(Access Control List)�� ���ؼ� 
///		   ���ǰų� ���� �� �� �ִ�. �⺻ ACL ��å�� ������ ����� Ȥ�� "Pr
///		   e-Windows 2000 compatible access" �׷쿡 ���� ������ ��� ������
///		   ����.
///		   https://msdn.microsoft.com/ko-kr/library/windows/desktop/aa370652(v=vs.85).aspx)
///
bool get_local_account_lists()
{
	LPUSER_INFO_0 pBuf			 = NULL;
	DWORD		  dwPrefMaxLen	 = MAX_PREFERRED_LENGTH;
	DWORD		  dwEntriesRead  = 0;
	DWORD		  dwTotalEntries = 0;
	DWORD		  dwResumeHandle = 0;
	if (NERR_Success != NetUserEnum(NULL,
									0,
									FILTER_NORMAL_ACCOUNT,
									(LPBYTE*)(&pBuf),
									dwPrefMaxLen,
									&dwEntriesRead,
									&dwTotalEntries,
									&dwResumeHandle))
	{
		// error
		return false;
	}

	LPUSER_INFO_0 pTmpBuf = pBuf;
	
	for (DWORD count = 0; count < dwEntriesRead; count++)
	{
		LPUSER_INFO_4 pAccountInfo = NULL;
		if (NERR_Success != NetUserGetInfo(NULL,
										pBuf->usri0_name,
										4,
										(LPBYTE*)&pAccountInfo))
		{
			continue;
		}

		pTmpBuf++;
	}
	
	if (NULL != pBuf) { NetApiBufferFree(pBuf); pBuf = NULL; }

	return true;
}

LPSTR MsiQueryProperty(_In_ LPCTSTR szProductCode,
					   _In_ LPCTSTR szUserSid,
					   _In_ MSIINSTALLCONTEXT dwContext,
					   _In_ LPCTSTR szProperty);

bool get_install_programs()
{
	WCHAR szSid[128] = { 0 };
	WCHAR szInstalledProductCode[39] = { 0 };
	BYTE  retValue	 = 0;
	DWORD cchSid	 = 0;
	DWORD dwIndex	 = 0;
	DWORD dwContext  = MSIINSTALLCONTEXT_USERMANAGED | MSIINSTALLCONTEXT_USERUNMANAGED | MSIINSTALLCONTEXT_MACHINE;
	
	MSIINSTALLCONTEXT dwInstalledContext;
	do {
		cchSid	 = sizeof(szSid) / sizeof(szSid[0]);

		retValue = MsiEnumProductsExW(NULL,
									  L"s-1-1-0",
									  dwContext,
									  dwIndex,
									  szInstalledProductCode,
									  &dwInstalledContext,
									  szSid,
									  &cchSid);

		if (ERROR_SUCCESS == retValue)
		{
			int temp = 0;
		}
	} while (ERROR_SUCCESS == retValue);

	if (ERROR_ACCESS_DENIED == retValue)
	{
		// error
		return false;
	}

	return true;
}

PSTR MsiQueryProperty(
	_In_ LPCTSTR szProductCode,
	_In_ LPCTSTR szUserSid,
	_In_ MSIINSTALLCONTEXT dwContext,
	_In_ LPCTSTR szProperty)
{

}