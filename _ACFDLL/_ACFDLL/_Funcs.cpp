#include "stdafx.h"
#include "cmath"
#include <vector>
#include <algorithm>
#include "_Funcs.h"

std::vector < double >  F_GenerateLagTimeVect(int v_Tmax, int v_ToA_st = 1, int v_lag_time_min = 3000, int v_B = 2, int v_TauRawLim = 500)
{
	std::vector < double >  v_TauRaw;
	int N = v_lag_time_min / v_ToA_st;
	v_TauRaw.push_back(N);
	double v_TmaxDivN = v_Tmax / 100;
	for (size_t i = 1; v_TauRaw[i] < v_TmaxDivN; ++i)
		v_TauRaw.push_back(v_TauRaw[i - 1] + N * pow(2, ceil((i - 1) / v_B))); // –азложнение
	if (v_TauRawLim > (int)v_TauRaw.size())
		v_TauRaw.erase(v_TauRaw.begin() + v_TauRawLim, v_TauRaw.end()); // ќбрезание
	return v_TauRaw;
}

struct _MyStruct
{
public:
	_MyStruct(double v_lag_time, double v_ACF)
	{
		V_lag_time = v_lag_time;
		V_ACF = v_ACF;
	};
	_MyStruct()
	{
		V_lag_time = 0;
		V_ACF = 0;
	};
	~_MyStruct()
	{

	};
	double Fv_GetTime() const
	{
		return V_lag_time;
	};
	double Fv_GetACF() const
	{
		return V_ACF;
	};
	void Fv_Set(double v_lag_time, double v_ACF)
	{
		V_lag_time = v_lag_time;
		V_ACF = v_ACF;
	};
private:
	double V_lag_time;
	double V_ACF;
};

int F_ACF(const std::vector < double >& v_AT1,const std::vector < double >& v_AT2)
{
	int i = 0, v_return = 0;
	for (size_t j = 0; j < v_AT1.size(); j++)
		if (v_AT1[j] >= v_AT2[i])
		{
		if (v_AT1[j] == v_AT2[i])
			v_return++;
		i++;
		}
	return v_return; // возвращает совпадающие времена
}

std::vector < _MyStruct > F_TTTCU(const std::vector < double >& v_dAT, int v_ToA_st = 1, int v_lag_time_min = 3000)
{
	std::vector < double > v_AT(v_dAT.size());
	v_AT[0] = v_dAT[0];
	for (size_t i = 1; i < v_dAT.size(); ++i)
		v_AT[i] = v_AT[i - 1] + v_dAT[i]; // накопление времени

	std::vector < double > v_Tau = F_GenerateLagTimeVect(
		max_element(v_AT.begin(), v_AT.end()) - v_AT.begin(), // «ачем искать максимум если он в конце???
		v_ToA_st,
		v_lag_time_min);

	std::vector < _MyStruct > v_ACFandTau(v_Tau.size());
	std::vector < double > v_AT2(v_AT.size());
	for (size_t i = 0; i < v_Tau.size(); i++)
	{
		for (size_t j = 0; j < v_AT.size(); j++)
			v_AT2[j] = v_AT[j] + v_Tau[i];
		v_ACFandTau[i].Fv_Set(v_Tau[i], (double)F_ACF(v_AT, v_AT2));
	}
	return v_ACFandTau;
}
