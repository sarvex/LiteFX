#pragma pack_matrix(row_major)

struct Particle
{
	float  distance;
	float  velocity;
	float2 position;
};

struct AnimationData
{
	float time;
	float speed;
	int particles;
};

ConstantBuffer<AnimationData> animation : register(b0, space1);
RWStructuredBuffer<Particle>  particles : register(u1, space1);

[numthreads(64, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
	if (id.x >= animation.particles)
		return;

	float velocity = particles[id.x].velocity;
	float distance = particles[id.x].distance;
	float2 position = float2(sin(animation.time * velocity), cos(animation.time * velocity)) / animation.speed;
	position *= distance;

	particles[id.x].position = position;
}