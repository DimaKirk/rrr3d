#ifndef R3D_GRAPH_OCTREESORT
#define R3D_GRAPH_OCTREESORT

#include "Actor.h"

#include <deque>

namespace r3d
{

namespace graph
{

class OctreeSort: public lsl::Object
{
public:
	class UserNode;

	typedef lsl::List<UserNode*> UserList;
private:
	class Node;

	typedef lsl::List<Node*> NodeList;
	typedef UserList Leaf;
	typedef lsl::List<Leaf*> LeafList;
	typedef std::map<UserNode*, unsigned> UserMap;

	struct NodeCull
	{
		NodeCull(): node(0) {}
		NodeCull(Node* mNode): node(mNode) {}
		NodeCull(Node* mNode, Frustum::SpaceContains mContains): node(mNode), contains(mContains) {}

		bool operator==(Node* value) const
		{
			return node == value;
		}

		Node* operator->() const
		{
			return node;
		}
		Node& operator*() const
		{
			return *node;
		}
		operator Node*() const
		{
			return node;
		}

		Node* node;
		Frustum::SpaceContains contains;
	};
	typedef lsl::List<NodeCull> NodeCullList;

	//������������ ���� �����
	class Node
	{
		friend UserNode;
	private:
		//�������������� ����
		AABB _aabb;
		//��������
		Node* _parent;
		//������ �������� �����
		NodeList _nodeList;
		
		//������ ������� �� �������, �.�. ��� ������������ ����������� �� ������� � ��������� �������������
		Leaf* _leaf;
		//������ ������� ���������� �������� �������� � �������� ����� ��������������� �� �������
		UserMap* _userMap;
		//���������� �������� � ����� �������� ����
		unsigned _userCnt;
		//����� ������ �� ����
		unsigned _refCnt;
		//���������� ���� �� ���������. ������� �������� ��� ������������ ������
		unsigned _lockCnt;

		//������� ����������� ������������ � ������
		void InsertToUserMap(UserNode* value, unsigned group);
		void RemoveFromUserMap(UserNode* value, unsigned group);

		//���������������� ������� ������������� ��� ������ ������� �������
		void DoInsertUser(UserNode* value, unsigned group);
		void DoRemoveUser(UserNode* value, unsigned group);
	public:
		Node(unsigned numGroups, const AABB& aabb, Node* parent);
		~Node();

		//�������� ��� ������� �������������
		void InsertUser(UserNode* value, unsigned group);
		void InsertUser(const UserList& value);
		void RemoveUser(UserNode* value, unsigned group);		
		void ClearUser(unsigned group);
		void ClearUser();

		//�������� � ��������� ������
		Node& AddNode(const AABB& aabb);
		void DelNode(Node* value);
		void ClearChildNode();

		//������
		void AddRef();
		void Release();

		//����������
		void Lock();
		void Unlock();

		//����������� ������
		const AABB& GetAABB() const;
		const Leaf& GetLeaf(unsigned group) const;
		const UserMap& GetUserMap(unsigned group) const;
		const NodeList& GetNodeList() const;

		//����� �������� ������������� � ����
		unsigned GetUserCnt() const;

		//����� ����� �����
		const unsigned cNumGroups;
	};
public:
	//������������ �������
	class UserNode: public lsl::Object
	{
		friend Node;
		friend OctreeSort;
		friend class lsl::Collection<UserNode, void, void, void>;
	private:
		typedef std::map<Node*, unsigned> NodeMap;
	private:
		//��������
		OctreeSort* _owner;
		//
		AABB _aabb;
		//
		lsl::Object* _data;
		
		//������ ����� � ������ ������� ������� ������������
		NodeMap _nodeMap;	
		//������ ����� � ������� �� ��������
		lsl::BoolVec _groups;
		//������������� �������
		unsigned long _idPass;

		UserNode() {}
		UserNode(OctreeSort* owner, const AABB& aabb, const lsl::BoolVec& groups);
		virtual ~UserNode();

		//�������� � ������ �����������, ��� ������ ������� �������
		void DoInsertToNode(const NodeMap::iterator& iter);
		NodeMap::iterator DoInsertToNode(Node* value);
		NodeMap::iterator DoRemoveFromNode(const NodeMap::iterator& iter);
		NodeMap::iterator DoRemoveFromNode(Node* value);

		//�������� � ������ �����������
		void InsertToNode(Node* value);
		NodeMap::iterator RemoveFromNode(const NodeMap::iterator& iter);
		NodeMap::iterator RemoveFromNode(Node* value);
		void RemoveFromAllNodes();

		//NodeMap::iterator SetGroup(const NodeMap::iterator& iter, unsigned group, bool value);

		//void MoveTo(const AABB& aabb, Node* node);
		//void Move(const AABB& aabb);
	public:
		OctreeSort* GetOwner();

		const AABB& GetAABB() const;

		//������
		bool GetGroup(unsigned group) const;
		//void SetGroup(unsigned group, bool value);
		const lsl::BoolVec& GetGroups() const;
		//void SetGroups(const lsl::BoolVec& value);

		lsl::Object* GetData();
		void SetData(lsl::Object* value);
	};

	//��������� �������� ������� � �������� ������ � ������������ �������������
	class Position
	{
		friend OctreeSort;
	private:
		//��������
		OctreeSort* _owner;
		//������ ����� ��� ������������
		NodeCullList _nodeList;

		//����� ������� ������������
		bool _stIterate;
		//������ ��� ������������
		unsigned _group;
		//���������
		NodeCullList::const_iterator _iterNode;
		UserMap::const_iterator _iterUser;

		void InsertNode(Node* value, Frustum::SpaceContains contains)
		{
			LSL_ASSERT(value && contains != Frustum::scNoOverlap && !_nodeList.IsFind(value));

			_nodeList.push_back(NodeCull(value, contains));
			value->AddRef();
		}
		//����������� ������ ���������� �������������
		void InsertNode(const NodeCullList& nodeList)
		{
			_nodeList.insert(_nodeList.end(), nodeList.begin(), nodeList.end());

			for (NodeCullList::iterator iter = _nodeList.begin(); iter != _nodeList.end(); ++iter)
				(*iter)->AddRef();
		}
		void RemoveNode(Node* value)
		{
			_nodeList.Remove(value);
			value->Release();
		}
		void ClearNodes()
		{
			for (NodeCullList::iterator iter = _nodeList.begin(); iter != _nodeList.end(); ++iter)
				(*iter)->Release();
			_nodeList.clear();
		}

		UserNode* Iterate()
		{
			while (_iterNode != _nodeList.end())
			{
				if (_iterUser != (*_iterNode)->GetUserMap(_group).end())
				{
					LSL_ASSERT(_iterUser->first);

					return (_iterUser++)->first;
				}
				else
				{
					//������������ ������ ����
					(*_iterNode)->Unlock();
					//
					if (++_iterNode != _nodeList.end())
					{
						//��������� ����� ����
						(*_iterNode)->Lock();
						//����� ������������
						_iterUser = (*_iterNode)->GetUserMap(_group).begin();
					}
				}
			}
			
			return 0;
		}

		Position(OctreeSort* owner): _owner(owner), _stIterate(false) {}
	public:
		Position(): _owner(0), _stIterate(false) {}

		Position(const Position& value): _owner(value._owner), _stIterate(false)
		{
			InsertNode(value._nodeList);
		}

		~Position()
		{
			Drop();
		}

		void Drop()
		{
			LSL_ASSERT(!_stIterate);

			ClearNodes();
		}

		void BeginIterate(unsigned group)
		{
			LSL_ASSERT(!_stIterate && _owner);

			_stIterate = true;
			//����������� ������������� ������� �� ����, ����� ������ ������ ��� ��������
			++(_owner->_idPass);
			_group = group;

			_iterNode = _nodeList.begin();
			if (_iterNode != _nodeList.end())
			{
				(*_iterNode)->Lock();
				_iterUser = (*_iterNode)->GetUserMap(_group).begin();
			}
		}

		void EndIterate()
		{
			LSL_ASSERT(_stIterate);

			_stIterate = false;

			if (_iterNode != _nodeList.end())			
				(*_iterNode)->Unlock();
		}

		UserNode* OctreeSort::Position::Next(const Frustum& frustum)
		{
			UserNode* res = 0;
			bool next = false;
			do
			{
				res = Iterate();

				//���� idPass ������������ ��������� � idPass ����, �� �� ��� ���������� � �������
				next = res && res->_idPass == _owner->_idPass;
				if (res && !next)
				{
					res->_idPass = _owner->_idPass;
					//���������� ����������� � �������� AABB �������. ������ ���� ������������ ��� �� ������ � ������ ���������
					next = _iterNode->contains != Frustum::scContainsFully && frustum.ContainsAABB(res->GetAABB()) == Frustum::scNoOverlap;
				}
			}			
			while (next);

			return res;
		}

		Position& operator=(const Position& value)
		{
			Drop();

			_owner = value._owner;
			InsertNode(value._nodeList);

			return *this;
		}
	};

	class Users: public lsl::Collection<UserNode, void, void, void>
	{
	private:
		typedef lsl::Collection<UserNode, void, void, void> _MyBase;
	private:
		OctreeSort* _owner;		
	protected:
		virtual void InsertItem(const Value& value)
		{
			_MyBase::InsertItem(value);

			_owner->InsertUser(value);
		}

		virtual void RemoveItem(const Value& value)
		{
			_MyBase::RemoveItem(value);

			_owner->RemoveUser(value);
		}
	public:
		Users(OctreeSort* owner): _owner(owner) {};

		UserNode& Add(const AABB& aabb, const lsl::BoolVec& groups)
		{
			LSL_ASSERT(groups.size() == _owner->cNumGroups);

			return _MyBase::AddItem(new UserNode(_owner, aabb, groups));
		}
	};	

	//������� ��������� ������, ��� ��������� ������ �� ��� ��������� ������������
	//����������� ���������� �������� ������� ����� ���� � ����
	static const unsigned cMinNodeUserCnt;
	//�����������, ���������� ������ ����
	static const unsigned cMinLeafSize;
	//
	static const unsigned cOctNum = 8;
private:
	Users* _users;
	Node* _root;

	D3DXVECTOR3 _nodeOff[cOctNum];
	//���������� ������������� �������
	unsigned long _idPass;

	void BuildOctree(Node& node, const UserList& userList);

	void InsertUser(UserNode* user);
	void RemoveUser(UserNode* user);

	//���������� true ���� ������ ������� �������� ���������
	bool DoCulling(const Frustum& frustum, Frustum::SpaceContains spaceCont, Node& node, Position& pos);
	//����, ������ ��������� ��������� � ����� ������
	void DoCulling(const Frustum& frustum, Node& node, Position& pos);
public:
	OctreeSort(unsigned numGroups);
	virtual ~OctreeSort();

	void DestroyOctree();
	void RebuildOctree(const AABB& worldAABB);
	bool IsBuildOctree() const;

	Position Culling(const Frustum& frustum);

	void RenderNode(graph::Engine& engine, const Frustum& frustum, const Node& node);
	void Render(graph::Engine& engine, const Frustum& frustum);
	void Render(graph::Engine& engine, const Position& pos);

	Users& GetUsers();
	const AABB& GetRootAABB() const;

	const unsigned cNumGroups;
};

}

}

#endif