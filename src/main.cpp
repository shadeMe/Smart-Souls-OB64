#include "Internals.h"
#include "Config.h"

namespace Hooks
{

	namespace InventoryChanges_SetBestSoulGem
	{
		AddressT Function{ REL::ID(407667) };
		AddressT Target{ Function.address() + 0xD2 };
		AddressT Return{ Function.address() + 0x3A3 };


		FinalCandidates* Hook_SelectBestSoulGem(InventoryChanges* Changes, TESObjectREFR* DeadActor, FinalCandidates* OutCandidates)
		{
			OutCandidates->BaseContainer = nullptr;
			OutCandidates->Inventory = nullptr;

			if (Changes->pRef == nullptr)
				return OutCandidates;

			REX::INFO("{}", std::string(70, '#'));
			REX::INFO("Soul Trap by '{:08X}' on '{:08X} [{:02X}]'",
			          Changes->pRef->iFormID,
			          DeadActor->iFormID,
					  DeadActor->data.pObjectReference->cFormType);

			auto Out { SelectBestSoulGem(Changes, DeadActor) };
			OutCandidates->BaseContainer = Out.BaseContainer;
			OutCandidates->Inventory = Out.Inventory;
			REX::INFO("{}", std::string(70, '#'));

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
				add(rsp, sizeof(FinalCandidates));

				// Ensure that we take the correct exitpath,
				// i.e., the one that restores non-volatile registers.
				xor_(rax, rax);
				xor_(r13, r13); 

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

			// Nop out the bits that would otherwise overwrite 
			// the outputs of our hook.
			Function.write_fill<0x3D8>(REL::NOP, 3); 
		}
	}

	namespace ItemChange_SetSoul
	{
		AddressT Function{ REL::ID(407674) };

		void Install()
		{
			// Nop out bugged, inverted jump after soul gem dynamic
			// cast check and the soul level comparison.
			Function.write_fill<0x3B>(REL::NOP, 9); 
			// Patch jnz to jz to exit when the dynamic cast fails.
			Function.write<0x44 + 0x1>({0x84});
		}
	}

	static void Install()
	{
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

OBSE_PLUGIN_LOAD(const OBSE::LoadInterface* a_obse)
{
	//while (IsDebuggerPresent() == FALSE)
	//	::Sleep(500);

	OBSE::Init(a_obse, { .trampoline = true, .trampolineSize = 0x100 });
	OBSE::GetMessagingInterface()->RegisterListener(MessageHandler);

	Config::Load();
	Hooks::Install();

	REX::INFO("Load");
	return true;
}
