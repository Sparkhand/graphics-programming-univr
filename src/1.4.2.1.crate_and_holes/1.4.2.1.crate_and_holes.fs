#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    vec4 color1 = texture(texture1, TexCoord);
    vec4 color2 = texture(texture2, TexCoord);

    // Draw texture2 (holes) above texture1 (crate)
    FragColor = vec4(mix(color1.rgb, color2.rgb, color2.a), 1.0);

    // FACT: The final 1.0 is ignored by the rendering pipeline
}