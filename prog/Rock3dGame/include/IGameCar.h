#ifndef R3D_GAME_IGAMECAR
#define R3D_GAME_IGAMECAR

#include "lslMath.h"
#include <vector>

namespace r3d
{

namespace game
{

class CarMotorDesc
{
public:
	static const int cNeutralGear = -1;
	static const int cBackGear = 0;

	CarMotorDesc();

	//��/���
	float CalcRPM(float wheelAxleSpeed, unsigned curGear) const;
	//��
	float CalcTorque(float rpm, unsigned curGear) const;

	//����. ����� �������� ���������, ��/���
	unsigned maxRPM; 
	//������� ��������� ����, ��/���
	unsigned idlingRPM;
	//����. ������ ��������� ��� maxRPM, �*�
	float maxTorque;
	//system effectiveness measure, ��� �� ����, ����
	float SEM;
	//���������� ����������� ��������
	float gearDiff;
	//��� ������� �������
	bool autoGear;
	//������������ �������
	//[0] - ������
	std::vector<float> gears;
	//��������� ������
	float brakeTorque;
	//������ ������������� �������� (������ �� ���� ������ � �.�.)
	float restTorque;

};

}

}

#endif