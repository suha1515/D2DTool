//����,��,���� ���
float4x4 worldMat;
float4x4 viewMat;
float4x4 projMat;

float4	animTex;			//�ִϸ��̼��� ���� ��������.
float2  texSize;			//�ؽ��� ũ��.
float	zValue;				//z��
texture tex0;
float3 ColortoFade;			//�÷���.
float3 ColortoAdd;			//������ �÷���.

float	gFloat;				//���̵�ƿ������� �帮������
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

//���ؽ� ���̴� �Լ� 
VS_OUT VS_MAIN(VS_IN In)
{
	VS_OUT v_out = (VS_OUT)0; //�ʱ�ȭ ���.

	//����*��*���� ��� ���ϱ�.
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
	VS_OUT v_out = (VS_OUT)0; //�ʱ�ȭ ���.

							  //����*��*���� ��� ���ϱ�.
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
	p_out.v_color =p_out.v_color*gFloat;

	return p_out;
}
PS_OUT PS_MAIN2(VS_OUT In)
{
	PS_OUT p_out = (PS_OUT)0;

	//tex2D �Լ� - �ؽ�ó���� uv���� ���� �ȼ��� �������� �Լ�.
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





 

