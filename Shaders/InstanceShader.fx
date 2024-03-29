//월드,뷰,투영 행렬

float4x4 viewMat;
float4x4 projMat;

texture tex0;

sampler s_2D = sampler_state
{
	Texture = (tex0);
	magfilter = point;
	minfilter = point;
	mipfilter = point;
};

struct VS_IN
{
	float3 pos    :POSITION;
	float3 normal :NOMRAL;
	float2 tex    :TEXCOORD0;

	float4 right	:TEXCOORD1;
	float4 up		:TEXCOORD2;
	float4 look		:TEXCOORD3;
	float4 position  :TEXCOORD4;

	float2 tex1		:TEXCOORD5;			//좌상단
	float2 tex2		:TEXCOORD6;			//크기.
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

	float4x4 matWorld = float4x4(In.right, In.up, In.look, In.position);

	
	matWorldView = mul(matWorld,viewMat);
	matWorldViewProj = mul(matWorldView, projMat);
		
	v_out.pos = mul(float4(In.pos,1.f), matWorldViewProj);
	v_out.uv = float2(In.tex.x*In.tex2.x,In.tex.y*In.tex2.y) + In.tex1;				//텍스쳐 좌표계산
	
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
	//p_out.v_color.r = g_float;

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
	//pass Default_State_None_Alpha
	//{
	//	CullMode = NONE;
	//	AlphaBlendEnable = false;
	//	SrcBlend = SrcAlpha;
	//	DestBlend = InvSrcAlpha;

	//	VertexShader = compile vs_3_0 VS_MAIN2();
	//	PixelShader = compile ps_3_0 PS_MAIN2();
	//}
}





 

