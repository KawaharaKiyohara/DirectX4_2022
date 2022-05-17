

float4 VSMain( float4 position : POSITION ) : SV_POSITION
{
    return float4( position.xyz, 1.0f);
}

float4 PSMain( float4 positon : SV_POSITION ) : SV_Target0
{
    return float4( 1.0f, 0.0f, 0.0f, 1.0f );
}