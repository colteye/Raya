#include "MeshObject.h"
#include "InstanceObject.h"
#include "Camera.h"
#include "PointLight.h"
#include "DirectLight.h"
#include "Scene.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtx/euler_angles.hpp" 

#include "LambertMaterial.h"
#include "BoxTreeObject.h"

#define PI 3.141592f

void project2() {

	//	Create scene 
	Scene scn;
	scn.SetSkyColor(Color(0.8f, 0.8f, 1.0f));

	//	Create ground
	MeshObject ground;
	ground.MakeBox(5.0f, 0.1f, 5.0f);
	scn.AddObject(ground);

	//	Create dragon
	MeshObject dragon;
	dragon.LoadPLY("Stanford_Dragon\\dragon_vrip_res2.ply", nullptr);
	dragon.Smooth();
	BoxTreeObject tree(10);
	tree.Construct(dragon);
	scn.AddObject(tree);

	//	Create instance 
	InstanceObject inst(tree);
	glm::mat4x4 mtx = glm::eulerAngleY(PI);
	mtx[3] = glm::vec4(-0.05f, 0.0f, -0.1f, 1.0f);
	inst.SetMatrix(mtx);
	scn.AddObject(inst);

	//	Create lights
	DirectLight sunlgt;
	sunlgt.SetBaseColor(Color(1.0f, 1.0f, 0.9f)); 
	sunlgt.SetIntensity(1.0f); 
	sunlgt.SetDirection(glm::vec3(2.0f, -3.0f, -2.0f)); 
	scn.AddLight(sunlgt);

	PointLight redlgt;
	redlgt.SetBaseColor(Color(1.0f, 0.2f, 0.2f));
	redlgt.SetIntensity(0.02f);
	redlgt.SetPosition(glm::vec3(-0.2f, 0.2f, 0.2f));
	scn.AddLight(redlgt);

	PointLight bluelgt;
	bluelgt.SetBaseColor(Color(0.2f, 0.2f, 1.0f));
	bluelgt.SetIntensity(0.02f);
	bluelgt.SetPosition(glm::vec3(0.1f, 0.1f, 0.3f));
	scn.AddLight(bluelgt);

	//	Create camera
	Camera cam(800, 400);
	cam.LookAt(glm::vec3(-0.1f, 0.1f, 0.2f), glm::vec3(-0.05f, 0.12f, 0.0f), glm::vec3(0, 1.0f, 0));
	cam.SetFOV(40.0f);

	//	Render image 
	cam.Render(scn); 
	cam.SaveBitmap("project2.bmp");
}