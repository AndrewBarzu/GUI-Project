#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#define DEBUG_NEW new( _NORMAL_BLOCK, __FILE__, __LINE__ )
#endif
#ifndef _DEBUG
#define DEBUG_NEW new
#endif