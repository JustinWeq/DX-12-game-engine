#include "RootSiganture.h"

//defualt constructor-- creates a new instance of root signature with defualt parameters
RootSignature::RootSignature()
{
	//set all pointers to 0 or NULL
	m_rootSignature = NULL;
}

//deconstructor-- cleans up memory for the root signature instance
RootSignature::~RootSignature() 
{
	//call close
	Close();
}

//initializes this root signature instance
//device- the device to use to initalize the root signature
bool RootSignature::Init(D3DInterface* device)
{
	HRESULT result;
	//create the root signature
	ID3DBlob* signature;
	result = D3D12SerializeRootSignature(&getRootSignatureDesc(), D3D_ROOT_SIGNATURE_VERSION_1,
		&signature, nullptr);
	if (FAILED(result))
	{
		//tell the user that the rot signature failed to be created
		MessageBox(NULL, L"Failed to serialize root siganture", L"Root signature was not serialized", MB_OK);
		return false;
	}

	//create the actual root siganture
	result = device->getDevice()->CreateRootSignature(NULL, signature->GetBufferPointer(), signature->GetBufferSize(), IID_PPV_ARGS(&m_rootSignature));
	if (FAILED(result))
	{
		//tell the user that the rot signature failed to be created
		MessageBox(NULL, L"Failed to create root signature", L"Root signature was not created", MB_OK);
		return false;
	}

	//return true since there was no errors
	return true;
}

//cleans up memory for the root signature
void RootSignature::Close()
{
	//safe release the root signature data
	SAFE_RELEASE(m_rootSignature);
}

//returns the root signature for this shader
ID3D12RootSignature* RootSignature::GetRootSignature()
{
	//return the root signature
	return  m_rootSignature;
}

RootSignature::operator ID3D12RootSignature*()
{
	return m_rootSignature;
}
