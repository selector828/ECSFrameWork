//グローバル
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{	
	float4 Pos : SV_POSITION;
	float2 Pos2d : TEXCOORD0;

};
//
//バーテックスシェーダー
//
VS_OUTPUT VS( float4 Pos : POSITION ,float2 TC : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;

	output.Pos=Pos;

	output.Pos2d.x=Pos.x/2.0+0.5;
	output.Pos2d.y=-Pos.y/2.0+0.5;

	return output;
}

//
//ピクセルシェーダー
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float Samples[10]={-0.08,-0.05,-0.03,-0.02,-0.01,0.01,0.02,0.03,0.05,0.08};

	float2 Pos2d=input.Pos2d;
	float2 Dir=float2(0.5,0.5)-Pos2d; 
	float Len=length(Dir);
	Dir=Dir/Len;
	float4 Color=g_texColor.Sample( g_samLinear, Pos2d );
	float4 Sum=Color;
	for(int i=0;i<10;i++)
	{
		Sum+=g_texColor.Sample( g_samLinear, Pos2d+Dir*Samples[i] );
	}
	Sum*=1.0/11.0;

	float t=saturate(Len)*2;
	float4 FinalColor=lerp(Color,Sum,t);

	return FinalColor;
}