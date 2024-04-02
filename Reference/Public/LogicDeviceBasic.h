#pragma once

#include "Engine_Defines.h"


BEGIN(Engine)

#pragma region ������

/// <summary>
/// fCur, fMax���� �����ϰ�, ���� ����� ���� ��ü
/// </summary>
class FGauge
{
public:
	explicit FGauge() : fMax(_float()), fCur(_float()), fPrevCur(fCur) {}
	explicit FGauge(_float _fMax, _bool bMax = false) : fMax(_fMax), fCur(_float(_float(bMax)* _float(_fMax))), fPrevCur(fCur) {}
	FGauge(const FGauge& rhs) : fMax(rhs.fMax), fCur(rhs.fCur), fPrevCur(rhs.fPrevCur) {}
	~FGauge() = default;

public:
	_float fMax, fCur;
private:
	_float fPrevCur;


public:
	// �� ������Ʈ �� �ƽ��� ���޽� ��ȯ
	_bool Increase(_float increase)
	{
		fPrevCur = fCur;
		fCur += increase;
		if (fCur >= fMax)
		{
			fCur = fMax;
			return true;
		}
		else if (fCur <= 0.f)
			fCur = 0.f;

		return false;
	}

	_bool Decrease(_float increase)
	{
		fPrevCur = fCur;
		fCur -= increase;
		if (fCur <= 0.f)
		{
			fCur = 0.f;
			return true;
		}
		else if (fCur >= fMax)
			fCur = fMax;

		return false;
	}

	// Ư�� ����Ʈ(����)�� �Ѿ�� ��ȯ
	_bool Increase_UpToPoint(_float increase, _float point)
	{
		fPrevCur = fCur;
		fCur += increase;
		if (fCur >= point)
		{
			fCur = point;
			return true;
		}
		else if (fCur <= 0.f)
			fCur = 0.f;

		return false;
	}

	// Ư�� ����Ʈ(����)�� �Ѿ�� ��ȯ, �ѹ� �����ص� ������ �����Ѵ�.
	_bool Increase_UpToPointOnce(_float increase, _float point)
	{
		fPrevCur = fCur;
		fCur += increase;
		if ((fCur >= point - increase * 0.5f) && (fCur < point + increase * 0.5f))
		{
			if (fCur >= fMax)
			{
				fCur = fMax;
			}
			return true;
		}
		else if (fCur <= 0.f)
			fCur = 0.f;

		return false;
	}

	// ���簪 �ʱ�ȭ
	void Reset()
	{
		fCur = _float();
		fPrevCur = fCur;
	}

	void Reset(_float vResetValue)
	{
		fCur = vResetValue;
		fPrevCur = fCur;
	}

	// fMax �� �缳�� �� ���簪 �ʱ�ȭ
	void Readjust(_float _fMax)
	{
		fMax = _fMax;
		fCur = _float();
	}

	// Ư�� ����Ʈ(����)�� �Ѿ�� ��� Ʈ��
	_bool IsUpTo(_float point)
	{
		return (fCur >= point);
	}

	// �������� üũ�ϰ��� �ϴ� ������ �ѹ��� ������ ���� üũ�մϴ�.
	_bool IsUpTo_Once(_float point, _float increase)
	{
		return ((fCur >= point - increase * 0.5f) && (fCur < point + increase * 0.5f));
	}

	// �ƽ��� ��� ��� Ʈ��
	_bool IsMax()
	{
		return (fCur >= fMax);
	}

	// �ƽ��� ��� �ѹ��� Ʈ��
	_bool IsMax_Once()
	{
		return (fCur >= fMax && fPrevCur != fCur);
	}

	// �ۼ�Ʈ �� ��ȯ
	_float Get_Percent()
	{
		return (static_cast<_float>(fCur) / static_cast<_float>(fMax));
	}
};

using FDelay = FGauge;

#pragma endregion


#pragma region ������

/// <summary>
/// fCur, fMin, fMax���� �����ϰ�
/// �� ���̸� fCur�� �պ��ϴ� ������ Ŭ�����̴�.
/// 
/// </summary>
class FCircuitGauge
{
public:
	explicit FCircuitGauge() : fMax(_float()), fCur(_float()), fPrevCur(fCur) {}
	explicit FCircuitGauge(_float _fMax, _bool bMax = false) : fMax(_fMax), fCur(_float(_float(bMax)* _float(_fMax))), fPrevCur(fCur) {}
	explicit FCircuitGauge(const FCircuitGauge& rhs) : fMax(rhs.fMax), fCur(rhs.fCur), fPrevCur(rhs.fPrevCur) {}
	~FCircuitGauge() = default;

public:
	_float fMin, fMax, fCur;
private:
	_float	fPrevCur;
	_byte	bSign;

public:
	// �� ������Ʈ �� �ƽ��� ���޽� ��ȯ
	_bool Increase(_float increase)
	{
		fPrevCur = fCur;
		fCur += increase * _float(bSign);
		if (fCur >= fMax)
		{
			fCur = fMax;
			return true;
		}

		return false;
	}

	// ���簪 �ʱ�ȭ
	void Reset()
	{
		fCur = _float();
		fPrevCur = fCur;
	}

	// fMax �� �缳�� �� ���簪 �ʱ�ȭ
	void Readjust(_float _fMin, _float _fMax)
	{
		fMin = _fMin;
		fMax = _fMax;
		fCur = _float();
	}

	// Ư�� ����Ʈ(����)�� �Ѿ�� ��� Ʈ��
	_bool IsUpTo(_float point)
	{
		return (fCur >= point);
	}

	// �������� üũ�ϰ��� �ϴ� ������ �ѹ��� ������ ���� üũ�մϴ�.
	_bool IsUpTo_Once(_float point, _float increase)
	{
		return ((fCur >= point - increase * 0.5f) && (fCur < point + increase * 0.5f));
	}

	// �ƽ��� ��� ��� Ʈ��
	_bool IsMax()
	{
		return (fCur >= fMax);
	}

	// �ƽ��� ��� �ѹ��� Ʈ��
	_bool IsMax_Once()
	{
		return (fCur >= fMax && fPrevCur != fCur);
	}

	// �ۼ�Ʈ �� ��ȯ
	_float Get_Percent()
	{
		return (static_cast<_float>(fCur) / static_cast<_float>(fMax));
	}
};

using FDelay = FGauge;

#pragma endregion

END