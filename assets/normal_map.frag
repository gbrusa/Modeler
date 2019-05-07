#version 400

// Interpolated inputs from Vertex Shader
in vec3 Color;
in vec2 Texcoord;
in mat3 TBN;
in vec3 world_vertex;
in vec3 world_eye;
in vec3 world_normal;

// Final color of this fragment
out vec4 frag_color;

// Built-in Variables
uniform sampler2D NormalMap;
uniform sampler2D TextureColor;
uniform vec3 point_light_ambient[4];
uniform vec3 point_light_intensity[4];
uniform vec3 point_light_position[4];
uniform float point_light_atten_quad[4];
uniform float point_light_atten_linear[4];
uniform float point_light_atten_const[4];
uniform vec3 dir_light_ambient[4];
uniform vec3 dir_light_intensity[4];
uniform vec3 dir_light_direction[4];

// User-defined Variables
vec3 normal;
vec3 texture_color;

vec3 DirLightContribution(int light_num) {
        // Directional Light Vectors
        vec3 N = normalize(normal);
        vec3 V = normalize(world_eye - world_vertex);
        vec3 L = TBN * -normalize(dir_light_direction[light_num]);
        vec3 H = normalize(V+L);

        float diffuseShade = max(dot(N, L), 0.0);
        vec3 ambient = texture_color * dir_light_ambient[light_num];
        vec3 diffuse = texture_color * diffuseShade * dir_light_intensity[light_num];

        return ambient + diffuse;
}

vec3 PointLightContribution(int light_num) {
        // Point Light Vectors
        vec3 N = normalize(normal);
        vec3 V = normalize(world_eye - world_vertex);
        vec3 L = TBN * normalize(point_light_position[light_num] - world_vertex);
        vec3 H = normalize(V+L);

        float diffuseShade = max(dot(N, L), 0.0);
        vec3 ambient = texture_color * point_light_ambient[light_num];
        vec3 diffuse = diffuseShade * texture_color * point_light_intensity[light_num];

        float dist = max(distance(point_light_position[light_num], world_vertex), 0.0);
        float denom = point_light_atten_const[light_num] + point_light_atten_linear[light_num]*dist + point_light_atten_quad[light_num]*dist*dist;
        float attenuation = denom < 0.00001 ? 0.0 : 1.0 / denom;

        return (attenuation * diffuse + ambient);
}

void main(void) {
    // Extract normal and color to be used by current pixel
	normal = normalize(texture(NormalMap, Texcoord).rgb*2.0 - 1.0);
	texture_color = vec3(texture(TextureColor, Texcoord)*0.5);
    
	// Calculate contribution of direction and point lighting
	vec3 dirlight_contribution = vec3(0,0,0);
    for (int i = 0; i < 4; i++) dirlight_contribution += DirLightContribution(i);
    vec3 pointlight_contribution = vec3(0,0,0);
    for (int i = 0; i < 4; i++) pointlight_contribution += PointLightContribution(i);
    
	// Return color of pixel
	frag_color = vec4((dirlight_contribution + pointlight_contribution), 1.0);
}
