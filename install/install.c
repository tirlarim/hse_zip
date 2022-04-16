#include <stdio.h>
#include <direct.h>
#include <stdlib.h>

void main(){
	char dir[FILENAME_MAX];
	_getcwd(dir, sizeof(dir));
	FILE* f = fopen("ins.reg", "w");
	
	char def1[] = "Windows Registry Editor Version 5.00\n[HKEY_CLASSES_ROOT\\*\\shell\\Compress]\n\"MUIVerb\"=\"Compress\"\n\"Position\"=\"Top\"\n\n[HKEY_CLASSES_ROOT\\*\\shell\\Compress\\command]\n\"\"=\"";
	
	fprintf(f, "%s", def1);

	for (int i=0; i<strlen(dir)-7; ++i) {
		fprintf(f, "%c", dir[i]);
	       if (dir[i] == '\\'){
	       	fprintf(f, "\\");
	       }	       
	}
	
	char def_end[] = "cmake-build-debug\\\\HSE_zip.exe \\\"%1\\\"\"";
	fprintf(f, "%s", def_end);

	char def2[] = "\n[HKEY_CLASSES_ROOT\\.hse_zip\\shell\\Unclench]\n\"MUIVerb\"=\"Unclench\"\n\"Position\"=\"Top\"\n\n[HKEY_CLASSES_ROOT\\.hse_zip\\shell\\Unclench\\command]\n\"\"=\"";
	fprintf(f, "%s", def2);

	for (int i=0; i<strlen(dir)-7; ++i) {
		fprintf(f, "%c", dir[i]);
	       if (dir[i] == '\\'){
	       	fprintf(f, "\\");
	       }	       
	}

	char def_end2[] = "cmake-build-debug\\\\HSE_zip.exe \\\"%1\\\"\"";
	fprintf(f, "%s", def_end2);
	fclose(f);

	system("Regedit ins.reg");
}
