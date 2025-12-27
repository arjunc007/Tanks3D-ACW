#pragma once
#include <map>
#include <vector>
#include "DirectXMath.h"
#include "ObserverSubject.h"

// Forward references
class GameObjectComponent;
class Message;


// Typedefs
typedef std::map<std::string, GameObjectComponent*> ComponentMap;
typedef std::map<std::string, GameObjectComponent*>::iterator ComponentMapIterator;
typedef std::vector<GameObjectComponent*>::iterator ComponentListIterator;


// Now new and improved to use a component-based architecture
class GameObject
	: public ObserverSubject
{
	// Constants
public:


	// Data
protected:
	std::string			_type;			// Type of the object
	DirectX::XMFLOAT3	_scale;			// Scale of the object (1 = normal)
	DirectX::XMFLOAT3	_position;		// Position of object's origin
	DirectX::XMFLOAT4	_rotation;		// Rotation stored as a Quaternion
	bool				_alive;			// Alive flag; when false, is not updated
	bool				_deleteFlag;	// Set when you want this game object to be deleted by the game

										// Components
	ComponentMap		_components;


	// Constructors
public:
	GameObject(std::string type);
	virtual ~GameObject();

	// Disable copy constructor + assignment operator
private:
	GameObject(const GameObject&);
	GameObject& operator=(const GameObject&);


	// Gets/sets
public:
	const DirectX::XMFLOAT3& GetScale()										const { return _scale; }
	void SetScale(float v) { _scale.x = _scale.y = _scale.z = v; }
	void SetScale(DirectX::XMFLOAT3 v) { _scale = v; }

	const DirectX::XMFLOAT3& GetPosition()								const { return _position; }
	void SetPosition(DirectX::XMFLOAT3 v) { _position = v; }

	const DirectX::XMFLOAT4& GetRotation()								const { return _rotation; }
	void SetRotation(DirectX::XMFLOAT4 r) { _rotation = r; }
	void SetRotation(float pitch, float yaw, float roll);

	DirectX::XMFLOAT3 GetForward()										const;

	bool IsAlive()														const { return _alive; }
	void SetAlive(bool v) { _alive = v; }

	bool ShouldBeDeleted()												const { return _deleteFlag; }
	void SetDeleteFlag(bool v) { _deleteFlag = v; }

	std::string GetType()												const { return _type; }

	// Component Functions
public:
	bool AddComponent(GameObjectComponent* goc);
	bool RemoveComponent(GameObjectComponent* goc);
	bool RemoveComponent(std::string componentType);
	GameObjectComponent* GetComponent(std::string type);

	// General Functions
public:
	// Setup function -- called to initialise object and its components. Should only be called once
	virtual void Start();

	// Main update function (called every frame)
	virtual void Update(double deltaTime);

	// Message handler (called when message occurs)
	virtual void OnMessage(Message* msg) { BroadcastMessage(msg); }

	// Shutdown function -- called when object is destroyed (i.e., from destructor)
	virtual void End();

	// Resets the game object to the start state (similar to Start(), but may be called more than once)
	virtual void Reset();
};

