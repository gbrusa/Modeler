/****************************************************
 * This is a coding excerpt of how my partner and I
 * implemented surfaces of revolution in the 3D
 * Modeler project.
 ***************************************************/

#define PI 3.1415f
#define UNIT_Z glm::vec3(0,0,1)

// Helper functions to create various components at each band in the surface
std::vector<float> CreateBand(std::vector<float> prev_band, float angle_subdiv);
std::vector<unsigned int> CreateTrianglesBand(unsigned int num_curve_points, unsigned int start);
std::vector<float> CreateUVBand(std::vector<float> curr_band, float division_percentage, float tot_height);

// Helper function to get i'th vertex in positions as vector
glm::vec3 get_position(unsigned int i, std::vector<float> positions);

// Functions to handle vectors
glm::vec3 cross_product(glm::vec3 a, glm::vec3 b);
float get_distance(glm::vec3 a, glm::vec3 b);

[...]

std::unique_ptr<Mesh> SurfaceOfRevolution::CreateMesh(const std::vector<glm::vec2>& curve_points, unsigned int subdivisions) {
    std::unique_ptr<Mesh> surface = std::make_unique<Mesh>("Surface of Revolution");

    float angle_subdiv = 2 * PI / subdivisions;

    // Points are implemented as triples of floats where i is
    // the x-coord, i+1 is the y-coord, and i+2 is the z-coord
    std::vector<float> positions = std::vector<float>();
    std::vector<unsigned int> triangles = std::vector<unsigned int>();
    std::vector<float> normals = std::vector<float>();
    float height = 0.0f;

    // Copies the 2D points into 3D space with z=0
    // Calculates normals to the curve in the xy-plane which will be rotated
    // with the points
    for (auto it = curve_points.begin(); it != curve_points.end(); it++) {
        positions.push_back(it->x);
        positions.push_back(it->y);
        positions.push_back(0.0f);

        glm::vec3 n;
        if (it == curve_points.end() - 1) {
            glm::vec3 t((it-1)->x - it->x, (it-1)->y - it->y, 0.0f);
            n = cross_product(t, UNIT_Z);
        } else {
            glm::vec3 t(it->x - (it+1)->x, it->y - (it+1)->y, 0.0f);
            n = cross_product(t, UNIT_Z);
            height += get_distance(glm::vec3(it->x, it->y, 0.0f),
                                   glm::vec3((it+1)->x, (it+1)->y, 0.0f));
        }
        normals.push_back(n.x);
        normals.push_back(n.y);
        normals.push_back(n.z);
    }

    // Fencepost the bands
    auto normals_band = CreateBand(normals, angle_subdiv);
    auto prev_band = CreateBand(positions, angle_subdiv);
    auto uvs = CreateUVBand(prev_band, 1.0f, height);

    unsigned int start_index = 0;
    for (size_t i = 0; i < subdivisions; i++) {
        // Insert fenceposted bands
        positions.insert(positions.end(), prev_band.begin(), prev_band.end());
        normals.insert(normals.end(), normals_band.begin(), normals_band.end());

        // Create new bands for the vertices and the normals
        auto curr_band = CreateBand(prev_band, angle_subdiv);
        normals_band = CreateBand(normals_band, angle_subdiv);

        // Create triangles for the current band
        auto triangle_band = CreateTrianglesBand(static_cast<unsigned int>(prev_band.size()) / 3, start_index);
        triangles.insert(triangles.end(), triangle_band.begin(), triangle_band.end());

        // Find uv-coords for the vertices on the current band
        auto curr_uvs = CreateUVBand(prev_band, static_cast<float>(subdivisions - i - 1) / subdivisions, height);
        uvs.insert(uvs.end(), curr_uvs.begin(), curr_uvs.end());

        start_index += curr_band.size() / 3;
        prev_band = curr_band;
    }
    surface->SetPositions(positions);
    surface->SetTriangles(triangles);
    surface->SetNormals(normals);
    surface->SetUVs(uvs);

    return surface;
}

std::vector<float> CreateBand(std::vector<float> prev_band, float angle_subdiv) {
    std::vector<float> curr_band = std::vector<float>();
    for (size_t i = 0; i < prev_band.size(); i+=3) {
        curr_band.push_back(cos(angle_subdiv)*prev_band[i] + sin(angle_subdiv)*prev_band[i+2]);
        curr_band.push_back(prev_band[i+1]);
        curr_band.push_back(cos(angle_subdiv)*prev_band[i+2] - sin(angle_subdiv)*prev_band[i]);
    }
    return curr_band;
}

std::vector<unsigned int> CreateTrianglesBand(unsigned int num_curve_points, unsigned int start) {
    std::vector<unsigned int> t_band = std::vector<unsigned int>();
    for (unsigned int i = start; i < start+num_curve_points - 1; i++) {
        t_band.push_back(i);
        t_band.push_back(i+num_curve_points+1);
        t_band.push_back(i+num_curve_points);

        t_band.push_back(i+1);
        t_band.push_back(i+num_curve_points+1);
        t_band.push_back(i);

    }
    return t_band;
}

std::vector<float> CreateUVBand(std::vector<float> curr_band, float division_percentage, float tot_height) {
    std::vector<float> uv_band;
    float curr_height = tot_height;
    glm::vec3 prev_point = get_position(curr_band.size() / 3 - 1, curr_band);
    uv_band.push_back(division_percentage);
    uv_band.push_back(1.0f);
    for (size_t i = curr_band.size()/3 - 2; i+1 > 0; i--) {
        glm::vec3 curr_point = get_position(i, curr_band);
        curr_height -= get_distance(curr_point, prev_point);

        uv_band.push_back(division_percentage);  // u
        uv_band.push_back(curr_height / tot_height);  // v

        prev_point = curr_point;
    }
    return uv_band;
}

glm::vec3 get_position(unsigned int i, std::vector<float> positions) {
    return glm::vec3(positions[i*3], positions[i*3 + 1], positions[i*3 + 2]);
}

glm::vec3 cross_product(glm::vec3 a, glm::vec3 b) {
    glm::vec3 tmp(a.y*b.z - a.z*b.y, a.z*b.x - a.x*b.z, a.x*b.y - a.y*b.x);
    float denom = sqrt(tmp.x*tmp.x + tmp.y*tmp.y + tmp.z*tmp.z);
    return glm::vec3(tmp.x/denom, tmp.y/denom, tmp.z/denom);
}

float get_distance(glm::vec3 a, glm::vec3 b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y) + (a.z - b.z) * (a.z - b.z));
}
