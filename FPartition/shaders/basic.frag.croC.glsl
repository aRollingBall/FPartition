/* shader used for wireframes only */
void main()
{  
	vec3 color = vec3(0.0,0.2,0.8);
    gl_FragColor.rgb = color;
    gl_FragColor.a = 0.6;
}
