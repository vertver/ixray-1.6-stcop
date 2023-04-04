# Test and set path to 7-Zip
$globalPaths = $env:Path -split ';'
ForEach ($line in $globalPaths) {
    If (!(Test-Path ($line + "\7z.exe"))) {
        $path = Join-Path -Path ${env:ProgramFiles} `
                          -ChildPath "7-Zip\7z.exe"
    }
}

# Getting 3ds Max 6.0 SDK from archive
If (!(Test-Path "sdk\3d_sdk\3dsmax")) {
    $uri = "https://github.com/ixray-team/ixray-1.6-stcop/" + `
           "releases/download/v1.6.02-2/sdk-3dsdk-3dsmax.7z"
    Invoke-WebRequest -Uri $uri `
                      -OutFile "sdk-3dsdk-3dsmax.7z"
    Start-Process -FilePath $path `
                  -ArgumentList "x sdk-3dsdk-3dsmax.7z" `
                  -NoNewWindow -Wait
    Remove-Item "sdk-3dsdk-3dsmax.7z"
}
