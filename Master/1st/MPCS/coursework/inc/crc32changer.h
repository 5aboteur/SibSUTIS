#ifndef _CRC32_CHANGER_H_
#define _CRC32_CHANGER_H_

#include <gtk/gtk.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "crc32.h"
#include "ui.h"

/* Change file CRC to any desired 32-bits value */
void crc32_changer(GtkWidget *widget, gpointer user_data);

/* Get the length of the file and its CRC value */
int get_file_parameters(char *filename, uint64_t *filelen, uint32_t *filecrc);

/* Inject 4 bytes to the end of a file */
int file_injection(char *filename, uint64_t *filelen, uint32_t *filecrc, uint32_t *crc);

/* Get file name from the entry box */
void get_filename(GtkWidget *widget, gpointer user_data);

/* Get file crc */
void get_filecrc(GtkWidget *widget, gpointer user_data);

/* Get the crc value from the entry box */
void get_crc(GtkWidget *widget, gpointer user_data);

/* Clear entries & user data */
void clear_entries(GtkWidget *widget, gpointer user_data);

#endif
