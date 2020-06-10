//internal includes
#include "glm/glm.hpp"
#include "glm/ext.hpp"
#include "Game.h"
#include "Cubemap.h"
#include "common.h"
#include "ShaderProgram.h"
#include "Particles.h"
#include "Fire.h"
#include "constants.h"

//External dependencies
#define GLFW_DLL
#include <GLFW/glfw3.h>
#include <random>
#include <assimp/Importer.hpp>

//#include <ft2build.h>
//#include FT_FREETYPE_H

#define CAN_RESIZE_WINDOW 0
#define SPEED 0.1

using glm::mat4, glm::vec4, glm::vec3, glm::radians, glm::vec2, glm::mat3;
//bkg1_back6.jpg
//vector<std::string> faces;
//{
//"right.jpg",
//"left.jpg",
//"top.jpg",
//"bottom.jpg",
//"back.jpg",
//"front.jpg"
//};

std::string textures[] = {"cubemap/right.png", "cubemap/left.png",
                          "cubemap/top.png", "cubemap/bot.png",
                          "cubemap/front.png", "cubemap/back.png"};
const std::unordered_map<std::string, std::string> text_textures({{"0", "0.png"}, {"1", "1.png"},
                                                                  {"2", "2.png"}, {"3", "3.png"},
                                                                  {"4", "3.png"}, {"5", "5.png"},
                                                                  {"6", "6.png"}, {"7", "7.png"},
                                                                  {"8", "8.png"}, {"9", "9.png"},
                                                                  {"cabin", "cabin.png"},
                                                                  {"loss", "lost.png"},
                                                                  {"win", "win.png"},
                                                                  {"crack", "cracked-Glass.png"}});

static GLsizei WIDTH =1280, HEIGHT = 960; //размеры окна
mat4 view(1.0), projection = glm::perspective(radians(FOV), (float)WIDTH/(float)HEIGHT, NEAREST, FURTHERST);

double global_time;

std::unordered_map<SHADER_TYPE, ShaderProgram*> pipelines;
std::unordered_map<MODEL_TYPE, Model*> loaded_models;


//std::vector<Model*> loaded_enemies;
Interface* interface = nullptr;


void Load_shaders() {
	std::unordered_map<GLenum, std::string> shaders;
	shaders[GL_VERTEX_SHADER]   = "../shaders/vertex_model.glsl";
	shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment_model.glsl";
	pipelines[SHADER_TYPE::ENEMY_SHADER] = new ShaderProgram(shaders);GL_CHECK_ERRORS;
	shaders.clear();

	shaders[GL_VERTEX_SHADER] = "../shaders/vertex_cubemap.glsl";
	shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment_cubemap.glsl";
	pipelines[SHADER_TYPE ::CUBEMAP_SHADER] = new ShaderProgram(shaders);GL_CHECK_ERRORS;
    shaders.clear();

	shaders[GL_VERTEX_SHADER] = "../shaders/vertex_aim.glsl";
	shaders[GL_GEOMETRY_SHADER] = "../shaders/geometry_aim.glsl";
	shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment_aim.glsl";
	pipelines[SHADER_TYPE ::AIM_SHADER] = new ShaderProgram(shaders);GL_CHECK_ERRORS;
	shaders.clear();

	shaders[GL_VERTEX_SHADER] = "../shaders/vertex_particles.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment_particles.glsl";
    pipelines[SHADER_TYPE ::PARCTICLES_SHADER] = new ShaderProgram(shaders);GL_CHECK_ERRORS;
    shaders.clear();

    shaders[GL_VERTEX_SHADER] ="../shaders/vertex_fire.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment_simple.glsl";
    pipelines[SHADER_TYPE ::FIRE_SHADER] = new ShaderProgram(shaders);GL_CHECK_ERRORS;
    shaders.clear();

    shaders[GL_VERTEX_SHADER] = "../shaders/vertex_shot.glsl";
    shaders[GL_GEOMETRY_SHADER] = "../shaders/geometry_shot.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment_model.glsl";
    pipelines[SHADER_TYPE ::EXPLOSION_SHADER] = new ShaderProgram(shaders);GL_CHECK_ERRORS;
    shaders.clear();

    shaders[GL_VERTEX_SHADER] = "../shaders/vertex_simple.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment_simple.glsl";
    pipelines[SHADER_TYPE ::BULLET_SHADER] = new ShaderProgram(shaders);GL_CHECK_ERRORS;
    shaders.clear();

    shaders[GL_VERTEX_SHADER] = "../shaders/vertex_info.glsl";
    shaders[GL_FRAGMENT_SHADER] = "../shaders/fragment_info.glsl";
    pipelines[SHADER_TYPE ::INFO_SHADER] = new ShaderProgram(shaders);GL_CHECK_ERRORS;
}

void Load_models() {
    loaded_models[MODEL_TYPE::ENEMY_1] = new Model("../models/Aircraft/E 45 Aircraft_obj.obj",
                                          glm::translate(mat4(1.0), vec3(0., 0, 0)) *
                                          glm::rotate(mat4(1.0), (float) radians(180.), vec3(0., 1., 0))*
                                          glm::scale(mat4(1.0), vec3(0.5, 0.5, 0.5))
                                          );
    loaded_models[MODEL_TYPE ::ENEMY_2] = new Model("../models/SciFi_Fighter_AK5/SciFi_Fighter_AK5.obj",
                                          glm::translate(mat4(1.0), vec3(0., 0., -10.)) *
                                          glm::scale(mat4(1.0), vec3(0.005, 0.005, 0.005))
                                          );
	loaded_models[MODEL_TYPE::ENEMY_3] = new Model("../models/Corvette-F3/Corvette-F3.obj",
										  glm::translate(mat4(1.0), vec3(0., 0., -10.)) *
										  glm::scale(mat4(1.0), vec3(0.005, 0.005, 0.005))
										  );

    loaded_models[MODEL_TYPE::FIRE] = new Model("../models/explosion.obj",
			glm::translate(mat4(1.0),vec3(0., -0.42, 2.25))*
			glm::scale(mat4(1.0), vec3(3.5, 4, 3.5)));
    loaded_models[MODEL_TYPE::MY_BULLET_1] = new Model("../models/sphere.obj",
    		glm::scale(mat4(1.f), vec3(0.1, 0.1, 0.1)));

    loaded_models[MODEL_TYPE::ASTEROID_1] = new Model("../models/asteroid1/10464_Asteroid_v1_Iterations-2.obj",
    		glm::scale(mat4(1.0), vec3(0.002, 0.002, 0.002)));
    loaded_models[MODEL_TYPE::ASTEROID_2] = new Model("../models/asteroid2/10464_Asteroid_v1_Iterations-2.obj",
			glm::rotate(mat4(1.0), float(radians(45.f)), vec3(1.f, 0.f, 0.f))*
    		glm::scale(mat4(1.0), vec3(0.002, 0.002, 0.002)));
    loaded_models[MODEL_TYPE::ENEMY_BULLET] = loaded_models[MODEL_TYPE::MY_BULLET_1];
    for ( auto &x: loaded_models) {
        x.second->Init();
    }
}

int initGL() {
	int res = 0;
	//грузим функции opengl через glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize OpenGL context" << std::endl;
		return -1;
	}

	std::cout << "Vendor: "   << glGetString(GL_VENDOR) << std::endl;
	std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;
	std::cout << "Version: "  << glGetString(GL_VERSION) << std::endl;
	std::cout << "GLSL: "     << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

	return 0;
}

static void windowResize(GLFWwindow* window, int width, int height) {
	WIDTH  = width;
	HEIGHT = height;
	interface->width = width;
	interface->height = height;
	projection = glm::perspective(radians(FOV), (float)WIDTH/(float)HEIGHT, NEAREST, FURTHERST);
}

static void mouseMove(GLFWwindow* window, double xpos, double ypos) {
	interface->cursor[0] = xpos;
	interface->cursor[1] = ypos;
}

static void mouseClick(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_RIGHT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &interface->clicked[0], &interface->clicked[1]);
        interface->click = true;

	}
}

static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action == GLFW_RELEASE) {
		return;
	}
	switch (key) {
        case GLFW_KEY_A:
            interface->angles[0] -= SPEED;
            break;
        case GLFW_KEY_D:
            interface->angles[0] += SPEED;
            break;
        case GLFW_KEY_W:
            interface->angles[1] += SPEED;
            break;
        case GLFW_KEY_S:
            interface->angles[1] -= SPEED;
            break;
    }
}

int main(int argc, char** argv)
{
	if(!glfwInit())
    return -1;

	//запрашиваем контекст opengl версии 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#if CAN_RESIZE_WINDOW == 1
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);
#else
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#endif


    GLFWwindow*  window = glfwCreateWindow(WIDTH, HEIGHT, "OpenGL basic sample", nullptr, nullptr);
    if (window == nullptr)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

#if CAN_RESIZE_WINDOW == 1
	glfwSetWindowSizeCallback(window, windowResize);
#endif
	glfwSetCursorPosCallback (window, mouseMove);
	glfwSetMouseButtonCallback(window, mouseClick);

	glfwMakeContextCurrent(window); 
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	if(initGL() != 0) 
		return -1;
	
  //Reset any OpenGL errors which could be present for some reason
	GLenum gl_error = glGetError();
	while (gl_error != GL_NO_ERROR)
		gl_error = glGetError();

    glfwSwapInterval(1); // force 60 frames per second


	Load_shaders();
	Load_models();


    interface = new Interface(int(WIDTH), int(HEIGHT), *pipelines[SHADER_TYPE::AIM_SHADER],
            *pipelines[SHADER_TYPE::INFO_SHADER], text_textures);
    Game game(pipelines, loaded_models, interface, textures);

    game.Init();
    glfwGetCursorPos(window, &interface->cursor[0], &interface->cursor[1]);

    glEnable(GL_DEPTH_TEST); GL_CHECK_ERRORS

    glfwSetTime(0.);

	while (!glfwWindowShouldClose(window))
	{

        glViewport  (0, 0, WIDTH, HEIGHT); GL_CHECK_ERRORS
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); GL_CHECK_ERRORS
        glClear     (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); GL_CHECK_ERRORS;

		glfwPollEvents();

		game.Update(glfwGetTime());
		game.Draw(view, projection);

		glfwSwapBuffers(window);
	}

	glfwTerminate();
	return 0;
}
