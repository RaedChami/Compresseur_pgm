#ifndef TREAT_QTC_FILE_H
#define TREAT_QTC_FILE_H

int read_qtc_file(const char *filename, int *n, int *p, char *time_compression);
void ignore_header(FILE* f);

#endif
