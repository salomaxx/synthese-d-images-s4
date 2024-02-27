#version 330 core

in vec2 vTextureCoords; // Entrée pour les coordonnées de texture

out vec4 FragColor; // Couleur du fragment

void main() {
    // Afficher en rouge
    FragColor = vec4(1.0, 0.0, 0.0, 1.0); // Rouge (composantes RGBA)
}