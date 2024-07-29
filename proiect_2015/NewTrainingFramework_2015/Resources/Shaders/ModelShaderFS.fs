precision mediump float;
varying vec3 v_color;
varying vec2 v_uv;  
varying vec3 v_pos;
uniform sampler2D u_texture0;

uniform float s_r;
uniform float b_r;
uniform vec3 fog_color;

uniform vec3 u_cam_pos;

uniform vec3 u_amb_color;
uniform float u_amb_ratio;
varying vec3 v_norm;
uniform vec3 u_light_direction;
uniform vec3 c_diff_light;
uniform vec3 c_spec_light;
uniform float specPower;

void main()
{
    vec4 color;
    color = texture2D(u_texture0, v_uv);
    if(color.a < 0.1) 
    discard;

    vec4 c_amb = color * vec4(u_amb_color, 1.0);

    vec4 c_diff = color * vec4(c_diff_light*max(dot(normalize(v_norm), -normalize(u_light_direction)), 0.0), 1.0);

    vec4 c_spec = vec4(c_spec_light * pow(max(dot(normalize(reflect(normalize(u_light_direction), normalize(v_norm))),normalize(u_cam_pos-v_pos)), 0.0), specPower), 1.0);

    vec4 c_final = u_amb_ratio*c_amb+(1.0-u_amb_ratio) * (c_diff+c_spec);

    float dist=distance(u_cam_pos, v_pos);
    float alpha = clamp((dist - s_r) / (b_r - s_r), 0.0, 1.0);
    vec4 culoare = alpha * vec4(fog_color,1.0) + (1.0-alpha) * c_final;
    gl_FragColor = culoare;




    //gl_FragColor = vec4(fog_color, 1.0);
   //gl_FragColor=vec4(0.0, 1.0, 0.0, 1.0);
}