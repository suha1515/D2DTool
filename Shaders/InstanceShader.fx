//����,��,���� ���

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

	float2 tex1		:TEXCOORD5;			//�»��
	float2 tex2		:TEXCOORD6;			//ũ��.
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

	float4x4 matWorld = float4x4(In.right, In.up, In.look, In.position);

	
	matWorldView = mul(matWorld,viewMat);
	matWorldViewProj = mul(matWorldView, projMat);
		
	v_out.pos = mul(float4(In.pos,1.f), matWorldViewProj);
	v_out.uv = float2(In.tex.x*In.tex2.x,In.tex.y*In.tex2.y) + In.tex1;				//�ؽ��� ��ǥ���
	
	return v_out;
}

// ARGB ���߿� ��ƼŸ���� ���ԵǸ� ARGB ������ �׷��ȵ���̽��� 3������ ����
// ������ ����ۿ��� ������ ���ִµ� ���߿� �׷��� ����̽��� ���۸� 3������ �÷����� ��
// �� �� �ִ�.


// �����غ���.
// ����Ÿ�� -�׷��� ����. ����ۿ� ������.(����۵� ���ʹٰ� �ȿ� ���°�)
// ��.. �׷��ȵ���̽��� �׷��ټ� �ִ� ����. ����۵� �� ������.
// �׷��� ����̽��� �����̽��� �׸��� �Ǵµ�. �����̽��� �� ����Ÿ��..

//����� float4 ������ ����ü�������� ��ȯ�� �� �ִٴ°�.
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





 
