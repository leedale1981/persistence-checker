#include <iostream>
#include <string>
#include <windows.h>
#include <strsafe.h>

#define MAX_KEY_LENGTH 255
#define MAX_VALUE_NAME 16383

void PrintRegLocation(HKEY, LPCWSTR);

int main(int argc, char* argv[])
{
    std::cout << "Current user run locations:" << std::endl;
    PrintRegLocation(HKEY_CURRENT_USER, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));

    std::cout << "Location machine run locations:" << std::endl;
    PrintRegLocation(HKEY_LOCAL_MACHINE, TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run"));
    
    return 0;
}

void PrintRegLocation(HKEY key, LPCWSTR location)
{
    HKEY hlcuRunKey;
    LONG result = RegOpenKeyEx(
        key, location, 0,
        KEY_READ, &hlcuRunKey);

    if (result == ERROR_FILE_NOT_FOUND)
    {
        std::cout << "Key not found." << std::endl;
    }

    if (result == ERROR_ACCESS_DENIED)
    {
        std::cout << "Not running as administrator." << std::endl;
    }
    
    if (result == ERROR_SUCCESS)
    {
        TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
        DWORD    cchClassName = MAX_PATH;  // size of class string 
        DWORD    cSubKeys=0;               // number of subkeys 
        DWORD    cbMaxSubKey;              // longest subkey size 
        DWORD    cchMaxClass;              // longest class string 
        DWORD    cValues;              // number of values for key 
        DWORD    cchMaxValue;          // longest value name 
        DWORD    cbMaxValueData;       // longest value data 
        DWORD    cbSecurityDescriptor; // size of security descriptor 
        FILETIME ftLastWriteTime;

        TCHAR achValue[MAX_VALUE_NAME]; 
        DWORD cchValue = MAX_VALUE_NAME; 
        
        result = RegQueryInfoKey(
                    hlcuRunKey,                    // key handle 
                    achClass,                // buffer for class name 
                    &cchClassName,           // size of class string 
                    NULL,                    // reserved 
                    &cSubKeys,               // number of subkeys 
                    &cbMaxSubKey,            // longest subkey size 
                    &cchMaxClass,            // longest class string 
                    &cValues,                // number of values for this key 
                    &cchMaxValue,            // longest value name 
                    &cbMaxValueData,         // longest value data 
                    &cbSecurityDescriptor,   // security descriptor 
                    &ftLastWriteTime);

        if (result == ERROR_SUCCESS)
        {
            if (cValues)
            {
                std::cout << "Number of values: " << cValues << std::endl;
                
                for (DWORD index = 0; index < cValues; index++) 
                {
                    char* data = new char[cbMaxValueData];
                    LPDWORD dataSize = (LPDWORD)malloc(cbMaxValueData);
                    data[0] = '\0';
                    cchValue = MAX_VALUE_NAME; 
                    achValue[0] = '\0';
                    
                    result = RegEnumValue(
                        hlcuRunKey,
                        index, 
                        achValue, 
                        &cchValue, 
                        NULL,
                        NULL,
                        LPBYTE(data),
                        dataSize); 
                        
 
                    if (result == ERROR_SUCCESS ) 
                    {
                        std::wstring value_wstr = achValue;
                        std::string value_str(value_wstr.begin(), value_wstr.end());

                        std:std::wstring data_wstr = std::wstring((wchar_t*)data);
                        std::string data_str(data_wstr.begin(), data_wstr.end());
                        
                        std::cout << value_str << " = " << data_str << std::endl;
                    } 
                }
            }
        }
    }
}
