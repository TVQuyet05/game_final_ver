
#include "ExplosionObject.h"

ExplosionObject::ExplosionObject()
{
	frame_ = 0;
}

ExplosionObject::~ExplosionObject()
{

}

void ExplosionObject::set_clip()
{
	for (int i = 0; i < 6; i++)
	{
		clip_[i].x = i * EXPLOSION_WIDTH;
		clip_[i].y = 0;
		clip_[i].w = EXPLOSION_WIDTH;
		clip_[i].h = EXPLOSION_HEIGHT;
	}
}

void ExplosionObject::ShowExplosion(SDL_Renderer* des)
{
	

	SDL_Rect renderQuad = { rect_.x, rect_.y, EXPLOSION_WIDTH, EXPLOSION_HEIGHT };
	SDL_RenderCopy(des, p_object_, &clip_[frame_], &renderQuad);
	

	frame_++;
}

void ExplosionObject::set_pos_of_explosion(const SDL_Rect& rect)
{
	rect_.x = (rect.x + rect.w * 0.5) - EXPLOSION_WIDTH * 0.5;
	rect_.y = (rect.y + rect.h * 0.5) - EXPLOSION_HEIGHT * 0.5;
}

