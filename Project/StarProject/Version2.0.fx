// �`�悷��e�N�X�`��
sampler Texture : register(s0);

//�^�C�}�[
float timer : register(c0);

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_INPUT
{
    float4 pos : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float4 color : COLOR0;
};

struct PS_OUTPUT
{
    float4 color : COLOR0;
};

PS_OUTPUT main(PS_INPUT input)
{
    PS_OUTPUT output;

    float4 TextureColor;

    //�h��̔g�̐�
    const float shakeLength = 10.0;

    //�h��̕�
    const float shakeWidth = 0.01;

    //�h��̃X�s�[�h
    const float speed = 3.0;
    
    float uvtmpx = sin(input.uv.x * shakeLength + timer * speed) * shakeWidth;

    TextureColor = tex2D(Texture, float2(input.uv.x + uvtmpx, input.uv.y));

    //if (0.3 < input.pos.y && input.pos.y < 0.7)

    output.color = TextureColor;

    return output;
}