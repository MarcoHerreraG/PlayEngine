#include "Game.h"
#include<iostream>
#include <glm.hpp>
#include <time.h>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>


Game::Game()
{

}

Game::~Game()
{
}

void Game::Init()
{
	physics = Physics::GetPtr();
	physics->InitPhysics();
	std::cout << " Menu Init" << std::endl;
	this->platform = PEPlatform::GetPtr();
	this->manager = GameStateManager::GetPtr();
	resourceManager = ResourceManager::GetPtr();
	
	std::map<std::string, std::string> paths = {
	{"Enemy","Assets/Models/pina_pose.obj"},
	{"Wall1","Assets/Models/lWall.obj"},
	{"Wall2","Assets/Models/rWall.obj"},
	{"Player","Assets/Models/pina_pose.obj"},
	};
	//LoadModels(paths);

	auto resourceManger = ResourceManager::GetPtr();
	resourceManger->Add(ResourceType::Model3d, "Floor", "Assets/Models/floor2.obj");
	resourceManger->Add(ResourceType::Model3d, "Enemy", "Assets/Models/pina_pose.obj");
	resourceManger->Add(ResourceType::Texture, "TEXTURE",  "Assets/Textures/pina.png");
	resourceManger->Add(ResourceType::Texture, "TEXTURE", "Assets/Textures/brick.png");

	resourceManger->Wait();
	resourceManger->Load();
	
	player = new Player();
	floor = new Floor();

	shaderManager = ShaderManager::getPtr();
	shaderManager->initShader(player->GetCamera());
	shaderManager->LoadShaders("OneColor", "Assets/Shaders/OneColor.vert", "Assets/Shaders/OneColor.frag");
	shaderManager->LoadShaders("gouraud-shader", "Assets/Shaders/gouraud-shader.vert", "Assets/Shaders/gouraud-shader.frag");
	shaderManager->LoadShaders("phong-shader", "Assets/Shaders/phong-shader.vert", "Assets/Shaders/phong-shader.frag");
	shaderManager->LoadShaders("toon-shader", "Assets/Shaders/toon-shader.vert", "Assets/Shaders/toon-shader.frag");

	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Assets/Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Assets/Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Assets/Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Assets/Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Assets/Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Assets/Textures/Skybox/cupertin-lake_ft.tga");
	skybox = new Skybox(skyboxFaces);


	std::vector<std::string> pathsEnemies = {
	"Enemy1",
	"Enemy2",
	};
	LoadEnemies(pathsEnemies);
}

void Game::LoadEnemies(const std::vector<std::string>& pathFileModels)
{
	for (int i=0;i<200;i++)
	{
		Enemy* enemy = new Enemy(player);
		enemies.push_back(enemy);
	}
}

void Game::LoadModels(const std::map<std::string, std::string> &models)
{
	for (auto model : models)
	{
		auto resourceManger = ResourceManager::GetPtr();
		resourceManger->Add(ResourceType::Model3d, model.first, model.second);
		auto asset = (Model*)ResourceManager::GetPtr()->GetElement(model.first, model.second);
		asset->AddTexture("Assets/Textures/brick.png");
		map.push_back(asset);
	}
}

void Game::LoadShaders()
{
	
}

void Game::Draw()
{
	platform->RenderClear();
	skybox->Draw(shaderManager->GetViewMatrix(), shaderManager->GetProjectionMatrix());

	shaderManager->Activate("phong-shader");
	shaderManager->draw();
	
	DrawMap();
	DrawEnemies();
	floor->Draw();
	platform->RenderPresent();
}
void Game::DrawMap()
{
	for (auto model : map)
	{
		Transform transform;
		transform.SetTranslation(0.0f, 0.0f, 0.0f);
		transform.SetScale(1.0f, 1.0f, 1.0f);
		transform.SetRotation(0, 0, 0);
		model->SetTransform(transform);
		model->Draw();
	}
}

void Game::DrawEnemies()
{
	for (auto enemi : enemies)
	{
		enemi->Draw();
	}
}

void Game::Update(unsigned int delta)
{
	floor->Update(delta);
	for (auto enemi : enemies)
	{
		enemi->Update(delta);
	}
	physics->Update(delta);
}

bool Game::MouseInput(int x, int y, bool leftbutton)
{
	if (x != -1 || y != -1)
		player->GetCamera()->mouseControl(x, y);
	return false;
}

bool Game::Input(std::map<int, bool> keys)
{
	
	player->GetCamera()->keyControl(keys, platform->GetDeltaTime());
	
	return false;
}

void Game::Close()
{
	std::cout << " Close Init" << std::endl;
}
