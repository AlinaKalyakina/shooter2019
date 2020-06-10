//
// Created by alina on 30.03.19.
//

#include "Game.h"
#include "constants.h"
//Cubemap skybox(textures, *pipelines[SHADER_TYPE::CUBEMAP_SHADER]);

using glm::vec4, glm::vec2;

Game::Game(std::unordered_map<SHADER_TYPE, ShaderProgram *> &p, std::unordered_map<MODEL_TYPE, Model *> &loaded_models,
           Interface *i, std::string *cubemap_textures) : pipelines(p), models(loaded_models), interface(i),
                                                          particles(*p[SHADER_TYPE::PARCTICLES_SHADER]),
                                                          angles_x(radians(-FOV / 5), radians(FOV / 3)),
                                                          angles_y(radians(-FOV / 2.5), radians(FOV / 2.5)),
                                                          gen(rd()),
                                                          cubemap(cubemap_textures, *p[SHADER_TYPE::CUBEMAP_SHADER]) {}


GameObject::GameObject(const ShaderProgram &ordinary, const ShaderProgram &explode, const Model *m, vec3 d, float s,
                       vec3 p) :
        ordinary_pipeline(ordinary), explode_pipeline(explode), dir(d), pos(p), speed(s), model(m),
        init_transform(m->transform) {}

bool GameObject::Update(float delta) {
    if (!explode) {
        pos += delta * speed * dir;
        model_mat = glm::translate(mat4(1.f), pos) * init_transform;
    } else {
        if (time_after_explosion > EXPLODE_DURALITY)
            return false;
        time_after_explosion += delta;
    }
    return pos.z < 0 && pos.z > -FURTHERST;
}

void GameObject::Draw(const mat4 &view, const mat4 &projection) const {
    auto shader = explode ? explode_pipeline : ordinary_pipeline;
    shader.StartUseShader();
    GL_CHECK_ERRORS;
    shader.SetUniform("view", view);
    GL_CHECK_ERRORS;
    shader.SetUniform("projection", projection);
    GL_CHECK_ERRORS;
    shader.SetUniform("model", model_mat);
    GL_CHECK_ERRORS;
    if (explode) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader.SetUniform("time", time_after_explosion);
    }
    model->Draw();
    glDisable(GL_BLEND);
    GL_CHECK_ERRORS;
    shader.StopUseShader();
}

void Bullet::Draw(const mat4 &view, const mat4 &projection) const {
    auto shader = explode ? explode_pipeline : ordinary_pipeline;
    shader.StartUseShader();
    GL_CHECK_ERRORS;
    shader.SetUniform("view", view);
    GL_CHECK_ERRORS;
    shader.SetUniform("projection", projection);
    GL_CHECK_ERRORS;
    shader.SetUniform("model", model_mat);
    GL_CHECK_ERRORS;
    if (explode) {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader.SetUniform("time", time_after_explosion);
    } else {
        shader.SetUniform("color", color);
    }
    model->Draw();
    glDisable(GL_BLEND);
    shader.StopUseShader();
}

Enemy::Enemy(vec2 angles, float dist, float v, const Model *obj, const ShaderProgram &ordinary,
             const ShaderProgram &shot) :
        GameObject(ordinary, shot, obj, vec3(0., 0., 0.), v, vec3(0., 0., 0.)) {

    mat4 rotation = glm::rotate(mat4(1.0), angles[0], vec3(1., 0., 0.));
    rotation = glm::rotate(rotation, angles[1], vec3(0., 1., 0.));

    pos = dist * vec3(rotation * vec4(0, 0, -1, 1));
    dir = -pos;
    dir.z += 1;
    dir = glm::normalize(dir);
    init_transform = rotation * init_transform;
    model_mat = glm::translate(mat4(1.0), pos) * init_transform;
}

Asteroid::Asteroid(vec2 angles, float dist, float v, const Model *obj, const ShaderProgram &ordinary,
                   const ShaderProgram &shot) :
        GameObject(ordinary, shot, obj, vec3(0., 0., 0.), v, vec3(0., 0., 0.)) {

    mat4 rotation = glm::rotate(mat4(1.0), angles[0], vec3(1., 0., 0.));
    rotation = glm::rotate(rotation, angles[1], vec3(0., 1., 0.));

    pos = dist * vec3(rotation * vec4(0, 0, -1, 1));
    dir = -pos;
    dir.z += 1;
    dir = glm::normalize(dir);
    init_transform = rotation * init_transform *
                     glm::rotate(mat4(1.0), glm::radians(360.f * float(rand() % 35) / 35.f), vec3(1.0, 0., 0.)) *
                     glm::rotate(mat4(1.0), glm::radians(360.f * float(rand() % 35) / 35.f), vec3(0.0, 1., 0.));
    model_mat = glm::translate(mat4(1.0), pos) * init_transform;
}


void Game::Init() {
//    Asteroid::Asteroid(vec3 init_pos, float speed, Model *model, const ShaderProgram & ordinary, const ShaderProgram &shot) :
//    GameObject(ordinary, shot, model, -normalize(init_pos), speed, init_pos){}
    //explodes.push_back(new Fire(*pipelines[SHADER_TYPE::FIRE_SHADER], models[MODEL_TYPE::FIRE], vec3(0., 0., -50.)));
    //   asteroids.push_back(new Asteroid(vec3(0, 0, -50), 10, models[MODEL_TYPE::ASTEROID_1],
    //           *pipelines[SHADER_TYPE::ENEMY_SHADER], *pipelines[SHADER_TYPE::EXPLOSION_SHADER]));
}

void Game::Draw(const mat4 &view, const mat4 &projection) {
    switch (state) {
        case GAME_LOSS:
            interface->Draw("loss");
            //std::cout << "loss";
            break;
            //exit(0);
        case GAME_ACTIVE:
            cubemap.Draw(view, projection);
            particles.Draw(view, projection);
            for (const auto &x : enemy_asteroid) {
                if (!x->explode)
                    x->Draw(view, projection);
            }
            for (const auto &x: explodes) {
                x->Draw(view, projection);
            }
            for (const auto &x : enemy_asteroid) {
                if (x->explode)
                    x->Draw(view, projection);
            }
            for (const auto &x: my_bullets) {
                x->Draw(view, projection);
            }
            for (const auto &x: enemy_bullets) {
                x->Draw(view, projection);
            }

            interface->Draw(health_score, achievements_score);
            break;
        case GAME_WIN:
            interface->Draw("win");
            //std::cout <<"WIN";
            break;
            //exit(0);
    }
}

void Game::Update(double cur_time) {
    double delta = cur_time - game_time;
    game_time = cur_time;
    if (state != GameState::GAME_ACTIVE)
        return;
    particles.Update(delta);
    for (auto it = enemy_asteroid.begin(); it != enemy_asteroid.end();) {
        if (!(*it)->Update(delta)) {
            if ((*it)->pos.z > 0) {
                state = GameState::GAME_LOSS;
                return;
            }
            free(*it);
            it = enemy_asteroid.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = my_bullets.begin(); it != my_bullets.end();) {
        if (!(*it)->Update(delta)) {
            free(*it);
            it = my_bullets.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = enemy_bullets.begin(); it != enemy_bullets.end();) {
        if (!(*it)->Update(delta)) {
            health_score -= 1;
            interface->add_crack(float(-1.0 / (*it)->dir.z) * (*it)->dir);
            free(*it);
            it = enemy_bullets.erase(it);
        } else {
            ++it;
        }
    }
    for (auto it = explodes.begin(); it != explodes.end();) {
        if (!(*it)->Update(delta)) {
            free(*it);
            it = explodes.erase(it);
        } else {
            ++it;
        }
    }
    search_hits();
    if (health_score < 0) {
        state = GAME_LOSS;
        return;
    }
    //if (achievements_score > 100) {
    //    state = GAME_WIN;
    //    return;
    //}
    auto tmp = interface->click_dir();
    if (tmp[2] != 0) {
        shoot(tmp);
    }
    if ((last_object_generated += delta) > MIN_TIME_GAP) {
        generate_objects();
    }
    if (!enemy_asteroid.empty())
        enemy_shoot();
}

void Game::generate_objects() {
    auto tmp = rand() % OBJECT_GENERATE_PROB;
    if (tmp == 0) {
        last_object_generated = 0.f;
        auto ship_type = rand() % 3;

        enemy_asteroid.push_back(new Enemy(glm::vec2(angles_x(gen), angles_y(gen)), FURTHERST,
                                           glm::min(game_time / 10, ENEMY_SPEED * 2) + ENEMY_SPEED,
                                           models[MODEL_TYPE(int(MODEL_TYPE::ENEMY_1) + ship_type)],
                                           *pipelines[SHADER_TYPE::ENEMY_SHADER],
                                           *pipelines[SHADER_TYPE::EXPLOSION_SHADER]));
    }
    if (tmp == 1) {
        last_object_generated = 0.f;
        auto asteroid_type = rand() % 2;

        enemy_asteroid.push_back(new Asteroid(glm::vec2(angles_x(gen), angles_y(gen)), FURTHERST,
                                              glm::min(game_time / 10, ENEMY_SPEED * 2) + ENEMY_SPEED,
                                              models[MODEL_TYPE(int(MODEL_TYPE::ASTEROID_1) + asteroid_type)],
                                              *pipelines[SHADER_TYPE::ENEMY_SHADER],
                                              *pipelines[SHADER_TYPE::EXPLOSION_SHADER]));
    }

}

void Game::search_hits() {
    for (auto &enemy : enemy_asteroid) {
        if (enemy->explode)
            continue;
        for (auto &bullet : my_bullets) {
            if (bullet->explode)
                continue;
            if (glm::length(enemy->pos - bullet->pos) < HIT_DIST) {
                enemy->explode = true;
                bullet->explode = true;
                explodes.push_back(new Fire(*pipelines[SHADER_TYPE::FIRE_SHADER], models[MODEL_TYPE::FIRE],
                                            0.5f * (enemy->pos + bullet->pos)));
                achievements_score += 1;
            }
        }
    }

}

void Game::enemy_shoot() {
    if ((rand() % SHOOT_PROB) == 0) {//one of enemies shoote
        size_t ind = rand() % enemy_asteroid.size();
        if (!enemy_asteroid[ind]->explode && enemy_asteroid[ind]->pos.z > -FURTHERST * 0.4 &&
            enemy_asteroid[ind]->type() == ObjectType::ENEMY)
            enemy_bullets.push_back(
                    new Bullet(enemy_asteroid[ind]->pos, -normalize(enemy_asteroid[ind]->pos), MY_BULLET_SPEED * 1.5,
                               models[MODEL_TYPE::ENEMY_BULLET], *pipelines[SHADER_TYPE::BULLET_SHADER],
                               *pipelines[SHADER_TYPE::EXPLOSION_SHADER], vec4(1., 1., 0., 1.)));
    }
}

void Game::shoot(vec3 dir) {
    my_bullets.push_back(new Bullet(2.f * dir, dir, MY_BULLET_SPEED, models[current_arm],
                                    *pipelines[SHADER_TYPE::BULLET_SHADER], *pipelines[SHADER_TYPE::EXPLOSION_SHADER],
                                    vec4(1., 0., 1., 1.)));
}


Bullet::Bullet(vec3 init_pos, vec3 dir, float speed, Model *model, const ShaderProgram &ordinary,
               const ShaderProgram &explode_shader, glm::vec4 c) :
        GameObject(ordinary, explode_shader, model, dir, speed, init_pos), color(c) {
}

bool Asteroid::Update(float delta) {
    if (!explode) {
        lifetime += delta;
        pos += delta * speed * dir;
        model_mat = glm::translate(mat4(1.f), pos) * init_transform * glm::rotate(mat4(1.0), lifetime, dir);
    } else {
        if (time_after_explosion > EXPLODE_DURALITY)
            return false;
        time_after_explosion += delta;
    }
    return pos.z < 0 && pos.z > -FURTHERST;
}
