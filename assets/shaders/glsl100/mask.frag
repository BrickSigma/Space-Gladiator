#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform sampler2D mask;

void main()
{
    vec4 maskColor = texture2D(mask, vec2(gl_FragCoord.x/64.0, 1.0 - (gl_FragCoord.y/56.0)));
    if (maskColor.r < 0.25) discard;
    gl_FragColor = texture2D(texture0, fragTexCoord);
}