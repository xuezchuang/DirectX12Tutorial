﻿#include "ApplicationWin32.h"
#include "Game.h"
#include "Common.h"
#include "MathLib.h"
#include "Camera.h"
#include "CommandQueue.h"
#include "D3D12RHI.h"
#include "d3dx12.h"
#include "RenderWindow.h"
#include "CommandListManager.h"
#include "CommandContext.h"
#include "RootSignature.h"
#include "GpuBuffer.h"
#include "PipelineState.h"
#include "DirectXTex.h"
#include "Texture.h"
#include "SamplerManager.h"
#include "MeshData.h"
#include "ObjLoader.h"
#include "BufferManager.h"
#include "GLTFLoader.h"
#include "Scene.h"
#include "Renderer.h"

#include <d3d12.h>
#include <dxgi1_4.h>
#include <chrono>
#include <iostream>

using namespace BufferManager;


class Tutorial6 : public FGame
{
public:
	Tutorial6(const GameDesc& Desc) : FGame(Desc), m_Scene(nullptr)
	{
	}

	void OnStartup()
	{
		SetupRootSignature();

		m_Scene = FGLTFLoader::LoadFromFile("../Resources/gltf2.0/DamagedHelmet/glTF/DamagedHelmet.gltf");

		SetupMesh();
		SetupShaders();
		SetupPipelineState();
	}

	void OnShutdown()
	{
		if (m_Scene)
		{
			delete m_Scene;
			m_Scene = nullptr;
		}
	}

	void OnUpdate()
	{

	}
	
	void OnRender()
	{
		// Frame limit set to 60 fps
		tEnd = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::milli>(tEnd - tStart).count();
		if (time < (1000.0f / 60.0f))
		{
			return;
		}
		tStart = std::chrono::high_resolution_clock::now();

		// Update Uniforms
		//m_elapsedTime += 0.001f * time;
		m_elapsedTime = fmodf(m_elapsedTime, 6.283185307179586f);
		//m_elapsedTime = MATH_PI / 4.f;
		m_uboVS.modelMatrix = FMatrix::RotateY(m_elapsedTime);

		FCamera camera(Vector3f(0.f, 0.f, 5.f), Vector3f(0.f, 0.0f, 0.f), Vector3f(0.f, 1.f, 0.f));
		m_uboVS.viewMatrix = camera.GetViewMatrix();
		FQuaternion Quat = FQuaternion(0.7071068286895752f, 0.f, 0.f, 0.7071068286895752f);
		m_uboVS.modelMatrix = Quat.ToMatrix();

		const float FovVertical = MATH_PI / 4.f;
		m_uboVS.projectionMatrix = FMatrix::MatrixPerspectiveFovLH(FovVertical, (float)GetDesc().Width / GetDesc().Height, 0.1f, 100.f);

		FCommandContext& CommandContext = FCommandContext::Begin();
		FillCommandLists(CommandContext);
		
		CommandContext.Finish(true);

		RenderWindow::Get().Present();
	}

private:
	void SetupRootSignature()
	{
		FSamplerDesc DefaultSamplerDesc;

		m_RootSignature.Reset(2, 1);
		m_RootSignature[0].InitAsConstants(0, sizeof(m_uboVS) / 4, D3D12_SHADER_VISIBILITY_VERTEX);
		m_RootSignature[1].InitAsDescriptorRange(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 0, 1, D3D12_SHADER_VISIBILITY_PIXEL);
		m_RootSignature.InitStaticSampler(0, DefaultSamplerDesc, D3D12_SHADER_VISIBILITY_PIXEL);
		m_RootSignature.Finalize(L"RootSignature", D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	}

	void SetupMesh()
	{
		
	}

	void SetupShaders()
	{
		m_vertexShader = D3D12RHI::Get().CreateShader(L"../Resources/Shaders/triangle.hlsl", "vs_main", "vs_5_1");

		m_pixelShader = D3D12RHI::Get().CreateShader(L"../Resources/Shaders/triangle.hlsl", "ps_main", "ps_5_1");
	}

	void SetupPipelineState()
	{
		m_PipelineState.SetRootSignature(m_RootSignature);
		m_PipelineState.SetRasterizerState(FPipelineState::RasterizerDefault);
		m_PipelineState.SetBlendState(FPipelineState::BlendTraditional);
		m_PipelineState.SetDepthStencilState(FPipelineState::DepthStateReadWrite);

		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElements;
		m_Scene->GetFirstMesh()->GetMeshLayout(inputElements);
		Assert(inputElements.size() > 0);
		m_PipelineState.SetInputLayout((UINT)inputElements.size(), &inputElements[0]);
		m_PipelineState.SetPrimitiveTopologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		m_PipelineState.SetRenderTargetFormats(1, &RenderWindow::Get().GetColorFormat(), g_SceneDepthZ.GetFormat());
		m_PipelineState.SetVertexShader(CD3DX12_SHADER_BYTECODE(m_vertexShader.Get()));
		m_PipelineState.SetPixelShader(CD3DX12_SHADER_BYTECODE(m_pixelShader.Get()));
		m_PipelineState.Finalize();
	}

	void FillCommandLists(FCommandContext& CommandContext)
	{
		// Set necessary state.
		CommandContext.SetRootSignature(m_RootSignature);
		CommandContext.SetPipelineState(m_PipelineState);
		CommandContext.SetViewportAndScissor(0, 0, m_GameDesc.Width, m_GameDesc.Height);

		CommandContext.SetConstantArray(0, sizeof(m_uboVS) / 4, &m_uboVS);

		RenderWindow& renderWindow = RenderWindow::Get();
		FColorBuffer& BackBuffer = renderWindow.GetBackBuffer();
		// Indicate that the back buffer will be used as a render target.
		CommandContext.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET);
		CommandContext.TransitionResource(g_SceneDepthZ, D3D12_RESOURCE_STATE_DEPTH_WRITE, true);
		CommandContext.SetRenderTargets(1, &BackBuffer.GetRTV(), g_SceneDepthZ.GetDSV());

		// Record commands.
		BackBuffer.SetClearColor(m_ClearColor);
		CommandContext.ClearColor(BackBuffer);
		CommandContext.ClearDepth(g_SceneDepthZ);
		CommandContext.SetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		FTexture* Texture = m_Scene->GetFirstMesh()->GetTexture(0, 0);
		if (Texture)
		{
			CommandContext.SetDynamicDescriptor(1, 0, Texture->GetSRV());
		}
		m_Renderer->Draw(m_Scene, CommandContext, false);

		CommandContext.TransitionResource(BackBuffer, D3D12_RESOURCE_STATE_PRESENT, true);
	}

private:
	struct
	{
		FMatrix projectionMatrix;
		FMatrix modelMatrix;
		FMatrix viewMatrix;
	} m_uboVS;

	FRootSignature m_RootSignature;
	FGraphicsPipelineState m_PipelineState;

	ComPtr<ID3DBlob> m_vertexShader;
	ComPtr<ID3DBlob> m_pixelShader;

	Vector3f m_ClearColor = Vector3f(0.5f, 0.58f, 0.8f);
	float m_elapsedTime = 0;
	std::chrono::high_resolution_clock::time_point tStart, tEnd;

	MeshData* m_mesh_data;
	Scene* m_Scene;
	Renderer* m_Renderer;
};

int main()
{
	ThrowIfFailed(CoInitializeEx(nullptr, COINIT_MULTITHREADED));
	GameDesc Desc;
	Desc.Caption = L"Tutorial 6 - Mesh Loader";
	Tutorial6 tutorial(Desc);
	ApplicationWin32::Get().Run(&tutorial);
	CoUninitialize();
	return 0;
}