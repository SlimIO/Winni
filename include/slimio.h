#include <windows.h>
#include <string>
#include <iomanip>
#include <sstream>

using namespace std;

namespace Slimio {

    /*
     * Convert GUID to std::string
     */
    string guidToString(GUID guid) {
        char guid_cstr[39];
        snprintf(guid_cstr, sizeof(guid_cstr),
                "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
                guid.Data1, guid.Data2, guid.Data3,
                guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

        return string(guid_cstr);
    }

    /*
     * Retrieve last message error with code of GetLastError()
     */
    string getLastErrorMessage() {
        char err[256];
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
        return string(err);
    }

    /*
     * Cast Byte sequence to std::string
     */
    string byteSeqToString(UCHAR bytes[], size_t n) {
        ostringstream stm;
        stm << hex << uppercase;

        for(size_t i = 0; i < n; ++i) {
            stm << setw(2) << setfill('0') << unsigned(bytes[i]);
        }

        return stm.str();
    }

    /*
     * Cast wchar_t* to std::string 
     */
    string wCharToString(wchar_t* field) {
        wstring ws(field);
        string ret(ws.begin(), ws.end());

        return ret;
    }

    /*
     * PWCharToCChar
     */
    char* PWCharToCChar(PWCHAR field) {
        size_t len = wcslen(field) + 1;
        char* CHARField = new char[len];
        wcstombs(CHARField, field, len);
        return CHARField;
    }

}
