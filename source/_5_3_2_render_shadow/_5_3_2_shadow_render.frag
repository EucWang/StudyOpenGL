#version 330 core

out vec4 fragColor;

in VS_OUT {
	vec3 fragPos;
	vec3 fragNorm;
	vec2 texCoords;
	vec4 fragPosLightSpace;
}vs_in;

uniform sampler2D diffuse_texture;
uniform sampler2D shadow_map;

uniform vec3 lightPos;
uniform vec3 viewPos;

//����д��������ɫ
vec3 lightColor = vec3(1.0);
float shininess  = 64.0;

float calcShadow(vec4 fragPosLightSpace) {

	//ִ��͸�ӳ���
	//�������ڶ�����ɫ�����һ�����пռ䶥��λ�õ�gl_Positionʱ��
	//OpenGL�Զ�����һ��͸�ӳ����������пռ�����ķ�Χ-w��wתΪ-1��1����Ҫ��x��y��zԪ�س���������wԪ����ʵ�֡�
	//���ڲ��пռ��fragPosLightSpace������ͨ��gl_Position����Ƭ����ɫ������Ǳ����Լ���͸�ӳ�����
	//��ʹ������ͶӰ���󣬶���wԪ���Ա��ֲ��䣬��һ��ʵ���Ϻ������塣
	//���ǣ���ʹ��͸��ͶӰ��ʱ����Ǳ�����ˣ�����Ϊ�˱�֤������ͶӰ�����¶���Ч�͵��������С�
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// �任��[0,1]�ķ�Χ
	//�����ͼ�������0��1�ķ�Χ������Ҳ����ʹ��projCoords�������ͼ��ȥ�������������ǽ�NDC����任Ϊ0��1�ķ�Χ�� 
	//����projCoords��������Ҫ�任��[0,1]��Χ��
	projCoords = projCoords * 0.5 + 0.5;

	// ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
	//������ЩͶӰ���꣬���Ǿ��ܴ������ͼ�в����õ�0��1�Ľ����
	//�ӵ�һ����Ⱦ�׶ε�projCoords����ֱ�Ӷ�Ӧ�ڱ任����NDC���ꡣ
	//���ǽ��õ����λ����Ұ���������ȣ�
	float closestDepth = texture(shadow_map, projCoords.xy).r;

	// ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
	float curDepth = projCoords.z;
	  
	// ��鵱ǰƬ���Ƿ�����Ӱ��
	return curDepth > closestDepth ? 1.0 : 0.0;
}

//Ӧ����Ӱƫ�ƽ����Ӱʧ�������
float calcShadow2(vec3 normal,  vec3 lightDir, vec4 fragPosLightSpace) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;//ִ��͸�ӳ���
	projCoords = projCoords * 0.5 + 0.5;// �任��[0,1]�ķ�Χ
	float closestDepth = texture(shadow_map, projCoords.xy).r;// ȡ�����������(ʹ��[0,1]��Χ�µ�fragPosLight������)
	float curDepth = projCoords.z;// ȡ�õ�ǰƬ���ڹ�Դ�ӽ��µ����
	
	//��һ��ƫ���������ֵ0.05����һ����Сֵ0.005�������ǻ��ڱ��淨�ߺ͹��շ���ġ�
	//������ذ������ı��漸�����Դ��ֱ���õ���ƫ�ƾͺ�С��������������Ĳ������ֱ���õ���ƫ�ƾ͸���
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	
	//float shadow = curDepth - bias > closestDepth ? 1.0 : 0.0;
	//PCF�㷨(percentage-closer filtering), ����һ�ֶ����ͬ���˷�ʽ�����,���������Ӱ,�����Ӱ��ݺ�Ӳ��
	//����˼���Ǵ������ͼ�ж�β�����ÿһ�β������������궼���в�ͬ��
	float shadow = 0;
	//textureSize����һ�����������������0��mipmap��vec2���͵Ŀ�͸ߡ�
	//��1����������һ�������������صĴ�С���������Զ������������ƫ�ƣ�ȷ��ÿ�������������Բ�ͬ�����ֵ��
	vec2 texSize = 1.0 / textureSize(shadow_map, 0);
	for(int x = -1; x <= 1; ++x) {
		for(int y = -1; y <= 1; ++y) {  
			//�����õ�9��ֵ��������ͶӰ�����x��yֵ����Χ��Ϊ��Ӱ�赲���в��ԣ�������ͨ������������Ŀ�����ƽ������
			float pcfDepth = texture(shadow_map, projCoords.xy + vec2(x, y) * texSize).r;
			shadow += curDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0;

	// ֻҪͶӰ������z�������1.0�����ǾͰ�shadow��ֵǿ����Ϊ0.0
	// ��һ����ȹ��Զƽ�滹ҪԶʱ������ͶӰ�����z�������1.0��
	// GL_CLAMP_TO_BORDER���Ʒ�ʽ�������ã���Ϊ���ǰ������zԪ�غ������ͼ��ֵ�����˶Աȣ�������Ϊ����1.0��z����true��
	//����ζ�ţ�ֻ���������ͼ��Χ���ڵı�ͶӰ��fragment���������Ӱ�������κγ�����Χ�Ķ�����û����Ӱ��
	if(projCoords.z > 1.0) {
		shadow = 0.0;
	}

	return shadow;// ��鵱ǰƬ���Ƿ�����Ӱ��
}

void main(){

	vec3 tex1 = texture(diffuse_texture, vs_in.texCoords).rgb;  //��ͼ����
	vec3 norm = normalize(vs_in.fragNorm);                      //��������
	vec3 lightDir = normalize(lightPos - vs_in.fragPos);        //��������
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);          //�ӽ�����
	vec3 halfwayDir = normalize(lightDir + viewDir);            //�ӽǺ͹��ߵ��м�����

	vec3 ambient = 0.15 * tex1;    //������

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;              //���������
	
	float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
	vec3 specular = spec * lightColor;          //��������

	//float shadow = calcShadow(vs_in.fragPosLightSpace);
	float shadow = calcShadow2(norm, lightDir, vs_in.fragPosLightSpace);

	//��diffuse��specular����(1-��ӰԪ��)�����ʾ���Ƭ���ж��ɷֲ�����Ӱ�С�
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * tex1;


	fragColor = vec4(lighting, 1.0);
}