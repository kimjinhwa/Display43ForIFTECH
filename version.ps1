echo "--connecting.."
$branch_name = git rev-parse --abbrev-ref HEAD
$branch_name = $branch_name+"_"
echo $branch_name 
$commit_hash = git rev-parse --short HEAD
$commit_hash = $commit_hash + "_V1.0"
echo $commit_hash 
$new_version = "/*`nThis is managed by Git commit `nDo not Edit `n*/`n#define version1 `"" + "UPS1P1P_"+$branch_name + $commit_hash+"`""
echo $new_version 
$file_path = "Version.h"
echo $file_path
$contents = Get-Content -Path $file_path
echo $contents 
$updated_contents = $new_version
echo $updated_contents 
$updated_contents  | Set-Content -Path $file_path
$contents = Get-Content -Path $file_path
echo $contents 
