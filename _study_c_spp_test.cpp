// study.debugger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Win32Study.h"
#include "gtest/gtest.h"

/// @breif test entry
int main(int argc, char** argv)
{
	
	::testing::InitGoogleTest(&argc, argv);
#ifdef _DEBUG
	_CrtMemState memoryState = { 0 };
	_CrtMemCheckpoint(&memoryState);
	// _CrtSetBreakAlloc(1045);
#endif //_DEBUG

	// 1. 구조체 바이트 정렬
	// struct_byte_alignment();
	// 2. FIELD_OFFSET를 이해 하기 위한 코드
	// field_offset_example();
	// 3. GetTibInfo And Print TibInfo
	// get_tib_info_and_print();

	int ret = RUN_ALL_TESTS();


	system("pause");
#ifdef _DEBUG
	_CrtMemDumpAllObjectsSince(&memoryState);
#endif //_DEBUG
    return ret;
}

