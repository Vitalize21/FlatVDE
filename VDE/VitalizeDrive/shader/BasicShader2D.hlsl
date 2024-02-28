
Texture2D g_texDecal : register(t0);
SamplerState g_samLinear : register(s0);

// ワールド行列 × ビュー × 射影行列
cbuffer cbMatrixWVP : register(b0)
{
	// 列優先
	column_major  float4x4 g_matWVP : packoffset(c0);
};

struct VS_OUTPUT
{
	float4 Pos : SV_POSITION;
	float4 Col : COLOR;
	float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos : POSITION, float4 Col : COLOR, float2 Tex : TEXCOORD0)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	output.Pos = Pos;
	//output.Pos = mul(Pos, g_matWVP);
	output.Col = Col;
	output.Tex = Tex;
	return output;
}

float4 PS(VS_OUTPUT input) : SV_Target
{
	//return g_texDecal.Sample(g_samLinear, input.Tex);


	//VS_OUTPUT OUT;
	//OUT.Col = g_texDecal.Sample(g_samLinear, input.Tex);
	////少し暗くする(白くなる問題とりあえず対応)
	//OUT.Col.r -= 0.2f;
	//OUT.Col.g -= 0.2f;
	//OUT.Col.b -= 0.2f;
	//return OUT.Col;


	float4 diffuse = g_texDecal.Sample(g_samLinear, input.Tex);
	return input.Col * diffuse;
}

