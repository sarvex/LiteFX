#pragma pack_matrix(row_major)

struct VertexData 
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
}; 

struct VertexInput
{
    //[[vk::location(0)]] 
    float3 Position : POSITION;
    
    //[[vk::location(1)]]
    float4 Color : COLOR;
};

struct CameraData
{
    float4x4 ViewProjection;
};

struct Particle
{
    float  distance;
    float  velocity;
    float2 position;
};

ConstantBuffer<CameraData> camera    : register(b0, space0);
StructuredBuffer<Particle> particles : register(t0, space1);

VertexData main(in VertexInput input, uint id : SV_InstanceID)
{
    VertexData vertex;
    
    float4 position = float4(particles[id].position, 0.0, 1.0);
    position += float4(input.Position, 1.0);

    vertex.Position = mul(position, camera.ViewProjection);
    vertex.Color = input.Color;
 
    return vertex;
}