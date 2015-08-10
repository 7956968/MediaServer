
#ifdef WIN32
#include <Windows.h>
#include <fstream>
#include <string>

using namespace std;
// _declspec(dllexport)ʹCodeConverter���Դ�dll�е���
class /*_declspec(dllexport)*/ CodeConverter
{
private:
	//��UTF8��ʽ�ļ��еõ�n��������UTF8�ַ�
	static int GetUtf8Array(ifstream &source, char **utf8, size_t n);
public:
	static size_t GB2312ToUnicode(char *source, size_t source_len, short **target);
	static size_t UnicodeToGB2312(short *source, size_t source_len, char **target);
	static size_t UnicodeToUtf8(short *source, size_t source_len, char **target);
	static size_t Utf8ToUnicode(char *source, size_t source_len, short **target);
	static size_t GB2312ToUtf8(char *source, size_t source_len, char **target);

	static size_t Utf8ToGB2312(char *source, size_t source_len, char **target);    
	static size_t Utf8ToGB2312(char *source,  char *target);
};
#endif