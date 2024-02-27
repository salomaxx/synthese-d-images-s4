#include "glimac/default_shader.hpp"
#include "p6/p6.h"

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP3 EX1"}};
    ctx.maximize_window();

    // shader
    const p6::Shader shader = p6::load_shader(
        "shaders/color2D.vs.glsl",
        "shaders/color2D.fs.glsl"
    );

    // INITIALISATION

    // création de vbo (ici un seul) = tableau permettant de stocker données d'un élément, puis binding
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // binding

    // on créé un tableau avec les coordonnées du triangle
    GLfloat vertices[] = {
        -0.5f, -0.5f, 1.f, 1.f, 0.f,
        0.5f, -0.5f, 0.f, 1.f, 1.f,
        0.0f, 0.5f, 1.f, 0.f, 1.f
    };

    // puis on envoie les données
    glBufferData(GL_ARRAY_BUFFER, 15 * sizeof(GLfloat), vertices, GL_STATIC_DRAW);

    // on debind pour éviter de modifier le vbo par erreur, meme fonction qu'au début mais avec 0
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // création de vao = spécification de sommet qui va permettre de dessiner, puis binding
    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao); // binding, un seul parametre contrairement à vbo

    // on va choisir l'attribut a activé, défini par un index
    // ici l'attribut est la position, on defini l'index à 0
    static constexpr GLuint vertex_attr_position = 0;
    static constexpr GLuint vertex_attr_color    = 1;
    glEnableVertexAttribArray(vertex_attr_position);
    glEnableVertexAttribArray(vertex_attr_color);

    // on rebind le vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // il faut indiquer où il va trouver les sommets à dessiner
    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0);
    // il faut indiquer où il va trouver les sommets à dessiner
    glVertexAttribPointer(vertex_attr_color, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (const GLvoid*)(2 * sizeof(GLfloat)));

    // on debind encore le vbo
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    // et finalement on debind le vao
    glBindVertexArray(0);

    // Declare your infinite update loop.
    ctx.update = [&]() {
        // DESSIN

        // on nettoie la fenetre
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao); // bind du vao

        glimac::bind_default_shader();
        shader.use();
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0); // debind du vao
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return 0;
}