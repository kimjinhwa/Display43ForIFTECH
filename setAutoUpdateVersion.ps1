# Set console encoding to UTF-8
[Console]::OutputEncoding = [System.Text.Encoding]::UTF8
$OutputEncoding = [System.Text.Encoding]::UTF8

# Read version information from Version.h
$content = Get-Content .\Version.h -Encoding utf8 -TotalCount 1
if ($content -match '#define VERSION "(.*?)"\s*//\s*(.*)')
{
    $version = $matches[1].Trim()
    $comment = $matches[2].Trim()
    
    Write-Host "Version from Version.h: $version" -ForegroundColor Cyan
    Write-Host "Comment: $comment" -ForegroundColor Cyan
    
    # Read and update version.json
    $jsonPath = ".\version.json"
    if (Test-Path $jsonPath) {
        $jsonContent = Get-Content $jsonPath -Raw -Encoding utf8 | ConvertFrom-Json
        $oldVersion = $jsonContent.latest
        
        Write-Host "Current version.json: $oldVersion" -ForegroundColor Yellow
        
        # Update version
        $jsonContent.latest = $version
        
        # Create properly formatted JSON (without extra spaces after colons)
        $jsonString = "{`n    `"latest`": `"$version`",`n    `"filename`": `"$($jsonContent.filename)`"`n}"
        $jsonString | Set-Content $jsonPath -Encoding utf8 -NoNewline
        
        Write-Host "version.json updated to: $version" -ForegroundColor Green
    } else {
        Write-Host "version.json file not found." -ForegroundColor Red
        exit 1
    }
    
    # Set copy paths
    $targetDir = "C:\DevWork\pythonWork\udpBroadCast\dist\ups1p1p"
    $firmwarePath = ".\.pio\build\esp32_Debug\firmware.bin"
    
    # Check file existence
    if (-not (Test-Path $firmwarePath)) {
        Write-Host "firmware.bin file not found: $firmwarePath" -ForegroundColor Red
        exit 1
    }
    
    # Confirm copy operation
    Write-Host "`n========================================" -ForegroundColor Magenta
    Write-Host "Files to copy:" -ForegroundColor Magenta
    Write-Host "  - version.json" -ForegroundColor White
    Write-Host "  - firmware.bin" -ForegroundColor White
    Write-Host "Destination: $targetDir" -ForegroundColor Yellow
    Write-Host "========================================" -ForegroundColor Magenta
    
    $response = Read-Host "`nProceed with copy? (y/n)"
    
    if ($response -eq 'y' -or $response -eq 'Y' -or $response -eq 'yes') {
        # Create target directory if not exists
        if (-not (Test-Path $targetDir)) {
            New-Item -ItemType Directory -Path $targetDir -Force | Out-Null
            Write-Host "Created target directory: $targetDir" -ForegroundColor Green
        }
        
        # Copy files
        try {
            Copy-Item $jsonPath -Destination $targetDir -Force
            Write-Host "[OK] version.json copied" -ForegroundColor Green
            
            Copy-Item $firmwarePath -Destination $targetDir -Force
            Write-Host "[OK] firmware.bin copied" -ForegroundColor Green
            
            Write-Host "`nAll files successfully copied!" -ForegroundColor Green
            
            # Git commit
            $gitDir = "C:\DevWork\pythonWork\udpBroadCast"
            $commitMessage = "ups1p1p version $version. $comment"
            
            Write-Host "`n========================================" -ForegroundColor Magenta
            Write-Host "Git Commit Information:" -ForegroundColor Magenta
            Write-Host "Repository: $gitDir" -ForegroundColor White
            Write-Host "Message: $commitMessage" -ForegroundColor White
            Write-Host "========================================" -ForegroundColor Magenta
            
            $gitResponse = Read-Host "`nProceed with git commit? (y/n)"
            
            if ($gitResponse -eq 'y' -or $gitResponse -eq 'Y' -or $gitResponse -eq 'yes') {
                Push-Location $gitDir
                try {
                    git add -A
                    git commit -am "$commitMessage"
                    Write-Host "`n[OK] Git commit completed!" -ForegroundColor Green
                    
                    # Git push
                    Write-Host "`n========================================" -ForegroundColor Magenta
                    Write-Host "Git Push Information:" -ForegroundColor Magenta
                    Write-Host "Remote: origin" -ForegroundColor White
                    Write-Host "Branch: main" -ForegroundColor White
                    Write-Host "========================================" -ForegroundColor Magenta
                    
                    $pushResponse = Read-Host "`nProceed with git push? (y/n)"
                    
                    if ($pushResponse -eq 'y' -or $pushResponse -eq 'Y' -or $pushResponse -eq 'yes') {
                        git push origin main
                        Write-Host "`n[OK] Git push completed!" -ForegroundColor Green
                    }
                    else {
                        Write-Host "Git push cancelled." -ForegroundColor Yellow
                    }
                }
                catch {
                    Write-Host "Error during git operation: $_" -ForegroundColor Red
                }
                finally {
                    Pop-Location
                }
            }
            else {
                Write-Host "Git commit cancelled." -ForegroundColor Yellow
            }
        }
        catch {
            Write-Host "Error during file copy: $_" -ForegroundColor Red
            exit 1
        }
    }
    else {
        Write-Host "Copy cancelled." -ForegroundColor Yellow
    }
}
else
{
    Write-Host "Version information not found in Version.h file." -ForegroundColor Red
    exit 1
}
