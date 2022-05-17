
// 頂点シェーダー。
float4 VSMain( float4 position : POSITION ) : SV_POSITION
{
    // 入力された頂点をそのまま出力する。
    return float4( position.xyz, 1.0f);
}

// ピクセルシェーダー。
float4 PSMain( float4 positon : SV_POSITION ) : SV_Target0
{
    // 赤いカラーを出力する。
    return float4( 1.0f, 0.0f, 0.0f, 1.0f );
}