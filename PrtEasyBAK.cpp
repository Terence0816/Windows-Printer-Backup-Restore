#ifndef UNICODE
#define UNICODE
#endif
#ifndef _UNICODE
#define _UNICODE
#endif

#include <windows.h>
#include <commctrl.h>
#include <richedit.h>
#include <shellapi.h>
#include <wincrypt.h>
#include <winspool.h>
#include <winsvc.h>

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cwctype>
#include <exception>
#include <filesystem>
#include <fstream>
#include <map>
#include <set>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#ifdef _MSC_VER
#pragma comment(lib, "Advapi32.lib")
#pragma comment(lib, "Comctl32.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Gdi32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "User32.lib")
#pragma comment(lib, "Winspool.lib")
#endif

namespace fs = std::filesystem;

enum class AppLanguage {
    English,
    TraditionalChinese
};

struct ProgressWindowState {
    HWND windowHandle = nullptr;
    HWND labelHandle = nullptr;
    HWND detailHandle = nullptr;
    HWND progressHandle = nullptr;
    int maximum = 0;
};

struct SelectionDialogState {
    std::wstring title;
    std::wstring prompt;
    std::vector<std::wstring> options;
    HWND listHandle = nullptr;
    int selectedIndex = -1;
    bool accepted = false;
};

enum class AppAction {
    Cancel = 0,
    Backup = 1,
    Restore = 2
};

struct ActionDialogState {
    std::wstring title;
    std::wstring prompt;
    AppAction action = AppAction::Cancel;
    bool accepted = false;
};

struct AboutDialogState {
    std::wstring title;
    std::wstring content;
    HWND textHandle = nullptr;
    bool accepted = false;
};

struct CheckListItem {
    std::wstring name;
    std::wstring detail;
    std::wstring port;
    bool checked = true;
};

struct CheckListDialogState {
    std::wstring title;
    std::wstring prompt;
    std::wstring description;
    std::vector<CheckListItem> items;
    HWND listHandle = nullptr;
    HWND countHandle = nullptr;
    bool accepted = false;
    std::vector<int> selectedIndices;
};

struct DriverBackupInfo {
    std::wstring name;
    std::wstring relativeInfPath;
    std::wstring relativeFolder;
};

struct PortInfo {
    std::wstring name;
    std::wstring kind;
    std::wstring printerHostAddress;
    std::wstring queueName;
    std::uint32_t portNumber = 0;
    bool snmpEnabled = false;
    std::uint32_t snmpIndex = 0;
    std::wstring snmpCommunity;
    std::wstring protocol;
    bool lprByteCounting = false;
    std::wstring portMonitor;
};

struct NativePrinterRecord {
    std::wstring name;
    std::wstring serverName;
    std::wstring shareName;
    std::wstring driverName;
    std::wstring portName;
    std::wstring comment;
    std::wstring location;
    std::wstring datatype;
    std::wstring printProcessor;
    DWORD attributes = 0;
};

struct PrinterEntry {
    std::wstring name;
    std::wstring driverName;
    std::wstring portName;
    bool shared = false;
    std::wstring shareName;
    std::wstring comment;
    std::wstring location;
    std::wstring datatype;
    std::wstring printProcessor;
    bool published = false;
    bool keepPrintedJobs = false;
    bool isDefault = false;
    bool isConnectionPrinter = false;
    std::wstring connectionName;
    std::wstring usbPnpKey;
    DriverBackupInfo driver;
    PortInfo port;
};

struct DriverRestoreMaterial {
    std::wstring requestedName;
    fs::path stagedFolder;
    std::vector<fs::path> infFiles;
    std::wstring resolvedName;
    bool prepared = false;
};

constexpr int kProgressWidth = 480;
constexpr int kProgressHeight = 194;
constexpr int kActionWidth = 600;
constexpr int kActionHeight = 280;
constexpr int kSelectionWidth = 560;
constexpr int kSelectionHeight = 458;
constexpr int kCheckListWidth = 980;
constexpr int kCheckListHeight = 682;
constexpr int kAboutWidth = 720;
constexpr int kAboutHeight = 560;
constexpr int kDialogButtonWidth = 112;
constexpr int kDialogButtonHeight = 34;
constexpr int kAppIconResourceId = 1;
constexpr int kActionPromptId = 1251;
constexpr int kActionAboutId = 1252;
constexpr int kActionLangComboId = 1253;
constexpr int kProgressLabelId = 1201;
constexpr int kProgressDetailId = 1203;
constexpr int kProgressBarId = 1202;
constexpr int kListPromptId = 1301;
constexpr int kListBoxId = 1302;
constexpr int kCheckPromptId = 1401;
constexpr int kCheckDescriptionId = 1402;
constexpr int kCheckCountId = 1403;
constexpr int kCheckListViewId = 1404;
constexpr int kCheckSelectAllId = 1405;
constexpr int kCheckClearId = 1406;
constexpr int kAboutTextId = 1501;

void ShowAboutDialog();

fs::path g_baseDir;
fs::path g_backupRoot;
fs::path g_driversRoot;
fs::path g_printersRoot;
fs::path g_configPath;
fs::path g_logPath;
fs::path g_tempRoot;
ProgressWindowState g_progressWindow;
AppLanguage g_appLanguage = AppLanguage::English;
std::wstring g_languageSetting = L"auto";
std::wstring g_uiFontFace = L"Segoe UI";
HFONT g_dialogFont = nullptr;
HFONT g_dialogBoldFont = nullptr;
HBRUSH g_dialogBackgroundBrush = nullptr;
HBRUSH g_panelBackgroundBrush = nullptr;
HBRUSH g_listBackgroundBrush = nullptr;

constexpr COLORREF kColorDialogBackground = RGB(241, 245, 250);
constexpr COLORREF kColorPanelBackground = RGB(230, 237, 246);
constexpr COLORREF kColorListBackground = RGB(252, 253, 255);
constexpr COLORREF kColorTextPrimary = RGB(25, 43, 63);
constexpr COLORREF kColorTextSecondary = RGB(84, 103, 124);
constexpr COLORREF kColorAccent = RGB(29, 116, 217);

std::wstring Trim(const std::wstring& value) {
    const wchar_t* whitespace = L" \t\r\n";
    const auto start = value.find_first_not_of(whitespace);
    if (start == std::wstring::npos) {
        return L"";
    }
    const auto end = value.find_last_not_of(whitespace);
    return value.substr(start, end - start + 1);
}

std::wstring ToLowerCopy(std::wstring value) {
    std::transform(value.begin(), value.end(), value.begin(), [](wchar_t ch) {
        return static_cast<wchar_t>(::towlower(ch));
    });
    return value;
}

bool IEquals(const std::wstring& left, const std::wstring& right) {
    return ToLowerCopy(left) == ToLowerCopy(right);
}

bool StartsWithI(const std::wstring& value, const std::wstring& prefix) {
    if (value.size() < prefix.size()) {
        return false;
    }
    return IEquals(value.substr(0, prefix.size()), prefix);
}

bool ContainsI(const std::wstring& value, const std::wstring& needle) {
    return ToLowerCopy(value).find(ToLowerCopy(needle)) != std::wstring::npos;
}

std::wstring WideFromBytes(const std::string& input, UINT codePage = CP_ACP) {
    if (input.empty()) {
        return L"";
    }
    const int size = MultiByteToWideChar(codePage, 0, input.data(), static_cast<int>(input.size()), nullptr, 0);
    if (size <= 0) {
        return L"";
    }
    std::wstring output(static_cast<std::size_t>(size), L'\0');
    MultiByteToWideChar(codePage, 0, input.data(), static_cast<int>(input.size()), output.data(), size);
    return output;
}

std::string BytesFromWide(const std::wstring& input, UINT codePage = CP_ACP) {
    if (input.empty()) {
        return "";
    }
    const int size = WideCharToMultiByte(codePage, 0, input.data(), static_cast<int>(input.size()), nullptr, 0, nullptr, nullptr);
    if (size <= 0) {
        return "";
    }
    std::string output(static_cast<std::size_t>(size), '\0');
    WideCharToMultiByte(codePage, 0, input.data(), static_cast<int>(input.size()), output.data(), size, nullptr, nullptr);
    return output;
}

std::wstring ReadTextFile(const fs::path& path) {
    std::ifstream input(path, std::ios::binary);
    if (!input) {
        return L"";
    }

    std::string bytes((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    if (bytes.size() >= 3 &&
        static_cast<unsigned char>(bytes[0]) == 0xEF &&
        static_cast<unsigned char>(bytes[1]) == 0xBB &&
        static_cast<unsigned char>(bytes[2]) == 0xBF) {
        return WideFromBytes(bytes.substr(3), CP_UTF8);
    }

    if (bytes.size() >= 2) {
        const unsigned char b0 = static_cast<unsigned char>(bytes[0]);
        const unsigned char b1 = static_cast<unsigned char>(bytes[1]);
        if (b0 == 0xFF && b1 == 0xFE) {
            const wchar_t* data = reinterpret_cast<const wchar_t*>(bytes.data() + 2);
            return std::wstring(data, (bytes.size() - 2) / sizeof(wchar_t));
        }
    }

    return WideFromBytes(bytes, CP_UTF8);
}

bool WriteUtf8File(const fs::path& path, const std::wstring& text) {
    std::error_code ec;
    fs::create_directories(path.parent_path(), ec);

    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output) {
        return false;
    }

    const std::string bytes = BytesFromWide(text, CP_UTF8);
    output.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
    return static_cast<bool>(output);
}

void AppendUtf8Line(const fs::path& path, const std::wstring& line) {
    std::error_code ec;
    fs::create_directories(path.parent_path(), ec);

    std::ofstream output(path, std::ios::binary | std::ios::app);
    if (!output) {
        return;
    }

    const std::string bytes = BytesFromWide(line + L"\r\n", CP_UTF8);
    output.write(bytes.data(), static_cast<std::streamsize>(bytes.size()));
}

std::vector<std::wstring> SplitLines(const std::wstring& text) {
    std::vector<std::wstring> lines;
    std::wstring current;
    for (wchar_t ch : text) {
        if (ch == L'\r') {
            continue;
        }
        if (ch == L'\n') {
            lines.push_back(current);
            current.clear();
        } else {
            current.push_back(ch);
        }
    }
    if (!current.empty()) {
        lines.push_back(current);
    }
    return lines;
}

std::wstring QuoteArg(const std::wstring& argument) {
    if (argument.empty()) {
        return L"\"\"";
    }

    bool needsQuotes = false;
    for (wchar_t ch : argument) {
        if (iswspace(ch) || ch == L'"') {
            needsQuotes = true;
            break;
        }
    }

    if (!needsQuotes) {
        return argument;
    }

    std::wstring result = L"\"";
    unsigned int backslashes = 0;
    for (wchar_t ch : argument) {
        if (ch == L'\\') {
            ++backslashes;
            continue;
        }
        if (ch == L'"') {
            result.append(backslashes * 2 + 1, L'\\');
            result.push_back(L'"');
            backslashes = 0;
            continue;
        }
        result.append(backslashes, L'\\');
        backslashes = 0;
        result.push_back(ch);
    }
    result.append(backslashes * 2, L'\\');
    result.push_back(L'"');
    return result;
}

std::wstring BuildCommandLine(const std::wstring& executable, const std::vector<std::wstring>& args) {
    std::wstring commandLine = QuoteArg(executable);
    for (const auto& arg : args) {
        commandLine.push_back(L' ');
        commandLine += QuoteArg(arg);
    }
    return commandLine;
}

std::wstring GetTimestamp() {
    SYSTEMTIME st{};
    GetLocalTime(&st);
    wchar_t buffer[64] = {};
    swprintf(buffer, 64, L"%04u-%02u-%02u %02u:%02u:%02u",
             st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
    return buffer;
}

std::wstring FormatWin32Error(DWORD errorCode) {
    LPWSTR buffer = nullptr;
    const DWORD flags = FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS;
    const DWORD length = FormatMessageW(flags, nullptr, errorCode, 0, reinterpret_cast<LPWSTR>(&buffer), 0, nullptr);
    std::wstring message = length && buffer ? Trim(buffer) : L"Unknown error";
    if (buffer) {
        LocalFree(buffer);
    }

    wchar_t prefix[64] = {};
    swprintf(prefix, 64, L"0x%08lX (%lu): ", errorCode, errorCode);
    return std::wstring(prefix) + message;
}

void Log(const std::wstring& message) {
    (void)message;
}

std::wstring JoinLines(const std::vector<std::wstring>& lines) {
    std::wstring result;
    for (std::size_t i = 0; i < lines.size(); ++i) {
        if (i > 0) {
            result += L"\r\n";
        }
        result += lines[i];
    }
    return result;
}

int ShowMessage(const std::wstring& title, const std::wstring& message, UINT flags) {
    return MessageBoxW(nullptr, message.c_str(), title.c_str(), flags | MB_TOPMOST);
}

void ShowErrorWithLog(const std::wstring& title, const std::wstring& message) {
    ShowMessage(title, message, MB_OK | MB_ICONERROR);
}

UINT GetUiDpi() {
    static UINT dpi = 0;
    if (!dpi) {
        HDC screen = GetDC(nullptr);
        dpi = screen ? static_cast<UINT>(GetDeviceCaps(screen, LOGPIXELSY)) : 96;
        if (screen) {
            ReleaseDC(nullptr, screen);
        }
        if (!dpi) {
            dpi = 96;
        }
    }
    return dpi;
}

int ScaleUi(int value) {
    return MulDiv(value, static_cast<int>(GetUiDpi()), 96);
}

HFONT GetDialogFont(bool bold = false) {
    if (!g_dialogFont) {
        const int dpi = static_cast<int>(GetUiDpi());

        g_dialogFont = CreateFontW(
            -MulDiv(11, dpi, 72),
            0,
            0,
            0,
            FW_NORMAL,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            g_uiFontFace.c_str());

        g_dialogBoldFont = CreateFontW(
            -MulDiv(12, dpi, 72),
            0,
            0,
            0,
            FW_SEMIBOLD,
            FALSE,
            FALSE,
            FALSE,
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            CLEARTYPE_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            g_uiFontFace.c_str());
    }

    return bold && g_dialogBoldFont ? g_dialogBoldFont : g_dialogFont;
}

void SetControlFont(HWND handle, bool bold = false) {
    if (handle) {
        SendMessageW(handle, WM_SETFONT, reinterpret_cast<WPARAM>(GetDialogFont(bold)), TRUE);
    }
}

const wchar_t* UiText(const wchar_t* english, const wchar_t* traditionalChinese) {
    return g_appLanguage == AppLanguage::TraditionalChinese ? traditionalChinese : english;
}

bool EnsureRichEditLoaded() {
    static HMODULE richEditModule = LoadLibraryW(L"Msftedit.dll");
    return richEditModule != nullptr;
}

std::wstring GetControlTextRange(HWND handle, LONG start, LONG end) {
    if (!handle || start < 0 || end <= start) {
        return L"";
    }

    std::vector<wchar_t> buffer(static_cast<std::size_t>(end - start + 1), L'\0');
    TEXTRANGEW textRange{};
    textRange.chrg.cpMin = start;
    textRange.chrg.cpMax = end;
    textRange.lpstrText = buffer.data();
    const LRESULT copied = SendMessageW(handle, EM_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&textRange));
    if (copied <= 0) {
        return L"";
    }

    return std::wstring(buffer.data(), static_cast<std::size_t>(copied));
}

bool OpenExternalUrl(const std::wstring& url) {
    if (url.empty()) {
        return false;
    }

    const HINSTANCE instance = ShellExecuteW(nullptr, L"open", url.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    return reinterpret_cast<INT_PTR>(instance) > 32;
}

std::wstring BuildAboutText() {
    if (g_appLanguage == AppLanguage::TraditionalChinese) {
        return JoinLines({
            L"Windows Printer Backup Restore",
            L"Windows 印表機備份與還原工具",
            L"",
            L"版本：v1.2.0.0",
            L"作者：Terence0816",
            L"授權：MIT License",
            L"GitHub：",
            L"https://github.com/Terence0816/Windows-Printer-Backup-Restore",
            L"",
            L"Windows Printer Backup Restore 是一套輕量化的 Windows 印表機備份與還原工具，適合 IT 管理員、系統維護人員、MSP 維護商，以及需要將印表機設定從一台電腦移轉到另一台電腦的使用者。",
            L"",
            L"本工具可協助備份本機已安裝的實體印表機，並盡可能在另一台 Windows 電腦上自動還原印表機、驅動程式、連接埠、列印偏好設定與預設印表機資訊。",
            L"",
            L"主要功能：",
            L"* 備份本機實體印表機",
            L"* 還原印表機到另一台 Windows 電腦",
            L"* 備份與還原印表機驅動程式",
            L"* 支援 TCP/IP、USB、Local Port、LPR Port 連接埠資訊",
            L"* 支援印表機偏好設定匯出與匯入",
            L"* 可記錄並還原原本的預設印表機",
            L"* 還原前可勾選要還原的印表機",
            L"* 還原清單可顯示印表機名稱、型號 / 驅動、連接埠",
            L"* 共享印表機支援原始連接、Local Port、LPR Port 三種還原模式",
            L"* 自動略過常見虛擬印表機",
            L"* 支援繁體中文與英文介面",
            L"* 可從網路分享路徑執行",
            L"* 提供可攜式 EXE 執行檔",
            L"",
            L"執行需求：",
            L"請使用系統管理員身分執行本程式。",
            L"",
            L"備份資料夾：",
            L"PrinterBackup",
            L"",
            L"注意事項：",
            L"* 部分印表機設定可能會受到驅動程式版本與 Windows 版本影響。",
            L"* USB 印表機還原後，可能仍需要重新插拔或手動確認 USB 連接埠。",
            L"* 網路共用印表機可能需要原始列印伺服器仍可連線。",
            L"* 建議正式大量還原前，先於目標環境進行測試。",
            L"",
            L"問題回報 / 建議：",
            L"請至 GitHub Issues 回報問題或提出建議。",
            L"",
            L"免責聲明：",
            L"本工具依現況提供，作者不保證所有印表機、驅動程式與 Windows 環境皆能完整還原。"
        });
    }

    return JoinLines({
        L"Windows Printer Backup Restore",
        L"Windows Printer Backup and Restore Utility",
        L"",
        L"Version: v1.2.0.0",
        L"Author: Terence0816",
        L"License: MIT License",
        L"GitHub:",
        L"https://github.com/Terence0816/Windows-Printer-Backup-Restore",
        L"",
        L"Windows Printer Backup Restore is a lightweight utility for backing up and restoring Windows printers. It is designed for IT administrators, support engineers, MSP service providers, and users who need to migrate printer settings from one Windows computer to another.",
        L"",
        L"This tool helps back up local physical printers and restore printers, drivers, ports, printer preferences, and the original default printer on another Windows PC as automatically as possible.",
        L"",
        L"Main Features:",
        L"* Backup local physical printers",
        L"* Restore printers on another Windows PC",
        L"* Backup and restore printer drivers",
        L"* Support TCP/IP, USB, Local Port, and LPR Port information",
        L"* Export and import printer preferences",
        L"* Record and restore the original default printer when possible",
        L"* Select which printers to restore before starting",
        L"* Display printer name, model / driver, and port information",
        L"* Support three restore modes for shared printers: Original Connection, Local Port, and LPR Port",
        L"* Skip common virtual printers automatically",
        L"* Traditional Chinese and English interface support",
        L"* Network-path friendly execution",
        L"* Portable EXE design",
        L"",
        L"Required Permission:",
        L"Please run this program as Administrator.",
        L"",
        L"Backup Folder:",
        L"PrinterBackup",
        L"",
        L"Notes:",
        L"* Some printer preferences may depend on the printer driver version and Windows version.",
        L"* USB printers may still need to be reconnected or manually confirmed after restore.",
        L"* Network shared printers may require the original print server to remain available.",
        L"* Please test in your own environment before large-scale deployment.",
        L"",
        L"Support / Feedback:",
        L"Please report issues or suggestions through GitHub Issues.",
        L"",
        L"Disclaimer:",
        L"This software is provided as-is. The author does not guarantee full compatibility with every printer, driver, or Windows environment."
    });
}

void ResetDialogFonts() {
    if (g_dialogFont) {
        DeleteObject(g_dialogFont);
        g_dialogFont = nullptr;
    }
    if (g_dialogBoldFont) {
        DeleteObject(g_dialogBoldFont);
        g_dialogBoldFont = nullptr;
    }
}

bool CanFontRenderText(const wchar_t* faceName, const wchar_t* sample) {
    HDC dc = GetDC(nullptr);
    if (!dc) {
        return false;
    }

    HFONT font = CreateFontW(
        -MulDiv(10, GetDeviceCaps(dc, LOGPIXELSY), 72),
        0,
        0,
        0,
        FW_NORMAL,
        FALSE,
        FALSE,
        FALSE,
        DEFAULT_CHARSET,
        OUT_DEFAULT_PRECIS,
        CLIP_DEFAULT_PRECIS,
        CLEARTYPE_QUALITY,
        DEFAULT_PITCH | FF_DONTCARE,
        faceName);
    if (!font) {
        ReleaseDC(nullptr, dc);
        return false;
    }

    WORD glyphs[32] = {};
    HGDIOBJ oldFont = SelectObject(dc, font);
    const int length = static_cast<int>(wcslen(sample));
    const DWORD result = GetGlyphIndicesW(dc, sample, length, glyphs, GGI_MARK_NONEXISTING_GLYPHS);
    if (oldFont) {
        SelectObject(dc, oldFont);
    }

    DeleteObject(font);
    ReleaseDC(nullptr, dc);

    if (result == GDI_ERROR) {
        return false;
    }

    for (int i = 0; i < length; ++i) {
        if (glyphs[i] == 0xFFFF) {
            return false;
        }
    }
    return true;
}

bool CanRenderTraditionalChinese() {
    const wchar_t* sample = L"\u5099\u4efd\u9084\u539f\u5370\u8868\u6a5f";
    const wchar_t* candidates[] = {
        L"Microsoft JhengHei UI",
        L"Microsoft JhengHei",
        L"PMingLiU",
        L"MingLiU",
        L"Segoe UI"
    };

    for (const auto* candidate : candidates) {
        if (CanFontRenderText(candidate, sample)) {
            g_uiFontFace = candidate;
            return true;
        }
    }

    return false;
}

AppLanguage DetectSystemLanguage() {
    wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = {};
    if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH) <= 0) {
        g_uiFontFace = L"Segoe UI";
        return AppLanguage::English;
    }

    const std::wstring locale = ToLowerCopy(localeName);
    if (locale.rfind(L"zh", 0) == 0 && CanRenderTraditionalChinese()) {
        return AppLanguage::TraditionalChinese;
    }

    g_uiFontFace = L"Segoe UI";
    return AppLanguage::English;
}

std::wstring ReadIniLanguageSetting(const fs::path& path) {
    const std::wstring text = ReadTextFile(path);
    if (text.empty()) {
        return L"auto";
    }

    for (const auto& rawLine : SplitLines(text)) {
        const std::wstring line = Trim(rawLine);
        if (line.empty() || StartsWithI(line, L"#") || StartsWithI(line, L";")) {
            continue;
        }

        const auto pos = line.find(L'=');
        if (pos == std::wstring::npos) {
            continue;
        }

        const std::wstring key = ToLowerCopy(Trim(line.substr(0, pos)));
        const std::wstring value = Trim(line.substr(pos + 1));
        if (key == L"lang" || key == L"language" || key == L"ui_lang") {
            return value.empty() ? L"auto" : value;
        }
    }

    return L"auto";
}

void EnsureConfigIniExists() {
    std::error_code ec;
    if (fs::exists(g_configPath, ec)) {
        return;
    }

    const std::wstring content =
        L"; PrtEasyBAK settings\r\n"
        L"; ui_lang examples / 語系範例\r\n"
        L"; ui_lang=auto   ; Auto detect. Chinese systems prefer Traditional Chinese when it can be rendered.\r\n"
        L"; ui_lang=zh-TW  ; Force Traditional Chinese / 強制繁體中文\r\n"
        L"; ui_lang=en     ; Force English / 強制英文\r\n"
        L"ui_lang=auto\r\n";
    WriteUtf8File(g_configPath, content);
}

void ApplyLanguageSetting(const std::wstring& value) {
    g_languageSetting = ToLowerCopy(Trim(value));
    g_uiFontFace = L"Segoe UI";

    if (g_languageSetting.empty() || g_languageSetting == L"auto") {
        g_appLanguage = DetectSystemLanguage();
        ResetDialogFonts();
        return;
    }

    if (g_languageSetting == L"en" ||
        g_languageSetting == L"en-us" ||
        g_languageSetting == L"english") {
        g_appLanguage = AppLanguage::English;
        ResetDialogFonts();
        return;
    }

    if (g_languageSetting == L"zh" ||
        g_languageSetting == L"zh-tw" ||
        g_languageSetting == L"zh-hant" ||
        g_languageSetting == L"zh-hk" ||
        g_languageSetting == L"tw" ||
        g_languageSetting == L"cht" ||
        g_languageSetting == L"traditional") {
        g_appLanguage = CanRenderTraditionalChinese() ? AppLanguage::TraditionalChinese : AppLanguage::English;
        if (g_appLanguage == AppLanguage::English) {
            g_uiFontFace = L"Segoe UI";
        }
        ResetDialogFonts();
        return;
    }

    g_appLanguage = DetectSystemLanguage();
    ResetDialogFonts();
}

HBRUSH GetDialogBackgroundBrush() {
    if (!g_dialogBackgroundBrush) {
        g_dialogBackgroundBrush = CreateSolidBrush(kColorDialogBackground);
    }
    return g_dialogBackgroundBrush;
}

HBRUSH GetPanelBackgroundBrush() {
    if (!g_panelBackgroundBrush) {
        g_panelBackgroundBrush = CreateSolidBrush(kColorPanelBackground);
    }
    return g_panelBackgroundBrush;
}

HBRUSH GetListBackgroundBrush() {
    if (!g_listBackgroundBrush) {
        g_listBackgroundBrush = CreateSolidBrush(kColorListBackground);
    }
    return g_listBackgroundBrush;
}

HICON GetAppIconLarge() {
    static HICON icon = reinterpret_cast<HICON>(
        LoadImageW(
            GetModuleHandleW(nullptr),
            MAKEINTRESOURCEW(kAppIconResourceId),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXICON),
            GetSystemMetrics(SM_CYICON),
            LR_DEFAULTCOLOR));
    return icon ? icon : LoadIconW(nullptr, IDI_APPLICATION);
}

HICON GetAppIconSmall() {
    static HICON icon = reinterpret_cast<HICON>(
        LoadImageW(
            GetModuleHandleW(nullptr),
            MAKEINTRESOURCEW(kAppIconResourceId),
            IMAGE_ICON,
            GetSystemMetrics(SM_CXSMICON),
            GetSystemMetrics(SM_CYSMICON),
            LR_DEFAULTCOLOR));
    return icon ? icon : LoadIconW(nullptr, IDI_APPLICATION);
}

void ApplyWindowIcons(HWND handle) {
    if (!handle) {
        return;
    }

    SendMessageW(handle, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(GetAppIconLarge()));
    SendMessageW(handle, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(GetAppIconSmall()));
}

void ApplyListViewTheme(HWND handle) {
    if (!handle) {
        return;
    }

    ListView_SetBkColor(handle, kColorListBackground);
    ListView_SetTextBkColor(handle, kColorListBackground);
    ListView_SetTextColor(handle, kColorTextPrimary);
}

INT_PTR HandleDialogControlColor(UINT message, WPARAM wParam, LPARAM lParam) {
    HDC dc = reinterpret_cast<HDC>(wParam);
    HWND control = reinterpret_cast<HWND>(lParam);

    if (message == WM_CTLCOLORDLG) {
        return reinterpret_cast<INT_PTR>(GetDialogBackgroundBrush());
    }

    if (message == WM_CTLCOLORSTATIC) {
        SetBkMode(dc, TRANSPARENT);
        COLORREF textColor = kColorTextPrimary;
        const int controlId = GetDlgCtrlID(control);
        if (controlId == kCheckDescriptionId || controlId == kProgressDetailId) {
            textColor = kColorTextSecondary;
        } else if (controlId == kCheckCountId) {
            textColor = kColorAccent;
        }
        SetTextColor(dc, textColor);
        return reinterpret_cast<INT_PTR>(GetDialogBackgroundBrush());
    }

    if (message == WM_CTLCOLORLISTBOX) {
        SetBkColor(dc, kColorListBackground);
        SetTextColor(dc, kColorTextPrimary);
        return reinterpret_cast<INT_PTR>(GetListBackgroundBrush());
    }

    if (message == WM_CTLCOLOREDIT) {
        SetBkColor(dc, kColorListBackground);
        SetTextColor(dc, kColorTextPrimary);
        return reinterpret_cast<INT_PTR>(GetListBackgroundBrush());
    }

    return 0;
}

void InitUiCommonControls() {
    static bool initialized = false;
    if (initialized) {
        return;
    }

    INITCOMMONCONTROLSEX icc{};
    icc.dwSize = sizeof(icc);
    icc.dwICC = ICC_PROGRESS_CLASS | ICC_LISTVIEW_CLASSES;
    InitCommonControlsEx(&icc);
    initialized = true;
}

HMENU MenuId(int value) {
    return reinterpret_cast<HMENU>(static_cast<INT_PTR>(value));
}

void PumpPendingMessages() {
    MSG msg{};
    while (PeekMessageW(&msg, nullptr, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);
    }
}

bool WaitForHandleWithMessages(HANDLE handle, DWORD timeoutMs, DWORD& waitResult) {
    const DWORD startTick = GetTickCount();
    for (;;) {
        const DWORD elapsed = GetTickCount() - startTick;
        if (timeoutMs != INFINITE && elapsed >= timeoutMs) {
            waitResult = WAIT_TIMEOUT;
            return false;
        }

        DWORD remaining = INFINITE;
        if (timeoutMs != INFINITE) {
            remaining = timeoutMs - elapsed;
            if (remaining > 50) {
                remaining = 50;
            }
        }

        const DWORD result = MsgWaitForMultipleObjects(1, &handle, FALSE, remaining, QS_ALLINPUT);
        if (result == WAIT_OBJECT_0) {
            waitResult = WAIT_OBJECT_0;
            return true;
        }
        if (result == WAIT_TIMEOUT && timeoutMs != INFINITE) {
            continue;
        }
        if (result == WAIT_OBJECT_0 + 1) {
            PumpPendingMessages();
            continue;
        }

        waitResult = result;
        return false;
    }
}

void SleepWithMessages(DWORD milliseconds) {
    const DWORD startTick = GetTickCount();
    while ((GetTickCount() - startTick) < milliseconds) {
        PumpPendingMessages();
        Sleep(20);
    }
}

void CenterWindowOnOwner(HWND handle, HWND owner, int width, int height) {
    RECT target{};
    if (owner && GetWindowRect(owner, &target)) {
        const int x = target.left + ((target.right - target.left) - width) / 2;
        const int y = target.top + ((target.bottom - target.top) - height) / 2;
        SetWindowPos(handle, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
        return;
    }

    RECT workArea{};
    SystemParametersInfoW(SPI_GETWORKAREA, 0, &workArea, 0);
    const int x = workArea.left + ((workArea.right - workArea.left) - width) / 2;
    const int y = workArea.top + ((workArea.bottom - workArea.top) - height) / 2;
    SetWindowPos(handle, nullptr, x, y, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
}

SIZE GetWindowSizeForClientArea(int clientWidth, int clientHeight, DWORD style, DWORD exStyle) {
    RECT rect{0, 0, clientWidth, clientHeight};
    AdjustWindowRectEx(&rect, style, FALSE, exStyle);

    SIZE size{};
    size.cx = rect.right - rect.left;
    size.cy = rect.bottom - rect.top;
    return size;
}

SIZE GetClientSize(HWND handle) {
    RECT rect{};
    GetClientRect(handle, &rect);

    SIZE size{};
    size.cx = std::max(0L, rect.right - rect.left);
    size.cy = std::max(0L, rect.bottom - rect.top);
    return size;
}

void ConfigureProgressWindowBar() {
    if (!g_progressWindow.progressHandle) {
        return;
    }

    SendMessageW(g_progressWindow.progressHandle, PBM_SETRANGE32, 0, std::max(1, g_progressWindow.maximum));
    InvalidateRect(g_progressWindow.progressHandle, nullptr, TRUE);
}

LRESULT CALLBACK ProgressWindowProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        ApplyWindowIcons(handle);
        const SIZE client = GetClientSize(handle);
        const int left = ScaleUi(18);
        const int top = ScaleUi(18);
        const int rightPadding = ScaleUi(18);
        const int labelHeight = ScaleUi(30);
        const int detailTop = ScaleUi(56);
        const int detailHeight = ScaleUi(52);
        const int progressTop = ScaleUi(120);
        const int progressHeight = ScaleUi(24);
        const int contentWidth = std::max(1, static_cast<int>(client.cx) - left - rightPadding);

        HWND label = CreateWindowExW(
            0, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE,
            left, top, contentWidth, labelHeight,
            handle, MenuId(kProgressLabelId), nullptr, nullptr);
        SetControlFont(label, true);
        g_progressWindow.labelHandle = label;

        HWND detail = CreateWindowExW(
            0, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE,
            left, detailTop, contentWidth, detailHeight,
            handle, MenuId(kProgressDetailId), nullptr, nullptr);
        SetControlFont(detail);
        g_progressWindow.detailHandle = detail;

        HWND progress = CreateWindowExW(
            0, PROGRESS_CLASSW, L"",
            WS_CHILD | WS_VISIBLE | PBS_SMOOTH,
            left, progressTop, contentWidth, progressHeight,
            handle, MenuId(kProgressBarId), nullptr, nullptr);
        g_progressWindow.progressHandle = progress;
        ConfigureProgressWindowBar();
        SendMessageW(progress, PBM_SETPOS, 0, 0);
        return 0;
    }
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
        return static_cast<LRESULT>(HandleDialogControlColor(message, wParam, lParam));
    case WM_CLOSE:
        return 0;
    default:
        break;
    }
    return DefWindowProcW(handle, message, wParam, lParam);
}

LRESULT CALLBACK AboutDialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    AboutDialogState* state = reinterpret_cast<AboutDialogState*>(GetWindowLongPtrW(handle, GWLP_USERDATA));

    switch (message) {
    case WM_NCCREATE: {
        auto* create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
        return TRUE;
    }
    case WM_CREATE: {
        state = reinterpret_cast<AboutDialogState*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
        if (!state) {
            return -1;
        }

        ApplyWindowIcons(handle);
        const bool richEditLoaded = EnsureRichEditLoaded();
        const SIZE client = GetClientSize(handle);
        const int sidePadding = ScaleUi(18);
        const int topPadding = ScaleUi(18);
        const int bottomPadding = ScaleUi(18);
        const int buttonWidth = ScaleUi(kDialogButtonWidth);
        const int buttonHeight = ScaleUi(kDialogButtonHeight);
        const int buttonY = static_cast<int>(client.cy) - bottomPadding - buttonHeight;
        const int textHeight = std::max(ScaleUi(220), buttonY - topPadding - ScaleUi(16));
        const int contentWidth = std::max(1, static_cast<int>(client.cx) - sidePadding * 2);

        state->textHandle = CreateWindowExW(
            WS_EX_CLIENTEDGE,
            richEditLoaded ? MSFTEDIT_CLASS : L"EDIT",
            L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL |
                ES_LEFT | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
            sidePadding, topPadding, contentWidth, textHeight,
            handle, MenuId(kAboutTextId), nullptr, nullptr);
        SetControlFont(state->textHandle);
        if (state->textHandle) {
            if (richEditLoaded) {
                SendMessageW(state->textHandle, EM_AUTOURLDETECT, TRUE, 0);
                const DWORD eventMask = static_cast<DWORD>(SendMessageW(state->textHandle, EM_GETEVENTMASK, 0, 0));
                SendMessageW(state->textHandle, EM_SETEVENTMASK, 0, static_cast<LPARAM>(eventMask | ENM_LINK));
                SendMessageW(state->textHandle, EM_SETBKGNDCOLOR, 0, static_cast<LPARAM>(kColorListBackground));
            }
            SetWindowTextW(state->textHandle, state->content.c_str());
            SendMessageW(state->textHandle, EM_SETSEL, 0, 0);
        }

        HWND closeButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"Close", L"關閉"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
            static_cast<int>(client.cx) - sidePadding - buttonWidth, buttonY, buttonWidth, buttonHeight,
            handle, MenuId(IDOK), nullptr, nullptr);
        SetControlFont(closeButton);

        SetFocus(closeButton);
        return 0;
    }
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLOREDIT:
        return static_cast<LRESULT>(HandleDialogControlColor(message, wParam, lParam));
    case WM_NOTIFY:
        if (!state || !lParam) {
            break;
        }
        if (reinterpret_cast<LPNMHDR>(lParam)->idFrom == kAboutTextId &&
            reinterpret_cast<LPNMHDR>(lParam)->code == EN_LINK) {
            const auto* link = reinterpret_cast<const ENLINK*>(lParam);
            if (link->msg == WM_LBUTTONUP ||
                (link->msg == WM_KEYUP && (link->wParam == VK_RETURN || link->wParam == VK_SPACE))) {
                OpenExternalUrl(GetControlTextRange(state->textHandle, link->chrg.cpMin, link->chrg.cpMax));
                return 0;
            }
        }
        break;
    case WM_COMMAND:
        if (!state) {
            break;
        }

        switch (LOWORD(wParam)) {
        case IDOK:
        case IDCANCEL:
            state->accepted = true;
            DestroyWindow(handle);
            return 0;
        default:
            break;
        }
        break;
    case WM_CLOSE:
        if (state) {
            state->accepted = true;
        }
        DestroyWindow(handle);
        return 0;
    default:
        break;
    }

    return DefWindowProcW(handle, message, wParam, lParam);
}

void ShowProgressWindow(const std::wstring& title,
                        const std::wstring& message,
                        int maximum = 0,
                        const std::wstring& detailMessage = L"") {
    g_progressWindow.maximum = maximum > 0 ? maximum : 1;
    const DWORD exStyle = WS_EX_DLGMODALFRAME;
    const DWORD style = WS_POPUP | WS_CAPTION | WS_CLIPCHILDREN;
    const int clientWidth = ScaleUi(kProgressWidth);
    const int clientHeight = ScaleUi(kProgressHeight);
    const SIZE windowSize = GetWindowSizeForClientArea(clientWidth, clientHeight, style, exStyle);

    if (!g_progressWindow.windowHandle) {
        InitUiCommonControls();

        static HINSTANCE instance = GetModuleHandleW(nullptr);
        WNDCLASSEXW windowClass{};
        windowClass.cbSize = sizeof(windowClass);
        windowClass.lpfnWndProc = ProgressWindowProc;
        windowClass.hInstance = instance;
        windowClass.hIcon = GetAppIconLarge();
        windowClass.hIconSm = GetAppIconSmall();
        windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
        windowClass.hbrBackground = GetDialogBackgroundBrush();
        windowClass.lpszClassName = L"PrtEasyBAKProgressWindow";
        RegisterClassExW(&windowClass);

        HWND owner = GetActiveWindow();
        g_progressWindow.windowHandle = CreateWindowExW(
            exStyle,
            L"PrtEasyBAKProgressWindow",
            title.c_str(),
            style,
            CW_USEDEFAULT, CW_USEDEFAULT, windowSize.cx, windowSize.cy,
            owner, nullptr, instance, nullptr);

        if (!g_progressWindow.windowHandle) {
            g_progressWindow = {};
            return;
        }

        ApplyWindowIcons(g_progressWindow.windowHandle);
        CenterWindowOnOwner(g_progressWindow.windowHandle, owner, windowSize.cx, windowSize.cy);
        ShowWindow(g_progressWindow.windowHandle, SW_SHOW);
        UpdateWindow(g_progressWindow.windowHandle);
    }

    SetWindowTextW(g_progressWindow.windowHandle, title.c_str());
    if (g_progressWindow.labelHandle) {
        SetWindowTextW(g_progressWindow.labelHandle, message.c_str());
    }
    if (g_progressWindow.detailHandle) {
        SetWindowTextW(g_progressWindow.detailHandle, detailMessage.c_str());
    }
    ConfigureProgressWindowBar();
    if (g_progressWindow.progressHandle) {
        SendMessageW(g_progressWindow.progressHandle, PBM_SETPOS, 0, 0);
    }
    PumpPendingMessages();
}

void UpdateProgressWindow(int value,
                          const std::wstring& message,
                          const std::wstring& detailMessage = L"") {
    if (g_progressWindow.labelHandle) {
        SetWindowTextW(g_progressWindow.labelHandle, message.c_str());
    }
    if (g_progressWindow.detailHandle) {
        SetWindowTextW(g_progressWindow.detailHandle, detailMessage.c_str());
    }
    if (g_progressWindow.progressHandle) {
        const int clampedValue = std::max(0, std::min(value, g_progressWindow.maximum));
        SendMessageW(g_progressWindow.progressHandle, PBM_SETPOS, clampedValue, 0);
    }
    PumpPendingMessages();
}

void CloseProgressWindow() {
    if (g_progressWindow.windowHandle) {
        DestroyWindow(g_progressWindow.windowHandle);
    }
    g_progressWindow = {};
    PumpPendingMessages();
}

std::wstring GetWindowTextString(HWND handle) {
    const int length = GetWindowTextLengthW(handle);
    std::wstring value(static_cast<std::size_t>(length) + 1, L'\0');
    GetWindowTextW(handle, value.data(), length + 1);
    value.resize(static_cast<std::size_t>(length));
    return value;
}

LRESULT CALLBACK SelectionDialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    SelectionDialogState* state = reinterpret_cast<SelectionDialogState*>(GetWindowLongPtrW(handle, GWLP_USERDATA));

    switch (message) {
    case WM_NCCREATE: {
        auto* create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
        return TRUE;
    }
    case WM_CREATE: {
        state = reinterpret_cast<SelectionDialogState*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
        if (!state) {
            return -1;
        }

        ApplyWindowIcons(handle);
        const SIZE client = GetClientSize(handle);
        const int sidePadding = ScaleUi(18);
        const int topPadding = ScaleUi(18);
        const int promptHeight = ScaleUi(48);
        const int listTop = ScaleUi(74);
        const int bottomPadding = ScaleUi(18);
        const int buttonGap = ScaleUi(16);
        const int buttonWidth = ScaleUi(kDialogButtonWidth);
        const int buttonHeight = ScaleUi(kDialogButtonHeight);
        const int contentWidth = std::max(1, static_cast<int>(client.cx) - sidePadding * 2);
        const int buttonY = static_cast<int>(client.cy) - bottomPadding - buttonHeight;
        const int listHeight = std::max(ScaleUi(180), buttonY - listTop - ScaleUi(18));

        HWND prompt = CreateWindowExW(
            0, L"STATIC", state->prompt.c_str(),
            WS_CHILD | WS_VISIBLE,
            sidePadding, topPadding, contentWidth, promptHeight,
            handle, MenuId(kListPromptId), nullptr, nullptr);
        SetControlFont(prompt, true);

        state->listHandle = CreateWindowExW(
            WS_EX_CLIENTEDGE, L"LISTBOX", L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_VSCROLL | LBS_NOTIFY | LBS_NOINTEGRALHEIGHT,
            sidePadding, listTop, contentWidth, listHeight,
            handle, MenuId(kListBoxId), nullptr, nullptr);
        SetControlFont(state->listHandle);

        for (const auto& option : state->options) {
            SendMessageW(state->listHandle, LB_ADDSTRING, 0, reinterpret_cast<LPARAM>(option.c_str()));
        }
        if (!state->options.empty()) {
            SendMessageW(state->listHandle, LB_SETCURSEL, 0, 0);
        }

        const int cancelX = static_cast<int>(client.cx) - sidePadding - buttonWidth;
        const int okX = cancelX - buttonGap - buttonWidth;

        HWND okButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"OK", L"確定"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
            okX, buttonY, buttonWidth, buttonHeight,
            handle, MenuId(IDOK), nullptr, nullptr);
        SetControlFont(okButton);

        HWND cancelButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"Cancel", L"取消"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            cancelX, buttonY, buttonWidth, buttonHeight,
            handle, MenuId(IDCANCEL), nullptr, nullptr);
        SetControlFont(cancelButton);

        SetFocus(state->listHandle);
        return 0;
    }
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
    case WM_CTLCOLORLISTBOX:
        return static_cast<LRESULT>(HandleDialogControlColor(message, wParam, lParam));
    case WM_COMMAND:
        if (!state) {
            break;
        }

        if (LOWORD(wParam) == kListBoxId && HIWORD(wParam) == LBN_DBLCLK) {
            SendMessageW(handle, WM_COMMAND, MAKEWPARAM(IDOK, BN_CLICKED), 0);
            return 0;
        }

        switch (LOWORD(wParam)) {
        case IDOK: {
            const int selected = static_cast<int>(SendMessageW(state->listHandle, LB_GETCURSEL, 0, 0));
            if (selected == LB_ERR) {
                MessageBoxW(
                    handle,
                    UiText(L"Please select an item first.", L"請先選擇一個項目。"),
                    state->title.c_str(),
                    MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
                return 0;
            }
            state->selectedIndex = selected;
            state->accepted = true;
            DestroyWindow(handle);
            return 0;
        }
        case IDCANCEL:
            state->accepted = false;
            DestroyWindow(handle);
            return 0;
        default:
            break;
        }
        break;
    case WM_CLOSE:
        if (state) {
            state->accepted = false;
        }
        DestroyWindow(handle);
        return 0;
    default:
        break;
    }

    return DefWindowProcW(handle, message, wParam, lParam);
}

void RefreshActionDialogUI(HWND handle) {
    if (!handle) return;
    SetWindowTextW(handle, UiText(L"PrtEasyBAK", L"PrtEasyBAK"));

    HWND prompt = GetDlgItem(handle, kActionPromptId);
    std::wstring promptText = std::wstring(UiText(
            L"Choose the action you want to run:\r\n\r\n",
            L"請選擇要執行的功能：\r\n\r\n")) +
        UiText(
            L"Backup: back up the local printers on this PC\r\n",
            L"備份：備份本機印表機\r\n") +
        UiText(
            L"Restore: restore printers from PrinterBackup\r\n",
            L"還原：從 PrinterBackup 還原到本機\r\n") +
        UiText(
            L"Cancel: close this tool",
            L"取消：離開程式");
    if (prompt) SetWindowTextW(prompt, promptText.c_str());

    HWND aboutBtn = GetDlgItem(handle, kActionAboutId);
    if (aboutBtn) SetWindowTextW(aboutBtn, UiText(L"About", L"關於"));

    HWND backupBtn = GetDlgItem(handle, IDYES);
    if (backupBtn) SetWindowTextW(backupBtn, UiText(L"Backup", L"備份"));

    HWND restoreBtn = GetDlgItem(handle, IDNO);
    if (restoreBtn) SetWindowTextW(restoreBtn, UiText(L"Restore", L"還原"));

    HWND cancelBtn = GetDlgItem(handle, IDCANCEL);
    if (cancelBtn) SetWindowTextW(cancelBtn, UiText(L"Cancel", L"取消"));
}

void SaveLanguageSettingToIni() {
    std::wstring text = ReadTextFile(g_configPath);
    std::wstring langVal = (g_appLanguage == AppLanguage::TraditionalChinese) ? L"zh-TW" : L"en";

    if (text.empty()) {
        WriteUtf8File(g_configPath, L"ui_lang=" + langVal + L"\r\n");
        return;
    }

    auto lines = SplitLines(text);
    bool found = false;
    for (auto& line : lines) {
        std::wstring trimmed = Trim(line);
        if (trimmed.empty() || StartsWithI(trimmed, L"#") || StartsWithI(trimmed, L";")) {
            continue;
        }
        auto pos = trimmed.find(L'=');
        if (pos != std::wstring::npos) {
            std::wstring key = ToLowerCopy(Trim(trimmed.substr(0, pos)));
            if (key == L"lang" || key == L"language" || key == L"ui_lang") {
                line = trimmed.substr(0, pos + 1) + langVal;
                found = true;
                break;
            }
        }
    }

    std::wstring newText;
    for (const auto& line : lines) {
        newText += line + L"\r\n";
    }

    if (!found) {
        newText += L"ui_lang=" + langVal + L"\r\n";
    }

    WriteUtf8File(g_configPath, newText);
}

void SwitchLanguage(AppLanguage lang, HWND dialogHandle) {
    g_appLanguage = lang;
    ApplyLanguageSetting(lang == AppLanguage::TraditionalChinese ? L"zh-TW" : L"en");
    RefreshActionDialogUI(dialogHandle);
    SaveLanguageSettingToIni();

    if (dialogHandle) {
        RedrawWindow(dialogHandle, nullptr, nullptr, RDW_INVALIDATE | RDW_UPDATENOW | RDW_ERASE | RDW_ALLCHILDREN);
    }
}

LRESULT CALLBACK ActionDialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    ActionDialogState* state = reinterpret_cast<ActionDialogState*>(GetWindowLongPtrW(handle, GWLP_USERDATA));

    switch (message) {
    case WM_NCCREATE: {
        auto* create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
        return TRUE;
    }
    case WM_CREATE: {
        state = reinterpret_cast<ActionDialogState*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
        if (!state) {
            return -1;
        }

        ApplyWindowIcons(handle);
        const SIZE client = GetClientSize(handle);
        const int sidePadding = ScaleUi(24);
        const int topPadding = ScaleUi(22);
        const int bottomPadding = ScaleUi(24);
        const int buttonHeight = ScaleUi(40);
        const int buttonWidthPreferred = ScaleUi(132);
        const int gapPreferred = ScaleUi(22);
        const int aboutButtonWidth = ScaleUi(84);
        const int aboutButtonHeight = ScaleUi(30);
        const int buttonY = static_cast<int>(client.cy) - bottomPadding - buttonHeight;
        const int promptHeight = std::max(ScaleUi(118), buttonY - topPadding - ScaleUi(24));
        const int contentWidth = std::max(1, static_cast<int>(client.cx) - sidePadding * 2);

        HWND prompt = CreateWindowExW(
            0, L"STATIC", state->prompt.c_str(),
            WS_CHILD | WS_VISIBLE,
            sidePadding, topPadding, contentWidth, promptHeight,
            handle, MenuId(kActionPromptId), nullptr, nullptr);
        SetControlFont(prompt, true);

        HWND aboutButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"About", L"關於"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            static_cast<int>(client.cx) - sidePadding - aboutButtonWidth, topPadding, aboutButtonWidth, aboutButtonHeight,
            handle, MenuId(kActionAboutId), nullptr, nullptr);
        SetControlFont(aboutButton);

        // 在關於按鈕下方新增語言 ComboBox
        const int langComboWidth = ScaleUi(100);
        const int langComboX = static_cast<int>(client.cx) - sidePadding - langComboWidth;
        HWND langCombo = CreateWindowExW(0, WC_COMBOBOXW, L"",
            WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST | WS_TABSTOP,
            langComboX, topPadding + aboutButtonHeight + ScaleUi(8),
            langComboWidth, ScaleUi(100),
            handle, MenuId(kActionLangComboId), nullptr, nullptr);
        SetControlFont(langCombo);
        SendMessageW(langCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"English"));
        SendMessageW(langCombo, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(L"繁體中文"));
        SendMessageW(langCombo, CB_SETCURSEL, (g_appLanguage == AppLanguage::TraditionalChinese) ? 1 : 0, 0);

        int buttonWidth = buttonWidthPreferred;
        int gap = gapPreferred;
        const int availableWidth = std::max(1, static_cast<int>(client.cx) - sidePadding * 2);
        int totalWidth = buttonWidth * 3 + gap * 2;
        if (totalWidth > availableWidth) {
            gap = ScaleUi(16);
            totalWidth = buttonWidth * 3 + gap * 2;
        }
        if (totalWidth > availableWidth) {
            buttonWidth = std::max(ScaleUi(88), (availableWidth - gap * 2) / 3);
            totalWidth = buttonWidth * 3 + gap * 2;
        }
        const int startX = sidePadding + std::max(0, (availableWidth - totalWidth) / 2);

        HWND backupButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"Backup", L"備份"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
            startX, buttonY, buttonWidth, buttonHeight,
            handle, MenuId(IDYES), nullptr, nullptr);
        SetControlFont(backupButton);

        HWND restoreButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"Restore", L"還原"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            startX + buttonWidth + gap, buttonY, buttonWidth, buttonHeight,
            handle, MenuId(IDNO), nullptr, nullptr);
        SetControlFont(restoreButton);

        HWND cancelButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"Cancel", L"取消"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            startX + (buttonWidth + gap) * 2, buttonY, buttonWidth, buttonHeight,
            handle, MenuId(IDCANCEL), nullptr, nullptr);
        SetControlFont(cancelButton);

        SetFocus(backupButton);
        return 0;
    }
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
        return static_cast<LRESULT>(HandleDialogControlColor(message, wParam, lParam));
    case WM_COMMAND:
        if (!state) {
            break;
        }

        if (LOWORD(wParam) == kActionLangComboId && HIWORD(wParam) == CBN_SELCHANGE) {
            HWND hCombo = reinterpret_cast<HWND>(lParam);
            int sel = static_cast<int>(SendMessageW(hCombo, CB_GETCURSEL, 0, 0));
            if (sel == 1) {
                SwitchLanguage(AppLanguage::TraditionalChinese, handle);
            } else {
                SwitchLanguage(AppLanguage::English, handle);
            }
            return 0;
        }

        switch (LOWORD(wParam)) {
        case kActionAboutId:
            ShowAboutDialog();
            return 0;
        case IDYES:
            state->action = AppAction::Backup;
            state->accepted = true;
            DestroyWindow(handle);
            return 0;
        case IDNO:
            state->action = AppAction::Restore;
            state->accepted = true;
            DestroyWindow(handle);
            return 0;
        case IDCANCEL:
            state->action = AppAction::Cancel;
            state->accepted = false;
            DestroyWindow(handle);
            return 0;
        default:
            break;
        }
        break;
    case WM_CLOSE:
        if (state) {
            state->action = AppAction::Cancel;
            state->accepted = false;
        }
        DestroyWindow(handle);
        return 0;
    default:
        break;
    }

    return DefWindowProcW(handle, message, wParam, lParam);
}

void UpdateCheckListCountLabel(CheckListDialogState* state) {
    if (!state || !state->listHandle || !state->countHandle) {
        return;
    }

    const int total = ListView_GetItemCount(state->listHandle);
    int selected = 0;
    for (int i = 0; i < total; ++i) {
        if (ListView_GetCheckState(state->listHandle, i)) {
            ++selected;
        }
    }

    const std::wstring text =
        std::wstring(UiText(L"Selected: ", L"已勾選：")) +
        std::to_wstring(selected) +
        L" / " +
        std::to_wstring(total);
    SetWindowTextW(state->countHandle, text.c_str());
}

void SetAllCheckListItems(CheckListDialogState* state, bool checked) {
    if (!state || !state->listHandle) {
        return;
    }

    const int total = ListView_GetItemCount(state->listHandle);
    for (int i = 0; i < total; ++i) {
        ListView_SetCheckState(state->listHandle, i, checked ? TRUE : FALSE);
    }

    UpdateCheckListCountLabel(state);
}

LRESULT CALLBACK CheckListDialogProc(HWND handle, UINT message, WPARAM wParam, LPARAM lParam) {
    CheckListDialogState* state = reinterpret_cast<CheckListDialogState*>(GetWindowLongPtrW(handle, GWLP_USERDATA));

    switch (message) {
    case WM_NCCREATE: {
        auto* create = reinterpret_cast<CREATESTRUCTW*>(lParam);
        SetWindowLongPtrW(handle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(create->lpCreateParams));
        return TRUE;
    }
    case WM_CREATE: {
        state = reinterpret_cast<CheckListDialogState*>(GetWindowLongPtrW(handle, GWLP_USERDATA));
        if (!state) {
            return -1;
        }

        ApplyWindowIcons(handle);
        const SIZE client = GetClientSize(handle);
        const int sidePadding = ScaleUi(18);
        const int promptTop = ScaleUi(18);
        const int promptHeight = ScaleUi(30);
        const int descriptionTop = ScaleUi(54);
        const int descriptionHeight = ScaleUi(64);
        const int countTop = ScaleUi(124);
        const int countHeight = ScaleUi(24);
        const int listTop = ScaleUi(156);
        const int bottomPadding = ScaleUi(26);
        const int buttonGap = ScaleUi(16);
        const int buttonWidth = ScaleUi(kDialogButtonWidth);
        const int buttonHeight = ScaleUi(kDialogButtonHeight);
        const int utilityButtonWidth = ScaleUi(102);
        const int contentWidth = std::max(1, static_cast<int>(client.cx) - sidePadding * 2);
        const int footerY = static_cast<int>(client.cy) - bottomPadding - buttonHeight;
        const int listHeight = std::max(ScaleUi(220), footerY - listTop - ScaleUi(18));

        HWND prompt = CreateWindowExW(
            0, L"STATIC", state->prompt.c_str(),
            WS_CHILD | WS_VISIBLE,
            sidePadding, promptTop, contentWidth, promptHeight,
            handle, MenuId(kCheckPromptId), nullptr, nullptr);
        SetControlFont(prompt, true);

        HWND description = CreateWindowExW(
            0, L"STATIC", state->description.c_str(),
            WS_CHILD | WS_VISIBLE,
            sidePadding, descriptionTop, contentWidth, descriptionHeight,
            handle, MenuId(kCheckDescriptionId), nullptr, nullptr);
        SetControlFont(description);

        state->countHandle = CreateWindowExW(
            0, L"STATIC", L"",
            WS_CHILD | WS_VISIBLE,
            sidePadding, countTop, std::min(contentWidth, ScaleUi(320)), countHeight,
            handle, MenuId(kCheckCountId), nullptr, nullptr);
        SetControlFont(state->countHandle, true);

        state->listHandle = CreateWindowExW(
            WS_EX_CLIENTEDGE,
            WC_LISTVIEWW,
            L"",
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | LVS_REPORT | LVS_SHOWSELALWAYS | WS_VSCROLL,
            sidePadding, listTop, contentWidth, listHeight,
            handle, MenuId(kCheckListViewId), nullptr, nullptr);
        SetControlFont(state->listHandle);

        ListView_SetExtendedListViewStyle(
            state->listHandle,
            LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT | LVS_EX_DOUBLEBUFFER | LVS_EX_LABELTIP);
        ApplyListViewTheme(state->listHandle);

        LVCOLUMNW column{};
        column.mask = LVCF_TEXT | LVCF_WIDTH | LVCF_SUBITEM;
        const int columnWidth0 = std::max(ScaleUi(220), contentWidth * 34 / 100);
        const int columnWidth1 = std::max(ScaleUi(260), contentWidth * 40 / 100);
        const int columnWidth2 = std::max(ScaleUi(150), contentWidth - columnWidth0 - columnWidth1 - ScaleUi(6));
        column.iSubItem = 0;
        column.pszText = const_cast<LPWSTR>(UiText(L"Printer Name", L"印表機名稱"));
        column.cx = columnWidth0;
        ListView_InsertColumn(state->listHandle, 0, &column);

        column.iSubItem = 1;
        column.pszText = const_cast<LPWSTR>(UiText(L"Driver / Model", L"驅動 / 型號"));
        column.cx = columnWidth1;
        ListView_InsertColumn(state->listHandle, 1, &column);

        column.iSubItem = 2;
        column.pszText = const_cast<LPWSTR>(UiText(L"Port", L"連接埠"));
        column.cx = columnWidth2;
        ListView_InsertColumn(state->listHandle, 2, &column);

        for (int i = 0; i < static_cast<int>(state->items.size()); ++i) {
            LVITEMW item{};
            item.mask = LVIF_TEXT;
            item.iItem = i;
            item.iSubItem = 0;
            item.pszText = const_cast<LPWSTR>(state->items[static_cast<std::size_t>(i)].name.c_str());
            ListView_InsertItem(state->listHandle, &item);
            ListView_SetItemText(state->listHandle, i, 1, const_cast<LPWSTR>(state->items[static_cast<std::size_t>(i)].detail.c_str()));
            ListView_SetItemText(state->listHandle, i, 2, const_cast<LPWSTR>(state->items[static_cast<std::size_t>(i)].port.c_str()));
            ListView_SetCheckState(state->listHandle, i, state->items[static_cast<std::size_t>(i)].checked ? TRUE : FALSE);
        }

        const int cancelX = static_cast<int>(client.cx) - sidePadding - buttonWidth;
        const int okX = cancelX - buttonGap - buttonWidth;

        HWND selectAllButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"Select All", L"全選"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            sidePadding, footerY, utilityButtonWidth, buttonHeight,
            handle, MenuId(kCheckSelectAllId), nullptr, nullptr);
        SetControlFont(selectAllButton);

        HWND clearButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"Clear", L"清除"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            sidePadding + utilityButtonWidth + ScaleUi(16), footerY, utilityButtonWidth, buttonHeight,
            handle, MenuId(kCheckClearId), nullptr, nullptr);
        SetControlFont(clearButton);

        HWND okButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"OK", L"確定"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_DEFPUSHBUTTON,
            okX, footerY, buttonWidth, buttonHeight,
            handle, MenuId(IDOK), nullptr, nullptr);
        SetControlFont(okButton);

        HWND cancelButton = CreateWindowExW(
            0, L"BUTTON", UiText(L"Cancel", L"取消"),
            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
            cancelX, footerY, buttonWidth, buttonHeight,
            handle, MenuId(IDCANCEL), nullptr, nullptr);
        SetControlFont(cancelButton);

        UpdateCheckListCountLabel(state);
        SetFocus(state->listHandle);
        return 0;
    }
    case WM_CTLCOLORDLG:
    case WM_CTLCOLORSTATIC:
        return static_cast<LRESULT>(HandleDialogControlColor(message, wParam, lParam));
    case WM_NOTIFY:
        if (!state) {
            break;
        }
        if (reinterpret_cast<LPNMHDR>(lParam)->idFrom == kCheckListViewId) {
            const auto* header = reinterpret_cast<LPNMHDR>(lParam);
            if (header->code == LVN_ITEMCHANGED || header->code == NM_CLICK) {
                UpdateCheckListCountLabel(state);
            }
            if (header->code == NM_DBLCLK) {
                const auto* info = reinterpret_cast<LPNMITEMACTIVATE>(lParam);
                if (info->iItem >= 0) {
                    const BOOL checked = ListView_GetCheckState(state->listHandle, info->iItem);
                    ListView_SetCheckState(state->listHandle, info->iItem, checked ? FALSE : TRUE);
                    UpdateCheckListCountLabel(state);
                    return 0;
                }
            }
        }
        break;
    case WM_COMMAND:
        if (!state) {
            break;
        }

        switch (LOWORD(wParam)) {
        case kCheckSelectAllId:
            SetAllCheckListItems(state, true);
            return 0;
        case kCheckClearId:
            SetAllCheckListItems(state, false);
            return 0;
        case IDOK: {
            state->selectedIndices.clear();
            const int total = ListView_GetItemCount(state->listHandle);
            for (int i = 0; i < total; ++i) {
                if (ListView_GetCheckState(state->listHandle, i)) {
                    state->selectedIndices.push_back(i);
                }
            }

            if (state->selectedIndices.empty()) {
                MessageBoxW(
                    handle,
                    UiText(L"Please select at least one printer.", L"請至少勾選一台印表機。"),
                    state->title.c_str(),
                    MB_OK | MB_ICONINFORMATION | MB_TOPMOST);
                return 0;
            }

            state->accepted = true;
            DestroyWindow(handle);
            return 0;
        }
        case IDCANCEL:
            state->accepted = false;
            DestroyWindow(handle);
            return 0;
        default:
            break;
        }
        break;
    case WM_CLOSE:
        if (state) {
            state->accepted = false;
        }
        DestroyWindow(handle);
        return 0;
    default:
        break;
    }

    return DefWindowProcW(handle, message, wParam, lParam);
}

template <typename TState>
bool RunModalWindow(const wchar_t* className,
                    WNDPROC procedure,
                    TState& state,
                    int width,
                    int height) {
    static HINSTANCE instance = GetModuleHandleW(nullptr);
    const DWORD exStyle = WS_EX_DLGMODALFRAME | WS_EX_CONTROLPARENT;
    const DWORD style = WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_CLIPCHILDREN;
    const int clientWidth = ScaleUi(width);
    const int clientHeight = ScaleUi(height);
    const SIZE windowSize = GetWindowSizeForClientArea(clientWidth, clientHeight, style, exStyle);

    WNDCLASSEXW windowClass{};
    windowClass.cbSize = sizeof(windowClass);
    windowClass.lpfnWndProc = procedure;
    windowClass.hInstance = instance;
    windowClass.hIcon = GetAppIconLarge();
    windowClass.hIconSm = GetAppIconSmall();
    windowClass.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    windowClass.hbrBackground = GetDialogBackgroundBrush();
    windowClass.lpszClassName = className;
    RegisterClassExW(&windowClass);

    HWND owner = GetActiveWindow();
    HWND handle = CreateWindowExW(
        exStyle,
        className,
        state.title.c_str(),
        style,
        CW_USEDEFAULT, CW_USEDEFAULT, windowSize.cx, windowSize.cy,
        owner, nullptr, instance, &state);

    if (!handle) {
        return false;
    }

    ApplyWindowIcons(handle);
    CenterWindowOnOwner(handle, owner, windowSize.cx, windowSize.cy);
    if (owner) {
        EnableWindow(owner, FALSE);
    }

    ShowWindow(handle, SW_SHOW);
    UpdateWindow(handle);

    MSG msg{};
    while (IsWindow(handle) && GetMessageW(&msg, nullptr, 0, 0) > 0) {
        if (!IsDialogMessageW(handle, &msg)) {
            TranslateMessage(&msg);
            DispatchMessageW(&msg);
        }
    }

    if (owner) {
        EnableWindow(owner, TRUE);
        SetActiveWindow(owner);
    }

    return state.accepted;
}

bool ShowSelectionDialog(const std::wstring& title,
                         const std::wstring& prompt,
                         const std::vector<std::wstring>& options,
                         std::wstring& selectedValue) {
    if (options.empty()) {
        return false;
    }

    InitUiCommonControls();

    SelectionDialogState state;
    state.title = title;
    state.prompt = prompt;
    state.options = options;

    if (!RunModalWindow(L"PrtEasyBAKSelectionDialog", SelectionDialogProc, state, kSelectionWidth, kSelectionHeight)) {
        return false;
    }
    if (state.selectedIndex < 0 || state.selectedIndex >= static_cast<int>(state.options.size())) {
        return false;
    }

    selectedValue = state.options[static_cast<std::size_t>(state.selectedIndex)];
    return true;
}

void ShowAboutDialog() {
    InitUiCommonControls();

    AboutDialogState state;
    state.title = UiText(L"About Windows Printer Backup Restore", L"關於 Windows 印表機備份與還原工具");
    state.content = BuildAboutText();

    RunModalWindow(L"PrtEasyBAKAboutDialog", AboutDialogProc, state, kAboutWidth, kAboutHeight);
}

AppAction ShowActionDialog() {
    InitUiCommonControls();

    ActionDialogState state;
    state.title = L"PrtEasyBAK";
    state.prompt =
        std::wstring(UiText(
            L"Choose the action you want to run:\r\n\r\n",
            L"請選擇要執行的功能：\r\n\r\n")) +
        UiText(
            L"Backup: back up the local printers on this PC\r\n",
            L"備份：備份本機印表機\r\n") +
        UiText(
            L"Restore: restore printers from PrinterBackup\r\n",
            L"還原：從 PrinterBackup 還原到本機\r\n") +
        UiText(
            L"Cancel: close this tool",
            L"取消：離開程式");

    if (!RunModalWindow(L"PrtEasyBAKActionDialog", ActionDialogProc, state, kActionWidth, kActionHeight)) {
        return AppAction::Cancel;
    }
    return state.action;
}

bool ShowCheckListDialog(const std::wstring& title,
                         const std::wstring& prompt,
                         const std::wstring& description,
                         const std::vector<CheckListItem>& items,
                         std::vector<int>& selectedIndices) {
    if (items.empty()) {
        return false;
    }

    InitUiCommonControls();

    CheckListDialogState state;
    state.title = title;
    state.prompt = prompt;
    state.description = description;
    state.items = items;

    if (!RunModalWindow(L"PrtEasyBAKCheckListDialog", CheckListDialogProc, state, kCheckListWidth, kCheckListHeight)) {
        return false;
    }

    selectedIndices = state.selectedIndices;
    return !selectedIndices.empty();
}

bool RunProcess(const std::wstring& executable,
                const std::vector<std::wstring>& args,
                DWORD& exitCode,
                DWORD timeoutMs = 120000,
                const fs::path& workingDirectory = fs::path()) {
    std::wstring commandLine = BuildCommandLine(executable, args);
    std::vector<wchar_t> mutableCommandLine(commandLine.begin(), commandLine.end());
    mutableCommandLine.push_back(L'\0');

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    PROCESS_INFORMATION pi{};
    Log(L"Run: " + commandLine);

    const std::wstring workingDirText = workingDirectory.empty() ? g_baseDir.wstring() : workingDirectory.wstring();
    const BOOL created = CreateProcessW(
        nullptr,
        mutableCommandLine.data(),
        nullptr,
        nullptr,
        FALSE,
        CREATE_NO_WINDOW,
        nullptr,
        workingDirText.c_str(),
        &si,
        &pi);

    if (!created) {
        exitCode = GetLastError();
        Log(L"CreateProcess failed: " + FormatWin32Error(exitCode));
        return false;
    }

    DWORD waitResult = WAIT_OBJECT_0;
    if (!WaitForHandleWithMessages(pi.hProcess, timeoutMs, waitResult) || waitResult == WAIT_TIMEOUT) {
        TerminateProcess(pi.hProcess, WAIT_TIMEOUT);
        exitCode = WAIT_TIMEOUT;
        Log(L"Process timeout; terminated.");
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    if (!GetExitCodeProcess(pi.hProcess, &exitCode)) {
        exitCode = GetLastError();
        Log(L"GetExitCodeProcess failed: " + FormatWin32Error(exitCode));
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);
    Log(L"ExitCode: " + std::to_wstring(exitCode));
    return true;
}

void ReadPipeToString(HANDLE readPipe, std::string& output) {
    for (;;) {
        DWORD available = 0;
        if (!PeekNamedPipe(readPipe, nullptr, 0, nullptr, &available, nullptr) || available == 0) {
            break;
        }

        char buffer[4096];
        const DWORD toRead = (available > sizeof(buffer)) ? static_cast<DWORD>(sizeof(buffer)) : available;
        DWORD bytesRead = 0;
        if (!ReadFile(readPipe, buffer, toRead, &bytesRead, nullptr) || bytesRead == 0) {
            break;
        }

        output.append(buffer, buffer + bytesRead);
    }
}

bool RunProcessCapture(const std::wstring& executable,
                       const std::vector<std::wstring>& args,
                       DWORD& exitCode,
                       std::wstring& outputText,
                       DWORD timeoutMs = 120000,
                       const fs::path& workingDirectory = fs::path()) {
    std::wstring commandLine = BuildCommandLine(executable, args);
    std::vector<wchar_t> mutableCommandLine(commandLine.begin(), commandLine.end());
    mutableCommandLine.push_back(L'\0');

    SECURITY_ATTRIBUTES sa{};
    sa.nLength = sizeof(sa);
    sa.bInheritHandle = TRUE;
    sa.lpSecurityDescriptor = nullptr;

    HANDLE readPipe = nullptr;
    HANDLE writePipe = nullptr;
    if (!CreatePipe(&readPipe, &writePipe, &sa, 0)) {
        exitCode = GetLastError();
        return false;
    }
    SetHandleInformation(readPipe, HANDLE_FLAG_INHERIT, 0);

    STARTUPINFOW si{};
    si.cb = sizeof(si);
    si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;
    si.wShowWindow = SW_HIDE;
    si.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    si.hStdOutput = writePipe;
    si.hStdError = writePipe;

    PROCESS_INFORMATION pi{};
    Log(L"Run(capture): " + commandLine);

    const std::wstring workingDirText = workingDirectory.empty() ? g_baseDir.wstring() : workingDirectory.wstring();
    const BOOL created = CreateProcessW(
        nullptr,
        mutableCommandLine.data(),
        nullptr,
        nullptr,
        TRUE,
        CREATE_NO_WINDOW,
        nullptr,
        workingDirText.c_str(),
        &si,
        &pi);

    CloseHandle(writePipe);
    writePipe = nullptr;

    if (!created) {
        exitCode = GetLastError();
        CloseHandle(readPipe);
        Log(L"CreateProcess(capture) failed: " + FormatWin32Error(exitCode));
        return false;
    }

    const DWORD startTick = GetTickCount();
    std::string outputBytes;
    bool timedOut = false;
    for (;;) {
        PumpPendingMessages();
        ReadPipeToString(readPipe, outputBytes);

        const DWORD waitResult = WaitForSingleObject(pi.hProcess, 50);
        if (waitResult == WAIT_OBJECT_0) {
            break;
        }
        if (waitResult == WAIT_FAILED) {
            exitCode = GetLastError();
            Log(L"WaitForSingleObject failed: " + FormatWin32Error(exitCode));
            CloseHandle(readPipe);
            CloseHandle(pi.hThread);
            CloseHandle(pi.hProcess);
            return false;
        }

        if (timeoutMs != INFINITE && (GetTickCount() - startTick) >= timeoutMs) {
            TerminateProcess(pi.hProcess, WAIT_TIMEOUT);
            exitCode = WAIT_TIMEOUT;
            timedOut = true;
            Log(L"Captured process timeout; terminated.");
            break;
        }
    }

    ReadPipeToString(readPipe, outputBytes);
    CloseHandle(readPipe);

    if (!timedOut && !GetExitCodeProcess(pi.hProcess, &exitCode)) {
        exitCode = GetLastError();
        Log(L"GetExitCodeProcess(capture) failed: " + FormatWin32Error(exitCode));
        CloseHandle(pi.hThread);
        CloseHandle(pi.hProcess);
        return false;
    }

    CloseHandle(pi.hThread);
    CloseHandle(pi.hProcess);

    outputText = WideFromBytes(outputBytes, CP_UTF8);
    if (outputText.empty() && !outputBytes.empty()) {
        outputText = WideFromBytes(outputBytes, CP_ACP);
    }

    if (!outputText.empty()) {
        Log(L"Captured output: " + outputText);
    }
    Log(L"ExitCode(capture): " + std::to_wstring(exitCode));
    return !timedOut;
}

std::wstring EscapeForPowerShellSingleQuoted(std::wstring text) {
    std::size_t pos = 0;
    while ((pos = text.find(L'\'', pos)) != std::wstring::npos) {
        text.insert(pos, L"'");
        pos += 2;
    }
    return text;
}

bool RunPowerShell(const std::wstring& script, DWORD& exitCode, DWORD timeoutMs = 120000) {
    const std::wstring wrapped =
        L"[Console]::OutputEncoding=[System.Text.UTF8Encoding]::UTF8;"
        L"$OutputEncoding=[Console]::OutputEncoding;"
        + script;

    return RunProcess(
        L"powershell.exe",
        {L"-NoLogo", L"-NoProfile", L"-ExecutionPolicy", L"Bypass", L"-Command", wrapped},
        exitCode,
        timeoutMs,
        g_baseDir);
}

bool RunPowerShellCapture(const std::wstring& script,
                          std::wstring& output,
                          DWORD& exitCode,
                          DWORD timeoutMs = 120000) {
    const std::wstring wrapped =
        L"[Console]::OutputEncoding=[System.Text.UTF8Encoding]::UTF8;"
        L"$OutputEncoding=[Console]::OutputEncoding;"
        + script;

    return RunProcessCapture(
        L"powershell.exe",
        {L"-NoLogo", L"-NoProfile", L"-ExecutionPolicy", L"Bypass", L"-Command", wrapped},
        exitCode,
        output,
        timeoutMs,
        g_baseDir);
}

bool IsRunningAsAdmin() {
    BOOL isMember = FALSE;
    SID_IDENTIFIER_AUTHORITY authority = SECURITY_NT_AUTHORITY;
    PSID adminGroup = nullptr;
    if (!AllocateAndInitializeSid(
            &authority, 2,
            SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS,
            0, 0, 0, 0, 0, 0,
            &adminGroup)) {
        return false;
    }

    CheckTokenMembership(nullptr, adminGroup, &isMember);
    FreeSid(adminGroup);
    return isMember == TRUE;
}

std::wstring JoinCurrentArgumentsWithoutExecutable() {
    int argc = 0;
    LPWSTR* argv = CommandLineToArgvW(GetCommandLineW(), &argc);
    if (!argv || argc <= 1) {
        if (argv) {
            LocalFree(argv);
        }
        return L"";
    }

    std::wstring params;
    for (int i = 1; i < argc; ++i) {
        if (!params.empty()) {
            params.push_back(L' ');
        }
        params += QuoteArg(argv[i]);
    }
    LocalFree(argv);
    return params;
}

bool RelaunchAsAdmin() {
    wchar_t modulePath[MAX_PATH] = {};
    if (!GetModuleFileNameW(nullptr, modulePath, MAX_PATH)) {
        return false;
    }

    const std::wstring params = JoinCurrentArgumentsWithoutExecutable();
    HINSTANCE instance = ShellExecuteW(
        nullptr,
        L"runas",
        modulePath,
        params.empty() ? nullptr : params.c_str(),
        g_baseDir.wstring().c_str(),
        SW_SHOWNORMAL);

    return reinterpret_cast<INT_PTR>(instance) > 32;
}

bool EnsureSpoolerRunning() {
    SC_HANDLE scm = OpenSCManagerW(nullptr, nullptr, SC_MANAGER_CONNECT);
    if (!scm) {
        Log(L"OpenSCManager failed: " + FormatWin32Error(GetLastError()));
        return false;
    }

    SC_HANDLE service = OpenServiceW(scm, L"Spooler", SERVICE_QUERY_STATUS | SERVICE_START);
    if (!service) {
        Log(L"OpenService(Spooler) failed: " + FormatWin32Error(GetLastError()));
        CloseServiceHandle(scm);
        return false;
    }

    SERVICE_STATUS_PROCESS status{};
    DWORD bytesNeeded = 0;
    const bool queried = QueryServiceStatusEx(
        service,
        SC_STATUS_PROCESS_INFO,
        reinterpret_cast<LPBYTE>(&status),
        sizeof(status),
        &bytesNeeded) == TRUE;

    if (!queried) {
        Log(L"QueryServiceStatusEx failed: " + FormatWin32Error(GetLastError()));
    } else if (status.dwCurrentState == SERVICE_RUNNING) {
        CloseServiceHandle(service);
        CloseServiceHandle(scm);
        return true;
    } else {
        if (!StartServiceW(service, 0, nullptr)) {
            const DWORD error = GetLastError();
            if (error != ERROR_SERVICE_ALREADY_RUNNING) {
                Log(L"StartService(Spooler) failed: " + FormatWin32Error(error));
            }
        }

        for (int i = 0; i < 30; ++i) {
            ZeroMemory(&status, sizeof(status));
            if (QueryServiceStatusEx(
                    service,
                    SC_STATUS_PROCESS_INFO,
                    reinterpret_cast<LPBYTE>(&status),
                    sizeof(status),
                    &bytesNeeded) == TRUE &&
                status.dwCurrentState == SERVICE_RUNNING) {
                CloseServiceHandle(service);
                CloseServiceHandle(scm);
                return true;
            }
            SleepWithMessages(1000);
        }
    }

    CloseServiceHandle(service);
    CloseServiceHandle(scm);
    return false;
}

bool EnsureDirectory(const fs::path& path) {
    std::error_code ec;
    fs::create_directories(path, ec);
    return !ec;
}

bool RemoveDirectoryTree(const fs::path& path) {
    std::error_code ec;
    if (!fs::exists(path, ec)) {
        return true;
    }
    fs::remove_all(path, ec);
    return !ec;
}

bool CopyDirectoryTree(const fs::path& source, const fs::path& destination) {
    std::error_code ec;
    if (!fs::exists(source, ec) || !fs::is_directory(source, ec)) {
        return false;
    }

    fs::remove_all(destination, ec);
    ec.clear();
    fs::copy(source, destination, fs::copy_options::recursive | fs::copy_options::overwrite_existing, ec);
    return !ec;
}

fs::path GetModuleDirectory() {
    wchar_t modulePath[MAX_PATH] = {};
    if (!GetModuleFileNameW(nullptr, modulePath, MAX_PATH)) {
        return fs::current_path();
    }
    return fs::path(modulePath).parent_path();
}

std::wstring GetEnvVar(const wchar_t* name) {
    wchar_t buffer[32767] = {};
    const DWORD size = GetEnvironmentVariableW(name, buffer, 32767);
    if (size == 0 || size >= 32767) {
        return L"";
    }
    return buffer;
}

fs::path GetTempRoot() {
    if (!g_tempRoot.empty()) {
        return g_tempRoot;
    }

    wchar_t buffer[MAX_PATH] = {};
    DWORD length = GetTempPathW(MAX_PATH, buffer);
    fs::path base = (length > 0 && length < MAX_PATH) ? fs::path(buffer) : fs::temp_directory_path();
    g_tempRoot = base / (L"PrtEasyBAK_" + std::to_wstring(GetTickCount64()));
    EnsureDirectory(g_tempRoot);
    return g_tempRoot;
}

fs::path NewTempPath(const std::wstring& category, const std::wstring& leafName) {
    fs::path root = GetTempRoot() / category;
    EnsureDirectory(root);
    return root / leafName;
}

void CleanupTempRoot() {
    if (!g_tempRoot.empty()) {
        RemoveDirectoryTree(g_tempRoot);
        g_tempRoot.clear();
    }
}

std::wstring HexEncodeUtf8(const std::wstring& text) {
    static const wchar_t* kHex = L"0123456789ABCDEF";
    const std::string bytes = BytesFromWide(text, CP_UTF8);
    std::wstring output;
    output.reserve(bytes.size() * 2);
    for (unsigned char ch : bytes) {
        output.push_back(kHex[(ch >> 4) & 0x0F]);
        output.push_back(kHex[ch & 0x0F]);
    }
    return output;
}

int HexToNibble(wchar_t ch) {
    if (ch >= L'0' && ch <= L'9') {
        return ch - L'0';
    }
    if (ch >= L'A' && ch <= L'F') {
        return 10 + (ch - L'A');
    }
    if (ch >= L'a' && ch <= L'f') {
        return 10 + (ch - L'a');
    }
    return -1;
}

std::wstring HexDecodeUtf8(const std::wstring& hex) {
    if (hex.empty() || (hex.size() % 2) != 0) {
        return L"";
    }

    std::string bytes;
    bytes.reserve(hex.size() / 2);
    for (std::size_t i = 0; i < hex.size(); i += 2) {
        const int hi = HexToNibble(hex[i]);
        const int lo = HexToNibble(hex[i + 1]);
        if (hi < 0 || lo < 0) {
            return L"";
        }
        bytes.push_back(static_cast<char>((hi << 4) | lo));
    }

    return WideFromBytes(bytes, CP_UTF8);
}

std::wstring Base64DecodeUtf8(const std::wstring& base64) {
    if (base64.empty()) {
        return L"";
    }

    DWORD bytesNeeded = 0;
    if (!CryptStringToBinaryW(base64.c_str(),
                              0,
                              CRYPT_STRING_BASE64,
                              nullptr,
                              &bytesNeeded,
                              nullptr,
                              nullptr) || bytesNeeded == 0) {
        return L"";
    }

    std::vector<char> buffer(bytesNeeded);
    if (!CryptStringToBinaryW(base64.c_str(),
                              0,
                              CRYPT_STRING_BASE64,
                              reinterpret_cast<BYTE*>(buffer.data()),
                              &bytesNeeded,
                              nullptr,
                              nullptr)) {
        return L"";
    }

    return WideFromBytes(std::string(buffer.begin(), buffer.begin() + bytesNeeded), CP_UTF8);
}

std::map<std::wstring, std::wstring> ReadKeyValueFile(const fs::path& path) {
    std::map<std::wstring, std::wstring> values;
    const std::wstring text = ReadTextFile(path);
    for (const auto& rawLine : SplitLines(text)) {
        const std::wstring line = Trim(rawLine);
        if (line.empty() || StartsWithI(line, L"#") || StartsWithI(line, L";")) {
            continue;
        }
        const auto pos = line.find(L'=');
        if (pos == std::wstring::npos) {
            continue;
        }
        const std::wstring key = Trim(line.substr(0, pos));
        const std::wstring value = Trim(line.substr(pos + 1));
        values[key] = value;
    }
    return values;
}

void AppendKeyText(std::wstring& output, const wchar_t* key, const std::wstring& value) {
    output += key;
    output += L"=";
    output += HexEncodeUtf8(value);
    output += L"\r\n";
}

void AppendKeyBool(std::wstring& output, const wchar_t* key, bool value) {
    output += key;
    output += L"=";
    output += value ? L"1" : L"0";
    output += L"\r\n";
}

void AppendKeyNumber(std::wstring& output, const wchar_t* key, std::uint32_t value) {
    output += key;
    output += L"=";
    output += std::to_wstring(value);
    output += L"\r\n";
}

std::wstring GetTextValue(const std::map<std::wstring, std::wstring>& values, const wchar_t* key) {
    const auto it = values.find(key);
    if (it == values.end()) {
        return L"";
    }
    return HexDecodeUtf8(it->second);
}

bool GetBoolValue(const std::map<std::wstring, std::wstring>& values, const wchar_t* key, bool defaultValue = false) {
    const auto it = values.find(key);
    if (it == values.end()) {
        return defaultValue;
    }
    const std::wstring lower = ToLowerCopy(Trim(it->second));
    return lower == L"1" || lower == L"true" || lower == L"yes";
}

std::uint32_t GetNumberValue(const std::map<std::wstring, std::wstring>& values, const wchar_t* key, std::uint32_t defaultValue = 0) {
    const auto it = values.find(key);
    if (it == values.end()) {
        return defaultValue;
    }
    return static_cast<std::uint32_t>(_wtoi(it->second.c_str()));
}

std::wstring SafeRelativePath(const fs::path& target, const fs::path& base) {
    std::error_code ec;
    fs::path relative = fs::relative(target, base, ec);
    if (!ec) {
        return relative.wstring();
    }
    return target.wstring();
}

std::wstring SanitizeFileName(const std::wstring& value) {
    std::wstring output;
    output.reserve(value.size());
    for (wchar_t ch : value) {
        switch (ch) {
        case L'\\':
        case L'/':
        case L':':
        case L'*':
        case L'?':
        case L'"':
        case L'<':
        case L'>':
        case L'|':
            output.push_back(L'_');
            break;
        default:
            if (ch < 32) {
                output.push_back(L'_');
            } else {
                output.push_back(ch);
            }
            break;
        }
    }
    output = Trim(output);
    if (output.empty()) {
        output = L"Printer";
    }
    while (!output.empty() && (output.back() == L'.' || output.back() == L' ')) {
        output.pop_back();
    }
    if (output.empty()) {
        output = L"Printer";
    }
    return output;
}

bool OpenFolderPath(const fs::path& path) {
    HINSTANCE instance = ShellExecuteW(nullptr, L"open", path.wstring().c_str(), nullptr, nullptr, SW_SHOWNORMAL);
    return reinterpret_cast<INT_PTR>(instance) > 32;
}

bool LooksLikeConnectionPath(const std::wstring& path) {
    if (!StartsWithI(path, L"\\\\")) {
        return false;
    }

    const std::size_t first = path.find(L'\\', 2);
    if (first == std::wstring::npos) {
        return false;
    }
    const std::size_t second = path.find(L'\\', first + 1);
    return second == std::wstring::npos && first + 1 < path.size();
}

std::wstring BuildConnectionPath(const std::wstring& serverName, const std::wstring& shareName) {
    const std::wstring trimmedServer = Trim(serverName);
    const std::wstring trimmedShare = Trim(shareName);
    if (trimmedServer.empty() || trimmedShare.empty()) {
        return L"";
    }

    std::wstring server = trimmedServer;
    while (!server.empty() && server.front() == L'\\') {
        server.erase(server.begin());
    }

    std::wstring share = trimmedShare;
    while (!share.empty() && share.front() == L'\\') {
        share.erase(share.begin());
    }
    while (!share.empty() && share.back() == L'\\') {
        share.pop_back();
    }

    if (server.empty() || share.empty()) {
        return L"";
    }
    return L"\\\\" + server + L"\\" + share;
}

bool SplitConnectionPath(const std::wstring& path, std::wstring& serverName, std::wstring& shareName) {
    if (!LooksLikeConnectionPath(path)) {
        return false;
    }

    const std::size_t first = path.find(L'\\', 2);
    if (first == std::wstring::npos) {
        return false;
    }

    serverName = path.substr(2, first - 2);
    shareName = path.substr(first + 1);
    return !serverName.empty() && !shareName.empty();
}

bool IsExcludedPrinter(const NativePrinterRecord& printer) {
    const std::wstring name = ToLowerCopy(printer.name);
    const std::wstring driverName = ToLowerCopy(printer.driverName);
    const std::wstring portName = ToLowerCopy(printer.portName);

    const std::vector<std::wstring> virtualNamePatterns = {
        L"microsoft print to pdf",
        L"microsoft xps document writer",
        L"onenote",
        L"adobe pdf",
        L"fax",
        L"pdf",
        L"xps"
    };

    for (const auto& pattern : virtualNamePatterns) {
        if (name.find(pattern) != std::wstring::npos || driverName.find(pattern) != std::wstring::npos) {
            return true;
        }
    }

    const std::vector<std::wstring> virtualPortPrefixes = {
        L"portprompt:",
        L"file:",
        L"nul:",
        L"xpsport:",
        L"shrfax:",
        L"fax:",
        L"prompt:"
    };

    for (const auto& prefix : virtualPortPrefixes) {
        if (StartsWithI(portName, prefix)) {
            return true;
        }
    }

    return false;
}

std::vector<NativePrinterRecord> EnumeratePrinters() {
    std::vector<NativePrinterRecord> printers;

    DWORD needed = 0;
    DWORD returned = 0;
    EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, nullptr, 0, &needed, &returned);
    if (needed == 0) {
        return printers;
    }

    std::vector<BYTE> buffer(needed);
    if (!EnumPrintersW(PRINTER_ENUM_LOCAL | PRINTER_ENUM_CONNECTIONS, nullptr, 2, buffer.data(), needed, &needed, &returned)) {
        Log(L"EnumPrinters failed: " + FormatWin32Error(GetLastError()));
        return printers;
    }

    const auto* info = reinterpret_cast<const PRINTER_INFO_2W*>(buffer.data());
    for (DWORD i = 0; i < returned; ++i) {
        NativePrinterRecord record;
        record.name = info[i].pPrinterName ? info[i].pPrinterName : L"";
        record.serverName = info[i].pServerName ? info[i].pServerName : L"";
        record.shareName = info[i].pShareName ? info[i].pShareName : L"";
        record.driverName = info[i].pDriverName ? info[i].pDriverName : L"";
        record.portName = info[i].pPortName ? info[i].pPortName : L"";
        record.comment = info[i].pComment ? info[i].pComment : L"";
        record.location = info[i].pLocation ? info[i].pLocation : L"";
        record.datatype = info[i].pDatatype ? info[i].pDatatype : L"";
        record.printProcessor = info[i].pPrintProcessor ? info[i].pPrintProcessor : L"";
        record.attributes = info[i].Attributes;
        printers.push_back(record);
    }

    std::sort(printers.begin(), printers.end(), [](const NativePrinterRecord& left, const NativePrinterRecord& right) {
        return ToLowerCopy(left.name) < ToLowerCopy(right.name);
    });

    return printers;
}

std::wstring GetDefaultPrinterName() {
    DWORD needed = 0;
    GetDefaultPrinterW(nullptr, &needed);
    if (needed == 0) {
        return L"";
    }

    std::wstring value(static_cast<std::size_t>(needed), L'\0');
    if (!GetDefaultPrinterW(value.data(), &needed)) {
        return L"";
    }
    value.resize(wcslen(value.c_str()));
    return value;
}

bool PortExists(const std::wstring& portName) {
    DWORD needed = 0;
    DWORD returned = 0;
    EnumPortsW(nullptr, 1, nullptr, 0, &needed, &returned);
    if (needed == 0) {
        return false;
    }

    std::vector<BYTE> buffer(needed);
    if (!EnumPortsW(nullptr, 1, buffer.data(), needed, &needed, &returned)) {
        return false;
    }

    const auto* ports = reinterpret_cast<PORT_INFO_1W*>(buffer.data());
    for (DWORD i = 0; i < returned; ++i) {
        if (ports[i].pName && IEquals(ports[i].pName, portName)) {
            return true;
        }
    }
    return false;
}

std::vector<std::wstring> EnumeratePortNames() {
    std::vector<std::wstring> ports;

    DWORD needed = 0;
    DWORD returned = 0;
    EnumPortsW(nullptr, 1, nullptr, 0, &needed, &returned);
    if (needed == 0) {
        return ports;
    }

    std::vector<BYTE> buffer(needed);
    if (!EnumPortsW(nullptr, 1, buffer.data(), needed, &needed, &returned)) {
        return ports;
    }

    const auto* portInfo = reinterpret_cast<PORT_INFO_1W*>(buffer.data());
    for (DWORD i = 0; i < returned; ++i) {
        if (portInfo[i].pName) {
            ports.emplace_back(portInfo[i].pName);
        }
    }

    std::sort(ports.begin(), ports.end(), [](const std::wstring& left, const std::wstring& right) {
        return ToLowerCopy(left) < ToLowerCopy(right);
    });
    ports.erase(std::unique(ports.begin(), ports.end()), ports.end());
    return ports;
}

std::vector<std::wstring> GetUsbPortNames() {
    std::vector<std::wstring> ports;
    for (const auto& port : EnumeratePortNames()) {
        if (StartsWithI(port, L"USB")) {
            ports.push_back(port);
        }
    }
    return ports;
}

bool PrinterExists(const std::wstring& printerName) {
    HANDLE printer = nullptr;
    const BOOL opened = OpenPrinterW(const_cast<LPWSTR>(printerName.c_str()), &printer, nullptr);
    if (!opened) {
        return false;
    }
    ClosePrinter(printer);
    return true;
}

bool WaitForPrinterState(const std::wstring& printerName, bool shouldExist, int timeoutSeconds = 20) {
    for (int i = 0; i < timeoutSeconds; ++i) {
        if (PrinterExists(printerName) == shouldExist) {
            return true;
        }
        SleepWithMessages(1000);
    }
    return PrinterExists(printerName) == shouldExist;
}

bool DeletePrinterByName(const std::wstring& printerName) {
    HANDLE printer = nullptr;
    PRINTER_DEFAULTSW defaults{};
    defaults.DesiredAccess = PRINTER_ACCESS_ADMINISTER;
    if (!OpenPrinterW(const_cast<LPWSTR>(printerName.c_str()), &printer, &defaults)) {
        return false;
    }

    const BOOL deleted = DeletePrinter(printer);
    const DWORD errorCode = deleted ? ERROR_SUCCESS : GetLastError();
    ClosePrinter(printer);
    if (!deleted) {
        Log(L"DeletePrinter failed for " + printerName + L": " + FormatWin32Error(errorCode));
    }
    return deleted == TRUE;
}

std::wstring QueryPrinterPnpId(const std::wstring& printerName) {
    const std::wstring script =
        L"$p = Get-CimInstance Win32_Printer | Where-Object { $_.Name -eq '" + EscapeForPowerShellSingleQuoted(printerName) + L"' } | "
        L"Select-Object -First 1 -ExpandProperty PNPDeviceID; "
        L"if ($p) { [Convert]::ToBase64String([System.Text.Encoding]::UTF8.GetBytes([string]$p)) }";

    std::wstring output;
    DWORD exitCode = 0;
    if (!RunPowerShellCapture(script, output, exitCode, 30000) || exitCode != 0) {
        return L"";
    }

    return Base64DecodeUtf8(Trim(output));
}

std::wstring NormalizeUsbPnpDeviceId(const std::wstring& pnpId) {
    if (Trim(pnpId).empty()) {
        return L"";
    }

    std::wstring upper = ToLowerCopy(Trim(pnpId));
    std::transform(upper.begin(), upper.end(), upper.begin(), [](wchar_t ch) {
        return static_cast<wchar_t>(::towupper(ch));
    });

    const std::size_t pos = upper.rfind(L"USB");
    if (pos != std::wstring::npos && pos + 3 < upper.size()) {
        bool allDigits = true;
        for (std::size_t i = pos + 3; i < upper.size(); ++i) {
            if (!iswdigit(upper[i])) {
                allDigits = false;
                break;
            }
        }
        if (allDigits) {
            upper.erase(pos);
        }
    }

    return Trim(upper);
}

bool ParsePowerShellKeyValueOutput(const std::wstring& output, std::map<std::wstring, std::wstring>& values) {
    for (const auto& rawLine : SplitLines(output)) {
        const std::wstring line = Trim(rawLine);
        if (line.empty()) {
            continue;
        }
        const auto pos = line.find(L'=');
        if (pos == std::wstring::npos) {
            continue;
        }

        const std::wstring key = Trim(line.substr(0, pos));
        const std::wstring encoded = Trim(line.substr(pos + 1));
        values[key] = Base64DecodeUtf8(encoded);
    }
    return !values.empty();
}

bool QueryPortInfo(const std::wstring& portName, PortInfo& info) {
    info = {};
    info.name = portName;

    const std::wstring script =
        L"function enc([object]$v) { "
        L"  if ($null -eq $v) { return '' } "
        L"  return [Convert]::ToBase64String([System.Text.Encoding]::UTF8.GetBytes([string]$v)) "
        L"} "
        L"$port = Get-PrinterPort -Name '" + EscapeForPowerShellSingleQuoted(portName) + L"' -ErrorAction Stop; "
        L"$map = [ordered]@{ "
        L"  Name=$port.Name; "
        L"  PortMonitor=$port.PortMonitor; "
        L"  PrinterHostAddress=$port.PrinterHostAddress; "
        L"  HostAddress=$port.HostAddress; "
        L"  QueueName=$port.QueueName; "
        L"  Queue=$port.Queue; "
        L"  LprQueueName=$port.LprQueueName; "
        L"  PortNumber=$port.PortNumber; "
        L"  SNMP=$port.SNMP; "
        L"  SNMPEnabled=$port.SNMPEnabled; "
        L"  SNMPCommunity=$port.SNMPCommunity; "
        L"  Protocol=$port.Protocol; "
        L"  LprByteCounting=$port.LprByteCounting "
        L"}; "
        L"$map.GetEnumerator() | ForEach-Object { '{0}={1}' -f $_.Key, (enc $_.Value) }";

    std::wstring output;
    DWORD exitCode = 0;
    std::map<std::wstring, std::wstring> values;
    if (RunPowerShellCapture(script, output, exitCode, 30000) && exitCode == 0) {
        ParsePowerShellKeyValueOutput(output, values);
    }

    if (values.count(L"Name")) {
        info.name = values[L"Name"];
    }

    const auto getValue = [&values](const wchar_t* key1, const wchar_t* key2 = nullptr, const wchar_t* key3 = nullptr) {
        auto it = values.find(key1);
        if (it != values.end() && !Trim(it->second).empty()) {
            return it->second;
        }
        if (key2) {
            it = values.find(key2);
            if (it != values.end() && !Trim(it->second).empty()) {
                return it->second;
            }
        }
        if (key3) {
            it = values.find(key3);
            if (it != values.end() && !Trim(it->second).empty()) {
                return it->second;
            }
        }
        return std::wstring();
    };

    info.printerHostAddress = getValue(L"PrinterHostAddress", L"HostAddress");
    info.queueName = getValue(L"QueueName", L"Queue", L"LprQueueName");
    info.portMonitor = getValue(L"PortMonitor");
    info.protocol = getValue(L"Protocol");
    info.portNumber = static_cast<std::uint32_t>(_wtoi(getValue(L"PortNumber").c_str()));
    info.snmpIndex = static_cast<std::uint32_t>(_wtoi(getValue(L"SNMP").c_str()));
    info.snmpEnabled = IEquals(getValue(L"SNMPEnabled"), L"True") || IEquals(getValue(L"SNMPEnabled"), L"1");
    info.snmpCommunity = getValue(L"SNMPCommunity");
    info.lprByteCounting = IEquals(getValue(L"LprByteCounting"), L"True") || IEquals(getValue(L"LprByteCounting"), L"1");

    if (info.printerHostAddress.empty() && StartsWithI(portName, L"IP_")) {
        info.printerHostAddress = portName.substr(3);
    }

    if (StartsWithI(portName, L"USB")) {
        info.kind = L"USB";
    } else if (StartsWithI(portName, L"WSD")) {
        info.kind = L"WSD";
    } else if (StartsWithI(portName, L"LPT")) {
        info.kind = L"LPT";
    } else if (!info.queueName.empty()) {
        info.kind = L"LPR";
    } else if (!info.printerHostAddress.empty() || StartsWithI(portName, L"IP_")) {
        info.kind = L"TCP";
    } else if (ContainsI(info.portMonitor, L"Local Port")) {
        info.kind = L"Local";
    } else {
        info.kind = L"Other";
    }

    return true;
}

std::wstring GetRegistryStringValue(HKEY root, const std::wstring& keyPath, const std::wstring& valueName) {
    HKEY keyHandle = nullptr;
    if (RegOpenKeyExW(root, keyPath.c_str(), 0, KEY_READ, &keyHandle) != ERROR_SUCCESS) {
        return L"";
    }

    DWORD type = 0;
    DWORD size = 0;
    std::wstring result;
    if (RegQueryValueExW(keyHandle, valueName.c_str(), nullptr, &type, nullptr, &size) == ERROR_SUCCESS &&
        (type == REG_SZ || type == REG_EXPAND_SZ)) {
        std::vector<wchar_t> buffer(size / sizeof(wchar_t) + 1, L'\0');
        if (RegQueryValueExW(keyHandle, valueName.c_str(), nullptr, &type, reinterpret_cast<LPBYTE>(buffer.data()), &size) == ERROR_SUCCESS) {
            result.assign(buffer.data());
        }
    }

    RegCloseKey(keyHandle);
    return result;
}

std::wstring GetDriverInfPathFromRegistry(const std::wstring& driverName, const std::wstring& environmentName) {
    const std::vector<std::wstring> environments = {
        environmentName,
        L"Windows x64",
        L"Windows NT x86"
    };
    const std::vector<std::wstring> versions = {
        L"Version-4",
        L"Version-3",
        L"Version-2"
    };

    for (const auto& env : environments) {
        if (Trim(env).empty()) {
            continue;
        }

        for (const auto& version : versions) {
            const std::wstring keyPath =
                L"SYSTEM\\CurrentControlSet\\Control\\Print\\Environments\\" + env + L"\\Drivers\\" + version + L"\\" + driverName;
            std::wstring infPath = Trim(GetRegistryStringValue(HKEY_LOCAL_MACHINE, keyPath, L"InfPath"));
            if (infPath.empty()) {
                continue;
            }

            if (infPath.find(L":\\") == std::wstring::npos) {
                const std::wstring windir = GetEnvVar(L"WINDIR");
                infPath = (windir.empty() ? L"C:\\Windows" : windir) + L"\\INF\\" + infPath;
            }

            std::error_code ec;
            if (fs::exists(infPath, ec)) {
                return infPath;
            }
        }
    }

    return L"";
}

std::vector<std::wstring> SplitMultiSz(const wchar_t* multiString) {
    std::vector<std::wstring> values;
    if (multiString == nullptr) {
        return values;
    }

    const wchar_t* current = multiString;
    while (*current != L'\0') {
        std::wstring item = current;
        if (!Trim(item).empty()) {
            values.push_back(item);
        }
        current += item.size() + 1;
    }
    return values;
}

bool IsDriverStoreDirectory(const fs::path& folderPath) {
    const std::wstring lowered = ToLowerCopy(folderPath.wstring());
    return lowered.find(L"\\system32\\driverstore\\filerepository\\") != std::wstring::npos;
}

fs::path FindFirstInfInFolder(const fs::path& folder) {
    std::error_code ec;
    if (!fs::exists(folder, ec) || !fs::is_directory(folder, ec)) {
        return {};
    }

    for (const auto& entry : fs::directory_iterator(folder, ec)) {
        if (ec) {
            break;
        }
        if (entry.is_regular_file() && IEquals(entry.path().extension().wstring(), L".inf")) {
            return entry.path();
        }
    }
    return {};
}

bool FindBestDriverStorePackage(const std::wstring& driverName,
                                const std::vector<std::wstring>& fileHints,
                                fs::path& folderPath,
                                fs::path& infPath) {
    const std::wstring windir = GetEnvVar(L"WINDIR");
    const fs::path root = fs::path(windir.empty() ? L"C:\\Windows" : windir) / L"System32" / L"DriverStore" / L"FileRepository";
    std::error_code ec;
    if (!fs::exists(root, ec)) {
        return false;
    }

    std::set<std::wstring> hintNames;
    for (const auto& hint : fileHints) {
        const std::wstring leaf = ToLowerCopy(fs::path(hint).filename().wstring());
        if (!leaf.empty()) {
            hintNames.insert(leaf);
        }
    }

    for (const auto& entry : fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            break;
        }
        if (!entry.is_regular_file()) {
            continue;
        }

        const std::wstring leaf = ToLowerCopy(entry.path().filename().wstring());
        if (!hintNames.empty() && hintNames.find(leaf) != hintNames.end()) {
            fs::path candidateInf = IEquals(entry.path().extension().wstring(), L".inf")
                                        ? entry.path()
                                        : FindFirstInfInFolder(entry.path().parent_path());
            if (!candidateInf.empty()) {
                folderPath = candidateInf.parent_path();
                infPath = candidateInf;
                return true;
            }
        }
    }

    const std::wstring driverNameLower = ToLowerCopy(driverName);
    for (const auto& entry : fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            break;
        }
        if (!entry.is_regular_file() || !IEquals(entry.path().extension().wstring(), L".inf")) {
            continue;
        }

        const std::wstring text = ToLowerCopy(ReadTextFile(entry.path()));
        if (!text.empty() && text.find(driverNameLower) != std::wstring::npos) {
            folderPath = entry.path().parent_path();
            infPath = entry.path();
            return true;
        }
    }

    return false;
}

bool ResolveDriverPackageFromPrinter(const std::wstring& printerName,
                                     const std::wstring& expectedDriverName,
                                     std::wstring& resolvedDriverName,
                                     fs::path& sourceFolder,
                                     fs::path& infPath,
                                     std::wstring& errorText) {
    HANDLE printerHandle = nullptr;
    if (!OpenPrinterW(const_cast<LPWSTR>(printerName.c_str()), &printerHandle, nullptr)) {
        errorText = L"無法開啟印表機：" + printerName + L"。";
        return false;
    }

    DWORD needed = 0;
    GetPrinterDriverW(printerHandle, nullptr, 6, nullptr, 0, &needed);
    if (needed == 0) {
        ClosePrinter(printerHandle);
        errorText = L"無法讀取印表機驅動資訊。";
        return false;
    }

    std::vector<BYTE> buffer(needed);
    if (!GetPrinterDriverW(printerHandle, nullptr, 6, buffer.data(), needed, &needed)) {
        ClosePrinter(printerHandle);
        errorText = L"GetPrinterDriver 失敗。";
        return false;
    }

    const auto* driver = reinterpret_cast<const DRIVER_INFO_6W*>(buffer.data());
    resolvedDriverName = driver->pName ? driver->pName : expectedDriverName;

    std::wstring infPathText = GetDriverInfPathFromRegistry(
        resolvedDriverName,
        driver->pEnvironment ? driver->pEnvironment : L"Windows x64");

    std::vector<std::wstring> fileHints;
    auto addHint = [&fileHints](const std::wstring& value) {
        if (!Trim(value).empty()) {
            fileHints.push_back(value);
        }
    };

    addHint(infPathText);
    addHint(driver->pDriverPath ? driver->pDriverPath : L"");
    addHint(driver->pDataFile ? driver->pDataFile : L"");
    addHint(driver->pConfigFile ? driver->pConfigFile : L"");
    addHint(driver->pHelpFile ? driver->pHelpFile : L"");
    for (const auto& dependent : SplitMultiSz(driver->pDependentFiles)) {
        addHint(dependent);
    }

    ClosePrinter(printerHandle);

    std::error_code ec;
    if (!infPathText.empty()) {
        infPath = fs::path(infPathText);
        if (fs::exists(infPath, ec) && IsDriverStoreDirectory(infPath.parent_path())) {
            sourceFolder = infPath.parent_path();
            return true;
        }
    }

    if (FindBestDriverStorePackage(resolvedDriverName, fileHints, sourceFolder, infPath)) {
        return true;
    }

    if (!infPathText.empty()) {
        infPath = fs::path(infPathText);
        if (fs::exists(infPath, ec)) {
            sourceFolder = infPath.parent_path();
            return true;
        }
    }

    if (!fileHints.empty()) {
        const fs::path hintPath = fs::path(fileHints.front());
        if (fs::exists(hintPath, ec)) {
            sourceFolder = hintPath.parent_path();
            fs::path firstInf = FindFirstInfInFolder(sourceFolder);
            if (!firstInf.empty()) {
                infPath = firstInf;
                return true;
            }
        }
    }

    errorText = L"找不到驅動 INF 或 DriverStore 資料夾：" + resolvedDriverName;
    return false;
}

bool CopyDriverPackage(const NativePrinterRecord& printer,
                       std::map<std::wstring, DriverBackupInfo>& driverCache,
                       DriverBackupInfo& driverInfo,
                       std::wstring& errorText) {
    const std::wstring cacheKey = ToLowerCopy(printer.driverName);
    const auto cached = driverCache.find(cacheKey);
    if (cached != driverCache.end()) {
        driverInfo = cached->second;
        return true;
    }

    std::wstring resolvedDriverName;
    fs::path sourceFolder;
    fs::path infPath;
    if (!ResolveDriverPackageFromPrinter(printer.name, printer.driverName, resolvedDriverName, sourceFolder, infPath, errorText)) {
        return false;
    }

    const std::wstring folderName = SanitizeFileName(resolvedDriverName + L"__" + sourceFolder.filename().wstring());
    const fs::path targetFolder = g_driversRoot / folderName;
    if (!CopyDirectoryTree(sourceFolder, targetFolder)) {
        errorText = L"複製驅動資料夾失敗：" + sourceFolder.wstring();
        return false;
    }

    fs::path copiedInf = targetFolder / infPath.filename();
    std::error_code ec;
    if (!fs::exists(copiedInf, ec)) {
        copiedInf = FindFirstInfInFolder(targetFolder);
    }
    if (copiedInf.empty() || !fs::exists(copiedInf, ec)) {
        errorText = L"複製後找不到驅動 INF：" + resolvedDriverName;
        return false;
    }

    driverInfo.name = resolvedDriverName;
    driverInfo.relativeInfPath = SafeRelativePath(copiedInf, g_backupRoot);
    driverInfo.relativeFolder = SafeRelativePath(targetFolder, g_backupRoot);

    driverCache[cacheKey] = driverInfo;
    return true;
}

bool ExportPrinterSettings(const std::wstring& printerName, const fs::path& settingsPath, std::wstring& errorText) {
    EnsureDirectory(settingsPath.parent_path());

    const fs::path localSettingsPath = NewTempPath(L"ExportSettings", SanitizeFileName(printerName) + L".dat");
    DWORD exitCode = 0;
    RunProcess(L"rundll32.exe",
               {L"printui.dll,PrintUIEntry", L"/Ss", L"/n", printerName, L"/a", localSettingsPath.wstring(), L"m", L"u"},
               exitCode,
               120000,
               g_baseDir);

    std::error_code ec;
    if (!fs::exists(localSettingsPath, ec)) {
        errorText = L"匯出設定檔失敗：" + printerName;
        return false;
    }

    EnsureDirectory(settingsPath.parent_path());
    fs::copy_file(localSettingsPath, settingsPath, fs::copy_options::overwrite_existing, ec);
    if (ec) {
        errorText = L"複製設定檔失敗：" + settingsPath.wstring();
        return false;
    }

    return true;
}

bool ImportPrinterSettings(const std::wstring& printerName, const fs::path& sourceSettingsPath) {
    std::error_code ec;
    if (!fs::exists(sourceSettingsPath, ec)) {
        return true;
    }

    const fs::path staged = NewTempPath(L"ImportSettings", sourceSettingsPath.filename().wstring());
    fs::copy_file(sourceSettingsPath, staged, fs::copy_options::overwrite_existing, ec);
    if (ec) {
        Log(L"Unable to stage settings file: " + sourceSettingsPath.wstring());
        return false;
    }

    DWORD exitCode = 0;
    RunProcess(L"rundll32.exe",
               {L"printui.dll,PrintUIEntry", L"/Sr", L"/n", printerName, L"/a", staged.wstring(), L"m", L"u", L"r", L"p"},
               exitCode,
               120000,
               g_baseDir);
    return exitCode == 0;
}

std::wstring FindInstalledDriverName(const std::wstring& driverName) {
    DWORD needed = 0;
    DWORD returned = 0;
    EnumPrinterDriversW(nullptr, nullptr, 2, nullptr, 0, &needed, &returned);
    if (needed == 0) {
        return L"";
    }

    std::vector<BYTE> buffer(needed);
    if (!EnumPrinterDriversW(nullptr, nullptr, 2, buffer.data(), needed, &needed, &returned)) {
        return L"";
    }

    const auto* drivers = reinterpret_cast<DRIVER_INFO_2W*>(buffer.data());
    for (DWORD i = 0; i < returned; ++i) {
        if (!drivers[i].pName) {
            continue;
        }

        const std::wstring installed = drivers[i].pName;
        if (IEquals(installed, driverName) ||
            StartsWithI(installed, driverName) ||
            StartsWithI(driverName, installed)) {
            return installed;
        }
    }

    return L"";
}

bool DriverExists(const std::wstring& driverName) {
    return !FindInstalledDriverName(driverName).empty();
}

bool WaitForDriver(const std::wstring& driverName, int timeoutSeconds = 45) {
    for (int i = 0; i < timeoutSeconds; ++i) {
        if (DriverExists(driverName)) {
            return true;
        }
        SleepWithMessages(1000);
    }
    return DriverExists(driverName);
}

std::wstring GetPrintUiArchitecture() {
    SYSTEM_INFO info{};
    GetNativeSystemInfo(&info);
    switch (info.wProcessorArchitecture) {
    case PROCESSOR_ARCHITECTURE_AMD64:
        return L"x64";
    case PROCESSOR_ARCHITECTURE_INTEL:
        return L"x86";
    case PROCESSOR_ARCHITECTURE_ARM64:
        return L"ARM64";
    default:
        return L"x64";
    }
}

bool AttemptDriverInstallWithInf(const fs::path& infPath, const std::wstring& driverName) {
    DWORD exitCode = 0;

    RunProcess(L"pnputil.exe", {L"/add-driver", infPath.wstring(), L"/install"}, exitCode, 120000, g_baseDir);
    if (WaitForDriver(driverName, 8)) {
        return true;
    }

    RunProcess(L"rundll32.exe",
               {L"printui.dll,PrintUIEntry", L"/ia", L"/m", driverName, L"/f", infPath.wstring(), L"/q"},
               exitCode,
               120000,
               g_baseDir);
    if (WaitForDriver(driverName, 15)) {
        return true;
    }

    RunProcess(L"rundll32.exe",
               {L"printui.dll,PrintUIEntry", L"/ia", L"/m", driverName, L"/f", infPath.wstring(),
                L"/h", GetPrintUiArchitecture(), L"/v", L"Type 3 - User Mode", L"/q"},
               exitCode,
               120000,
               g_baseDir);

    return WaitForDriver(driverName, 15);
}

fs::path FindPrintingAdminScript(const std::wstring& scriptName) {
    const std::wstring windir = GetEnvVar(L"WINDIR");
    const fs::path base = fs::path(windir.empty() ? L"C:\\Windows" : windir) / L"System32" / L"Printing_Admin_Scripts";
    std::error_code ec;
    if (!fs::exists(base, ec)) {
        return {};
    }

    wchar_t localeName[LOCALE_NAME_MAX_LENGTH] = {};
    std::vector<std::wstring> preferred;
    if (GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH) > 0) {
        preferred.emplace_back(localeName);
    }
    preferred.emplace_back(L"en-US");

    for (const auto& folder : preferred) {
        const fs::path candidate = base / folder / scriptName;
        if (fs::exists(candidate, ec)) {
            return candidate;
        }
    }

    for (const auto& entry : fs::directory_iterator(base, ec)) {
        if (ec || !entry.is_directory()) {
            continue;
        }
        const fs::path candidate = entry.path() / scriptName;
        if (fs::exists(candidate, ec)) {
            return candidate;
        }
    }

    const fs::path direct = base / scriptName;
    if (fs::exists(direct, ec)) {
        return direct;
    }
    return {};
}

bool EnsureLocalPort(const std::wstring& portName) {
    if (portName.empty()) {
        return false;
    }
    if (PortExists(portName)) {
        return true;
    }

    DWORD exitCode = 0;
    const std::wstring script =
        L"$ErrorActionPreference='Stop'; Add-PrinterPort -Name '" + EscapeForPowerShellSingleQuoted(portName) + L"'";
    RunPowerShell(script, exitCode, 120000);
    return PortExists(portName);
}

bool EnsureTcpPort(const PortInfo& port) {
    if (port.name.empty()) {
        return false;
    }
    if (PortExists(port.name)) {
        return true;
    }
    if (port.printerHostAddress.empty()) {
        return false;
    }

    std::wstring script =
        L"$ErrorActionPreference='Stop'; Add-PrinterPort -Name '" + EscapeForPowerShellSingleQuoted(port.name) +
        L"' -PrinterHostAddress '" + EscapeForPowerShellSingleQuoted(port.printerHostAddress) + L"'";
    if (port.portNumber > 0) {
        script += L" -PortNumber " + std::to_wstring(port.portNumber);
    }
    if (port.snmpEnabled && port.snmpIndex > 0) {
        script += L" -SNMP " + std::to_wstring(port.snmpIndex);
    }
    if (!port.snmpCommunity.empty()) {
        script += L" -SNMPCommunity '" + EscapeForPowerShellSingleQuoted(port.snmpCommunity) + L"'";
    }

    DWORD exitCode = 0;
    RunPowerShell(script, exitCode, 120000);
    if (PortExists(port.name)) {
        return true;
    }

    const fs::path prnPort = FindPrintingAdminScript(L"prnport.vbs");
    if (!prnPort.empty()) {
        std::vector<std::wstring> args = {
            L"//nologo",
            prnPort.wstring(),
            L"-a",
            L"-r",
            port.name,
            L"-h",
            port.printerHostAddress,
            L"-o",
            L"raw",
            L"-n",
            port.portNumber > 0 ? std::to_wstring(port.portNumber) : L"9100"
        };
        if (port.snmpEnabled) {
            args.push_back(L"-me");
            if (port.snmpIndex > 0) {
                args.push_back(L"-i");
                args.push_back(std::to_wstring(port.snmpIndex));
            }
            if (!port.snmpCommunity.empty()) {
                args.push_back(L"-y");
                args.push_back(port.snmpCommunity);
            }
        } else {
            args.push_back(L"-md");
        }
        RunProcess(L"cscript.exe", args, exitCode, 120000, g_baseDir);
    }

    return PortExists(port.name);
}

bool EnsureLprPort(const PortInfo& port) {
    if (port.name.empty() || port.printerHostAddress.empty() || port.queueName.empty()) {
        return false;
    }
    if (PortExists(port.name)) {
        return true;
    }

    std::wstring script =
        L"$ErrorActionPreference='Stop'; Add-PrinterPort -Name '" + EscapeForPowerShellSingleQuoted(port.name) +
        L"' -LprHostAddress '" + EscapeForPowerShellSingleQuoted(port.printerHostAddress) +
        L"' -LprQueueName '" + EscapeForPowerShellSingleQuoted(port.queueName) + L"'";
    if (port.snmpEnabled && port.snmpIndex > 0) {
        script += L" -SNMP " + std::to_wstring(port.snmpIndex);
    }
    if (!port.snmpCommunity.empty()) {
        script += L" -SNMPCommunity '" + EscapeForPowerShellSingleQuoted(port.snmpCommunity) + L"'";
    }
    if (port.lprByteCounting) {
        script += L" -LprByteCounting";
    }

    DWORD exitCode = 0;
    RunPowerShell(script, exitCode, 120000);
    if (PortExists(port.name)) {
        return true;
    }

    const fs::path prnPort = FindPrintingAdminScript(L"prnport.vbs");
    if (!prnPort.empty()) {
        std::vector<std::wstring> args = {
            L"//nologo",
            prnPort.wstring(),
            L"-a",
            L"-r",
            port.name,
            L"-h",
            port.printerHostAddress,
            L"-o",
            L"lpr",
            L"-q",
            port.queueName
        };
        if (port.snmpEnabled) {
            args.push_back(L"-me");
            if (port.snmpIndex > 0) {
                args.push_back(L"-i");
                args.push_back(std::to_wstring(port.snmpIndex));
            }
            if (!port.snmpCommunity.empty()) {
                args.push_back(L"-y");
                args.push_back(port.snmpCommunity);
            }
        } else {
            args.push_back(L"-md");
        }
        args.push_back(port.lprByteCounting ? L"-2e" : L"-2d");
        RunProcess(L"cscript.exe", args, exitCode, 120000, g_baseDir);
    }

    return PortExists(port.name);
}

bool AddPrinterQueue(const std::wstring& printerName,
                     const std::wstring& driverName,
                     const std::wstring& portName,
                     bool shared,
                     const std::wstring& shareName,
                     DWORD& lastError) {
    PRINTER_INFO_2W info{};
    info.pPrinterName = const_cast<LPWSTR>(printerName.c_str());
    info.pPortName = const_cast<LPWSTR>(portName.c_str());
    info.pDriverName = const_cast<LPWSTR>(driverName.c_str());
    info.pPrintProcessor = const_cast<LPWSTR>(L"WinPrint");
    info.pDatatype = const_cast<LPWSTR>(L"RAW");
    info.Attributes = PRINTER_ATTRIBUTE_LOCAL;

    if (shared) {
        info.Attributes |= PRINTER_ATTRIBUTE_SHARED;
        const std::wstring& effectiveShareName = shareName.empty() ? printerName : shareName;
        info.pShareName = const_cast<LPWSTR>(effectiveShareName.c_str());
    }

    HANDLE printer = AddPrinterW(nullptr, 2, reinterpret_cast<LPBYTE>(&info));
    if (!printer) {
        lastError = GetLastError();
        Log(L"AddPrinter failed: " + FormatWin32Error(lastError));
        return false;
    }

    ClosePrinter(printer);
    lastError = ERROR_SUCCESS;
    return true;
}

bool TryCreatePrinterQueueWithPrnMngr(const std::wstring& printerName,
                                      const std::wstring& driverName,
                                      const std::wstring& portName) {
    const fs::path prnMngr = FindPrintingAdminScript(L"prnmngr.vbs");
    if (prnMngr.empty()) {
        return false;
    }

    DWORD exitCode = 0;
    RunProcess(L"cscript.exe",
               {L"//nologo", prnMngr.wstring(), L"-a", L"-p", printerName, L"-m", driverName, L"-r", portName},
               exitCode,
               120000,
               g_baseDir);
    if (exitCode == 0) {
        SleepWithMessages(800);
    }
    return WaitForPrinterState(printerName, true, 10);
}

bool TryCreatePrinterQueueWithPrintUi(const fs::path& infPath,
                                      const std::wstring& printerName,
                                      const std::wstring& portName,
                                      const std::wstring& driverName) {
    if (Trim(driverName).empty() || infPath.empty()) {
        return false;
    }

    DWORD exitCode = 0;
    RunProcess(L"rundll32.exe",
               {L"printui.dll,PrintUIEntry", L"/if", L"/b", printerName, L"/f", infPath.wstring(),
                L"/r", portName, L"/m", driverName, L"/z", L"/q"},
               exitCode,
               120000,
               g_baseDir);
    if (exitCode == 0) {
        SleepWithMessages(1200);
    }
    return WaitForPrinterState(printerName, true, 12);
}

bool ApplyPrinterProperties(const PrinterEntry& entry, const std::wstring& targetPrinterName) {
    HANDLE printer = nullptr;
    PRINTER_DEFAULTSW defaults{};
    defaults.DesiredAccess = PRINTER_ACCESS_ADMINISTER;
    if (!OpenPrinterW(const_cast<LPWSTR>(targetPrinterName.c_str()), &printer, &defaults)) {
        Log(L"OpenPrinter for SetPrinter failed: " + FormatWin32Error(GetLastError()));
        return false;
    }

    DWORD needed = 0;
    GetPrinterW(printer, 2, nullptr, 0, &needed);
    if (needed == 0) {
        ClosePrinter(printer);
        return false;
    }

    std::vector<BYTE> buffer(needed);
    if (!GetPrinterW(printer, 2, buffer.data(), needed, &needed)) {
        Log(L"GetPrinter(2) failed: " + FormatWin32Error(GetLastError()));
        ClosePrinter(printer);
        return false;
    }

    auto* info = reinterpret_cast<PRINTER_INFO_2W*>(buffer.data());
    std::wstring shareName = entry.shareName.empty() ? entry.name : entry.shareName;
    std::vector<wchar_t> shareBuffer(shareName.begin(), shareName.end());
    shareBuffer.push_back(L'\0');
    std::vector<wchar_t> commentBuffer(entry.comment.begin(), entry.comment.end());
    commentBuffer.push_back(L'\0');
    std::vector<wchar_t> locationBuffer(entry.location.begin(), entry.location.end());
    locationBuffer.push_back(L'\0');
    std::vector<wchar_t> datatypeBuffer(entry.datatype.begin(), entry.datatype.end());
    datatypeBuffer.push_back(L'\0');
    std::vector<wchar_t> printProcessorBuffer(entry.printProcessor.begin(), entry.printProcessor.end());
    printProcessorBuffer.push_back(L'\0');

    if (entry.shared) {
        info->Attributes |= PRINTER_ATTRIBUTE_SHARED;
        info->pShareName = shareBuffer.data();
    } else {
        info->Attributes &= ~PRINTER_ATTRIBUTE_SHARED;
    }

    if (entry.published) {
        info->Attributes |= PRINTER_ATTRIBUTE_PUBLISHED;
    } else {
        info->Attributes &= ~PRINTER_ATTRIBUTE_PUBLISHED;
    }

    if (entry.keepPrintedJobs) {
        info->Attributes |= PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS;
    } else {
        info->Attributes &= ~PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS;
    }

    if (!entry.comment.empty()) {
        info->pComment = commentBuffer.data();
    }
    if (!entry.location.empty()) {
        info->pLocation = locationBuffer.data();
    }
    if (!entry.datatype.empty()) {
        info->pDatatype = datatypeBuffer.data();
    }
    if (!entry.printProcessor.empty()) {
        info->pPrintProcessor = printProcessorBuffer.data();
    }

    const BOOL ok = SetPrinterW(printer, 2, reinterpret_cast<LPBYTE>(info), 0);
    const DWORD errorCode = ok ? ERROR_SUCCESS : GetLastError();
    ClosePrinter(printer);

    if (!ok) {
        Log(L"SetPrinter failed for " + targetPrinterName + L": " + FormatWin32Error(errorCode));
    }

    return ok == TRUE;
}

bool SetPrinterAsDefault(const std::wstring& printerName) {
    if (!SetDefaultPrinterW(printerName.c_str())) {
        Log(L"SetDefaultPrinter failed: " + FormatWin32Error(GetLastError()));
        return false;
    }
    return true;
}

void OpenPrintersFolder() {
    HINSTANCE instance = ShellExecuteW(
        nullptr,
        L"open",
        L"explorer.exe",
        L"shell:::{A8A91A66-3A7D-4424-8D24-04E180695C7A}",
        nullptr,
        SW_SHOWNORMAL);
    if (reinterpret_cast<INT_PTR>(instance) <= 32) {
        ShellExecuteW(nullptr, L"open", L"control.exe", L"/name Microsoft.DevicesAndPrinters", nullptr, SW_SHOWNORMAL);
    }
}

bool WriteBackupManifest(const std::wstring& defaultPrinterName, int printerCount) {
    std::wstring text;
    AppendKeyNumber(text, L"version", 1);
    AppendKeyText(text, L"backup_time", GetTimestamp());
    AppendKeyText(text, L"computer_name", GetEnvVar(L"COMPUTERNAME"));
    AppendKeyText(text, L"default_printer", defaultPrinterName);
    AppendKeyNumber(text, L"printer_count", static_cast<std::uint32_t>(printerCount));
    return WriteUtf8File(g_backupRoot / L"backup.ini", text);
}

bool WritePrinterEntryFile(const fs::path& path, const PrinterEntry& entry) {
    std::wstring text;
    AppendKeyNumber(text, L"version", 1);
    AppendKeyText(text, L"name", entry.name);
    AppendKeyText(text, L"driver_name", entry.driverName);
    AppendKeyText(text, L"port_name", entry.portName);
    AppendKeyBool(text, L"shared", entry.shared);
    AppendKeyText(text, L"share_name", entry.shareName);
    AppendKeyText(text, L"comment", entry.comment);
    AppendKeyText(text, L"location", entry.location);
    AppendKeyText(text, L"datatype", entry.datatype);
    AppendKeyText(text, L"print_processor", entry.printProcessor);
    AppendKeyBool(text, L"published", entry.published);
    AppendKeyBool(text, L"keep_printed_jobs", entry.keepPrintedJobs);
    AppendKeyBool(text, L"is_default", entry.isDefault);
    AppendKeyBool(text, L"is_connection_printer", entry.isConnectionPrinter);
    AppendKeyText(text, L"connection_name", entry.connectionName);
    AppendKeyText(text, L"usb_pnp_key", entry.usbPnpKey);
    AppendKeyText(text, L"driver_package_name", entry.driver.name);
    AppendKeyText(text, L"driver_relative_inf", entry.driver.relativeInfPath);
    AppendKeyText(text, L"driver_relative_folder", entry.driver.relativeFolder);
    AppendKeyText(text, L"port_kind", entry.port.kind);
    AppendKeyText(text, L"port_real_name", entry.port.name);
    AppendKeyText(text, L"port_host", entry.port.printerHostAddress);
    AppendKeyText(text, L"port_queue", entry.port.queueName);
    AppendKeyNumber(text, L"port_number", entry.port.portNumber);
    AppendKeyBool(text, L"snmp_enabled", entry.port.snmpEnabled);
    AppendKeyNumber(text, L"snmp_index", entry.port.snmpIndex);
    AppendKeyText(text, L"snmp_community", entry.port.snmpCommunity);
    AppendKeyText(text, L"protocol", entry.port.protocol);
    AppendKeyBool(text, L"lpr_byte_counting", entry.port.lprByteCounting);
    AppendKeyText(text, L"port_monitor", entry.port.portMonitor);

    return WriteUtf8File(path, text);
}

bool ReadPrinterEntryFile(const fs::path& path, PrinterEntry& entry) {
    const auto values = ReadKeyValueFile(path);
    if (values.empty()) {
        return false;
    }

    entry = {};
    entry.name = GetTextValue(values, L"name");
    entry.driverName = GetTextValue(values, L"driver_name");
    entry.portName = GetTextValue(values, L"port_name");
    entry.shared = GetBoolValue(values, L"shared");
    entry.shareName = GetTextValue(values, L"share_name");
    entry.comment = GetTextValue(values, L"comment");
    entry.location = GetTextValue(values, L"location");
    entry.datatype = GetTextValue(values, L"datatype");
    entry.printProcessor = GetTextValue(values, L"print_processor");
    entry.published = GetBoolValue(values, L"published");
    entry.keepPrintedJobs = GetBoolValue(values, L"keep_printed_jobs");
    entry.isDefault = GetBoolValue(values, L"is_default");
    entry.isConnectionPrinter = GetBoolValue(values, L"is_connection_printer");
    entry.connectionName = GetTextValue(values, L"connection_name");
    entry.usbPnpKey = GetTextValue(values, L"usb_pnp_key");
    entry.driver.name = GetTextValue(values, L"driver_package_name");
    entry.driver.relativeInfPath = GetTextValue(values, L"driver_relative_inf");
    entry.driver.relativeFolder = GetTextValue(values, L"driver_relative_folder");
    entry.port.kind = GetTextValue(values, L"port_kind");
    entry.port.name = GetTextValue(values, L"port_real_name");
    entry.port.printerHostAddress = GetTextValue(values, L"port_host");
    entry.port.queueName = GetTextValue(values, L"port_queue");
    entry.port.portNumber = GetNumberValue(values, L"port_number", 0);
    entry.port.snmpEnabled = GetBoolValue(values, L"snmp_enabled");
    entry.port.snmpIndex = GetNumberValue(values, L"snmp_index", 0);
    entry.port.snmpCommunity = GetTextValue(values, L"snmp_community");
    entry.port.protocol = GetTextValue(values, L"protocol");
    entry.port.lprByteCounting = GetBoolValue(values, L"lpr_byte_counting");
    entry.port.portMonitor = GetTextValue(values, L"port_monitor");

    if (entry.port.name.empty()) {
        entry.port.name = entry.portName;
    }

    return !entry.name.empty();
}

fs::path GetSettingsPathForPrinterFolder(const fs::path& printerFolder) {
    return printerFolder / L"settings.dat";
}

bool ResetBackupRoot() {
    if (ToLowerCopy(g_backupRoot.filename().wstring()) != L"printerbackup") {
        return false;
    }

    if (!RemoveDirectoryTree(g_backupRoot)) {
        return false;
    }

    return EnsureDirectory(g_driversRoot) && EnsureDirectory(g_printersRoot);
}

bool StageDirectoryToTemp(const fs::path& source, const std::wstring& category, fs::path& stagedPath) {
    stagedPath = NewTempPath(category, source.filename().wstring() + L"_" + std::to_wstring(GetTickCount64()));
    return CopyDirectoryTree(source, stagedPath);
}

std::vector<fs::path> EnumerateInfFilesRecursively(const fs::path& root) {
    std::vector<fs::path> infFiles;
    std::error_code ec;
    if (!fs::exists(root, ec)) {
        return infFiles;
    }

    for (const auto& entry : fs::recursive_directory_iterator(root, fs::directory_options::skip_permission_denied, ec)) {
        if (ec) {
            break;
        }
        if (entry.is_regular_file() && IEquals(entry.path().extension().wstring(), L".inf")) {
            infFiles.push_back(entry.path());
        }
    }
    return infFiles;
}

bool PrepareDriverMaterial(const PrinterEntry& entry,
                           std::map<std::wstring, DriverRestoreMaterial>& cache,
                           DriverRestoreMaterial& material,
                           std::wstring& errorText) {
    const std::wstring key = ToLowerCopy(entry.driver.name.empty() ? entry.driverName : entry.driver.name);
    auto cached = cache.find(key);
    if (cached != cache.end() && cached->second.prepared) {
        material = cached->second;
        return true;
    }

    fs::path sourceFolder = g_backupRoot / entry.driver.relativeFolder;
    std::error_code ec;
    if (!fs::exists(sourceFolder, ec)) {
        errorText = L"找不到備份驅動資料夾：" + sourceFolder.wstring();
        return false;
    }

    DriverRestoreMaterial prepared;
    prepared.requestedName = entry.driver.name.empty() ? entry.driverName : entry.driver.name;
    if (!StageDirectoryToTemp(sourceFolder, L"DriverInstall", prepared.stagedFolder)) {
        errorText = L"暫存驅動資料夾失敗：" + sourceFolder.wstring();
        return false;
    }

    prepared.infFiles = EnumerateInfFilesRecursively(prepared.stagedFolder);
    if (prepared.infFiles.empty()) {
        errorText = L"在驅動備份資料夾中找不到 INF：" + sourceFolder.wstring();
        return false;
    }

    prepared.prepared = true;
    cache[key] = prepared;
    material = prepared;
    return true;
}

bool EnsureDriverInstalled(const PrinterEntry& entry,
                           DriverRestoreMaterial& material,
                           std::wstring& resolvedDriverName,
                           std::wstring& errorText) {
    const std::wstring requestedName = material.requestedName.empty() ? entry.driverName : material.requestedName;
    resolvedDriverName = FindInstalledDriverName(requestedName);
    if (!resolvedDriverName.empty()) {
        material.resolvedName = resolvedDriverName;
        return true;
    }

    for (const auto& infPath : material.infFiles) {
        Log(L"Trying driver INF: " + infPath.wstring());
        if (AttemptDriverInstallWithInf(infPath, requestedName)) {
            resolvedDriverName = FindInstalledDriverName(requestedName);
            if (resolvedDriverName.empty()) {
                resolvedDriverName = requestedName;
            }
            material.resolvedName = resolvedDriverName;
            return true;
        }
    }

    resolvedDriverName = FindInstalledDriverName(requestedName);
    if (!resolvedDriverName.empty()) {
        material.resolvedName = resolvedDriverName;
        return true;
    }

    errorText = L"驅動安裝失敗：" + requestedName;
    return false;
}

struct UsbPrinterSnapshot {
    std::wstring name;
    std::wstring driverName;
    std::wstring portName;
    std::wstring pnpKey;
};

std::vector<UsbPrinterSnapshot> GetCurrentUsbPrinters() {
    std::vector<UsbPrinterSnapshot> usbPrinters;
    for (const auto& printer : EnumeratePrinters()) {
        if (!StartsWithI(printer.portName, L"USB")) {
            continue;
        }

        UsbPrinterSnapshot snapshot;
        snapshot.name = printer.name;
        snapshot.driverName = printer.driverName;
        snapshot.portName = printer.portName;
        snapshot.pnpKey = NormalizeUsbPnpDeviceId(QueryPrinterPnpId(printer.name));
        usbPrinters.push_back(snapshot);
    }
    return usbPrinters;
}

bool ResolveUsbPortName(const PrinterEntry& entry, std::wstring& resolvedPortName, std::wstring& errorText) {
    const auto usbPorts = GetUsbPortNames();
    if (usbPorts.empty()) {
        errorText = UiText(
            L"No available USB printer ports were found on this PC.",
            L"本機找不到可用的 USB 印表機連接埠。");
        return false;
    }

    const auto currentUsbPrinters = GetCurrentUsbPrinters();
    if (!entry.usbPnpKey.empty()) {
        for (const auto& printer : currentUsbPrinters) {
            if (!printer.pnpKey.empty() && IEquals(printer.pnpKey, entry.usbPnpKey)) {
                resolvedPortName = printer.portName;
                return true;
            }
        }
    }

    for (const auto& printer : currentUsbPrinters) {
        if (IEquals(printer.name, entry.name)) {
            resolvedPortName = printer.portName;
            return true;
        }
    }

    for (const auto& printer : currentUsbPrinters) {
        if (IEquals(printer.driverName, entry.driverName)) {
            resolvedPortName = printer.portName;
            return true;
        }
    }

    std::set<std::wstring> usedPorts;
    for (const auto& printer : currentUsbPrinters) {
        usedPorts.insert(ToLowerCopy(printer.portName));
    }

    std::vector<std::wstring> freePorts;
    for (const auto& port : usbPorts) {
        if (usedPorts.find(ToLowerCopy(port)) == usedPorts.end()) {
            freePorts.push_back(port);
        }
    }
    if (freePorts.size() == 1) {
        resolvedPortName = freePorts.front();
        return true;
    }

    if (usbPorts.size() == 1) {
        resolvedPortName = usbPorts.front();
        return true;
    }

    if (!entry.port.name.empty() && PortExists(entry.port.name) && usedPorts.find(ToLowerCopy(entry.port.name)) == usedPorts.end()) {
        resolvedPortName = entry.port.name;
        return true;
    }

    if (ShowSelectionDialog(
            UiText(L"Choose USB Port", L"選擇 USB 連接埠"),
            UiText(
                L"Multiple USB printer ports were found. Please choose the port to use.",
                L"偵測到多個 USB 印表機連接埠，請選擇要使用的連接埠。"),
            usbPorts,
            resolvedPortName)) {
        return true;
    }

    errorText = UiText(
        L"USB port selection was cancelled.",
        L"已取消 USB 連接埠選擇。");
    return false;
}

bool ResolveExistingPortSelection(const std::wstring& prompt, std::wstring& portName, std::wstring& errorText) {
    const auto portNames = EnumeratePortNames();
    if (portNames.empty()) {
        errorText = UiText(
            L"This PC does not have any available printer ports.",
            L"本機沒有可用的印表機連接埠。");
        return false;
    }

    if (ShowSelectionDialog(UiText(L"Choose Port", L"選擇連接埠"), prompt, portNames, portName)) {
        return true;
    }

    errorText = UiText(
        L"Port selection was cancelled.",
        L"已取消連接埠選擇。");
    return false;
}

bool EnsurePrinterPort(const PrinterEntry& entry,
                       std::wstring& resolvedPortName,
                       std::wstring& errorText) {
    const PortInfo& port = entry.port;
    resolvedPortName = port.name.empty() ? entry.portName : port.name;

    if (port.kind == L"USB") {
        return ResolveUsbPortName(entry, resolvedPortName, errorText);
    }

    if (port.kind == L"TCP") {
        if (EnsureTcpPort(port)) {
            resolvedPortName = port.name;
            return true;
        }
        errorText = L"建立 TCP/IP 連接埠失敗：" + port.name;
        return false;
    }

    if (port.kind == L"LPR") {
        if (EnsureLprPort(port)) {
            resolvedPortName = port.name;
            return true;
        }
        errorText = L"建立 LPR 連接埠失敗：" + port.name;
        return false;
    }

    if (port.kind == L"WSD") {
        if (PortExists(resolvedPortName)) {
            return true;
        }
        return ResolveExistingPortSelection(L"找不到原始的 WSD 連接埠，請選擇本機現有連接埠。", resolvedPortName, errorText);
    }

    if (PortExists(resolvedPortName)) {
        return true;
    }

    if (port.kind == L"Local" || port.kind == L"Connection") {
        if (EnsureLocalPort(resolvedPortName)) {
            return true;
        }
        errorText = L"建立 Local Port 失敗：" + resolvedPortName;
        return false;
    }

    if (!port.printerHostAddress.empty()) {
        if (EnsureTcpPort(port)) {
            resolvedPortName = port.name;
            return true;
        }
        errorText = L"建立連接埠失敗：" + port.name;
        return false;
    }

    return ResolveExistingPortSelection(L"找不到原始連接埠，請選擇要使用的連接埠。", resolvedPortName, errorText);
}

bool EnsurePrinterCreated(const PrinterEntry& entry,
                          DriverRestoreMaterial& material,
                          const std::wstring& resolvedDriverName,
                          const std::wstring& portName,
                          std::wstring& errorText) {
    if (PrinterExists(entry.name)) {
        DeletePrinterByName(entry.name);
        WaitForPrinterState(entry.name, false, 20);
    }

    for (int attempt = 0; attempt < 3; ++attempt) {
        DWORD addPrinterError = ERROR_SUCCESS;
        if (AddPrinterQueue(entry.name, resolvedDriverName, portName, entry.shared, entry.shareName, addPrinterError)) {
            return WaitForPrinterState(entry.name, true, 10);
        }

        if (addPrinterError == ERROR_PRINTER_ALREADY_EXISTS) {
            DeletePrinterByName(entry.name);
            WaitForPrinterState(entry.name, false, 10);
        } else if (addPrinterError == ERROR_UNKNOWN_PORT) {
            PortInfo retryPort = entry.port;
            retryPort.name = portName;
            if (retryPort.kind == L"TCP") {
                EnsureTcpPort(retryPort);
            } else if (retryPort.kind == L"LPR") {
                EnsureLprPort(retryPort);
            } else {
                EnsureLocalPort(portName);
            }
        } else if (addPrinterError == ERROR_UNKNOWN_PRINTER_DRIVER) {
            std::wstring ignoredError;
            std::wstring ignoredDriver;
            EnsureDriverInstalled(entry, material, ignoredDriver, ignoredError);
        }

        SleepWithMessages(1500);
    }

    if (TryCreatePrinterQueueWithPrnMngr(entry.name, resolvedDriverName, portName)) {
        return true;
    }

    for (const auto& infPath : material.infFiles) {
        if (TryCreatePrinterQueueWithPrintUi(infPath, entry.name, portName, resolvedDriverName)) {
            return true;
        }
        if (!IEquals(resolvedDriverName, entry.driverName) &&
            TryCreatePrinterQueueWithPrintUi(infPath, entry.name, portName, entry.driverName)) {
            return true;
        }
    }

    errorText = L"建立印表機失敗：" + entry.name;
    return PrinterExists(entry.name);
}

std::wstring FindInstalledConnectionPrinterName(const PrinterEntry& entry) {
    std::wstring connectionServer;
    std::wstring connectionShare;
    SplitConnectionPath(entry.connectionName, connectionServer, connectionShare);

    for (const auto& printer : EnumeratePrinters()) {
        if (IEquals(printer.name, entry.name)) {
            return printer.name;
        }
        if (!connectionShare.empty()) {
            if (IEquals(printer.name, connectionShare) || StartsWithI(printer.name, connectionShare)) {
                return printer.name;
            }
        }
        if (!connectionServer.empty() && ContainsI(printer.name, connectionServer) && !connectionShare.empty() && ContainsI(printer.name, connectionShare)) {
            return printer.name;
        }
    }

    return L"";
}

bool InstallSharedPrinterConnection(const PrinterEntry& entry, std::wstring& installedPrinterName) {
    if (entry.connectionName.empty()) {
        return false;
    }

    DWORD exitCode = 0;
    const std::wstring script =
        L"$ErrorActionPreference='Stop'; Add-Printer -ConnectionName '" + EscapeForPowerShellSingleQuoted(entry.connectionName) + L"'";
    RunPowerShell(script, exitCode, 120000);

    if (exitCode != 0) {
        RunProcess(L"rundll32.exe",
                   {L"printui.dll,PrintUIEntry", L"/in", L"/n", entry.connectionName, L"/q"},
                   exitCode,
                   120000,
                   g_baseDir);
    }

    for (int attempt = 0; attempt < 40; ++attempt) {
        SleepWithMessages(750);
        installedPrinterName = FindInstalledConnectionPrinterName(entry);
        if (!installedPrinterName.empty()) {
            return true;
        }
    }

    return false;
}

bool RestoreConnectionPrinter(const PrinterEntry& entry,
                              DriverRestoreMaterial& material,
                              std::wstring& actualPrinterName,
                              std::wstring& errorText) {
    std::wstring resolvedDriverName;
    if (!EnsureDriverInstalled(entry, material, resolvedDriverName, errorText)) {
        return false;
    }

    if (InstallSharedPrinterConnection(entry, actualPrinterName)) {
        ApplyPrinterProperties(entry, actualPrinterName);
        return true;
    }

    PortInfo localPort = entry.port;
    localPort.kind = L"Local";
    localPort.name = entry.connectionName;
    PrinterEntry fallbackLocal = entry;
    fallbackLocal.port = localPort;

    std::wstring resolvedPort;
    if (EnsurePrinterPort(fallbackLocal, resolvedPort, errorText) &&
        EnsurePrinterCreated(fallbackLocal, material, resolvedDriverName, resolvedPort, errorText)) {
        ApplyPrinterProperties(fallbackLocal, fallbackLocal.name);
        actualPrinterName = fallbackLocal.name;
        return true;
    }

    std::wstring serverName;
    std::wstring shareName;
    if (!SplitConnectionPath(entry.connectionName, serverName, shareName)) {
        errorText = L"共享印表機還原失敗：" + entry.connectionName;
        return false;
    }

    PortInfo lprPort;
    lprPort.kind = L"LPR";
    lprPort.name = serverName + L":" + shareName;
    lprPort.printerHostAddress = serverName;
    lprPort.queueName = shareName;
    lprPort.lprByteCounting = false;

    PrinterEntry fallbackLpr = entry;
    fallbackLpr.port = lprPort;

    if (EnsurePrinterPort(fallbackLpr, resolvedPort, errorText) &&
        EnsurePrinterCreated(fallbackLpr, material, resolvedDriverName, resolvedPort, errorText)) {
        ApplyPrinterProperties(fallbackLpr, fallbackLpr.name);
        actualPrinterName = fallbackLpr.name;
        return true;
    }

    errorText = L"共享印表機還原失敗：" + entry.connectionName;
    return false;
}

bool LoadBackupEntries(std::vector<std::pair<fs::path, PrinterEntry>>& entries, std::wstring& defaultPrinterName) {
    entries.clear();
    defaultPrinterName.clear();

    const fs::path manifestPath = g_backupRoot / L"backup.ini";
    std::error_code ec;
    if (!fs::exists(manifestPath, ec)) {
        return false;
    }

    const auto manifestValues = ReadKeyValueFile(manifestPath);
    defaultPrinterName = GetTextValue(manifestValues, L"default_printer");

    std::vector<fs::path> infoFiles;
    if (!fs::exists(g_printersRoot, ec)) {
        return false;
    }

    for (const auto& directory : fs::directory_iterator(g_printersRoot, ec)) {
        if (ec || !directory.is_directory()) {
            continue;
        }

        const fs::path infoPath = directory.path() / L"info.ini";
        if (fs::exists(infoPath, ec)) {
            infoFiles.push_back(infoPath);
        }
    }

    std::sort(infoFiles.begin(), infoFiles.end());
    for (const auto& infoPath : infoFiles) {
        PrinterEntry entry;
        if (ReadPrinterEntryFile(infoPath, entry)) {
            entries.push_back({infoPath.parent_path(), entry});
        }
    }

    return !entries.empty();
}

template <typename T>
std::vector<T> SelectItemsByIndices(const std::vector<T>& source, const std::vector<int>& selectedIndices) {
    std::vector<T> selected;
    selected.reserve(selectedIndices.size());
    for (int index : selectedIndices) {
        if (index >= 0 && index < static_cast<int>(source.size())) {
            selected.push_back(source[static_cast<std::size_t>(index)]);
        }
    }
    return selected;
}

std::vector<CheckListItem> BuildCheckListItems(const std::vector<NativePrinterRecord>& printers) {
    std::vector<CheckListItem> items;
    items.reserve(printers.size());
    for (const auto& printer : printers) {
        CheckListItem item;
        item.name = printer.name;
        item.detail = printer.driverName;
        item.port = printer.portName;
        item.checked = true;
        items.push_back(item);
    }
    return items;
}

std::vector<CheckListItem> BuildCheckListItems(const std::vector<std::pair<fs::path, PrinterEntry>>& entries) {
    std::vector<CheckListItem> items;
    items.reserve(entries.size());
    for (const auto& entryPair : entries) {
        const PrinterEntry& entry = entryPair.second;
        CheckListItem item;
        item.name = entry.name;
        item.detail = entry.driverName;
        if (entry.isConnectionPrinter && !entry.connectionName.empty()) {
            item.port = entry.connectionName;
        } else if (!entry.port.name.empty()) {
            item.port = entry.port.name;
        } else {
            item.port = entry.portName;
        }
        item.checked = true;
        items.push_back(item);
    }
    return items;
}

bool BackupLocalPrinters() {
    std::vector<NativePrinterRecord> targetPrinters;
    std::vector<NativePrinterRecord> skippedPrinters;
    for (const auto& printer : EnumeratePrinters()) {
        if (IsExcludedPrinter(printer)) {
            skippedPrinters.push_back(printer);
        } else {
            targetPrinters.push_back(printer);
        }
    }

    if (targetPrinters.empty()) {
        ShowMessage(
            L"PrtEasyBAK",
            UiText(
                L"No eligible local printers were found on this PC.",
                L"這台電腦找不到可備份的本機印表機。"),
            MB_OK | MB_ICONINFORMATION);
        return true;
    }

    std::vector<int> selectedIndices;
    std::wstring backupDescription = UiText(
        L"Choose the local printers you want to back up. Virtual printers are skipped automatically and do not appear in the list.",
        L"請勾選要備份的本機印表機。虛擬印表機會自動略過，不會出現在清單中。");
    if (!skippedPrinters.empty()) {
        backupDescription += L"\r\n" +
            std::wstring(UiText(L"Skipped printers: ", L"已略過虛擬印表機：")) +
            std::to_wstring(skippedPrinters.size()) +
            UiText(L" item(s).", L" 台。");
    }

    if (!ShowCheckListDialog(
            UiText(L"Choose Printers to Back Up", L"選擇要備份的印表機"),
            UiText(L"Choose the local printers to back up:", L"可勾選要備份的本機印表機："),
            backupDescription,
            BuildCheckListItems(targetPrinters),
            selectedIndices)) {
        return true;
    }

    targetPrinters = SelectItemsByIndices(targetPrinters, selectedIndices);
    if (targetPrinters.empty()) {
        return true;
    }

    std::vector<std::wstring> previewLines;
    previewLines.push_back(
        std::wstring(UiText(L"The following printers will be backed up: ", L"以下印表機將會備份：")) +
        std::to_wstring(targetPrinters.size()) +
        UiText(L" item(s)", L" 台"));
    previewLines.push_back(L"");
    for (const auto& printer : targetPrinters) {
        previewLines.push_back(L"- " + printer.name);
    }
    previewLines.push_back(L"");
    previewLines.push_back(UiText(
        L"The PrinterBackup folder will be recreated before backup starts.",
        L"開始前會先重新建立 PrinterBackup 資料夾。"));

    if (ShowMessage(L"PrtEasyBAK", JoinLines(previewLines), MB_YESNO | MB_ICONQUESTION) != IDYES) {
        return true;
    }

    if (!ResetBackupRoot()) {
        ShowErrorWithLog(
            L"PrtEasyBAK",
            UiText(
                L"Failed to recreate the backup folder.",
                L"無法重新建立備份資料夾。"));
        return false;
    }

    const std::wstring defaultPrinterName = GetDefaultPrinterName();
    std::map<std::wstring, DriverBackupInfo> driverCache;
    std::vector<std::wstring> warnings;
    std::vector<std::wstring> failures;
    int successCount = 0;
    const int totalSteps = static_cast<int>(targetPrinters.size()) * 3 + 1;

    ShowProgressWindow(
        L"PrtEasyBAK",
        UiText(L"Preparing backup...", L"正在準備備份..."),
        totalSteps,
        UiText(L"Checking local printers...", L"正在檢查本機印表機..."));

    for (std::size_t index = 0; index < targetPrinters.size(); ++index) {
        const auto& printer = targetPrinters[index];
        const std::wstring progressText =
            std::wstring(UiText(L"Backing up printer ", L"正在備份印表機 ")) +
            std::to_wstring(index + 1) + L" / " + std::to_wstring(targetPrinters.size());
        const int stepBase = static_cast<int>(index) * 3;

        UpdateProgressWindow(
            stepBase + 1,
            progressText,
            printer.name + L"\r\n" + UiText(L"Collecting driver package...", L"正在收集驅動程式封裝..."));

        DriverBackupInfo driverInfo;
        std::wstring errorText;
        if (!CopyDriverPackage(printer, driverCache, driverInfo, errorText)) {
            failures.push_back(printer.name + L": " + errorText);
            continue;
        }

        const std::wstring folderName = std::wstring(index + 1 < 10 ? L"0" : L"") + std::to_wstring(index + 1) + L"_" + SanitizeFileName(printer.name);
        const fs::path printerFolder = g_printersRoot / folderName;
        EnsureDirectory(printerFolder);

        UpdateProgressWindow(
            stepBase + 2,
            progressText,
            printer.name + L"\r\n" + UiText(L"Exporting printer settings...", L"正在匯出印表機設定..."));

        const fs::path settingsPath = GetSettingsPathForPrinterFolder(printerFolder);
        std::wstring settingsError;
        if (!ExportPrinterSettings(printer.name, settingsPath, settingsError)) {
            warnings.push_back(printer.name + L": " + settingsError);
        }

        PrinterEntry entry;
        entry.name = printer.name;
        entry.driverName = printer.driverName;
        entry.portName = printer.portName;
        entry.shared = (printer.attributes & PRINTER_ATTRIBUTE_SHARED) != 0;
        entry.shareName = printer.shareName;
        entry.comment = printer.comment;
        entry.location = printer.location;
        entry.datatype = printer.datatype;
        entry.printProcessor = printer.printProcessor;
        entry.published = (printer.attributes & PRINTER_ATTRIBUTE_PUBLISHED) != 0;
        entry.keepPrintedJobs = (printer.attributes & PRINTER_ATTRIBUTE_KEEPPRINTEDJOBS) != 0;
        entry.isDefault = IEquals(printer.name, defaultPrinterName);
        entry.driver = driverInfo;

        const std::wstring connectionName =
            BuildConnectionPath(printer.serverName, printer.shareName).empty()
                ? (LooksLikeConnectionPath(printer.portName) ? printer.portName : L"")
                : BuildConnectionPath(printer.serverName, printer.shareName);
        entry.connectionName = connectionName;
        entry.isConnectionPrinter = !connectionName.empty() || ((printer.attributes & PRINTER_ATTRIBUTE_NETWORK) != 0);

        if (entry.isConnectionPrinter) {
            entry.port.name = connectionName.empty() ? printer.portName : connectionName;
            entry.port.kind = L"Connection";
            std::wstring serverName;
            std::wstring shareName;
            if (SplitConnectionPath(entry.port.name, serverName, shareName)) {
                entry.port.printerHostAddress = serverName;
                entry.port.queueName = shareName;
            }
        } else {
            QueryPortInfo(printer.portName, entry.port);
        }

        if (StartsWithI(printer.portName, L"USB")) {
            entry.usbPnpKey = NormalizeUsbPnpDeviceId(QueryPrinterPnpId(printer.name));
        }

        UpdateProgressWindow(
            stepBase + 3,
            progressText,
            printer.name + L"\r\n" + UiText(L"Saving printer metadata...", L"正在儲存印表機資訊..."));

        if (!WritePrinterEntryFile(printerFolder / L"info.ini", entry)) {
            failures.push_back(
                printer.name +
                UiText(L": failed to write printer metadata.", L"：寫入印表機資訊失敗。"));
            continue;
        }

        ++successCount;
    }

    UpdateProgressWindow(
        totalSteps,
        UiText(L"Finishing backup...", L"正在完成備份..."),
        g_backupRoot.wstring());
    CloseProgressWindow();
    WriteBackupManifest(defaultPrinterName, successCount);
    OpenFolderPath(g_backupRoot);

    std::vector<std::wstring> resultLines;
    resultLines.push_back(UiText(L"Backup completed.", L"備份完成。"));
    resultLines.push_back(
        std::wstring(UiText(L"Succeeded: ", L"成功：")) +
        std::to_wstring(successCount) +
        UiText(L" item(s)", L" 台"));
    if (!skippedPrinters.empty()) {
        resultLines.push_back(
            std::wstring(UiText(L"Skipped virtual printers: ", L"已略過虛擬印表機：")) +
            std::to_wstring(skippedPrinters.size()) +
            UiText(L" item(s)", L" 台"));
    }
    resultLines.push_back(std::wstring(UiText(L"Backup folder: ", L"備份資料夾：")) + g_backupRoot.wstring());
    if (!warnings.empty()) {
        resultLines.push_back(L"");
        resultLines.push_back(std::wstring(UiText(L"Warnings: ", L"警告：")) + std::to_wstring(warnings.size()));
        resultLines.insert(resultLines.end(), warnings.begin(), warnings.end());
    }
    if (!failures.empty()) {
        resultLines.push_back(L"");
        resultLines.push_back(std::wstring(UiText(L"Failed: ", L"失敗：")) + std::to_wstring(failures.size()));
        resultLines.insert(resultLines.end(), failures.begin(), failures.end());
    }

    ShowMessage(L"PrtEasyBAK", JoinLines(resultLines), MB_OK | MB_ICONINFORMATION);
    return failures.empty();
}

bool RestoreLocalPrinters() {
    std::vector<std::pair<fs::path, PrinterEntry>> entries;
    std::wstring desiredDefaultPrinterName;
    if (!LoadBackupEntries(entries, desiredDefaultPrinterName)) {
        ShowErrorWithLog(
            L"PrtEasyBAK",
            UiText(
                L"No usable backup data was found. Please confirm that PrinterBackup\\backup.ini and the printer folders exist.",
                L"找不到可用的備份資料。請確認 PrinterBackup\\backup.ini 與印表機資料夾都存在。"));
        return false;
    }

    std::vector<int> selectedIndices;
    if (!ShowCheckListDialog(
            UiText(L"Choose Printers to Restore", L"選擇要還原的印表機"),
            UiText(L"Choose the printers to restore:", L"可勾選要還原的印表機："),
            UiText(
                L"Only the checked printers will be restored. Existing queues or ports may be created or updated during restore.",
                L"只會還原已勾選的印表機。還原過程中可能會建立或更新現有的佇列、連接埠與驅動程式。"),
            BuildCheckListItems(entries),
            selectedIndices)) {
        return true;
    }

    entries = SelectItemsByIndices(entries, selectedIndices);
    if (entries.empty()) {
        return true;
    }

    std::vector<std::wstring> previewLines;
    previewLines.push_back(
        std::wstring(UiText(L"The following printers will be restored: ", L"以下印表機將會還原：")) +
        std::to_wstring(entries.size()) +
        UiText(L" item(s)", L" 台"));
    previewLines.push_back(L"");
    for (const auto& item : entries) {
        previewLines.push_back(L"- " + item.second.name);
    }
    previewLines.push_back(L"");
    previewLines.push_back(UiText(
        L"Existing printer queues, ports, or drivers may be created or updated during restore.",
        L"還原過程中可能會建立或更新印表機佇列、連接埠與驅動程式。"));

    if (ShowMessage(L"PrtEasyBAK", JoinLines(previewLines), MB_YESNO | MB_ICONQUESTION) != IDYES) {
        return true;
    }

    std::map<std::wstring, DriverRestoreMaterial> driverCache;
    std::vector<std::wstring> warnings;
    std::vector<std::wstring> failures;
    std::wstring actualDefaultPrinterName;
    int successCount = 0;
    const int totalSteps = static_cast<int>(entries.size()) * 4 + 1;

    ShowProgressWindow(
        L"PrtEasyBAK",
        UiText(L"Preparing restore...", L"正在準備還原..."),
        totalSteps,
        UiText(L"Checking backup data...", L"正在檢查備份資料..."));

    for (std::size_t index = 0; index < entries.size(); ++index) {
        const fs::path printerFolder = entries[index].first;
        PrinterEntry entry = entries[index].second;
        const std::wstring progressText =
            std::wstring(UiText(L"Restoring printer ", L"正在還原印表機 ")) +
            std::to_wstring(index + 1) + L" / " + std::to_wstring(entries.size());
        const int stepBase = static_cast<int>(index) * 4;

        UpdateProgressWindow(
            stepBase + 1,
            progressText,
            entry.name + L"\r\n" + UiText(L"Preparing driver files...", L"正在準備驅動程式檔案..."));

        DriverRestoreMaterial material;
        std::wstring errorText;
        if (!PrepareDriverMaterial(entry, driverCache, material, errorText)) {
            failures.push_back(entry.name + L": " + errorText);
            continue;
        }

        std::wstring actualPrinterName;
        bool restored = false;
        if (entry.isConnectionPrinter && !entry.connectionName.empty()) {
            UpdateProgressWindow(
                stepBase + 2,
                progressText,
                entry.name + L"\r\n" + UiText(L"Restoring shared printer connection...", L"正在還原共用印表機連線..."));
            restored = RestoreConnectionPrinter(entry, material, actualPrinterName, errorText);
            if (restored) {
                UpdateProgressWindow(
                    stepBase + 3,
                    progressText,
                    entry.name + L"\r\n" + UiText(L"Refreshing printer information...", L"正在重新整理印表機資訊..."));
            }
        } else {
            std::wstring resolvedDriverName;
            UpdateProgressWindow(
                stepBase + 2,
                progressText,
                entry.name + L"\r\n" + UiText(L"Installing printer driver...", L"正在安裝印表機驅動程式..."));
            if (!EnsureDriverInstalled(entry, material, resolvedDriverName, errorText)) {
                restored = false;
            } else {
                std::wstring resolvedPortName;
                UpdateProgressWindow(
                    stepBase + 3,
                    progressText,
                    entry.name + L"\r\n" + UiText(L"Preparing port and creating queue...", L"正在準備連接埠並建立印表機佇列..."));
                if (!EnsurePrinterPort(entry, resolvedPortName, errorText)) {
                    restored = false;
                } else if (!EnsurePrinterCreated(entry, material, resolvedDriverName, resolvedPortName, errorText)) {
                    restored = false;
                } else {
                    ApplyPrinterProperties(entry, entry.name);
                    actualPrinterName = entry.name;
                    restored = true;
                }
            }
        }

        if (!restored) {
            failures.push_back(entry.name + L": " + errorText);
            continue;
        }

        UpdateProgressWindow(
            stepBase + 4,
            progressText,
            entry.name + L"\r\n" + UiText(L"Applying saved printer settings...", L"正在套用已備份的印表機設定..."));

        const fs::path settingsPath = GetSettingsPathForPrinterFolder(printerFolder);
        if (!ImportPrinterSettings(actualPrinterName.empty() ? entry.name : actualPrinterName, settingsPath)) {
            warnings.push_back(
                entry.name +
                UiText(L": failed to import settings.dat.", L"：匯入 settings.dat 失敗。"));
        }

        const std::wstring effectivePrinterName = actualPrinterName.empty() ? entry.name : actualPrinterName;
        if (entry.isDefault || (!desiredDefaultPrinterName.empty() && IEquals(entry.name, desiredDefaultPrinterName))) {
            actualDefaultPrinterName = effectivePrinterName;
        }

        ++successCount;
    }

    UpdateProgressWindow(
        totalSteps,
        UiText(L"Finishing restore...", L"正在完成還原..."),
        UiText(L"Setting default printer and opening Printers.", L"正在設定預設印表機並開啟印表機視窗..."));
    CloseProgressWindow();

    if (!actualDefaultPrinterName.empty() && PrinterExists(actualDefaultPrinterName)) {
        SetPrinterAsDefault(actualDefaultPrinterName);
    }

    OpenPrintersFolder();

    std::vector<std::wstring> resultLines;
    resultLines.push_back(UiText(L"Restore completed.", L"還原完成。"));
    resultLines.push_back(
        std::wstring(UiText(L"Succeeded: ", L"成功：")) +
        std::to_wstring(successCount) +
        UiText(L" item(s)", L" 台"));
    if (!warnings.empty()) {
        resultLines.push_back(L"");
        resultLines.push_back(std::wstring(UiText(L"Warnings: ", L"警告：")) + std::to_wstring(warnings.size()));
        resultLines.insert(resultLines.end(), warnings.begin(), warnings.end());
    }
    if (!failures.empty()) {
        resultLines.push_back(L"");
        resultLines.push_back(std::wstring(UiText(L"Failed: ", L"失敗：")) + std::to_wstring(failures.size()));
        resultLines.insert(resultLines.end(), failures.begin(), failures.end());
    }

    ShowMessage(L"PrtEasyBAK", JoinLines(resultLines), MB_OK | MB_ICONINFORMATION);
    return failures.empty();
}

int WINAPI wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    SetProcessDPIAware();

    g_baseDir = GetModuleDirectory();
    g_backupRoot = g_baseDir / L"PrinterBackup";
    g_driversRoot = g_backupRoot / L"Drivers";
    g_printersRoot = g_backupRoot / L"Printers";
    g_configPath = g_baseDir / L"PrtEasyBAK.ini";
    g_logPath = g_baseDir / L"PrtEasyBAK.log";

    std::error_code ec;
    fs::remove(g_logPath, ec);
    EnsureConfigIniExists();
    ApplyLanguageSetting(ReadIniLanguageSetting(g_configPath));

    if (!IsRunningAsAdmin()) {
        if (!RelaunchAsAdmin()) {
            ShowErrorWithLog(
                L"PrtEasyBAK",
                UiText(
                    L"Administrator permission is required to run this tool.",
                    L"執行此工具需要系統管理員權限。"));
            return 1;
        }
        return 0;
    }

    if (!EnsureSpoolerRunning()) {
        ShowErrorWithLog(
            L"PrtEasyBAK",
            UiText(
                L"The Print Spooler service is not running, so the tool cannot continue.",
                L"Print Spooler 服務未啟動，工具無法繼續執行。"));
        return 1;
    }

    const AppAction action = ShowActionDialog();
    if (action == AppAction::Cancel) {
        return 0;
    }

    bool ok = true;
    try {
        if (action == AppAction::Backup) {
            ok = BackupLocalPrinters();
        } else if (action == AppAction::Restore) {
            ok = RestoreLocalPrinters();
        }
    } catch (const std::exception& ex) {
        ShowErrorWithLog(
            L"PrtEasyBAK",
            std::wstring(UiText(L"An unexpected error occurred:\r\n", L"執行時發生未預期錯誤：\r\n")) +
                WideFromBytes(ex.what(), CP_UTF8));
        ok = false;
    } catch (...) {
        ShowErrorWithLog(
            L"PrtEasyBAK",
            UiText(
                L"An unexpected error occurred while running this tool.",
                L"執行工具時發生未預期錯誤。"));
        ok = false;
    }

    CleanupTempRoot();
    return ok ? 0 : 1;
}
