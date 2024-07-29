precision highp float;

attribute vec3 a_posL;
attribute vec3 a_color;
attribute vec2 a_uv; 
varying vec3 v_color;
varying vec2 v_uv;
varying vec2 v2_uv;
uniform mat4 uMVP;
uniform float N; 

uniform sampler2D u_texture0;
uniform vec3 u_height;
uniform vec2 deplasament;

void main()
{

vec4 posL = vec4(a_posL, 1.0);
v_color = a_color;
v_uv = a_uv;
v2_uv = (a_uv+vec2(deplasament.y, deplasament.x))/N;


vec4 c_blend = texture2D(u_texture0, (a_uv+vec2(deplasament.y, deplasament.x))/N);


vec4 pos_nou = vec4(a_posL, 1.0);
pos_nou.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;

gl_Position = uMVP * pos_nou;
}
   