#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D mask;

// Output fragment color
out vec4 finalColor;

void main()
{
    vec4 maskColor = texture(mask, vec2(gl_FragCoord.x/64.0, 1.0 - (gl_FragCoord.y/56.0)));
    if (maskColor.r < 0.25) discard;
    finalColor = texture(texture0, fragTexCoord);
}