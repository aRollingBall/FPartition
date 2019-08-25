/* shader used for wireframes only */
void main()
{  
	vec3 color = vec3(0,0,0);
    gl_FragColor.rgb = color;
    gl_FragColor.a = 1.0;
}
