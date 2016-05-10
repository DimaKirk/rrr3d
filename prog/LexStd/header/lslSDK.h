#pragma once

#include "lslObject.h"
#include "lslContainer.h"
#include "lslUtility.h"
#include <stack>

namespace lsl
{

class ThreadPool: public Object
{
public:
	enum Flag
	{
		//������ �������. ����������� ����������� �������� ��������������� ������ ��� ������ ������, �������� ���� ��� ������� � �������� ���������� ��������� �����. ������ � ����� ������, �������� � ����� ������ ���������� �����������, ������ ��������� � �������. �.�. �� ��������� ��� ������� ���������.
		tfLongFunc = 0,

		//������� �����, ��� ������ ������ ����������� ���������� ��������� �����
		tfBackground,
		//
		cFlagEnd
	};
	typedef lsl::Bitset<cFlagEnd> Flags;
	
	class UserWork: public virtual lsl::Object
	{
	private:
		std::string _name;

		volatile bool _execution;
		volatile bool _terminating;		
	public:
		UserWork(const std::string& name = ""): _name(name), _execution(false), _terminating(false) {}
		
		virtual void Execute(Object* arg) = 0;
		
		const std::string& GetName() const
		{
			return _name;
		}

		virtual void OnTerminate() {}

		//��������� ����������
		void BeginExecution()
		{
			_execution = true;
		}
		void EndExecution()
		{
			OnTerminate();

			_execution = false;
			_terminating = false;
		}
		bool IsExecuting() const
		{
			return _execution;
		}

		//������� �� ����������
		void Terminate()
		{
			_terminating = true;
		}
		//������ ��������, ���������� ������������
		bool IsTerminating() const
		{
			return _terminating;
		}
	};
public:
	virtual void QueueWork(UserWork* work, Object* arg, Flags flags = Flags(0)) = 0;

	virtual unsigned GetMinThreads() = 0;
	virtual void SetMinThreads(unsigned value) = 0;

	virtual unsigned GetMaxThreads() = 0;	
	virtual void SetMaxThreads(unsigned value) = 0;
};

class LockedObj: public lsl::Object 
{
	friend class SDK;
private:
	void* _data;
};

class ThreadEvent: public lsl::Object
{
protected:
	ThreadEvent() {}
	virtual ~ThreadEvent() {}
public:
	//������ � ������� � �������
	virtual bool WaitOne(unsigned mlsTimeOut = INFINITE) = 0;
	//������������� ������ ��� ������
	virtual void Set() = 0;
	//����������(�������) ������ ��� ������
	virtual void Reset() = 0;
};

class Profiler
{
private:
	static Profiler* _i;

	struct SampleData
	{
		float dt;
		float summDt;
		float minDt;
		float maxDt;
		unsigned frames;

		__int64 time;
		bool updated;
	};	
	typedef std::stack<lsl::string> SampleStack;
public:
	typedef std::map<lsl::string, SampleData> Samples;
private:
	Samples _samples;
	SampleStack _stack;
	__int64 _cpuFreq;

	void ResetSample(SampleData& data);
public:
	Profiler();

	void Begin(const lsl::string& name);
	void End();

	void ResetSample(const lsl::string& name);
	const Samples& samples() const;

	static void Init(Profiler* profiler);
	static Profiler& I();
};

class SDK: public lsl::Object
{
protected:
	void* GetDataFrom(LockedObj* obj);
	void SetDataTo(LockedObj* obj, void* data);
public:
	//
	virtual ThreadPool* GetThreadPool() = 0;
	//������������� ���������� ��������� ������. ������ ���� ����� ����� �������� ������ � ���	
	virtual LockedObj* CreateLockedObj() = 0;
	virtual void DestroyLockedObj(LockedObj* value) = 0;
	virtual void Lock(LockedObj* obj) = 0;
	virtual void Unlock(LockedObj* obj) = 0;
	//����������� �������
	//manualReset = true - ����� ���� ����� ��������, ������ ����� ��������� � ������
	//open - ������ �� ������ � ������ �������������
	virtual ThreadEvent* CreateThreadEvent(bool manualReset, bool open, const std::string& name) = 0;
	virtual void DestroyThreadEvent(ThreadEvent* value) = 0;

	virtual float GetTime() = 0;
	virtual double GetTimeDbl() = 0;
};

SDK* GetSDK();
void ReleaseSDK();




inline void SpinWait(DWORD time)
{
	DWORD tick = GetTickCount();
	DWORD newTick = tick;

	while (newTick - tick < time)
	{
		newTick = GetTickCount();
	}
}

}