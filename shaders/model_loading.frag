#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform bool has_texture;

void main()
{
    vec4 color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    FragColor = texture(texture_diffuse1, TexCoords) * color;
}
