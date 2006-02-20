/* GtkSHRuler
 *
 *  Copyright (C) 2000-2005 Alfons Hoogervorst & The Sylpheed Claws Team
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */
 
/* I derived this class from hruler. S in HRuler could be read as
 * Sylpheed (sylpheed.good-day.net), but also [S]ettable HRuler.
 * I basically ripped apart the draw_ticks member of HRuler; it
 * now draws the ticks at ruler->max_size. so gtk_ruler_set_range's
 * last parameter has the distance between two ticks (which is
 * the width of the fixed font character!
 * 
 * -- Alfons
 */

#include <math.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtkhruler.h>
#include "gtkshruler.h"

#define RULER_HEIGHT          14
#define MINIMUM_INCR          5
#define MAXIMUM_SUBDIVIDE     5
#define MAXIMUM_SCALES        10

#define ROUND(x) ((int) ((x) + 0.5))

static void gtk_shruler_class_init   	(GtkSHRulerClass *klass);
static void gtk_shruler_init         	(GtkSHRuler      *hruler);
static void gtk_shruler_draw_ticks 	(GtkRuler        *ruler);
#if 0
static void gtk_shruler_draw_pos      	(GtkRuler        *ruler);
#endif

GType
gtk_shruler_get_type(void)
{
	static GType shruler_type = 0;

  	if ( !shruler_type ) {
   		static const GTypeInfo shruler_info = {
			sizeof (GtkSHRulerClass),

			(GBaseInitFunc) NULL,
			(GBaseFinalizeFunc) NULL,

			(GClassInitFunc) gtk_shruler_class_init,
			(GClassFinalizeFunc) NULL,
			NULL,	/* class_data */

			sizeof (GtkSHRuler),
			0,	/* n_preallocs */
			(GInstanceInitFunc) gtk_shruler_init,
		};
		/* inherit from GtkHRuler */
		shruler_type = g_type_register_static (GTK_TYPE_HRULER, "GtkSHRuler", &shruler_info, (GTypeFlags)0);
	}
	return shruler_type;
}

static void
gtk_shruler_class_init(GtkSHRulerClass * klass)
{
 	GtkWidgetClass * widget_class;
  	GtkRulerClass * hruler_class;

  	widget_class = GTK_WIDGET_CLASS(klass);
  	hruler_class = GTK_RULER_CLASS(klass);

	/* just neglect motion notify events */
  	widget_class->motion_notify_event = NULL /* gtk_shruler_motion_notify */;

  	/* we want the old ruler draw ticks... */
  	/* ruler_class->draw_ticks = gtk_hruler_draw_ticks; */
	hruler_class->draw_ticks = gtk_shruler_draw_ticks;
	
	/* unimplemented draw pos */
	hruler_class->draw_pos = NULL;
/*
  	hruler_class->draw_pos = gtk_shruler_draw_pos;
*/
}

static void
gtk_shruler_init (GtkSHRuler * shruler)
{
	GtkWidget * widget;
	
	widget = GTK_WIDGET (shruler);
	widget->requisition.width = widget->style->xthickness * 2 + 1;
	widget->requisition.height = widget->style->ythickness * 2 + RULER_HEIGHT;
}


GtkWidget*
gtk_shruler_new(void)
{
	return GTK_WIDGET( g_object_new( gtk_shruler_get_type(), NULL ) );
}

static void
gtk_shruler_draw_ticks(GtkRuler *ruler)
{
	GtkWidget *widget;
	GdkGC *gc, *bg_gc;
	gint i;
	gint width, height;
	gint xthickness;
	gint ythickness;
	gint pos;

	g_return_if_fail (ruler != NULL);
	g_return_if_fail (GTK_IS_HRULER (ruler));

	if (!GTK_WIDGET_DRAWABLE (ruler)) 
		return;

	widget = GTK_WIDGET (ruler);
	
	gc = widget->style->fg_gc[GTK_STATE_NORMAL];
	bg_gc = widget->style->bg_gc[GTK_STATE_NORMAL];

	xthickness = widget->style->xthickness;
	ythickness = widget->style->ythickness;

	width = widget->allocation.width;
	height = widget->allocation.height - ythickness * 2;
  
	gtk_paint_box (widget->style, ruler->backing_store,
		       GTK_STATE_NORMAL, GTK_SHADOW_OUT, 
		       NULL, widget, "hruler",
		       0, 0, 
		       widget->allocation.width, widget->allocation.height);

#if 0
	gdk_draw_line (ruler->backing_store, gc,
		       xthickness,
		       height + ythickness,
		       widget->allocation.width - xthickness,
		       height + ythickness);
#endif

	/* assume ruler->max_size has the char width */
	/* i is increment of char_width,  pos is label number
	 * y position is based on height of widget itself */
	for ( i = 0, pos = 0; i < widget->allocation.width - xthickness; i += ruler->max_size, pos++ ) {	
		gint length = height / 8;
	
		if ( pos % 10 == 0 ) length = ( 2 * height / 3 );
		else if ( pos % 5 == 0 ) length = ( height / 3 );
		
		gdk_draw_line(ruler->backing_store, gc,
			      i, height + ythickness,
			      i, height - length);			
		
		if ( pos % 10 == 0 ) {
			gchar buf[8];
			PangoLayout *layout;

			/* draw label */
			g_snprintf(buf, sizeof buf, "%d", pos);

			layout = gtk_widget_create_pango_layout
				(GTK_WIDGET(ruler), buf);

			gdk_draw_layout(ruler->backing_store, gc, i + 2,
					0, layout);

			g_object_unref(layout);
		}
	}
}

/* gtk_ruler_set_pos() - does not work yet, need to reimplement 
 * gtk_ruler_draw_pos(). */
void
gtk_shruler_set_pos(GtkSHRuler * ruler, gfloat pos)
{
	GtkRuler * ruler_;
	g_return_if_fail( ruler != NULL );
	
	ruler_ = GTK_RULER(ruler);
	
	if ( pos < ruler_->lower ) 
		pos = ruler_->lower;
	if ( pos > ruler_->upper )
		pos = ruler_->upper;
	
	ruler_->position = pos;	
	
	/*  Make sure the ruler has been allocated already  */
	if ( ruler_->backing_store != NULL )
		gtk_ruler_draw_pos(ruler_);
}
