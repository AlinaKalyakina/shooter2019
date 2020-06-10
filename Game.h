//
// Created by alina on 30.03.19.
//

#ifndef MAIN_GAME_H
#define MAIN_GAME_H

#include "Model.h"
#include "Interface.h"
#include "Fire.h"
#include "Particles.h"
#include "Cubemap.h"

#include <random>
#include <memory>

using glm::vec3, glm::mat4, glm::radians, glm::vec2;

enum GameState {
    GAME_ACTIVE,
    GAME_WIN,
    GAME_LOSS
};

enum class ObjectType {
    ASTEROID,
    ENEMY,
    BULLET,
    NONE
};

class GameObject {
protected:

    mat4 model_mat = mat4(1.f);
    mat4 init_transform = mat4(1.f);
    float speed;
    const Model* model;
    const ShaderProgram& ordinary_pipeline;
    const ShaderProgram& explode_pipeline;
    float time_after_explosion = 0.f;
public:
    bool explode = false;
    vec3 pos = vec3(0., 0., 0.);
    vec3 dir;
    GameObject(const ShaderProgram& ordinary, const ShaderProgram& explode, const Model*, vec3 dir, float speed, vec3 pos = vec3(0., 0., 0.));
    virtual void Draw(const mat4&, const mat4&) const;
    virtual bool Update(float);
    virtual ObjectType type() { return ObjectType::NONE;};
};

class Bullet : public GameObject{
    glm::vec4 color;
public:
    Bullet(vec3 init_pos, vec3 dir, float speed, Model* model,
            const ShaderProgram& ordinary, const ShaderProgram& explode_shader,
            glm::vec4 color);
    void Draw(const mat4& view, const mat4& projective) const;
    ObjectType type() { return ObjectType::BULLET;};
};


class Enemy: public GameObject{
public:
    Enemy(vec2 angles, float dist, float speed, const Model*,
            const ShaderProgram& ordinary_pipeline, const ShaderProgram& shot_pipeline);
    ObjectType type() { return ObjectType::ENEMY;};
};

class Asteroid : public GameObject{
    float lifetime = 0.f;
public:
    Asteroid(vec2 angles, float dist, float speed, const Model*,
             const ShaderProgram& ordinary_pipeline, const ShaderProgram& shot_pipeline);
    bool Update(float);
    ObjectType type() { return ObjectType::ASTEROID;};
};


class Game {
    GameState state = GAME_ACTIVE;
    int health_score = 100;
    int achievements_score = 0;
    std::vector<Bullet*> my_bullets;
    std::vector<Bullet*> enemy_bullets;
    std::vector<Fire*> explodes;
    std::vector<GameObject*> enemy_asteroid;

    std::unordered_map<SHADER_TYPE, ShaderProgram*>& pipelines;
    std::unordered_map<MODEL_TYPE, Model*>&models;

    Interface* interface;
    Particles particles;
    Cubemap cubemap;

    float last_object_generated = 0.f;
    double game_time = 0.;
    MODEL_TYPE current_arm = MODEL_TYPE::MY_BULLET_1;

    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen;
    std::uniform_real_distribution<> angles_x;
    std::uniform_real_distribution<> angles_y;

    void shoot(vec3 dir);
    void search_hits();
    void generate_objects();
    void enemy_shoot();
public:
    Game(std::unordered_map<SHADER_TYPE, ShaderProgram*>& p, std::unordered_map<MODEL_TYPE, Model*>& loaded_models,
         Interface * i, std::string* cubemap_textures);
    void Init();
    void Update(double);
    void Draw(const mat4& view, const mat4& project);
};


#endif //MAIN_GAME_H
