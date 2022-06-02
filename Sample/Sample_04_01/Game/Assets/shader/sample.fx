// 
struct VertexShaderInput{
    float4 position : POSITION;
    float2 uv       : TEXCOORD0;
};
struct VertexShaderOutput{
    float4 position : SV_Position;
    float2 uv       : TEXCOORD0;
};


Texture2D<float4>   g_tex       : register(t0);		//アルベドマップ
sampler             g_sampler   : register(s0);	    //サンプラステート。

// 頂点シェーダー。
VertexShaderOutput VSMain( VertexShaderInput vsIn ) 
{
    VertexShaderOutput vsOut;
    vsOut.position = vsIn.position;
    vsOut.uv = vsIn.uv;
    return vsOut;
}

// ピクセルシェーダー。
float4 PSMain( VertexShaderOutput vsOut ) : SV_Target0
{
    // 赤いカラーを出力する。
    return g_tex.Sample(g_sampler, vsOut.uv);
    
}