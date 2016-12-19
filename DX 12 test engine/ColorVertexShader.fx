
//simple vertex shader
//position- a float3 represetning a vertex position
//returns the position of the vertex
float4 main(float3 position : POSITION) SV_POSITION
{
	return float4(position, 1.0f);
}