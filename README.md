![Downloads](https://img.shields.io/github/downloads/Terence0816/Windows-Printer-Backup-Restore/total?label=Downloads&color=success)
![Release](https://img.shields.io/github/v/release/Terence0816/Windows-Printer-Backup-Restore?label=Release&color=blue)

# 🖨️ Windows Printer Backup Restore

**[🇺🇸 English](#-english-introduction) | [🇹🇼 繁體中文](#-繁體中文介紹)**

---

## 🇹🇼 繁體中文介紹

**Windows Printer Backup Restore** 是一個用於備份與還原 Windows 本機印表機的輕量化工具，適合 **IT 管理員**、**系統維護人員**，以及需要將印表機設定從一台電腦移轉到另一台電腦的使用者。

它可以將印表機驅動程式、連接埠、印表機設定與預設印表機資訊一起備份，並在另一台 Windows 電腦上盡可能自動還原。

### ✨ 主要功能

- **備份本機實體印表機**：自動備份本機已安裝的實體印表機。
- **還原到另一台 Windows 電腦**：可將備份資料帶到其他電腦進行還原。
- **備份驅動程式與連接埠**：包含印表機驅動、TCP/IP、USB、Local Port 與 LPR Port 資訊。
- **備份印表機設定**：支援匯出與匯入印表機偏好設定。
- **預設印表機還原**：備份時記錄原本的預設印表機，還原時若條件符合會一併恢復。
- **中英雙語介面**：支援繁體中文與 English，並依系統語系自動決定預設語言。
- **可選擇還原的印表機**：還原前可勾選要還原的印表機，預設全選。
- **顯示型號與連接埠資訊**：還原清單會顯示印表機名稱、型號或驅動、連接埠。
- **指定預設印表機**：還原時可選一台作為預設印表機，也可選擇不變更。
- **共享印表機三種還原模式**：遇到 `\\server\share` 時，可選擇原始連接、Local Port 或 LPR Port 方式還原。
- **略過常見虛擬印表機**：例如 Microsoft Print to PDF、OneNote、Adobe PDF 等。
- **支援網路路徑執行**：可從網路分享路徑執行 EXE 進行備份與還原。
- **已數位簽章 EXE**：提供已完成數位簽章的可攜式執行檔。

### 🆕 今日更新

更新日期：`2026-06-02`

- 共享印表機 `\\server\share` 還原時，新增三種模式可選：原始連接、Local Port、LPR Port。
- 共享印表機還原前會先安裝備份的驅動程式，再進行後續連接或建立。
- 改善共享印表機的路徑解析，會更準確使用原始伺服器與分享名稱。
- 調整 Local Port 與 LPR Port 的還原流程，讓共享印表機更容易在另一台電腦建立成功。
- 發布 `v1.1.1.0` 數位簽章 EXE。

### 🚀 使用方式

#### 備份

1. 以 **系統管理員身分** 執行工具。
2. 選擇 `備份本機印表機`。
3. 工具會自動掃描可備份的印表機並建立 `PrinterBackup` 資料夾。
4. 完成後可將整個備份資料夾複製到另一台電腦。

#### 還原

1. 在目標電腦上以 **系統管理員身分** 執行工具。
2. 確認 `PrinterBackup` 資料夾放在工具旁邊。
3. 選擇 `還原印表機設定到本機`。
4. 先勾選要還原的印表機，必要時指定預設印表機。
5. 若為共享印表機，可依需求選擇原始連接、Local Port 或 LPR Port 方式還原。
6. 工具會盡可能還原印表機、連接埠、驅動程式、印表機設定與預設印表機。

### 📦 下載

目前版本：`v1.1.1.0`

- [前往 Releases / 發行版本頁面](https://github.com/Terence0816/Windows-Printer-Backup-Restore/releases)
- [直接下載 PrtBAK_BackupRestore_v1.1.1.0.exe](https://github.com/Terence0816/Windows-Printer-Backup-Restore/releases/download/v1.1.1.0/PrtBAK_BackupRestore_v1.1.1.0.exe)

### ⚠️ 注意事項

- 必須以系統管理員權限執行。
- 部分印表機設定仍可能受到驅動程式版本與 Windows 版本影響。
- USB 印表機在還原後，可能仍需要重新插拔或手動確認 USB 連接埠。
- 網路共用印表機可能需要原始列印伺服器仍可連線。
- 本工具主要設計給實體印表機備份、移機與 IT 維護用途使用。

### 📄 授權

MIT License

---

## 🇺🇸 English Introduction

**Windows Printer Backup Restore** is a lightweight utility for backing up and restoring local Windows printers. It is designed for **IT administrators**, **support engineers**, and users who need to migrate printer settings from one Windows computer to another.

The tool can back up printer drivers, ports, printer preferences, and the original default printer, then restore them on another Windows PC as automatically as possible.

### ✨ Key Features

- **Backup local physical printers**: Automatically backs up installed physical printers.
- **Restore on another Windows PC**: Move the backup folder to another computer and restore from it.
- **Backup drivers and ports**: Includes printer drivers and TCP/IP, USB, Local Port, and LPR Port information.
- **Backup printer settings**: Supports exporting and importing printer preferences.
- **Restore default printer**: Records the original default printer during backup and restores it when possible.
- **Bilingual interface**: Supports Traditional Chinese and English, with automatic default language detection based on the system UI language.
- **Selectable restore list**: Choose which printers to restore before the restore process starts.
- **Model and port display**: The restore list shows printer name, model or driver, and port.
- **Default printer choice**: You can mark one restored printer as the default printer, or leave the current system default unchanged.
- **Three restore modes for shared printers**: For `\\server\share` printers, choose Original connection, Local Port, or LPR Port during restore.
- **Skip common virtual printers**: Such as Microsoft Print to PDF, OneNote, and Adobe PDF.
- **Network-path friendly**: Can run from a shared network path for both backup and restore.
- **Digitally signed EXE**: Includes a digitally signed portable executable.

### 🆕 Today's Update

Updated on `2026-06-02`

- Added three restore modes for shared `\\server\share` printers: Original connection, Local Port, and LPR Port.
- Shared-printer restore now installs the backed-up driver first before connecting or building the printer.
- Improved shared-printer path handling so the original server and share name are used more accurately.
- Adjusted the Local Port and LPR Port restore flow to improve shared-printer recovery on another PC.
- Released the digitally signed `v1.1.1.0` EXE.

### 🚀 Usage

#### Backup

1. Run the tool as **Administrator**.
2. Choose `備份本機印表機`.
3. The tool scans eligible printers and creates a `PrinterBackup` folder.
4. Copy the backup folder to another computer if needed.

#### Restore

1. Run the tool as **Administrator** on the target computer.
2. Make sure the `PrinterBackup` folder is placed next to the tool.
3. Choose `還原印表機設定到本機`.
4. Select the printers you want to restore and choose a default printer if needed.
5. For shared printers, choose Original connection, Local Port, or LPR Port as needed.
6. The tool restores printers, ports, drivers, printer settings, and the default printer as much as possible.

### 📦 Download

Current version: `v1.1.1.0`

- [Open the Releases page](https://github.com/Terence0816/Windows-Printer-Backup-Restore/releases)
- [Download PrtBAK_BackupRestore_v1.1.1.0.exe directly](https://github.com/Terence0816/Windows-Printer-Backup-Restore/releases/download/v1.1.1.0/PrtBAK_BackupRestore_v1.1.1.0.exe)

### ⚠️ Notes

- Administrator permission is required.
- Some printer preferences may still depend on the printer driver version and Windows version.
- USB printers may still need to be reconnected or manually confirmed after restore.
- Network shared printers may require the original print server to remain available.
- This tool is mainly intended for physical printer migration, backup, and IT maintenance use.

### 📄 License

MIT License
