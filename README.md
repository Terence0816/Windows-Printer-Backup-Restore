# Windows Printer Backup Restore

## 中文介紹

Windows Printer Backup Restore 是一個用於備份與還原 Windows 本機印表機的簡易工具。

這個工具主要是為 IT 技術人員、系統管理員，以及需要將印表機設定從一台 Windows 電腦移轉到另一台電腦的使用者所設計。

### 功能特色

- 備份本機已安裝的實體印表機
- 在另一台 Windows 電腦上還原印表機
- 備份印表機驅動程式
- 備份印表機連接埠
- 備份印表機設定
- 自動略過常見虛擬印表機，例如 Microsoft Print to PDF、OneNote、Adobe PDF
- 提供可攜式 EXE 版本
- 提供繁體中文操作介面

### 使用方式

#### 備份

1. 以系統管理員身分執行工具。
2. 選擇 `備份`。
3. 工具會匯出印表機資訊與相關檔案。
4. 如有需要，可將產生的備份資料夾複製到另一台電腦。

#### 還原

1. 在目標電腦上以系統管理員身分執行工具。
2. 確認備份資料夾已放在工具旁邊。
3. 選擇 `還原`。
4. 工具會盡可能還原印表機、連接埠、驅動程式與印表機設定。

### 注意事項

- 必須以系統管理員權限執行。
- 部分印表機設定可能會受到驅動程式版本與 Windows 版本影響。
- USB 印表機在還原後，可能仍需要重新插拔或手動確認對應的 USB 連接埠。
- 網路共用印表機可能需要原始列印伺服器仍可連線。
- 本工具主要用於實體印表機的備份與還原，以及 IT 維護用途。

### 下載

目前版本：`v1.0.0.0`

- [前往 Releases / 發行版本頁面](https://github.com/Terence0816/Windows-Printer-Backup-Restore/releases)
- [直接下載 PrtBAK_BackupRestore_v1.0.0.0.exe](https://github.com/Terence0816/Windows-Printer-Backup-Restore/releases/download/v1.0.0.0/PrtBAK_BackupRestore_v1.0.0.0.exe)

### 授權

MIT License

---

## English

Windows Printer Backup Restore is a simple tool for backing up and restoring local Windows printers.

It is designed for IT technicians, system administrators, and users who need to migrate printer settings from one Windows computer to another.

### Features

- Backup local physical printers
- Restore printers on another Windows PC
- Backup printer drivers
- Backup printer ports
- Backup printer settings
- Automatically skip common virtual printers such as Microsoft Print to PDF, OneNote, and Adobe PDF
- Portable EXE version
- Traditional Chinese interface

### Usage

#### Backup

1. Run the tool as administrator.
2. Choose `Backup`.
3. The tool will export printer information and related files.
4. Copy the generated backup folder to another computer if needed.

#### Restore

1. Run the tool as administrator on the target computer.
2. Make sure the backup folder is placed next to the tool.
3. Choose `Restore`.
4. The tool will restore printers, ports, drivers, and settings as much as possible.

### Notes

- Administrator permission is required.
- Some printer settings may depend on the printer driver version and Windows version.
- USB printers may still need to be reconnected or manually confirmed after restore.
- Network shared printers may require the source print server to remain available.
- This tool is mainly intended for physical printers and IT maintenance use.

### Download

Current version: `v1.0.0.0`

- [Open the Releases page](https://github.com/Terence0816/Windows-Printer-Backup-Restore/releases)
- [Download PrtBAK_BackupRestore_v1.0.0.0.exe directly](https://github.com/Terence0816/Windows-Printer-Backup-Restore/releases/download/v1.0.0.0/PrtBAK_BackupRestore_v1.0.0.0.exe)

### License

MIT License
