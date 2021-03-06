/* ***************************************************************************
 * LCUI_Label.c -- LCUI's Label widget
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
 * LCUI_Label.c -- LCUI 的文本标签部件
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

//#define DEBUG
#include <LCUI_Build.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_MISC_H 
#include LC_GRAPH_H
#include LC_LABEL_H

/**************************** 私有函数 **********************************/
static void Label_Init(LCUI_Widget *widget)
/* 功能：初始化label部件数据 */
{
	LCUI_TextLayer *layer;
	
	layer = Widget_Create_PrivData( widget, sizeof(LCUI_Label) );
	TextLayer_Init( layer );
	widget->private_data = layer;
	Enable_Widget_Auto_Size( widget );
	TextLayer_Using_StyleTags( layer, TRUE );
}

static void Destroy_Label(LCUI_Widget *widget)
/* 功能：释放label部件占用的资源 */
{
	LCUI_TextLayer *layer;
	layer = Get_Widget_PrivData( widget );
	Destroy_TextLayer( layer );
}

static void Refresh_Label_FontBitmap(LCUI_Widget *widget)
/* 功能：刷新label部件内的字体位图 */
{
	LCUI_TextLayer *layer;
	
	layer = Get_Widget_PrivData( widget );
	TextLayer_Refresh( layer ); 
}

static void Update_Label(LCUI_Widget *widget)
/* 功能：更新label部件 */
{
	int mode; 
	LCUI_Size max;
	LCUI_TextLayer *layer;
	
	layer = Get_Widget_PrivData( widget );
	max = TextLayer_Get_Size( layer );  
	if( widget->auto_size && Size_Cmp( max, widget->size ) != 0 ) {
		/* 如果开启了自动调整大小,并且尺寸有改变 */ 
		Resize_Widget(widget, max );
		Refresh_Widget(widget);
		return;
	}
	if(!Graph_Valid(&widget->background_image)) {
		mode = GRAPH_MIX_FLAG_REPLACE; /* 替换模式 */
	} else {
		mode = GRAPH_MIX_FLAG_OVERLAY; /* 叠加模式 */ 
	}
	layer = Get_Widget_PrivData( widget );
	TextLayer_Draw( widget, layer, mode );
}

static void Redraw_Label(LCUI_Widget *widget)
/* 重绘Label部件 */
{
	Refresh_Label_FontBitmap(widget);
	Update_Label(widget);
}

/****************************** END ***********************************/

/************************* Label部件操作 *******************************/
void Set_Label_Text(LCUI_Widget *widget, const char *fmt, ...)
/* 功能：设定与标签关联的文本内容 */
{
	char text[LABEL_TEXT_MAX_SIZE];
	memset(text, 0, sizeof(text)); 
    
	LCUI_TextLayer *layer; 
	layer = Get_Widget_PrivData( widget );
	
	va_list ap;
	va_start( ap, fmt );
	vsnprintf(text, LABEL_TEXT_MAX_SIZE, fmt, ap);
	va_end( ap ); 
	 
	TextLayer_Text( layer, text );
	Update_Label( widget ); 
}

int Set_Label_TextStyle( LCUI_Widget *widget, LCUI_TextStyle style )
/* 为Label部件内显示的文本设定文本样式 */
{
	LCUI_TextLayer *layer;
	layer = Get_Widget_PrivData( widget );
	TextLayer_Text_Set_Default_Style( layer, style );
	Update_Label( widget );
	return 0;
}
/***************************** END ************************************/


void Register_Label()
/* 功能：注册部件类型-文本标签至部件库 */
{
	/* 添加几个部件类型 */
	WidgetType_Add("label");
	
	/* 为部件类型关联相关函数 */
	WidgetFunc_Add("label",	Label_Init,	FUNC_TYPE_INIT);
	WidgetFunc_Add("label",	Redraw_Label,	FUNC_TYPE_UPDATE); 
	WidgetFunc_Add("label", Destroy_Label,	FUNC_TYPE_DESTROY);
}
