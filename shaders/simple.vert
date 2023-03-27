#version 460 core

uniform vec2 resolution;
uniform float time;
uniform float camera_scale;
uniform vec2 camera_pos;

layout(location = 0) in vec2 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 uv;

out vec4 out_color;
out vec2 out_uv;

mat4 ortho_project()
{
  float l = 0.0f;
  float r = 1920.0f;
  float b = 0.0f;
  float t = 1080.0f;
  float n = 0.0f;
  float f = 4000.0f;

  mat4 projection = mat4(vec4(2.0/(r-l),     0.0,          0.0,         0.0),
			 vec4(0.0,           2.0/(t-b),    0.0,         0.0),
			 vec4(0.0,           0.0,         -2.0/(f-n),   0.0),
			 vec4(-(r+l)/(r-l), -(t+b)/(t-b), -(f+n)/(f-n), 1.0));

  return projection;
}

vec2 camera_project(vec2 point)
{
    return 2.0 * (point - camera_pos) * camera_scale / resolution;
}

void main() {
    // gl_Position = vec4(camera_project(position), 0, 1);
  gl_Position = ortho_project() * vec4(position.xy, 0.0, 1.0);
    out_color = color;
    out_uv = uv;
}
