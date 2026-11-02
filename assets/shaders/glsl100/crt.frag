#version 100

precision mediump float;

// Source: https://rickcarvalho.itch.io/crt-shader-for-raylib-go

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;

void main()
{
    vec2 uv = fragTexCoord;
    
    // Sample the texture
    vec4 color = texture2D(texture0, uv);
    
    // CRT scanlines
    float scanline = sin(uv.y * 800.0) * 0.04;
    color.rgb -= scanline;
    
    // CRT curve (slight barrel distortion)
    vec2 cc = uv - 0.5;
    float dist = dot(cc, cc) * 0.1;
    uv = uv + cc * (1.0 + dist) * dist;
    
    // Vignette effect
    float vignette = smoothstep(0.7, 0.4, length(cc));
    
    // RGB shift for chromatic aberration
    float shift = 0.002;
    float r = texture2D(texture0, uv + vec2(shift, 0.0)).r;
    float g = texture2D(texture0, uv).g;
    float b = texture2D(texture0, uv - vec2(shift, 0.0)).b;
    
    // Combine effects
    color = vec4(r, g, b, 1.0);
    color.rgb *= vignette;
    color.rgb -= scanline;
    
    // Subtle flicker
    color.rgb *= 0.97 + 0.03 * sin(time * 10.0);
    
    // Add slight noise
    float noise = fract(sin(dot(uv, vec2(12.9898, 78.233)) + time) * 43758.5453);
    color.rgb += noise * 0.02;
    
    gl_FragColor = color * colDiffuse * fragColor;
}