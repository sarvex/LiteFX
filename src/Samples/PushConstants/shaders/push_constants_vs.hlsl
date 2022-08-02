#pragma pack_matrix(row_major)

// For DXIL we need to define a root signature, in order for shader reflection to properly pick up the push constants.
#define ROOT_SIGNATURE \
    "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT), " \
    "CBV(b0, space = 0, flags = DATA_STATIC_WHILE_SET_AT_EXECUTE), " \
    "RootConstants(num32BitConstants = 20, b0, space = 1, visibility = SHADER_VISIBILITY_VERTEX)"

struct VertexData 
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
}; 

struct VertexInput
{
    float3 Position : POSITION;
    float4 Color : COLOR;
};

struct CameraData
{
    float4x4 ViewProjection;
};

struct TransformData
{
    float4x4 Model;
    float4 Color;
};

ConstantBuffer<CameraData>    camera    : register(b0, space0);

#ifdef SPIRV
[[vk::push_constant]] ConstantBuffer<TransformData> transform;
#elif DXIL
ConstantBuffer<TransformData> transform : register(b0, space1);
#endif

[RootSignature(ROOT_SIGNATURE)]
VertexData main(in VertexInput input)
{
    VertexData vertex;
    
    float4 position = mul(float4(input.Position, 1.0), transform.Model);
    vertex.Position = mul(position, camera.ViewProjection);
    
    vertex.Color = transform.Color;
 
    return vertex;
}