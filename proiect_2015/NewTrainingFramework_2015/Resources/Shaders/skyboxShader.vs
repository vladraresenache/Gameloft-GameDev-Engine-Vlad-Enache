attribute vec3 a_posL;
attribute vec3 a_color; 
attribute vec2 a_uv;

varying vec3 v_color;  
varying vec3 v_coord;

uniform mat4 uMVP;

void main()
{
    v_color = a_color;
    v_coord = a_posL;
    gl_Position = uMVP * vec4(a_posL, 1.0);
}