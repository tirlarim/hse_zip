#include <stdio.h>
#include <direct.h>

#ifdef __cplusplus__
  #include <cstdlib>
#else
  #include <stdlib.h>
#endif

void main(){
	char dir[FILENAME_MAX];
	_getcwd(dir, sizeof(dir));
	FILE* f = fopen("unins.reg","w");
	
	char def[] = "Windows Registry Editor Version 5.00\n[-HKEY_CLASSES_ROOT\\*\\shell\\Compress]";
	
	fprintf(f,"%s",def);

	
	char def1[] = "Windows Registry Editor Version 5.00\n[-HKEY_CLASSES_ROOT\\.hse_zip]";
	fprintf(f, "%s", def1);

	fclose(f);

	system("Regedit unins.reg");
}
