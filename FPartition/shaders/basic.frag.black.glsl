/* shader used for wireframes only */
void main()
{  
	vec3 color = vec3(0.5,0.5,0.5);
    gl_FragColor.rgb = color;
    gl_FragColor.a = 0.5;
}
