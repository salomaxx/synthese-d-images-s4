// #version 330 core

// layout(location = 0) in vec2 aVertexPosition;
// layout(location = 1) in vec3 aVertexColor;

// out vec3 vFragPosition;
// out vec3 vFragColor;
// out vec2 vVertexPosition; // Ajout de la sortie pour la position du vertex

// mat3 rotate(float angle){

//   angle = radians(angle);
//   mat3 M = mat3(
//     vec3(cos(angle), sin(angle), 0),
//     vec3(-sin(angle), cos(angle), 0),
//     vec3(0, 0, 1)
//   );
//   return M;
// }

// mat3 scale(float size){
//   mat3 M = mat3(
//     vec3(size, 0, 0),
//     vec3(0, size, 0),
//     vec3(0, 0, 1)
//   );
//   return M;
// }

// float halo(float a, float b, float distance) {
//   return a * exp(-b * pow(distance, 2));
// }

// void main() {
//   // vFragColor = aVertexColor;
//   // gl_Position = vec4(aVertexPosition, 0., 1.);
  
//   //gl_Position = vec4(aVertexPosition, 0., 1.)+vec4(0.5f, 0.5f, 0.f, 0.f); // translation de (0.5;0.5)
//   //gl_Position = vec4(aVertexPosition*1.5f, 0., 1.); // triangle x1.5
//   //gl_Position = vec4(aVertexPosition, 0., 1.)*vec4(2.f, 0.5f, 0.f, 1.f); // deformation 

//   vFragPosition = vec3(aVertexPosition, 0.0); // Si nécessaire pour d'autres calculs
//   vVertexPosition = aVertexPosition;
//   vFragColor = aVertexColor;

//   gl_Position = vec4(((rotate(45)*translate(.5, .3)*scale(1.2, .1)) * vec3(aVertexPosition, 1.)).xy, 0.0, 1.0); // Assurez-vous que gl_Position est correctement défini
  
// }

#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragPosition;
out vec3 vFragColor;
out vec2 vVertexPosition; // Ajout de la sortie pour la position du vertex

// Fonction qui renvoie la matrice de translation tx et ty
mat3 translate(float tx, float ty) {
  mat3 M = mat3(
    vec3(1, 0, 0), // Première colonne de la matrice
    vec3(0, 1, 0), // Deuxième colonne de la matrice
    vec3(tx, ty, 1) // Troisième colone de la matrice
  );
  return M;
}
// Fonction qui renvoie la matrice de scale sx et sy
mat3 scale(float sx, float sy) {
  mat3 M = mat3(
    vec3(sx, 0, 0), // Première colonne de la matrice
    vec3(0, sy, 0), // Deuxième colonne de la matrice
    vec3(0, 0, 1) // Troisième colone de la matrice
  );
  return M;
}

// Fonction qui renvoie la matrice de rotation d'angle a
mat3 rotate(float a) {
  a = radians(a);
  mat3 M = mat3(
    vec3(cos(a), sin(a), 0), // Première colonne de la matrice
    vec3(-sin(a), cos(a), 0), // Deuxième colonne de la matrice
    vec3(0, 0, 1) // Troisième colone de la matrice
  );
  return M;
}

void main() {
  vFragPosition = vec3(aVertexPosition, 0.0); // Si nécessaire pour d'autres calculs
  vVertexPosition = aVertexPosition;
  vFragColor = aVertexColor;

  gl_Position = vec4(((rotate(0)*translate(.0, .0)*scale(1.5, 1.5)) * vec3(aVertexPosition, 1.)).xy, 0.0, 1.0); // Assurez-vous que gl_Position est correctement défini
}