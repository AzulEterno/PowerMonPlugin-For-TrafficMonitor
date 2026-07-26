[CmdletBinding()]
param(
    [Parameter(Mandatory = $false)]
    [ValidateSet('Debug', 'Release')]
    [string]$Configuration = 'Release',

    [Parameter(Mandatory = $false)]
    [ValidateSet('x64', 'Win32', 'ARM64', 'ARM64EC')]
    [string]$Platform = 'x64',

    [switch]$Clean,
    [switch]$Rebuild,
    [switch]$NoPrompt
)

$ErrorActionPreference = 'Stop'

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
$projectPath = Join-Path $repoRoot 'PowerMonPlugin\PowerMonPlugin.vcxproj'
if (-not (Test-Path -LiteralPath $projectPath)) {
    throw "Project not found: $projectPath"
}

$buildTarget = if ($Rebuild) { 'Rebuild' } elseif ($Clean) { 'Clean' } else { 'Build' }
$msbuildArgs = @(
    '/m',
    '/v:minimal',
    "/t:$buildTarget",
    "/p:Configuration=$Configuration",
    "/p:Platform=$Platform",
    $projectPath
)

$logDir = Join-Path $repoRoot 'shell\logs'
New-Item -ItemType Directory -Path $logDir -Force | Out-Null
$logFile = Join-Path $logDir ([string]::Format('PowerMonPlugin-{0}-{1}.log', $Configuration, $Platform))

$msbuildExe = $null
$msbuildCmd = Get-Command msbuild.exe -ErrorAction SilentlyContinue
if ($msbuildCmd) {
    $msbuildExe = $msbuildCmd.Source
} else {
    $programFilesX86 = [Environment]::GetFolderPath('ProgramFilesX86')
    $programFiles = [Environment]::GetFolderPath('ProgramFiles')

    $vswherePath = Join-Path $programFilesX86 'Microsoft Visual Studio\Installer\vswhere.exe'
    if (-not (Test-Path -LiteralPath $vswherePath)) {
        $vswherePath = Join-Path $programFiles 'Microsoft Visual Studio\Installer\vswhere.exe'
    }
    if (-not (Test-Path -LiteralPath $vswherePath)) {
        throw 'Cannot find vswhere.exe. Install Visual Studio / Build Tools with C++ workload or use a VS Code C++ dev terminal.'
    }

    $vsInstallation = & $vswherePath -latest -products * -requires Microsoft.Component.MSBuild -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if (-not $vsInstallation) {
        throw 'Visual Studio C++ build tools not found.'
    }

    $candidatePaths = @(
        Join-Path $vsInstallation 'MSBuild\Current\Bin\MSBuild.exe'
        Join-Path $vsInstallation 'MSBuild\Bin\MSBuild.exe'
    )

    foreach ($candidate in $candidatePaths) {
        if (Test-Path -LiteralPath $candidate) {
            $msbuildExe = $candidate
            break
        }
    }

    if (-not $msbuildExe) {
        $msbuildExe = Get-ChildItem -Path (Split-Path -Parent $vsInstallation) -Recurse -Filter MSBuild.exe -ErrorAction SilentlyContinue |
            Where-Object { $_.FullName -match 'MSBuild\\Current\\Bin\\MSBuild\\.exe$' } |
            Select-Object -First 1 -ExpandProperty FullName
    }
}

if (-not $msbuildExe) {
    throw 'MSBuild.exe not found. Install Visual Studio build tools or open VS Code Developer terminal.'
}

$stdout = & $msbuildExe @msbuildArgs 2>&1 | Tee-Object -FilePath $logFile -Encoding UTF8
$exitCode = $LASTEXITCODE
if ($exitCode -ne 0) {
    Write-Error "Build failed. ExitCode=$exitCode. See: $logFile"
    if ($stdout) { Write-Output $stdout }
    exit $exitCode
}

if (-not $NoPrompt) {
    Write-Host '[OK] PowerMonPlugin build succeeded.'
    Write-Host ("  Configuration: {0}" -f $Configuration)
    Write-Host ("  Platform:      {0}" -f $Platform)
    Write-Host ("  Output:        {0}" -f (Join-Path $repoRoot "PowerMonPlugin\bin\PowerMonPlugin\$Configuration\$Platform\PowerMonPlugin.dll"))
    Write-Host ("  Log:           {0}" -f $logFile)
}
