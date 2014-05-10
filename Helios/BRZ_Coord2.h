
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Coord2.h" )
#endif

#include "BRZ_Define.h"

namespace BRZ
{
	class Coord2
	{
	public:
		Coord2();
		Coord2(signed int x, signed int y);
		Coord2(const Coord2 & ref);
		~Coord2();

	public:
		BRZ::Coord2		Abs() const;
		bool			IsPositive() const;
		bool			IsNegative() const;
		bool			IsMixed() const;
		bool			IsNull() const;

	public:
		const signed int & operator[](unsigned int index) const;
		signed int & operator[](unsigned int index);

		BRZ::Coord2 operator+(const BRZ::Coord2 & ref) const;

	public:
		// DiskStream utility functions:
		BRZRESULT		Unpack(BRZ::DiskStream & inputFile);
		unsigned int	Pack(BRZ::DiskStream & outputFile) const;

	public:
		signed int x;
		signed int y;

	protected:
	private:
		static signed int BAD_INDEX;
	};
}