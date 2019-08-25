

varying float lightAmount;

void main()
{
    vec3 color = vec3(0.9,0.9,1.0);

    //if (!gl_FrontFacing) color*=0.5;

    gl_FragColor.rgb = color;// * lightAmount;
    gl_FragColor.a = 1.0;
}
