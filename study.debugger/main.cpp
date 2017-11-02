// study.debugger.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "StructByteAlignment.h"
#include "TIBInfo.h"

int main()
{
	// 1. 구조체 바이트 정렬
	// struct_byte_alignment();
	// 2. FIELD_OFFSET를 이해 하기 위한 코드
 	// field_offset_example();
	// 3. GetTibInfo And Print TibInfo
	get_tib_info_and_print();

	system("pause");
    return 0;
}

