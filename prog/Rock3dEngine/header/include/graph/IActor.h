#ifndef R3D_GRAPH_IACTOR
#define R3D_GRAPH_IACTOR

#include "lslUtility.h"

namespace r3d
{
	
namespace graph
{

class IActor
{
public:
	enum Lighting
	{
		glNone, //��� ���������
		glStd,  //�����. �������
		glPix,  //������������ ���������
		glRefl, //�������� �����
		glBump, //����
		glRefr, //�����������
		glPlanarRefl, //�������� �� ����

		cLightingEnd
	};
	
	enum GraphProp
	{
		gpReflWater = 0, //���������� � ����
		gpReflScene,     //���������� �� �������� �����(������������ ������)
		gpShadowCast,    //������� ����
		gpShadowApp,     //��������� ����		

		gpColor,         //����� � �����

		gpDynamic,       //������������, �������� ������������ � ������������
		gpMorph,         //�����������, �� ����� ���������� ���������, ����������� ����������. �� ��������
		gpCullOpacity,   //���������� ���������� ��� ���������� ���������

		cGraphPropEnd
	};

	enum Order
	{
		goDefault = 0,  //������� �� ����� ��������
		goEffect,       //�������� ��������. ������ ����������, ���� �� �������, ��������� ���������
		goOpacity,      //����������
		goFirst,
		goLast,

		cOrderEnd
	};

	typedef lsl::Bitset<cGraphPropEnd> GraphProps;

	struct GraphDesc
	{
		GraphDesc(): lighting(glStd), props(0), order(goDefault) {props.set(gpColor);}

		GraphDesc(Lighting mLighting, const GraphProps& mProps, const Order& mOrder): lighting(mLighting), props(mProps), order(mOrder) {}

		bool operator==(const GraphDesc& value) const
		{
			return lighting == value.lighting && props == value.props && order == value.order;
		}
		bool operator!=(const GraphDesc& value) const
		{
			return !operator==(value);
		}

		Lighting lighting;
		GraphProps props;
		Order order;
	};
};

static const char* cLightingStr[IActor::cLightingEnd] = {"glNone", "glStd", "glPix", "glRefl", "glBump", "glRefr", "glPlanarRefl"};

static const char* cGraphPropStr[IActor::cGraphPropEnd] = {"gpReflWater", "gpReflScene", "gpShadowCast", "gpShadowApp", "gpColor", "gpDynamic", "gpMorph", "gpCullOpacity"};

static const char* cGraphOrderStr[IActor::cOrderEnd] = {"goDefault", "goOpacity", "goEffect", "goLast"};

}

}

#endif