#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec2 aTextureCoords; // Nouvelle entrée pour les coordonnées de texture

out vec2 vTextureCoords; // Sortie pour les coordonnées de texture

uniform float uTime;

// Fonction qui renvoie la matrice de rotation d'angle a
mat3 rotate(float a) {
  a = radians(a);
  mat3 M = mat3(
    vec3(cos(a), sin(a), 0), // Première colonne de la matrice
    vec3(-sin(a), cos(a), 0), // Deuxième colonne de la matrice
    vec3(0, 0, 1) // Troisième colone de la matrice
  );
}

void main() {
    vTextureCoords = aTextureCoords; // Passer les coordonnées de texture au fragment shader
    gl_Position = vec4(aVertexPosition*rotate(uTime), 0.0, 1.0); // Position du sommet
}