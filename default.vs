attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;
attribute vec2 aVertexUV;
attribute vec4 aVertexColor;
attribute vec4 aTextureWeights;


uniform mat4 uWorldMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform vec4 uColor;

uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;
uniform int uTextureCount;

varying vec4 vColor;
varying vec2 vVertexUV;
varying vec2 vTextureWeights;

void main(void) {

    vec4 worldPos  = uViewMatrix *  uWorldMatrix* vec4(aVertexPosition, 1.0);
    gl_Position = uProjectionMatrix * worldPos;
    

    vec3 transformedNormal = (uWorldMatrix * vec4(aVertexNormal, 0.0f)).xyz;
	transformedNormal = normalize(transformedNormal);
    vec3 uLightingDirection = vec3(0.0, 0.8, 0.6);

    float directionalLightWeighting = max(dot(transformedNormal, uLightingDirection), 0.0);

	vec4 finalColor = vec4(1.0);

    
	finalColor *= aVertexColor*uColor;

    vColor = vec4((finalColor.rgb*0.2 + finalColor.rgb*directionalLightWeighting * 0.8), finalColor.a);

	vVertexUV = aVertexUV;
	vTextureWeights = aTextureWeights;
}
