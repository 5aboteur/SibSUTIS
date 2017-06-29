#include "crc32changer.h"

/* Change file CRC to any desired 32-bits value */
void crc32_changer(GtkWidget *widget, gpointer user_data)
{
	// Get user data
	struct t_ud *ud = (struct t_ud *)user_data;

	fprintf(stdout, "\nChanging %x to %x\n ...\n",
		ud->filecrc, ud->crc);

	// Inject new CRC value
	if (file_injection(ud->filename, &ud->filelen, &ud->filecrc, &ud->crc))
		return;

	int isCorrect = 0;

	if (get_file_parameters(ud->filename, &ud->filelen, &ud->filecrc))
		return;

	// Verify injection
	if ((isCorrect = (ud->filecrc == ud->crc)))
		fprintf(stdout, "Success! Changed CRC-32: %x\n",
			ud->filecrc);
	else
		fprintf(stdout, "Fail! Changed CRC-32: %x\n",
			ud->filecrc);

	return;
}

/* Get the length of the file and its CRC value */
int get_file_parameters(char *filename, uint64_t *filelen, uint32_t *filecrc)
{
	// Stores read bytes
	uint64_t n;

	FILE *fp;

	if (!(fp = fopen(filename, "r+b"))) {
		fprintf(stderr, "Cannot open the file '%s'.\n", filename);
		return -1;
	}

	// Set to default
	*filelen = 0;
	*filecrc = 0xFFFFFFFF;

	// Set to start position
	rewind(fp);

	while (!feof(fp)) {
		// Buffer for block reading
		uint8_t buf[BUFSIZ];

		// Read n bytes from the file
		n = fread(buf, sizeof(buf[0]), BUFSIZ, fp);

		if (ferror(fp)) {
			fprintf(stderr, "Error in file reading.\n");
			fclose(fp);
			return -1;
		}

		// Get CRC value for that block
		crc32(&(*filecrc), buf, n);

		// Increase length to n bytes
		*filelen += n;
	}

	// Do the final xor
	*filecrc ^= 0xFFFFFFFF;

	fclose(fp);
	return 0;
}

/* Inject 4 bytes to the end of a file */
int file_injection(char *filename, uint64_t *filelen, uint32_t *filecrc, uint32_t *crc)
{
	FILE *fp;

	if (!(fp = fopen(filename, "r+b"))) {
		fprintf(stderr, "Cannot open the file '%s'.\n", filename);
		return -1;
	}

	// Set to start position
	rewind(fp);

	// Set to offset position
	fseek(fp, (long)(*filelen), SEEK_CUR);

	// Inverse our desired CRC value because of reversed table
	uint32_t crc1 = *crc ^ 0xFFFFFFFF;

	// Store indices of bytes that fit to our CRC value
	uint32_t idx[4];
	for (int i = 3; i >= 0; --i) {
		idx[i] = rcrc32tbl[crc1 >> 24];
		crc1 = (crc1 ^ crc32tbl[idx[i]]) << 8;
	}

	// Inverse file CRC
	uint32_t crc2 = *filecrc ^ 0xFFFFFFFF;

	// Bytes we want to append
	uint8_t byte[4];

	// Add 4 bytes to set our CRC value
	for (int i = 0; i < 4; ++i) {
		byte[i] = (crc2 ^ idx[i]) & 0xFF;
		crc2 = (crc2 >> 8) ^ crc32tbl[byte[i] ^ (crc2 & 0xFF)];

		// Add bytes to the end of the file
		fputc(byte[i], fp);
	}

	fprintf(stdout, "Patched bytes: 0x%.2x, 0x%.2x, 0x%.2x, 0x%.2x\n",
		byte[0], byte[1], byte[2], byte[3]);

	fclose(fp);
	return 0;
}

/* Get file name from the entry box */
void get_filename(GtkWidget *widget, gpointer user_data)
{
	struct t_ui *ui = (struct t_ui *)user_data;

	ui->ud->filename = (char *)gtk_entry_get_text(GTK_ENTRY(ui->entry1));

	if (get_file_parameters(ui->ud->filename, &ui->ud->filelen, &ui->ud->filecrc))
		return;

	fprintf(stdout, "File name: %s\n", ui->ud->filename);
	fprintf(stdout, "Length: %llu\n", ui->ud->filelen);
}

/* Set file CRC value to the entry box */
void get_filecrc(GtkWidget *widget, gpointer user_data)
{
	struct t_ui *ui = (struct t_ui *)user_data;

	char *buf = malloc(BUFSIZ * sizeof(char));

	sprintf(buf, "%x", ui->ud->filecrc);
	gtk_entry_set_text(GTK_ENTRY(ui->entry2), buf);

	fprintf(stdout, "File CRC: %x\n", ui->ud->filecrc);

	free(buf);
}

/* Get the CRC value from the entry box */
void get_crc(GtkWidget *widget, gpointer user_data)
{
	struct t_ui *ui = (struct t_ui *)user_data;

	if (sscanf(gtk_entry_get_text(GTK_ENTRY(ui->entry3)), "%x", &(ui->ud->crc)) != 1) {
		fprintf(stderr, "Incorrect CRC value. Type another one!\n");
		return;
	}

	fprintf(stdout, "New CRC value: %x\n", ui->ud->crc);
}

/* Clear entries & user data */
void clear_entries(GtkWidget *widget, gpointer user_data)
{
	struct t_ui *ui = (struct t_ui *)user_data;

	gtk_entry_set_text(GTK_ENTRY(ui->entry1), "");
	gtk_entry_set_text(GTK_ENTRY(ui->entry2), "");
	gtk_entry_set_text(GTK_ENTRY(ui->entry3), "");

	ui->ud->filename = "";
	ui->ud->filelen = 0;
	ui->ud->filecrc = 0;
	ui->ud->crc = 0;

	fprintf(stdout, "All cleared.\n");
}
