#version 410

layout (location = 0) in vec3 vertex_position;
layout (location = 1) in vec3 vertex_normal; // per-vertex texture co-ords
layout (location = 2) in vec2 texture_coords;

uniform mat4 model;
uniform mat4 view, proj;

out vec2 st;
out vec3 pos_eye;
out vec3 norm_eye;


void main() {
	st = texture_coords;
	pos_eye = (view * model * vec4(vertex_normal, 0.0)).xyz;
	norm_eye = (view * model * vec4(vertex_position, 1.0)).xyz;
	gl_Position = proj * view *  model * vec4 (vertex_position, 1.0);
}
