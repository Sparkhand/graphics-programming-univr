#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D textureContainer;
uniform sampler2D textureFace;

// texture interpolation coefficient
uniform float textureInterpCoeff;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = mix(texture(textureContainer, TexCoord), texture(textureFace, TexCoord), textureInterpCoeff);
}