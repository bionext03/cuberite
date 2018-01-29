
#include "BoxBlockTrace.h"





cBoxBlockTrace::cBoxBlockTrace() :
	m_DirX(0),
	m_DirY(0),
	m_DirZ(0),
	m_Height(0.0),
	m_Width(0.0),
	m_Start(0.0, 0.0, 0.0),
	m_End(0.0, 0.0, 0.0)
{
}



sBlockHitBoxes cBoxBlockTrace::GetBlockStairsHitBoxes(cWorld & a_World, Vector3i a_BlockCoords)
{
	NIBBLETYPE BackBlockType, BackBlockMeta;
	NIBBLETYPE FrontBlockType, FrontBlockMeta;
	NIBBLETYPE BlockMeta = a_World.GetBlockMeta(a_BlockCoords);
	
	int b0, b1, b2, b3;
	switch (BlockMeta & 0x03)
	{
		case E_BLOCK_STAIRS_XP:
		{
			b0 = 0;
			b1 = 1;
			b2 = 2;
			b3 = 3;
			BackBlockType = a_World.GetBlock(a_BlockCoords.x + 1, a_BlockCoords.y, a_BlockCoords.z);
			FrontBlockType = a_World.GetBlock(a_BlockCoords.x - 1, a_BlockCoords.y, a_BlockCoords.z);
			BackBlockMeta = a_World.GetBlockMeta(a_BlockCoords.x + 1, a_BlockCoords.y, a_BlockCoords.z);
			FrontBlockMeta = a_World.GetBlockMeta(a_BlockCoords.x - 1, a_BlockCoords.y, a_BlockCoords.z);
			break;
		}
		case E_BLOCK_STAIRS_XM:
		{
			b0 = 2;
			b1 = 3;
			b2 = 0;
			b3 = 1;
			BackBlockType = a_World.GetBlock(a_BlockCoords.x - 1, a_BlockCoords.y, a_BlockCoords.z);
			FrontBlockType = a_World.GetBlock(a_BlockCoords.x + 1, a_BlockCoords.y, a_BlockCoords.z);
			BackBlockMeta = a_World.GetBlockMeta(a_BlockCoords.x - 1, a_BlockCoords.y, a_BlockCoords.z);
			FrontBlockMeta = a_World.GetBlockMeta(a_BlockCoords.x + 1, a_BlockCoords.y, a_BlockCoords.z);
			break;
		}
		case E_BLOCK_STAIRS_ZP:
		{
			b0 = 1;
			b1 = 2;
			b2 = 3;
			b3 = 0;
			BackBlockType = a_World.GetBlock(a_BlockCoords.x, a_BlockCoords.y, a_BlockCoords.z + 1);
			FrontBlockType = a_World.GetBlock(a_BlockCoords.x, a_BlockCoords.y, a_BlockCoords.z - 1);
			BackBlockMeta = a_World.GetBlockMeta(a_BlockCoords.x, a_BlockCoords.y, a_BlockCoords.z + 1);
			FrontBlockMeta = a_World.GetBlockMeta(a_BlockCoords.x, a_BlockCoords.y, a_BlockCoords.z - 1);
			break;
		}
		case E_BLOCK_STAIRS_ZM:
		{
			b0 = 3;
			b1 = 0;
			b2 = 1;
			b3 = 2;
			BackBlockType = a_World.GetBlock(a_BlockCoords.x, a_BlockCoords.y, a_BlockCoords.z - 1);
			FrontBlockType = a_World.GetBlock(a_BlockCoords.x, a_BlockCoords.y, a_BlockCoords.z + 1);
			BackBlockMeta = a_World.GetBlockMeta(a_BlockCoords.x, a_BlockCoords.y, a_BlockCoords.z - 1);
			FrontBlockMeta = a_World.GetBlockMeta(a_BlockCoords.x, a_BlockCoords.y, a_BlockCoords.z + 1);
			break;
		}
	}

	bool UpSideDown = BlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN;
	sBits MetaBits = ConverMetaToBits(BlockMeta);
	
	if (IsBlockStairs(BackBlockType) && (UpSideDown == (BackBlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN)))
	{
		sBits BackMetaBits = ConverMetaToBits(BackBlockMeta);
		MetaBits.bit[b0] &= (BackMetaBits.bit[b3] | BackMetaBits.bit[b0]);
		MetaBits.bit[b1] &= (BackMetaBits.bit[b2] | BackMetaBits.bit[b1]);
	}
	else if (IsBlockStairs(FrontBlockType) && (UpSideDown == (FrontBlockMeta & E_BLOCK_STAIRS_UPSIDE_DOWN)))
	{
		sBits FrontMetaBits = ConverMetaToBits(FrontBlockMeta);
		MetaBits.bit[b3] |= (FrontMetaBits.bit[b0] & FrontMetaBits.bit[b3]);
		MetaBits.bit[b2] |= (FrontMetaBits.bit[b1] & FrontMetaBits.bit[b2]);
	}

	sBlockHitBoxes BlockHitBoxes;
	int h = 0;

	if (UpSideDown)
	{
		BlockHitBoxes.HitBox[h] = &HitBoxes[E_HITBOX_SLAB_UP];

		for (int i = 0; i < 4; i++)
		{
			if (MetaBits.bit[i])
			{
				h++;
				BlockHitBoxes.HitBox[h] = &HitBoxes[E_HITBOX_STAIRS_UP_XP_ZM + i];
			}
		}
	}
	else
	{
		BlockHitBoxes.HitBox[0] = &HitBoxes[E_HITBOX_SLAB_DOWN];

		for (int i = 0; i < 4; i++)
		{
			if (MetaBits.bit[i])
			{
				h++;
				BlockHitBoxes.HitBox[h] = &HitBoxes[E_HITBOX_STAIRS_DOWN_XP_ZM + i];
			}
		}
	}
	
	return BlockHitBoxes;
}




             
sBlockHitBoxes cBoxBlockTrace::GetBlockHitBoxes(cWorld & a_World, Vector3i a_BlockCoords)
{
	
	NIBBLETYPE BlockType = a_World.GetBlock(a_BlockCoords);
	if (IsBlockStairs(BlockType))
	{
		return GetBlockStairsHitBoxes(a_World, a_BlockCoords);
	}
	return{};
}





bool cBoxBlockTrace::BoxOnGroundTrace(cWorld & a_World, const Vector3d & a_BoxCoords, double a_Width)
{
	return{};
}





bool cBoxBlockTrace::Trace(cWorld & a_World, const Vector3d & a_Start, const Vector3d & a_End,
						Vector3d & a_HitCoords, double a_Height, double a_Width, bool & a_OnGround
)
{
	m_Start = a_Start;
	m_End = a_End;

	if (m_Start.x < m_End.x)
	{
		m_DirX = 1;
	}
	else if (m_Start.x > m_End.x)
	{
		m_DirX = -1;
	}

	if (m_Start.y < m_End.y)
	{
		m_DirY = 1;
	}
	else if (m_Start.y > m_End.y)
	{
		m_DirY = -1;
	}

	if (m_Start.z < m_End.z)
	{
		m_DirZ = 1;
	}
	else if (m_Start.z > m_End.z)
	{
		m_DirZ = -1;
	}

	m_DirVector = { m_End.x - m_Start.x, m_End.y - m_Start.y, m_End.z - m_Start.z };
	if (m_DirVector.HasNonZeroLength())
	{
		m_DirVector.Normalize();
	}
	
	return{};
}