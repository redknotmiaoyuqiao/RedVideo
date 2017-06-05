#version 330 core
in vec3 TexCoord;

out vec4 color;

uniform sampler2D y;
uniform sampler2D u;
uniform sampler2D v;

void main()
{
    vec2 t = vec2(1.0 - TexCoord.x,1.0 - TexCoord.y);

    vec3 yuv;
    vec3 rgb;
    yuv.x = texture2D(y, t).r;
    yuv.y = texture2D(u, t).r - 0.5;
    yuv.z = texture2D(v, t).r - 0.5;
    rgb = mat3( 1,       1,         1,
                0,       -0.39465,  2.03211,
                1.13983, -0.58060,  0) * yuv;
    color = vec4(rgb, 1);
}
