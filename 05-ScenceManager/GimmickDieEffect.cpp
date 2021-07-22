#include "GimmickDieEffect.h"
#include "GameObject.h"

CGimmickDieEffect::CGimmickDieEffect()
{
	DebugOut(L"[ERROR] This is Gimmick Die!\n");
}

void CGimmickDieEffect::drawDieEffect(float positionX, float positionY)
{
	/*switch (direction)
	{
	case 0:
		vx = 0;
		vy = GIMMICKDIEEFFECT_SPEED;
		break;
	case 1:
		vx = GIMMICKDIEEFFECT_SPEED_675;
		vy = GIMMICKDIEEFFECT_SPEED_225;
		break;
	case 2:
		vx = GIMMICKDIEEFFECT_SPEED_450;
		vy = GIMMICKDIEEFFECT_SPEED_450;
		break;
	case 3:
		vx = GIMMICKDIEEFFECT_SPEED_225;
		vy = GIMMICKDIEEFFECT_SPEED_675;
		break;
	case 4:
		vx = GIMMICKDIEEFFECT_SPEED;
		vy = 0;
		break;
	case 5:
		vx = GIMMICKDIEEFFECT_SPEED_225;
		vy = -GIMMICKDIEEFFECT_SPEED_675;
		break;
	case 6:
		vx = GIMMICKDIEEFFECT_SPEED_450;
		vy = -GIMMICKDIEEFFECT_SPEED_450;
		break;
	case 7:
		vx = GIMMICKDIEEFFECT_SPEED_675;
		vy = -GIMMICKDIEEFFECT_SPEED_225;
		break;
	case 8:
		vx = 0;
		vy = -GIMMICKDIEEFFECT_SPEED;
		break;
	case 9:
		vx = -GIMMICKDIEEFFECT_SPEED_675;
		vy = -GIMMICKDIEEFFECT_SPEED_225;
		break;
	case 10:
		vx = -GIMMICKDIEEFFECT_SPEED_450;
		vy = -GIMMICKDIEEFFECT_SPEED_450;
		break;
	case 11:
		vx = -GIMMICKDIEEFFECT_SPEED_225;
		vy = -GIMMICKDIEEFFECT_SPEED_675;
		break;
	case 12:
		vx = -GIMMICKDIEEFFECT_SPEED;
		vy = 0;
		break;
	case 13:
		vx = -GIMMICKDIEEFFECT_SPEED_225;
		vy = GIMMICKDIEEFFECT_SPEED_675;
		break;
	case 14:
		vx = -GIMMICKDIEEFFECT_SPEED_450;
		vy = GIMMICKDIEEFFECT_SPEED_450;
		break;
	case 15:
		vx = -GIMMICKDIEEFFECT_SPEED_675;
		vy = GIMMICKDIEEFFECT_SPEED_225;
		break;
	default:
		break;
	}*/
}

//void CGimmickDieEffect::SetAnimationSet(float positionX, float positionY) { 
//
//}


void CGimmickDieEffect::Render()
{
	animation_set->at(0)->Render(x, y);
}

void CGimmickDieEffect::GetBoundingBox(float& left, float& top, float& right, float& bottom)
{
	left = 0;
	top = 0;
	right = 0;
	bottom = 0;
}
