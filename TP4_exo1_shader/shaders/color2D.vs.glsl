#version 330 core

layout(location = 0) in vec2 aVertexPosition;
layout(location = 1) in vec3 aVertexColor;

out vec3 vFragColor;

mat3 rotate(float angle){

  angle = radians(angle);
  mat3 M = mat3(
    vec3(cos(angle), sin(angle), 0),
    vec3(-sin(angle), cos(angle), 0),
    vec3(0, 0, 1)
  );
  return M;
}

mat3 scale(float size){
  mat3 M = mat3(
    vec3(size, 0, 0),
    vec3(0, size, 0),
    vec3(0, 0, 1)
  );
  return M;
}

void main() {
  vFragColor = aVertexColor;
  gl_Position = vec4(aVertexPosition, 0., 1.);
  //gl_Position = vec4(aVertexPosition, 0., 1.)+vec4(0.5f, 0.5f, 0.f, 0.f); // translation de (0.5;0.5)
  //gl_Position = vec4(aVertexPosition*1.5f, 0., 1.); // triangle x1.5
  //gl_Position = vec4(aVertexPosition, 0., 1.)*vec4(2.f, 0.5f, 0.f, 1.f); // deformation 

  float attenuation = 1 * exp(-20*length(vVertexPosition)*length(vVertexPosition));

  gl_Position = vec4((rotate(90)*scale(1.5f) * vec3(aVertexPosition, 1.)).xy, 0., 1.)*attenuation;
  
}