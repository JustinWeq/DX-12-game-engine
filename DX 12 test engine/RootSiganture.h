#pragma once
#include "stdafx.h"
#include "D3DInterface.h"
// a pure virtual class that is a wrapper for D3D12RootSiganture
class RootSignature {
public:

	//defualt constructor-- creates a new instance of root signature with defualt parameters
	RootSignature();

	//deconstructor-- cleans up memory for the root signature instance
	~RootSignature();

	//initializes this root signature instance
	//device- the device to use to initalize the root signature
	bool Init(D3DInterface* device);

	//cleans up memory for the root signature
	void Close();

	//returns the root signature for this shader
	ID3D12RootSignature* GetRootSignature();

	operator ID3D12RootSignature*();

protected:
	//returns the root signature description for the this shader,
	// this must be overwritten as it is a pure virtual function
	virtual CD3DX12_ROOT_SIGNATURE_DESC getRootSignatureDesc() = 0;

private:
	// the root signature that describes what resources this shader will have access to.
	ID3D12RootSignature* m_rootSignature;

	
};