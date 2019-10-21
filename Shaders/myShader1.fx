//월드,뷰,투영 행렬
float4x4 worldMat;
float4x4 viewMat;
float4x4 projMat;

float4	animTex;			//애니메이션을 위한 전역변수.
float2  texSize;			//텍스쳐 크기.
float	zValue;				//z값
texture tex0;
float3 ColortoFade;			//컬러값.
float3 ColortoAdd;			//더해줄 컬러값.

float	gFloat;				//페이드아웃인전용 흐리기전용
float   gFloat2;
sampler s_2D = sampler_state
{
	Texture = (tex0);
	magfilter = None;
	minfilter = None;
	mipfilter = None;
};

struct VS_IN
{
	float3 pos    :POSITION;
	float3 normal :NOMRAL;
	float2 tex    :TEXCOORD0;
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

//버텍스 쉐이더 함수 
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT v_out = (VS_OUT)0; //초기화 방법.

	//월드*뷰*투영 행렬 곱하기.
	float4x4 matWorldView;
	float4x4 matWorldViewProj;

	float texX = texSize.x*0.5f;
	float texY = texSize.y*0.5f;

	
	float3 pos = float3(In.pos.x*texX, In.pos.y*texY, zValue);

	matWorldView = mul(worldMat,viewMat);
	matWorldViewProj = mul(matWorldView, projMat);
		
	v_out.pos = mul(float4(pos,1.0f), matWorldViewProj);
	//v_out.uv  = In.tex;
	v_out.uv = float2(In.tex.x*animTex.x, In.tex.y*animTex.y) + float2(animTex.z, animTex.w);
	
	//v_out.uv = float2(1.f - v_out.uv.x, v_out.uv.y);
	return v_out;
}

VS_OUT VS_MAIN2(VS_IN In)
{
	VS_OUT v_out = (VS_OUT)0; //초기화 방법.

							  //월드*뷰*투영 행렬 곱하기.
	float4x4 matWorldView;
	float4x4 matWorldViewProj;

	float texX = texSize.x*0.5f;
	float texY = texSize.y*0.5f;


	float3 pos = float3(In.pos.x*texX, In.pos.y*texY, zValue);

	matWorldView = mul(worldMat, viewMat);
	matWorldViewProj = mul(matWorldView, projMat);

	v_out.pos = mul(float4(pos, 1.0f), matWorldViewProj);
	//v_out.uv  = In.tex;
	v_out.uv = float2(In.tex.x*animTex.x, In.tex.y*animTex.y) + float2(animTex.z, animTex.w);

	//v_out.uv = float2(1.f - v_out.uv.x, v_out.uv.y);
	return v_out;
}
// ARGB 나중에 멀티타겟을 쓰게되면 ARGB 말고도 그래픽디바이스에 3개까지 들어간다
// 지금은 백버퍼에만 색깔을 뺴주는데 나중에 그래픽 디바이스에 버퍼를 3개까지 컬러값을 뽑
// 을 수 있다.


// 공부해보자.
// 렌더타겟 -그려줄 공간. 백버퍼와 같은것.(백버퍼도 렌터다겟 안에 들어가는것)
// 즉.. 그래픽디바이스가 그려줄수 있는 공간. 백버퍼도 그 한종류.
// 그래픽 디바이스가 서페이스에 그리게 되는데. 서페이스는 즉 렌더타겟..

//결론은 float4 말고도 구조체형식으로 반환할 수 있다는것.
PS_OUT PS_MAIN(VS_OUT In)
{
	PS_OUT p_out = (PS_OUT)0;

	//tex2D 함수 - 텍스처에서 uv값에 맞춰 픽셀을 가져오는 함수.
	p_out.v_color = tex2D(s_2D,In.uv);
	p_out.v_color =p_out.v_color*gFloat;

	return p_out;
}
PS_OUT PS_MAIN2(VS_OUT In)
{
	PS_OUT p_out = (PS_OUT)0;

	//tex2D 함수 - 텍스처에서 uv값에 맞춰 픽셀을 가져오는 함수.
	p_out.v_color = tex2D(s_2D, In.uv);
	//p_out.v_color = p_out.v_color*gFloat;

	
	//float3 preColor = p_out.v_color.rgb;
	float3 colorDiff = ColortoFade - p_out.v_color.rgb;
	float preColor = p_out.v_color.rgb;
	float3 lerpColor = p_out.v_color + colorDiff*gFloat2;
	p_out.v_color.rgb = lerpColor;
	//p_out.v_color = float4(1.0f, lerpColor);

	return p_out;
}
PS_OUT PS_MAIN3(VS_OUT In)
{
	PS_OUT p_out = (PS_OUT)0;
	p_out.v_color = tex2D(s_2D, In.uv);
	p_out.v_color.r = p_out.v_color.a;
	return p_out;
}
PS_OUT PS_MAIN4(VS_OUT In)
{
	PS_OUT p_out = (PS_OUT)0;

	p_out.v_color = tex2D(s_2D, In.uv);

	//p_out.v_color.rgb += float3(0.1f,0.1f,0.1f);
	float3 coloradd = ColortoAdd;
	float4 color = float4(coloradd, 1.0f);
	p_out.v_color = (p_out.v_color*color)*gFloat;

	return p_out;
}


// 장치성능에 따른 셰이더 선택을 위해 존재.
technique Default_Device
{
	// 어떤 객체를 렌더링함에 있어 표현하고 싶은 렌더링효과의 셋 기준으로 구분한다.
	pass Default_State
	{
		CullMode = NONE;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	//패스를 통해 쉐이더에서 기능을 적용하고 함수도 달라져야한다.
	pass HitEffect
	{
		CullMode = NONE;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN2();
		PixelShader = compile ps_3_0 PS_MAIN2();
	}
	pass LightEffect
	{
		CullMode = NONE;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;
		ZwriteEnable = false;
		ZEnable = false;
		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN3();
	}
	pass DashEffect
	{
		CullMode = NONE;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN4();
	}
}





 

