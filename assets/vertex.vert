#version 400

in vec3 position;
in vec3 normal;
in vec2 texcoord;

out vec3 world_normal;
out vec3 world_vertex;
out vec3 world_eye;
out vec2 UV;

uniform mat4 model_matrix;
uniform mat4 view_matrix;
uniform mat4 projection_matrix;

uniform float deform_angle;
uniform float deform_size;
void main() {
        mat4 modelview_matrix = view_matrix * model_matrix;
        mat3 normal_matrix = transpose(inverse(mat3(model_matrix)));

        // expand
        // Vertex position, normal, and camera position in world space for lighting
        world_vertex = vec3(model_matrix * vec4(position, 1.0));
        world_normal = normalize(normal_matrix * normal);
        world_eye = vec3(inverse(view_matrix) * vec4(0.f,0.f,0.f,1.f));

        UV = texcoord;

        // Always have to transform vertex positions so they end
        // up in the right place on the screen.
        vec3 transformed = position;
        transformed.x = position.x + sin(position.y*10.0 + deform_angle * 10.0)*0.01*deform_size;
        gl_Position = projection_matrix * modelview_matrix *
                vec4(transformed, 1.0);
}
