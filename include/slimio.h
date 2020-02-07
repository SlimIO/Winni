#include <windows.h>
#include <string>
#include <iomanip>
#include <sstream>

namespace Slimio {

    /*
     * Convert GUID to std::string
     */
    std::string guidToString(GUID guid) {
        char guid_cstr[39];
        snprintf(guid_cstr, sizeof(guid_cstr),
                "{%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x}",
                guid.Data1, guid.Data2, guid.Data3,
                guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
                guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);

        return std::string(guid_cstr);
    }

    /*
     * Retrieve last message error with code of GetLastError()
     */
    std::string getLastErrorMessage() {
        char err[256];
        FormatMessageA(FORMAT_MESSAGE_FROM_SYSTEM, NULL, GetLastError(),
                    MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), err, 255, NULL);
        return std::string(err);
    }

    /*
     * Cast Byte sequence to std::string
     */
    std::string byteSeqToString(UCHAR bytes[], size_t n) {
        std::ostringstream stm;
        stm << std::hex << std::uppercase;

        for(size_t i = 0; i < n; ++i) {
            stm << std::setw(2) << std::setfill('0') << unsigned(bytes[i]);
        }

        return stm.str();
    }

    /*
     * Cast wchar_t* to std::string
     */
    std::string wCharToString(wchar_t* field) {
        std::wstring ws(field);
        std::string ret(ws.begin(), ws.end());

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
