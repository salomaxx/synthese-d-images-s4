#version 330 core

// Entrées du shader
in vec3 vPosition_vs; // Position du sommet transformée dans l'espace View (vs)
in vec3 vNormal_vs; // Normale du sommet transformée dans l'espace View (vs)
in vec2 vTexCoords; // Coordonnées de texture du sommet

// Sortie du shader (couleur finale)
out vec4 fragColor;

void main() {
    // Couleur de base
    vec3 baseColor = vec3(0.8, 0.8, 0.8); // Gris clair

    // Calcul de l'éclairage diffus
    vec3 lightDir = normalize(vec3(1.0, 1.0, 1.0)); // Direction de la source lumineuse
    float diffIntensity = max(dot(normalize(vNormal_vs), lightDir), 0.0);
    vec3 diffuse = diffIntensity * vec3(1.0, 1.0, 1.0); // Couleur de l'éclairage diffus

    // Combinaison de la couleur de base et de l'éclairage diffus
    vec3 finalColor = baseColor * diffuse;

    // Assignation de la couleur finale au fragment
    fragColor = vec4(finalColor, 1.0);
}