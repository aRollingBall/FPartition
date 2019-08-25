attribute vec3 pos;

void main()
{		
    // projection
	gl_Position = gl_ModelViewProjectionMatrix * vec4( pos[0],pos[1]+1.0,pos[2] , 1.0);
}