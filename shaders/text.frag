#version 330 core

in vec4 out_color;
in vec2 out_uv;
out vec4 color;

uniform sampler2D text;
uniform float time;

void main() {
  vec3 r = vec3(1.0, 0.0, 0.0);
  vec3 b = vec3(0.0, 0.0, 1.0);

  float s = abs(sin(time));
  vec3 m = mix(r, b, s);

  vec4 sampled = vec4(m, texture(text, out_uv).r);

  color = out_color * sampled;
  // color = texture(image, out_uv).r*out_color;
}
