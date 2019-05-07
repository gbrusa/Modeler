#version 400

in vec3 position;
in vec3 color;
in vec3 normal;
in vec3 tangent;
in vec3 bitangent;
in vec2 texcoord;

out mat3 TBN;
out vec3 tangent_eye;

out vec3 world_vertex;
out vec3 world_eye;

out vec3 Color;
out vec2 Texcoord;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

void main()
{
    mat4 modelview_matrix = view_matrix * model_matrix;
    mat3 normal_matrix = transpose(inverse(mat3(model_matrix)));

    world_vertex = vec3(model_matrix * vec4(position, 1.0));
    world_eye = vec3(inverse(view_matrix) * vec4(0.f,0.f,0.f,1.f));

//    world_vertex = vec3(modelview_matrix*vec4(position,1.0));
//    world_eye = normalize(-world_vertex);

    vec3 world_normal = normalize(normal_matrix * normal);
    vec3 world_tangent = normalize(normal_matrix * tangent);  // normalize(normal_matrix * tangent);
    vec3 world_bitangent = normalize(normal_matrix * bitangent);  // normalize(normal_matrix * bitangent);

    TBN = inverse(mat3(world_tangent, world_bitangent, world_normal));

    //tangent_eye = vec3(TBN * vec3(0.f,0.f,0.f));

    Color = color;
    Texcoord = texcoord;
    gl_Position = projection_matrix * view_matrix * model_matrix * vec4(position, 1.0);
}
