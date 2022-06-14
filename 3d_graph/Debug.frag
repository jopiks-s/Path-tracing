uniform bool load;
uniform int load_count;

float loop()
{
	float a=1;
	for(int i=0;i<7500000;i++)
		for(int k=0;k<12;k++)
			a += int(i%2 == 1)*2 -1;
		
	return a;
}


void main()
{
	if(load)
	{
		float b = loop();
		gl_FragColor = vec4(b,0.1,0.3,1);
	}
	else
		gl_FragColor = vec4(0.3,0.6,sin(load_count/2.0),1);
}