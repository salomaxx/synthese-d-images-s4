#version 330 core

in vec2 vTextureCoords; // Entrée pour les coordonnées de texture

out vec4 FragColor; // Couleur du fragment

uniform float uTime; // Déclaration de la variable uniforme pour le temps

void main() {
    // Exemple d'utilisation de uTime pour modifier la couleur en fonction du temps
    float greenComponent = sin(uTime); // Utilisation de sin pour créer une variation
    FragColor = vec4(1.0, greenComponent, 0.22, 1.0); // Rouge (composantes RGBA)
}
