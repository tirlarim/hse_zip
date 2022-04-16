#include <stdio.h>
#include <direct.h>
#include <stdlib.h>

void main(){
	char dir[FILENAME_MAX];
	_getcwd(dir, sizeof(dir));
	FILE* f = fopen("unins.reg","w");
	char def1[] = "Windows Registry Editor Version 5.00\n[-HKEY_CLASSES_ROOT\\*\\shell\\Compress]";
	char def2[] = "Windows Registry Editor Version 5.00\n[-HKEY_CLASSES_ROOT\\.hse_zip]";
	fprintf(f,"%s",def1);
	fprintf(f, "%s", def2);
	fclose(f);
	system("Regedit unins.reg");
}
