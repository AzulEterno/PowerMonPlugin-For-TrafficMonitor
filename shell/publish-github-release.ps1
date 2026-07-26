[CmdletBinding()]
param(
    [Parameter(Mandatory = $true)]
    [ValidatePattern('^v[0-9]+\.[0-9]+\.[0-9]+(?:[-+][0-9A-Za-z.-]+)?$')]
    [string]$Version,

    [ValidateSet('x64', 'Win32', 'ARM64', 'ARM64EC')]
    [string[]]$Platform = @('x64', 'ARM64', 'ARM64EC'),

    [string]$Repository = 'AzulEterno/PowerMonPlugin-For-TrafficMonitor',

    [switch]$Draft,
    [switch]$Prerelease
)

$ErrorActionPreference = 'Stop'

function Require-Command {
    param([string]$Name, [string]$InstallHint)

    if (-not (Get-Command $Name -ErrorAction SilentlyContinue)) {
        throw "$Name is required. $InstallHint"
    }
}

$repoRoot = (Resolve-Path (Join-Path $PSScriptRoot '..')).Path
Set-Location $repoRoot

Require-Command -Name 'git' -InstallHint 'Install Git for Windows, then run this script again.'
Require-Command -Name 'gh' -InstallHint 'Install GitHub CLI from https://cli.github.com/, run ''gh auth login'', then run this script again.'

& gh auth status --hostname github.com
if ($LASTEXITCODE -ne 0) {
    throw 'GitHub CLI is not authenticated. Run ''gh auth login'' and try again.'
}

if ((git status --porcelain).Count -gt 0) {
    throw 'The working tree is not clean. Commit, stash, or discard local changes before creating a release.'
}

$head = (git rev-parse HEAD).Trim()
$remoteMain = (git ls-remote "https://github.com/$Repository.git" refs/heads/main).Split("`t")[0]
if (-not $remoteMain -or $remoteMain -ne $head) {
    throw "GitHub main is not the current commit ($head). Push main to $Repository before releasing."
}

$existingTag = git ls-remote --tags "https://github.com/$Repository.git" "refs/tags/$Version"
if ($existingTag) {
    throw "The tag $Version already exists on GitHub. Choose a new version instead of replacing a published release."
}

$releaseRoot = Join-Path $repoRoot "shell\release\$Version"
if (Test-Path -LiteralPath $releaseRoot) {
    throw "Release directory already exists: $releaseRoot. Remove or rename it before running this script."
}
New-Item -ItemType Directory -Path $releaseRoot -Force | Out-Null

$buildScript = Join-Path $repoRoot 'shell\build-powermon.ps1'
$libraryPath = Join-Path $repoRoot 'PowerMonPlugin\lib\LibreHardwareMonitorLib.dll'
$assets = [System.Collections.Generic.List[string]]::new()

foreach ($targetPlatform in $Platform) {
    & $buildScript -Configuration Release -Platform $targetPlatform -NoPrompt
    if ($LASTEXITCODE -ne 0) {
        throw "Build failed for $targetPlatform."
    }

    $outputCandidates = @(
        (Join-Path $repoRoot "PowerMonPlugin\bin\PowerMonPlugin\Release\$targetPlatform\PowerMonPlugin.dll"),
        (Join-Path $repoRoot "bin\PowerMonPlugin\Release\$targetPlatform\PowerMonPlugin.dll")
    )
    $pluginPath = $outputCandidates | Where-Object { Test-Path -LiteralPath $_ } | Select-Object -First 1
    if (-not $pluginPath) {
        throw "PowerMonPlugin.dll was not found after building $targetPlatform."
    }
    if (-not (Test-Path -LiteralPath $libraryPath)) {
        throw "Required dependency not found: $libraryPath"
    }

    $packageName = "PowerMonPlugin-$Version-$targetPlatform"
    $packageDir = Join-Path $releaseRoot $packageName
    New-Item -ItemType Directory -Path $packageDir -Force | Out-Null
    Copy-Item -LiteralPath $pluginPath -Destination $packageDir
    Copy-Item -LiteralPath $libraryPath -Destination $packageDir

    @(
        'Installation',
        '============',
        'Copy PowerMonPlugin.dll to TrafficMonitor\\plugins.',
        'Copy LibreHardwareMonitorLib.dll next to TrafficMonitor.exe, not to plugins.',
        'Restart TrafficMonitor and check More Functions > Plugin Management.'
    ) | Set-Content -LiteralPath (Join-Path $packageDir 'INSTALL.txt') -Encoding utf8

    $archivePath = Join-Path $releaseRoot "$packageName.zip"
    Compress-Archive -Path (Join-Path $packageDir '*') -DestinationPath $archivePath
    $assets.Add($archivePath)
}

$checksumPath = Join-Path $releaseRoot 'SHA256SUMS.txt'
$assets | ForEach-Object {
    $hash = (Get-FileHash -LiteralPath $_ -Algorithm SHA256).Hash.ToLowerInvariant()
    "$hash  $(Split-Path -Leaf $_)"
} | Set-Content -LiteralPath $checksumPath -Encoding ascii
$assets.Add($checksumPath)

$releaseArgs = [System.Collections.Generic.List[string]]::new()
$releaseArgs.AddRange([string[]]@(
    'release', 'create', $Version,
    '--repo', $Repository,
    '--target', $head,
    '--title', "PowerMon Plugin $Version",
    '--generate-notes'
))
if ($Draft) { $releaseArgs.Add('--draft') }
if ($Prerelease) { $releaseArgs.Add('--prerelease') }
$releaseArgs.AddRange($assets)

& gh @releaseArgs
if ($LASTEXITCODE -ne 0) {
    throw "GitHub Release creation failed. The packaged files remain in $releaseRoot."
}

Write-Host "[OK] Published $Version to https://github.com/$Repository/releases/tag/$Version"
