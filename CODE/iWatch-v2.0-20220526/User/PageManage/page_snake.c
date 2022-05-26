#include "iWatch.h"

#define PANEL_WIDTH		92
#define PANEL_HEIGHT	64
#define	MAX_SNAKE_SPEED	50
#define	CONTROL_SENSITIVITY	10	//控制的灵敏度，越小越灵敏
static unsigned char XDATA i = 0;
static unsigned char XDATA snake_direction = 0;	//蛇当前的前进方向
static unsigned char XDATA snake_speed = 10;		//蛇当前的前进速度
static unsigned char XDATA past_x[256];
static unsigned char XDATA past_y[256];
static unsigned char XDATA snake_head_x = 64, snake_head_y = 32, snake_len = 20;
static unsigned char XDATA test_now = 0, delicious_x= 80,delicious_y = 40;
static unsigned char XDATA best_score = 0;	//最佳成绩
static bit game_loop = 0;
static bit game_over = 0;
static bit game_pause = 0;

static unsigned char CODE Icon[] = {
	48, 48,
	0x00,0x00,0xE0,0xE0,0x38,0x38,0x38,0x38,0x38,0x38,0x00,0xF8,0xF8,0xF0,0xC0,0xC0,
	0x00,0x00,0xF8,0xF8,0x00,0xC0,0xF8,0xF8,0x38,0x38,0x38,0xF8,0xF8,0xC0,0x00,0xF8,
	0xF8,0x00,0x00,0xC0,0xF8,0x38,0x00,0xC0,0xF0,0xF8,0x38,0x38,0x38,0x38,0x38,0x00,
	0x00,0x00,0x07,0x07,0x0C,0x0C,0x0C,0x0C,0xF8,0xF8,0x00,0xFF,0xFF,0xFF,0x00,0x03,
	0x0F,0x0C,0xFF,0xFF,0x00,0xFF,0xFF,0xFF,0x0C,0x0C,0x0C,0xFF,0xFF,0xFF,0x00,0xFF,
	0xFF,0x1C,0x7F,0xF3,0xC0,0x00,0x00,0xFF,0xFF,0xFF,0x0C,0x0C,0x0C,0x0C,0x00,0x00,
	0x00,0x00,0x07,0x07,0x07,0x07,0x07,0x07,0x03,0x03,0x00,0x07,0x07,0x07,0x00,0x00,
	0x00,0x00,0x07,0x07,0x00,0x07,0x07,0x07,0x00,0x00,0x00,0x07,0x07,0x07,0x00,0x07,
	0x07,0x00,0x00,0x01,0x07,0x07,0x00,0x01,0x07,0x07,0x07,0x07,0x07,0x07,0x07,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,
	0xC0,0xC0,0xC0,0xC0,0xC0,0xC0,0x00,0x00,0x00,0xF8,0xF8,0xF8,0x38,0x38,0xF8,0xF8,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0E,0x0E,
	0x0E,0x00,0x00,0x0E,0x0E,0x0E,0x0E,0x0E,0x0E,0xFE,0xFE,0xFE,0xC0,0xC0,0xFF,0xFF,
	0xFF,0x01,0x01,0xFF,0xFF,0xFF,0x00,0x00,0x00,0xFF,0xFF,0xFF,0x00,0x00,0xFF,0xFF,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
	0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x01,0x01,0x01,0x01,0x01,0x01,
	0x01,0x00,0x00,0x0F,0x0F,0x0F,0x0E,0x0E,0x0E,0x0F,0x0F,0x0F,0x00,0x00,0x7F,0x7F
	/* (48 X 48 )*/
};
static t_psWidget text1, text2, label_score, label_best, label_game;
static t_psWidget canvas1;
static t_psGroup group1;

/**
  * @brief  页面初始化事件
  * @param  无
  * @retval 无
  */
static void Setup(unsigned char condition)
{
	unsigned char i;
	text1 = og_label_create(FONT_ASCII_5X7, 93, 0);
	og_label_setText(text1, "Score:");
	label_score = og_label_create(FONT_ASCII_5X7, 93, 8);
	text2 = og_label_create(FONT_ASCII_5X7, 93, 40);
	og_label_setText(text2, "Best:");
	label_best = og_label_create(FONT_ASCII_5X7, 93, 48);
	label_game = og_label_create(FONT_ASCII_7X8, 34, -8);
	og_label_setText(label_game, "Game over!");
	og_widget_setAlignment(label_game, ALIGN_HORIZONMID);
	og_widget_setShow(label_game, 0);
	canvas1 = og_canvas_create(92, 64, 0, 0);
	og_canvas_drawRectangle(canvas1, 92, 64, 0, 0);
	group1 = og_group_create(6);
	og_group_addWidget(group1, text1, 0);
	og_group_addWidget(group1, text2, 0);
	og_group_addWidget(group1, label_score, 0);
	og_group_addWidget(group1, label_best, 0);
	og_group_addWidget(group1, label_game, 0);
	og_group_addWidget(group1, canvas1, 0);
	og_group_setPosOffset(group1, 0, 64);
	og_group_addAnimOffset(group1, 0, -64, ANIM_TIME_NORM, ANIM_NULL_CB);
	LSM6DSMConfigAcc(ACC_ODR_208_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_ODR_208_HZ, GYR_SCALE_500_DPS);
	srand((time.second << 8) | time.minute);		//随便整个数作为随机数播种
	snake_direction = 0;
	snake_head_x = 64;
	snake_head_y = 32;
	snake_len = 20;
	game_over = 0;
	game_pause = 0;
	for(i = 0; i < snake_len; i++)
	{
		past_x[i] = 64;
		past_y[i] = 32;
	}
}
static void delet(void)
{
	og_group_delet(group1);
	pageSetStatus(page_snake, PAGE_IDLE);
}
/**
  * @brief  页面退出事件
  * @param  无
  * @retval 无
  */
static void Exit(unsigned char condition)
{
	if(config.lsm6dsm_cfg == 0)
		LSM6DSMConfigAcc(ACC_POWER_DOWN, ACC_SCALE_4_G);
	else
		LSM6DSMConfigAcc(ACC_ODR_26_HZ, ACC_SCALE_4_G);
	LSM6DSMConfigGyr(GYR_POWER_DOWN, GYR_SCALE_500_DPS);
	og_group_addAnimOffset(group1, 0, 64, ANIM_TIME_NORM, delet);
}
/**
  * @brief  页面循环执行的内容
  * @param  无
  * @retval 无
  */
static void Loop()
{
	if(pageExecuteRate(&Rate125Hz))
	{
		iWatchKeepActive();
		LSM6DSMReadGYRAndACC(&LSM6DSM);
		IMUupdate(&LSM6DSM);
	}
	if(pageExecuteRate(&Rate1000Hz))
	{
		static unsigned char t_cnt = 0;
		if(++t_cnt == (MAX_SNAKE_SPEED - snake_speed))
		{
			t_cnt = 0;
			game_loop = 1;
		}
		if(game_loop)
		{
			game_loop = 0;
			if(game_pause != 1)
			{
				og_canvas_clear(canvas1);
				og_canvas_drawRectangle(canvas1, 92, 64, 0, 0);
				lablel_printf(label_score, "%u", snake_len);
				if(snake_len > best_score)
				best_score = snake_len;
				lablel_printf(label_best, "%u", best_score);
				
				if(snake_direction == 0)
				{
					if(LSM6DSM.AngleY > CONTROL_SENSITIVITY)
						snake_direction = 1;
					else if(LSM6DSM.AngleY < -CONTROL_SENSITIVITY)
						snake_direction = 3;
				}
				else if(snake_direction == 1)
				{
					if(LSM6DSM.AngleX > CONTROL_SENSITIVITY)
						snake_direction = 0;
					else if(LSM6DSM.AngleX < -CONTROL_SENSITIVITY)
						snake_direction = 2;
				}
				else if(snake_direction == 2)
				{
					if(LSM6DSM.AngleY > CONTROL_SENSITIVITY)
						snake_direction = 1;
					else if(LSM6DSM.AngleY < -CONTROL_SENSITIVITY)
						snake_direction = 3;
				}
				else if(snake_direction == 3)
				{
					if(LSM6DSM.AngleX > CONTROL_SENSITIVITY)
						snake_direction = 0;
					else if(LSM6DSM.AngleX < -CONTROL_SENSITIVITY)
						snake_direction = 2;
				}
				//这段是按键程序解析后转换为方向以及点位置的程序，
				//sneak_direction是方向，test_x是头部的x轴值，test_y是头部的y轴值
				switch(snake_direction)
				{
					case 0:		//右
						snake_head_x++;
						break;
					case 1:		//下
						snake_head_y++;
						break;
					case 2:		//左
						snake_head_x--;
						break;
					case 3:		//上
						snake_head_y--;
						break;
				}
				if(snake_head_x >= PANEL_WIDTH)
					snake_head_x = 1;
				else if(snake_head_x <= 0)
					snake_head_x = PANEL_WIDTH - 1;
				if(snake_head_y >= PANEL_HEIGHT)
					snake_head_y = 1;
				else if(snake_head_y <= 0)
					snake_head_y = PANEL_HEIGHT - 1;
				//检查是否碰到自己的身体
				for(i = 0; i < snake_len; i++)
				{
					if((snake_head_x == past_x[i]) && (snake_head_y == past_y[i]))
					{
						og_widget_setShow(label_game, 1);
						og_anim_create(label_game, label_game->x, 24, ANIM_TIME_NORM, ANIM_NULL_CB);
						game_over = 1;
						game_pause = 1;
						break;
					}
				}
				//这段是可以让贪吃蛇有可变长度尾巴的测试程序，test_num就是尾巴的数量，最大50，
				//程序原理是把每次过去的坐标都记录在数组里面。显示的时候就把过去的点也都显示出来
				test_now++;
				test_now = test_now % snake_len;
				past_x[test_now] = snake_head_x;
				past_y[test_now] = snake_head_y;
				for(i = 0; i < snake_len; i++)
				{
					og_canvas_drawDot(canvas1, past_x[i], past_y[i]);
				}
				//这段程序放被吃掉的小东西，设计delicious_x,delicious_y为食物的坐标
				//吃掉食物以后会放一个新的食物，并且test_num也就是尾巴的数量会大1
				if((snake_head_x <= delicious_x+1) && (snake_head_y <= delicious_y+1) && (snake_head_x >= delicious_x-1) && (snake_head_y>=delicious_y-1))
				{
					snake_len++;
					past_x[snake_len - 1] = past_x[snake_len - 2];//解决小白点的问题
					past_y[snake_len - 1] = past_y[snake_len - 2];
					if(config.key_sound)
						Bee();
					delicious_x = rand() % 92;
					if(delicious_x > PANEL_WIDTH - 2)
						delicious_x = PANEL_WIDTH - 2;
					else if(delicious_x < 2)
						delicious_x  = 2;
					delicious_y = rand() % 64;
					if(delicious_y > PANEL_HEIGHT - 2)
						delicious_y = PANEL_HEIGHT - 2;
					else if(delicious_y < 2)
						delicious_y  = 2;
				}
				og_canvas_drawDot(canvas1, delicious_x,delicious_y);
				og_canvas_drawDot(canvas1, delicious_x+1,delicious_y);
				og_canvas_drawDot(canvas1, delicious_x-1,delicious_y);
				og_canvas_drawDot(canvas1, delicious_x,delicious_y+1);
				og_canvas_drawDot(canvas1, delicious_x+1,delicious_y+1);
				og_canvas_drawDot(canvas1, delicious_x-1,delicious_y+1);
				og_canvas_drawDot(canvas1, delicious_x,delicious_y-1);
				og_canvas_drawDot(canvas1, delicious_x+1,delicious_y-1);
				og_canvas_drawDot(canvas1, delicious_x-1,delicious_y-1);
				if(snake_len < 30)
					snake_speed = 10;
				else if(snake_len < 40)
					snake_speed = 20;
				else if(snake_len < 60)
					snake_speed = 30;
				else if(snake_len < 90)
					snake_speed = 35;
				else if(snake_len < 130)
					snake_speed = 40;
				else
					snake_speed = 45;
			}
			if(game_over)
			{
				
			}
		}
	}
}
/**
  * @brief  页面事件
  * @param  btn:发出事件的按键
  * @param  event:事件编号
  * @retval 无
  */
static void Event(unsigned char event)
{
	if(event == KEY1_PRESSED)
		pageShift(page_menu, PAGE_RETURN);
	else if(event == KEY2_PRESSED)
	{
		
	}
	else if(event == KEY3_PRESSED)
	{
		if(game_over != 1)
		{
			if(game_pause)
				game_pause = 0;
			else
				game_pause = 1;
		}
	}
	else if(event == KEY4_PRESSED)
	{
		//og_anim_create(text2, 48, 0, ANIM_TIME_NORM, NULL);
	}
}
void pageRegister_page_snake(unsigned char pageID)
{
	pageRegister(pageID, "SNAKE", Icon, Setup, Loop, Exit, Event);
}