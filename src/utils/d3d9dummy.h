#pragma once
#ifndef D3D9DUMMY_H
#define D3D9DUMMY_H

#include <pch.h>
#include "vmtmgr.h"

typedef enum {
	ifnQueryInterface, ifnAddRef, ifnRelease, ifnTestCooperativeLevel, ifnGetAvailableTextureMem,
	ifnEvictManagedResources, ifnGetDirect3D, ifnGetDeviceCaps, ifnGetDisplayMode, ifnGetCreationParameters,
	ifnSetCursorProperties, ifnSetCursorPosition, ifnShowCursor, ifnCreateAdditionalSwapChain, ifnGetSwapChain,
	ifnGetNumberOfSwapChains, ifnReset, ifnPresent, ifnGetBackBuffer, ifnGetRasterStatus, ifnSetDialogBoxMode,
	ifnSetGammaRamp, ifnGetGammaRamp, ifnCreateTexture, ifnCreateVolumeTexture, ifnCreateCubeTexture,
	ifnCreateVertexBuffer, ifnCreateIndexBuffer, ifnCreateRenderTarget, ifnCreateDepthStencilSurface,
	ifnUpdateSurface, ifnUpdateTexture, ifnGetRenderTargetData, ifnGetFrontBufferData, ifnStretchRect,
	ifnColorFill, ifnCreateOffscreenPlainSurface, ifnSetRenderTarget, ifnGetRenderTarget,
	ifnSetDepthStencilSurface, ifnGetDepthStencilSurface, ifnBeginScene, ifnEndScene, ifnClear,
	ifnSetTransform, ifnGetTransform, ifnMultiplyTransform, ifnSetViewport, ifnGetViewport,
	ifnSetMaterial, ifnGetMaterial, ifnSetLight, ifnGetLight, ifnLightEnable, ifnGetLightEnable,
	ifnSetClipPlane, ifnGetClipPlane, ifnSetRenderState, ifnGetRenderState, ifnCreateStateBlock,
	ifnBeginStateBlock, ifnEndStateBlock, ifnSetClipStatus, ifnGetClipStatus, ifnGetTexture,
	ifnSetTexture, ifnGetTextureStageState, ifnSetTextureStageState, ifnGetSamplerState, ifnSetSamplerState,
	ifnValidateDevice, ifnSetPaletteEntries, ifnGetPaletteEntries, ifnSetCurrentTexturePalette,
	ifnGetCurrentTexturePalette, ifnSetScissorRect, ifnGetScissorRect, ifnSetSoftwareVertexProcessing,
	ifnGetSoftwareVertexProcessing, ifnSetNPatchMode, ifnGetNPatchMode, ifnDrawPrimitive, ifnDrawIndexedPrimitive,
	ifnDrawPrimitiveUP, ifnDrawIndexedPrimitiveUP, ifnProcessVertices, ifnCreateVertexDeclaration,
	ifnSetVertexDeclaration, ifnGetVertexDeclaration, ifnSetFVF, ifnGetFVF, ifnCreateVertexShader,
	ifnSetVertexShader, ifnGetVertexShader, ifnSetVertexShaderConstantF, ifnGetVertexShaderConstantF,
	ifnSetVertexShaderConstantI, ifnGetVertexShaderConstantI, ifnSetVertexShaderConstantB,
	ifnGetVertexShaderConstantB, ifnSetStreamSource, ifnGetStreamSource, ifnSetStreamSourceFreq,
	ifnGetStreamSourceFreq, ifnSetIndices, ifnGetIndices, ifnCreatePixelShader, ifnSetPixelShader,
	ifnGetPixelShader, ifnSetPixelShaderConstantF, ifnGetPixelShaderConstantF, ifnSetPixelShaderConstantI,
	ifnGetPixelShaderConstantI, ifnSetPixelShaderConstantB, ifnGetPixelShaderConstantB, ifnDrawRectPatch,
	ifnDrawTriPatch, ifnDeletePatch, ifnCreateQuery, iD3D9DevFnTableLength
} D3D9DevFnTable;

class D3D9Dummy
{
private:
	HWND              hWindow = (HWND)NULL;
	IDirect3D9*       d3d9_dummy = nullptr;
	IDirect3DDevice9* d3d9dev_dummy = nullptr;
	mem_voidptr_t*    d3d9dev_vtable = nullptr;
	std::unique_ptr<VMTManager> d3d9dev_vmtmgr = nullptr;

public:
	D3D9Dummy();
	~D3D9Dummy();

public:
	IDirect3DDevice9* GetDevice();
	mem_voidptr_t     GetFunction(D3D9DevFnTable Index);
};

#endif