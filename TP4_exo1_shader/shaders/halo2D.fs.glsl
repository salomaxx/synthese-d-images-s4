#version 330 core

in vec2 vVertexPosition; // Déclarez vVertexPosition comme une entrée de type vec2
in vec3 vFragColor;

out vec4 fFragColor;

float halo(float a, float b, float distance) {
  return a * exp(-b * pow(distance, 2));
}

void main() {
  // Calcul de la distance du fragment à la position du vertex
  float distance = distance(vec2(0,0), vVertexPosition);

  // Paramètres d'atténuation
  float alpha = halo(1.0, 15.0, distance);

  // Application de l'atténuation à la couleur finale
  fFragColor = vec4(vFragColor.x * alpha, vFragColor.y * alpha, vFragColor.z * alpha, 1.0);

  // fFragColor = fFragColor * length(fract(5.0 * vFragColor));
  //fFragColor = fFragColor * length(abs(fract(5.0 * vFragColor) * 2.0 - 1.0));
  //fFragColor = fFragColor * mod(floor(10.0 * vFragColor.x) + floor(10.0 * vFragColor.y), 2.0);
  //fFragColor = fFragColor * smoothstep(0.3, 0.32, length(fract(5.0 * vFragColor) - 0.5));
  //fFragColor = fFragColor * smoothstep(0.4, 0.5, max(abs(fract(8.0 * vFragColor.x - 0.5 * mod(floor(8.0 * vFragColor.y), 2.0)) - 0.5), abs(fract(8.0 * vFragColor.y) - 0.5)));
}