#version 330 core
out vec4 FragColor;
in vec2 textureCoord;

uniform sampler2D diffuse0;

void main()
{
	if (texture(diffuse0, textureCoord).a < 0.1)
		discard;
	FragColor = texture(diffuse0, textureCoord);
}