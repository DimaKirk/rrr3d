#ifndef R3D_GAME_GAMEOBJECT
#define R3D_GAME_GAMEOBJECT

#include "GameBase.h"

namespace r3d
{

namespace game
{

class Proj;
class GameCar;

class GameObject: public lsl::Component, public GameObjListener, public px::ActorUser, IFrameEvent, IProgressEvent, ILateProgressEvent, IFixedStepEvent
{
	friend class MapObj;
	friend class PxActorUser;
	friend class Logic;
private:
	typedef Component _MyBase;
public:
	typedef std::list<GameObject*> Children;
	typedef MapObjects IncludeList;

	typedef lsl::Container<GameObjListener*> ListenerList;

	enum LiveState {lsLive, lsDeath};

	struct RayCastHit
	{
		GameObject* gameActor;
		float distance;
	};

	struct MyEventData: public EventData
	{		
		int targetPlayerId;
		GameObject* target;
		float damage;
		GameObject::DamageType damageType;

		MyEventData(): targetPlayerId(cUndefPlayerId), target(NULL) {}
		MyEventData(int mPlayerId, int mTargetPlayerId = cUndefPlayerId, GameObject* mTarget = NULL, float mDamage = 0.0f, GameObject::DamageType mDamageType = GameObject::dtSimple): EventData(mPlayerId), targetPlayerId(mTargetPlayerId), target(mTarget), damage(mDamage), damageType(mDamageType) {}
	};
private:
	MapObj* _mapObj;
	Logic* _logic;	
	
	graph::Actor* _grActor;
	px::Actor* _pxActor;	

	GameObject* _parent;
	Children _children;
	IncludeList* _includeList;
	ListenerList _listenerList;
	Behaviors* _behaviors;	

	LiveState _liveState;
	//��������� ���������� ��������
	//<0 - ����������� ����� ��������
	float _maxLife;
	float _maxTimeLife;
	//������� ����� ��������
	float _life;
	float _timeLife;
	//
	bool _immortalFlag;
	//
	float _immortalTime;
	//��������������� ����, ����� �������� �������� ������ Destroy
	bool _destroy;

	unsigned _frameEventCount;
	unsigned _progressEventCount;
	unsigned _lateProgressEventCount;
	unsigned _fixedStepEventCount;
	bool _syncFrameEvent;
	bool _bodyProgressEvent;

	int _touchPlayerId;
	float _touchPlayerTime;

	D3DXVECTOR3 _posSync;
	D3DXVECTOR3 _posSyncDir;
	float _posSyncLength;

	D3DXQUATERNION _rotSync;
	D3DXVECTOR3 _rotSyncAxis;
	float _rotSyncAngle;

	D3DXVECTOR3 _posSync2;
	D3DXVECTOR3 _posSyncDir2;
	float _posSyncDist2;
	float _posSyncLength2;

	D3DXQUATERNION _rotSync2;
	D3DXVECTOR3 _rotSyncAxis2;
	float _rotSyncAngle2;
	float _rotSyncLength2;

	D3DXVECTOR3 _pxPosLerp;
	D3DXQUATERNION _pxRotLerp;
	D3DXVECTOR3 _pxVelocityLerp;

	D3DXVECTOR3 _pxPrevPos;
	D3DXQUATERNION _pxPrevRot;
	D3DXVECTOR3 _pxPrevVelocity;

	void SetSyncFrameEvent(bool value);
	void SetBodyProgressEvent(bool value);	
protected:
	void Destroy();

	void RegFrameEvent();
	void UnregFrameEvent();

	void RegProgressEvent();
	void UnregProgressEvent();

	void RegLateProgressEvent();
	void UnregLateProgressEvent();

	void RegFixedStepEvent();
	void UnregFixedStepEvent();

	D3DXVECTOR3 GetContactPoint(const px::Scene::OnContactEvent& contact);
	bool ContainsContactGroup(NxContactStreamIterator& contIter, unsigned actorIndex, px::Scene::CollDisGroup group);

	//��������� ������ �� ������ �����, ���
	virtual void LogicReleased() {};
	virtual void LogicInited() {};

	virtual bool OnContactModify(const px::Scene::OnContactModifyEvent& contact) {return true;}
	virtual void OnContact(const px::Scene::OnContactEvent& contact);	
	virtual void OnWake();
	virtual void OnSleep();

	virtual void OnImmortalStatus(bool status) {}

	void RayCastClosestActor(const D3DXVECTOR3& rayStart, const D3DXVECTOR3& rayDir, NxShapesType shapesType, RayCastHit& hit, unsigned groups = 0xFFFFFFFF, unsigned mask = 0, float maxDist = NX_MAX_F32);

	void DoDeath(DamageType damageType = dtSimple, GameObject* target = NULL);
	void SendDeath(DamageType damageType = dtSimple, GameObject* target = NULL);

	//������������� � �������, ����� ��������� �� OnProgress
	virtual void OnPxSync(float alpha);
	virtual void OnLateProgress(float deltaTime, bool pxStep);
	virtual void OnFrame(float deltaTime, float pxAlpha);
	virtual void OnFixedStep(float deltaTime);

	void SaveCoords(lsl::SWriter* writer);
	void LoadCoords(lsl::SReader* reader);
	//
	virtual void SaveSource(lsl::SWriter* writer);
	virtual void LoadSource(lsl::SReader* reader);
	//
	virtual void SaveProxy(lsl::SWriter* writer);
	virtual void LoadProxy(lsl::SReader* reader);

	virtual void Save(lsl::SWriter* writer);
	virtual void Load(lsl::SReader* reader);
	virtual void OnFixUp(const FixUpNames& fixUpNames);
public:
	GameObject();
	virtual ~GameObject();

	void Assign(GameObject* value);

	virtual void InsertChild(GameObject* value);
	virtual void RemoveChild(GameObject* value);
	void ClearChildren();

	void InsertListener(GameObjListener* value);
	void RemoveListener(GameObjListener* value);
	void ClearListenerList();

	void Death(DamageType damageType = dtSimple, GameObject* target = NULL);
	void Resc();
	void Damage(int senderPlayerId, float value, float newLife, bool death, DamageType damageType);
	void Damage(int senderPlayerId, float value, DamageType damageType = dtSimple);
	//���������, ��������� �� MaxLife
	void Healt(float life);
	void LowLife(Behavior* behavior);

	bool GetImmortalFlag() const;
	void SetImmortalFlag(bool value);

	void Immortal(float time);
	bool IsImmortal() const;

	MapObj* GetMapObj();

	Logic* GetLogic();
	void SetLogic(Logic* value);

	void SendEvent(unsigned id, int playerId, MyEventData* data = NULL);
	void SendEvent(unsigned id, MyEventData* data = NULL);

	//��� ������ ��� ������������ ����� ��������� ��� ������� �����
	GameObject* GetParent();
	void SetParent(GameObject* value);
	const Children& GetChildren() const;

	IncludeList& GetIncludeList();

	graph::Actor& GetGrActor();
	px::Actor& GetPxActor();
	NxActor* GetNxActor();

	virtual Proj* IsProj();
	virtual GameCar* IsCar();
	
	//��������� ������������ ��� ��������� �������������
	//��������� �������������
	//�������
	const D3DXVECTOR3& GetPos() const;
	virtual void SetPos(const D3DXVECTOR3& value);
	//����������
	const D3DXVECTOR3& GetScale() const;
	virtual void SetScale(const D3DXVECTOR3& value);
	void SetScale(float value);
	//�������
	const D3DXQUATERNION& GetRot() const;
	virtual void SetRot(const D3DXQUATERNION& value);	
	//���������� �������������
	//�������
	D3DXVECTOR3 GetWorldPos() const;
	virtual void SetWorldPos(const D3DXVECTOR3& value);
	//�������
	D3DXQUATERNION GetWorldRot() const;
	virtual void SetWorldRot(const D3DXQUATERNION& value);
	
	void SetWorldDir(const D3DXVECTOR3& value);
	void SetWorldUp(const D3DXVECTOR3& value);

	const D3DXVECTOR3& GetPosSync() const;
	void SetPosSync(const D3DXVECTOR3& value);

	const D3DXQUATERNION& GetRotSync() const;
	void SetRotSync(const D3DXQUATERNION& value);

	const D3DXVECTOR3& GetPosSync2() const;
	void SetPosSync2(const D3DXVECTOR3& curSync, const D3DXVECTOR3& newSync);

	const D3DXQUATERNION& GetRotSync2() const;
	void SetRotSync2(const D3DXQUATERNION& curSync, const D3DXQUATERNION& newSync);

	const D3DXVECTOR3& GetPxPosLerp() const;
	const D3DXQUATERNION& GetPxRotLerp() const;
	const D3DXVECTOR3& GetPxVelocityLerp() const;

	const D3DXVECTOR3& GetPxPrevPos() const;
	const D3DXQUATERNION& GetPxPrevRot() const;
	const D3DXVECTOR3& GetPxPrevVelocity() const;

	LiveState GetLiveState() const;
	
	float GetMaxLife() const;
	void SetMaxLife(float value);

	float GetTimeLife() const;
	void SetTimeLife(float value);

	float GetMaxTimeLife() const;
	void SetMaxTimeLife(float value);

	float GetLife() const;
	void SetLife(float value);

	int GetTouchPlayerId() const;

	Behaviors& GetBehaviors();

	bool storeSource;
	bool storeProxy;

	virtual void OnProgress(float deltaTime);

	static GameObject* GetGameObjFromActor(px::Actor* actor);
	static GameObject* GetGameObjFromActor(NxActor* actor);
	static GameObject* GetGameObjFromShape(NxShape* shape);
};

}

}

#endif