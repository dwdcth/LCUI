/* ***************************************************************************
 * keyboard.c -- keyboard support
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * keyboard.c -- 键盘支持
 *
 * 版权所有 (C) 2012 归属于 
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/

#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_GRAPH_H
#include LC_DRAW_H

#include <termios.h>
#include <unistd.h>
#include <sys/fcntl.h>


static struct termios tm;//, tm_old; 
static int fd = STDIN_FILENO;

int Set_Raw(int t)
{
	if (t > 0) {
		if(tcgetattr(fd, &tm) < 0) {
			return -1; 
		} 
		/* 设置终端为无回显无缓冲模式 */
		tm.c_lflag &= ~(ICANON|ECHO);
		tm.c_cc[VMIN] = 1;
		tm.c_cc[VTIME] = 0;
		if(tcsetattr(fd, TCSANOW, &tm) < 0) {
			return -1; 
		}
		printf("\033[?25l");/* 隐藏光标 */
	} else {
		tm.c_lflag |= ICANON;
		tm.c_lflag |= ECHO;
		tm.c_cc[VMIN] = 1;
		tm.c_cc[VTIME] = 0;
		if(tcsetattr(fd,TCSANOW,&tm)<0) {
			return -1;
		}
		printf("\033[?25h"); /* 显示光标 */ 
	}
	return 0;
}

int Check_Key(void)  
/* 
 * 功能：检测是否有按键输入 
 * 返回值：
 *   1   有按键输入
 *   2   无按键输入
 * */
{
	struct termios oldt;//, newt;  
	int ch;  
	int oldf;  
	tcgetattr(STDIN_FILENO, &oldt);  
	//newt = oldt;  
	//newt.c_lflag &= ~(ICANON | ECHO);  
	//tcsetattr(STDIN_FILENO, TCSANOW, &newt);  
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);  
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);  
	ch = getchar();  /* 获取一个字符 */
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);  
	fcntl(STDIN_FILENO, F_SETFL, oldf);  
	if(ch != EOF)  
	{/* 如果字符有效，将字符放回输入缓冲区中 */
		ungetc(ch, stdin);  
		return 1;  
	} 
	return 0;
}


int Get_Key(void)
/* 功能：获取被按下的按键的键值 */
{ 
	int k,c;
	static int input, count = 0;
	++count;
	k = fgetc(stdin); /* 获取输入缓冲中的字符 */
	input += k; 
	if(Check_Key()) {/* 如果还有字符在缓冲中 */
		Get_Key(); /* 递归调用 */
	}
	c = input;
	--count;
	if(count == 0) {/* 递归结束后就置0 */
		input = 0;
	}
	if(c == 3) {
		exit(1);
	}
	return c; 
}

int Find_Pressed_Key(int key)
/*
 * 功能：检测指定键值的按键是否处于按下状态
 * 返回值：
 *   1 存在
 *   0 不存在
 **/
{
	int t;
	int i, total;
	
	total = Queue_Get_Total(&LCUI_Sys.press_key);
	for(i=0; i<total; ++i) {
		t = *(int*)Queue_Get(&LCUI_Sys.press_key, i);
		if(t == key) {
			return 1;
		}
	}
	return 0;
}

int debug_mark = 0;
static int disable_key = IS_FALSE;
extern int LCUI_Active();

#include LC_DISPLAY_H

static void * Handle_Key_Input ()
/* 功能：处理按键输入 */
{
	int key; 
	int sleep_time = 1500;
	
	LCUI_Rect area;
	LCUI_Graph graph;
	Graph_Init(&graph);
	area = Rect((Get_Screen_Width()-320)/2, (Get_Screen_Height()-240)/2, 320, 240);
	while (LCUI_Active()) {
		if (Check_Key ()) {/* 如果有按键输入 */ 
			sleep_time = 1500;
			key = Get_Key ();
			if(key == 'd') debug_mark = 1;
			else  debug_mark = 0;
			#define __NEED_CATCHSCREEN__
			#ifdef __NEED_CATCHSCREEN__
			if(key == 'c')
			{//当按下c键后，可以进行截图，只截取指定区域的图形
				time_t rawtime;
				struct tm * timeinfo;
				char filename[100];
				time ( &rawtime );
				timeinfo = localtime ( &rawtime ); /* 获取系统当前时间 */ 
				sprintf(filename, "%4d-%02d-%02d-%02d-%02d-%02d.png",
					timeinfo->tm_year+1900, timeinfo->tm_mon+1, 
					timeinfo->tm_mday, timeinfo->tm_hour, 
					timeinfo->tm_min, timeinfo->tm_sec
				);
				Catch_Screen_Graph_By_FB( area, &graph );
				write_png(filename, &graph);
			}
			#endif
			/* 处理程序中关联的按键事件 */
			Handle_Event(&LCUI_Sys.key_event, key);
		}
		else if(disable_key == IS_TRUE) break;
		else {/* 停顿一段时间 */
			usleep(sleep_time);
			sleep_time+=1500;
			if(sleep_time >= 30000) {
				sleep_time = 30000;
			}
		}
	}
	//Graph_Free(&graph);
	disable_key = IS_FALSE; 
	pthread_exit (NULL);
}

int Enable_Key_Input()
/* 功能：启用按键输入处理 */
{
	if(disable_key == IS_FALSE) {
		Set_Raw(1);/* 设置终端属性 */ 
		/* 创建一个线程，用于处理按键输入 */
		return pthread_create (&LCUI_Sys.key_thread, 
				NULL, Handle_Key_Input, NULL);
	}
	return 0;
}

int Disable_Key_Input()
/* 功能：撤销按键输入处理 */
{
	if(disable_key == IS_FALSE) {
		disable_key = IS_TRUE;
		Set_Raw(0);/* 恢复终端属性 */
		return pthread_join (LCUI_Sys.key_thread, NULL);/* 等待线程结束 */
	}
	return 0;
}
/*************************** Key End **********************************/
