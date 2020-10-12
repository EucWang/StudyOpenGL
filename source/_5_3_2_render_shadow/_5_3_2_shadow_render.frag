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

//这里写死光线颜色
vec3 lightColor = vec3(1.0);
float shininess  = 64.0;

float calcShadow(vec4 fragPosLightSpace) {

	//执行透视除法
	//当我们在顶点着色器输出一个裁切空间顶点位置到gl_Position时，
	//OpenGL自动进行一个透视除法，将裁切空间坐标的范围-w到w转为-1到1，这要将x、y、z元素除以向量的w元素来实现。
	//由于裁切空间的fragPosLightSpace并不会通过gl_Position传到片段着色器里，我们必须自己做透视除法：
	//当使用正交投影矩阵，顶点w元素仍保持不变，这一步实际上毫无意义。
	//可是，当使用透视投影的时候就是必须的了，所以为了保证在两种投影矩阵下都有效就得留着这行。
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// 变换到[0,1]的范围
	//深度贴图的深度在0到1的范围，我们也打算使用projCoords从深度贴图中去采样，所以我们将NDC坐标变换为0到1的范围。 
	//整个projCoords向量都需要变换到[0,1]范围。
	projCoords = projCoords * 0.5 + 0.5;

	// 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
	//有了这些投影坐标，我们就能从深度贴图中采样得到0到1的结果，
	//从第一个渲染阶段的projCoords坐标直接对应于变换过的NDC坐标。
	//我们将得到光的位置视野下最近的深度：
	float closestDepth = texture(shadow_map, projCoords.xy).r;

	// 取得当前片段在光源视角下的深度
	float curDepth = projCoords.z;
	  
	// 检查当前片段是否在阴影中
	return curDepth > closestDepth ? 1.0 : 0.0;
}

//应用阴影偏移解决阴影失真的问题
float calcShadow2(vec3 normal,  vec3 lightDir, vec4 fragPosLightSpace) {
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;//执行透视除法
	projCoords = projCoords * 0.5 + 0.5;// 变换到[0,1]的范围
	float closestDepth = texture(shadow_map, projCoords.xy).r;// 取得最近点的深度(使用[0,1]范围下的fragPosLight当坐标)
	float curDepth = projCoords.z;// 取得当前片段在光源视角下的深度
	
	//有一个偏移量的最大值0.05，和一个最小值0.005，它们是基于表面法线和光照方向的。
	//这样像地板这样的表面几乎与光源垂直，得到的偏移就很小，而比如立方体的侧面这种表面得到的偏移就更大。
	float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
	float shadow = curDepth - bias > closestDepth ? 1.0 : 0.0;
	
	return shadow;// 检查当前片段是否在阴影中
}


void main(){

	vec3 tex1 = texture(diffuse_texture, vs_in.texCoords).rgb;  //贴图纹理
	vec3 norm = normalize(vs_in.fragNorm);                      //法线向量
	vec3 lightDir = normalize(lightPos - vs_in.fragPos);        //光线向量
	vec3 viewDir = normalize(viewPos - vs_in.fragPos);          //视角向量
	vec3 halfwayDir = normalize(lightDir + viewDir);            //视角和光线的中间向量

	vec3 ambient = 0.15 * tex1;    //环境光

	float diff = max(dot(lightDir, norm), 0.0);
	vec3 diffuse = diff * lightColor;              //漫反射分量
	
	float spec = pow(max(dot(norm, halfwayDir), 0.0), shininess);
	vec3 specular = spec * lightColor;          //镜面光分量

	//float shadow = calcShadow(vs_in.fragPosLightSpace);
	float shadow = calcShadow2(norm, lightDir, vs_in.fragPosLightSpace);

	//把diffuse和specular乘以(1-阴影元素)，这表示这个片段有多大成分不在阴影中。
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * tex1;


	fragColor = vec4(lighting, 1.0);
}