// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// Standard Headers
#include <string>
#include <list>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <iostream>
#include <utility>
#include <memory>
#include <fstream>

// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
using namespace std;

// DirectX
#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include <DirectXMath.h>
using namespace DirectX;

// User Headers
#include "Extern.h"
#include "Define.h"
#include "Enum.h"
#include "Struct.h"
#include "Function.h"
#include "Functor.h"
#include "Vertex.h"
#include "Math.h"


// Managers
#include "DeviceMgr.h"			//디바이스 매니저
#include "TextureMgr.h"			//텍스처   매니저
#include "CameraMgr.h"			//카메라   매니저
#include "TimeMgr.h"			//타임	   매니저
#include "FrameMgr.h"			//프레임   매니저
#include "AnimationMgr.h"		//애니메이션 매니저
#include "ScriptMgr.h"			//스크립트 매니저
#include "InstanceMgr.h"		//인스턴스 매니저
#include "ObjectMgr.h"			//오브젝트 매니저
#include "ShaderMgr.h"			//쉐이더   매니저
#include "CollisionMgr.h"		//콜라이더 매니저
#include "SceneMgr.h"			//씬 매니저
#include "KeyMgr.h"				//키 매니저
	


#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")