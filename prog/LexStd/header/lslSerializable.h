//���� ���� ������ ����������� ������ ��� ���������� ������ ���������, �� ����� ����� �� ������, ��������� ��������� ������������ �������� � ���������� �������������� ����� ������ ������ �������� �� ������� ��������

#ifndef LSL_SERIALIZABLE
#define LSL_SERIALIZABLE

#include "lslCommon.h"
#include "lslUtility.h"
#include "lslException.h"

namespace lsl
{

class SWriter;
class SReader;
class SerialNode;
class Component;
class CollectionItem;

class Serializable
{
	friend SWriter;
	friend SReader;
	friend SerialNode;
public:
	struct FixUpName
	{
		//��� ������(��� ����� ����������������� ������, ��� ����)
		std::string name;
		//���������� ���� �� ���������� � �������� ����������� ������� ��� ���
		std::string path;
		//��� �������� ���������
		std::string nameCollItem;

		//����������� FixUp Serializable(��� ����� Serializable, ������� ��������� ������)
		Serializable* target;
		//���� �� �������� �����. ������
		SReader* sender;
		//������� ���������, �� ��������� ���� ����. �� �������
		Component* component;
		//������� ������� ���������, �� ��������� ���� ����� ���������
		CollectionItem* collItem;

		template<class _Class> _Class GetComponent() const
		{
			return StaticCast<_Class>(component);
		}
		template<class _Class> _Class GetCollItem() const
		{
			return StaticCast<_Class>(collItem);
		}

		bool IsValid(bool collItem) const
		{
			return (!collItem || !nameCollItem.empty()) && !path.empty() && !name.empty();
		}
	};
	typedef std::list<FixUpName> FixUpNames;

	typedef std::list<Serializable*> ProxySerList;

	static const char* cInclude;
	static const char* cMaster;
	static const char* cItem;
private:
	Serializable* _masterSer;
	ProxySerList _proxySerList;
	unsigned _lockCnt;
protected:
	virtual void Save(SWriter* writer) = 0;
	virtual void Load(SReader* reader) = 0;
	//�� ��������� ��������� ������ ���� ��� ��� ����� � ���� ���� ������������� ����-���
	//������� ����������� �����. ������(�.�. ����� ��������� ��������), �� ��������� �������� - ������ ���������� ������ ������� ��������������� ������
	virtual void OnFixUp(const FixUpNames& fixUpNames) {};
public:
	Serializable();
	~Serializable();

	//����������� ������ �� ��������� value. �������������� ����� ������������. ���� ������������ ������ ������, �� ��. ����� ���� (proxyRef)
	void AssignFromSNode(SerialNode* node);
	void AssignFromSer(Serializable* value, lsl::Component* root);

	//������ ������ ����� Serializable ��� ���������� ������ ����������� ����������. �������� � �������� ���� ���� ������ ���� ���������� ������� � ������������, ����� ������ ������ ������������� SerialNode(� ������������� � Serializable), �.�. ���� � �������� ���������� ���������� � ������� �������� ���������� ��� ���� ��������� �����. ���� ������ �� ����� ���� ��������� �� ���������� ���������� ������ ���� ���� "include" � ����� ������ master. ��� �������� ������������ ��� ������, � ��� "include" ���������� ��� ������� �������� �������� master.
	void InsertProxySer(Serializable* value);
	void RemoveProxySer(Serializable* value);
	void ClearProxySer();
	void SyncProxySer(lsl::Component* root);
	//������ ������
	Serializable* GetMasterSer();
	void SetMasterSer(Serializable* value);
	//������ ������ ��������
	const ProxySerList& GetProxySerList() const;
};

}

#endif