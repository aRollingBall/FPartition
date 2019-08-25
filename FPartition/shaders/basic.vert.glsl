
//uniform mat4 gl_ModelViewProjectionMatrix;
attribute vec3 pos;
attribute vec3 norm;
attribute vec3 c_color;
//varying vec3 cc_color;
varying float lightAmount;
varying vec3 cc_color;

void main()
{		
	cc_color=c_color;
    // a very crude lighting...
    //lightAmount = 0.80 + 0.20*abs(normalize(norm).y);

    // projection
	//symcircle_o_pl.vtk
    //gl_Position = gl_ModelViewProjectionMatrix * vec4( pos[1]-3.9,pos[0]+4.0,pos[2] , 1.0);
	//two_circle_o_pl_vector.vtk
	gl_Position = gl_ModelViewProjectionMatrix * vec4( pos[0],pos[1]+1.0,pos[2] , 1.0);
}