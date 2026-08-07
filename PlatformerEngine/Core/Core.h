#pragma once

// 4251 경고 끄기 - template을 dll로 export하면 무조건 발생하는 경고 -> 근데 방법이 없음
#pragma warning(disable:4251)

// 빌드 모드에 따라서 변경될 값.
#define DLLEXPORT	__declspec(dllexport)
#define DLLIMPORT	__declspec(dllimport)

// 특정 값에 따라 분기되도록 처리
#if ENGINE_BUILD_DLL
#define PLATFORMER_API	DLLEXPORT
#else
#define PLATFORMER_API	DLLIMPORT
#endif