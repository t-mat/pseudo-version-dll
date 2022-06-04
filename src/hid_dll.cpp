#include "common.hpp"
#if defined(EXPORT_HID_DLL_FUNCTIONS) && (EXPORT_HID_DLL_FUNCTIONS)
#include "hid_dll.hpp"
#include <hidsdi.h>

///////////////////////////////////////////////////////////////
static HMODULE hModuleDll = nullptr;
static const wchar_t dllFname[] = L"hid";

namespace hid_dll {

bool checkFname(const wchar_t* fname) {
    return _wcsicmp(fname, dllFname) == 0;
}

void loadGenuineDll(const wchar_t* systemDirectory) {
    DEBUG_TRACE(L"loadGenuineDll : begin, dllFname=%s", dllFname);
    unloadGenuineDll();

    // systemDirectory : "C:\Windows\System32"
    // fullpathDllName : "C:\Windows\System32\<dllFname>.dll"
    wchar_t fullpathDllName[MAX_PATH];
    swprintf_s(fullpathDllName, std::size(fullpathDllName), L"%s\\%s.dll", systemDirectory, dllFname);
    DEBUG_TRACE(L"loadGenuineDll : fullpathDllName = %s", fullpathDllName);

    // Load "genuine" DLL
    hModuleDll = LoadLibraryW(fullpathDllName);
    DEBUG_TRACE(L"hModuleDll(%s) = 0x%p", fullpathDllName, hModuleDll);

    DEBUG_TRACE(L"loadGenuineDll : end");
}

void unloadGenuineDll() {
    if(hModuleDll == nullptr) {
        return;
    }
    FreeLibrary(hModuleDll);
    hModuleDll = nullptr;
}

} // namespace hid_dll


///////////////////////////////////////////////////////////////
template<typename T>
void setup(T*& funcPtr, const char* funcName) {
    if(funcPtr != nullptr) {
        return;
    }
    funcPtr = reinterpret_cast<T*>(GetProcAddress(hModuleDll, funcName));
}

#define D(funcname, ...)            \
    static decltype(funcname)* p;   \
    setup(p, #funcname);            \
    return p(__VA_ARGS__);          \
    __pragma(comment(linker, "/EXPORT:" __FUNCTION__ "=" __FUNCDNAME__))


// https://github.com/reactos/reactos/blob/7033ab18dfa936168a279913035587858dd143e7/dll/win32/hid/hid.c#L376
extern "C" ULONG WINAPI HidD_Hello(PCHAR Buffer, ULONG BufferLength) {
    D(HidD_Hello, Buffer, BufferLength);
}

extern "C" BOOLEAN WINAPI HidD_GetAttributes(HANDLE HidDeviceObject, PHIDD_ATTRIBUTES Attributes) {
    D(HidD_GetAttributes, HidDeviceObject, Attributes);
}

extern "C" void WINAPI HidD_GetHidGuid(LPGUID HidGuid) {
    D(HidD_GetHidGuid, HidGuid);
}

extern "C" BOOLEAN WINAPI HidD_GetPreparsedData(HANDLE HidDeviceObject, PHIDP_PREPARSED_DATA * PreparsedData) {
    D(HidD_GetPreparsedData, HidDeviceObject, PreparsedData);
}

extern "C" BOOLEAN WINAPI HidD_FreePreparsedData(PHIDP_PREPARSED_DATA PreparsedData) {
    D(HidD_FreePreparsedData, PreparsedData);
}

extern "C" BOOLEAN WINAPI HidD_FlushQueue(HANDLE HidDeviceObject) {
    D(HidD_FlushQueue, HidDeviceObject);
}

extern "C" BOOLEAN WINAPI HidD_GetConfiguration(HANDLE HidDeviceObject, PHIDD_CONFIGURATION Configuration, ULONG ConfigurationLength) {
    D(HidD_GetConfiguration, HidDeviceObject, Configuration, ConfigurationLength);
}

extern "C" BOOLEAN WINAPI HidD_SetConfiguration(HANDLE HidDeviceObject, PHIDD_CONFIGURATION Configuration, ULONG ConfigurationLength) {
    D(HidD_SetConfiguration, HidDeviceObject, Configuration, ConfigurationLength);
}

extern "C" BOOLEAN WINAPI HidD_GetFeature(HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength) {
    D(HidD_GetFeature, HidDeviceObject, ReportBuffer, ReportBufferLength);
}

extern "C" BOOLEAN WINAPI HidD_SetFeature(HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength) {
    D(HidD_SetFeature, HidDeviceObject, ReportBuffer, ReportBufferLength);
}

extern "C" BOOLEAN WINAPI HidD_GetInputReport(HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength) {
    D(HidD_GetInputReport,  HidDeviceObject, ReportBuffer, ReportBufferLength);
}

extern "C" BOOLEAN WINAPI HidD_SetOutputReport(HANDLE HidDeviceObject, PVOID ReportBuffer, ULONG ReportBufferLength) {
    D(HidD_SetOutputReport,  HidDeviceObject, ReportBuffer, ReportBufferLength);
}

extern "C" BOOLEAN WINAPI HidD_GetNumInputBuffers(HANDLE HidDeviceObject, PULONG NumberBuffers) {
    D(HidD_GetNumInputBuffers, HidDeviceObject, NumberBuffers);
}

extern "C" BOOLEAN WINAPI HidD_SetNumInputBuffers(HANDLE HidDeviceObject, ULONG NumberBuffers) {
    D(HidD_SetNumInputBuffers, HidDeviceObject, NumberBuffers);
}

extern "C" BOOLEAN WINAPI HidD_GetPhysicalDescriptor(HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength) {
    D(HidD_GetPhysicalDescriptor, HidDeviceObject, Buffer, BufferLength);
}

extern "C" BOOLEAN WINAPI HidD_GetManufacturerString(HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength) {
    D(HidD_GetManufacturerString, HidDeviceObject, Buffer, BufferLength);
}

extern "C" BOOLEAN WINAPI HidD_GetProductString(HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength) {
    D(HidD_GetProductString, HidDeviceObject, Buffer, BufferLength);
}

extern "C" BOOLEAN WINAPI HidD_GetIndexedString(HANDLE HidDeviceObject, ULONG StringIndex, PVOID Buffer, ULONG BufferLength) {
    D(HidD_GetIndexedString, HidDeviceObject, StringIndex, Buffer, BufferLength);
}

extern "C" BOOLEAN WINAPI HidD_GetSerialNumberString(HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength) {
    D(HidD_GetSerialNumberString, HidDeviceObject, Buffer, BufferLength);
}

extern "C" BOOLEAN WINAPI HidD_GetMsGenreDescriptor(HANDLE HidDeviceObject, PVOID Buffer, ULONG BufferLength) {
    D(HidD_GetMsGenreDescriptor, HidDeviceObject, Buffer, BufferLength);
}

extern "C" NTSTATUS WINAPI HidP_GetCaps(PHIDP_PREPARSED_DATA PreparsedData, PHIDP_CAPS Capabilities) {
    D(HidP_GetCaps, PreparsedData, Capabilities);
}

extern "C" NTSTATUS WINAPI HidP_GetLinkCollectionNodes(PHIDP_LINK_COLLECTION_NODE LinkCollectionNodes, PULONG LinkCollectionNodesLength, PHIDP_PREPARSED_DATA PreparsedData) {
    D(HidP_GetLinkCollectionNodes, LinkCollectionNodes, LinkCollectionNodesLength, PreparsedData);
}

extern "C" NTSTATUS WINAPI HidP_GetSpecificButtonCaps(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, PHIDP_BUTTON_CAPS ButtonCaps, PUSHORT ButtonCapsLength, PHIDP_PREPARSED_DATA PreparsedData) {
    D(HidP_GetSpecificButtonCaps, ReportType, UsagePage, LinkCollection, Usage, ButtonCaps, ButtonCapsLength, PreparsedData);
}

extern "C" NTSTATUS WINAPI HidP_GetButtonCaps(HIDP_REPORT_TYPE ReportType, PHIDP_BUTTON_CAPS ButtonCaps, PUSHORT ButtonCapsLength, PHIDP_PREPARSED_DATA PreparsedData) {
    D(HidP_GetButtonCaps, ReportType, ButtonCaps, ButtonCapsLength, PreparsedData);
}

extern "C" NTSTATUS WINAPI HidP_GetSpecificValueCaps(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, PHIDP_VALUE_CAPS ValueCaps, PUSHORT ValueCapsLength, PHIDP_PREPARSED_DATA PreparsedData) {
    D(HidP_GetSpecificValueCaps, ReportType, UsagePage, LinkCollection, Usage, ValueCaps, ValueCapsLength, PreparsedData);
}

extern "C" NTSTATUS WINAPI HidP_GetValueCaps(HIDP_REPORT_TYPE ReportType, PHIDP_VALUE_CAPS ValueCaps, PUSHORT ValueCapsLength, PHIDP_PREPARSED_DATA PreparsedData) {
    D(HidP_GetValueCaps, ReportType, ValueCaps, ValueCapsLength, PreparsedData);
}

extern "C" NTSTATUS WINAPI HidP_GetExtendedAttributes(HIDP_REPORT_TYPE ReportType, USHORT DataIndex, PHIDP_PREPARSED_DATA PreparsedData, PHIDP_EXTENDED_ATTRIBUTES Attributes, PULONG LengthAttributes) {
    D(HidP_GetExtendedAttributes, ReportType, DataIndex, PreparsedData, Attributes, LengthAttributes);
}

extern "C" NTSTATUS WINAPI HidP_InitializeReportForID(HIDP_REPORT_TYPE ReportType, UCHAR ReportID, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_InitializeReportForID, ReportType, ReportID, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_SetData(HIDP_REPORT_TYPE ReportType, PHIDP_DATA DataList, PULONG DataLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_SetData, ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_GetData(HIDP_REPORT_TYPE ReportType, PHIDP_DATA DataList, PULONG DataLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_GetData, ReportType, DataList, DataLength, PreparsedData, Report, ReportLength);
}

extern "C" ULONG WINAPI HidP_MaxDataListLength(HIDP_REPORT_TYPE ReportType, PHIDP_PREPARSED_DATA PreparsedData) {
    D(HidP_MaxDataListLength, ReportType, PreparsedData);
}

extern "C" NTSTATUS WINAPI HidP_SetUsages(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, PUSAGE UsageList, PULONG UsageLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength ) {
    D(HidP_SetUsages, ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_UnsetUsages(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, PUSAGE UsageList, PULONG UsageLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_UnsetUsages, ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_GetUsages(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, PUSAGE UsageList, PULONG UsageLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_GetUsages, ReportType, UsagePage, LinkCollection, UsageList, UsageLength, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_GetUsagesEx(HIDP_REPORT_TYPE ReportType, USHORT LinkCollection, PUSAGE_AND_PAGE ButtonList, ULONG * UsageLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_GetUsagesEx, ReportType, LinkCollection, ButtonList, UsageLength, PreparsedData, Report, ReportLength);
}

extern "C" ULONG WINAPI HidP_MaxUsageListLength(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, PHIDP_PREPARSED_DATA PreparsedData) {
    D(HidP_MaxUsageListLength, ReportType, UsagePage, PreparsedData);
}

extern "C" NTSTATUS WINAPI HidP_SetUsageValue(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, ULONG UsageValue, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_SetUsageValue, ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_SetScaledUsageValue(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, LONG UsageValue, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_SetScaledUsageValue, ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_SetUsageValueArray(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, PCHAR UsageValue, USHORT UsageValueByteLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_SetUsageValueArray, ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_GetUsageValue(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, PULONG UsageValue, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_GetUsageValue, ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_GetScaledUsageValue(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, PLONG UsageValue, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_GetScaledUsageValue, ReportType, UsagePage, LinkCollection, Usage, UsageValue, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_GetUsageValueArray(HIDP_REPORT_TYPE ReportType, USAGE UsagePage, USHORT LinkCollection, USAGE Usage, PCHAR UsageValue, USHORT UsageValueByteLength, PHIDP_PREPARSED_DATA PreparsedData, PCHAR Report, ULONG ReportLength) {
    D(HidP_GetUsageValueArray, ReportType, UsagePage, LinkCollection, Usage, UsageValue, UsageValueByteLength, PreparsedData, Report, ReportLength);
}

extern "C" NTSTATUS WINAPI HidP_UsageListDifference(PUSAGE PreviousUsageList, PUSAGE CurrentUsageList, PUSAGE BreakUsageList, PUSAGE MakeUsageList, ULONG UsageListLength) {
    D(HidP_UsageListDifference, PreviousUsageList, CurrentUsageList, BreakUsageList, MakeUsageList, UsageListLength);
}
#else // EXPORT_HID_DLL_FUNCTIONS
namespace hid_dll {
    bool checkFname(const wchar_t*) { return false; }
    void loadGenuineDll(const wchar_t*) {}
    void unloadGenuineDll() {}
}
#endif // EXPORT_HID_DLL_FUNCTIONS
