//������ ������� �������� ��������� ������� ������ � �����������. ����� ������������ ���������� ������ (��� Begin/EndUpdate � �.�.) ��� ����������� ����������

#ifndef LSL_UPDATE_ABLE
#define LSL_UPDATE_ABLE

#include "lslCommon.h"

namespace lsl
{

//����� ����������������, ���� ����������� ����� ����� �����, �� ��� ����������� ���������� ������ ��������� �� �������� ���� ����� ����� ��������� ������ ������� � ���� (Begin/End)Update, ���������� �������� ������� ����� ������������ ������ ���� ��� ����� ������ EndUpdate
//� ������ ����������� ����������, ��� ���������� ������� ������������ �� �������� � ������ ������ ������������ ���� � ������
class UpdateAble
{
public:
	struct UpdateData;
private:
	typedef std::map<UpdateAble*, UpdateData*> _UpdateMap;
public:
	//���������: �������� Unite ���� �� ����, ��� � ��� ����� ������
	struct UpdateData 
	{
		static unsigned GetUniqueId();

		virtual ~UpdateData();

		//������� � ���� ������ ���� �������
		virtual UpdateData* Clone() const = 0;
		//�������� �����������
		virtual void Unite(const UpdateData& value) = 0;
		//�������������� ������������� ���� ��� ���������� �������, � ������ ���� ������������� �� sender ����������
		virtual unsigned GetId() const
		{
			return 0;
		}
	};	
private:
	unsigned _updateCount;
	_UpdateMap _updateMap;	
protected:
	//��������� �������� ��������� �� ������� ������������ (�������� � �� ������ ����), ����� �� ����� ����������� �����������(��� ���������) ��������� ����� ����������.
	virtual void DoNotifyChanged(UpdateAble* sender, const UpdateData* data) = 0;
public:
	UpdateAble();

	//����������� �� ��� ����������� �������
	void NotifyChanged(UpdateAble* sender, const UpdateData* data);
	void BeginUpdate();
	void EndUpdate();
	virtual bool IsUpdating() const;
};

class UpdateAbleOwned: public virtual UpdateAble
{
private:
	UpdateAble* _owner;
protected:
	virtual void DoNotifyChanged(UpdateAble* sender, const UpdateData* data);

	UpdateAble* GetOwner();
	void SetOwner(UpdateAble* value);
public:
	UpdateAbleOwned(UpdateAble* owner);
};

}

#endif