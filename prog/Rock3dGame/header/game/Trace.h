#pragma once

namespace r3d
{

namespace game
{

class WayNode;
class Trace;
class WayPath;

class WayPoint: public Object
{
	friend WayNode;
public:
	typedef lsl::List<WayPoint*> Points;
	typedef lsl::List<WayNode*> Nodes;
private:
	Trace* _trace;
	unsigned _id;

	D3DXVECTOR3 _pos;
	float _size;
	D3DXVECTOR3 _off;

	Nodes _nodes;

	void InsertNode(WayNode* node);
	void RemoveNode(WayNode* node);

	void Changed();
public:
	WayPoint(Trace* trace, unsigned id);
	virtual ~WayPoint();

	bool RayCast(const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayVec, float* dist = 0) const;
	bool IsContains(const D3DXVECTOR3& point, float* dist = 0) const;

	unsigned GetId() const;

	//�������, � ������� �����������
	const D3DXVECTOR3& GetPos() const;
	void SetPos(const D3DXVECTOR3& value);
	//������, � ���� �������� �����
	float GetSize() const;
	void SetSize(float value);
	//����������� �����, � ���� �������� ������������ �������
	const D3DXVECTOR3& GetOff() const;
	void SetOff(const D3DXVECTOR3& value);

	//����������� �� ����� ����
	bool IsFind(WayPath* path) const;
	//
	bool IsFind(WayNode* node, WayNode* ignore = NULL) const;
	//�������� ��������� ���� �� ����������� � ignore
	WayNode* GetRandomNode(WayNode* ignore, bool hasNext);
	//
	const Nodes& GetNodes() const;
};

//����, ������������ ������� ��������� ������������ ��������������� ����. ���� �������� ����� � ����� ��������� �� ���
class WayNode: public Object
{
	friend class WayPath;
private:
	//����, ���������� � ���� _node
	//������������ ����� 3� �������, ������������ ����� ������ _node � _node->_next. ��� ��������� �������� ��� ��������� ������������ 2� �������
	class Tile
	{
	private:
		WayNode* _node;

		//������������ ������, ������������ node, nextNode
		mutable D3DXVECTOR2 _dir;
		//���������� ����� �����
		mutable float _dirLength;
		//������� � ������������� �������
		mutable D3DXVECTOR2 _norm;
		//
		mutable D3DXVECTOR3 _normLine;
		//������������ �����
		mutable D3DXVECTOR3 _dirLine;
		//������� ������������ ������
		mutable D3DXVECTOR2 _midDir;
		//������� � _midDir
		mutable D3DXVECTOR2 _midNorm;
		//���������� �������������� ����� ����� node
		mutable D3DXVECTOR3 _midNormLine;
		//������� � _midDir � ����������� � ����������� ���� (���� ������ � ������� ����)
		mutable D3DXVECTOR2 _edgeNorm;
		//������� ������ ���������� ������� ����������� ����
		mutable float _nodeRadius;
		//����� ����� ���������� ���� �������� ��������������� _midNorm
		mutable D3DXVECTOR3 _edgeLine;
		//������ ���� �������� ���� (���� ������� ������), � ���
		mutable float _turnAngle;
		//��������� �� ��������� ����
		mutable float _finishDist;
		//��������� �� ���������� ����
		mutable float _startDist;
		//���� ���������
		mutable bool _changed;

		//��������� ���������
		void ApplyChanges() const;

		const D3DXVECTOR3& GetPos() const;
		float GetHeight() const;
		//
		const D3DXVECTOR3& GetNextPos() const;
		float GetNextHeight() const;
		const D3DXVECTOR2& GetPrevDir() const;
		const D3DXVECTOR2& GetNextMidNorm() const;
		const D3DXVECTOR3& GetNextNormLine() const;
		float GetNextNodeRadius() const;

		D3DXPLANE GetWayPlane() const;		
	public:
		Tile(WayNode* node);

		void Changed();
		//���� upVec ������, �� � 3� ����������� ����� � 2� �����������
		void GetVBuf(D3DXVECTOR3* vBuf, unsigned length, const D3DXVECTOR3* upVec) const;
		//������ � ���������
		//����� ������� ������������� ������� � 0 �� ������� ����� � ����������� ������� GetNorm
		//����� �� �������
		unsigned ComputeTrackInd(const D3DXVECTOR2& point) const;
		//������������� ������ �������� �� �������� ������� track ������������ point
		D3DXVECTOR2 ComputeTrackNormOff(const D3DXVECTOR2& point, unsigned track) const;

		bool RayCast(const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayVec, float* dist = 0) const;
		//lengthClamp - ������������ �� ���� �� �����
		bool IsContains(const D3DXVECTOR3& point, bool lengthClamp = true, float* dist = 0, float widthErr = 0.0f) const;
		bool IsZLevelContains(const D3DXVECTOR3& point, float* dist = 0) const;

		const D3DXVECTOR2& GetDir() const;
		float GetDirLength() const;
		const D3DXVECTOR2& GetNorm() const;
		const D3DXVECTOR3& GetDirLine() const;
		const D3DXVECTOR2& GetMidDir() const;
		const D3DXVECTOR2& GetMidNorm() const;
		const D3DXVECTOR3& GetMidNormLine() const;
		const D3DXVECTOR2& GetEdgeNorm() const;
		float GetNodeRadius() const;
		const D3DXVECTOR3& GetEdgeLine() const;
		float GetTurnAngle() const;
		float GetFinishDist() const;
		float GetStartDist() const;

		//coordX - ������������� ���������� ���������� �����
		float ComputeCoordX(float dist) const;
		float ComputeCoordX(const D3DXVECTOR2& point) const;
		//����� �����
		float ComputeLength(float coordX) const;
		//����� ������� � ������������ �����
		float ComputeWidth(float coordX) const;
		//������ ����� � ������������ �����, coordX = [0..1]
		float ComputeHeight(float coordX) const;
		//Z ���������� ����� � ����� coordX
		float ComputeZCoord(float coordX) const;
		//
		D3DXVECTOR3 GetPoint(float coordX) const;
		//
		float GetLength(const D3DXVECTOR2& point) const;
		float GetHeight(const D3DXVECTOR2& point) const;
		float GetWidth(const D3DXVECTOR2& point) const;
		float GetZCoord(const D3DXVECTOR2& point) const;
		D3DXVECTOR3 GetCenter3() const;

		const unsigned cTrackCnt;
	};
private:
	WayPath* _path;
	WayPoint* _point;
	Tile* _tile;

	WayNode* _prev;
	WayNode* _next;

	WayNode(WayPath* path, WayPoint* point);
	~WayNode();

	void SetPrev(WayNode* node);
	void SetNext(WayNode* node);
public:
	void Changed();

	bool RayCast(const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayVec, float* dist = 0) const;
	bool IsContains2(const D3DXVECTOR2& point, float* dist = 0) const;
	bool IsContains(const D3DXVECTOR3& point, float* dist = 0) const;

	WayPath* GetPath();
	WayPoint* GetPoint();
	//����, ������� ���������� � ������� ����. ������������ ����� 2� ���������
	const Tile& GetTile() const;

	WayNode* GetPrev();
	WayNode* GetNext();

	const D3DXVECTOR3& GetPos() const;
	D3DXVECTOR2 GetPos2() const;
	float GetSize() const;
	float GetRadius() const;
};

class WayPath
{
	friend WayNode;
	friend class Trace;
private:
	Trace* _trace;
	bool _enclosed;

	WayNode* _first;
	WayNode* _last;
	unsigned _count;

	WayPath(Trace* trace);
	~WayPath();
public:
	//mWhere - ����� ����� mWhere, == 0 � �����
	WayNode* Add(WayPoint* point, WayNode* mWhere = 0);
	void Delete(WayNode* value);
	void Clear();

	bool IsEnclosed() const;
	void Enclosed(bool value);

	WayNode* RayCast(const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayVec, WayNode* mWhere = 0, float* dist = 0) const;
	//����� ����-����� ����������� point. ��� ������ ������������ �������� ����� � ������ ���������� ����
	WayNode* IsTileContains(const D3DXVECTOR3& point, WayNode* mWhere = 0) const;
	//���� upVec ������, �� � 3� ����������� ����� � 2� �����������
	void GetTriStripVBuf(res::VertexData& data, const D3DXVECTOR3* upVec);

	Trace* GetTrace();
	WayNode* GetFirst();
	WayNode* GetLast();
	unsigned GetCount() const;

	float GetLength() const;
};

class Trace: public Component
{
	typedef Component _MyBase;
public:
	typedef lsl::List<WayPoint*> Points;
	typedef lsl::List<WayPath*> Pathes;
private:
	Points _points;
	Pathes _pathes;

	unsigned _pointId;

	WayPoint* AddPoint(unsigned id);
protected:
	virtual void Save(SWriter* writer);
	virtual void Load(SReader* reader);
public:
	Trace(unsigned tracksCnt);
	virtual ~Trace();

	WayPoint* AddPoint();
	void DelPoint(WayPoint* value);
	void ClearPoints();

	WayPath* AddPath();
	void DelPath(WayPath* value);
	void ClearPathes();

	void Clear();
	WayNode* RayCast(const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayVec, float* dist = 0) const;
	WayNode* IsTileContains(const D3DXVECTOR3& point, WayNode* mWhere = 0) const;
	WayPoint* FindPoint(unsigned id);
	WayNode* FindClosestNode(const D3DXVECTOR3& point);

	const Points& GetPoints() const;
	const Pathes& GetPathes() const;

	const unsigned cTrackCnt;
};

}

}