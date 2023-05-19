# myGit
## Intro
A simple version control system, like Git for local control.

The program named gitlet will support the commands: init, add, commit and so on. The specific usage of gitlet is descriped in the file `design_pattern.md`.

## Environment
Language: C++17 Python3(test)

OS: Ubuntu 22.04 LTS

Compiler: g++ 11.3.0

Lib: boost(serialization)


## Run
run the program
```
make gitlet
./gitlet [command]
```

## Test
Run a single test, within the testing subdirectory, run the command
```
cd testing/
make gitlet
python3 tester.py --verbose samples/FILE.IN ...
```
where FILE.IN ... is a list of specific .in files you want to check.


The command
```
python3 tester.py --verbose --keep samples/FILE.in
```
will, in addition, keep around the directory that tester.py produces so that you can examine its files at the point the tester script detected an error. If your test did not error, then the directory will still remain there with the final contents of everything.