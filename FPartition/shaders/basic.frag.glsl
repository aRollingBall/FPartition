/* shader used for wireframes only */

//varying float lightAmount;
varying vec3 cc_color;
uniform int flag;
void main()
{

  //  vec3 color = vec3(0.25,0.25,0.25); // on dark background
  //vec3 color = vec3(0.0,0.0,0.0); // on light background
	vec3 color=cc_color;	
  // if(flag==1)  gl_FragColor.rgb=vec3(1.0,0.0,0.0);
  // else 
       gl_FragColor.rgb = color;
    gl_FragColor.a = 1.0;//  * lightAmount ;
}
