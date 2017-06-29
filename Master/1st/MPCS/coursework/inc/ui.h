#ifndef _UI_H_
#define _UI_H_

#include <gtk/gtk.h>
#include <stdint.h>
#include <stdlib.h>

/* User data storage */
struct t_ud {
	char	*filename;
	uint64_t filelen;
	uint32_t filecrc;
	uint32_t crc;
};

/* UI entity */
struct t_ui {
	GtkWidget *window;		// window entity
	GtkWidget *table;		// table container

//	GtkWidget *title1;		// filename
//	GtkWidget *title2;		// get crc
//	GtkWidget *title3;		// new crc

	GtkWidget *button1;		// filename
	GtkWidget *button2;		// get crc
	GtkWidget *button3;		// new crc

	GtkWidget *button4;		// change
	GtkWidget *button5;		// clear
	GtkWidget *button6;		// quit

	GtkWidget *entry1;		// file name
	GtkWidget *entry2;		// get crc
	GtkWidget *entry3;		// new crc

	GtkWidget *dialog;		// for file selection

	GdkPixbuf *icon;		// app icon

	struct t_ud *ud;		// user data
};

/* Create a pixel buffer for an application icon */
GdkPixbuf *create_pixbuf(const gchar *filename);

/* Initialize user interface */
void ui_init(struct t_ui *ui);

#endif
