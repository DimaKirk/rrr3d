#ifndef LSL_COMPONENT
#define LSL_COMPONENT

#include "lslCommon.h"
#include "lslSerializable.h"
#include "lslObject.h"
#include "lslUpdateAble.h"
#include "lslContainer.h"

namespace lsl
{

//�������� � ����������� ���������� ���������� �������
//��� ����������� � ������ ��� ������� �� �������������
//��� ������ ���������� ������������ ������������� ����, ������������ ������ ����������
//��� ������������ ������ �� ��������� ����� ����� ������������ ���������� ����, �.�. ���� ������������ ��������� ����������(���� � �������� Owner = 0)
class Component: public virtual Object, public virtual UpdateAble, public virtual lsl::Serializable
{
public:
	class Elements: public lsl::Container<Component*>
	{
	private:
		typedef lsl::Container<Component*> _MyBase;
	private:
		Component* _owner;
	protected:
		virtual void InsertItem(Item& item)
		{
			_MyBase::InsertItem(item);

			if (!_owner->ValidateChild(item))
				LSL_ASSERT(false);

			item->_owner = _owner;
		}
		virtual void RemoveItem(Item& item)
		{
			_MyBase::RemoveItem(item);

			item->_owner = 0;
		}
	public:
		Elements(Component* owner): _owner(owner) {}
	};

	static const char* cDefItemName;
private:	
	std::string _name;

	//��� �������������� ������ ������������ ��������� ���� �� ���������(������ ����� ��������������� ������ �� ����������)
	Component* _owner;
	Elements* _elements;
protected:
	bool ValidateChild(Component* child);

	virtual void DoNotifyChanged(UpdateAble* sender, const UpdateData* data) {}

	virtual void Save(SWriter* writer);
	virtual void Load(SReader* reader);
public:
	Component();
	virtual ~Component();

	//����� ������ �� �����
	Component* FindChild(const std::string& name);
	const Component* FindChild(const std::string& name) const;
	//���� � �����. ����� ���������� ������������ ����� ������ ����., ��� ������ ���������� � ���� �� ����������!
	Component* FindComponent(const std::string& name);
	//���� � �����. ����� ���������� � ����� ��������� ����������, ��� ��������� ���������� ����������� ���������� � ����!
	Component* AbsoluteFindComponent(const std::string& name);
	//���� �� ���� top, �� ������� ���, ���� �������� 0 �� ���� ����� ���������� � ����� ��������� ���������� root
	std::string GetComponentPath(Component* top) const;

	bool ValidateName(const std::string& name) const;
	std::string MakeUniqueName(const std::string& base) const;

	Component* GetOwner();
	void SetOwner(Component* value);
	Component* GetRoot();
	const std::string& GetName() const;
	void SetName(const std::string& value);
	Elements& GetElements() const;

	bool storeName;
};

bool ValidateComponentName(const std::string& name);

}

#endif