#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D textureCrate;
uniform sampler2D textureHoles;

void main()
{
    vec4 colorCrate = texture(textureCrate, TexCoord);
    vec4 colorHoles = texture(textureHoles, TexCoord);

    // Draw textureHoles above textureCrate
    FragColor = vec4(mix(colorCrate.rgb, colorHoles.rgb, colorHoles.a), 1.0);
}