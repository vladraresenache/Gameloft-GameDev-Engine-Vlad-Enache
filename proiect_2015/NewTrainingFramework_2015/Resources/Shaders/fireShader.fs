precision mediump float;
varying vec3 v_color;
varying vec2 v_uv;  
uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;

uniform float s_r;
uniform float b_r;
uniform vec3 fog_color;
uniform float u_Time;
uniform float u_dispMax;    

void main()
{
    //vec4 color;
    //color = texture2D(u_texture0, v_uv);
    //if(color.a < 0.1) 
    //discard;
    
    
   vec2 disp = texture2D(u_texture2, vec2(v_uv.x, v_uv.y + u_Time)).rg;
   vec2 offset = ((2.0*disp)-1.0)*u_dispMax;
   vec2 v_uv_displaced = v_uv + offset;
   vec4 c_fire = texture2D(u_texture0, v_uv_displaced);
   vec4 c_alpha = texture2D(u_texture1, v_uv); 
   c_fire.a = c_fire.a*c_alpha.r;


   gl_FragColor=c_fire;
  // gl_FragColor=vec4(offset,0.0, 1.0);
}