#version 400

in vec3 position_eye;
in vec3 normal_eye;

out vec4 frag_color;

// fixed point light properties
vec3 light_position_world  = vec3(0.0, 0.0, -1.0);
vec3 Ls = vec3 (1.0, 1.0, 1.0); // white specular colour
vec3 Ld = vec3 (0.7, 0.7, 0.7); // dull white diffuse light colour
vec3 La = vec3(0.2, 0.2, 0.2); // grey ambient colour
  
// surface reflectance
vec3 Ks = vec3 (1.0, 1.0, 1.0); // fully reflect specular light
vec3 Kd = vec3 (1.0, 0.5, 0.0); // orange diffuse surface reflectance
vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
float specular_exponent = 16.0; // specular 'power'
float ambient_strength = 0.1;
float specular_strength = 0.5;
uniform mat4 view;

void main() 
{
    // ambient
    vec3 Ia = La * Ka * ambient_strength;

    // diffuse
    // normalize normal after interpolation
    vec3 norm = normalize(normal_eye);
    vec3 lightPos = vec3(view * vec4(light_position_world, 1.0));
    vec3 light_dir = normalize(lightPos - position_eye);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 Id = Ld * Kd * diff;

    // specular
    vec3 view_dir = normalize(-position_eye);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), specular_exponent);
    vec3 Is = Ls * Ks * specular_strength;

    frag_color = vec4(Ia + Id + Is, 1.0);
}