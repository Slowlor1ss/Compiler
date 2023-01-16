param(
    [string]$arch = "SKL"
)

$location = "C:/School_3rdyr/Gradwork/Project/Compiler/Output/"
$codeFile = $location + "out.s"
$markerFile = $location + "outWMarkers.s"
$outFile = $location + "outCompiled.exe"

# Add markers
$text = "	movl	`$111, %ebx		# Start marker bytes`n	.byte	0x64, 0x67, 0x90		# Start marker bytes`n`n"
$content = Get-Content -Path $codeFile
$content = $text + (Out-String -InputObject $content)
$text = "`n`n	movl	`$222, %ebx		# End marker bytes`n	.byte	0x64, 0x67, 0x90		# End marker bytes`n"
$content += $text
$content | Set-Content -Path $markerFile

& "C:\cygwin64\bin\bash.exe" -c "gcc $markerFile -o $outFile"

$iacaFile = $location + "iaca.exe"
#echo $iacaFile
& $iacaFile -arch $arch $outFile > ($location + "BenchmarkResult.txt")