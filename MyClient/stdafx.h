// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �Ǵ� ������Ʈ ���� ���� ������
// ��� �ִ� ���� �����Դϴ�.
//

#pragma once

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
// Windows ��� ����:
#include <windows.h>

// C ��Ÿ�� ��� �����Դϴ�.
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

// TODO: ���α׷��� �ʿ��� �߰� ����� ���⿡�� �����մϴ�.
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
#include "DeviceMgr.h"			//����̽� �Ŵ���
#include "TextureMgr.h"			//�ؽ�ó   �Ŵ���
#include "CameraMgr.h"			//ī�޶�   �Ŵ���
#include "TimeMgr.h"			//Ÿ��	   �Ŵ���
#include "FrameMgr.h"			//������   �Ŵ���
#include "AnimationMgr.h"		//�ִϸ��̼� �Ŵ���
#include "ScriptMgr.h"			//��ũ��Ʈ �Ŵ���
#include "InstanceMgr.h"		//�ν��Ͻ� �Ŵ���
#include "ObjectMgr.h"			//������Ʈ �Ŵ���
#include "ShaderMgr.h"			//���̴�   �Ŵ���
#include "CollisionMgr.h"		//�ݶ��̴� �Ŵ���
#include "SceneMgr.h"			//�� �Ŵ���
#include "KeyMgr.h"				//Ű �Ŵ���
	


#pragma comment(linker,"/entry:wWinMainCRTStartup /subsystem:console")