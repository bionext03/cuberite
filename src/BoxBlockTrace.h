// BoxBlockTracer.h

// Declares the cBoxBlockTracer class that detect collisions in an incompletely solid block



#pragma once

#include "Globals.h"
#include "Defines.h"
#include "World.h"
#include "LineBlockTracer.h"





typedef unsigned char HITBOXTYPE;

struct sHitBox
{
	Vector3d Min;
	Vector3d Max;
};

struct sBlockHitBoxes
{
	const sHitBox *HitBox[4];
};

struct sBits
{
	bool bit[4];
};

enum ENUM_HITBOX_ID : HITBOXTYPE
{
	E_HITBOX_FULLY_SOLID = 0,
	E_HITBOX_SLAB_DOWN = 1,
	E_HITBOX_SLAB_UP = 2,
	E_HITBOX_STAIRS_DOWN_XP_ZM = 3,
	E_HITBOX_STAIRS_DOWN_XP_ZP = 4,
	E_HITBOX_STAIRS_DOWN_XM_ZP = 5,
	E_HITBOX_STAIRS_DOWN_XM_ZM = 6,
	E_HITBOX_STAIRS_UP_XP_ZM = 7,
	E_HITBOX_STAIRS_UP_XP_ZP = 8,
	E_HITBOX_STAIRS_UP_XM_ZP = 9,
	E_HITBOX_STAIRS_UP_XM_ZM = 10,
};

enum
{
	E_HITBOX_XP_ZP = 1,
	E_HITBOX_XP_ZM = 2,
	E_HITBOX_XM_ZP = 4,
	E_HITBOX_XM_ZM = 8

};

const sHitBox HitBoxes[]
{
	{ { 0.0, 0.0, 0.0 },{ 1.0, 1.0, 1.0 } }, // 0) E_HITBOX_FULLY_SOLID
	{ { 0.0, 0.0, 0.0 },{ 1.0, 0.5, 1.0 } }, // 1) E_HITBOX_SLAB_DOWN
	{ { 0.0, 0.5, 0.0 },{ 1.0, 1.0, 1.0 } }, // 2) E_HITBOX_SLAB_UP
	{ { 0.5, 0.5, 0.0 },{ 1.0, 1.0, 0.5 } }, // 3) E_HITBOX_STAIRS_DOWN_XP_ZM
	{ { 0.5, 0.5, 0.5 },{ 1.0, 1.0, 1.0 } }, // 4) E_HITBOX_STAIRS_DOWN_XP_ZP
	{ { 0.0, 0.5, 0.5 },{ 0.5, 1.0, 1.0 } }, // 5) E_HITBOX_STAIRS_DOWN_XM_ZP
	{ { 0.0, 0.5, 0.0 },{ 0.5, 1.0, 0.5 } }, // 6) E_HITBOX_STAIRS_DOWN_XM_ZM
	{ { 0.5, 0.0, 0.0 },{ 1.0, 0.5, 0.5 } }, // 7) E_HITBOX_STAIRS_UP_XP_ZM
	{ { 0.5, 0.0, 0.5 },{ 1.0, 0.5, 1.0 } }, // 8) E_HITBOX_STAIRS_UP_XP_ZP
	{ { 0.0, 0.0, 0.5 },{ 0.5, 0.5, 1.0 } }, // 9) E_HITBOX_STAIRS_UP_XM_ZP
	{ { 0.0, 0.0, 0.0 },{ 0.5, 0.5, 0.5 } }, // 10) E_HITBOX_STAIRS_UP_XM_ZM
};






inline bool IsBlockStairs(BLOCKTYPE a_BlockType)
{
	switch (a_BlockType)
	{
		case E_BLOCK_OAK_WOOD_STAIRS:
		case E_BLOCK_COBBLESTONE_STAIRS:
		case E_BLOCK_BRICK_STAIRS:
		case E_BLOCK_STONE_BRICK_STAIRS:
		case E_BLOCK_NETHER_BRICK_STAIRS:
		case E_BLOCK_SANDSTONE_STAIRS:
		case E_BLOCK_SPRUCE_WOOD_STAIRS:
		case E_BLOCK_BIRCH_WOOD_STAIRS:
		case E_BLOCK_JUNGLE_WOOD_STAIRS:
		case E_BLOCK_QUARTZ_STAIRS:
		case E_BLOCK_ACACIA_WOOD_STAIRS:
		case E_BLOCK_DARK_OAK_WOOD_STAIRS:
		case E_BLOCK_RED_SANDSTONE_STAIRS:
		case E_BLOCK_PURPUR_STAIRS:
		{
			return true;
		}
		default:
		{
			return false;
		}
	}
}


inline sBits ConverMetaToBits(BLOCKTYPE a_BlockMeta)
{
	switch (a_BlockMeta & 0x03)
	{
		case E_BLOCK_STAIRS_XP:
		{
			return{ true, true, false, false };
		}
		case E_BLOCK_STAIRS_XM:
		{
			return{ false, false, true, true };
		}
		case E_BLOCK_STAIRS_ZP:
		{
			return{ false, true, true, false };
		}
		case E_BLOCK_STAIRS_ZM:
		{
			return{ true, false, false, true };
		}
		default:
		{
			return{ false, false, false, false };
		}
	}
	
}


// NIBBLETYPE a_MetaData
inline bool IsBlockNotFullySolid(BLOCKTYPE a_BlockType)
{
	return IsBlockStairs(a_BlockType); //||
}



class cBoxBlockTrace
{
public:
	cBoxBlockTrace();

	bool Trace(
		cWorld & a_World,
		const Vector3d & a_Start,
		const Vector3d & a_End,
		Vector3d & a_HitCoords,
		double a_Height,
		double a_Width,
		bool & a_OnGround
	);
private:
	sBlockHitBoxes GetBlockStairsHitBoxes(cWorld & a_World, Vector3i a_BlockCoords);
	sBlockHitBoxes GetBlockHitBoxes(cWorld & a_World, Vector3i a_BlockCoords);
	
	bool BoxOnGroundTrace(cWorld & a_World, const Vector3d & a_BoxCoords, double a_Width);
	
private:
	int m_DirX;
	int m_DirY;
	int m_DirZ;
	double m_Height;
	double m_Width;
	Vector3d m_Start;
	Vector3d m_End;
	Vector3d m_HitCoords;
	Vector3d m_DirVector;
	Vector3i m_HitBlockCoords;
	sBlockHitBoxes m_BlockHitBoxes;
};