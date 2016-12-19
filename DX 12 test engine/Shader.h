#pragma once
#include "stdafx.h"
#include "D3DInterface.h"
#include <fstream>
#define PS_5_0 = "ps_5_0"
#define VS_5_0 = "vs_5_0"
// a simple shader class made to lower overhead for custom shaders.
class Shader
{
public:

	//defualt shader-- constructs a new instance of Shader with defualt parameters
	Shader();

	//deconstructor-- cleans up memory for this instance of shader
	~Shader();

	//initializes the shader
	//device- a pointer to the d3dinterface to use for intializtion of the shader
	bool Init(D3DInterface* device);

	//Cleans up memory for the shader
	virtual void Close();

protected:

	

	//returns the input element desc for this shader, this must be overwritten
	virtual D3D12_INPUT_ELEMENT_DESC getInputElementLayout() = 0;

	//returns the graphics pipeline state description, this is pure virtual so it must be overwritten
	//rootSignature- the root signature of the graphics pipeline state object
	//inputLayout- the input layout desc for the shader
	//device- the d3d interface to use for intialization(a command allocator is executed on the devices queue and the current frame fence value is incremented)
	virtual D3D12_GRAPHICS_PIPELINE_STATE_DESC getGraphicsPipelineStateDesc(ID3D12RootSignature* rootSignature, D3D12_INPUT_LAYOUT_DESC inputLayout,
		D3DInterface device) = 0;

	//loads the shader using the passed in shader file name and target
	//shaderFileName- the name of the shader to compile
	static bool LoadShader(LPCWSTR shaderFileName, LPCSTR targetType,D3D12_SHADER_BYTECODE* shaderCode);



	// pipeline state object for this shader
	ID3D12PipelineState* m_pipelineStateObject;

	//// the vertex buffer resource in GPU memory
	//ID3D12Resource* m_vertexBuffer;

	//// a stucture that contains data fot the verticies in gpu memory
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferView;

private:

	//writes a txt file out to the root directory with shader compiling errors
	//shaderErrorMessage- the error message for the failed to compile shader
	//shaderFileName- the name of the shader that failed to compile
	static void writeShaderErrorLog(ID3DBlob* shaderErrorMessage, LPCWSTR shaderFileName);
};
