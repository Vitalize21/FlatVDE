#pragma once

class VitalizeBit
{
public:
#pragma region _Static
	//�w�蕪�r�b�g�����V�t�g�������l��Ԃ�
	static constexpr inline int bitLeftShift(int shiftBitNum) { return (1 << shiftBitNum); }
	//checkBit��bit�������Ă��邩�ǂ���
	static bool bitCheck(int targetBit, int checkBit);
	//targetBit��onBit�̃r�b�g�𗧂Ă����l��Ԃ�
	static void bitOn(int& targetBit, int onBit);
	//targetBit����offBit�̃r�b�g���~�낵�����l��Ԃ�
	static void bitOff(int& targetBit, int offBit);
	//targetBit�̑S�r�b�g�𔽓]����
	static void bitReverse(int& targetBit);

	//checkBit��num�Ԗڂ������Ă��邩�ǂ���
	static bool bitCheckNum(int targetBit, int num);
	//targetBit��num�Ԗڂ𗧂Ă����l��Ԃ�
	static void bitOnNum(int& targetBit, int num);
	//targetBit����num�Ԗڂ̃r�b�g���~�낵�����l��Ԃ�
	static void bitOffNum(int& targetBit, int num);
#pragma endregion

	//checkBit��bit�������Ă��邩�ǂ���
	bool bitCheck(int checkBit) const;

	inline VitalizeBit& operator = (const int& b) { this->bit = b; return *this; }
	inline VitalizeBit& operator = (const VitalizeBit& b) { this->bit = b.bit; return *this; }
	inline VitalizeBit& operator += (const int& b) { bitOn(this->bit, b); return *this; }
	inline VitalizeBit& operator += (const VitalizeBit& b) { bitOn(this->bit, b.bit); return *this; }
	inline VitalizeBit& operator -= (const int& b) { bitOff(this->bit, b); return *this; }
	inline VitalizeBit& operator -= (const VitalizeBit& b) { bitOff(this->bit, b.bit); return *this; }
	inline VitalizeBit& operator + (const int& b) const { VitalizeBit ret = *this; bitOn(ret.bit, b); return ret; }
	inline VitalizeBit& operator + (const VitalizeBit& b) const { VitalizeBit ret = *this; bitOn(ret.bit, b.bit); return ret; }
	inline VitalizeBit& operator - (const int& b) const { VitalizeBit ret = *this; bitOff(ret.bit, b); return ret; }
	inline VitalizeBit& operator - (const VitalizeBit& b) const { VitalizeBit ret = *this; bitOff(ret.bit, b.bit); return ret; }

	inline bool operator == (const int& b) const { return bitCheck(this->bit, b); }
	inline bool operator == (const VitalizeBit& b) const { return bitCheck(this->bit, b.bit); }

private:
	int bit;

public:
	VitalizeBit() : bit(0) {}
};

#define BIT_L_SHIFT(SHIFT) VitalizeBit::bitLeftShift(SHIFT)
#define BIT_CHECK(BIT, NUM) VitalizeBit::bitCheck(BIT, NUM)
#define BIT_ON(BIT, NUM) VitalizeBit::bitOn(BIT, NUM)
#define BIT_OFF(BIT, NUM) VitalizeBit::bitOff(BIT, NUM)
#define BIT_REVERSE(BIT) VitalizeBit::bitReverse(BIT)

#define BIT_CHECK_NUM(BIT, NUM) VitalizeBit::bitCheckNum(BIT, NUM)
#define BIT_ON_NUM(BIT, NUM) VitalizeBit::bitOnNum(BIT, NUM)
#define BIT_OFF_NUM(BIT, NUM) VitalizeBit::bitOffNum(BIT, NUM)

