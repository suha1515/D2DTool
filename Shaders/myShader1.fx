//����,��,���� ���
float4x4 worldMat;
float4x4 viewMat;
float4x4 projMat;

float4	animTex;			//�ִϸ��̼��� ���� ��������.
float2  texSize;			//�ؽ��� ũ��.

texture tex0;
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

//���ؽ� ���̴� �Լ� 
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT v_out = (VS_OUT)0; //�ʱ�ȭ ���.

	//����*��*���� ��� ���ϱ�.
	float4x4 matWorldView;
	float4x4 matWorldViewProj;

	float texX = texSize.x*0.5f;
	float texY = texSize.y*0.5f;

	
	float3 pos = float3(In.pos.x*texX, In.pos.y*texY, In.pos.z);

	matWorldView = mul(worldMat,viewMat);
	matWorldViewProj = mul(matWorldView, projMat);
		
	v_out.pos = mul(float4(pos,1.f), matWorldViewProj);
	//v_out.uv  = In.tex;
	v_out.uv = float2(In.tex.x*animTex.x, In.tex.y*animTex.y) + float2(animTex.z, animTex.w);
	
	//v_out.uv = float2(1.f - v_out.uv.x, v_out.uv.y);
	return v_out;
}

// ARGB ���߿� ��ƼŸ���� ���ԵǸ� ARGB ���� �׷��ȵ���̽��� 3������ ����
// ������ ����ۿ��� ������ ���ִµ� ���߿� �׷��� ����̽��� ���۸� 3������ �÷����� ��
// �� �� �ִ�.


// �����غ���.
// ����Ÿ�� -�׷��� ����. ����ۿ� ������.(����۵� ���ʹٰ� �ȿ� ���°�)
// ��.. �׷��ȵ���̽��� �׷��ټ� �ִ� ����. ����۵� �� ������.
// �׷��� ����̽��� �����̽��� �׸��� �Ǵµ�. �����̽��� �� ����Ÿ��..

//����� float4 ���� ����ü�������� ��ȯ�� �� �ִٴ°�.
PS_OUT PS_MAIN(VS_OUT In)
{
	PS_OUT p_out = (PS_OUT)0;

	//tex2D �Լ� - �ؽ�ó���� uv���� ���� �ȼ��� �������� �Լ�.
	p_out.v_color = tex2D(s_2D,In.uv);
	//p_out.v_color.r = g_float;

	return p_out;
}
// ��ġ���ɿ� ���� ���̴� ������ ���� ����.
technique Default_Device
{
	// � ��ü�� �������Կ� �־� ǥ���ϰ� ���� ������ȿ���� �� �������� �����Ѵ�.
	pass Default_State
	{
		CullMode = NONE;
		AlphaBlendEnable = true;
		SrcBlend = SrcAlpha;
		DestBlend = InvSrcAlpha;

		VertexShader = compile vs_3_0 VS_MAIN();
		PixelShader = compile ps_3_0 PS_MAIN();
	}
	//�н��� ���� ���̴����� ����� �����ϰ� �Լ��� �޶������Ѵ�.
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





 

