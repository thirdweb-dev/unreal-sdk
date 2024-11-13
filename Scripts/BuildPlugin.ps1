# Powershell script

param (
    [string]$Mode = 'build',
    [string[]]$EngineVersion = 'All',
    [string[]]$Platforms = @('Win64', 'Android', 'Linux', 'LinuxArm64'),
    [string]$Destination = "$env:UserProfile\Downloads\ThirdwebSDK",
    [string]$EnginePath,
    [switch]$Reverse = $false
)

class EngineInstall
{
    [string] $Version
    [string] $Path

    EngineInstall([string]$Version, [string]$Path)
    {
        $this.Version = $Version
        $this.Path = $Path
    }
}

class Config
{
    [EngineInstall[]] $EngineInstalls

    Config()
    {
        $this.EngineInstalls = @()
    }

    [void]
    AddEngineInstall([EngineInstall]$install)
    {
        $this.EngineInstalls += $install
    }

    [bool]
    HasVersion([string]$Version)
    {
        ForEach ($EngineInstall in $this.EngineInstalls)
        {
            if ( $EngineInstall.Version.StartsWith($Version))
            {
                return $true
            }
        }
        return $false
    }

    [string]
    GetVersionPath([string]$Version)
    {
        ForEach ($EngineInstall in $this.EngineInstalls)
        {
            if ( $EngineInstall.Version.StartsWith($Version))
            {
                return $EngineInstall.Path
            }
        }
        return ""
    }

    [void]
    AddVersion([string]$Version, [string]$Path)
    {
        if ( $this.hasVersion($Version))
        {
            Write-Host $Version already saved for $Path
            exit
        }
        $newEngineInstall = [EngineInstall]::new($Version, $Path)
        $this.AddEngineInstall($newEngineInstall)
    }

    [string[]]
    GetAllVersions([bool]$Reverse)
    {
        $Installs = $this.EngineInstalls;
        if ($Reverse)
        {
            [array]::Reverse($Installs)
        }
        $Result = @()
        foreach ($Install in $Installs)
        {
            $Result.Add($Install.Version)
        }
        return $Result
    }
    [string[]]
    GetAllVersions()
    {
        return $this.GetAllVersions($false)
    }
}

$ConfigFile = Join-Path -Path "$env:USERPROFILE" -ChildPath ".build-plugin.json"
$PluginDir = Split-Path -Path $PSScriptRoot -Parent

if (!(Test-Path -Path $ConfigFile))
{
    Write-Host "Creating config file..."
    $DefaultConfig = [Config]::new()
    $DefaultData = $DefaultConfig | ConvertTo-Json
    New-Item -Path $ConfigFile -ItemType "File" -Value $DefaultData
}

# Deserialize the Config from JSON
$jsonContent = Get-Content -Path $ConfigFile | ConvertFrom-Json
$Config = [Config]::new()
$jsonContent.EngineInstalls | ForEach-Object {
    $Config.AddEngineInstall([EngineInstall]::new($_.Version, $_.Path))
}

function SetUERoot
{
    param ([string]$NewRoot)
    if (!($NewRoot))
    {
        Write-Host "SetUERoot: Error: No path set"
        exit
    }
    $CurrentRoot = ue4 root 2> $null
    if ($NewRoot -ne $CurrentRoot)
    {
        ue4 setroot $NewRoot 2> $null
        return
    }
    # Verbose
    # Write-Host "UE Root already set. Skipping..."
}

$ClangFolders = @{
    "5.5" = "v23_clang-18.1.0-rockylinux8"
    "5.4" = "v22_clang-16.0.6-centos7"
    "5.3" = "v22_clang-16.0.6-centos7"
    "5.2" = "v21_clang-15.0.1-centos7"
    "5.1" = "v20_clang-13.0.1-centos7"
    "5.0" = "v19_clang-11.0.1-centos7"
}

function SetClang
{
    param ([string]$Version)

    if ( $Platforms.Contains("Linux"))
    {
        $env:LINUX_MULTIARCH_ROOT = "C:\UnrealToolchains\" + $ClangFolders[$Version] + "\"
        # Verbose
        # Write-Host "LINUX_MULTIARCH_ROOT set to" $env:LINUX_MULTIARCH_ROOT
    }
}

switch ($Mode)
{
    'build' {
        $Versions = $EngineVersion
        if ($EngineVersion -eq "All")
        {
            $Versions = $Config.GetAllVersions($Reverse)
        }
        foreach ($Version in $Versions)
        {
            Write-Host Building Plugin for UE_$Version
            if (!($Config.hasVersion($Version)))
            {
                Write-Host "Error: No engine path saved for $Version"
                continue

            }
            SetUERoot($Config.getVersionPath($Version))
            SetClang($Version)
            $VersionDestination = $Destination + "_" + $Version
            $TargetPlatforms = $Platforms -join "+"

            Start-Process ue4 `
                    -Wait `
                    -NoNewWindow `
                    -WorkingDirectory $PluginDir `
                    -ArgumentList `
                        "package", `
                                "-NoHostPlatform", `
                                "-TargetPlatforms=`"$TargetPlatforms`"", `
                                "-Package=`"$VersionDestination`""
        }
    }
    'save' {
        if ($EngineVersion[0] == "All")
        {
            Write-Host "Error: No engine version specified"
            exit
        }
        $Config.AddVersion($EngineVersion, $EnginePath)
        Write-Host "Saving $EngineVersion as $EnginePath"
        $Config | ConvertTo-Json | Set-Content -Path $ConfigFile
    }
    'list' {
        Write-Host "Engine Versions:"
        foreach ($Install in $Config.EngineInstalls)
        {
            Write-Host $Install.Version - $Install.Path
        }
        exit
    }
    default {
        Write-Host "Invalid mode selected. Please choose either 'build' or 'save'."
        exit
    }
}
