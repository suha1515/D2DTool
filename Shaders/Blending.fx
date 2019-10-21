texture tex0;
texture tex1;

sampler s_2D = sampler_state
{
	Texture = (tex0);
	magfilter = None;
	minfilter = None;
	mipfilter = None;
};

sampler s_2D_light = sampler_state
{
	Texture = (tex1);
	magfilter = None;
	minfilter = None;
	mipfilter = None;
};

struct VS_OUT
{
	float4 pos	    :POSITION;
	float2 uv		:TEXCOORD0;
};
struct PS_OUT
{
	float4 v_color	:COLOR;
};

PS_OUT PS_MAIN(VS_OUT In)
{
	PS_OUT p_out = (PS_OUT)0;

	p_out.v_color = tex2D(s_2D, In.uv);

	float4 light_color = tex2D(s_2D_light, In.uv);
	light_color = light_color.rrrr;
	float3 ambient = float3(0.7f, 0.7f, 0.7f);

	light_color.xyz += ambient;
	light_color = saturate(light_color);

	p_out.v_color *= light_color;
	return p_out;	
}
technique Default_Device
{
	pass Default_State
	{
		CullMode = NONE;
		ZwriteEnable = false;
		ZEnable = false;
		VertexShader = NULL;
		PixelShader = compile ps_3_0 PS_MAIN();
	}
}





 

