precision highp float;

varying vec3 v_color;
varying vec2 v_uv;  
varying vec2 v2_uv;  

uniform sampler2D u_texture0;
uniform sampler2D u_texture1;
uniform sampler2D u_texture2;
uniform sampler2D u_texture3;

uniform vec3 u_height;

void main()
{
    //vec4 color;


    //color = texture2D(u_texture0, v_uv);
    //if(color.a < 0.1) 
    //discard;
    //gl_FragColor=color;
    



    vec4 c_blend = texture2D(u_texture0, v2_uv); 

    vec4 c_rock = texture2D(u_texture1, v_uv); 
    vec4 c_dirt = texture2D(u_texture2, v_uv); 
    vec4 c_grass = texture2D(u_texture3, v_uv); 

   
    vec4 c_final = c_blend.r * c_rock + c_blend.g * c_dirt + c_blend.b * c_grass;
    c_final.a = 1.0; 

    gl_FragColor = c_final; 
    //gl_FragColor=vec4(v2_uv, 1.0, 1.0);
    //gl_FragColor=vec4(1.0, 1.0, 0.0, 1.0);
}