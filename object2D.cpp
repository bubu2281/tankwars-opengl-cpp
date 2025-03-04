#include "object2D.h"

#include <vector>
#include <numeric>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* object2D::CreateSquare(
    const std::string &name,
    glm::vec3 leftBottomCorner,
    float length,
    glm::vec3 color,
    bool fill)
{
    glm::vec3 corner = leftBottomCorner;

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(corner, color),
        VertexFormat(corner + glm::vec3(length, 0, 0), color),
        VertexFormat(corner + glm::vec3(length, length, 0), color),
        VertexFormat(corner + glm::vec3(0, length, 0), color)
    };

    Mesh* square = new Mesh(name);
    std::vector<unsigned int> indices = { 0, 1, 2, 3 };

    if (!fill) {
        square->SetDrawMode(GL_LINE_LOOP);
    } else {
        // Draw 2 triangles. Add the remaining 2 indices
        indices.push_back(0);
        indices.push_back(2);
    }

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* object2D::CreateTank(const std::string& name, glm::vec3 color1, glm::vec3 color2)
{
    glm::vec3 center = glm::vec3(0,0,0);

    std::vector<VertexFormat> vertices =
    {

        VertexFormat(center + glm::vec3(-3, 0, 0), color2),
        VertexFormat(center + glm::vec3(3, 0, 0), color2),
        VertexFormat(center + glm::vec3(3.6, 0.8, 0), color2),

        VertexFormat(center + glm::vec3(3.6, 0.8, 0), color2),
        VertexFormat(center + glm::vec3(-3.6, 0.8, 0), color2),
        VertexFormat(center + glm::vec3(-3, 0, 0), color2),

        VertexFormat(center + glm::vec3(-4.6, 0.8, 0), color1),
        VertexFormat(center + glm::vec3(4.6, 0.8, 0), color1),
        VertexFormat(center + glm::vec3(-3.7, 2.3, 0), color1),

        VertexFormat(center + glm::vec3(-3.7, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(4.6, 0.8, 0), color1),
        VertexFormat(center + glm::vec3(3.8, 2.3, 0), color1),
        //////
        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(1.45, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(1.39, 2.71, 0), color1),

        VertexFormat(center + glm::vec3(1.39, 2.71, 0), color1),
        VertexFormat(center + glm::vec3(1.23, 3.07, 0), color1),
        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),

        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(0.88, 3.45, 0), color1),
        VertexFormat(center + glm::vec3(0.46, 3.68, 0), color1),

        VertexFormat(center + glm::vec3(0.46, 3.68, 0), color1),
        VertexFormat(center + glm::vec3(0, 3.75, 0), color1),
        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),

        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(1.23, 3.07, 0), color1),
        VertexFormat(center + glm::vec3(0.88, 3.45, 0), color1),
        ///////
        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(-1.39, 2.71, 0), color1),
        VertexFormat(center + glm::vec3(-1.45, 2.3, 0), color1),

        VertexFormat(center + glm::vec3(-1.39, 2.71, 0), color1),
        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(-1.23, 3.07, 0), color1),

        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(-0.46, 3.68, 0), color1),
        VertexFormat(center + glm::vec3(-0.88, 3.45, 0), color1),

        VertexFormat(center + glm::vec3(-0.46, 3.68, 0), color1),
        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(0, 3.75, 0), color1),

        VertexFormat(center + glm::vec3(0, 2.3, 0), color1),
        VertexFormat(center + glm::vec3(-0.88, 3.45, 0), color1),
        VertexFormat(center + glm::vec3(-1.23, 3.07, 0), color1),
    };

    Mesh* tank = new Mesh(name);
    std::vector<unsigned int> indices(42, 0);
    std::iota(indices.begin(), indices.end(), 0);

    if (false) {
        tank->SetDrawMode(GL_LINE_LOOP);
    }

    tank->InitFromData(vertices, indices);
    return tank;
}

Mesh* object2D::CreateBarrel(const std::string& name)
{
    glm::vec3 center = glm::vec3(0, 0, 0);
    glm::vec3 color = glm::vec3(0);
    float barrel_width = 0.3;
    float barrel_height = 4;

    std::vector<VertexFormat> vertices =
    {

        VertexFormat(center + glm::vec3(-barrel_width, 0, 0), color),
        VertexFormat(center + glm::vec3(barrel_width, 0, 0), color),
        VertexFormat(center + glm::vec3(barrel_width, barrel_height, 0), color),

        VertexFormat(center + glm::vec3(barrel_width, barrel_height, 0), color),
        VertexFormat(center + glm::vec3(-barrel_width, barrel_height, 0), color),
        VertexFormat(center + glm::vec3(-barrel_width, 0, 0), color),
    };

    Mesh* tank = new Mesh(name);
    std::vector<unsigned int> indices(6, 0);
    std::iota(indices.begin(), indices.end(), 0);

    if (false) {
        tank->SetDrawMode(GL_LINE_LOOP);
    }

    tank->InitFromData(vertices, indices);
    return tank;
}

Mesh* object2D::CreateCircle(const std::string& name, glm::vec3 color)
{
    glm::vec3 center = glm::vec3(0, 0, 0);
    float radius = 1.0f;

    std::vector<VertexFormat> vertices = {
        VertexFormat(center, color),  

        VertexFormat(center + glm::vec3(radius * cos(0.0f), radius * sin(0.0f), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 1), radius * sin(2.0f * M_PI / 20 * 1), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 2), radius * sin(2.0f * M_PI / 20 * 2), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 3), radius * sin(2.0f * M_PI / 20 * 3), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 4), radius * sin(2.0f * M_PI / 20 * 4), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 5), radius * sin(2.0f * M_PI / 20 * 5), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 6), radius * sin(2.0f * M_PI / 20 * 6), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 7), radius * sin(2.0f * M_PI / 20 * 7), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 8), radius * sin(2.0f * M_PI / 20 * 8), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 9), radius * sin(2.0f * M_PI / 20 * 9), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 10), radius * sin(2.0f * M_PI / 20 * 10), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 11), radius * sin(2.0f * M_PI / 20 * 11), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 12), radius * sin(2.0f * M_PI / 20 * 12), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 13), radius * sin(2.0f * M_PI / 20 * 13), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 14), radius * sin(2.0f * M_PI / 20 * 14), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 15), radius * sin(2.0f * M_PI / 20 * 15), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 16), radius * sin(2.0f * M_PI / 20 * 16), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 17), radius * sin(2.0f * M_PI / 20 * 17), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 18), radius * sin(2.0f * M_PI / 20 * 18), 0), color),
        VertexFormat(center + glm::vec3(radius * cos(2.0f * M_PI / 20 * 19), radius * sin(2.0f * M_PI / 20 * 19), 0), color),
    };

    std::vector<unsigned int> indices = {
        // Hardcoded indices for each triangle
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 5,
        0, 5, 6,
        0, 6, 7,
        0, 7, 8,
        0, 8, 9,
        0, 9, 10,
        0, 10, 11,
        0, 11, 12,
        0, 12, 13,
        0, 13, 14,
        0, 14, 15,
        0, 15, 16,
        0, 16, 17,
        0, 17, 18,
        0, 18, 19,
        0, 19, 1,  // Connect last to the first
    };

    Mesh* circle = new Mesh(name);

    if (false) {
        circle->SetDrawMode(GL_LINE_LOOP);  // Optional: Draw only the outline
    }

    circle->InitFromData(vertices, indices);
    return circle;
}

Mesh* object2D::CreateHealthBar(const std::string& name)
{
    glm::vec3 center = glm::vec3(0, 0, 0);
    glm::vec3 color = glm::vec3(1);

    std::vector<VertexFormat> vertices =
    {

        VertexFormat(center + glm::vec3(-8, -2, 0), color),
        VertexFormat(center + glm::vec3(8.5, -2.5, 0), color),
        VertexFormat(center + glm::vec3(8, -2, 0), color),

        VertexFormat(center + glm::vec3(8, -2, 0), color),
        VertexFormat(center + glm::vec3(8.5, -2.5, 0), color),
        VertexFormat(center + glm::vec3(8, 2, 0), color),

        VertexFormat(center + glm::vec3(8.5, -2.5, 0), color),
        VertexFormat(center + glm::vec3(8.5, 2.5, 0), color),
        VertexFormat(center + glm::vec3(8, 2, 0), color),

        VertexFormat(center + glm::vec3(8, 2, 0), color),
        VertexFormat(center + glm::vec3(8.5, 2.5, 0), color),
        VertexFormat(center + glm::vec3(-8.5, 2.5, 0), color),

        VertexFormat(center + glm::vec3(-8, 2, 0), color),
        VertexFormat(center + glm::vec3(8, 2, 0), color),
        VertexFormat(center + glm::vec3(-8.5, 2.5, 0), color),

        VertexFormat(center + glm::vec3(-8.5, 2.5, 0), color),
        VertexFormat(center + glm::vec3(-8.5, -2.5, 0), color),
        VertexFormat(center + glm::vec3(-8, 2, 0), color),

        VertexFormat(center + glm::vec3(-8, 2, 0), color),
        VertexFormat(center + glm::vec3(-8.5, -2.5, 0), color),
        VertexFormat(center + glm::vec3(-8, -2, 0), color),

        VertexFormat(center + glm::vec3(-8.5, -2.5, 0), color),
        VertexFormat(center + glm::vec3(8.5, -2.5, 0), color),
        VertexFormat(center + glm::vec3(-8, -2, 0), color),
    };

    Mesh* tank = new Mesh(name);
    std::vector<unsigned int> indices(24, 0);
    std::iota(indices.begin(), indices.end(), 0);

    if (false) {
        tank->SetDrawMode(GL_LINE_LOOP);
    }

    tank->InitFromData(vertices, indices);
    return tank;
}

