//
// COMP-GENG 421 - Ty Aramaki
//
// Pong game module
//

#include "main.h"
#include "adc.h"
#include "lcd.h"
#include "pong.h"

#define PADDLE_POS_Y_MAX		24
#define RIGHT_PADDLE_X			124		//x position of right paddle
#define DEG_CENTER				135
#define ACTIVE_RANGE			60

#define ACTIVE_DEG_MAX			(DEG_CENTER + ACTIVE_RANGE)
#define DEG_TO_ACTIVE_CONV		(ACTIVE_RANGE*2 / PADDLE_POS_Y_MAX)

#define UPDATE_TICKS			25		// update every 25 msec


void PongInit(void)
{
	// For now - render the ball in the center of the display
	LcdRenderObject(LCD_OBJ_BALL, 60, 12);
}


void PongClock(void)
{
	static Int UpdateTickCount = UPDATE_TICKS;
	static Int LastLeftPos = -1;	// invalid position => first render
	static Int LastRightPos = -1;	// invalid position => first render
	Int LeftPosition;
	Int RightPosition;

	// Update delay
	if (--UpdateTickCount > 0)
	{
		return;
	}
	UpdateTickCount = UPDATE_TICKS;

	// Code to move find paddle position from ADC converter
	LeftPosition = (ACTIVE_DEG_MAX-AdcLeftDegGet()) / DEG_TO_ACTIVE_CONV;
	RightPosition = (ACTIVE_DEG_MAX-AdcRightDegGet()) / DEG_TO_ACTIVE_CONV;

	if (LeftPosition < 0)
	{
		LeftPosition = 0;
	}
	else if (LeftPosition > PADDLE_POS_Y_MAX)
	{
		LeftPosition = PADDLE_POS_Y_MAX;
	}

	if (RightPosition < 0)
	{
		RightPosition = 0;
	}
	else if (RightPosition > PADDLE_POS_Y_MAX)
	{
		RightPosition = PADDLE_POS_Y_MAX;
	}

	// Erase old paddle position and render the new paddle
	if (LeftPosition!=LastLeftPos)
	{
		if (LastLeftPos != -1)
		{
			LcdRenderObject(LCD_OBJ_ERASE, 0, LastLeftPos);
		}
		LastLeftPos=LeftPosition;
		LcdRenderObject(LCD_OBJ_PADDLE, 0, LeftPosition);
	}
	if (RightPosition!=LastRightPos)
	{
		if (LastRightPos != -1)
		{
			LcdRenderObject(LCD_OBJ_ERASE, RIGHT_PADDLE_X, LastRightPos);
		}
		LastRightPos=RightPosition;
		LcdRenderObject(LCD_OBJ_PADDLE, RIGHT_PADDLE_X, RightPosition);
	}

	// LcdPrintf(0, 3, "%3d         %3d", AdcLeftDegGet(), AdcRightDegGet());
}
