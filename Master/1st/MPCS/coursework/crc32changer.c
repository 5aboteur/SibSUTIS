#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "crc32.h"

void crc32_changer(FILE *fp, uint64_t offset, uint32_t crc);

/* --------------------------------------------------------- */

int main(int argc, char *argv[])
{
	if (argc != 4) {
		fprintf(stderr, "Arguments: filename, offset, crc32.\n");
		exit(EXIT_FAILURE);
	}

	// Pointer to the file we want to change
	FILE *fp;

	if (!(fp = fopen(argv[1], "r+b"))) {
		fprintf(stderr, "Cannot open the file.\n");
		exit(EXIT_FAILURE);
	}

	// Our byte offset
	uint64_t offset;

	// Parse offset value from cmd as uint64
	if (sscanf(argv[2], "%llu", &offset) != 1) {
		fprintf(stderr, "Incorrect offset. Type another one!\n");
		exit(EXIT_FAILURE);
	}

	// Our new crc value
	uint32_t crc;

	// Parse crc value from cmd as uint32
	if (sscanf(argv[3], "%x", &crc) != 1) {
		fprintf(stderr, "Incorrect crc32. Type another one!\n");
		exit(EXIT_FAILURE);
	}

	// If everything's OK, try to change the crc
	crc32_changer(fp, offset, crc);

	return 0;
}

/* This function change file's CRC-32 to whatever you want */
void crc32_changer(FILE *fp, uint64_t offset, uint32_t crc)
{
	// For length of the file
	uint64_t filelen = 0;

	// For CRC-32 value
	uint32_t filecrc = 0xFFFFFFFF;

	// For read bytes
	uint64_t n;

	// Set to start position
	rewind(fp);

	// Get the length of the file and its CRC-32
	while (!feof(fp)) {
		// Buffer for block reading
		uint8_t buf[BUFSIZ];

		// Read n bytes from the file
		n = fread(buf, sizeof(buf[0]), BUFSIZ, fp);

		if (ferror(fp)) {
			fprintf(stderr, "Error in file reading.\n");
			fclose(fp);
			return;
		}

		// Get CRC value for that block
		crc32(&filecrc, buf, n);

		// Increase length to n bytes
		filelen += n;
	}

	// Do the final xor
	filecrc ^= 0xFFFFFFFF;

	if (offset + 4 > filelen) {
		fprintf(stderr, "Addition is out of the file's range.\n");
		fclose(fp);
		return;
	}

	fprintf(stdout, "CRC-32: %x >>> %x\n ...\n", filecrc, crc);

	/* ---------- Inject the new CRC-32 value ---------- */

	// Set to start position
	rewind(fp);

	// Set to offset position
	fseek(fp, (long)offset, SEEK_CUR);

	// Inverse our desired CRC-32 value because of reversed table
	uint32_t crc1 = crc ^ 0xFFFFFFFF;

	// Store indices of bytes that fit to our CRC-32 value
	uint32_t idx[4];
	for (int i = 3; i >= 0; --i) {
		idx[i] = rcrc32tbl[crc1 >> 24];
		crc1 = (crc1 ^ crc32tbl[idx[i]]) << 8;
	}

	// Inverse file's CRC-32  
	uint32_t crc2 = filecrc ^ 0xFFFFFFFF;

	// Bytes we want to sub for
	uint8_t byte[4];

	// Change 4 bytes to set our CRC-32 value
	for (int i = 0; i < 4; ++i) {
		byte[i] = (crc2 ^ idx[i]) & 0xFF;
		crc32b(&crc2, byte[i]);

		// Insert changed byte
		if (fputc(byte[i], fp) == EOF) {
			fprintf(stderr, "EOF was reached. Some bytes may not be changed!\n");
			fclose(fp);
			return;
		}
	}

	/* ---------- Verify the new CRC-32 value ---------- */

	int isCorrect = 0;

	filelen = 0;
	filecrc = 0xFFFFFFFF;

	// Set to start position
	rewind(fp);

	// Get the length of the file and its CRC-32
	while (!feof(fp)) {
		// Buffer for block reading
		uint8_t buf[BUFSIZ];

		// Read n bytes from the file
		n = fread(buf, sizeof(buf[0]), BUFSIZ, fp);

		if (ferror(fp)) {
			fprintf(stderr, "Error in file reading.\n");
			fclose(fp);
			return;
		}

		// Get CRC value for that block
		crc32(&filecrc, buf, n);

		// Increase length to n bytes
		filelen += n;
	}

	// Do the final xor
	filecrc ^= 0xFFFFFFFF;

	if ((isCorrect = (filecrc == crc)))
		fprintf(stdout, "Success! Changed CRC-32: %x\n", filecrc);
	else
		fprintf(stdout, "Fail! Changed CRC-32: %x\n", filecrc);

	fclose(fp);
	return;
}
