#pragma once

#include "IDataBase.h"

namespace r3d
{

namespace edit
{

class Edit;

class MapObjRec: public IMapObjRec, public ExternImpl<game::MapObjRec>
{
protected:
	virtual VirtImpl* GetImpl() {return this;}
public:
	MapObjRec(Inst* inst);

	unsigned GetCategory();
	std::string GetCategoryName();

	const std::string& GetName() const;
	void SetName(const std::string& value);

	//����� ������������ ������ ������ ��������� ������, ����� �� �������� �� ������� �������� � ����� ������-����������
	game::RecordNode::RecordList::const_iterator libIter;
};

class RecordNode: public virtual IRecordNode, public ExternImpl<game::RecordNode>
{
protected:
	virtual VirtImpl* GetImpl() {return this;}
public:
	RecordNode(Inst* inst);

	//����� ������������ ������ ������ ��������� ������, ����� �� �������� �� ������� �������� � ����� ������-����������
	IMapObjRecRef FirstRecord();
	void NextRecord(IMapObjRecRef& ref);

	IRecordNodeRef FirstNode();
	void NextNode(IRecordNodeRef& ref);

	const std::string& GetName() const;

	game::RecordNode::NodeList::const_iterator libIter;
};

//��������� ��������� warning
#pragma warning(disable : WARNING_MULTIPLE_VIRTUAL_INHERIT_C4250)
class MapObjLib: public IMapObjLib, public RecordNode
{
public:
	typedef game::MapObjLib Inst;
protected:
	virtual VirtImpl* GetImpl() {return this;}
public:
	MapObjLib(Inst* inst);
};
//������������ ���������
#pragma warning(default : WARNING_MULTIPLE_VIRTUAL_INHERIT_C4250)

class DataBase: public IDataBase
{
private:
	Edit* _edit;
public:
	DataBase(Edit* edit);

	game::DataBase* GetInst();

	IMapObjLibRef GetMapObjLib(unsigned i);
	unsigned GetMapObjLibCnt();
};

}

}