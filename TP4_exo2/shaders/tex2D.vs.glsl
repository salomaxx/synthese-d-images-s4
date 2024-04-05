#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 inTextureCoordinates;

out vec2 vTextureCoords;

uniform mat3 uModelMatrix; // Variable uniforme pour la matrice de modèle
uniform float uTime; // Déclaration de la variable uniforme pour le temps

void main() {
    vec3 transformedPosition = uModelMatrix * vec3(aVertexPosition, 1.0);
    gl_Position = vec4(transformedPosition.xy, 0.0, 1.0);
    vTextureCoords = inTextureCoordinates;
}
