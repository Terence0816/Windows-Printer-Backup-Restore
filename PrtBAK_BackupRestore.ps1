[CmdletBinding()]
param()

$ErrorActionPreference = 'Stop'

function Test-IsAdministrator {
    $identity = [Security.Principal.WindowsIdentity]::GetCurrent()
    $principal = New-Object Security.Principal.WindowsPrincipal($identity)
    return $principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)
}

if (-not (Test-IsAdministrator)) {
    $elevatedArgs = @(
        '-NoProfile'
        '-ExecutionPolicy'
        'Bypass'
        '-File'
        ('"{0}"' -f $PSCommandPath)
    )
    Start-Process powershell.exe -ArgumentList $elevatedArgs -Verb RunAs
    exit
}

Add-Type -AssemblyName System.Windows.Forms
Add-Type -AssemblyName System.Drawing

function Convert-ToNativePath {
    param(
        [string]$Path,
        [switch]$AllowMissing
    )

    if ([string]::IsNullOrWhiteSpace($Path)) {
        return $null
    }

    $candidate = $Path.Trim()
    if ($candidate -match '^[^:]+::') {
        $candidate = $candidate -replace '^[^:]+::', ''
    }

    try {
        if (Test-Path -LiteralPath $candidate) {
            $resolved = Resolve-Path -LiteralPath $candidate -ErrorAction Stop | Select-Object -First 1
            if ($resolved -and $resolved.ProviderPath) {
                return $resolved.ProviderPath.TrimEnd('\')
            }
        }
    } catch {
    }

    if ($AllowMissing) {
        if ([System.IO.Path]::IsPathRooted($candidate)) {
            return [System.IO.Path]::GetFullPath($candidate).TrimEnd('\')
        }

        $basePath = (Get-Location).ProviderPath
        return [System.IO.Path]::GetFullPath((Join-Path -Path $basePath -ChildPath $candidate)).TrimEnd('\')
    }

    return $candidate.TrimEnd('\')
}

function Get-LocalTempRoot {
    if (-not $script:LocalTempRoot) {
        $script:LocalTempRoot = Join-Path $env:TEMP ('PrtBAK_' + [Guid]::NewGuid().Guid)
        New-Item -ItemType Directory -Path $script:LocalTempRoot -Force | Out-Null
    }

    return $script:LocalTempRoot
}

function New-LocalTempPath {
    param(
        [string]$Category,
        [string]$LeafName
    )

    $root = Get-LocalTempRoot
    $folder = Join-Path $root $Category
    New-Item -ItemType Directory -Path $folder -Force | Out-Null
    return Join-Path $folder $LeafName
}

function Stage-FileForLocalUse {
    param(
        [string]$SourcePath,
        [string]$Category = 'InputFiles'
    )

    $nativeSourcePath = Convert-ToNativePath -Path $SourcePath
    $leafName = Split-Path -Path $nativeSourcePath -Leaf
    $targetPath = New-LocalTempPath -Category $Category -LeafName ('{0}_{1}' -f ([Guid]::NewGuid().Guid), $leafName)
    Copy-Item -LiteralPath $nativeSourcePath -Destination $targetPath -Force
    return $targetPath
}

function Stage-FolderForLocalUse {
    param(
        [string]$SourceFolder,
        [string]$Category = 'InputFolders'
    )

    $nativeSourceFolder = Convert-ToNativePath -Path $SourceFolder
    $leafName = Split-Path -Path $nativeSourceFolder -Leaf
    $targetRoot = New-LocalTempPath -Category $Category -LeafName ([Guid]::NewGuid().Guid)
    New-Item -ItemType Directory -Path $targetRoot -Force | Out-Null
    $targetFolder = Join-Path $targetRoot $leafName
    Copy-Item -LiteralPath $nativeSourceFolder -Destination $targetFolder -Recurse -Force
    return $targetFolder
}

$BaseDir = Convert-ToNativePath -Path $PSScriptRoot -AllowMissing
if ([string]::IsNullOrWhiteSpace($BaseDir)) {
    $BaseDir = Convert-ToNativePath -Path (Get-Location).ProviderPath -AllowMissing
}
$BaseDir = $BaseDir.TrimEnd('\')

$BackupRoot = Join-Path $BaseDir 'PrinterBackup'
$PrintersRoot = Join-Path $BackupRoot 'Printers'
$DriversRoot = Join-Path $BackupRoot 'Drivers'
$ManifestPath = Join-Path $BackupRoot 'manifest.json'

function New-UiText {
    param([int[]]$CodePoints)
    return [string]([char[]]$CodePoints)
}

function Get-UiText {
    param([string]$Key)

    if (-not $script:UiTextMap) {
        $script:UiTextMap = @{
            ActionTitle            = 'UHJ0QkFLIOWNsOihqOapn+W3peWFtw=='
            ActionPrompt           = '6KuL6YG45pOH6KaB5Z+36KGM55qE5Yqf6IO977ya'
            SelectItemPrompt       = '6KuL6YG45pOH6aCF55uu77ya'
            Ok                     = '56K65a6a'
            Cancel                 = '5Y+W5raI'
            BackupNoPrinters       = '5rKS5pyJ5om+5Yiw5Y+v5YKZ5Lu955qE5Y2w6KGo5qmf44CC'
            BackupErrorTitle       = '5YKZ5Lu95aSx5pWX'
            BackupConfirmTitle     = '5YKZ5Lu956K66KqN'
            BackupTargets          = '5Y2z5bCH5YKZ5Lu955qE5Y2w6KGo5qmf77yaezB9IOWPsA=='
            SkippedVirtuals        = '55Wl6YGO55qE6Jmb5pOs5Y2w6KGo5qmf77yaezB9IOWPsA=='
            ContinueOverwrite      = '5piv5ZCm57m857qM5Lux6KaG6JOL5pei5pyJ55qEIFByaW50ZXJCYWNrdXAg6LOH5paZ5aS+77yf'
            BackupCompleteTitle    = '5YKZ5Lu95a6M5oiQ'
            BackupFinished         = '5YKZ5Lu95a6M5oiQ44CC'
            BackupProgressTitle    = '5YKZ5Lu96YCy6KGM5Lit'
            BackupProgressStarting = '5q2j5Zyo5rqW5YKZ5YKZ5Lu9Li4u'
            BackupProgressCurrent  = '5YKZ5Lu96YCy5bqm77yaezB9IC8gezF9'
            BackupProgressDetail   = '5q2j5Zyo5YKZ5Lu977yaezB9'
            BackupProgressFinalize = '5q2j5Zyo5pW055CG5YKZ5Lu957WQ5p6cLi4u'
            SuccessCount           = '5oiQ5Yqf77yaezB9IOWPsA=='
            SkippedCount           = '55Wl6YGO6Jmb5pOs5Y2w6KGo5qmf77yaezB9IOWPsA=='
            BackupFolder           = '5YKZ5Lu96LOH5paZ5aS+77yaezB9'
            WarningsCount          = '6K2m5ZGK77yaezB9IOmghQ=='
            SettingsNotExported    = 'ezB977ya5Y2w6KGo5qmf6Kit5a6a5pyq5Yyv5Ye677yIezF977yJ'
            FailedCount            = '5aSx5pWX77yaezB9IOWPsA=='
            UsbPortTitle           = '6YG45pOHIFVTQiDpgKPmjqXln6A='
            UsbPortPrompt          = '54Sh5rOV6Ieq5YuV5Yik5pa35bCN5oeJ55qEIFVTQiDpgKPmjqXln6DvvIzoq4vmiYvli5Xpgbjmk4fvvJo='
            UsbPortCancelled       = '5bey5Y+W5raI6YG45pOHIFVTQiDpgKPmjqXln6DjgII='
            NoUsbPorts             = '5pys5qmf5om+5LiN5Yiw5Y+v55So55qEIFVTQiDljbDooajmqZ/pgKPmjqXln6DjgII='
            PortTitle              = '6YG45pOH6YCj5o6l5Z+g'
            NoPorts                = '5pys5qmf5rKS5pyJ5Y+v55So55qE5Y2w6KGo5qmf6YCj5o6l5Z+g44CC'
            PortCancelled          = '5bey5Y+W5raI6YG45pOH6YCj5o6l5Z+g44CC'
            MissingPortInfo        = '5YKZ5Lu96LOH5paZ57y65bCR6YCj5o6l5Z+g6LOH6KiK44CC'
            TcpMissingHost         = 'VENQIOmAo+aOpeWfoOe8uuWwkeS4u+apn+S9jeWdgO+8mnswfQ=='
            LprIncomplete          = 'TFBSIOmAo+aOpeWfoOizh+aWmeS4jeWujOaVtO+8mnswfQ=='
            WsdPortMissing         = '5om+5LiN5Yiw5Y6f5aeL55qEIFdTRCDpgKPmjqXln6AgW3swfV3vvIzoq4vpgbjmk4fmnKzmqZ/nj77mnInpgKPmjqXln6DjgII='
            PortMissing            = '5om+5LiN5Yiw5Y6f5aeL6YCj5o6l5Z+gIFt7MH1d77yM6KuL6YG45pOH6KaB5L2/55So55qE6YCj5o6l5Z+g44CC'
            BackupInfMissing       = '5om+5LiN5Yiw5YKZ5Lu955qE6amF5YuVIElORu+8mnswfQ=='
            DriverInstallFailed    = '6amF5YuV5a6J6KOd5aSx5pWX77yaezB9'
            RestoreErrorTitle      = '6YKE5Y6f5aSx5pWX'
            ManifestMissing        = '5om+5LiN5Yiw5YKZ5Lu95riF5Zau77yaezB9'
            NoRestorePrinters      = '5YKZ5Lu95riF5Zau5YWn5rKS5pyJ5Y+v6YKE5Y6f55qE5Y2w6KGo5qmf6LOH5paZ44CC'
            RestoreConfirmTitle    = '6YKE5Y6f56K66KqN'
            RestoreTargets         = '5Y2z5bCH6YKE5Y6f55qE5Y2w6KGo5qmf77yaezB9IOWPsA=='
            ExistingPrinterNote    = '6Iul5pys5qmf5bey5pyJ5ZCM5ZCN5Y2w6KGo5qmf77yM5pyD5YWI56e76Zmk5YaN6YeN5bu644CC'
            RestoreCompleteTitle   = '6YKE5Y6f5a6M5oiQ'
            RestoreFinished        = '6YKE5Y6f5a6M5oiQ44CC'
            RestoreProgressTitle   = '6YKE5Y6f6YCy6KGM5Lit'
            RestoreProgressStarting = '5q2j5Zyo5rqW5YKZ6YKE5Y6fLi4u'
            RestoreProgressCurrent = '6YKE5Y6f6YCy5bqm77yaezB9IC8gezF9'
            RestoreProgressDetail  = '5q2j5Zyo6YKE5Y6f77yaezB9'
            RestoreProgressFinalize = '5q2j5Zyo5pW055CG6YKE5Y6f57WQ5p6cLi4u'
            RestoreProgressDriver  = '5q2j5Zyo5a6J6KOd6amF5YuV77yaezB9'
            RestoreProgressPort    = '5q2j5Zyo5bu656uL6YCj5o6l5Z+g77yaezB9'
            RestoreProgressCreate  = '5q2j5Zyo5bu656uL5Y2w6KGo5qmf77yaezB9'
            RestoreProgressSettings = '5q2j5Zyo5aWX55So6Kit5a6a77yaezB9'
            PrintUiFailed          = 'UHJpbnRVSUVudHJ5IOWft+ihjOWkseaVl++8jOS7o+eivCB7MH3vvJp7MX0='
            ExportSettingsFailed   = '5Yyv5Ye65Y2w6KGo5qmf6Kit5a6a5aSx5pWX77yaezB9'
            ImportSettingsFailed   = '5Yyv5YWl5Y2w6KGo5qmf6Kit5a6a5aSx5pWX77yaezB9'
            DriverInfNotFound      = '5om+5LiN5Yiw5Y2w6KGo5qmf6amF5YuVIElORu+8mnswfQ=='
            DriverInfMissingInFolder = '5Zyo6amF5YuV5YKZ5Lu96LOH5paZ5aS+5Lit5om+5LiN5YiwIElORu+8mnswfQ=='
            UnexpectedErrorTitle   = '5Z+36KGM5aSx5pWX'
            UnexpectedError        = '5Z+36KGM5pmC55m855Sf6Yyv6Kqk77yaezB9'
            BackupPathSafetyFailed = '5YKZ5Lu96Lev5b6R5a6J5YWo5qOA5p+l5aSx5pWX77yaezB9'
        }
    }

    $base64 = $script:UiTextMap[$Key]
    if ([string]::IsNullOrWhiteSpace($base64)) {
        return $Key
    }

    return [Text.Encoding]::UTF8.GetString([Convert]::FromBase64String($base64))
}

function Format-UiText {
    param(
        [string]$Key,
        [Parameter(ValueFromRemainingArguments = $true)]
        [object[]]$Arguments
    )

    $text = Get-UiText -Key $Key
    if ($Arguments.Count -gt 0) {
        $formatArgs = @()
        if ($Arguments.Count -eq 1 -and $Arguments[0] -is [System.Array] -and -not ($Arguments[0] -is [string])) {
            $formatArgs = @($Arguments[0])
        } else {
            $formatArgs = @($Arguments)
        }

        return [string]::Format([System.Globalization.CultureInfo]::CurrentCulture, $text, [object[]]$formatArgs)
    }

    return $text
}

$UiBackupButtonText = New-UiText -CodePoints @(0x5099, 0x4EFD, 0x672C, 0x6A5F, 0x5370, 0x8868, 0x6A5F)
$UiRestoreButtonText = New-UiText -CodePoints @(0x9084, 0x539F, 0x5370, 0x8868, 0x6A5F, 0x8A2D, 0x5B9A, 0x5230, 0x672C, 0x6A5F)
$UiCancelText = Get-UiText -Key 'Cancel'

function Show-MsgBox {
    param(
        [string]$Text,
        [string]$Title = 'PrtBAK',
        [System.Windows.Forms.MessageBoxButtons]$Buttons = [System.Windows.Forms.MessageBoxButtons]::OK,
        [System.Windows.Forms.MessageBoxIcon]$Icon = [System.Windows.Forms.MessageBoxIcon]::Information
    )

    $owner = New-Object System.Windows.Forms.Form
    $owner.StartPosition = 'Manual'
    $owner.Location = New-Object System.Drawing.Point(-2000, -2000)
    $owner.Size = New-Object System.Drawing.Size(1, 1)
    $owner.ShowInTaskbar = $false
    $owner.FormBorderStyle = 'FixedToolWindow'
    $owner.Opacity = 0
    $owner.TopMost = $true
    [void]$owner.Show()

    try {
        return [System.Windows.Forms.MessageBox]::Show($owner, $Text, $Title, $Buttons, $Icon)
    } finally {
        $owner.Close()
        $owner.Dispose()
    }
}

function New-ProgressWindow {
    param(
        [string]$Title,
        [int]$Maximum,
        [string]$StatusText,
        [string]$DetailText
    )

    $form = New-Object System.Windows.Forms.Form
    $form.Text = $Title
    $form.Size = New-Object System.Drawing.Size(520, 190)
    $form.StartPosition = 'CenterScreen'
    $form.MinimizeBox = $false
    $form.MaximizeBox = $false
    $form.ControlBox = $false
    $form.ShowInTaskbar = $true
    $form.TopMost = $true
    $form.FormBorderStyle = 'FixedDialog'

    $statusLabel = New-Object System.Windows.Forms.Label
    $statusLabel.Location = New-Object System.Drawing.Point(18, 18)
    $statusLabel.Size = New-Object System.Drawing.Size(470, 24)
    $statusLabel.Font = New-Object System.Drawing.Font('Microsoft JhengHei UI', 10, [System.Drawing.FontStyle]::Bold)
    $statusLabel.Text = $StatusText
    $form.Controls.Add($statusLabel)

    $detailLabel = New-Object System.Windows.Forms.Label
    $detailLabel.Location = New-Object System.Drawing.Point(18, 52)
    $detailLabel.Size = New-Object System.Drawing.Size(470, 36)
    $detailLabel.Text = $DetailText
    $form.Controls.Add($detailLabel)

    $progressBar = New-Object System.Windows.Forms.ProgressBar
    $progressBar.Location = New-Object System.Drawing.Point(18, 103)
    $progressBar.Size = New-Object System.Drawing.Size(470, 24)
    $progressBar.Minimum = 0
    $progressBar.Maximum = [Math]::Max(1, $Maximum)
    $progressBar.Value = 0
    $progressBar.Style = 'Continuous'
    $form.Controls.Add($progressBar)

    [void]$form.Show()
    [System.Windows.Forms.Application]::DoEvents()

    return [pscustomobject]@{
        Form        = $form
        StatusLabel = $statusLabel
        DetailLabel = $detailLabel
        ProgressBar = $progressBar
        Maximum     = $progressBar.Maximum
    }
}

function Update-ProgressWindow {
    param(
        [object]$ProgressWindow,
        [int]$Value,
        [string]$StatusText,
        [string]$DetailText
    )

    if ($null -eq $ProgressWindow) {
        return
    }

    if ($PSBoundParameters.ContainsKey('StatusText')) {
        $ProgressWindow.StatusLabel.Text = $StatusText
    }

    if ($PSBoundParameters.ContainsKey('DetailText')) {
        $ProgressWindow.DetailLabel.Text = $DetailText
    }

    $clampedValue = [Math]::Max(0, [Math]::Min($ProgressWindow.Maximum, $Value))
    if ($ProgressWindow.ProgressBar.Value -ne $clampedValue) {
        $ProgressWindow.ProgressBar.Value = $clampedValue
    }

    $ProgressWindow.Form.Refresh()
    [System.Windows.Forms.Application]::DoEvents()
}

function Close-ProgressWindow {
    param([object]$ProgressWindow)

    if ($null -eq $ProgressWindow) {
        return
    }

    try {
        $ProgressWindow.Form.Close()
        $ProgressWindow.Form.Dispose()
    } catch {
    }
}

function Invoke-PrintUiEntry {
    param(
        [string[]]$Arguments,
        [switch]$IgnoreExitCode
    )

    $argumentText = (($Arguments | Where-Object { -not [string]::IsNullOrWhiteSpace($_) }) -join ' ')
    $process = Start-Process -FilePath 'rundll32.exe' `
        -ArgumentList $argumentText `
        -PassThru -Wait -NoNewWindow

    if (-not $IgnoreExitCode -and $process.ExitCode -ne 0) {
        throw (Format-UiText 'PrintUiFailed' $process.ExitCode, $argumentText)
    }

    return $process
}

function Open-PrintersFolder {
    try {
        Start-Process 'explorer.exe' -ArgumentList 'shell:::{A8A91A66-3A7D-4424-8D24-04E180695C7A}' -ErrorAction Stop
    } catch {
        Start-Process 'control.exe' -ArgumentList '/name Microsoft.DevicesAndPrinters'
    }
}

function Get-ObjectValue {
    param(
        [object]$InputObject,
        [string]$PropertyName,
        [object]$DefaultValue = $null
    )

    if ($null -eq $InputObject) {
        return $DefaultValue
    }

    $property = $InputObject.PSObject.Properties[$PropertyName]
    if ($null -ne $property) {
        return $property.Value
    }

    return $DefaultValue
}

function Get-FirstValue {
    param(
        [object]$InputObject,
        [string[]]$PropertyNames
    )

    foreach ($propertyName in $PropertyNames) {
        $value = Get-ObjectValue -InputObject $InputObject -PropertyName $propertyName
        if ($null -ne $value -and "$value" -ne '') {
            return $value
        }
    }

    return $null
}

function Get-SafeFileName {
    param([string]$Name)

    $safeName = $Name -replace '[<>:"/\\|?*]', '_'
    $safeName = $safeName.Trim().TrimEnd('.')
    if ([string]::IsNullOrWhiteSpace($safeName)) {
        return 'item'
    }

    return $safeName
}

function Get-RelativePath {
    param(
        [string]$BasePath,
        [string]$TargetPath
    )

    $nativeBasePath = Convert-ToNativePath -Path $BasePath -AllowMissing
    $nativeTargetPath = Convert-ToNativePath -Path $TargetPath -AllowMissing
    $baseUri = New-Object System.Uri(($nativeBasePath.TrimEnd('\') + '\'))
    $targetUri = New-Object System.Uri([System.IO.Path]::GetFullPath($nativeTargetPath))
    return [System.Uri]::UnescapeDataString($baseUri.MakeRelativeUri($targetUri).ToString()).Replace('/', '\')
}

function Test-IsChildPath {
    param(
        [string]$ParentPath,
        [string]$ChildPath
    )

    $parentFull = [System.IO.Path]::GetFullPath((Convert-ToNativePath -Path $ParentPath -AllowMissing)).TrimEnd('\')
    $childFull = [System.IO.Path]::GetFullPath((Convert-ToNativePath -Path $ChildPath -AllowMissing))
    return $childFull.StartsWith($parentFull + '\', [System.StringComparison]::OrdinalIgnoreCase) -or
           $childFull.Equals($parentFull, [System.StringComparison]::OrdinalIgnoreCase)
}

function Reset-BackupRoot {
    if (Test-Path -LiteralPath $BackupRoot) {
        if (-not (Test-IsChildPath -ParentPath $BaseDir -ChildPath $BackupRoot)) {
            throw (Format-UiText 'BackupPathSafetyFailed' $BackupRoot)
        }

        Remove-Item -LiteralPath $BackupRoot -Recurse -Force
    }

    New-Item -ItemType Directory -Path $BackupRoot | Out-Null
    New-Item -ItemType Directory -Path $PrintersRoot | Out-Null
    New-Item -ItemType Directory -Path $DriversRoot | Out-Null
}

function Show-SelectionWindow {
    param(
        [string[]]$Options,
        [string]$Title,
        [string]$Prompt = $(Get-UiText -Key 'SelectItemPrompt')
    )

    $form = New-Object System.Windows.Forms.Form
    $form.Text = $Title
    $form.Size = New-Object System.Drawing.Size(460, 520)
    $form.StartPosition = 'CenterScreen'
    $form.MinimizeBox = $false
    $form.MaximizeBox = $false

    $label = New-Object System.Windows.Forms.Label
    $label.Text = $Prompt
    $label.Location = New-Object System.Drawing.Point(10, 12)
    $label.Size = New-Object System.Drawing.Size(420, 36)
    $form.Controls.Add($label)

    $listBox = New-Object System.Windows.Forms.ListBox
    $listBox.Location = New-Object System.Drawing.Point(10, 50)
    $listBox.Size = New-Object System.Drawing.Size(420, 350)
    foreach ($option in @($Options)) {
        [void]$listBox.Items.Add("$option")
    }
    $form.Controls.Add($listBox)

    $okButton = New-Object System.Windows.Forms.Button
    $okButton.Text = Get-UiText -Key 'Ok'
    $okButton.Location = New-Object System.Drawing.Point(120, 420)
    $okButton.Size = New-Object System.Drawing.Size(90, 30)
    $okButton.DialogResult = [System.Windows.Forms.DialogResult]::OK
    $form.Controls.Add($okButton)

    $cancelButton = New-Object System.Windows.Forms.Button
    $cancelButton.Text = Get-UiText -Key 'Cancel'
    $cancelButton.Location = New-Object System.Drawing.Point(235, 420)
    $cancelButton.Size = New-Object System.Drawing.Size(90, 30)
    $cancelButton.DialogResult = [System.Windows.Forms.DialogResult]::Cancel
    $form.Controls.Add($cancelButton)

    $form.AcceptButton = $okButton
    $form.CancelButton = $cancelButton

    $dialogResult = $form.ShowDialog()
    if ($dialogResult -ne [System.Windows.Forms.DialogResult]::OK) {
        return $null
    }

    return $listBox.SelectedItem
}

function Show-ActionMenu {
    $form = New-Object System.Windows.Forms.Form
    $form.Text = Get-UiText -Key 'ActionTitle'
    $form.Size = New-Object System.Drawing.Size(420, 250)
    $form.StartPosition = 'CenterScreen'
    $form.MinimizeBox = $false
    $form.MaximizeBox = $false

    $label = New-Object System.Windows.Forms.Label
    $label.Text = Get-UiText -Key 'ActionPrompt'
    $label.Location = New-Object System.Drawing.Point(25, 20)
    $label.Size = New-Object System.Drawing.Size(300, 25)
    $label.Font = New-Object System.Drawing.Font('Microsoft JhengHei UI', 10)
    $form.Controls.Add($label)

    $backupButton = New-Object System.Windows.Forms.Button
    $backupButton.Text = $UiBackupButtonText
    $backupButton.Location = New-Object System.Drawing.Point(40, 70)
    $backupButton.Size = New-Object System.Drawing.Size(320, 40)
    $backupButton.Add_Click({
        $form.Tag = 'Backup'
        $form.Close()
    })
    $form.Controls.Add($backupButton)

    $restoreButton = New-Object System.Windows.Forms.Button
    $restoreButton.Text = $UiRestoreButtonText
    $restoreButton.Location = New-Object System.Drawing.Point(40, 120)
    $restoreButton.Size = New-Object System.Drawing.Size(320, 40)
    $restoreButton.Add_Click({
        $form.Tag = 'Restore'
        $form.Close()
    })
    $form.Controls.Add($restoreButton)

    $cancelButton = New-Object System.Windows.Forms.Button
    $cancelButton.Text = $UiCancelText
    $cancelButton.Location = New-Object System.Drawing.Point(155, 170)
    $cancelButton.Size = New-Object System.Drawing.Size(90, 30)
    $cancelButton.Add_Click({
        $form.Tag = $null
        $form.Close()
    })
    $form.Controls.Add($cancelButton)

    [void]$form.ShowDialog()
    return $form.Tag
}

function Test-IsExcludedPrinter {
    param([object]$Printer)

    $name = [string](Get-ObjectValue -InputObject $Printer -PropertyName 'Name' -DefaultValue '')
    $driverName = [string](Get-ObjectValue -InputObject $Printer -PropertyName 'DriverName' -DefaultValue '')
    $portName = [string](Get-ObjectValue -InputObject $Printer -PropertyName 'PortName' -DefaultValue '')

    $virtualPatterns = @(
        'Microsoft Print to PDF'
        'Microsoft XPS Document Writer'
        'OneNote'
        'Adobe PDF'
        '\bFax\b'
        '\bPDF\b'
        '\bXPS\b'
    )

    foreach ($pattern in $virtualPatterns) {
        if ($name -match $pattern -or $driverName -match $pattern) {
            return $true
        }
    }

    $virtualPortPatterns = @(
        '^PORTPROMPT:'
        '^FILE:'
        '^NUL:'
        '^XPSPORT:'
        '^SHRFAX:'
        '^FAX:'
        '^PROMPT:'
    )

    foreach ($pattern in $virtualPortPatterns) {
        if ($portName -match $pattern) {
            return $true
        }
    }

    return $false
}

function Get-TargetPrinters {
    $allPrinters = @(Get-Printer | Sort-Object Name)
    $selectedPrinters = @()
    $skippedPrinters = @()

    foreach ($printer in $allPrinters) {
        if (Test-IsExcludedPrinter -Printer $printer) {
            $skippedPrinters += $printer
        } else {
            $selectedPrinters += $printer
        }
    }

    return [pscustomobject]@{
        Selected = $selectedPrinters
        Skipped  = $skippedPrinters
    }
}

function Get-DefaultPrinterName {
    $defaultPrinter = Get-CimInstance Win32_Printer | Where-Object { $_.Default } | Select-Object -First 1
    if ($null -eq $defaultPrinter) {
        return $null
    }

    return $defaultPrinter.Name
}

function Normalize-UsbPnpDeviceId {
    param([string]$PnpDeviceId)

    if ([string]::IsNullOrWhiteSpace($PnpDeviceId)) {
        return $null
    }

    return (($PnpDeviceId -replace 'USB\d+$', '').Trim()).ToUpperInvariant()
}

function Get-PrinterWmiSnapshot {
    param([string]$PrinterName)

    $printer = Get-CimInstance Win32_Printer | Where-Object { $_.Name -eq $PrinterName } | Select-Object -First 1
    if ($null -eq $printer) {
        return $null
    }

    return [pscustomobject]@{
        Name        = $printer.Name
        DriverName  = $printer.DriverName
        PortName    = $printer.PortName
        PnpDeviceId = $printer.PNPDeviceID
        UsbPnpKey   = Normalize-UsbPnpDeviceId -PnpDeviceId $printer.PNPDeviceID
    }
}

function Get-PrinterPortSnapshot {
    param([string]$PortName)

    $port = Get-PrinterPort -Name $PortName -ErrorAction Stop
    $hostAddress = Get-FirstValue -InputObject $port -PropertyNames @('PrinterHostAddress', 'HostAddress')
    $queueName = Get-FirstValue -InputObject $port -PropertyNames @('Queue', 'QueueName', 'LprQueueName')
    $portNumber = Get-FirstValue -InputObject $port -PropertyNames @('PortNumber')
    $snmpIndex = Get-FirstValue -InputObject $port -PropertyNames @('SNMP', 'SNMPIndex')
    $snmpEnabled = Get-FirstValue -InputObject $port -PropertyNames @('SNMPEnabled')
    $snmpCommunity = Get-FirstValue -InputObject $port -PropertyNames @('SNMPCommunity')
    $protocol = Get-FirstValue -InputObject $port -PropertyNames @('Protocol')
    $byteCounting = Get-FirstValue -InputObject $port -PropertyNames @('LprByteCounting', 'LprByteCountingEnabled')
    $monitorName = Get-FirstValue -InputObject $port -PropertyNames @('PortMonitor', 'MonitorName')

    $kind = 'Other'
    if ($PortName -match '^USB\d+$') {
        $kind = 'USB'
    } elseif ($PortName -match '^WSD') {
        $kind = 'WSD'
    } elseif ($PortName -match '^LPT\d+:$') {
        $kind = 'LPT'
    } elseif ($queueName) {
        $kind = 'LPR'
    } elseif ($hostAddress -or $PortName -match '^IP_') {
        $kind = 'TCP'
    } elseif ($monitorName -match 'Local Port') {
        $kind = 'Local'
    }

    if (-not $hostAddress -and $PortName -match '^IP_(.+)$') {
        $hostAddress = $Matches[1]
    }

    return [pscustomobject]@{
        Name               = $PortName
        Kind               = $kind
        PrinterHostAddress = $hostAddress
        QueueName          = $queueName
        PortNumber         = $portNumber
        SNMPEnabled        = $snmpEnabled
        SNMPIndex          = $snmpIndex
        SNMPCommunity      = $snmpCommunity
        Protocol           = $protocol
        LprByteCounting    = $byteCounting
        PortMonitor        = $monitorName
    }
}

function Copy-DriverPackage {
    param(
        [string]$DriverName,
        [hashtable]$DriverCache
    )

    if ($DriverCache.ContainsKey($DriverName)) {
        return $DriverCache[$DriverName]
    }

    $driver = Get-PrinterDriver -Name $DriverName -ErrorAction Stop | Select-Object -First 1
    $infPath = Convert-ToNativePath -Path (Get-ObjectValue -InputObject $driver -PropertyName 'InfPath')
    if ([string]::IsNullOrWhiteSpace($infPath) -or -not (Test-Path -LiteralPath $infPath)) {
        throw (Format-UiText 'DriverInfNotFound' $DriverName)
    }

    $sourceFolder = Split-Path -Path $infPath -Parent
    $sourceLeaf = Split-Path -Path $sourceFolder -Leaf
    $targetFolder = Join-Path $DriversRoot (Get-SafeFileName -Name ("{0}__{1}" -f $DriverName, $sourceLeaf))

    if (Test-Path -LiteralPath $targetFolder) {
        Remove-Item -LiteralPath $targetFolder -Recurse -Force
    }

    Copy-Item -LiteralPath $sourceFolder -Destination $targetFolder -Recurse -Force

    $copiedInf = Join-Path $targetFolder (Split-Path -Path $infPath -Leaf)
    if (-not (Test-Path -LiteralPath $copiedInf)) {
        $fallbackInf = Get-ChildItem -LiteralPath $targetFolder -Filter '*.inf' -Recurse | Select-Object -First 1
        if ($null -eq $fallbackInf) {
            throw (Format-UiText 'DriverInfMissingInFolder' $DriverName)
        }
        $copiedInf = $fallbackInf.FullName
    }

    $driverInfo = [pscustomobject]@{
        Name            = $DriverName
        RelativeInfPath = Get-RelativePath -BasePath $BackupRoot -TargetPath $copiedInf
        RelativeFolder  = Get-RelativePath -BasePath $BackupRoot -TargetPath $targetFolder
    }

    $DriverCache[$DriverName] = $driverInfo
    return $driverInfo
}

function Export-PrinterSettings {
    param(
        [string]$PrinterName,
        [string]$SettingsPath
    )

    $nativeSettingsPath = Convert-ToNativePath -Path $SettingsPath -AllowMissing
    $settingsFolder = Split-Path -Path $nativeSettingsPath -Parent
    if (-not (Test-Path -LiteralPath $settingsFolder)) {
        New-Item -ItemType Directory -Path $settingsFolder -Force | Out-Null
    }

    $localSettingsPath = New-LocalTempPath -Category 'ExportSettings' -LeafName ((Get-SafeFileName -Name $PrinterName) + '.dat')

    [void](Invoke-PrintUiEntry -Arguments @(
        'printui.dll,PrintUIEntry'
        '/Ss'
        ('/n "{0}"' -f $PrinterName)
        ('/a "{0}"' -f $localSettingsPath)
        'm'
        'u'
    ))

    if (-not (Test-Path -LiteralPath $localSettingsPath)) {
        throw (Format-UiText 'ExportSettingsFailed' $PrinterName)
    }

    Copy-Item -LiteralPath $localSettingsPath -Destination $nativeSettingsPath -Force
}

function Import-PrinterSettings {
    param(
        [string]$PrinterName,
        [string]$SettingsPath
    )

    $nativeSettingsPath = Convert-ToNativePath -Path $SettingsPath
    if (-not (Test-Path -LiteralPath $nativeSettingsPath)) {
        return
    }

    $localSettingsPath = Stage-FileForLocalUse -SourcePath $nativeSettingsPath -Category 'ImportSettings'

    [void](Invoke-PrintUiEntry -Arguments @(
        'printui.dll,PrintUIEntry'
        '/Sr'
        ('/n "{0}"' -f $PrinterName)
        ('/a "{0}"' -f $localSettingsPath)
        'm'
        'u'
        'r'
        'p'
    ))
}

function Backup-LocalPrinters {
    $inventory = Get-TargetPrinters
    $targetPrinters = @($inventory.Selected)
    $skippedPrinters = @($inventory.Skipped)

    if ($targetPrinters.Count -eq 0) {
        [void](Show-MsgBox -Text (Get-UiText 'BackupNoPrinters') -Title (Get-UiText 'BackupErrorTitle') -Icon Error)
        return
    }

    $previewLines = @()
    $previewLines += (Format-UiText 'BackupTargets' $targetPrinters.Count)
    $previewLines += ''
    $previewLines += ($targetPrinters | ForEach-Object { '- ' + $_.Name })

    if ($skippedPrinters.Count -gt 0) {
        $previewLines += ''
        $previewLines += (Format-UiText 'SkippedVirtuals' $skippedPrinters.Count)
        $previewLines += ($skippedPrinters | ForEach-Object { '- ' + $_.Name })
    }

    $previewLines += ''
    $previewLines += (Get-UiText 'ContinueOverwrite')

    $confirm = Show-MsgBox -Text ($previewLines -join "`r`n") -Title (Get-UiText 'BackupConfirmTitle') -Buttons YesNo -Icon Question
    if ($confirm -ne [System.Windows.Forms.DialogResult]::Yes) {
        return
    }

    Reset-BackupRoot

    $driverCache = @{}
    $manifestPrinters = @()
    $failedPrinters = @()
    $warningPrinters = @()
    $defaultPrinterName = Get-DefaultPrinterName
    $progressWindow = $null

    try {
        $progressWindow = New-ProgressWindow `
            -Title (Get-UiText 'BackupProgressTitle') `
            -Maximum $targetPrinters.Count `
            -StatusText (Get-UiText 'BackupProgressStarting') `
            -DetailText ''

        for ($printerIndex = 0; $printerIndex -lt $targetPrinters.Count; $printerIndex++) {
            $printer = $targetPrinters[$printerIndex]
            Update-ProgressWindow `
                -ProgressWindow $progressWindow `
                -Value $printerIndex `
                -StatusText (Format-UiText 'BackupProgressCurrent' ($printerIndex + 1), $targetPrinters.Count) `
                -DetailText (Format-UiText 'BackupProgressDetail' $printer.Name)

            try {
                $driverInfo = Copy-DriverPackage -DriverName $printer.DriverName -DriverCache $driverCache
                $folderName = '{0:D2}_{1}' -f ($manifestPrinters.Count + 1), (Get-SafeFileName -Name $printer.Name)
                $printerFolder = Join-Path $PrintersRoot $folderName
                New-Item -ItemType Directory -Path $printerFolder | Out-Null

                $settingsPath = Join-Path $printerFolder 'settings.dat'
                $settingsRelativePath = $null
                $settingsExportError = $null
                try {
                    Export-PrinterSettings -PrinterName $printer.Name -SettingsPath $settingsPath
                    $settingsRelativePath = Get-RelativePath -BasePath $BackupRoot -TargetPath $settingsPath
                } catch {
                    $settingsExportError = $_.Exception.Message
                    $warningPrinters += (Format-UiText 'SettingsNotExported' $printer.Name, $settingsExportError)
                }

                $manifestPrinters += [pscustomobject]@{
                    Name            = $printer.Name
                    DriverName      = $printer.DriverName
                    PortName        = $printer.PortName
                    Shared          = [bool](Get-ObjectValue -InputObject $printer -PropertyName 'Shared' -DefaultValue $false)
                    ShareName       = Get-ObjectValue -InputObject $printer -PropertyName 'ShareName'
                    Comment         = Get-ObjectValue -InputObject $printer -PropertyName 'Comment'
                    Location        = Get-ObjectValue -InputObject $printer -PropertyName 'Location'
                    Datatype        = Get-ObjectValue -InputObject $printer -PropertyName 'Datatype'
                    PrintProcessor  = Get-ObjectValue -InputObject $printer -PropertyName 'PrintProcessor'
                    Published       = Get-ObjectValue -InputObject $printer -PropertyName 'Published'
                    KeepPrintedJobs = Get-ObjectValue -InputObject $printer -PropertyName 'KeepPrintedJobs'
                    RenderingMode   = Get-ObjectValue -InputObject $printer -PropertyName 'RenderingMode'
                    IsDefault       = ($printer.Name -eq $defaultPrinterName)
                    Driver          = $driverInfo
                    Port            = Get-PrinterPortSnapshot -PortName $printer.PortName
                    PrinterWmi      = Get-PrinterWmiSnapshot -PrinterName $printer.Name
                    SettingsFile    = $settingsRelativePath
                    SettingsWarning = $settingsExportError
                }
            } catch {
                $failedPrinters += ('{0}: {1}' -f $printer.Name, $_.Exception.Message)
            } finally {
                Update-ProgressWindow `
                    -ProgressWindow $progressWindow `
                    -Value ($printerIndex + 1) `
                    -StatusText (Format-UiText 'BackupProgressCurrent' ($printerIndex + 1), $targetPrinters.Count) `
                    -DetailText (Format-UiText 'BackupProgressDetail' $printer.Name)
            }
        }

        Update-ProgressWindow `
            -ProgressWindow $progressWindow `
            -Value $targetPrinters.Count `
            -StatusText (Get-UiText 'BackupProgressFinalize') `
            -DetailText ''

        $manifest = [pscustomobject]@{
            BackupTime         = (Get-Date).ToString('yyyy-MM-dd HH:mm:ss')
            ComputerName       = $env:COMPUTERNAME
            DefaultPrinterName = $defaultPrinterName
            PrinterCount       = $manifestPrinters.Count
            SkippedPrinters    = @($skippedPrinters | ForEach-Object { $_.Name })
            Printers           = $manifestPrinters
        }

        $manifest | ConvertTo-Json -Depth 8 | Set-Content -LiteralPath $ManifestPath -Encoding UTF8
    } finally {
        Close-ProgressWindow -ProgressWindow $progressWindow
    }

    $messageLines = @()
    $messageLines += (Get-UiText 'BackupFinished')
    $messageLines += (Format-UiText 'SuccessCount' $manifestPrinters.Count)
    $messageLines += (Format-UiText 'SkippedCount' $skippedPrinters.Count)
    $messageLines += (Format-UiText 'BackupFolder' $BackupRoot)

    if ($warningPrinters.Count -gt 0) {
        $messageLines += ''
        $messageLines += (Format-UiText 'WarningsCount' $warningPrinters.Count)
        $messageLines += $warningPrinters
    }

    if ($failedPrinters.Count -gt 0) {
        $messageLines += ''
        $messageLines += (Format-UiText 'FailedCount' $failedPrinters.Count)
        $messageLines += $failedPrinters
    }

    [void](Show-MsgBox -Text ($messageLines -join "`r`n") -Title (Get-UiText 'BackupCompleteTitle'))
}

function Resolve-UsbPortName {
    param(
        [string]$PreferredPortName,
        [object]$PrinterInfo
    )

    $usbPorts = @(Get-PrinterPort | Where-Object { $_.Name -match '^USB\d+$' } | Sort-Object Name)
    if ($usbPorts.Count -eq 0) {
        throw (Get-UiText 'NoUsbPorts')
    }

    $currentUsbPrinters = @(
        Get-CimInstance Win32_Printer |
        Where-Object { $_.PortName -match '^USB\d+$' } |
        Select-Object Name, DriverName, PortName, PNPDeviceID
    )

    $backupPnpKey = $null
    if ($null -ne $PrinterInfo.PrinterWmi) {
        $backupPnpKey = Normalize-UsbPnpDeviceId -PnpDeviceId $PrinterInfo.PrinterWmi.PnpDeviceId
    }

    if ($backupPnpKey) {
        $matchedByPnp = $currentUsbPrinters |
            Where-Object { (Normalize-UsbPnpDeviceId -PnpDeviceId $_.PNPDeviceID) -eq $backupPnpKey } |
            Select-Object -First 1
        if ($matchedByPnp) {
            return $matchedByPnp.PortName
        }
    }

    $matchedByName = $currentUsbPrinters |
        Where-Object { $_.Name -eq $PrinterInfo.Name } |
        Select-Object -First 1
    if ($matchedByName) {
        return $matchedByName.PortName
    }

    $matchedByDriver = $currentUsbPrinters |
        Where-Object { $_.DriverName -eq $PrinterInfo.DriverName } |
        Select-Object -First 1
    if ($matchedByDriver) {
        return $matchedByDriver.PortName
    }

    $usedPorts = @($currentUsbPrinters | ForEach-Object { $_.PortName } | Where-Object { $_ })
    $freePorts = @($usbPorts | Where-Object { $usedPorts -notcontains $_.Name })
    if ($freePorts.Count -eq 1) {
        return $freePorts[0].Name
    }

    if ($usbPorts.Count -eq 1) {
        return $usbPorts[0].Name
    }

    if ($PreferredPortName) {
        $preferredPort = $usbPorts | Where-Object { $_.Name -eq $PreferredPortName } | Select-Object -First 1
        if ($preferredPort -and $usedPorts -notcontains $PreferredPortName) {
            return $PreferredPortName
        }
    }

    $selected = Show-SelectionWindow `
        -Options ($usbPorts | ForEach-Object { $_.Name }) `
        -Title (Get-UiText 'UsbPortTitle') `
        -Prompt (Get-UiText 'UsbPortPrompt')
    if ($selected) {
        return $selected
    }

    throw (Get-UiText 'UsbPortCancelled')
}

function Resolve-ExistingPortSelection {
    param([string]$Prompt)

    $portNames = @(Get-PrinterPort | Sort-Object Name | ForEach-Object { $_.Name })
    if ($portNames.Count -eq 0) {
        throw (Get-UiText 'NoPorts')
    }

    $selected = Show-SelectionWindow -Options $portNames -Title (Get-UiText 'PortTitle') -Prompt $Prompt
    if (-not $selected) {
        throw (Get-UiText 'PortCancelled')
    }

    return $selected
}

function Ensure-PrinterPort {
    param([object]$PortInfo)

    if ($null -eq $PortInfo) {
        throw (Get-UiText 'MissingPortInfo')
    }

    $portName = $PortInfo.Name

    switch ($PortInfo.Kind) {
        'USB' {
            return Resolve-UsbPortName -PreferredPortName $portName -PrinterInfo $PortInfo.OwnerPrinter
        }
        'TCP' {
            if ($portName -and (Get-PrinterPort -Name $portName -ErrorAction SilentlyContinue)) {
                return $portName
            }

            if (-not $PortInfo.PrinterHostAddress) {
                throw (Format-UiText 'TcpMissingHost' $portName)
            }

            $params = @{
                Name               = $portName
                PrinterHostAddress = $PortInfo.PrinterHostAddress
            }

            if ($PortInfo.PortNumber) {
                $params.PortNumber = [uint32]$PortInfo.PortNumber
            }

            if ($PortInfo.SNMPEnabled -and $PortInfo.SNMPIndex) {
                $params.SNMP = [uint32]$PortInfo.SNMPIndex
            }

            if ($PortInfo.SNMPCommunity) {
                $params.SNMPCommunity = [string]$PortInfo.SNMPCommunity
            }

            Add-PrinterPort @params
            return $portName
        }
        'LPR' {
            if ($portName -and (Get-PrinterPort -Name $portName -ErrorAction SilentlyContinue)) {
                return $portName
            }

            if (-not $PortInfo.PrinterHostAddress -or -not $PortInfo.QueueName) {
                throw (Format-UiText 'LprIncomplete' $portName)
            }

            $params = @{
                Name           = $portName
                LprHostAddress = $PortInfo.PrinterHostAddress
                LprQueueName   = $PortInfo.QueueName
            }

            if ($PortInfo.LprByteCounting) {
                $params.LprByteCounting = $true
            }

            if ($PortInfo.SNMPEnabled -and $PortInfo.SNMPIndex) {
                $params.SNMP = [uint32]$PortInfo.SNMPIndex
            }

            if ($PortInfo.SNMPCommunity) {
                $params.SNMPCommunity = [string]$PortInfo.SNMPCommunity
            }

            Add-PrinterPort @params
            return $portName
        }
        'WSD' {
            if ($portName -and (Get-PrinterPort -Name $portName -ErrorAction SilentlyContinue)) {
                return $portName
            }

            return Resolve-ExistingPortSelection -Prompt (Format-UiText 'WsdPortMissing' $portName)
        }
        default {
            if ($portName -match '^USB\d+$') {
                return Resolve-UsbPortName -PreferredPortName $portName -PrinterInfo $PortInfo.OwnerPrinter
            }

            if ($portName -and (Get-PrinterPort -Name $portName -ErrorAction SilentlyContinue)) {
                return $portName
            }

            if ($PortInfo.PrinterHostAddress) {
                $params = @{
                    Name               = $portName
                    PrinterHostAddress = $PortInfo.PrinterHostAddress
                }
                Add-PrinterPort @params
                return $portName
            }

            return Resolve-ExistingPortSelection -Prompt (Format-UiText 'PortMissing' $portName)
        }
    }
}

function Install-DriverFromBackup {
    param([object]$DriverInfo)

    if (Get-PrinterDriver -Name $DriverInfo.Name -ErrorAction SilentlyContinue) {
        return
    }

    $infPath = Convert-ToNativePath -Path (Join-Path $BackupRoot $DriverInfo.RelativeInfPath)
    if (-not (Test-Path -LiteralPath $infPath)) {
        throw (Format-UiText 'BackupInfMissing' $DriverInfo.RelativeInfPath)
    }

    $sourceDriverFolder = Split-Path -Path $infPath -Parent
    $localDriverFolder = Stage-FolderForLocalUse -SourceFolder $sourceDriverFolder -Category 'DriverInstall'
    $localInfPath = Join-Path $localDriverFolder (Split-Path -Path $infPath -Leaf)

    if (-not (Test-Path -LiteralPath $localInfPath)) {
        $fallbackInf = Get-ChildItem -LiteralPath $localDriverFolder -Filter '*.inf' -Recurse | Select-Object -First 1
        if ($null -eq $fallbackInf) {
            throw (Format-UiText 'BackupInfMissing' $DriverInfo.RelativeInfPath)
        }
        $localInfPath = $fallbackInf.FullName
    }

    $installProcess = Invoke-PrintUiEntry -Arguments @(
        'printui.dll,PrintUIEntry'
        '/ia'
        ('/m "{0}"' -f $DriverInfo.Name)
        ('/f "{0}"' -f $localInfPath)
    ) -IgnoreExitCode

    if ($installProcess.ExitCode -ne 0) {
        $infFiles = @(Get-ChildItem -LiteralPath $localDriverFolder -Filter '*.inf' -Recurse)
        foreach ($infFile in $infFiles) {
            [void](Start-Process -FilePath 'pnputil.exe' `
                -ArgumentList @('/add-driver', $infFile.FullName, '/install') `
                -PassThru -Wait -NoNewWindow)
        }
    }

    Start-Sleep -Seconds 1

    if (-not (Get-PrinterDriver -Name $DriverInfo.Name -ErrorAction SilentlyContinue)) {
        throw (Format-UiText 'DriverInstallFailed' $DriverInfo.Name)
    }
}

function Apply-PrinterProperties {
    param([object]$PrinterInfo)

    $setParams = @{
        Name   = $PrinterInfo.Name
        Shared = [bool]$PrinterInfo.Shared
    }

    if ($PrinterInfo.ShareName) {
        $setParams.ShareName = [string]$PrinterInfo.ShareName
    }

    if ($PrinterInfo.Comment) {
        $setParams.Comment = [string]$PrinterInfo.Comment
    }

    if ($PrinterInfo.Location) {
        $setParams.Location = [string]$PrinterInfo.Location
    }

    if ($PrinterInfo.Datatype) {
        $setParams.Datatype = [string]$PrinterInfo.Datatype
    }

    if ($PrinterInfo.PrintProcessor) {
        $setParams.PrintProcessor = [string]$PrinterInfo.PrintProcessor
    }

    if ($null -ne $PrinterInfo.Published) {
        $setParams.Published = [bool]$PrinterInfo.Published
    }

    if ($null -ne $PrinterInfo.KeepPrintedJobs) {
        $setParams.KeepPrintedJobs = [bool]$PrinterInfo.KeepPrintedJobs
    }

    if ($PrinterInfo.RenderingMode) {
        $setParams.RenderingMode = $PrinterInfo.RenderingMode
    }

    Set-Printer @setParams | Out-Null
}

function Restore-LocalPrinters {
    if (-not (Test-Path -LiteralPath $ManifestPath)) {
        [void](Show-MsgBox -Text (Format-UiText 'ManifestMissing' $ManifestPath) -Title (Get-UiText 'RestoreErrorTitle') -Icon Error)
        return
    }

    $manifest = Get-Content -LiteralPath $ManifestPath -Raw -Encoding UTF8 | ConvertFrom-Json
    $printers = @($manifest.Printers)
    if ($printers.Count -eq 0) {
        [void](Show-MsgBox -Text (Get-UiText 'NoRestorePrinters') -Title (Get-UiText 'RestoreErrorTitle') -Icon Error)
        return
    }

    $messageLines = @()
    $messageLines += (Format-UiText 'RestoreTargets' $printers.Count)
    $messageLines += ''
    $messageLines += ($printers | ForEach-Object { '- ' + $_.Name })
    $messageLines += ''
    $messageLines += (Get-UiText 'ExistingPrinterNote')

    $confirm = Show-MsgBox -Text ($messageLines -join "`r`n") -Title (Get-UiText 'RestoreConfirmTitle') -Buttons YesNo -Icon Question
    if ($confirm -ne [System.Windows.Forms.DialogResult]::Yes) {
        return
    }

    $restoreFailures = @()
    $defaultPrinterName = $null
    $driverNamesDone = @{}
    $progressWindow = $null

    if ($manifest.PSObject.Properties['DefaultPrinterName']) {
        $defaultPrinterName = [string]$manifest.DefaultPrinterName
    }

    try {
        $progressWindow = New-ProgressWindow `
            -Title (Get-UiText 'RestoreProgressTitle') `
            -Maximum $printers.Count `
            -StatusText (Get-UiText 'RestoreProgressStarting') `
            -DetailText ''

        for ($printerIndex = 0; $printerIndex -lt $printers.Count; $printerIndex++) {
            $printerInfo = $printers[$printerIndex]
            Update-ProgressWindow `
                -ProgressWindow $progressWindow `
                -Value $printerIndex `
                -StatusText (Format-UiText 'RestoreProgressCurrent' ($printerIndex + 1), $printers.Count) `
                -DetailText (Format-UiText 'RestoreProgressDetail' $printerInfo.Name)

            try {
                if (-not $driverNamesDone.ContainsKey($printerInfo.Driver.Name)) {
                    Update-ProgressWindow `
                        -ProgressWindow $progressWindow `
                        -Value $printerIndex `
                        -StatusText (Format-UiText 'RestoreProgressCurrent' ($printerIndex + 1), $printers.Count) `
                        -DetailText (Format-UiText 'RestoreProgressDriver' $printerInfo.Name)
                    Install-DriverFromBackup -DriverInfo $printerInfo.Driver
                    $driverNamesDone[$printerInfo.Driver.Name] = $true
                }

                Update-ProgressWindow `
                    -ProgressWindow $progressWindow `
                    -Value $printerIndex `
                    -StatusText (Format-UiText 'RestoreProgressCurrent' ($printerIndex + 1), $printers.Count) `
                    -DetailText (Format-UiText 'RestoreProgressPort' $printerInfo.Name)

                $portInfo = $printerInfo.Port
                $portInfo | Add-Member -NotePropertyName OwnerPrinter -NotePropertyValue $printerInfo -Force
                $portName = Ensure-PrinterPort -PortInfo $portInfo

                $existingPrinter = Get-Printer -Name $printerInfo.Name -ErrorAction SilentlyContinue
                if ($existingPrinter) {
                    Remove-Printer -Name $printerInfo.Name
                    Start-Sleep -Seconds 1
                }

                Update-ProgressWindow `
                    -ProgressWindow $progressWindow `
                    -Value $printerIndex `
                    -StatusText (Format-UiText 'RestoreProgressCurrent' ($printerIndex + 1), $printers.Count) `
                    -DetailText (Format-UiText 'RestoreProgressCreate' $printerInfo.Name)

                $addParams = @{
                    Name       = $printerInfo.Name
                    DriverName = $printerInfo.Driver.Name
                    PortName   = $portName
                }

                if ($printerInfo.Shared) {
                    $addParams.Shared = $true
                    if ($printerInfo.ShareName) {
                        $addParams.ShareName = [string]$printerInfo.ShareName
                    }
                }

                Add-Printer @addParams | Out-Null
                Start-Sleep -Seconds 1

                Apply-PrinterProperties -PrinterInfo $printerInfo

                if ($printerInfo.SettingsFile) {
                    Update-ProgressWindow `
                        -ProgressWindow $progressWindow `
                        -Value $printerIndex `
                        -StatusText (Format-UiText 'RestoreProgressCurrent' ($printerIndex + 1), $printers.Count) `
                        -DetailText (Format-UiText 'RestoreProgressSettings' $printerInfo.Name)
                    $settingsPath = Join-Path $BackupRoot $printerInfo.SettingsFile
                    Import-PrinterSettings -PrinterName $printerInfo.Name -SettingsPath $settingsPath
                }

                if (-not $defaultPrinterName -and $printerInfo.IsDefault) {
                    $defaultPrinterName = $printerInfo.Name
                }
            } catch {
                $restoreFailures += ('{0}: {1}' -f $printerInfo.Name, $_.Exception.Message)
            } finally {
                Update-ProgressWindow `
                    -ProgressWindow $progressWindow `
                    -Value ($printerIndex + 1) `
                    -StatusText (Format-UiText 'RestoreProgressCurrent' ($printerIndex + 1), $printers.Count) `
                    -DetailText (Format-UiText 'RestoreProgressDetail' $printerInfo.Name)
            }
        }
    } finally {
        Update-ProgressWindow `
            -ProgressWindow $progressWindow `
            -Value $printers.Count `
            -StatusText (Get-UiText 'RestoreProgressFinalize') `
            -DetailText ''
        Close-ProgressWindow -ProgressWindow $progressWindow
    }

    if ($defaultPrinterName -and (Get-Printer -Name $defaultPrinterName -ErrorAction SilentlyContinue)) {
        try {
            (New-Object -ComObject WScript.Network).SetDefaultPrinter($defaultPrinterName)
        } catch {
        }
    }

    Open-PrintersFolder
    Start-Sleep -Milliseconds 700

    $doneCount = $printers.Count - $restoreFailures.Count
    $resultLines = @()
    $resultLines += (Get-UiText 'RestoreFinished')
    $resultLines += (Format-UiText 'SuccessCount' $doneCount)

    if ($restoreFailures.Count -gt 0) {
        $resultLines += (Format-UiText 'FailedCount' $restoreFailures.Count)
        $resultLines += ''
        $resultLines += $restoreFailures
    }

    [void](Show-MsgBox -Text ($resultLines -join "`r`n") -Title (Get-UiText 'RestoreCompleteTitle'))
}

$action = Show-ActionMenu

try {
    switch ($action) {
        'Backup' {
            Backup-LocalPrinters
        }
        'Restore' {
            Restore-LocalPrinters
        }
        default {
        }
    }
} catch {
    [void](Show-MsgBox -Text (Format-UiText 'UnexpectedError' $_.Exception.Message) -Title (Get-UiText 'UnexpectedErrorTitle') -Icon Error)
} finally {
    if ($script:LocalTempRoot -and (Test-Path -LiteralPath $script:LocalTempRoot)) {
        try {
            Remove-Item -LiteralPath $script:LocalTempRoot -Recurse -Force
        } catch {
        }
    }
}
