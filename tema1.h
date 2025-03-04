#pragma once

#include "components/simple_scene.h"
#include <vector>

#define MAP_DIVISIONS 1000.0f 
#define DIVISION_LENGTH (1280.0f / MAP_DIVISIONS)

#define TANK_SPEED 150.0f
#define BARREL_SPEED 1.5f 
#define PROJECTILE_SPEED projectile_speed

#define TANK_SCALE 10.0f
#define PROJECTILE_SCALE 7.0f

#define CRATER_RADIUS 40.0f

#define TANK_HIT_BOX 40.0f
#define TANK_DAMAGE 7.5f

#define PI 3.1415926535f

//2.3 is the center
#define BARREL_POSY 3.0f

#define SLIDING_FACTOR 0.15f 

#define TRAJECTORY_POINTS 50.0f
#define TRAJECTORY_UPDATE_TIME 0.5f


namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

     protected:        
        struct Tank {
            float x;
            float y;
            float angle;
            float health = 100.0f;
            float barrel_angle;
            bool is_alive = true;
        };

        struct Projectile {
            float x_start;
            float y_start;
            float time = 0;
            float angle;
            Projectile(Tank tank) :x_start(tank.x- sin(tank.angle) * TANK_SCALE * BARREL_POSY), y_start(tank.y + cos(tank.angle)* TANK_SCALE* BARREL_POSY), angle(tank.angle + PI/2 + tank.barrel_angle) {}
        };
        std::vector<float> map_grid;

        Tank tank1;
        Tank tank2;

        std::vector<Projectile> projectiles;
        void UpdateTank(Tank& tank1, float deltaTimeSeconds, const std::string& tank_name);
        void UpdateTerrain();
        void UpdateProjectiles(float deltaTimeSeconds);
        float Calculate_projectile_crater(int k, int impact_point);
        void RenderHealthBar(Tank tank);
        void UpdateTerrainSliding();
        float terrain_func(float x);
        float motion_func(float angle, float x, float speed);
        float motion_x(float angle, float time, float speed);
        float motion_y(float angle, float time, float speed);

        float projectile_speed = 75.0f;
    };
} 
