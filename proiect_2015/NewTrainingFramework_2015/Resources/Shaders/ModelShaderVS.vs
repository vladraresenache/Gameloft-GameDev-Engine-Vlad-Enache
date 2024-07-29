attribute vec3 a_posL;
attribute vec3 a_color;
attribute vec2 a_uv;
attribute vec3 a_norm;

varying vec3 v_color;
varying vec3 v_norm;
uniform mat4 uMVP;

varying vec2 v_uv;
varying vec3 v_pos;

uniform mat4 uModel;

void main()
{
vec4 posL = vec4(a_posL, 1.0);
v_uv = a_uv;

v_pos = (uModel * vec4(a_posL, 1.0)).xyz;
v_norm = (uModel * vec4(normalize(a_norm), 0.0)).xyz; 

gl_Position = uMVP * vec4(a_posL, 1);
}
   