#include <iostream>
#include <vector>
#include "glimac/TrackballCamera.hpp"
#include "glimac/sphere_vertices.hpp"
#include "glm/ext/quaternion_geometric.hpp"
#include "glm/ext/scalar_constants.hpp"
#include "glm/fwd.hpp"
#include "glm/gtc/random.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "img/src/Image.h"
#include "p6/p6.h"
#include "glm/gtc/random.hpp"

int const window_width  = 1280;
int const window_height = 720;

struct EarthProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uEarthTexture;
    GLint uCloudTexture;

    EarthProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/multiTex3D.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uEarthTexture = glGetUniformLocation(m_Program.id(), "uEarthTexture");
        uCloudTexture = glGetUniformLocation(m_Program.id(), "uCloudTexture");
    }
};

struct MoonProgram {
    p6::Shader m_Program;

    GLint uMVPMatrix;
    GLint uMVMatrix;
    GLint uNormalMatrix;
    GLint uTexture;

    MoonProgram()
        : m_Program{p6::load_shader("shaders/3D.vs.glsl", "shaders/tex3D.fs.glsl")}
    {
        uMVPMatrix    = glGetUniformLocation(m_Program.id(), "uMVPMatrix");
        uMVMatrix     = glGetUniformLocation(m_Program.id(), "uMVMatrix");
        uNormalMatrix = glGetUniformLocation(m_Program.id(), "uNormalMatrix");
        uTexture      = glGetUniformLocation(m_Program.id(), "uTexture");
    }
};

int main()
{
    auto ctx = p6::Context{{window_width, window_height, "TP5 EX3"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    
    //declaration des deux structures a la place du load shaders
    EarthProgram earthProgram{};
    MoonProgram moonProgram{};

    //load texture
    img::Image img_terre = p6::load_image_buffer("assets/textures/EarthMap.jpg");
    img::Image img_lune = p6::load_image_buffer("assets/textures/MoonMap.jpg");
    img::Image img_nuage = p6::load_image_buffer("assets/textures/CloudMap.jpg");

    
    // //recup variable uniforme
    // GLint uMVPMatrix    = glGetUniformLocation(Shader.id(), "uMVPMatrix");
    // GLint uMVMatrix     = glGetUniformLocation(Shader.id(), "uMVMatrix");
    // GLint uNormalMatrix = glGetUniformLocation(Shader.id(), "uNormalMatrix");


    // // Récupère la location des textures dans le shader
    // GLint uText1 = glGetUniformLocation(Shader.id(), "uText1");
    // // Récupère la location de la deuxième texture dans le shader
    // GLint uText2 = glGetUniformLocation(Shader.id(), "uText2");


    //creation du vbo
    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    //creation de la shape
    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16);

    // fill those coords in the vbo / Static is for constant variables
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);

    //oprion pour voir les tests en profondeur?
    glEnable(GL_DEPTH_TEST);

    //initialisation de textures -> multitexturing
    GLuint textureTerre;
    glGenTextures(1, &textureTerre);
    glActiveTexture(GL_TEXTURE0); // la texture textureTerre est bindée sur l'unité GL_TEXTURE0
    glBindTexture(GL_TEXTURE_2D, textureTerre);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_terre.width(), img_terre.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_terre.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    //debind text
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, 0);
    
    GLuint textureMoon;
    glGenTextures(1, &textureMoon);
    glActiveTexture(GL_TEXTURE1); // la texture textureCloud est bindée sur l'unité GL_TEXTURE1
    glBindTexture(GL_TEXTURE_2D, textureMoon);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_lune.width(), img_lune.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_lune.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, 0);

    GLuint textureCloud;
    glGenTextures(1, &textureCloud);
    glBindTexture(GL_TEXTURE_2D, textureCloud);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_nuage.width(), img_nuage.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, img_nuage.data());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);


    // creation du vao
    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    const GLuint VERTEX_ATTR_POSITION = 0;
    const GLuint VERTEX_ATTR_NORM     = 1;
    const GLuint VERTEX_ATTR_UV       = 2;
    glEnableVertexAttribArray(VERTEX_ATTR_POSITION);
    glEnableVertexAttribArray(VERTEX_ATTR_NORM);
    glEnableVertexAttribArray(VERTEX_ATTR_UV);

    glVertexAttribPointer(VERTEX_ATTR_POSITION, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, position)));
    glVertexAttribPointer(VERTEX_ATTR_NORM, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, normal)));
    glVertexAttribPointer(VERTEX_ATTR_UV, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)(offsetof(glimac::ShapeVertex, texCoords)));

    // debind du vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // debind du vao
    glBindVertexArray(0);

    //calcul matrice de projection pour le shader
    glm::mat4 ProjMatrix;
    ProjMatrix = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);


    //axe de rotation aléatoire lune
    std::vector<glm::vec3> angleRotation;
    std::vector<glm::vec3> axeTranslation;

    for(int i = 0; i<32; i++){
        angleRotation.push_back(glm::sphericalRand(2.f));
        axeTranslation.push_back(glm::sphericalRand(2.f));
    }

    //creation cam & initialisation des mouvements
    TrackballCamera camera(5, 0, 0);
    bool right = false;
    bool left = false;
    bool up = false;
    bool down = false;

    
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/
        //CAMERA
        //methode 1
        // //pas opti mais fonctionne
        // ctx.key_repeated = [&](p6::Key key){
        // if(key.physical == GLFW_KEY_RIGHT){
        //     camera.rotateLeft(+0.1);

        // } else if(key.physical == GLFW_KEY_LEFT){
        //     camera.rotateLeft(-0.1);

        // }else if(key.physical == GLFW_KEY_UP){
        //     camera.rotateUp(+0.1);
        // }else {
        //     camera.rotateUp(-0.1);
        // }
        // };

        // ctx.mouse_scrolled = [&](p6::MouseScroll scroll){
        //     // std::cout << "dx : " << scroll.dx << " et dy : " << scroll.dy << std::endl;
        //     //dy = -1 recul 
        //     //dy = 1 avance
        //     camera.moveFront(-scroll.dy);

        // };

        // glm::mat4 viewMatrix = camera.getViewMatrix();

        //methode 2
        if(right){
            camera.rotateLeft(-0.1f);
        }
        if(left){
            camera.rotateLeft(0.1f);
        }
        if(up){
            camera.rotateUp(0.1f);
        }
        if(down){
            camera.rotateUp(-0.1f);
        }

        ctx.key_pressed = [&right, &up, &left, &down](p6::Key key){
            if(key.physical == GLFW_KEY_RIGHT){
                right = true;
            }
            if(key.physical == GLFW_KEY_LEFT){
                left = true;
            }
            if(key.physical == GLFW_KEY_UP){
                up = true;
            }
            if(key.physical == GLFW_KEY_DOWN){
                down = true;
            }
        };

        ctx.key_released = [&right, &up, &left, &down](p6::Key key){
            if(key.physical == GLFW_KEY_RIGHT){
                right = false;
            }
            if(key.physical == GLFW_KEY_LEFT){
                left = false;
            }
            if(key.physical == GLFW_KEY_UP){
                up = false;
            }
            if(key.physical == GLFW_KEY_DOWN){
                down = false;
            }
        };

        ctx.mouse_dragged = [&camera](const p6::MouseDrag& button) {
            camera.rotateLeft(button.delta.x * 5);
            camera.rotateUp(-button.delta.y * 5);
        };

        ctx.mouse_scrolled = [&](p6::MouseScroll scroll){
            // std::cout << "dx : " << scroll.dx << " et dy : " << scroll.dy << std::endl;
            //dy = -1 recul 
            //dy = 1 avance
            camera.moveFront(-scroll.dy);
        };

        glm::mat4 viewMatrix = camera.getViewMatrix();






        // Shader.use();
        earthProgram.m_Program.use();

        glClearColor(0.2f, 0.2f, 0.2f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //bind vao
        glBindVertexArray(vao);


        //TERRE
        // //bind texture terre + nuage
        // glActiveTexture(GL_TEXTURE0);
        // glBindTexture(GL_TEXTURE_2D, textureTerre);
        // glUniform1i(uText1,0);
        glUniform1i(earthProgram.uEarthTexture, 0);

        // glActiveTexture(GL_TEXTURE1);
        // glBindTexture(GL_TEXTURE_2D, textureCloud);       
        // glUniform1i(uText2, 1);
        glUniform1i(earthProgram.uCloudTexture, 1);


        // MVMatrix = glm::translate(glm::mat4(1.0),glm::vec3(0., 0., -5.));
        // MVMatrix = glm::rotate(MVMatrix, -ctx.time(), glm::vec3(0, 1, 0));
        // NormalMatrix = glm::transpose(glm::inverse(MVMatrix));
        // //application de met matrice dans les shaders
        // glUniformMatrix4fv(uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        // glUniformMatrix4fv(uMVMatrix, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        // glUniformMatrix4fv(uNormalMatrix, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        const glm::mat4 globalMVMatrix = glm::translate(glm::mat4{1.f}, {0.f, 0.f, -5.f});
        glm::mat4 earthMVMatrix = glm::rotate(globalMVMatrix, ctx.time(), {0.f, 1.f, 0.f});
        earthMVMatrix = viewMatrix*earthMVMatrix; //variation de la MV (simuler une cam)
        // const glm::mat4 earthMVMatrix = glm::rotate(viewMatrix, ctx.time(), {0.f, 1.f, 0.f});
        //app matrice dans le shader
        glUniformMatrix4fv(earthProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(earthMVMatrix));
        glUniformMatrix4fv(earthProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(earthMVMatrix))));
        glUniformMatrix4fv(earthProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * earthMVMatrix));

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureTerre);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textureCloud);

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());        

        //LUNE
        moonProgram.m_Program.use();
        glUniform1i(moonProgram.uTexture, 0);
        glActiveTexture(GL_TEXTURE0);
        

        for(int i = 0; i<32; i++){
            glm::mat4 moonMVMatrix = glm::rotate(globalMVMatrix, ctx.time(), angleRotation[i]);
            moonMVMatrix = glm::rotate(moonMVMatrix, ctx.time(), angleRotation[i]); // Translation * Rotation
            moonMVMatrix = glm::translate(moonMVMatrix, axeTranslation[i]); // Translation * Rotation * Translation
            moonMVMatrix = glm::scale(moonMVMatrix, glm::vec3{0.2f}); // Translation * Rotation * Translation * Scale
            //app matrice dans le shader
            moonMVMatrix = viewMatrix*moonMVMatrix; //variation de la MV (simuler une cam)
            glUniformMatrix4fv(moonProgram.uMVMatrix, 1, GL_FALSE, glm::value_ptr(moonMVMatrix));
            glUniformMatrix4fv(moonProgram.uNormalMatrix, 1, GL_FALSE, glm::value_ptr(glm::transpose(glm::inverse(moonMVMatrix))));
            glUniformMatrix4fv(moonProgram.uMVPMatrix, 1, GL_FALSE, glm::value_ptr(ProjMatrix * moonMVMatrix));

            glBindTexture(GL_TEXTURE_2D, textureMoon);

            glDrawArrays(GL_TRIANGLES, 0, vertices.size());
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);

        //debind vao
        glBindVertexArray(0);

    };

    

    ctx.start();

    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteTextures(1, &textureTerre);
    glDeleteTextures(1, &textureMoon);
    glDeleteTextures(1, &textureCloud);

    return 0;
}