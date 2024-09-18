#version 330 core
out vec4 FragColor;
  
in vec3 ourColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec2 happyReverse = vec2(-1 * TexCoord.s, TexCoord.t);
    FragColor = mix(texture(texture1, TexCoord), texture(texture2, happyReverse), 0.2);
}