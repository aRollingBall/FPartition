
//uniform mat4 gl_ModelViewProjectionMatrix;
attribute vec3 pos;
//attribute vec3 norm;


varying float lightAmount;


void main()
{
    // a very crude lighting...
    lightAmount = 0.80;

    // projection   
	gl_Position = gl_ModelViewProjectionMatrix * vec4( pos[0],pos[1]+1.0,pos[2] , 1.0);
}

