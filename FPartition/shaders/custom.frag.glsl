uniform int flag;
void main()
{
	if(flag==1) 
	    gl_FragColor.rgb = vec3(1.0,0.0,0.0);
	else if(flag==0)
        gl_FragColor.rgb = vec3(0.0,0.0,1.0);
    gl_FragColor.a = 1.0; 
}