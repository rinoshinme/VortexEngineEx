#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;

out vec3 position_eye;
out vec3 normal_eye;

uniform mat4 model;
uniform mat4 view;
uniform mat4 proj;

void main() 
{
    position_eye = vec3(view * model * vec4(position, 1.0));
    // normal_eye = vec3(view * model * vec4(normal, 1.0));
    normal_eye = mat3(transpose(inverse(view * model))) * normal;
    
    gl_Position = proj * vec4(position_eye, 1.0);
}
