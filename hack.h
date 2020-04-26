#include "sdk.h"
#include "Menu.h"
class Esp
{
public:
	static void DrawSkeleton(AActor* pEntity)
	{
		auto pMesh = pEntity->GetMesh();
		auto IsVisible = UnrealEngine4::IsVisible(pEntity);
		if (pMesh) 
		{
			FVector neckpos = pMesh->GetBoneWithRotation(EBoneIndex::neck_01);
			FVector pelvispos = pMesh->GetBoneWithRotation(EBoneIndex::pelvis);
			FVector previous(0, 0, 0);
			FVector current, p1, c1;

			for (auto a : skeleton)
			{
				previous = FVector(0, 0, 0);

				for (int bone : a)
				{
					current = bone == EBoneIndex::neck_01 ? neckpos : (bone == EBoneIndex::pelvis ? pelvispos : pMesh->GetBoneWithRotation(bone));
					if (previous.X == 0.f)
					{
						previous = current;
						continue;
					}
					p1 = UnrealEngine4::WorldToScreen(previous);
					c1 = UnrealEngine4::WorldToScreen(current);			
					Render::DrawLine(p1.X + 1, p1.Y, c1.X + 1, c1.Y, Color::Black());
					Render::DrawLine(p1.X, p1.Y, c1.X, c1.Y, IsVisible ? Color::Green() : Color::Red());
					Render::DrawLine(p1.X - 1, p1.Y, c1.X - 1, c1.Y, Color::Black());
					previous = current;
				}
			}
		}
	}
};
class Hack
{
public:
	static void MainLoop()
	{
		if (UnrealEngine4::IsInGame()) {
			auto pLocal = UnrealEngine4::GetLocalEntity();
			auto pCamera = UnrealEngine4::GetCameraManager();
			if (pLocal)
			{
				// pre loop hacks
				// todo: anticolllision
			}
			auto EntityList = UnrealEngine4::GetEntityList();
			if (EntityList.IsValid()) {
				auto HighestEntityIndex = EntityList.Count();
				for (int EntityIndex = 0; EntityIndex < HighestEntityIndex; EntityIndex++)
				{
					auto pEntity = EntityList[EntityIndex];
					if (!pEntity)
						continue;
					if (pEntity == pLocal)
						continue;
					auto GlobalName = UnrealEngine4::GetGNameByID(pEntity->GetObjectID());
					if (UnrealEngine4::IsPlayer(GlobalName))
					{					
						if (Settings::espskeleton_enable)
							Esp::DrawSkeleton(pEntity);
						// todo: esp, aim
					}
				}
			}
			if (pLocal)
			{
				// post loop hacks/restore
			}
		}
	}
};