#include "lab_m1/tema1/tema1.h"

#include <vector>
#include <iostream>
#include <cmath>

#include "lab_m1/tema1/transform2D.h"
#include "lab_m1/tema1/object2D.h"

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema1::Tema1()
{
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    glm::ivec2 resolution = window->GetResolution();
    auto camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    glm::vec3 corner = glm::vec3(0, -1, 0);
    float squareSide = 1;

    Mesh* square = object2D::CreateSquare("square", corner, squareSide, glm::vec3(0.62, 0.475, 0.204), true);
    AddMeshToList(square);

    Mesh* square_white = object2D::CreateSquare("square_white", corner, squareSide, glm::vec3(1, 0, 0), true);
    AddMeshToList(square_white);



    map_grid = std::vector<float>(MAP_DIVISIONS + 1, 0.0f);

    for (int i = 0; i < MAP_DIVISIONS + 1; i++) {
        float a_x = i * DIVISION_LENGTH;
        float a_y = terrain_func(a_x);
        
        map_grid[i] = a_y;
    }

    tank1.x = 100.0f;
    tank1.angle = 0.0f;
    tank1.barrel_angle = -PI/2;

    tank2.x = 1000.0f;
    tank2.angle = 0.0f;
    tank2.barrel_angle = PI / 2;


    Mesh* tank1_mesh = object2D::CreateTank("tank1", glm::vec3(0.85, 0, 0.85), glm::vec3(0.60, 0, 0.60));
    AddMeshToList(tank1_mesh);
    Mesh* tank2_mesh = object2D::CreateTank("tank2", glm::vec3(0, 0.85, 0), glm::vec3(0, 0.60, 0));
    AddMeshToList(tank2_mesh);
    Mesh* barrel_mesh = object2D::CreateBarrel("barrel");
    AddMeshToList(barrel_mesh);

    Mesh* projectile_mesh = object2D::CreateCircle("projectile", glm::vec3(0, 0, 0));
    AddMeshToList(projectile_mesh);

    Mesh* trajectory_mesh = object2D::CreateCircle("trajectory", glm::vec3(0.75, 0.75, 0.75));
    AddMeshToList(trajectory_mesh);

    Mesh* health_bar_mesh = object2D::CreateHealthBar("health_bar");
    AddMeshToList(health_bar_mesh);

    cout << "W A S D (SPACE)- controale tank1" << endl;
    cout << "UP DOWN LEFT RIGH (ENTER) - controale tank2" << endl;
    cout << "control viteza proiecil - N M" << endl;
}


void Tema1::FrameStart()
{ 
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.42, 0.067, 0.067, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


float Tema1::terrain_func(float x) {
    return 200 + 55 * sin(0.02 * x) + 50 * cos(0.013 * x);
}

float Tema1::motion_func(float angle, float x, float speed) {
    return x * tan(angle) - 10.0f * x * x / (2 * (speed * cos(angle)) * (speed * cos(angle)));
}

float Tema1::motion_x(float angle, float time, float speed) {
    return speed * time * cos(angle);
}

float Tema1::motion_y(float angle, float time, float speed) {
    return speed * time * sin(angle) - 0.5f * 10 * time * time;
}

void Tema1::UpdateTank(Tank &tank, float deltaTimeSeconds, const std::string& tank_name) {

    if (!tank.is_alive)
        return;

    for (int i = 0; i < MAP_DIVISIONS; i++) {
        float a_x = i * DIVISION_LENGTH;
        float b_x = (i + 1) * DIVISION_LENGTH;
        float a_y = map_grid[i];
        float b_y = map_grid[i + 1];


        if (a_x <= tank.x && b_x >= tank.x) {
            //calculates tank Y pos and angle
            float t = (tank.x - a_x) / (b_x - a_x);
            tank.y = a_y + t * (b_y - a_y);
            tank.angle = atan2(b_y - a_y, b_x - a_x);


            glm::mat3 modelMatrix = glm::mat3(1);

            //renders the tank
            modelMatrix *= transform2D::Translate(tank.x, tank.y);
            modelMatrix *= transform2D::Rotate(tank.angle);
            modelMatrix *= transform2D::Scale(TANK_SCALE, TANK_SCALE);
            RenderMesh2D(meshes[tank_name], shaders["VertexColor"], modelMatrix);

            //renders the tank barrel
            modelMatrix = glm::mat3(1);
            modelMatrix *= transform2D::Translate(tank.x, tank.y);
            //SOON TO BE EXPLAINED
            modelMatrix *= transform2D::Translate(-sin(tank.angle) * TANK_SCALE * BARREL_POSY, cos(tank.angle) * TANK_SCALE * BARREL_POSY);
            modelMatrix *= transform2D::Rotate(tank.angle + tank.barrel_angle);
            modelMatrix *= transform2D::Scale(TANK_SCALE, TANK_SCALE);
            RenderMesh2D(meshes["barrel"], shaders["VertexColor"], modelMatrix);

        }
    }

    //IF PROJECTILE TOUCHES MAP OR LEAVES SCREEN THEN DELETE FROM VECTOR
    Projectile trajectory(tank);
    for (int i = 0; i < TRAJECTORY_POINTS; i++) {
        trajectory.time += TRAJECTORY_UPDATE_TIME;
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(trajectory.x_start + motion_x(trajectory.angle, trajectory.time, PROJECTILE_SPEED), motion_y(trajectory.angle, trajectory.time, PROJECTILE_SPEED) + trajectory.y_start);
        modelMatrix *= transform2D::Scale(2, 2);
        RenderMesh2D(meshes["trajectory"], shaders["VertexColor"], modelMatrix);
    }
}

void Tema1::UpdateTerrain() {
    for (int i = 0; i < MAP_DIVISIONS; i++) {
        float a_x = i * DIVISION_LENGTH;
        float b_x = (i + 1) * DIVISION_LENGTH;
        float a_y = map_grid[i];
        float b_y = map_grid[i + 1];

        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(a_x, a_y);
        modelMatrix *= transform2D::Shearing(b_y - a_y, b_x - a_x);
        modelMatrix *= transform2D::Scale(b_x - a_x, max(b_y, a_y));
        RenderMesh2D(meshes["square"], shaders["VertexColor"], modelMatrix);
    }
}

float Tema1::Calculate_projectile_crater(int k, int impact_point_y) {
    float radius_squared = CRATER_RADIUS * CRATER_RADIUS;
    float y = impact_point_y - sqrt(radius_squared - k*DIVISION_LENGTH * k*DIVISION_LENGTH);
    return y;
}

void Tema1::UpdateProjectiles(float deltaTimeSeconds) {
    for (auto& projectile : projectiles) {
        if (projectile.time == -1) {
            continue;
        }
        projectile.time += deltaTimeSeconds * 10;
        float projectile_pos_x = projectile.x_start + motion_x(projectile.angle, projectile.time, PROJECTILE_SPEED);
        float projectile_pos_y = motion_y(projectile.angle, projectile.time, PROJECTILE_SPEED) + projectile.y_start;
        glm::mat3 modelMatrix = glm::mat3(1);
        modelMatrix *= transform2D::Translate(projectile_pos_x, projectile_pos_y);
        modelMatrix *= transform2D::Scale(PROJECTILE_SCALE, PROJECTILE_SCALE);
        RenderMesh2D(meshes["projectile"], shaders["VertexColor"], modelMatrix);

        //need function 
        if (tank1.x - TANK_HIT_BOX <= projectile_pos_x && projectile_pos_x <= tank1.x + TANK_HIT_BOX && projectile.time > 0.7f) {
            if (tank1.y - TANK_HIT_BOX <= projectile_pos_y && projectile_pos_y <= tank1.y + TANK_HIT_BOX) {
                tank1.health -= TANK_DAMAGE;
                if (tank1.health < 0) {
                    tank1.health = 0;
                    tank1.is_alive = false;
                    tank1.x = 0;
                    tank1.y = 0;
                }
                projectile.time = -1;
                continue;
            }
        }
        if (tank2.x - TANK_HIT_BOX <= projectile_pos_x && projectile_pos_x <= tank2.x + TANK_HIT_BOX && projectile.time > 0.7f) {
            if (tank2.y - TANK_HIT_BOX <= projectile_pos_y && projectile_pos_y <= tank2.y + TANK_HIT_BOX) {
                tank2.health -= TANK_DAMAGE;
                if (tank2.health < 0) {
                    tank2.health = 0;
                    tank2.is_alive = false;
                    tank2.x = 0;
                    tank2.y = 0;
                }
                projectile.time = -1;
                continue;
            }
        }

        //checks for projectile impact with terrain
        int impact_division_x = projectile_pos_x / DIVISION_LENGTH;
        if (impact_division_x < 0 || impact_division_x > MAP_DIVISIONS)
            continue;
        float impact_division_y = map_grid[impact_division_x];
        if (map_grid[impact_division_x] < projectile_pos_y && projectile_pos_y > 0)
            continue;
        for (int division = impact_division_x - CRATER_RADIUS / DIVISION_LENGTH; division <= impact_division_x + CRATER_RADIUS / DIVISION_LENGTH; division++) {
            if (division < 0 || division > MAP_DIVISIONS)
                continue;
            float new_height = Calculate_projectile_crater(division - impact_division_x, impact_division_y);
            if (new_height < map_grid[division]) {
                map_grid[division] = new_height;
            }
        }
        projectile.time = -1;
    }
}

void Tema1::RenderHealthBar(Tank tank) {
    if (!tank.is_alive)
        return;
    glm::mat3 modelMatrix = glm::mat3(1);
    modelMatrix *= transform2D::Translate(tank.x, tank.y + 70);
    //modelMatrix *= transform2D::Scale(3, 3);
    RenderMesh2D(meshes["square_white"], shaders["VertexColor"], modelMatrix * transform2D::Translate(-32, 7) * transform2D::Scale(64 * tank.health / 100.0f, 14));
    modelMatrix *= transform2D::Scale(4, 3.5);
    RenderMesh2D(meshes["health_bar"], shaders["VertexColor"], modelMatrix);
}

void Tema1::UpdateTerrainSliding() {
    for (size_t i = 1; i < MAP_DIVISIONS - 1; i++) {
        float average = (map_grid[i - 1] + map_grid[i] + map_grid[i + 1]) / 3.0f;
        float add = SLIDING_FACTOR * (average - map_grid[i]);
        map_grid[i] += add;
    }
}

void Tema1::Update(float deltaTimeSeconds)
{
    RenderHealthBar(tank1);
    RenderHealthBar(tank2);
   
    UpdateTerrain();

    UpdateProjectiles(deltaTimeSeconds);

    UpdateTank(tank1, deltaTimeSeconds, "tank1");
    UpdateTank(tank2, deltaTimeSeconds, "tank2");

    UpdateTerrainSliding();
}


void Tema1::FrameEnd()
{
}



void Tema1::OnInputUpdate(float deltaTime, int mods)
{
    if (window->KeyHold(GLFW_KEY_D)) {
        tank1.x += deltaTime * TANK_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_A)) {
        tank1.x -= deltaTime * TANK_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_W)) {
        if (tank1.barrel_angle <= PI/2)
            tank1.barrel_angle += deltaTime * BARREL_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_S)) {
        if (tank1.barrel_angle >= -PI / 2)
            tank1.barrel_angle -= deltaTime * BARREL_SPEED;
    }
    
    if (window->KeyHold(GLFW_KEY_RIGHT)) {
        tank2.x += deltaTime * TANK_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_LEFT)) {
        tank2.x -= deltaTime * TANK_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_UP)) {
        if (tank2.barrel_angle <= PI / 2)
            tank2.barrel_angle += deltaTime * BARREL_SPEED;
    }
    if (window->KeyHold(GLFW_KEY_DOWN)) {
        if (tank2.barrel_angle >= -PI / 2)
            tank2.barrel_angle -= deltaTime * BARREL_SPEED;
    }

    if (window->KeyHold(GLFW_KEY_M)) {
        projectile_speed += deltaTime * 10;
    }
    if (window->KeyHold(GLFW_KEY_N)) {
        projectile_speed -= deltaTime * 10;
    }
}


void Tema1::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_SPACE && tank1.is_alive) {
        projectiles.push_back(Projectile(tank1));
    }
    if (key == GLFW_KEY_ENTER && tank2.is_alive) {
        projectiles.push_back(Projectile(tank2));
    }
}


void Tema1::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event
}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}
