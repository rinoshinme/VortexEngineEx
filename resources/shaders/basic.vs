#version 400

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec2 texture_coord;
layout (location = 2) in vec3 vertex_normal;

out vec3 normal;
out vec3 pos_eye;

uniform float time;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;


void main() 
{
    gl_Position = proj * view * model * vec4(vertex_position, 1.0);
    normal = vertex_normal;
    pos_eye = vertex_position;
    // pos_eye.z += sin(time);
}
