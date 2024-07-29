precision mediump float;

varying vec3 v_color;   
varying vec3 v_coord;

uniform samplerCube u_cube_texture;

void main()
{
    vec4 color = textureCube(u_cube_texture, v_coord); 
    gl_FragColor = color;
}