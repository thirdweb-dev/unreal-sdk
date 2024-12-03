/* 
 * QR Code generator library (C++)
 * 
 * Copyright (c) Project Nayuki. (MIT License)
 * https://www.nayuki.io/page/qr-code-generator-library
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 * - The above copyright notice and this permission notice shall be included in
 *   all copies or substantial portions of the Software.
 * - The Software is provided "as is", without warranty of any kind, express or
 *   implied, including but not limited to the warranties of merchantability,
 *   fitness for a particular purpose and noninfringement. In no event shall the
 *   authors or copyright holders be liable for any claim, damages or other
 *   liability, whether in an action of contract, tort or otherwise, arising from,
 *   out of or in connection with the Software or the use or other dealings in the
 *   Software.
 */

#include "QRCodeGenerator.h"

#include <algorithm>
#include <cassert>
#include <climits>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <sstream>
#include <utility>

#include "ThirdwebLog.h"

using std::int8_t;
using std::uint8_t;
using std::size_t;
using std::vector;

namespace QRCodeGenerator
{
	/*---- Class QRSegment ----*/

	FQRSegment::FMode::FMode(const int Mode, const int CC0, const int CC1, const int CC2) :
		ModeBits(Mode)
	{
		NumBitsCharCount[0] = CC0;
		NumBitsCharCount[1] = CC1;
		NumBitsCharCount[2] = CC2;
	}

	int FQRSegment::FMode::GetModeBits() const
	{
		return ModeBits;
	}

	int FQRSegment::FMode::NumCharCountBits(int Ver) const
	{
		return NumBitsCharCount[(Ver + 7) / 17];
	}

	const FQRSegment::FMode FQRSegment::FMode::Numeric(0x1, 10, 12, 14);
	const FQRSegment::FMode FQRSegment::FMode::Alphanumeric(0x2, 9, 11, 13);
	const FQRSegment::FMode FQRSegment::FMode::Byte(0x4, 8, 16, 16);
	const FQRSegment::FMode FQRSegment::FMode::Kanji(0x8, 8, 10, 12);
	const FQRSegment::FMode FQRSegment::FMode::ECI(0x7, 0, 0, 0);

	FQRSegment FQRSegment::MakeBytes(const vector<uint8_t>& Data)
	{
		if (Data.size() > static_cast<unsigned int>(INT_MAX))
		{
			TW_LOG(Error, TEXT("QRSegment::makeBytes::Data too long"))
			return FQRSegment();
		}
		FBitBuffer BB;
		for (uint8_t b : Data)
			BB.AppendBits(b, 8);
		return FQRSegment(FMode::Byte, static_cast<int>(Data.size()), std::move(BB));
	}

	FQRSegment FQRSegment::MakeNumeric(const char* Digits)
	{
		FBitBuffer BB;
		int AccumData = 0;
		int AccumCount = 0;
		int CharCount = 0;
		for (; *Digits != '\0'; Digits++, CharCount++)
		{
			char c = *Digits;
			if (c < '0' || c > '9')
			{
				TW_LOG(Error, TEXT("QRSegment::makeNumeric::String contains non-numeric characters"));
				return FQRSegment();
			}
			AccumData = AccumData * 10 + (c - '0');
			AccumCount++;
			if (AccumCount == 3)
			{
				BB.AppendBits(static_cast<uint32_t>(AccumData), 10);
				AccumData = 0;
				AccumCount = 0;
			}
		}
		if (AccumCount > 0) // 1 or 2 digits remaining
			BB.AppendBits(static_cast<uint32_t>(AccumData), AccumCount * 3 + 1);
		return FQRSegment(FMode::Numeric, CharCount, std::move(BB));
	}

	FQRSegment FQRSegment::MakeAlphanumeric(const char* Text)
	{
		FBitBuffer BB;
		int AccumData = 0;
		int AccumCount = 0;
		int CharCount = 0;
		for (; *Text != '\0'; Text++, CharCount++)
		{
			const char* Temp = std::strchr(Alphanumeric_Charset, *Text);
			if (Temp == nullptr)
			{
				TW_LOG(Error, TEXT("QRSegment::makeAlphanumeric::String contains unencodable characters in alphanumeric mode"));
				return FQRSegment();
			}
			AccumData = AccumData * 45 + static_cast<int>(Temp - Alphanumeric_Charset);
			AccumCount++;
			if (AccumCount == 2)
			{
				BB.AppendBits(static_cast<uint32_t>(AccumData), 11);
				AccumData = 0;
				AccumCount = 0;
			}
		}
		if (AccumCount > 0) // 1 character remaining
			BB.AppendBits(static_cast<uint32_t>(AccumData), 6);
		return FQRSegment(FMode::Alphanumeric, CharCount, std::move(BB));
	}

	vector<FQRSegment> FQRSegment::MakeSegments(const char* Text)
	{
		// Select the most efficient segment encoding automatically
		vector<FQRSegment> Result;
		if (*Text == '\0')
		{
			// Leave result empty
		}
		else if (IsNumeric(Text))
			Result.push_back(MakeNumeric(Text));
		else if (IsAlphanumeric(Text))
			Result.push_back(MakeAlphanumeric(Text));
		else
		{
			vector<uint8_t> Bytes;
			for (; *Text != '\0'; Text++)
				Bytes.push_back(static_cast<uint8_t>(*Text));
			Result.push_back(MakeBytes(Bytes));
		}
		return Result;
	}

	FQRSegment FQRSegment::MakeEci(long AssignVal)
	{
		FBitBuffer BB;
		if (AssignVal < 0)
		{
			TW_LOG(Error, TEXT("QRSegment::makeEci::ECI assignment value out of range"));
			return FQRSegment();
		}
		if (AssignVal < (1 << 7))
			BB.AppendBits(static_cast<uint32_t>(AssignVal), 8);
		else if (AssignVal < (1 << 14))
		{
			BB.AppendBits(2, 2);
			BB.AppendBits(static_cast<uint32_t>(AssignVal), 14);
		}
		else if (AssignVal < 1000000L)
		{
			BB.AppendBits(6, 3);
			BB.AppendBits(static_cast<uint32_t>(AssignVal), 21);
		}
		else
		{
			TW_LOG(Error, TEXT("QRSegment::makeEci::ECI assignment value out of range"));
			return FQRSegment();
		}
		return FQRSegment(FMode::ECI, 0, std::move(BB));
	}

	FQRSegment::FQRSegment(const FMode& MD, const int NumCh, const std::vector<bool>& DT) :
		Mode(&MD),
		NumChars(NumCh),
		Data(DT)
	{
		if (NumCh < 0)
		{
			TW_LOG(Error, TEXT("QRSegment::QRSegment::Invalid value"))
		}
	}

	FQRSegment::FQRSegment(const FMode& MD, const int NumCh, std::vector<bool>&& DT) :
		Mode(&MD),
		NumChars(NumCh),
		Data(std::move(DT))
	{
		if (NumCh < 0)
		{
			TW_LOG(Error, TEXT("QRSegment::QRSegment::Invalid value"))
		}
	}

	FQRSegment::FQRSegment(): Mode(nullptr), NumChars(0)
	{
	}

	int FQRSegment::GetTotalBits(const vector<FQRSegment>& Segs, const int Version)
	{
		int Result = 0;
		for (const FQRSegment& Seg : Segs)
		{
			int Ccbits = Seg.Mode->NumCharCountBits(Version);
			if (Seg.NumChars >= (1L << Ccbits))
				return -1; // The segment's length doesn't fit the field's bit width
			if (4 + Ccbits > INT_MAX - Result)
				return -1; // The sum will overflow an int type
			Result += 4 + Ccbits;
			if (Seg.Data.size() > static_cast<unsigned int>(INT_MAX - Result))
				return -1; // The sum will overflow an int type
			Result += static_cast<int>(Seg.Data.size());
		}
		return Result;
	}

	bool FQRSegment::IsNumeric(const char* Text)
	{
		for (; *Text != '\0'; Text++)
		{
			if (char c = *Text; c < '0' || c > '9')
				return false;
		}
		return true;
	}

	bool FQRSegment::IsAlphanumeric(const char* Text)
	{
		for (; *Text != '\0'; Text++)
		{
			if (std::strchr(Alphanumeric_Charset, *Text) == nullptr)
				return false;
		}
		return true;
	}

	const FQRSegment::FMode& FQRSegment::GetMode() const
	{
		return *Mode;
	}

	int FQRSegment::GetNumChars() const
	{
		return NumChars;
	}

	const std::vector<bool>& FQRSegment::GetData() const
	{
		return Data;
	}

	const char* FQRSegment::Alphanumeric_Charset = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ $%*+-./:";

	/*---- Class FQRCode ----*/

	int FQRCode::GetFormatBits(const ECC Ecl)
	{
		switch (Ecl)
		{
		case ECC::Low: return 1;
		case ECC::Medium: return 0;
		case ECC::Quartile: return 3;
		case ECC::High: return 2;
		default:
			{
				TW_LOG(Error, TEXT("FQRCode::getFormatBits::Unreachable"))
				return 0;
			};
		}
	}

	FQRCode FQRCode::EncodeText(const char* Text, const ECC Ecl)
	{
		vector<FQRSegment> Segs = FQRSegment::MakeSegments(Text);
		return EncodeSegments(Segs, Ecl);
	}

	FQRCode FQRCode::EncodeBinary(const vector<uint8_t>& Data, const ECC Ecl)
	{
		vector Segs{FQRSegment::MakeBytes(Data)};
		return EncodeSegments(Segs, Ecl);
	}

	FQRCode FQRCode::EncodeSegments(const vector<FQRSegment>& Segs,
	                                ECC Ecl,
	                                const int MinVersion,
	                                const int MaxVersion,
	                                const int Mask,
	                                const bool bBoostEcl)
	{
		if (!(Min_Version <= MinVersion && MinVersion <= MaxVersion && MaxVersion <= Max_Version) || Mask < -1 || Mask > 7)
		{
			TW_LOG(Error, TEXT("FQRCode::encodeSegments::Invalid value"))
			return FQRCode();
		}

		// Find the minimal version number to use
		int Version, DataUsedBits;
		for (Version = MinVersion; ; Version++)
		{
			int DataCapacityBits = GetNumDataCodewords(Version, Ecl) * 8; // Number of data bits available
			DataUsedBits = FQRSegment::GetTotalBits(Segs, Version);
			if (DataUsedBits != -1 && DataUsedBits <= DataCapacityBits)
				break; // This version number is found to be suitable
			if (Version >= MaxVersion)
			{
				// All versions in the range could not fit the given data
				std::ostringstream SB;
				if (DataUsedBits == -1)
					SB << "Segment too long";
				else
				{
					SB << "Data length = " << DataUsedBits << " bits, ";
					SB << "Max capacity = " << DataCapacityBits << " bits";
				}
				TW_LOG(Error, TEXT("FQRCode::encodeSegments::Invalid size"))
				return FQRCode();
			}
		}
		assert(dataUsedBits != -1);

		// Increase the error correction level while the data still fits in the current version number
		for (ECC NewEcl : {ECC::Medium, ECC::Quartile, ECC::High})
		{
			// From low to high
			if (bBoostEcl && DataUsedBits <= GetNumDataCodewords(Version, NewEcl) * 8)
				Ecl = NewEcl;
		}

		// Concatenate all segments to create the data bit string
		FBitBuffer BB;
		for (const FQRSegment& Seg : Segs)
		{
			BB.AppendBits(static_cast<uint32_t>(Seg.GetMode().GetModeBits()), 4);
			BB.AppendBits(static_cast<uint32_t>(Seg.GetNumChars()), Seg.GetMode().NumCharCountBits(Version));
			BB.insert(BB.end(), Seg.GetData().begin(), Seg.GetData().end());
		}
		assert(bb.size() == static_cast<unsigned int>(dataUsedBits));

		// Add terminator and pad up to a byte if applicable
		size_t DataCapacityBits = static_cast<size_t>(GetNumDataCodewords(Version, Ecl)) * 8;
		assert(bb.size() <= dataCapacityBits);
		BB.AppendBits(0, std::min(4, static_cast<int>(DataCapacityBits - BB.size())));
		BB.AppendBits(0, (8 - static_cast<int>(BB.size() % 8)) % 8);
		assert(bb.size() % 8 == 0);

		// Pad with alternating bytes until data capacity is reached
		for (uint8_t PadByte = 0xEC; BB.size() < DataCapacityBits; PadByte ^= 0xEC ^ 0x11)
			BB.AppendBits(PadByte, 8);

		// Pack bits into bytes in big endian
		vector<uint8_t> DataCodewords(BB.size() / 8);
		for (size_t i = 0; i < BB.size(); i++)
			DataCodewords.at(i >> 3) |= (BB.at(i) ? 1 : 0) << (7 - (i & 7));

		// Create the QR Code object
		return FQRCode(Version, Ecl, DataCodewords, Mask);
	}

	FQRCode::FQRCode(const int Ver, const ECC Ecl, const vector<uint8_t>& DataCodewords, int Msk) :
		// Initialize fields and check arguments
		Version(Ver),
		ErrorCorrectionLevel(Ecl)
	{
		if (Ver < Min_Version || Ver > Max_Version)
		{
			TW_LOG(Error, TEXT("FQRCode::FQRCode::Min Version out of range"))
			return;
		}
		if (Msk < -1 || Msk > 7)
		{
			TW_LOG(Error, TEXT("FQRCode::applyMask::Mask value out of range"))
			return;
		}
		Size = Ver * 4 + 17;
		size_t Sz = static_cast<size_t>(Size);
		Modules = vector(Sz, vector<bool>(Sz)); // Initially all light
		IsFunction = vector(Sz, vector<bool>(Sz));

		// Compute ECC, draw modules
		DrawFunctionPatterns();
		const vector<uint8_t> AllCodewords = AddEccAndInterleave(DataCodewords);
		DrawCodewords(AllCodewords);

		// Do masking
		if (Msk == -1)
		{
			// Automatically choose best mask
			long MinPenalty = LONG_MAX;
			for (int i = 0; i < 8; i++)
			{
				ApplyMask(i);
				DrawFormatBits(i);
				if (long Penalty = GetPenaltyScore(); Penalty < MinPenalty)
				{
					Msk = i;
					MinPenalty = Penalty;
				}
				ApplyMask(i); // Undoes the mask due to XOR
			}
		}
		assert(0 <= msk && msk <= 7);
		Mask = Msk;
		ApplyMask(Msk); // Apply the final choice of mask
		DrawFormatBits(Msk); // Overwrite old format bits

		IsFunction.clear();
		IsFunction.shrink_to_fit();
	}

	FQRCode::FQRCode(): Version(0), Size(0), ErrorCorrectionLevel(), Mask(0)
	{
	}

	int FQRCode::GetVersion() const
	{
		return Version;
	}

	int FQRCode::GetSize() const
	{
		return Size;
	}

	FQRCode::ECC FQRCode::GetErrorCorrectionLevel() const
	{
		return ErrorCorrectionLevel;
	}

	int FQRCode::GetMask() const
	{
		return Mask;
	}

	bool FQRCode::GetModule(const int X, const int Y) const
	{
		return 0 <= X && X < Size && 0 <= Y && Y < Size && Module(X, Y);
	}

	void FQRCode::DrawFunctionPatterns()
	{
		// Draw horizontal and vertical timing patterns
		for (int i = 0; i < Size; i++)
		{
			SetFunctionModule(6, i, i % 2 == 0);
			SetFunctionModule(i, 6, i % 2 == 0);
		}

		// Draw 3 finder patterns (all corners except bottom right; overwrites some timing modules)
		DrawFinderPattern(3, 3);
		DrawFinderPattern(Size - 4, 3);
		DrawFinderPattern(3, Size - 4);

		// Draw numerous alignment patterns
		const vector<int> AlignPatPos = GetAlignmentPatternPositions();
		size_t NumAlign = AlignPatPos.size();
		for (size_t i = 0; i < NumAlign; i++)
		{
			for (size_t j = 0; j < NumAlign; j++)
			{
				// Don't draw, on the three finder corners
				if (!((i == 0 && j == 0) || (i == 0 && j == NumAlign - 1) || (i == NumAlign - 1 && j == 0)))
					DrawAlignmentPattern(AlignPatPos.at(i), AlignPatPos.at(j));
			}
		}

		// Draw configuration data
		DrawFormatBits(0); // Dummy mask value; overwritten later in the constructor
		DrawVersion();
	}

	void FQRCode::DrawFormatBits(int Msk)
	{
		// Calculate error correction code and pack bits
		int Data = GetFormatBits(ErrorCorrectionLevel) << 3 | Msk; // errCorrLvl is uint2, msk is uint3
		int Rem = Data;
		for (int i = 0; i < 10; i++)
			Rem = (Rem << 1) ^ ((Rem >> 9) * 0x537);
		int Bits = (Data << 10 | Rem) ^ 0x5412; // uint15
		assert(bits >> 15 == 0);

		// Draw first copy
		for (int i = 0; i <= 5; i++)
			SetFunctionModule(8, i, GetBit(Bits, i));
		SetFunctionModule(8, 7, GetBit(Bits, 6));
		SetFunctionModule(8, 8, GetBit(Bits, 7));
		SetFunctionModule(7, 8, GetBit(Bits, 8));
		for (int i = 9; i < 15; i++)
			SetFunctionModule(14 - i, 8, GetBit(Bits, i));

		// Draw second copy
		for (int i = 0; i < 8; i++)
			SetFunctionModule(Size - 1 - i, 8, GetBit(Bits, i));
		for (int i = 8; i < 15; i++)
			SetFunctionModule(8, Size - 15 + i, GetBit(Bits, i));
		SetFunctionModule(8, Size - 8, true); // Always dark
	}

	void FQRCode::DrawVersion()
	{
		if (Version < 7)
			return;

		// Calculate error correction code and pack bits
		int Rem = Version; // version is uint6, in the range [7, 40]
		for (int i = 0; i < 12; i++)
			Rem = (Rem << 1) ^ ((Rem >> 11) * 0x1F25);
		long Bits = static_cast<long>(Version) << 12 | Rem; // uint18
		assert(bits >> 18 == 0);

		// Draw two copies
		for (int i = 0; i < 18; i++)
		{
			bool bBIt = GetBit(Bits, i);
			int a = Size - 11 + i % 3;
			int b = i / 3;
			SetFunctionModule(a, b, bBIt);
			SetFunctionModule(b, a, bBIt);
		}
	}

	void FQRCode::DrawFinderPattern(int X, int Y)
	{
		for (int Dy = -4; Dy <= 4; Dy++)
		{
			for (int DX = -4; DX <= 4; DX++)
			{
				int Dist = std::max(std::abs(DX), std::abs(Dy)); // Chebyshev/infinity norm
				int Xx = X + DX;
				if (int Yy = Y + Dy; 0 <= Xx && Xx < Size && 0 <= Yy && Yy < Size)
					SetFunctionModule(Xx, Yy, Dist != 2 && Dist != 4);
			}
		}
	}

	void FQRCode::DrawAlignmentPattern(int X, int Y)
	{
		for (int Dy = -2; Dy <= 2; Dy++)
		{
			for (int DX = -2; DX <= 2; DX++)
				SetFunctionModule(X + DX, Y + Dy, std::max(std::abs(DX), std::abs(Dy)) != 1);
		}
	}

	void FQRCode::SetFunctionModule(int X, int Y, bool bIsDark)
	{
		size_t Ux = static_cast<size_t>(X);
		size_t Uy = static_cast<size_t>(Y);
		Modules.at(Uy).at(Ux) = bIsDark;
		IsFunction.at(Uy).at(Ux) = true;
	}

	bool FQRCode::Module(int x, int y) const
	{
		return Modules.at(static_cast<size_t>(y)).at(static_cast<size_t>(x));
	}

	vector<uint8_t> FQRCode::AddEccAndInterleave(const vector<uint8_t>& Data) const
	{
		if (Data.size() != static_cast<unsigned int>(GetNumDataCodewords(Version, ErrorCorrectionLevel)))
		{
			TW_LOG(Error, TEXT("FQRCode::AddEccAndInterleave::Invalid argument"))
			return {};
		}

		// Calculate parameter numbers
		int NumBlocks = Num_Error_Correction_Blocks[static_cast<int>(ErrorCorrectionLevel)][Version];
		int BlockEccLen = Ecc_Codewords_Per_Block[static_cast<int>(ErrorCorrectionLevel)][Version];
		int RawCodewords = GetNumRawDataModules(Version) / 8;
		int NumShortBlocks = NumBlocks - RawCodewords % NumBlocks;
		int ShortBlockLen = RawCodewords / NumBlocks;

		// Split data into blocks and append ECC to each block
		vector<vector<uint8_t>> Blocks;
		const vector<uint8_t> RsDiv = ReedSolomonComputeDivisor(BlockEccLen);
		for (int i = 0, k = 0; i < NumBlocks; i++)
		{
			vector Dat(Data.cbegin() + k, Data.cbegin() + (k + ShortBlockLen - BlockEccLen + (i < NumShortBlocks ? 0 : 1)));
			k += static_cast<int>(Dat.size());
			const vector<uint8_t> Ecc = ReedSolomonComputeRemainder(Dat, RsDiv);
			if (i < NumShortBlocks)
				Dat.push_back(0);
			Dat.insert(Dat.end(), Ecc.cbegin(), Ecc.cend());
			Blocks.push_back(std::move(Dat));
		}

		// Interleave (not concatenate) the bytes from every block into a single sequence
		vector<uint8_t> Result;
		for (size_t i = 0; i < Blocks.at(0).size(); i++)
		{
			for (size_t j = 0; j < Blocks.size(); j++)
			{
				// Skip the padding byte in short blocks
				if (i != static_cast<unsigned int>(ShortBlockLen - BlockEccLen) || j >= static_cast<unsigned int>(NumShortBlocks))
					Result.push_back(Blocks.at(j).at(i));
			}
		}
		assert(result.size() == static_cast<unsigned int>(rawCodewords));
		return Result;
	}

	void FQRCode::DrawCodewords(const vector<uint8_t>& Data)
	{
		if (Data.size() != static_cast<unsigned int>(GetNumRawDataModules(Version) / 8))
		{
			TW_LOG(Error, TEXT("FQRCode::AddEccAndInterleave::Invalid argument"))
			return;
		}

		size_t i = 0; // Bit index into the data
		// Do the funny zigzag scan
		for (int Right = Size - 1; Right >= 1; Right -= 2)
		{
			// Index of right column in each column pair
			if (Right == 6)
				Right = 5;
			for (int Vert = 0; Vert < Size; Vert++)
			{
				// Vertical counter
				for (int j = 0; j < 2; j++)
				{
					size_t x = static_cast<size_t>(Right - j); // Actual x coordinate
					bool bUpward = ((Right + 1) & 2) == 0;
					size_t y = static_cast<size_t>(bUpward ? Size - 1 - Vert : Vert); // Actual y coordinate
					if (!IsFunction.at(y).at(x) && i < Data.size() * 8)
					{
						Modules.at(y).at(x) = GetBit(Data.at(i >> 3), 7 - static_cast<int>(i & 7));
						i++;
					}
					// If this QR Code has any remainder bits (0 to 7), they were assigned as
					// 0/false/light by the constructor and are left unchanged by this method
				}
			}
		}
		assert(i == data.size() * 8);
	}

	void FQRCode::ApplyMask(const int Msk)
	{
		if (Msk < 0 || Msk > 7)
		{
			TW_LOG(Error, TEXT("FQRCode::applyMask::Mask value out of range"))
			return;
		}

		size_t Sz = static_cast<size_t>(Size);
		for (size_t y = 0; y < Sz; y++)
		{
			for (size_t x = 0; x < Sz; x++)
			{
				bool bInvert;
				switch (Msk)
				{
				case 0: bInvert = (x + y) % 2 == 0;
					break;
				case 1: bInvert = y % 2 == 0;
					break;
				case 2: bInvert = x % 3 == 0;
					break;
				case 3: bInvert = (x + y) % 3 == 0;
					break;
				case 4: bInvert = (x / 3 + y / 2) % 2 == 0;
					break;
				case 5: bInvert = x * y % 2 + x * y % 3 == 0;
					break;
				case 6: bInvert = (x * y % 2 + x * y % 3) % 2 == 0;
					break;
				case 7: bInvert = ((x + y) % 2 + x * y % 3) % 2 == 0;
					break;
				default:
					{
						TW_LOG(Error, TEXT("FQRCode::applyMask::Unreachable"))
						return;
					};
				}
				Modules.at(y).at(x) = Modules.at(y).at(x) ^ (bInvert & !IsFunction.at(y).at(x));
			}
		}
	}

	long FQRCode::GetPenaltyScore() const
	{
		long Result = 0;

		// Adjacent modules in row having same color, and finder-like patterns
		for (int y = 0; y < Size; y++)
		{
			bool bRunColor = false;
			int RunX = 0;
			std::array<int, 7> RunHistory = {};
			for (int x = 0; x < Size; x++)
			{
				if (Module(x, y) == bRunColor)
				{
					RunX++;
					if (RunX == 5)
						Result += Penalty_N1;
					else if (RunX > 5)
						Result++;
				}
				else
				{
					FinderPenaltyAddHistory(RunX, RunHistory);
					if (!bRunColor)
						Result += FinderPenaltyCountPatterns(RunHistory) * Penalty_N3;
					bRunColor = Module(x, y);
					RunX = 1;
				}
			}
			Result += FinderPenaltyTerminateAndCount(bRunColor, RunX, RunHistory) * Penalty_N3;
		}
		// Adjacent modules in column having same color, and finder-like patterns
		for (int x = 0; x < Size; x++)
		{
			bool bRunColor = false;
			int RunY = 0;
			std::array<int, 7> RunHistory = {};
			for (int y = 0; y < Size; y++)
			{
				if (Module(x, y) == bRunColor)
				{
					RunY++;
					if (RunY == 5)
						Result += Penalty_N1;
					else if (RunY > 5)
						Result++;
				}
				else
				{
					FinderPenaltyAddHistory(RunY, RunHistory);
					if (!bRunColor)
						Result += FinderPenaltyCountPatterns(RunHistory) * Penalty_N3;
					bRunColor = Module(x, y);
					RunY = 1;
				}
			}
			Result += FinderPenaltyTerminateAndCount(bRunColor, RunY, RunHistory) * Penalty_N3;
		}

		// 2*2 blocks of modules having same color
		for (int y = 0; y < Size - 1; y++)
		{
			for (int x = 0; x < Size - 1; x++)
			{
				if (bool bColor = Module(x, y); bColor == Module(x + 1, y) &&
					bColor == Module(x, y + 1) &&
					bColor == Module(x + 1, y + 1))
					Result += Penalty_N2;
			}
		}

		// Balance of dark and light modules
		int Dark = 0;
		for (const vector<bool>& Row : Modules)
		{
			for (bool bColor : Row)
			{
				if (bColor)
					Dark++;
			}
		}
		int Total = Size * Size; // Note that size is odd, so dark/total != 1/2
		// Compute the smallest integer k >= 0 such that (45-5k)% <= dark/total <= (55+5k)%
		int k = static_cast<int>((std::abs(Dark * 20L - Total * 10L) + Total - 1) / Total) - 1;
		assert(0 <= k && k <= 9);
		Result += k * Penalty_N4;
		assert(0 <= result && result <= 2568888L); // Non-tight upper bound based on default values of PENALTY_N1, ..., N4
		return Result;
	}

	vector<int> FQRCode::GetAlignmentPatternPositions() const
	{
		if (Version == 1)
		{
			return vector<int>();
		}
		int NumAlign = Version / 7 + 2;
		int Step = (Version * 8 + NumAlign * 3 + 5) / (NumAlign * 4 - 4) * 2;
		vector<int> Result;
		for (int i = 0, Pos = Size - 7; i < NumAlign - 1; i++, Pos -= Step)
			Result.insert(Result.begin(), Pos);
		Result.insert(Result.begin(), 6);
		return Result;
	}

	int FQRCode::GetNumRawDataModules(const int Ver)
	{
		if (Ver < Min_Version || Ver > Max_Version)
		{
			TW_LOG(Error, TEXT("FQRCode::getNumRawDataModules::Version number out of range"))
			return Min_Version;
		}
		int Result = (16 * Ver + 128) * Ver + 64;
		if (Ver >= 2)
		{
			int NumAlign = Ver / 7 + 2;
			Result -= (25 * NumAlign - 10) * NumAlign - 55;
			if (Ver >= 7)
				Result -= 36;
		}
		assert(208 <= result && result <= 29648);
		return Result;
	}

	int FQRCode::GetNumDataCodewords(const int Ver, ECC Ecl)
	{
		return GetNumRawDataModules(Ver) / 8
			- Ecc_Codewords_Per_Block[static_cast<int>(Ecl)][Ver]
			* Num_Error_Correction_Blocks[static_cast<int>(Ecl)][Ver];
	}

	vector<uint8_t> FQRCode::ReedSolomonComputeDivisor(const int Degree)
	{
		if (Degree < 1 || Degree > 255)
		{
			TW_LOG(Error, TEXT("FQRCode::reedSolomonComputeDivisor::Degree out of range"))
			return {};
		}
		// Polynomial coefficients are stored from highest to lowest power, excluding the leading term which is always 1.
		// For example the polynomial x^3 + 255x^2 + 8x + 93 is stored as the uint8 array {255, 8, 93}.
		vector<uint8_t> Result(static_cast<size_t>(Degree));
		Result.at(Result.size() - 1) = 1; // Start off with the monomial x^0

		// Compute the product polynomial (x - r^0) * (x - r^1) * (x - r^2) * ... * (x - r^{degree-1}),
		// and drop the highest monomial term which is always 1x^degree.
		// Note that r = 0x02, which is a generator element of this field GF(2^8/0x11D).
		uint8_t Root = 1;
		for (int i = 0; i < Degree; i++)
		{
			// Multiply the current product by (x - r^i)
			for (size_t j = 0; j < Result.size(); j++)
			{
				Result.at(j) = ReedSolomonMultiply(Result.at(j), Root);
				if (j + 1 < Result.size())
					Result.at(j) ^= Result.at(j + 1);
			}
			Root = ReedSolomonMultiply(Root, 0x02);
		}
		return Result;
	}

	vector<uint8_t> FQRCode::ReedSolomonComputeRemainder(const vector<uint8_t>& Data, const vector<uint8_t>& Divisor)
	{
		vector<uint8_t> Result(Divisor.size());
		for (uint8_t b : Data)
		{
			// Polynomial division
			uint8_t Factor = b ^ Result.at(0);
			Result.erase(Result.begin());
			Result.push_back(0);
			for (size_t i = 0; i < Result.size(); i++)
				Result.at(i) ^= ReedSolomonMultiply(Divisor.at(i), Factor);
		}
		return Result;
	}

	uint8_t FQRCode::ReedSolomonMultiply(const uint8_t X, const uint8_t Y)
	{
		// Russian peasant multiplication
		int z = 0;
		for (int i = 7; i >= 0; i--)
		{
			z = (z << 1) ^ ((z >> 7) * 0x11D);
			z ^= ((Y >> i) & 1) * X;
		}
		assert(z >> 8 == 0);
		return static_cast<uint8_t>(z);
	}

	int FQRCode::FinderPenaltyCountPatterns(const std::array<int, 7>& RunHistory)
	{
		int n = RunHistory.at(1);
		assert(n <= size * 3);
		bool bCore = n > 0 && RunHistory.at(2) == n && RunHistory.at(3) == n * 3 && RunHistory.at(4) == n && RunHistory.at(5) == n;
		return (bCore && RunHistory.at(0) >= n * 4 && RunHistory.at(6) >= n ? 1 : 0)
			+ (bCore && RunHistory.at(6) >= n * 4 && RunHistory.at(0) >= n ? 1 : 0);
	}

	int FQRCode::FinderPenaltyTerminateAndCount(const bool bCurrentRunColor, int CurrentRunLength, std::array<int, 7>& RunHistory) const
	{
		if (bCurrentRunColor)
		{
			// Terminate dark run
			FinderPenaltyAddHistory(CurrentRunLength, RunHistory);
			CurrentRunLength = 0;
		}
		CurrentRunLength += Size; // Add light border to final run
		FinderPenaltyAddHistory(CurrentRunLength, RunHistory);
		return FinderPenaltyCountPatterns(RunHistory);
	}

	void FQRCode::FinderPenaltyAddHistory(int CurrentRunLength, std::array<int, 7>& RunHistory) const
	{
		if (RunHistory.at(0) == 0)
			CurrentRunLength += Size; // Add light border to initial run
		std::copy_backward(RunHistory.cbegin(), RunHistory.cend() - 1, RunHistory.end());
		RunHistory.at(0) = CurrentRunLength;
	}

	bool FQRCode::GetBit(const long X, const int I)
	{
		return ((X >> I) & 1) != 0;
	}

	/*---- Tables of constants ----*/

	const int FQRCode::Penalty_N1 = 3;
	const int FQRCode::Penalty_N2 = 3;
	const int FQRCode::Penalty_N3 = 40;
	const int FQRCode::Penalty_N4 = 10;

	const int8_t FQRCode::Ecc_Codewords_Per_Block[4][41] = {
		// Version: (note that index 0 is for padding, and is set to an illegal value)
		//0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
		{-1, 7, 10, 15, 20, 26, 18, 20, 24, 30, 18, 20, 24, 26, 30, 22, 24, 28, 30, 28, 28, 28, 28, 30, 30, 26, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}, // Low
		{-1, 10, 16, 26, 18, 24, 16, 18, 22, 22, 26, 30, 22, 22, 24, 24, 28, 28, 26, 26, 26, 26, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28}, // Medium
		{-1, 13, 22, 18, 26, 18, 24, 18, 22, 20, 24, 28, 26, 24, 20, 30, 24, 28, 28, 26, 30, 28, 30, 30, 30, 30, 28, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}, // Quartile
		{-1, 17, 28, 22, 16, 22, 28, 26, 26, 24, 28, 24, 28, 22, 24, 24, 30, 28, 28, 26, 28, 30, 24, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30}, // High
	};

	const int8_t FQRCode::Num_Error_Correction_Blocks[4][41] = {
		// Version: (note that index 0 is for padding, and is set to an illegal value)
		//0, 1, 2, 3, 4, 5, 6, 7, 8, 9,10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40    Error correction level
		{-1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 4, 4, 4, 4, 4, 6, 6, 6, 6, 7, 8, 8, 9, 9, 10, 12, 12, 12, 13, 14, 15, 16, 17, 18, 19, 19, 20, 21, 22, 24, 25}, // Low
		{-1, 1, 1, 1, 2, 2, 4, 4, 4, 5, 5, 5, 8, 9, 9, 10, 10, 11, 13, 14, 16, 17, 17, 18, 20, 21, 23, 25, 26, 28, 29, 31, 33, 35, 37, 38, 40, 43, 45, 47, 49}, // Medium
		{-1, 1, 1, 2, 2, 4, 4, 6, 6, 8, 8, 8, 10, 12, 16, 12, 17, 16, 18, 21, 20, 23, 23, 25, 27, 29, 34, 34, 35, 38, 40, 43, 45, 48, 51, 53, 56, 59, 62, 65, 68}, // Quartile
		{-1, 1, 1, 2, 4, 4, 4, 5, 6, 8, 8, 11, 11, 16, 16, 18, 16, 19, 21, 25, 25, 25, 34, 30, 32, 35, 37, 40, 42, 45, 48, 51, 54, 57, 60, 63, 66, 70, 74, 77, 81}, // High
	};

	FData_Too_Long::FData_Too_Long(const std::string& Msg) :
		std::length_error(Msg)
	{
	}

	/*---- Class BitBuffer ----*/

	FBitBuffer::FBitBuffer()
	{
	}

	void FBitBuffer::AppendBits(const std::uint32_t Val, const int Len)
	{
		if (Len < 0 || Len > 31 || Val >> Len != 0)
		{
			TW_LOG(Error, TEXT("BitBuffer::appendBits::Value out of range"))
			return;
		}
		for (int i = Len - 1; i >= 0; i--) // Append bit by bit
			this->push_back(((Val >> i) & 1) != 0);
	}
}
