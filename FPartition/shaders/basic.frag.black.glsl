/* shader used for wireframes only */
void main()
{  
	vec3 color = vec3(0.7,0.7,0.7);
    gl_FragColor.rgb = color;
    gl_FragColor.a = 0.5;
}
