#include "MainObject.h"

MainObject::MainObject()
{
	width_frame_ = 0;
	height_frame_ = 0;
	
	frame_ = 0;
}

MainObject::~MainObject()
{

}

void MainObject::HandleInputAction(SDL_Event events, SDL_Renderer* screen, Mix_Chunk* bullet_sound)
{
	// Control the character with the keyboard
	if (events.type == SDL_KEYDOWN)
	{
		switch (events.key.keysym.sym)
		{
			case SDLK_UP:
			{
				y_val_ = - PLAYER_SPEED;
				break;
			}
			case SDLK_DOWN:
			{
				y_val_ = PLAYER_SPEED;
				break;
			}
			case SDLK_RIGHT:
			{
				x_val_ = PLAYER_SPEED;
				break;
			}
			case SDLK_LEFT:
			{
				x_val_ = - PLAYER_SPEED;
				break;
			}
		}
	}
	else if (events.type == SDL_KEYUP)
	{
		switch (events.key.keysym.sym)
		{
			case SDLK_UP:
			{
				y_val_ = 0;
				break;
			}
			case SDLK_DOWN:
			{
				y_val_ = 0;
				break;
			}
			case SDLK_RIGHT:
			{
				x_val_ = 0;
				break;
			}
			case SDLK_LEFT:
			{
				x_val_ = 0;
				break;
			}
		}
	}

	// Calculate angle of player
	if (events.type == SDL_MOUSEMOTION)
	{
		int x_mouse = events.motion.x;
		int y_mouse = events.motion.y;

		angle_ = 90 + atan2(x_mouse - rect_.x, y_mouse - rect_.y) * 180.0 * -1 / M_PI;
	}

	// Init bullet of player when click left mouse
	if (events.type == SDL_MOUSEBUTTONDOWN)
	{
		if (events.button.button == SDL_BUTTON_LEFT)
		{
			BulletObject* p_bullet = new BulletObject();
			p_bullet->LoadImg("image/bullet_of_player.png", screen);

			p_bullet->set_angle_of_bullet(angle_);
			p_bullet->set_speed_of_bullet(BULLET_OF_PLAYER_SPEED);
			p_bullet->SetRect(this->rect_.x + this->rect_.w/2 - 16, this->rect_.y + this->rect_.h/2 - 3);

			p_bullet->set_is_move(true);
			p_bullet_list_.push_back(p_bullet);

			Mix_PlayChannel(-1, bullet_sound, 0);
		}
	}
	 
}

void MainObject::DoPlayer(const SDL_Rect& other_object) 
{
	rect_.x += x_val_;

	if (rect_.x < 0 || rect_.x + 80 > SCREEN_WIDTH || CheckCollision(other_object)) rect_.x -= x_val_;



	rect_.y += y_val_;

	if (rect_.y < 0 || rect_.y + 80 > SCREEN_HEIGHT || CheckCollision(other_object)) rect_.y -= y_val_;

	//std::cout << rect_.x << " " << rect_.y << "\n";
	
}

void MainObject::HandleBullet(SDL_Renderer* des)
{
	for (int i = 0; i < p_bullet_list_.size(); i++)
	{
		BulletObject* p_bullet = p_bullet_list_.at(i);
		if (p_bullet != NULL)
		{
			if (p_bullet->get_is_move() == true)
			{
				p_bullet->HandleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
				p_bullet->Render(des);
			}
			else
			{
				p_bullet_list_.erase(p_bullet_list_.begin() + i);
				if (p_bullet != NULL)
				{
					delete p_bullet;
					p_bullet = NULL;
				}
			}
		}
	}
}

void MainObject::Show(SDL_Renderer* des)
{
	if (frame_ > 50)
	{
		frame_ = 0;
	}

	SDL_Rect renderQuad = { rect_.x, rect_.y, width_frame_, height_frame_ };
	SDL_RenderCopyEx(des, p_object_, &clip_[frame_ / 10], &renderQuad, angle_, NULL, SDL_FLIP_NONE);
	//SDL_RenderCopy(des, p_object_, &clip_[frame_ / 10], &renderQuad);

	frame_++;

	//Render(des, NULL);
	
}

bool MainObject::LoadImg(std::string path, SDL_Renderer* screen)
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

void MainObject::set_clip()
{
	for (int i = 0; i < 6; i++)
	{
		clip_[i].x = i * width_frame_;
		clip_[i].y = 0;
		clip_[i].w = width_frame_;
		clip_[i].h = height_frame_;
	}
}