texture tex0;

sampler s_2D = sampler_state
{
	Texture = (tex0);
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





 

