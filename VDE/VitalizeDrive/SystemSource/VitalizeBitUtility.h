#pragma once

class VitalizeBit
{
public:
#pragma region _Static
	//指定分ビットを左シフトした数値を返す
	static constexpr inline int bitLeftShift(int shiftBitNum) { return (1 << shiftBitNum); }
	//checkBitにbitが立っているかどうか
	static bool bitCheck(int targetBit, int checkBit);
	//targetBitにonBitのビットを立てた数値を返す
	static void bitOn(int& targetBit, int onBit);
	//targetBitからoffBitのビットを降ろした数値を返す
	static void bitOff(int& targetBit, int offBit);
	//targetBitの全ビットを反転する
	static void bitReverse(int& targetBit);

	//checkBitのnum番目が立っているかどうか
	static bool bitCheckNum(int targetBit, int num);
	//targetBitのnum番目を立てた数値を返す
	static void bitOnNum(int& targetBit, int num);
	//targetBitからnum番目のビットを降ろした数値を返す
	static void bitOffNum(int& targetBit, int num);
#pragma endregion

	//checkBitにbitが立っているかどうか
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

