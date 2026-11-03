#version 330

// Source: https://rickcarvalho.itch.io/crt-shader-for-raylib-go

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;

// Output fragment color
out vec4 finalColor;

vec4 sampleSafe(vec2 uv) {
  if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0)
    return vec4(0.0, 0.0, 0.0, 1.0);
  return texture(texture0, uv);
}

void main() {
  vec2 uv = fragTexCoord;

  // Sample the texture
  vec4 color = sampleSafe(uv);

  // CRT scanlines
  float scrollSpeed = 0.02;
  float scanline = sin((uv.y + time*scrollSpeed) * 672.0) * 0.04;
  color.rgb -= scanline;

  // CRT curve (slight barrel distortion)
  vec2 cc = uv - 0.5;
  float dist = dot(cc, cc) * 0.1;
  uv = uv + cc * (1.0 + dist) * dist;

  if (uv.x < 0.0 || uv.x > 1.0 || uv.y < 0.0 || uv.y > 1.0) {
    finalColor = vec4(0.0, 0.0, 0.0, 1.0);
    return;
  }

  // Vignette effect
  float vignette = smoothstep(0.75, 0.5, length(cc));

  // RGB shift for chromatic aberration
  float shift = 0.002;
  float r = sampleSafe(uv + vec2(shift, 0.0)).r;
  float g = sampleSafe(uv).g;
  float b = sampleSafe(uv - vec2(shift, 0.0)).b;

  // Combine effects
  color = vec4(r, g, b, 1.0);
  color.rgb *= vignette;
  color.rgb -= scanline;

  // Subtle flicker
  color.rgb *= 0.97 + 0.03 * sin(time * 10.0);

  // Add slight noise
  float noise = fract(sin(dot(uv, vec2(12.9898, 78.233)) + time) * 43758.5453);
  color.rgb += noise * 0.02;

  finalColor = color * colDiffuse * fragColor;
}