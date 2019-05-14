// 描画するテクスチャ
sampler Texture : register(s0);

// ピクセルシェーダーの出力
struct PS_INPUT
{
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};

struct PS_OUTPUT
{
    float4 color : COLOR0;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    float4 TextureColor = float4(0, 0, 0, 0);
    
    TextureColor = tex2D(Texture, input.uv);

    TextureColor.rgb *= float3(255, 255, 255);

    TextureColor.rgb = (1 - TextureColor.rgb);

    output.color = TextureColor;

    return output;
}