layout(location = 0) attribute vec3 aVertexPosition;
attribute vec4 aVertexColor;


uniform mat4 uWorldMatrix;
uniform mat4 uViewMatrix;
uniform mat4 uProjectionMatrix;

varying vec4 vColor;
varying vec3 vPosition;

void main(void) {

    vec4 worldPos  =   uWorldMatrix* vec4(aVertexPosition, 1.0);
    gl_Position = uProjectionMatrix * uViewMatrix *worldPos;
    


    vColor = aVertexColor;

}
