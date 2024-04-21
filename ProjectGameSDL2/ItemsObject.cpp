
#include "ItemsObject.h"

ItemsObject::ItemsObject()
{
	width_frame_ = 0;;
	height_frame_ = 0;
	frame_ = 0;
}

ItemsObject::~ItemsObject()
{

}

bool ItemsObject::LoadImg(std::string path, SDL_Renderer* screen)
{
	bool ret = BaseObject::LoadImg(path, screen);

	if (ret)
	{
		width_frame_ = rect_.w / 6;
		height_frame_ = rect_.h;
	}

	rect_.w = width_frame_;

	set_clip();

	return ret;
}

void ItemsObject::set_clip()
{
	for (int i = 0; i < 6; i++)
	{
		clip_[i].x = i * width_frame_;
		clip_[i].y = 0;
		clip_[i].w = width_frame_;
		clip_[i].h = height_frame_;
	}
}

void ItemsObject::ShowItems(SDL_Renderer* des)
{
	if (frame_ > 50)
	{
		frame_ = 0;
	}
		
	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
	SDL_RenderCopy(des, p_object_, &clip_[frame_ / 10], &renderQuad);

	frame_++;
}