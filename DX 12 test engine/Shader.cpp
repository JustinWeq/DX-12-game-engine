#include "Shader.h"

//defualt shader-- constructs a new instance of Shader with defualt parameters
Shader::Shader()
{
	//init pointer data to null
	m_pipelineStateObject = NULL;
}

//deconstructor-- cleans up memory for this instance of shader
Shader::~Shader() 
{
	//call close
	Close();
}

//loads the shader using the passed in shader file name and target
//shaderFileName- the name of the shader to compile
bool Shader::LoadShader(LPCWSTR shaderFileName, LPCSTR targetType, D3D12_SHADER_BYTECODE* shaderCode)
{
	HRESULT result;
	//set up the blob for the error message and shader code
	ID3DBlob* shaderData;
	ID3DBlob* errorMessage;

	//attempt to compile the shader from a file
	result = D3DCompileFromFile(shaderFileName,
		nullptr,
		nullptr,
		"main",
		targetType,
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&shaderData,
		&errorMessage);
	if (FAILED(result))
	{
		writeShaderErrorLog(errorMessage, shaderFileName);
		return false;
	}

	//fill out the byte code
	shaderCode = {};
	shaderCode->BytecodeLength = shaderData->GetBufferSize();
	shaderCode->pShaderBytecode = shaderData->GetBufferPointer();

	//return true since nothing went wrong
	return true;

}

//writes a txt file out to the root directory with shader compiling errors
//shaderErrorMessage- the error message for the failed to compile shader
//shaderFileName- the name of the shader that failed to compile
void Shader::writeShaderErrorLog(ID3DBlob* shaderErrorMessage, LPCWSTR shaderFileName)
{
	//get the compile errors
	char* shaderErrors = (char*)shaderErrorMessage->GetBufferPointer();

	//get the error size
	unsigned long errorsSize = shaderErrorMessage->GetBufferSize();

	//open file to write the shader error message out to
	std::ofstream fout("ShaderErrors.txt");

	//write out the shader errors
	for (int i = 0; i < errorsSize; i++)
	{
		fout << shaderErrors[i];
	}

	//close the shader error message file
	fout.close();

	//release the shader error message blob
	shaderErrorMessage->Release();
	shaderErrorMessage = NULL;

	//display message to the user that the shader failed to compile
	MessageBox(NULL, L"Failed to compile shader file, check ShaderErrors.txt\n at the root directory for details.",
		shaderFileName, MB_OK);

}



//initializes the shader
//device- a pointer to the d3dinterface to use for intializtion of the shader
bool Shader::Init(D3DInterface* device)
{
	return true;
}

//Cleans up memory for the shader
void Shader::Close()
{
	//release the pipeline state device
	SAFE_RELEASE(m_pipelineStateObject);
	
}