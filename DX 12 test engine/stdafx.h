#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <Windows.h>
#include <d3d12.h>
#include <dxgi1_4.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <thread>
#include "d3dx12.h"
//safe release relaese something only if it exists in the first place
#define SAFE_RELEASE(p){ if((p)) {(p)->Release();(p) = 0;}}
