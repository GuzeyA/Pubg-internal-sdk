#pragma once
#include "xorstr.h"
#include "math.h"
#include <list>
#include "memory.h"

address64 Address_UWorld;
address64 Address_LocalPlayer;
address64 Address_GNames;
address64 Address_Decrypt;
address64 Address_LineOfSightTo;

offset64 Offset_Chunk;
offset64 Offset_Level;
offset64 Offset_Actors;
offset64 Offset_PlayerController;
offset64 Offset_LocalPawn;
offset64 Offset_ObjectID;
offset64 Offset_PlayerCameraManager;
offset64 Offset_Health;
offset64 Offset_TeamID;
offset64 Offset_Mesh;
offset64 Offset_BoneArray;
offset64 Offset_ComponentToWorld;
offset64 Offset_CameraLocation;
offset64 Offset_CameraRotation;
offset64 Offset_CameraFov;
offset64 Offset_LineOfSightTo;

address64 decryptkey;
typedef int64_t(__fastcall* Decryptfn)(address64 key, address64 addr);
Decryptfn oDecrypt = NULL;
class Decrypt
{
public:
	static void Init()
	{
		address64 GameBaseAddress = (address64)GetModuleHandleA(NULL);
		address64 DecryptPtr = *reinterpret_cast<address64*>(GameBaseAddress + Address_Decrypt);
		if (DecryptPtr) {
			int32_t Tmp1Add = *reinterpret_cast<uint32_t*>(DecryptPtr + 3);
			if (Tmp1Add) {
				decryptkey = Tmp1Add + DecryptPtr + 7;
				unsigned char ShellcodeBuff[1024] = { NULL };
				ShellcodeBuff[0] = 0x90;
				ShellcodeBuff[1] = 0x90;
				memcpy(&ShellcodeBuff[0x2], (PVOID)DecryptPtr, sizeof(ShellcodeBuff) - 2);
				ShellcodeBuff[2] = 0x48;
				ShellcodeBuff[3] = 0x8B;
				ShellcodeBuff[4] = 0xC1;
				ShellcodeBuff[5] = 0x90;
				ShellcodeBuff[6] = 0x90;
				ShellcodeBuff[7] = 0x90;
				ShellcodeBuff[8] = 0x90;
				oDecrypt = (Decryptfn)VirtualAlloc(NULL, sizeof(ShellcodeBuff) + 4, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
				RtlCopyMemory((LPVOID)oDecrypt, (LPVOID)ShellcodeBuff, sizeof(ShellcodeBuff));
			}
		}
	}
	static int64_t DecryptData(address64 encrypted)
	{
		return oDecrypt(decryptkey, encrypted);
	}
	static DWORD DecryptIndex(DWORD value)
	{
		DWORD v10 = _rotl(value ^ 0x0689121a, 0x1);
		DWORD result = v10 ^ (v10 << 0x10) ^ 0xb91c28fc;
		return result;
	}
};
class Offsets
{
public:
	static void Init()
	{
		auto MainInstance = reinterpret_cast<address64>(GetModuleHandleA(nullptr));
		Address_UWorld = Memory::GetOffset(MainInstance, Memory::FindPattern(MainInstance, 0x7FFFFFFF, (BYTE*)"\x48\x8B\x0D\x00\x00\x00\x00\x75\x14", "xxx????xx") + 0x3);
		Address_LocalPlayer = Memory::GetOffset(MainInstance, Memory::FindPattern(MainInstance, 0x7FFFFFFF, (BYTE*)"\x48\x89\x3D\x00\x00\x00\x00\xF2\x0F\x10\x05", "xxx????xxxx") + 0x3);
		Address_GNames = Memory::GetOffset(MainInstance, Memory::FindPattern(MainInstance, 0x7FFFFFFF, (BYTE*)"\x48\x8B\x15\x00\x00\x00\x00\xB9\x00\x00\x00\x00\xFF\x15\x00\x00\x00\x00\x48\x8B\x0D", "xxx????x????xx????xxx") + 0x3);
		Address_Decrypt = Memory::GetOffset(MainInstance, Memory::FindPattern(MainInstance, 0x7FFFFFFF, (BYTE*)"\xFF\x15\x00\x00\x00\x00\x48\x83\x3D\x00\x00\x00\x00\x00\x75\x17", "xx????xxx?????xx") + 0x2);
		Address_LineOfSightTo = Memory::FindPattern(MainInstance, 0x7FFFFFFF, (BYTE*)"\x40\x53\x56\x57\x41\x54\x41\x55\x41\x56\x41\x57\x48\x81\xEC\x00\x00\x00\x00\x48\xC7\x44\x24\x00\x00\x00\x00\x00\x0F", "xxxxxxxxxxxxxxx????xxxx?????x");

		Offset_Chunk = 0x416C;
		Offset_Level = 0x0200;
		Offset_Actors = 0x01F0;
		Offset_PlayerController = 0x0030;
		Offset_LocalPawn = 0x0478;
		Offset_ObjectID = 0x0010;
		Offset_PlayerCameraManager = 0x0498;
		Offset_Health = 0xF34;
		Offset_TeamID = 0x0ED0;
		Offset_Mesh = 0x0518;
		Offset_BoneArray = 0x0AC0;
		Offset_ComponentToWorld = 0x270;
		Offset_CameraLocation = 0x15F4;
		Offset_CameraRotation = 0x1074;
		Offset_CameraFov = 0x1054;
		Offset_LineOfSightTo = 0x680;
	}
};
#pragma section DirectXSdk
typedef struct _D3DMATRIX {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;

		};
		float m[4][4];
	};
} D3DMATRIX;
typedef struct D3DXFLOAT16 {
	WORD Value;
} D3DXFLOAT16, * LPD3DXFLOAT16;

typedef struct _D3DMATRIX D3DXMATRIX, * LPD3DXMATRIX;
#pragma end

#pragma section PubgSdk
class UWorld;
class ULevel;
class AActor;
class ULocalPlayer;
class UPlayerController;
class UComponentMesh;
class UPlayerCameraManager;
struct FQuat
{
	float x;
	float y;
	float z;
	float w;
};
struct FTransform
{
	FQuat rot;
	FVector translation;
	char pad[4];
	FVector scale;
	char pad1[4];

	D3DMATRIX ToMatrixWithScale()
	{
		D3DMATRIX m;
		m._41 = translation.X;
		m._42 = translation.Y;
		m._43 = translation.Z;

		float x2 = rot.x + rot.x;
		float y2 = rot.y + rot.y;
		float z2 = rot.z + rot.z;

		float xx2 = rot.x * x2;
		float yy2 = rot.y * y2;
		float zz2 = rot.z * z2;
		m._11 = (1.0f - (yy2 + zz2)) * scale.X;
		m._22 = (1.0f - (xx2 + zz2)) * scale.Y;
		m._33 = (1.0f - (xx2 + yy2)) * scale.Z;

		float yz2 = rot.y * z2;
		float wx2 = rot.w * x2;
		m._32 = (yz2 - wx2) * scale.Z;
		m._23 = (yz2 + wx2) * scale.Y;

		float xy2 = rot.x * y2;
		float wz2 = rot.w * z2;
		m._21 = (xy2 - wz2) * scale.Y;
		m._12 = (xy2 + wz2) * scale.X;

		float xz2 = rot.x * z2;
		float wy2 = rot.w * y2;
		m._31 = (xz2 + wy2) * scale.Z;
		m._13 = (xz2 - wy2) * scale.X;

		m._14 = 0.0f;
		m._24 = 0.0f;
		m._34 = 0.0f;
		m._44 = 1.0f;

		return m;
	}
};
D3DXMATRIX Matrix(FVector rot, FVector origin = FVector(0, 0, 0))
{
	float radPitch = (rot.X * float(PI) / 180.f);
	float radYaw = (rot.Y * float(PI) / 180.f);
	float radRoll = (rot.Z * float(PI) / 180.f);

	float SP = sinf(radPitch);
	float CP = cosf(radPitch);
	float SY = sinf(radYaw);
	float CY = cosf(radYaw);
	float SR = sinf(radRoll);
	float CR = cosf(radRoll);

	D3DMATRIX matrix;
	matrix.m[0][0] = CP * CY;
	matrix.m[0][1] = CP * SY;
	matrix.m[0][2] = SP;
	matrix.m[0][3] = 0.f;

	matrix.m[1][0] = SR * SP * CY - CR * SY;
	matrix.m[1][1] = SR * SP * SY + CR * CY;
	matrix.m[1][2] = -SR * CP;
	matrix.m[1][3] = 0.f;

	matrix.m[2][0] = -(CR * SP * CY + SR * SY);
	matrix.m[2][1] = CY * SR - CR * SP * SY;
	matrix.m[2][2] = CR * CP;
	matrix.m[2][3] = 0.f;

	matrix.m[3][0] = origin.X;
	matrix.m[3][1] = origin.Y;
	matrix.m[3][2] = origin.Z;
	matrix.m[3][3] = 1.f;

	return matrix;
}
D3DMATRIX MatrixMultiplication(D3DMATRIX pM1, D3DMATRIX pM2)
{
	D3DMATRIX pOut;
	pOut._11 = pM1._11 * pM2._11 + pM1._12 * pM2._21 + pM1._13 * pM2._31 + pM1._14 * pM2._41;
	pOut._12 = pM1._11 * pM2._12 + pM1._12 * pM2._22 + pM1._13 * pM2._32 + pM1._14 * pM2._42;
	pOut._13 = pM1._11 * pM2._13 + pM1._12 * pM2._23 + pM1._13 * pM2._33 + pM1._14 * pM2._43;
	pOut._14 = pM1._11 * pM2._14 + pM1._12 * pM2._24 + pM1._13 * pM2._34 + pM1._14 * pM2._44;
	pOut._21 = pM1._21 * pM2._11 + pM1._22 * pM2._21 + pM1._23 * pM2._31 + pM1._24 * pM2._41;
	pOut._22 = pM1._21 * pM2._12 + pM1._22 * pM2._22 + pM1._23 * pM2._32 + pM1._24 * pM2._42;
	pOut._23 = pM1._21 * pM2._13 + pM1._22 * pM2._23 + pM1._23 * pM2._33 + pM1._24 * pM2._43;
	pOut._24 = pM1._21 * pM2._14 + pM1._22 * pM2._24 + pM1._23 * pM2._34 + pM1._24 * pM2._44;
	pOut._31 = pM1._31 * pM2._11 + pM1._32 * pM2._21 + pM1._33 * pM2._31 + pM1._34 * pM2._41;
	pOut._32 = pM1._31 * pM2._12 + pM1._32 * pM2._22 + pM1._33 * pM2._32 + pM1._34 * pM2._42;
	pOut._33 = pM1._31 * pM2._13 + pM1._32 * pM2._23 + pM1._33 * pM2._33 + pM1._34 * pM2._43;
	pOut._34 = pM1._31 * pM2._14 + pM1._32 * pM2._24 + pM1._33 * pM2._34 + pM1._34 * pM2._44;
	pOut._41 = pM1._41 * pM2._11 + pM1._42 * pM2._21 + pM1._43 * pM2._31 + pM1._44 * pM2._41;
	pOut._42 = pM1._41 * pM2._12 + pM1._42 * pM2._22 + pM1._43 * pM2._32 + pM1._44 * pM2._42;
	pOut._43 = pM1._41 * pM2._13 + pM1._42 * pM2._23 + pM1._43 * pM2._33 + pM1._44 * pM2._43;
	pOut._44 = pM1._41 * pM2._14 + pM1._42 * pM2._24 + pM1._43 * pM2._34 + pM1._44 * pM2._44;

	return pOut;
}
enum EBoneIndex
{
	root = 0,
	pelvis = 1,
	spine_01 = 2,
	spine_02 = 3,
	spine_03 = 4,
	neck_01 = 5,
	Head = 6,
	face_root = 7,
	eyebrows_pos_root = 8,
	eyebrows_root = 9,
	eyebrows_r = 10,
	eyebrows_l = 11,
	eyebrow_l = 12,
	eyebrow_r = 13,
	forehead_root = 14,
	forehead = 15,
	jaw_pos_root = 16,
	jaw_root = 17,
	jaw = 18,
	mouth_down_pos_root = 19,
	mouth_down_root = 20,
	lip_bm_01 = 21,
	lip_bm_02 = 22,
	lip_br = 23,
	lip_bl = 24,
	jaw_01 = 25,
	jaw_02 = 26,
	cheek_pos_root = 27,
	cheek_root = 28,
	cheek_r = 29,
	cheek_l = 30,
	nose_side_root = 31,
	nose_side_r_01 = 32,
	nose_side_r_02 = 33,
	nose_side_l_01 = 34,
	nose_side_l_02 = 35,
	eye_pos_r_root = 36,
	eye_r_root = 37,
	eye_rot_r_root = 38,
	eye_lid_u_r = 39,
	eye_r = 40,
	eye_lid_b_r = 41,
	eye_pos_l_root = 42,
	eye_l_root = 43,
	eye_rot_l_root = 44,
	eye_lid_u_l = 45,
	eye_l = 46,
	eye_lid_b_l = 47,
	nose_pos_root = 48,
	nose = 49,
	mouth_up_pos_root = 50,
	mouth_up_root = 51,
	lip_ul = 52,
	lip_um_01 = 53,
	lip_um_02 = 54,
	lip_ur = 55,
	lip_l = 56,
	lip_r = 57,
	hair_root = 58,
	hair_b_01 = 59,
	hair_b_02 = 60,
	hair_l_01 = 61,
	hair_l_02 = 62,
	hair_r_01 = 63,
	hair_r_02 = 64,
	hair_f_02 = 65,
	hair_f_01 = 66,
	hair_b_pt_01 = 67,
	hair_b_pt_02 = 68,
	hair_b_pt_03 = 69,
	hair_b_pt_04 = 70,
	hair_b_pt_05 = 71,
	hair_l_pt_01 = 72,
	hair_l_pt_02 = 73,
	hair_l_pt_03 = 74,
	hair_l_pt_04 = 75,
	hair_l_pt_05 = 76,
	hair_r_pt_01 = 77,
	hair_r_pt_02 = 78,
	hair_r_pt_03 = 79,
	hair_r_pt_04 = 80,
	hair_r_pt_05 = 81,
	camera_fpp = 82,
	GunReferencePoint = 83,
	GunRef = 84,
	breast_l = 85,
	breast_r = 86,
	clavicle_l = 87,
	upperarm_l = 88,
	lowerarm_l = 89,
	hand_l = 90,
	thumb_01_l = 91,
	thumb_02_l = 92,
	thumb_03_l = 93,
	thumb_04_l_MBONLY = 94,
	index_01_l = 95,
	index_02_l = 96,
	index_03_l = 97,
	index_04_l_MBONLY = 98,
	middle_01_l = 99,
	middle_02_l = 100,
	middle_03_l = 101,
	middle_04_l_MBONLY = 102,
	ring_01_l = 103,
	ring_02_l = 104,
	ring_03_l = 105,
	ring_04_l_MBONLY = 106,
	pinky_01_l = 107,
	pinky_02_l = 108,
	pinky_03_l = 109,
	pinky_04_l_MBONLY = 110,
	item_l = 111,
	lowerarm_twist_01_l = 112,
	upperarm_twist_01_l = 113,
	clavicle_r = 114,
	upperarm_r = 115,
	lowerarm_r = 116,
	hand_r = 117,
	thumb_01_r = 118,
	thumb_02_r = 119,
	thumb_03_r = 120,
	thumb_04_r_MBONLY = 121,
	index_01_r = 122,
	index_02_r = 123,
	index_03_r = 124,
	index_04_r_MBONLY = 125,
	middle_01_r = 126,
	middle_02_r = 127,
	middle_03_r = 128,
	middle_04_r_MBONLY = 129,
	ring_01_r = 130,
	ring_02_r = 131,
	ring_03_r = 132,
	ring_04_r_MBONLY = 133,
	pinky_01_r = 134,
	pinky_02_r = 135,
	pinky_03_r = 136,
	pinky_04_r_MBONLY = 137,
	item_r = 138,
	lowerarm_twist_01_r = 139,
	upperarm_twist_01_r = 140,
	Backpack = 141,
	backpack_01 = 142,
	backpack_02 = 143,
	slot_primary = 144,
	slot_secondary = 145,
	slot_melee = 146,
	slot_throwable = 147,
	coat_l_01 = 148,
	coat_l_02 = 149,
	coat_l_03 = 150,
	coat_l_04 = 151,
	coat_fl_01 = 152,
	coat_fl_02 = 153,
	coat_fl_03 = 154,
	coat_fl_04 = 155,
	coat_b_01 = 156,
	coat_b_02 = 157,
	coat_b_03 = 158,
	coat_b_04 = 159,
	coat_r_01 = 160,
	coat_r_02 = 161,
	coat_r_03 = 162,
	coat_r_04 = 163,
	coat_fr_01 = 164,
	coat_fr_02 = 165,
	coat_fr_03 = 166,
	coat_fr_04 = 167,
	thigh_l = 168,
	calf_l = 169,
	foot_l = 170,
	ball_l = 171,
	calf_twist_01_l = 172,
	thigh_twist_01_l = 173,
	thigh_r = 174,
	calf_r = 175,
	foot_r = 176,
	ball_r = 177,
	calf_twist_01_r = 178,
	thigh_twist_01_r = 179,
	slot_sidearm = 180,
	skirt_l_01 = 181,
	skirt_l_02 = 182,
	skirt_l_03 = 183,
	skirt_f_01 = 184,
	skirt_f_02 = 185,
	skirt_f_03 = 186,
	skirt_b_01 = 187,
	skirt_b_02 = 188,
	skirt_b_03 = 189,
	skirt_r_01 = 190,
	skirt_r_02 = 191,
	skirt_r_03 = 192,
	ik_hand_root = 193,
	ik_hand_gun = 194,
	ik_hand_r = 195,
	ik_hand_l = 196,
	ik_aim_root = 197,
	ik_aim_l = 198,
	ik_aim_r = 199,
	ik_foot_root = 200,
	ik_foot_l = 201,
	ik_foot_r = 202,
	camera_tpp = 203,
	ik_target_root = 204,
	ik_target_l = 205,
	ik_target_r = 206,
	VB_spine_03_spine_03 = 207,
	VB_upperarm_r_lowerarm_r = 208,
	VB_GunRef_spine_03 = 209,
	VB_camera_fpp_hand_l = 210,
	VB_camera_fpp_hand_r = 211,
	VB_camera_fpp_clavicle_l = 212,
	VB_camera_fpp_clavicle_r = 213,
	VB_lowerarm_l_hand_l = 214,
};

std::list<int> upper_part = { EBoneIndex::neck_01, EBoneIndex::Head, EBoneIndex::forehead };
std::list<int> right_arm = { EBoneIndex::neck_01, EBoneIndex::upperarm_r, EBoneIndex::lowerarm_r, EBoneIndex::hand_r };
std::list<int> left_arm = { EBoneIndex::neck_01, EBoneIndex::upperarm_l, EBoneIndex::lowerarm_l, EBoneIndex::hand_l };
std::list<int> spine = { EBoneIndex::neck_01, EBoneIndex::spine_01, EBoneIndex::spine_02, EBoneIndex::pelvis };
std::list<int> lower_right = { EBoneIndex::pelvis, EBoneIndex::thigh_r, EBoneIndex::calf_r, EBoneIndex::foot_r };
std::list<int> lower_left = { EBoneIndex::pelvis, EBoneIndex::thigh_l, EBoneIndex::calf_l, EBoneIndex::foot_l };
std::list<std::list<int>> skeleton = { upper_part, right_arm, left_arm, spine, lower_right, lower_left };
template <class T>
struct PubgArray
{
public:
	inline PubgArray()
	{
		Data = nullptr;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline int Count()
	{
		return count;
	}

	inline const T& operator[](int i) const
	{
		return Data[i];
	};
	bool IsValid()
	{
		return (Data != NULL && count > 0 && max > 0);
	}
private:
	T* Data;
	int count;
	int max;
};
class UPlayerCameraManager
{
public:
	FVector GetCameraLocation()
	{
		return *Memory::Ptr<FVector*>(this, Offset_CameraLocation);
	}
	FVector GetCameraRotation()
	{
		return *Memory::Ptr<FVector*>(this, Offset_CameraRotation);
	}
	float GetFov()
	{
		return *Memory::Ptr<float*>(this, Offset_CameraFov);
	}
	FVector WorldToScreen(FVector WorldLocation)
	{
		FVector Screenlocation = FVector(0, 0, 0);

		FVector Rotation = GetCameraRotation();
		D3DMATRIX tempMatrix = Matrix(Rotation);

		FVector vAxisX, vAxisY, vAxisZ;

		vAxisX = FVector(tempMatrix.m[0][0], tempMatrix.m[0][1], tempMatrix.m[0][2]);
		vAxisY = FVector(tempMatrix.m[1][0], tempMatrix.m[1][1], tempMatrix.m[1][2]);
		vAxisZ = FVector(tempMatrix.m[2][0], tempMatrix.m[2][1], tempMatrix.m[2][2]);

		FVector vDelta = WorldLocation - GetCameraLocation();
		FVector vTransformed = FVector(vDelta.Dot(vAxisY), vDelta.Dot(vAxisZ), vDelta.Dot(vAxisX));

		if (vTransformed.Z < 1.f)
			vTransformed.Z = 1.f;

		Vec2 displaysize = renderer->GetDisplaySize();

		float FovAngle = GetFov();
		float ScreenCenterX = displaysize.x / 2;
		float ScreenCenterY = displaysize.y / 2;

		Screenlocation.X = ScreenCenterX + vTransformed.X * (ScreenCenterX / tanf(FovAngle * (float)PI / 360.f)) / vTransformed.Z;
		Screenlocation.Y = ScreenCenterY - vTransformed.Y * (ScreenCenterX / tanf(FovAngle * (float)PI / 360.f)) / vTransformed.Z;

		return Screenlocation;
	}
};
class UComponentMesh
{
public:
	FTransform GetBoneIndex(int index)
	{
		address64 bonearray = *Memory::Ptr<address64*>(this, Offset_BoneArray);
		if (Memory::IsValidPtr(bonearray))
			return *Memory::Ptr<FTransform*>(bonearray, (index * 0x30));
		return FTransform();
	}
	FVector GetBoneWithRotation(int id)
	{
		FTransform bone = GetBoneIndex(id);
		FTransform ComponentToWorld = *Memory::Ptr<FTransform*>(this, Offset_ComponentToWorld);
		D3DMATRIX Matrix;
		Matrix = MatrixMultiplication(bone.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());
		return FVector(Matrix._41, Matrix._42, Matrix._43);
	}
};
class AActor
{
public:
	DWORD GetObjectID()
	{
		return Decrypt::DecryptIndex(GetEncryptedObjectID());
	}
	int GetTeamId()
	{
		return *Memory::Ptr<int*>(this, Offset_TeamID);
	}
	UComponentMesh* GetMesh()
	{
		return *Memory::Ptr<UComponentMesh**>(this, Offset_Mesh);
	}
private:
	uint32_t GetEncryptedObjectID()
	{
		return *Memory::Ptr<uint32_t*>(this, Offset_ObjectID);
	}
};
class ULevel
{
public:
	PubgArray<AActor*> GetEntityList()
	{
		auto enlist = Decrypt::DecryptData(GetEncryptedEntityList());
		if (Memory::IsValidPtr(enlist))
			return *(PubgArray<AActor*>*)enlist;
		return PubgArray<AActor*>();
	}
private:
	address64 GetEncryptedEntityList()
	{
		return *Memory::Ptr<address64*>(this, Offset_Actors);
	}
};
class UWorld
{
public:
	ULevel* GetLevel()
	{
		auto Ulevel = Decrypt::DecryptData(GetEncryptedLevel());
		if (Memory::IsValidPtr(Ulevel))
			return (ULevel*)Ulevel;
		return nullptr;
	}
private:
	address64 GetEncryptedLevel()
	{
		return *Memory::Ptr<address64*>(this, Offset_Level);
	}
};
class UPlayerController
{
public:
	AActor* GetLocalPawn()
	{
		auto LocalPawn = Decrypt::DecryptData(GetEncryptedLocalPawn());
		if (Memory::IsValidPtr(LocalPawn))
			return (AActor*)LocalPawn;
		return nullptr;
	}
	UPlayerCameraManager* GetPlayerCameraManager()
	{
		return *Memory::Ptr<UPlayerCameraManager**>(this, Offset_PlayerCameraManager);
	}
	bool IsVisible(AActor* target)
	{
		// you can use this

		FVector fillVec = FVector(0, 0, 0);
		auto result = (*(int(__fastcall**)(UPlayerController*, AActor*, FVector*, bool))(*(address64*)this + Offset_LineOfSightTo))(this, target, &fillVec, false);
		return result;

		// or you can use this, not really matter

		//FVector fillVec = FVector(0, 0, 0);
		//auto result = ((int(__fastcall*)(UPlayerController*, AActor*, FVector*, bool))(Address_LineOfSightTo))(this, target, &fillVec, false);
		//return result;
	}
private:
	address64 GetEncryptedLocalPawn()
	{
		return *Memory::Ptr<address64*>(this, Offset_LocalPawn);
	}
};
class ULocalPlayer
{
public:
	UPlayerController* GetPlayerController()
	{
		auto PlayerController = Decrypt::DecryptData(GetEncryptedPlayerController());
		if (Memory::IsValidPtr(PlayerController))
			return (UPlayerController*)PlayerController;
		return nullptr;
	}
private:
	address64 GetEncryptedPlayerController()
	{
		return *Memory::Ptr<address64*>(this, Offset_PlayerController);
	}
};
class UnrealEngine4
{
public:
	static UWorld* GetWorld()
	{
		auto Uworld = Decrypt::DecryptData(GetEncryptedWorld());
		if (Memory::IsValidPtr(Uworld))
			return (UWorld*)Uworld;
		return nullptr;
	}
	static ULocalPlayer* GetLocalPlayer()
	{
		address64 GameBaseAddress = (address64)GetModuleHandleA(NULL);
		return *Memory::Ptr<ULocalPlayer**>(GameBaseAddress, Address_LocalPlayer);
	}
	static AActor* GetLocalEntity()
	{
		auto LocalPlayer = GetLocalPlayer();
		if (LocalPlayer)
		{
			auto PlayerController = LocalPlayer->GetPlayerController();
			if (PlayerController)
			{
				return PlayerController->GetLocalPawn();
			}
		}
		return nullptr;
	}
	static UPlayerCameraManager* GetCameraManager()
	{
		auto LocalPlayer = GetLocalPlayer();
		if (LocalPlayer)
		{
			auto PlayerController = LocalPlayer->GetPlayerController();
			if (PlayerController)
			{
				return PlayerController->GetPlayerCameraManager();
			}
		}
		return nullptr;
	}
	static bool IsVisible(AActor* Entity)
	{
		auto LocalPlayer = GetLocalPlayer();
		if (LocalPlayer)
		{
			auto PlayerController = LocalPlayer->GetPlayerController();
			if (PlayerController)
			{
				return PlayerController->IsVisible(Entity);
			}
		}
		return false;
	}
	static PubgArray<AActor*> GetEntityList()
	{
		auto World = GetWorld();
		if (World)
		{
			auto Level = World->GetLevel();
			if (Level)
			{
				return Level->GetEntityList();
			}
		}
		return PubgArray<AActor*>();
	}
	static FVector WorldToScreen(FVector WorldPos)
	{
		auto CameraManager = GetCameraManager();
		if (CameraManager) {
			return CameraManager->WorldToScreen(WorldPos);
		}
		return FVector();
	}
	static std::string GetGNameByID(int32_t ObjectID)
	{
		auto GNamesAddress = GetGNamesAddress();
		int64_t fNamePtr = *Memory::Ptr<address64*>(GNamesAddress, int(ObjectID / Offset_Chunk) * 0x8);
		if (fNamePtr) {
			int64_t fName = *Memory::Ptr<address64*>(fNamePtr, int(ObjectID % Offset_Chunk) * 0x8);
			if (fName) {
				char pBuffer[64] = { NULL };
				memcpy(&pBuffer, (PVOID)(fName + 0x10), sizeof(pBuffer));
				return std::string(pBuffer);
			}
		}
		return std::string();
	}
	static bool IsPlayer(std::string GName)
	{
		if (GName == xorstr("PlayerFemale_A") || GName == xorstr("PlayerFemale_A_C") || GName == xorstr("PlayerMale_A") || GName == xorstr("PlayerMale_A_C"))
			return true;
		return false;
	}
	static bool IsInGame() {
		return !InLobby();
	}
	static bool InLobby()
	{
		auto EntityList = GetEntityList();
		if (EntityList.IsValid()) {
			auto HighestEntityIndex = EntityList.Count();
			for (int EntityIndex = 0; EntityIndex < HighestEntityIndex; EntityIndex++)
			{
				auto pEntity = EntityList[EntityIndex];
				if (!pEntity)
					continue;
				auto GlobalName = UnrealEngine4::GetGNameByID(pEntity->GetObjectID());
				if (GlobalName.find(xorstr("Lobby")) != std::string::npos)
					return true;
			}
		}
		return false;
	}
private:
	static address64 GetEncryptedWorld()
	{
		address64 GameBaseAddress = (address64)GetModuleHandleA(NULL);
		return *Memory::Ptr<address64*>(GameBaseAddress, Address_UWorld);
	}
	static address64 GetGNamesAddress()
	{
		address64 GameBaseAddress = (address64)GetModuleHandleA(NULL);
		return Decrypt::DecryptData(*Memory::Ptr<address64*>(Decrypt::DecryptData(*Memory::Ptr<address64*>(GameBaseAddress, Address_GNames - 0x20)), 0x0));
	}
};
#pragma end