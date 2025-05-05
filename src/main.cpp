#include "Config.h"
#include "Internals.h"

namespace Hooks
{
	using namespace RE;

	namespace InventoryChanges_SetBestSoulGem
	{
		AddressT Function{ REL::Offset(0x6671860) };
		AddressT Target{ Function.address() + 0xA5 };
		AddressT Return{ Function.address() + 0x3DB };


		FinalCandidates* Hook_SelectBestSoulGem(InventoryChanges* Changes, TESObjectREFR* DeadActor, FinalCandidates* OutCandidates)
		{
			OutCandidates->BaseContainer = nullptr;
			OutCandidates->Inventory = nullptr;

			if (Changes->pRef == nullptr)
				return OutCandidates;

			const auto IsNpc{ DeadActor->data.pObjectReference->cFormType == FormType::CREA_ID };

			REX::INFO("Changes owner: {:08X} | DeadActor: {:08X} | IsCreature: {}",
			          Changes->pRef->iFormID,
			          DeadActor->iFormID,
			          !IsNpc);

			return OutCandidates;
		}

		struct Hook : Xbyak::CodeGenerator
		{
			Hook()
			{
				push(rcx);
				push(rdx);
				push(r8);
				push(r9);
				sub(rsp, 0x20);

				sub(rsp, sizeof(FinalCandidates));
				mov(r8, ptr[rsp - sizeof(FinalCandidates)]);
				mov(rdx, rbx);
				mov(rcx, rsi);
				mov(rax, reinterpret_cast<std::uintptr_t>(Hook_SelectBestSoulGem));
				call(rax);
				mov(r12, ptr[rax]);
				mov(rdi, ptr[rax + 0x8]);
				add(rsp, 0x10);

				add(rsp, 0x20);
				pop(r9);
				pop(r8);
				pop(rdx);
				pop(rcx);

				jmp(ptr[rip]);
				dq(Return.address());
			}
		};

		void Install()
		{
			Hook Code;
			Code.ready();

			auto& Trampoline = REL::GetTrampoline();
			Trampoline.write_jmp6(
				Target.address(),
				reinterpret_cast<std::uintptr_t>(Trampoline.allocate(Code)));

		}
	}

	namespace ItemChange_SetSoul
	{
		AddressT Function{ REL::Offset(0x6672790) };

		void Install()
		{
			// Patch bugged, inverted jump after soul gem type check.
			Function.write<0x3B>({0xF});
			// Skip the next check for the soul value.
			Function.write_fill<0x3D>(REL::NOP, 5);
			// Unconditional jump out of the function.
			Function.write<0x44>({0xEB});
		}
	}

	static void Install()
	{
		//REL::GetTrampoline().create(0x100);
		InventoryChanges_SetBestSoulGem::Install();
		ItemChange_SetSoul::Install();

		REX::INFO("Installed hooks");
	}
}

namespace
{
	void MessageHandler(OBSE::MessagingInterface::Message* a_msg)
	{
		switch (a_msg->type)
		{
		case OBSE::MessagingInterface::kPostLoad:
			REX::INFO("PostLoad");
			break;
		case OBSE::MessagingInterface::kPostPostLoad:
			REX::INFO("PostPostLoad");
			break;
		default:
			break;
		}
	}
}

OBSE_PLUGIN_PRELOAD(const OBSE::PreLoadInterface* a_obse)
{
	//while (IsDebuggerPresent() == FALSE)
	//	::Sleep(500);

	OBSE::Init(a_obse);
	REX::INFO("Preload");
	return true;
}

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse)
{
	OBSE::Init(a_obse, { .trampoline = true, .trampolineSize = 0x100 });
	OBSE::GetMessagingInterface()->RegisterListener(MessageHandler);
	Hooks::Install();
	REX::INFO("Load");
	return true;
}
