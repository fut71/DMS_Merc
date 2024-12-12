#pragma once
#include "pch.h"
#include "Transform.h"
#include "Entity.h"
#include "Scene.h"
#include "World.h"

using namespace entt::literals;

// ������Ʈ �ø��������
template <IsComponent T>
json SerializeComponent(const T& _component)
{
	json j;
	auto type = entt::resolve<T>();
	for (auto data : type.data())
	{
		j[data.name().data()] = data.get(_component).template cast<float>();
	}
	return j;
}

// ������Ʈ ��ø��������
template <IsComponent T>
T DeserializeComponent(const json& j)
{
	T component;
	auto type = entt::resolve<T>();
	for (auto data : type.data())
	{
		auto it = j.find(data.name().data());
		if (it != j.end())
		{
			data.set(component, it->get<float>());
		}
	}
	return component;
}

// ��ƼƼ �ø��������
json SerializeEntity(const std::shared_ptr<Entity>& _entity)
{
	json j;
	j["id"] = _entity->GetUID();
	if (_entity->HasComponent<Transform>())
	{
		j["Transform"] = SerializeComponent(_entity->GetComponent<Transform>());
	}
	// �ٸ� ������Ʈ�鵵 �̰��� �߰�

	return j;
}

// ��ƼƼ ��ø��������
void DeserializeEntity(const json& j, std::shared_ptr<Entity>& _entity)
{
	_entity->SetUID(j["id"]);
	if (j.contains("Transform"))
	{
		auto transform = DeserializeComponent<Transform>(j.at("Transform"));
		_entity->AddComponent<Transform>(transform);
	}
	// �ٸ� ������Ʈ�鵵 �̰��� �߰�
}

// �� �ø��������
json SerializeScene(const std::shared_ptr<Scene>& _scene)
{
	json j;
	j["id"] = _scene->GetUID();
	j["entities"] = json::array();
	for (auto& [uid, entity] : _scene->GetEntityMap())
	{
		j["entities"].push_back(SerializeEntity(entity));
	}
	return j;
}

// �� ��ø��������
void DeserializeScene(const json& j, std::shared_ptr<Scene>& _scene)
{
	_scene->SetUID(j["id"]);
	for (const auto& entityJson : j.at("entities"))
	{
		auto entity = _scene->CreateEntity(entityJson["name"]);
		DeserializeEntity(entityJson, entity);
	}
}

// ���� �ø��������
json SerializeWorld(const std::shared_ptr<World>& _world)
{
	json j;
	j["id"] = _world->GetUID();
	j["scenes"] = json::array();
	for (auto& [uid, scene] : _world->GetSceneMap())
	{
		j["scenes"].push_back(SerializeScene(scene));
	}
	return j;
}

// ���� �ø��������
void DeserializeWorld(const json& j, std::shared_ptr<World>& _world)
{
	_world->SetUID(j["id"]);
	for (const auto& sceneJson : j.at("scenes"))
	{
		auto scene = _world->CreateScene(sceneJson["name"]);
		DeserializeScene(sceneJson, scene);
	}
}
