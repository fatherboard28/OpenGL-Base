#version 330 core
out vec4 FragColor;
  
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    FragColor = mix(texture(texture1, vec2(TexCoord.x, TexCoord.y)), texture(texture2, vec2(2*TexCoord.x, 2*TexCoord.y)), 0.1);
}