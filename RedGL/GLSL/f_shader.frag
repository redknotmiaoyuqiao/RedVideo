#version 330 core
in vec3 TexCoord;

out vec4 color;

uniform sampler2D ourTexture0;
uniform sampler2D ourTexture1;

void main()
{
    //color = mix(texture(ourTexture1, TexCoord.xy),texture(ourTexture0, TexCoord.xy),0.5);
    //color = vec4(TexCoord,1.0);
    color = texture(ourTexture0, TexCoord.xy);
    color = vec4(color.r);
}
