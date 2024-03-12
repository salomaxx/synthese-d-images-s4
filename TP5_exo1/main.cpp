#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "glimac/common.hpp"
#include "glimac/sphere_vertices.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    /*********************************
     * HERE SHOULD COME THE INITIALIZATION CODE
     *********************************/
    const p6::Shader shader = p6::load_shader(
        "shaders/3D.vs.glsl",
        "shaders/normal.fs.glsl"
    );

    const std::vector<glimac::ShapeVertex> vertices = glimac::sphere_vertices(1.f, 32, 16); // création des vertices de la sphere

    // Création d'un VBO :
    GLuint vbo;
    glGenBuffers(1, &vbo);
    // À partir de ce point, la variable vbo contient l'identifiant d'un VBO
    // Binding d'un VBO sur la cible GL_ARRAY_BUFFER:
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    // On peut à présent modifier le VBO en passant par la cible GL_ARRAY_BUFFER

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glimac::ShapeVertex), vertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0); // debind du vbo

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    static constexpr GLuint vertex_attr_position = 0;
    glEnableVertexAttribArray(vertex_attr_position);
    static constexpr GLuint vertex_attr_normal = 1;
    glEnableVertexAttribArray(vertex_attr_normal);
    static constexpr GLuint vertex_attr_color = 2;
    glEnableVertexAttribArray(vertex_attr_color);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(vertex_attr_position, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, position)); // attributs position : 3 coordonnées
    glVertexAttribPointer(vertex_attr_normal, 3, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, normal));
    glVertexAttribPointer(vertex_attr_color, 2, GL_FLOAT, GL_FALSE, sizeof(glimac::ShapeVertex), (const GLvoid*)offsetof(glimac::ShapeVertex, texCoords));

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        /*********************************
         * HERE SHOULD COME THE RENDERING CODE
         *********************************/

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        shader.use();
        glEnable(GL_DEPTH_TEST);
        // recuperation des matrices du shader
        GLint uMVPMatrixLocation    = glGetUniformLocation(shader.id(), "uMVPMatrix");
        GLint uMVMatrixLocation     = glGetUniformLocation(shader.id(), "uMVMatrix");
        GLint uNormalMatrixLocation = glGetUniformLocation(shader.id(), "uNormalMatrix");

        std::cerr << "uMVPMatrix Location: " << uMVPMatrixLocation << std::endl;
        std::cerr << "uMVMatrix Location: " << uMVMatrixLocation << std::endl;
        std::cerr << "uNormalMatrix Location: " << uNormalMatrixLocation << std::endl;

        glm::mat4 ProjMatrix   = glm::perspective(glm::radians(70.f), ctx.aspect_ratio(), 0.1f, 100.f);
        glm::mat4 MVMatrix     = glm::translate(glm::mat4{1.f}, glm::vec3(0.f, 0.f, -5.f));
        glm::mat4 NormalMatrix = glm::transpose(glm::inverse(MVMatrix));

        // envoi des matrices vers le GPU
        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, vertices.size());

        // MVMatrix = glm::rotate(MVMatrix, ctx.time(), {0.f, 1.f, 0.f}); // Translation * Rotation
        MVMatrix = glm::translate(glm::mat4{1.f}, glm::vec3(0.f, 0.f, -5.f));
        MVMatrix = glm::translate(MVMatrix, {-2.f, 0.2f, 1.f}); // Translation * Rotation * Translation
        MVMatrix = glm::scale(MVMatrix, glm::vec3{0.2f});       // Translation * Rotation * Translation * Scale
        glUniformMatrix4fv(uMVPMatrixLocation, 1, GL_FALSE, glm::value_ptr(ProjMatrix * MVMatrix));
        glUniformMatrix4fv(uMVMatrixLocation, 1, GL_FALSE, glm::value_ptr(MVMatrix));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(NormalMatrix));

        glDrawArrays(GL_TRIANGLES, 0, vertices.size());
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
}