#include "glimac/default_shader.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "p6/p6.h"

struct Vertex2DUV {
    glm::vec2 position;
    glm::vec2 textureCoordinates;
};

glm::mat3 translate(float tx, float ty)
{
    return glm::mat3(
        glm::vec3(1, 0, 0),
        glm::vec3(0, 1, 0),
        glm::vec3(tx, ty, 1)
    );
}

glm::mat3 scale(float sx, float sy)
{
    return glm::mat3(
        glm::vec3(sx, 0, 0),
        glm::vec3(0, sy, 0),
        glm::vec3(0, 0, 1)
    );
}

glm::mat3 rotate(float a)
{
    a = glm::radians(a);
    return glm::mat3(
        glm::vec3(cos(a), sin(a), 0),
        glm::vec3(-sin(a), cos(a), 0),
        glm::vec3(0, 0, 1)
    );
}

int main()
{
    auto ctx = p6::Context{{1280, 720, "TP4 EX2"}};
    ctx.maximize_window();

    // shader
    const p6::Shader shader = p6::load_shader(
        "shaders/tex2D.vs.glsl",
        "shaders/tex2D.fs.glsl"
    );

    // INITIALISATION

    // création de vbo (ici un seul) = tableau permettant de stocker données d'un élément, puis binding
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo); // binding

    // on créé un tableau avec les coordonnées du triangle
    Vertex2DUV vertices[] = {
        {{-1.0f, -1.0f}, {0.f, 0.f}},
        {{1.0f, -1.0f}, {0.f, 0.f}},
        {{0.0f, 1.0f}, {0.f, 0.f}}
    };

    // puis on envoie les données
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

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
    glVertexAttribPointer(vertex_attr_position, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (void*)offsetof(Vertex2DUV, position));
    // il faut indiquer où il va trouver les sommets à dessiner
    glVertexAttribPointer(vertex_attr_color, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex2DUV), (void*)offsetof(Vertex2DUV, textureCoordinates));

    // on debind encore le vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // et finalement on debind le vao
    glBindVertexArray(0);

    GLint uTimeLocation = glGetUniformLocation(shader.id(), "uTime");

    if (uTimeLocation == -1)
    {
        std::cerr << "Warning: Uniform uTime not found in shader!" << '\n';
    }

    float uTime = 0.0f;

    // Declare your infinite update loop.
    ctx.update = [&]() {
        
        float     angle          = uTime;
        glm::mat3 rotationMatrix = rotate(angle);

        // DESSIN

        // on nettoie la fenetre
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(vao); // bind du vao

        glimac::bind_default_shader();
        shader.use();
        glUniformMatrix3fv(uTimeLocation, 1, GL_FALSE, glm::value_ptr(rotationMatrix));
        //glUniform1f(uTimeLocation, uTime);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        glBindVertexArray(0); // debind du vao
    };

    // Should be done last. It starts the infinite loop.
    ctx.start();
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    return 0;
}