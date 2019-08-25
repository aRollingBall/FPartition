/* shader used for wireframes only */

//varying float lightAmount;
varying vec3 cc_color;
void main()
{  
    vec3 color = vec3(0.0,1.0,0.0); // on light background
	//vec3 color=cc_color;	
    gl_FragColor.rgb = color;
    gl_FragColor.a = 1.0;//  * lightAmount ;
}
