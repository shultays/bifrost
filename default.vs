attribute vec3 aVertexPosition;
attribute vec3 aVertexNormal;


uniform mat4 uWorldMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;
uniform vec4 uColor;

varying vec4 vColor;

void main(void) {

    vec4 worldPos  = uViewMatrix *  uWorldMatrix* vec4(aVertexPosition, 1.0);
    gl_Position = uProjectionMatrix * worldPos;
    

    vec3 transformedNormal = (uWorldMatrix * vec4(aVertexNormal, 0.0f)).xyz;
	transformedNormal = normalize(transformedNormal);
    vec3 uLightingDirection = vec3(0.0, 0.0, 1.0);

    float directionalLightWeighting = max(dot(transformedNormal, uLightingDirection), 0.0);

    
    vColor = vec4((uColor.rgb*0.2 + uColor.rgb*directionalLightWeighting * 0.8), uColor.a);
}
