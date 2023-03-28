#version 330 core

in vec4 out_color;
in vec2 out_uv;
out vec4 color;

uniform sampler2D text;

void main() {
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, out_uv).r);
  color = out_color * sampled;
  // color = texture(image, out_uv).r*out_color;
}
