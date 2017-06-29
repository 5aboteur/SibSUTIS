#include "ui.h"

/* Create a pixel buffer for an application icon */
GdkPixbuf *create_pixbuf(const gchar *filename)
{
	GdkPixbuf *pixbuf;
	GError *error = NULL;	// error handle

	// Create a pixels buffer by loading an image from a file
	pixbuf = gdk_pixbuf_new_from_file(filename, &error);

	if (!pixbuf) {
		fprintf(stderr, "%s\n", error->message);
		g_error_free(error);
	}

	return pixbuf;
}

/* Initialize user interface */
void ui_init(struct t_ui *ui)
{
	/* ---------- WINDOW ---------- */

	ui->window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(ui->window), "crc32changer");
	gtk_window_set_position(GTK_WINDOW(ui->window), GTK_WIN_POS_CENTER);
	gtk_window_set_default_size(GTK_WINDOW(ui->window), 530, 180);
	gtk_window_set_resizable(GTK_WINDOW(ui->window), FALSE);
	gtk_container_set_border_width(GTK_CONTAINER(ui->window), 20);

	/* ---------- ICON ---------- */

	ui->icon = create_pixbuf("misc/crc32.png");
	gtk_window_set_icon(GTK_WINDOW(ui->window), ui->icon);

	/* ---------- Table ---------- */

	ui->table = gtk_table_new(3, 4, FALSE);
	gtk_table_set_col_spacings(GTK_TABLE(ui->table), 3);
	gtk_table_set_row_spacing(GTK_TABLE(ui->table), 0, 3);
	gtk_container_add(GTK_CONTAINER(ui->window), ui->table);

	/* ---------- Titles ---------- */
/*
	ui->title1 = gtk_label_new("Filename: ");
	gtk_table_attach(GTK_TABLE(ui->table), ui->title1, 0, 1, 0, 1,
		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 0);

	ui->title2 = gtk_label_new("Get CRC: ");
	gtk_table_attach(GTK_TABLE(ui->table), ui->title2, 0, 1, 1, 2,
		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 0);

	ui->title3 = gtk_label_new("New CRC: ");
	gtk_table_attach(GTK_TABLE(ui->table), ui->title3, 0, 1, 2, 3,
		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 0, 0);
*/
	/* ---------- BUTTONS ---------- */

	ui->button1 = gtk_button_new_with_label("Open");
	gtk_widget_set_size_request(ui->button1, 70, 30);
	gtk_table_attach(GTK_TABLE(ui->table), ui->button1, 0, 2, 0, 1,
		GTK_FILL, GTK_SHRINK, 2, 2);

	ui->button2 = gtk_button_new_with_label("File CRC");
	gtk_widget_set_size_request(ui->button2, 70, 30);
	gtk_table_attach(GTK_TABLE(ui->table), ui->button2, 0, 2, 1, 2,
		GTK_FILL, GTK_SHRINK, 2, 2);

	ui->button3 = gtk_button_new_with_label("New CRC");
	gtk_widget_set_size_request(ui->button3, 70, 30);
	gtk_table_attach(GTK_TABLE(ui->table), ui->button3, 0, 2, 2, 3,
		GTK_FILL, GTK_SHRINK, 2, 2);

	ui->button4 = gtk_button_new_with_label("Change");
	gtk_widget_set_size_request(ui->button4, 70, 30);
	gtk_table_attach(GTK_TABLE(ui->table), ui->button4, 0, 1, 3, 4,
		GTK_FILL, GTK_SHRINK, 2, 2);

	ui->button5 = gtk_button_new_with_label("Clear");
	gtk_widget_set_size_request(ui->button5, 70, 30);
	gtk_table_attach(GTK_TABLE(ui->table), ui->button5, 2, 3, 3, 4,
		GTK_FILL, GTK_SHRINK, 2, 2);

	ui->button6 = gtk_button_new_with_label("Quit");
	gtk_widget_set_size_request(ui->button6, 70, 30);
	gtk_table_attach(GTK_TABLE(ui->table), ui->button6, 1, 2, 3, 4,
		GTK_FILL, GTK_SHRINK, 2, 2);

	/* ---------- ENTRIES ---------- */

	ui->entry1 = gtk_entry_new();
	gtk_widget_set_tooltip_text(ui->entry1, "Input name of the file");
	gtk_table_attach(GTK_TABLE(ui->table), ui->entry1, 2, 3, 0, 1,
		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 2, 2);

	ui->entry2 = gtk_entry_new();
	gtk_widget_set_tooltip_text(ui->entry2, "File CRC");
	gtk_widget_set_sensitive(ui->entry2, FALSE);
	gtk_table_attach(GTK_TABLE(ui->table), ui->entry2, 2, 3, 1, 2,
		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 2, 2);

	ui->entry3 = gtk_entry_new();
	gtk_widget_set_tooltip_text(ui->entry3, "Input new CRC value");
	gtk_table_attach(GTK_TABLE(ui->table), ui->entry3, 2, 3, 2, 3,
		GTK_FILL | GTK_SHRINK, GTK_FILL | GTK_SHRINK, 2, 2);
}
