//グローバル
Texture2D g_texColor: register(t0);
SamplerState g_samLinear : register(s0);

//グローバル
cbuffer global_0:register(b0)
{
	matrix g_mW;//ワールド行列
	matrix g_mWVP; //ワールドから射影までの変換行列
	float4 g_vLightDir;  //ライトの方向ベクトル
	float4 g_vEye;//カメラ位置
};

cbuffer global_1:register(b1)
{
	float4 g_Ambient=float4(0,0,0,0);//アンビエント光
	float4 g_Diffuse=float4(1,0,0,0); //拡散反射(色）
	float4 g_Specular=float4(1,1,1,1);//鏡面反射
};

//バーテックスシェーダー出力構造体
struct VS_OUTPUT
{	
	float4 Pos : SV_POSITION;
	float4 Color : COLOR0;
	float3 Light : TEXCOORD0;
	float3 Normal : TEXCOORD1;
	float3 EyeVector : TEXCOORD2;
	float2 Tex : TEXCOORD3;
};
//
//バーテックスシェーダー
//
VS_OUTPUT VS( float4 Pos : POSITION ,float4 Norm : NORMAL,float2 Tex : TEXCOORD)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
	output.Normal=mul(Norm, (float3x3)g_mW);
	output.Pos=mul(Pos,g_mWVP);
	//ライト方向
	output.Light=g_vLightDir;
	//視線ベクトル
	float3 PosWorld = mul(Pos,g_mW);
    output.EyeVector = g_vEye - PosWorld;
	
	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector); 
	float4 NL = saturate(dot(Normal, LightDir)); 
	
	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	output.Color= g_Diffuse * NL + specular*g_Specular;
	
	//テクスチャー座標
	output.Tex=Tex;

	return output;
}

//
//ピクセルシェーダー
//
float4 PS( VS_OUTPUT input ) : SV_Target
{
	float4 color=g_texColor.Sample( g_samLinear, input.Tex );
	color+=input.Color/2;

	return color;
}

//
//バーテックスシェーダー
//
VS_OUTPUT VS_NoTeX( float4 Pos : POSITION ,float4 Norm : NORMAL)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//射影変換（ワールド→ビュー→プロジェクション）
	//法線をワールド空間に
	output.Normal=mul(Norm, (float3x3)g_mW);
	output.Pos=mul(Pos,g_mWVP);
	//ライト方向
	output.Light=g_vLightDir;
	//視線ベクトル
	float3 PosWorld = mul(Pos,g_mW);
    output.EyeVector = g_vEye - PosWorld;
	
	float3 Normal = normalize(output.Normal);
	float3 LightDir = normalize(output.Light);
	float3 ViewDir = normalize(output.EyeVector); 
	float4 NL = saturate(dot(Normal, LightDir)); 
	
	float3 Reflect = normalize(2 * NL * Normal - LightDir);
	float4 specular = pow(saturate(dot(Reflect, ViewDir)), 4); 

	output.Color= g_Diffuse * NL + specular*g_Specular;
	
	return output;
}

//
//ピクセルシェーダー
//
float4 PS_NoTex( VS_OUTPUT input ) : SV_Target
{
	float4 color=input.Color;

	return color;
}