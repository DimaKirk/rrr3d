#pragma once

#include "IMapObj.h"

namespace r3d
{

namespace edit
{

//�� ����� ���� �������� ���� �� ����������� �����������, � ������� ���������� ���������������� ���� � ����������� � ������, � �� ������ ���������� �������� ��������� �� ����������� ������������. � ����� ���������� ������� ����������� ��� ���������� ��������� �����
class IScNodeCont: public Object
{
public:
	virtual void Select(bool active) = 0;
	//������� �������� ����
	virtual bool RayCastInters(const D3DXVECTOR3& rayPos, const D3DXVECTOR3& rayVec) const = 0;
	//�������� � �������� �����
	virtual bool Compare(const IMapObjRef& node) const = 0;

	//������������� � ������ none
	virtual void OnStartDrag(const D3DXVECTOR3& scrRayPos, const D3DXVECTOR3& scrRayVec) {};
	virtual void OnEndDrag(const D3DXVECTOR3& scrRayPos, const D3DXVECTOR3& scrRayVec) {};
	virtual void OnDrag(const D3DXVECTOR3& pos, const D3DXVECTOR3& scrRayPos, const D3DXVECTOR3& scrRayVec) {};
	//�������� ��� ������� shift
	virtual void OnShiftAction(const D3DXVECTOR3& scrRayPos, const D3DXVECTOR3& scrRayVec) {};

	//� ������� �����������
	virtual D3DXVECTOR3 GetPos() const = 0;
	virtual void SetPos(const D3DXVECTOR3& value) = 0;
	//
	virtual D3DXQUATERNION GetRot() const = 0;
	virtual void SetRot(const D3DXQUATERNION& value) = 0;
	//
	virtual D3DXVECTOR3 GetScale() const = 0;
	virtual void SetScale(const D3DXVECTOR3& value) = 0;
	//
	virtual D3DXVECTOR3 GetDir() const = 0;
	virtual D3DXVECTOR3 GetRight() const = 0;
	virtual D3DXVECTOR3 GetUp() const = 0;

	virtual D3DXMATRIX GetMat() const = 0;
	//������� �������� ����
	virtual AABB GetAABB() const = 0;
};

typedef lsl::AutoRef<IScNodeCont> IScNodeContRef;

class ISceneControl: public Object
{
public:
	enum SelMode {smNone, smLink, smMove, smRotate, smScale};
	enum DirMove {dmNone, dmX, dmY, dmZ, dmXY, dmXZ, dmYZ, dmView, cDirMoveEnd};

	typedef IScNodeCont INode;
	typedef IScNodeContRef INodeRef;
public:
	virtual D3DXVECTOR3 ComputePoint(const D3DXVECTOR3& curPos, const D3DXVECTOR3& rayStart, const D3DXVECTOR3& rayVec, DirMove dirMove, const D3DXVECTOR3& centerOff) = 0;
	virtual D3DXVECTOR3 ComputePos(INode* node, const D3DXVECTOR3& rayStart, const D3DXVECTOR3& rayVec, DirMove dirMove, const D3DXVECTOR3& centerOff) = 0;

	virtual INodeRef GetSelNode() = 0;
	virtual void SelectNode(const INodeRef& value) = 0;

	virtual SelMode GetSelMode() const = 0;
	virtual void SetSelMode(SelMode value) = 0;

	virtual bool GetLinkBB() const = 0;
	virtual void SetLinkBB(bool value) = 0;
};

}

}