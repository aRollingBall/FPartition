/* shader used for wireframes only */
void main()
{  
	vec3 color = vec3(0.1,0.2,0.6);
    gl_FragColor.rgb = color;
    gl_FragColor.a = 1.0;
}
