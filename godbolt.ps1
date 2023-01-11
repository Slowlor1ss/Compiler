# Declare variables for the paths to the two text files
$codeFile = "C:\School_3rdyr\Gradwork\Project\Compiler\input.cpp"

# Declare a variable for the path to the executable file
$exeFile = "C:\School_3rdyr\Gradwork\Project\Compiler\Build\Debug\compiler.exe"

# Declare a variable for the path to the file where the hash will be saved
$hashFile = "C:\School_3rdyr\Gradwork\Project\Compiler\hashFile.txt"

for(;;) {
    # Create a hash of file1.txt
    $hash1 = Get-FileHash -Path $codeFile -Algorithm MD5

    $savedHash = Get-Content -Path $hashFile

    # Compare the two hashes
    if ($hash1.Hash -ne $savedHash) {
      # If the hashes do not match, run the executable file
      echo $exeFile
      & $exeFile -o ..\..\School_3rdyr\Gradwork\Project\Compiler\out.s -i ..\..\School_3rdyr\Gradwork\Project\Compiler\input.cpp

      # Save the hash of file1.txt to the hash file
      $hash1.Hash | Out-File -FilePath $hashFile -Force
    }

    # Wait x seconds
    Start-Sleep -Seconds 0.6

    # Repeat the process
}
