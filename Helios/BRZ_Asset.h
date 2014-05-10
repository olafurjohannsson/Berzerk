
#pragma once
#ifdef BRZ_HEADERTRACE
#pragma message ( "Header Trace: BRZ_Asset.h" )
#endif

#include "BRZ_GameDefine.h"


namespace BRZ
{
	class Asset
	{
	private:
		class Component;

	public:
		Asset(std::ofstream & log);
		~Asset();

	public:
		// Functions for loading assets and saving:
		BRZRESULT ImportASEObject(const BRZSTRING & file);
		BRZRESULT ImportHeliosAsset(const BRZSTRING & file);
		BRZRESULT ExportHeliosAsset(const BRZSTRING & file);

		// Functions for acquiring asset information:
		const BRZSTRING &	Name() const;
		unsigned int		NumElements() const;
		BRZ::Vec2			ElementPosition(unsigned int index) const;

		// Functions for extracting asset data in usable forms:
		BRZRESULT ExtractGeometry(BRZ::RawGeometry & output) const;
		BRZRESULT ExtractElement(BRZ::RawElement & output, unsigned int index) const;
		BRZRESULT ExtractCollision(BRZ::RigidElement & output, unsigned int index) const;

	protected:
		// Processes an imported (from ASE) component, fixing the local origin, colour,
		//	bounds and points:
		// Note that this method is only invoked when dealing with the data acquired from
		//	the ASEImport class.
		BRZRESULT ProcessComponent(BRZ::Asset::Component & target);

	private:
		std::ofstream &	log;
		bool			loaded;
		BRZSTRING		name;
		Component *		subObjects;
		unsigned int	subCount;


	private:
		class Component
		{
		public:
			Component();
			~Component();

		public:
			BRZRESULT		Unpack(BRZ::DiskStream & inputFile);
			unsigned int	Pack(BRZ::DiskStream & outputFile) const;

		public:
			BRZ::Vec2		localOrigin;
			float			radius;
			float			boundRadius;
			BRZ::Colour		colour;
			bool			isRigid;

			std::vector<BRZ::Vec2>		points;
			std::vector<BRZ::Coord2>	lines;
		};
	};
}