float2 TexOffset;                    
float2 AddDropletsPos;               
float  Distortion;                   

sampler tex0 : register(s0);
sampler tex1 : register(s1);
sampler tex2 : register(s2);

struct VS_OUTPUT
{
	float4 Pos : POSITION;
	float2 Tex : TEXCOORD0;
};

VS_OUTPUT VS(float4 Pos    : POSITION,
	float4 Color : COLOR0,
	float2 Tex : TEXCOORD0)
{
	VS_OUTPUT Out;

	Out.Pos = Pos;
	Out.Tex = Tex;

	return Out;
}

struct PS_OUTPUT
{
	float4 Col1 : COLOR0;
	float4 Col2 : COLOR1;
};

float4 UpdateDropletsMap(float2 Tex)
{
	float4 Out = 0.0f;

		float4 c1 = tex2D(tex2, Tex + float2(-TexOffset.x * 2.0f, 0.0f));
		float4 c2 = tex2D(tex2, Tex + float2(-TexOffset.x, 0.0f));
		float4 c3 = tex2D(tex2, Tex);
		float4 c4 = tex2D(tex2, Tex + float2(TexOffset.x, 0.0f));
		float4 c5 = tex2D(tex2, Tex + float2(TexOffset.x * 2.0f, 0.0f));

		if (c3.r <= c2.r && c3.r <= c4.r || c2.r == c4.r)
			Out += tex2D(tex0, Tex + float2(0.0f, -TexOffset.y));

	if (!(c4.r <= c3.r && c4.r <= c5.r || c3.r == c5.r) && c3.r <= c5.r)
		Out += tex2D(tex0, Tex + float2(TexOffset.x, -TexOffset.y));

	if (!(c2.r <= c1.r && c2.r <= c3.r || c1.r == c3.r) && c3.r < c1.r)
		Out += tex2D(tex0, Tex + float2(-TexOffset.x, -TexOffset.y));

	return Out;
}

PS_OUTPUT PS_AddDroplets(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.Col1 = UpdateDropletsMap(In.Tex);

	if (abs(In.Tex.x - AddDropletsPos.x) <= TexOffset.x * 0.3f &&
		abs(In.Tex.y - AddDropletsPos.y) <= TexOffset.y * 0.3f)
	{
		Out.Col1 = float4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	Out.Col2 = tex2D(tex1, In.Tex) + Out.Col1 - 0.002f;

	return Out;
}

PS_OUTPUT PS_NoneAddDroplets(VS_OUTPUT In)
{
	PS_OUTPUT Out;

	Out.Col1 = UpdateDropletsMap(In.Tex);

	Out.Col2 = tex2D(tex1, In.Tex) + Out.Col1 - 0.002f;

	return Out;
}

float4 PS_Blur(VS_OUTPUT In) : COLOR0
{
	float2 Texel0 = In.Tex + float2(-TexOffset.x, 0.0f);
	float2 Texel1 = In.Tex + float2(TexOffset.x, 0.0f);
	float2 Texel2 = In.Tex + float2(0.0f, TexOffset.y);
	float2 Texel3 = In.Tex + float2(0.0f, -TexOffset.y);

	float2 Texel4 = In.Tex + float2(-TexOffset.x, -TexOffset.y);
	float2 Texel5 = In.Tex + float2(TexOffset.x, -TexOffset.y);
	float2 Texel6 = In.Tex + float2(-TexOffset.x, TexOffset.y);
	float2 Texel7 = In.Tex + float2(TexOffset.x, TexOffset.y);

	float4 p0 = tex2D(tex0, In.Tex) * 0.2f;

	float4 p1 = tex2D(tex0, Texel0) * 0.1f;
	float4 p2 = tex2D(tex0, Texel1) * 0.1f;
	float4 p3 = tex2D(tex0, Texel2) * 0.1f;
	float4 p4 = tex2D(tex0, Texel3) * 0.1f;

	float4 p5 = tex2D(tex0, Texel4) * 0.1f;
	float4 p6 = tex2D(tex0, Texel5) * 0.1f;
	float4 p7 = tex2D(tex0, Texel6) * 0.1f;
	float4 p8 = tex2D(tex0, Texel7) * 0.1f;

	return p0 + p1 + p2 + p3 + p4 + p5 + p6 + p7 + p8;
}

float4 PS_CreateNormalMap(VS_OUTPUT In) : COLOR0
{
	float H1 = tex2D(tex0, In.Tex + float2(TexOffset.x, 0.0f)).r;
	float H2 = tex2D(tex0, In.Tex + float2(0.0f, TexOffset.y)).r;
	float H3 = tex2D(tex0, In.Tex + float2(-TexOffset.x, 0.0f)).r;
	float H4 = tex2D(tex0, In.Tex + float2(0.0f, -TexOffset.y)).r;

	float tu = 0.5f * (H3 - H1) + 0.5f;

	float tv = 0.5f * (H4 - H2) + 0.5f;

	return float4(float3(tu, tv, 1.0f), 1.0f);
}

float4 PS_Final(VS_OUTPUT In) : COLOR0
{
	float4 Out;

	float4 offset = tex2D(tex0, In.Tex);
		offset.rgb = (offset.rgb - 0.5f) * 2.0f;
	offset.r *= -1.0f;

	float p = 1.0f - dot(normalize(offset.rgb), float3(0.0f, 0.0f, 1.0f));

	Out = tex2D(tex1, In.Tex + offset * Distortion) + p;

	return Out;
}

technique TShader
{
	pass P0
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_3_0 PS_AddDroplets();
	}

	pass P1
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_3_0 PS_NoneAddDroplets();
	}

	pass P2
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS_Blur();
	}

	pass P3
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS_CreateNormalMap();
	}

	pass P4
	{
		VertexShader = compile vs_1_1 VS();
		PixelShader = compile ps_2_0 PS_Final();
	}
}