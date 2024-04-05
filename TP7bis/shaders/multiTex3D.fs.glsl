#version 330

//variables d'entrées
in vec3 vPosition_vs;
in vec3 vNormal_vs;
in vec2 vTexCoords;

vec2 fCorrectCoords;

out vec4 fFragColor;

uniform sampler2D uEarthTexture;
uniform sampler2D uCloudTexture;

void main() {

    fCorrectCoords = vec2(vTexCoords.x, 1-vTexCoords.y); //pour le template p6

	fFragColor = texture(uEarthTexture, fCorrectCoords) + texture(uCloudTexture, fCorrectCoords);
	
}