#version 400

in vec3 world_normal;
in vec3 world_vertex;
in vec3 world_eye;
in vec2 UV;

uniform vec3 point_light_ambient[4];
uniform vec3 point_light_intensity[4];
uniform vec3 point_light_position[4];
uniform float point_light_atten_quad[4];
uniform float point_light_atten_linear[4];
uniform float point_light_atten_const[4];
uniform samplerCube point_light_shadowmap[4];
uniform vec3 dir_light_ambient[4];
uniform vec3 dir_light_intensity[4];
uniform vec3 dir_light_direction[4];
uniform sampler2D Color;

vec3 ObjectColor;

float DirLightContribution(int light_num) {
    vec3 N = normalize(world_normal);
    vec3 V = normalize(world_eye - world_vertex);
    vec3 L = -normalize(dir_light_direction[light_num]);
    vec3 H = normalize(V+L);

    // Contribution
    float diffuseShade = max(dot(N, L), 0.0);
    vec3 intensity = dir_light_intensity[light_num];
    float avg_intensity = (intensity.x + intensity.y + intensity.z) / 3;

    return diffuseShade * avg_intensity; //dir_light_intensity[light_num];
}

float PointLightContribution(int light_num) {
        vec3 N = normalize(world_normal);
        vec3 V = normalize(world_eye - world_vertex);
        vec3 L = normalize(point_light_position[light_num] - world_vertex);
        vec3 H = normalize(V+L);

        // Contribution
        float diffuseShade = max(dot(N, L), 0.0);
        vec3 intensity = point_light_intensity[light_num];
        float avg_intensity = (intensity.x + intensity.y + intensity.z) / 3;

        float dist = max(distance(point_light_position[light_num], world_vertex), 0.0);
        float denom = point_light_atten_const[light_num] + point_light_atten_linear[light_num]*dist + point_light_atten_quad[light_num]*dist*dist;
        float attenuation = denom < 0.00001 ? 0.0 : 1.0 / denom;

        return diffuseShade * avg_intensity * attenuation;
}

void main(void) {
    if ((dot(normalize(world_normal), normalize(world_eye - world_vertex))) <= 0.2) {
        gl_FragColor = vec4(0,0,0,1);
    } else {
        ObjectColor = texture(Color, UV).xyz;
        vec3 color;
        float dirlight_contribution = 0.0;//vec3(0,0,0);
        for (int i = 0; i < 4; i++) dirlight_contribution += DirLightContribution(i);
        float pointlight_contribution = 0.0;//vec3(0,0,0);
        for (int i = 0; i < 4; i++) pointlight_contribution += PointLightContribution(i);

        //float total_intensity = length(dirlight_contribution);
        float total_intensity = dirlight_contribution + pointlight_contribution;
        if (total_intensity > 0.95) {
            color = ObjectColor + vec3(0.25, 0.25, 0.25);  //vec3(1.0,0.5,0.5);  // 167%
        } else if (total_intensity > 0.5) {
            color = ObjectColor;  //vec3(0.6,0.3,0.3);  // 100%
        } else if (total_intensity > 0.25) {
            color = ObjectColor / 1.5;  //vec3(0.4,0.2,0.2);  // 67%
        } else {
            color = ObjectColor / 3;  //vec3(0.2,0.1,0.1);  // 33%
        }
        gl_FragColor = vec4(color, 1.0);
    }
}
