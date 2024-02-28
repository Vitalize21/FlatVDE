#pragma once

#include <windows.h>
#include <tchar.h>
#include <d3d11.h>
#include <stdio.h>

#include "VitalizeDefine.h"

#pragma comment(lib, "d3d11.lib")

#if D3DX9_SDK_INSTALLED
#pragma comment(lib, "d3dx11.lib")
#include "d3dx11.h"
#else
#include <d3dCompiler.h>
#pragma comment(lib,"d3dCompiler.lib")
#endif

//WinMain�ɂ���E�B���h�E�n���h�������J
//�V�X�e�����ŗ��p�B�A�v�����ł͐G��Ȃ����ƁB
extern HWND mainWindowHandle;

//Vitalize Dev Env ���C���V�X�e��
namespace VitalizeSystem
{
	class mainSystem
	{
	public:
		enum class ScreenType : UINT
		{
			NONE = 0,
			HD,
			HD_PLUS,
			FULL_HD
		};

		static bool Initialize(HWND hWnd);
		static void Release();

		//�����o�[
		static HRESULT CreateBuffer(ID3D11Buffer** pBuffer, void* pData, size_t size, UINT CPUAccessFlag, D3D11_BIND_FLAG BindFlag);
		static void setScreenType(ScreenType type);

		//�ϐ�
		static ID3D11Device*			mpDevice;
		static ID3D11DeviceContext*		mpDeviceContext;
		static IDXGIDevice1*			mpDXGI;
		static IDXGIAdapter*			mpAdapter;
		static IDXGIFactory*			mpDXGIFactory;
		static IDXGISwapChain*			mpDXGISwpChain;
		static ID3D11Texture2D*			mpBackBuffer;
		static ID3D11RenderTargetView*	mpRenderTargetView;
		static ID3D11DepthStencilView*  mpDepthStencilView;

		static ScreenType screenType;
		static UINT ScreenWidth;
		static UINT ScreenHeight;
	private:
		static HRESULT CreateDepthStencilView();
		
	};

	class basicShader
	{
	public:
		static bool Initialize();
		static void Release();

		//�����o�[
		static ID3D11InputLayout*  mpVertexLayout;
		static ID3D11VertexShader* mpVertexShader;//���_�V�F�[�_�[
		static ID3D11PixelShader*  mpPixelShader;//�s�N�Z���V�F�[�_�[
		// �[�x�X�e���V���X�e�[�g
		static ID3D11DepthStencilState*  mpDepthStencilState;
		static ID3D11RasterizerState*	 mpRasterizerState;
		// �X�e���V���X�e�[�g
		static ID3D11DepthStencilState* mpStencilStateReader;
		static ID3D11DepthStencilState* mpStencilStateWriter;

	private:
		static bool CompileVertexShader(ID3DBlob** pCompiledShader, ID3DBlob** pErrors)
		{
#if D3DX9_SDK_INSTALLED
			if (FAILED(D3DX11CompileFromFile("../VitalizeDrive/shader/BasicShader2D.hlsl",
				NULL, NULL, "VS", "vs_5_0", 0, 0, NULL, pCompiledShader, pErrors, NULL)))
			{
				MessageBox(0, "���_�V�F�[�_�[�ǂݍ��ݎ��s", NULL, MB_OK);
				return false;
			}
#else
			UINT Flag = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR | D3DCOMPILE_ENABLE_STRICTNESS;
			HRESULT hr = D3DCompileFromFile(
				L"../VitalizeDrive/shader/BasicShader2D.hlsl", nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"VS", "vs_5_0", Flag, 0, pCompiledShader, pErrors);
			if (FAILED(hr))
			{
				MessageBox(0, "���_�V�F�[�_�[�ǂݍ��ݎ��s", NULL, MB_OK);
				return false;
			}
#endif
			return true;
		}
		static bool CompilePixelShader(ID3DBlob** pCompiledShader, ID3DBlob** pErrors)
		{
#if D3DX9_SDK_INSTALLED
			if (FAILED(D3DX11CompileFromFile("../VitalizeDrive/shader/BasicShader2D.hlsl",
				NULL, NULL, "PS", "ps_5_0", 0, 0, NULL, pCompiledShader, pErrors, NULL)))
			{
				MessageBox(0, "�s�N�Z���V�F�[�_�[�ǂݍ��ݎ��s", NULL, MB_OK);
				return false;
			}
#else
			UINT Flag = D3DCOMPILE_PACK_MATRIX_COLUMN_MAJOR | D3DCOMPILE_ENABLE_STRICTNESS;
			HRESULT hr = D3DCompileFromFile(
				L"../VitalizeDrive/shader/BasicShader2D.hlsl", nullptr,
				D3D_COMPILE_STANDARD_FILE_INCLUDE,
				"PS", "ps_5_0", Flag, 0, pCompiledShader, pErrors);
			if (FAILED(hr))
			{
				MessageBox(0, "�s�N�Z���V�F�[�_�[�ǂݍ��ݎ��s", NULL, MB_OK);
				return false;
			}
#endif
			return true;
		}
	};

}

