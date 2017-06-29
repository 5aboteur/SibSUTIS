#include "crc32.h"
#include "crc32changer.h"
#include "ui.h"

int main(int argc, char *argv[])
{
	// Main window content
	struct t_ui *ui =
		(struct t_ui *)malloc(sizeof(struct t_ui));

	// User data
	ui->ud = malloc(sizeof(struct t_ud));

	// Init GTK+
	gtk_init(&argc, &argv);

	// Init UI
	ui_init(ui);

	// Display the window
	gtk_widget_show_all(ui->window);

	// Get file name from the entry box
	g_signal_connect(ui->button1, "clicked",
		G_CALLBACK(get_filename), (gpointer)ui);

	// Get file CRC
	g_signal_connect(ui->button2, "clicked",
		G_CALLBACK(get_filecrc), (gpointer)ui);

	// Set user CRC
	g_signal_connect(ui->button3, "clicked",
		G_CALLBACK(get_crc), (gpointer)ui);

	// Change file CRC
	g_signal_connect(ui->button4, "clicked",
		G_CALLBACK(crc32_changer), (gpointer)ui->ud);

	// Clear entries
	g_signal_connect(ui->button5, "clicked",
		G_CALLBACK(clear_entries), (gpointer)ui);

	// Quit the program
	g_signal_connect(ui->button6, "clicked",
		G_CALLBACK(gtk_main_quit), NULL);

	// Connect window to the destroy signal
	g_signal_connect(ui->window, "destroy",
		G_CALLBACK(gtk_main_quit), NULL);

	// Decrease the ref counter of an object
	g_object_unref(ui->icon);

	// GTK+ events loop
	gtk_main();

	// Mem free
	free(ui->ud);
	free(ui);

	fprintf(stderr, "Quit.\n");

	return 0;
}
