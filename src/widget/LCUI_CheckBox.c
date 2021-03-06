/* ***************************************************************************
 * LCUI_CheckBox.c -- LCUI's CheckBox widget
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
 * LCUI_CheckBox.c -- LCUI 的复选框部件
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
#include LC_WIDGET_H
#include LC_LABEL_H
#include LC_PICBOX_H
#include LC_CHECKBOX_H
#include LC_GRAPH_H
#include LC_RES_H 
#include LC_MISC_H 
#include LC_INPUT_H

void Set_CheckBox_On(LCUI_Widget *widget)
/* 功能：设定复选框为选中状态 */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
			Get_Widget_PrivData(widget); 
	
	check_box->on = IS_TRUE;
	Draw_Widget(widget);
}

void Set_CheckBox_Off(LCUI_Widget *widget)
/* 功能：设定复选框为未选中状态 */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
			Get_Widget_PrivData(widget); 
	
	check_box->on = IS_FALSE;
	Draw_Widget(widget);
}

int Get_CheckBox_Status(LCUI_Widget *widget)
/* 功能：获取复选框的状态 */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
			Get_Widget_PrivData(widget); 
	
	return check_box->on;
}

int CheckBox_Is_On(LCUI_Widget *widget)
/* 功能：检测复选框是否被选中 */
{
	if(IS_TRUE == Get_CheckBox_Status(widget))
		return 1;
	
	return 0;
}

int CheckBox_Is_Off(LCUI_Widget *widget)
/* 功能：检测复选框是否未选中 */
{
	if(IS_TRUE == Get_CheckBox_Status(widget)) return 0;
	return 1;
}

void Switch_CheckBox_Status(LCUI_Widget *widget, void *arg)
/* 
 * 功能：切换复选框的状态
 * 说明：这个状态，指的是打勾与没打勾的两种状态
 *  */
{ 
	if(CheckBox_Is_On(widget)) Set_CheckBox_Off(widget);
	else Set_CheckBox_On(widget); 
}

void CheckBox_Set_ImgBox_Size(LCUI_Widget *widget, LCUI_Size size)
/* 功能：设定复选框中的图像框的尺寸 */
{
	if(size.w <= 0 && size.h <= 0) return;
		
	LCUI_Widget *imgbox = Get_CheckBox_ImgBox(widget);
	Resize_Widget(imgbox, size);
	/* 由于没有布局盒子，不能自动调整部件间的间隔，暂时用这个方法 */
	Set_Widget_Align(imgbox->parent, ALIGN_MIDDLE_LEFT, Pos(size.w, 0));
}

static void CheckBox_Init(LCUI_Widget *widget)
/* 功能：初始化复选框部件的数据 */
{
	LCUI_Widget *container[2];
	LCUI_CheckBox *check_box = (LCUI_CheckBox*)
		Widget_Create_PrivData(widget, sizeof(LCUI_CheckBox));
	
	check_box->on = IS_FALSE;
	/* 初始化图像数据 */ 
	Graph_Init(&check_box->img_off_disable);
	Graph_Init(&check_box->img_off_normal);
	Graph_Init(&check_box->img_off_focus);
	Graph_Init(&check_box->img_off_down);
	Graph_Init(&check_box->img_off_over);
	Graph_Init(&check_box->img_on_disable);
	Graph_Init(&check_box->img_on_normal);
	Graph_Init(&check_box->img_on_focus);
	Graph_Init(&check_box->img_on_down);
	Graph_Init(&check_box->img_on_over);
	/* 创建所需的部件 */
	check_box->label = Create_Widget("label");
	check_box->imgbox = Create_Widget("picture_box");  
	/* 创建两个容器，用于调整上面两个部件的位置 */
	container[0] = Create_Widget(NULL);
	container[1] = Create_Widget(NULL);
	
	/* 启用这些部件的自动尺寸调整的功能 */
	Enable_Widget_Auto_Size(widget);
	Enable_Widget_Auto_Size(container[0]);
	Enable_Widget_Auto_Size(container[1]);
	
	/* 将按钮部件作为label部件的容器 */
	Widget_Container_Add(container[0], check_box->imgbox);
	Widget_Container_Add(container[1], check_box->label);
	Widget_Container_Add(widget, container[0]);
	Widget_Container_Add(widget, container[1]);
	
	/* 调整尺寸 */
	Resize_Widget(check_box->imgbox, Size(20, 20));
	//Resize_Widget(container[0], Size(18,18));
	//Resize_Widget(widget, Size(18,18));
	/* 调整布局 */
	Set_Widget_Align(container[0], ALIGN_MIDDLE_LEFT, Pos(0,0));
	Set_Widget_Align(container[1], ALIGN_MIDDLE_LEFT, Pos(22,0));
	Set_Widget_Align(check_box->imgbox, ALIGN_MIDDLE_CENTER, Pos(0,0));
	Set_Widget_Align(check_box->label, ALIGN_MIDDLE_CENTER, Pos(0,0));
	/* 设置图像框的尺寸模式为拉伸 */
	Set_PictureBox_Size_Mode(check_box->imgbox, SIZE_MODE_STRETCH);
	
	/* 显示之 */
	Show_Widget(check_box->label);
	Show_Widget(check_box->imgbox);
	Show_Widget(container[0]);
	Show_Widget(container[1]);
	/* 关联鼠标左键点击事件 */
	Widget_Clicked_Event_Connect(widget, Switch_CheckBox_Status, NULL);
	/* 响应状态改变 */
	Response_Status_Change(widget);
}

static void Destroy_CheckBox(LCUI_Widget *widget)
/* 功能：释放复选框部件占用的资源 */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
			Get_Widget_PrivData(widget); 
	/* 释放图像数据占用的内存资源 */ 
	Graph_Free(&check_box->img_off_disable);
	Graph_Free(&check_box->img_off_normal);
	Graph_Free(&check_box->img_off_focus);
	Graph_Free(&check_box->img_off_down);
	Graph_Free(&check_box->img_off_over);
	Graph_Free(&check_box->img_on_disable);
	Graph_Free(&check_box->img_on_normal);
	Graph_Free(&check_box->img_on_focus);
	Graph_Free(&check_box->img_on_down);
	Graph_Free(&check_box->img_on_over);
}

static void Exec_Update_CheckBox(LCUI_Widget *widget)
/* 功能：更新复选框的图形数据 */
{
	LCUI_Graph *p;
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
			Get_Widget_PrivData(widget); 
								
	if(Strcmp(&widget->style, "custom") == 0){
		/* 如果为自定义风格，那就使用用户指定的图形，具体可参考按钮部件的处理方法 */ 
		if(widget->enabled == IS_FALSE) 
			widget->status = WIDGET_STATUS_DISABLE;
			
		switch(widget->status){
		case WIDGET_STATUS_NORMAL:
			if(check_box->on == IS_TRUE)
				p = &check_box->img_on_normal;
			else
				p = &check_box->img_off_normal;
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_OVERLAY :
			if(check_box->on == IS_TRUE)
				p = &check_box->img_on_over;
			else
				p = &check_box->img_off_over;
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKING : 
			if(check_box->on == IS_TRUE)
				p = &check_box->img_on_down;
			else
				p = &check_box->img_off_down;
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKED : 
			break;
		case WIDGET_STATUS_FOCUS : 
			break;
		case WIDGET_STATUS_DISABLE :
			if(check_box->on == IS_TRUE)
				p = &check_box->img_on_disable;
			else
				p = &check_box->img_off_disable;
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p); 
			break;
			default :
			break;
		} 
	} else {/* 如果按钮的风格为缺省 */
		Strcpy(&widget->style, "default");
		if(widget->enabled == IS_FALSE) 
			widget->status = WIDGET_STATUS_DISABLE;
		
		/* 先释放PictureBox部件中保存的图形数据的指针 */
		p = Get_PictureBox_Graph(check_box->imgbox);
		Graph_Free(p);
		
		/* 由于本函数在退出后，使用局部变量保存的图形数据会无效，因此，申请内存空间来储存 */
		p = (LCUI_Graph*)calloc(1,sizeof(LCUI_Graph));
		
		switch(widget->status) { 
		case WIDGET_STATUS_NORMAL:
			if(check_box->on == IS_TRUE)
				Load_Graph_Default_CheckBox_On_Normal(p);
			else
				Load_Graph_Default_CheckBox_Off_Normal(p);
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_OVERLAY :
			if(check_box->on == IS_TRUE)
				Load_Graph_Default_CheckBox_On_Selected(p);
			else
				Load_Graph_Default_CheckBox_Off_Selected(p);
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKING : 
			if(check_box->on == IS_TRUE)
				Load_Graph_Default_CheckBox_On_Selected(p);
			else
				Load_Graph_Default_CheckBox_Off_Selected(p);
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
		case WIDGET_STATUS_CLICKED : 
			break;
		case WIDGET_STATUS_FOCUS : 
			break;
		case WIDGET_STATUS_DISABLE :
			if(check_box->on == IS_TRUE)
				Load_Graph_Default_CheckBox_On_Disabled(p);
			else
				Load_Graph_Default_CheckBox_Off_Disabled(p);
			
			Set_PictureBox_Image_From_Graph(check_box->imgbox, p);
			break;
			default : break;
		}
	} 
}

LCUI_Widget *Get_CheckBox_Label(LCUI_Widget *widget)
/* 功能：获取复选框部件中的label部件的指针 */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
			Get_Widget_PrivData(widget); 
	if(NULL == check_box)
		return NULL;
		
	return check_box->label;
}

LCUI_Widget *Get_CheckBox_ImgBox(LCUI_Widget *widget)
/* 功能：获取复选框部件中的PictureBox部件的指针 */
{
	LCUI_CheckBox *check_box = (LCUI_CheckBox *)
			Get_Widget_PrivData(widget); 
	if(NULL == check_box) return NULL;
		
	return check_box->imgbox;
}

void Set_CheckBox_Text(LCUI_Widget *widget, const char *fmt, ...)
/* 功能：设定与复选框部件关联的文本内容 */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *label = Get_CheckBox_Label(widget); 
	
	memset(text, 0, sizeof(text)); 
    
	va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap);

	Set_Label_Text(label, text); 
}

LCUI_Widget *Create_CheckBox_With_Text(const char *fmt, ...)
/* 功能：创建一个带文本内容的复选框 */
{
	char text[LABEL_TEXT_MAX_SIZE];
	LCUI_Widget *widget = Create_Widget("check_box");
	
	memset(text, 0, sizeof(text)); 
    
	va_list ap; 
	va_start(ap, fmt);
	vsnprintf(text, LABEL_TEXT_MAX_SIZE-1, fmt, ap);
	va_end(ap); 
	
	Set_CheckBox_Text(widget, text);
	return widget;
}


void Register_CheckBox()
/* 功能：注册部件类型-窗口至部件库 */
{
	/* 添加几个部件类型 */
	WidgetType_Add("check_box"); 
	
	/* 为部件类型关联相关函数 */ 
	WidgetFunc_Add("check_box", CheckBox_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("check_box", Exec_Update_CheckBox, FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("check_box", Destroy_CheckBox, FUNC_TYPE_DESTROY);
}
