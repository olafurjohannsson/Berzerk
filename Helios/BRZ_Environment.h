
#pragma once;
#ifdef BRZ_HEADERTRACE
#pragma message("Header Trace: BRZ_Environment.h")
#endif

#include "BRZ_GameDefine.h"
#include "BRZ_Input.h"

#include "BRZ_Asset.h"
#include "BRZ_BindingModule.h"
#include "BRZ_Config.h"
#include "BRZ_Display.h"
#include "BRZ_InputCore.h"
#include "BRZ_LineObject.h"
#include "BRZ_Time.h"
#include "BRZ_Window.h"

namespace BRZ
{
	class Environment : public BRZ::ActiveObject
	{
	public:
		Environment(std::ofstream & log);
		~Environment();

	public:
		BRZRESULT	Initialize();
		BRZRESULT	Engage();

		BRZRESULT	ResolveInput(BRZ::InputEvent action);

	protected:
		BRZRESULT	LoadAssets();
		BRZRESULT	LoadBindings();

	protected:
		void		Note(const BRZSTRING & message);

	private:
		// Engine core objects:
		std::ofstream &		log;
		BRZ::Window			wnd;
		BRZ::Config			cfg;
		BRZ::Time			time;
		BRZ::Display		vid;
		BRZ::InputCore		input;
		BRZ::BindingModule	envBinds;

		// Engine state objects:
		bool				exit;
		bool				loaded;
		bool				running;

		// Game specific objects:
		BRZ::Scene *		world;
		BRZ::LineObject		grid;
		BRZ::Asset			as_craft;
		BRZ::Asset			as_g_centre;
		BRZ::Asset			as_helper;
		BRZ::Asset			as_rock[10];
	};
}