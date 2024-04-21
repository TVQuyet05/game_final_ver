#include "CommonFunc.h"
#include "BaseObject.h"
#include "MainObject.h"
#include "ThreatToHouse.h"
#include "ThreatToMain.h"
#include "ImpTimer.h"
#include "ExplosionObject.h"
#include "TextObject.h"
#include "ItemsObject.h"

int TIME_THREAT_TO_HOUSE_APPEAR = 150000;
int TIME_THREAT_TO_MAIN_APPEAR = 7000;

BaseObject g_background;

TTF_Font* font_time = NULL;
TTF_Font* font_name = NULL;
TTF_Font* font_score = NULL;
TTF_Font* font_score_small = NULL;

int score_of_player = 0;

const int max_blood_of_house = 1000;
const int max_blood_of_player = 170;

int blood_of_house = max_blood_of_house;
int blood_of_player = max_blood_of_player;

int numbers_threat_to_house_created = 0;
int numbers_threat_to_main_created = 0;

void Close()
{
	g_background.Free();

	SDL_DestroyRenderer(g_screen);
	g_screen = NULL;

	SDL_DestroyWindow(g_window);
	g_window = NULL;

	IMG_Quit();
	SDL_Quit();
}

bool CheckFocusWithRect(const int& x, const int& y, const SDL_Rect& rect)
{
	if (x >= rect.x && x <= rect.x + rect.w &&
		y >= rect.y && y <= rect.y + rect.h)
	{
		return true;
	}
	return false;
}


// Function pause game
int pause_game()
{
	ImpTimer time_pause;
	time_pause.start();

	BaseObject background_pause;
	background_pause.LoadImg("image/image_pause.png", g_screen);

	BaseObject button_continue;
	button_continue.LoadImg("image/button_continue.png", g_screen);
	button_continue.SetRect(600, 100);

	BaseObject button_continue_down;
	button_continue_down.LoadImg("image/button_continue_down.png", g_screen);
	button_continue_down.SetRect(600, 100);

	BaseObject button_quit;
	button_quit.LoadImg("image/button_quit.png", g_screen);
	button_quit.SetRect(600, 210);

	BaseObject button_quit_down;
	button_quit_down.LoadImg("image/button_quit_down.png", g_screen);
	button_quit_down.SetRect(600, 210);

	
	int xmm = 0;
	int ymm = 0;
	int button = 0;
	SDL_Event m_event;
	int th = 0;
	int tk = 0;
	while (1) {

		background_pause.Render(g_screen);
		

		if (th == 1)
		{
			button_continue_down.Render(g_screen);
		}
		else
		{
			button_continue.Render(g_screen);
		}

		if (tk == 1)
		{
			button_quit_down.Render(g_screen);
		}
		else
		{
			button_quit.Render(g_screen);
		}


		while (SDL_PollEvent(&m_event))
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
			{
				Close();
				return 0;
			}
			case SDL_MOUSEMOTION:
			{
				button = SDL_GetMouseState(&xmm, &ymm);

				if (CheckFocusWithRect(xmm, ymm, button_continue.GetRect()))
				{
					th = 1;

				}
				else
				{
					th = 0;
				}


				if (CheckFocusWithRect(xmm, ymm, button_quit.GetRect()))
				{
					tk = 1;
				}
				else
				{
					tk = 0;
				}

				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				xmm = m_event.button.x;
				ymm = m_event.button.y;

				if (CheckFocusWithRect(xmm, ymm, button_continue.GetRect()))
				{

					return time_pause.get_ticks(); // Continue play return time pause
				}
				else if (CheckFocusWithRect(xmm, ymm, button_quit.GetRect()))
				{
					
					return -1;

				}
			}
			default:
				break;
			}
		}

		SDL_RenderPresent(g_screen);
	}
}

// Init list item
std::vector<ItemsObject*> list_item_house;
std::vector<ItemsObject*> list_item_main;

void add_item(const SDL_Rect& threat)
{
	int rand_ = rand() % 5;
	if (rand_ == 1) 
	{
		ItemsObject* item = new ItemsObject();
		item->LoadImg("image/heal_main.png", g_screen);
		item->SetRect(threat.x, threat.y);
		list_item_main.push_back(item);
	}
	else if (rand_ == 2 || rand_ == 3)
	{
		ItemsObject* item = new ItemsObject();
		item->LoadImg("image/heal_house.png", g_screen);
		item->SetRect(threat.x, threat.y);
		list_item_house.push_back(item);
	}
}

bool InitData()
{
	bool success = true;
	int ret = SDL_Init(SDL_INIT_VIDEO);
	if (ret < 0)
	{
		return false;
	}
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
	g_window = SDL_CreateWindow("Game Cpp SDL 2.0 Tran Van Quyet", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

	if (g_window == NULL)
	{
		success = false;
	}
	else
	{
		g_screen = SDL_CreateRenderer(g_window, -1, SDL_RENDERER_ACCELERATED);
		if (g_screen == NULL)
		{
			success = false;
		}
		else
		{
			SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
			int imgFlags = IMG_INIT_PNG;
			if (!(IMG_Init(imgFlags) && imgFlags))
			{
				success = false;
			}
		}

		// Init font of text
		if (TTF_Init() == -1)
		{
			success = false;
		}

		// Load up font
		font_time = TTF_OpenFont("font/good.ttf", 35);
		font_name = TTF_OpenFont("font/font_text.ttf", 100);
		font_score = TTF_OpenFont("font/good.ttf", 65);
		font_score_small = TTF_OpenFont("font/good.ttf", 54);

		if (font_time == NULL || font_name == NULL)
		{
			success = false;
		}

		// Init mixer
		if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
		{
			success = false;
		}
		
		// Read file wav audio
		g_sound_bullet = Mix_LoadWAV("sound/laser.wav");

		if (g_sound_bullet == NULL)
		{
			success = false;
		}
		//Mix_VolumeChunk(g_sound_bullet, MIX_MAX_VOLUME / 8);

		g_sound_track = Mix_LoadMUS("sound/soundtrack.mp3");
		if (g_sound_track == NULL)
		{
			success = false;
		}

	}

	return success;
}

bool LoadBackground()
{
	bool ret = g_background.LoadImg("image/background.png", g_screen);

	return ret;
}



int main(int argc, char* argv[])
{
	if (InitData() == false)
	{
		return -1;
	}
	if (LoadBackground() == false)
	{
		return -1;
	}

	// Play soundtrack
	Mix_PlayMusic(g_sound_track, -1);

	// Init pause text
	TextObject pause_text;
	pause_text.SetColor(TextObject::WHITE_TEXT);
	std::string direction_pause = "Press P to pause game";
	pause_text.SetText(direction_pause);
	pause_text.LoadFromRenderText(font_time, g_screen);

	// Make menu
	BaseObject image_menu;
	image_menu.LoadImg("image/backhome.png", g_screen);
	
	BaseObject button_play;
	button_play.LoadImg("image/button_play.png", g_screen);
	button_play.SetRect(1160, 350);
	
	BaseObject button_quit;
	button_quit.LoadImg("image/button_quit.png", g_screen);
	button_quit.SetRect(1160, 460);

	BaseObject button_play_down;
	button_play_down.LoadImg("image/button_play_down.png", g_screen);
	button_play_down.SetRect(1160, 350);

	BaseObject button_quit_down;
	button_quit_down.LoadImg("image/button_quit_down.png", g_screen);
	button_quit_down.SetRect(1160, 460);

	TextObject first_name_text;
	first_name_text.SetColor(TextObject::BLACK_TEXT);
	std::string first_name = "DYNAMIC";
	first_name_text.SetText(first_name);
	first_name_text.LoadFromRenderText(font_name, g_screen);
	
	TextObject second_name_text;
	second_name_text.SetColor(TextObject::BLACK_TEXT);
	std::string second_name = "TANK";
	second_name_text.SetText(second_name);
	second_name_text.LoadFromRenderText(font_name, g_screen);

	bool start = false;
	int xmm = 0;
	int ymm = 0;
	int button = 0;
	SDL_Event m_event;
	int th = 0;
	int tk = 0;
	while (!start) {

		image_menu.Render(g_screen);
		first_name_text.RenderText(g_screen, 100, 30);
		second_name_text.RenderText(g_screen, 180, 140);
		
		if (th == 1)
		{
			button_play_down.Render(g_screen);
		}
		else
		{ 
			button_play.Render(g_screen);
		}
		
		if (tk == 1)
		{
			button_quit_down.Render(g_screen);
		}
		else
		{
			button_quit.Render(g_screen);
		}
		

		while (SDL_PollEvent(&m_event))
		{
			switch (m_event.type)
			{
			case SDL_QUIT:
				return 0;
			case SDL_MOUSEMOTION:
			{
				button = SDL_GetMouseState(&xmm, &ymm);

				if (CheckFocusWithRect(xmm, ymm, button_play.GetRect()))
				{
					th = 1;
					
				}
				else
				{
					th = 0;
				}

				
				if (CheckFocusWithRect(xmm, ymm, button_quit.GetRect()))
				{
					tk = 1;
				}
				else
				{
					tk = 0;
				}

				break;
			}
			case SDL_MOUSEBUTTONDOWN:
			{
				xmm = m_event.button.x;
				ymm = m_event.button.y;

				if (CheckFocusWithRect(xmm, ymm, button_play.GetRect()))
				{
					start = true;
				}
				else if (CheckFocusWithRect(xmm, ymm, button_quit.GetRect()))
				{
					return 1;
				}
			}
			default:
				break;
			}
		}

		SDL_RenderPresent(g_screen);
	}



	// Init time to calculate fps
	ImpTimer fps_timer;

	// Init time start of game
	ImpTimer time_start;
	time_start.start();

	// Init blood bar of house
	BaseObject blood_bar_of_house;
	blood_bar_of_house.LoadImg("image/heartbar.png", g_screen);
	blood_bar_of_house.SetRect(15, 9);

	// Init list of explosion
	std::vector<ExplosionObject*> p_explosion_list;


	// Init main object player
	MainObject p_player;
	p_player.LoadImg("image/sprite_main.png", g_screen);

	// Init main house
	BaseObject main_house;
	main_house.LoadImg("image/image_house.png", g_screen);
	main_house.SetRect(SCREEN_WIDTH/2 - 160, SCREEN_HEIGHT/2 - 120);
	SDL_Rect rect_of_main_house = main_house.GetRect();


	// Init list threat to house
	std::vector<ThreatToHouse*> p_threat_to_house_list;

	// Init list threat to main
	std::vector<ThreatToMain*> p_threat_to_main_list;

	// Time text
	TextObject time_game_text;
	time_game_text.SetColor(TextObject::WHITE_TEXT);

	// Score text
	TextObject score_text;
	score_text.SetColor(TextObject::WHITE_TEXT);

	
	int time_pause_game = 0;

	bool is_quit = false;
	while (!is_quit)
	{
		fps_timer.start();

		int time_of_game = time_start.get_ticks();
		time_of_game -= time_pause_game;
		//std::cout << time_of_game << "\n";

		while (SDL_PollEvent(&g_event) != 0)
		{
			if (g_event.type == SDL_QUIT)
			{
				is_quit = true;
			}

			p_player.HandleInputAction(g_event, g_screen, g_sound_bullet);

			if (g_event.type == SDL_KEYDOWN)
			{
				if (g_event.key.keysym.sym == SDLK_p)
				{
					int time_of_pause = pause_game();
					if (time_of_pause == -1)
					{
						Close();
						return 0;
					}
					time_pause_game += time_of_pause;
					
				}
			}
		}

		SDL_SetRenderDrawColor(g_screen, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR, RENDER_DRAW_COLOR);
		SDL_RenderClear(g_screen);

		g_background.Render(g_screen, NULL);



		// Render item
		for (int i = 0; i < list_item_house.size(); i++)
		{
			ItemsObject* item = list_item_house.at(i);
			item->ShowItems(g_screen);

			bool check = p_player.CheckCollision(item->GetRect());
			if (check)
			{
				blood_of_house += 8;

				list_item_house.erase(list_item_house.begin() + i);
				delete item;
				item = NULL;
			}
		}

		for (int i = 0; i < list_item_main.size(); i++)
		{
			ItemsObject* item = list_item_main.at(i);
			item->ShowItems(g_screen);

			bool check = p_player.CheckCollision(item->GetRect());
			if (check)
			{
				blood_of_player += 2;

				list_item_main.erase(list_item_main.begin() + i);
				delete item;
				item = NULL;
			}
		}

	

		// Add threat into threat to house list
		if (time_of_game / TIME_THREAT_TO_HOUSE_APPEAR > numbers_threat_to_house_created)
		{
			ThreatToHouse* threat_to_house = new ThreatToHouse();
			threat_to_house->LoadImg("image/threat_to_house.png", g_screen);
			p_threat_to_house_list.push_back(threat_to_house);

			numbers_threat_to_house_created++;
		}

		// Move threat of threat to house list
		for (int i = 0; i < p_threat_to_house_list.size(); i++)
		{
			ThreatToHouse* threat_to_house = p_threat_to_house_list.at(i);

			// Check threat attack house and do threat
			bool ret1 = threat_to_house->DoThreat(rect_of_main_house);
			if (ret1)
			{
				blood_of_house -= 1;
			}

			// Check bullet of player with threat
			std::vector<BulletObject*> list_bullet_of_player = p_player.get_bullet_list();
			for (int i2 = 0; i2 < list_bullet_of_player.size(); i2++)
			{
				BulletObject* p_bullet = list_bullet_of_player.at(i2);
				if (p_bullet != NULL && threat_to_house != NULL)
				{
					bool ret2 = threat_to_house->CheckCollision(p_bullet->GetRect());
					if (ret2)
					{
						// Delete this bullet out list
						p_bullet->set_is_move(false);

						// Decrease blood of threat
						threat_to_house->DecreaseBlood();

					}
				}
			}

			// If blood = 0 -> delete threat
			if (threat_to_house->get_blood() < 1)
			{
				// Add item
				add_item(threat_to_house->GetRect());

				// Init explosion
				ExplosionObject* exp_1 = new ExplosionObject();
				exp_1->LoadImg("image/explosion.png", g_screen);
				exp_1->set_pos_of_explosion(threat_to_house->GetRect());
				exp_1->set_clip();
				p_explosion_list.push_back(exp_1);


				p_threat_to_house_list.erase(p_threat_to_house_list.begin() + i);
				if (threat_to_house != NULL)
				{
					delete threat_to_house;
					threat_to_house = NULL;
				}

				// Increase score
				score_of_player += 1;

			

			}
			else
			{
				threat_to_house->Render(g_screen, NULL);
			}

			
		}



		// Add threat into threat to main list
		if (time_of_game / TIME_THREAT_TO_MAIN_APPEAR > numbers_threat_to_main_created)
		{
			ThreatToMain* threat_to_main = new ThreatToMain();
			threat_to_main->LoadImg("image/sprite_threat_to_main.png", g_screen);
			p_threat_to_main_list.push_back(threat_to_main);

			numbers_threat_to_main_created++;
		}

		// Move threat of threat to main list
		for (int i = 0; i < p_threat_to_main_list.size(); i++)
		{
			ThreatToMain* threat_to_main = p_threat_to_main_list.at(i);
			threat_to_main->DoThreat(p_player.GetRect(), rect_of_main_house);
			threat_to_main->HandleBullet(g_screen);

			//Check bullet of threat with main and house
			std::vector<BulletObject*> list_bullet_of_threat = threat_to_main->get_bullet_list();
			for (int i2 = 0; i2 < list_bullet_of_threat.size(); i2++)
			{
				BulletObject* p_bullet = list_bullet_of_threat.at(i2);
				if (p_bullet != NULL)
				{
					// Check bullet with house
					bool ret1 = p_bullet->CheckCollision(rect_of_main_house);
					if (ret1)
					{
						blood_of_house -= 1;
					}

					// Check bullet with player
					bool ret2 = p_bullet->CheckCollision(p_player.GetRect());
					if (ret2)
					{
						blood_of_player -= 1;
					}

					// Delete bullet out list 
					if (ret1 || ret2)
					{
						p_bullet->set_is_move(false);
					}
				}
			}

			// Check bullet of player with threat
			std::vector<BulletObject*> list_bullet_of_player = p_player.get_bullet_list();
			for (int i2 = 0; i2 < list_bullet_of_player.size(); i2++)
			{
				BulletObject* p_bullet = list_bullet_of_player.at(i2);
				if (p_bullet != NULL && threat_to_main != NULL)
				{
					bool ret2 = threat_to_main->CheckCollision(p_bullet->GetRect());
					if (ret2)
					{
						// Delete this bullet out list
						p_bullet->set_is_move(false);

						// Decrease blood of threat
						threat_to_main->DecreaseBlood();

					}
				}
			}

			bool ret1 = threat_to_main->CheckCollision(rect_of_main_house);
			if (ret1)
			{
				blood_of_house -= 5;
			}

			// If blood = 0 or threat collision with house -> delete threat
			if (ret1 || threat_to_main->get_blood() < 1)
			{
				// Add item
				add_item(threat_to_main->GetRect());

				// Init explosion
				ExplosionObject* exp_1 = new ExplosionObject();
				exp_1->LoadImg("image/explosion.png", g_screen);
				exp_1->set_pos_of_explosion(threat_to_main->GetRect());
				exp_1->set_clip();
				p_explosion_list.push_back(exp_1);

				p_threat_to_main_list.erase(p_threat_to_main_list.begin() + i);
				if (threat_to_main != NULL)
				{
					delete threat_to_main;
					threat_to_main = NULL;
				}

				// Increase score
				score_of_player += 2;
			}
			else
			{
				threat_to_main->Show(g_screen);
			}
			
		}

		// Move player
		p_player.DoPlayer(rect_of_main_house);
		p_player.Show(g_screen);
		p_player.HandleBullet(g_screen);


		// Show explosion
		for (int ex = 0; ex < p_explosion_list.size(); ex++)
		{
			ExplosionObject* exp = p_explosion_list.at(ex);
			exp->ShowExplosion(g_screen);
			if (exp->get_frame() == 6)
			{
				p_explosion_list.erase(p_explosion_list.begin() + ex);
				if (exp != NULL)
				{
					delete exp;
					exp = NULL;
				}
			}
		}

		//std::cout << p_explosion_list.size() << "\n";

		// Render main house
		main_house.Render(g_screen, NULL);


		// Render blood bar of house
		blood_bar_of_house.Render(g_screen);
		int blood_remain_of_house = blood_of_house * 558 / max_blood_of_house;
		SDL_Rect blood_losted = { blood_remain_of_house + 44, 21, 558 - blood_remain_of_house, 18 };
		SDL_RenderFillRect(g_screen, &blood_losted);
		
		// Render blood bar of player
		SDL_SetRenderDrawColor(g_screen, 0, 153, 0, 255);
		int x_pos_bar = p_player.GetRect().x - 9;
		int y_pos_bar = p_player.GetRect().y - 15;
		SDL_Rect blood_of_main_game = { x_pos_bar, y_pos_bar, 80, 12 };
		SDL_RenderFillRect(g_screen, &blood_of_main_game);
		int blood_remain_of_player = blood_of_player * 80 / max_blood_of_player;
		SDL_Rect blood_losted_of_main = {blood_remain_of_player + x_pos_bar, y_pos_bar, 80 - blood_remain_of_player, 12};
		SDL_SetRenderDrawColor(g_screen, 102, 0, 51, 255);
		SDL_RenderFillRect(g_screen, &blood_losted_of_main);


		if (blood_of_house < 0 || blood_of_player < 0) 
		{
			// Make screen end game

			int a[6];
			std::ifstream file_score("HighScore.txt");
			for (int i = 0; i < 5; i++)
			{
				file_score >> a[i];
				//std::cout << a[i] << " ";
			}
			file_score.close();
			a[5] = score_of_player;
			std::sort(a, a + 6);



			BaseObject image_end;
			image_end.LoadImg("image/screen_end_game.png", g_screen);

			TextObject your_score_text;
			std::string your_score = "YOUR SCORE : ";
			your_score += std::to_string(score_of_player);
			your_score_text.SetColor(150, 200, 100);
			your_score_text.SetText(your_score);
			your_score_text.LoadFromRenderText(font_score, g_screen);

			TextObject high_score_text;
			std::string high_score = "HIGH SCORE : ";
			high_score += std::to_string(a[5]);
			high_score_text.SetColor(80, 240, 60);
			high_score_text.SetText(high_score);
			high_score_text.LoadFromRenderText(font_score, g_screen);

			std::vector<TextObject*> list_text_score;
			for (int i = 0; i < 4; i++)
			{
				TextObject* text_ = new TextObject();
				text_->SetColor(60, 240, 120);
				std::string text = std::to_string(a[4 - i]);
				text_->SetText(text);
				text_->LoadFromRenderText(font_score_small, g_screen);
				list_text_score.push_back(text_);
			}
			
			button_quit.SetRect(610, 550);
			button_quit_down.SetRect(610, 550);

			bool end = false;
			while (!end)
			{
				image_end.Render(g_screen);
				your_score_text.RenderText(g_screen, 420, 20);
				high_score_text.RenderText(g_screen, 350, 150);

				for (int i = 0; i < 4; i++)
				{
					list_text_score.at(i)->RenderText(g_screen, 760, 240 + i * 60);
				}


				if (tk == 1)
				{
					button_quit_down.Render(g_screen);
				}
				else
				{
					button_quit.Render(g_screen);
				}

				while (SDL_PollEvent(&m_event))
				{
					switch (m_event.type)
					{
					case SDL_QUIT:
					{
						end = true;
					}
					case SDL_MOUSEMOTION:
					{
						button = SDL_GetMouseState(&xmm, &ymm);

						if (CheckFocusWithRect(xmm, ymm, button_quit.GetRect()))
						{
							tk = 1;
						}
						else
						{
							tk = 0;
						}

						break;
					}
					case SDL_MOUSEBUTTONDOWN:
					{
						xmm = m_event.button.x;
						ymm = m_event.button.y;

						if(CheckFocusWithRect(xmm, ymm, button_quit.GetRect()))
						{
							end = true;
						}
					}
					default:
						break;
					}
				}

				SDL_RenderPresent(g_screen);
			}			

			std::ofstream file("HighScore.txt");
			for (int i = 5; i > 0; i--)
			{
				file << a[i] << " ";
				//std::cout << a[i] << " ";
			}
			file.close();

			Close();
			return 2;
		}

		
		
		// Show time of game
		std::string str_time = "Time : ";
		std::string str_val = std::to_string(time_of_game / 1000);
		str_time += str_val;
		time_game_text.SetText(str_time);
		time_game_text.LoadFromRenderText(font_time, g_screen);
		time_game_text.RenderText(g_screen, SCREEN_WIDTH - 400, 12);

		// Show score of player
		std::string str_score = "Score : ";
		str_val = std::to_string(score_of_player);
		str_score += str_val;
		score_text.SetText(str_score);
		score_text.LoadFromRenderText(font_time, g_screen);
		score_text.RenderText(g_screen, SCREEN_WIDTH - 700, 12);

		// Show direction to pause
		pause_text.RenderText(g_screen, 20, 678);

		
		SDL_RenderPresent(g_screen);


		// Solve time make threat
		if (time_of_game > 15)
		{
			TIME_THREAT_TO_HOUSE_APPEAR = 3000;
			TIME_THREAT_TO_MAIN_APPEAR = 5000;
		}
		if (time_of_game > 30)
		{
			TIME_THREAT_TO_HOUSE_APPEAR = 2000;
			TIME_THREAT_TO_MAIN_APPEAR = 4000;
		}

		int real_imp_time = fps_timer.get_ticks();
		int time_one_frame = 1000 / FRAME_PER_SECOND;

		if (real_imp_time < time_one_frame)
		{
			int delay_time = time_one_frame - real_imp_time;
			SDL_Delay(delay_time);
		}
	}

	

	Close();

	return 0;
}