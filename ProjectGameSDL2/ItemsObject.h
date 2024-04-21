
#ifndef ITEMS_OBJECT_H_
#define ITEMS_OBJECT_H_

#include "CommonFunc.h"
#include "BaseObject.h"

class ItemsObject : public BaseObject
{
public:
	ItemsObject();
	~ItemsObject();

	bool LoadImg(std::string path, SDL_Renderer* screen);
	void set_clip();
	void ShowItems(SDL_Renderer* des);

private:
	int width_frame_;
	int height_frame_;
	int frame_;

	SDL_Rect clip_[6];
};

#endif // !ITEMS_OBJECT_H_
