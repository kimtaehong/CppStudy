#pragma once

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