# DISKFRISK
[![Build Status](https://travis-ci.org/parkerduckworth/diskfrisk.svg?branch=master)](https://travis-ci.org/parkerduckworth/diskfrisk)

A simple, developer-friendly UNIX shell file search solution.

## Quick Links
* [Code Example](#code-example)
* [Installation](#installation)
* [Detailed Usage](#detailed-usage)
* [Testing](#testing)
* [Verified Build Environments](#verified-build-environments)
* [Other Useful Information](#other-useful-information)
* [Contribute](#contribute)
* [Credits](#credits)

## Motivation
As developers, we spend significant time at the command line. Born of the 
desire to search for files without constantly switching windows, **DISKFRISK** 
enables quick and easy file lookups.  With features such as auto-open, adjustable 
case sensitivity, and pattern matching, searches can be tailored for any need.

## Features
- **Case Sensitivity:** Can be toggled on and off.
- **Target Directory Selection:** Set base directory as needed.
- **Auto-Open:** Automatically open first match upon discovery.
- **Permission Error Display:** Helpful if unsure of file permission settings.
- **Pattern Matching:** GREP it.

## Code Example
To initiate DISKFRISK, enter `dfstart`.
```
USER-MBP:~ user$ dfstart
DISKFRISK INITIATED
____________________________________________
Enter search, or "settings" for more options
DISKFRISK$ 
```

Enter the filename you wish to search.
```
____________________________________________
Enter search, or "settings" for more options
DISKFRISK$ main.c


DISKFRISK -- VERSION 0.0.0


Searching for:            [main.c]
Search type:              Exact match
Case sensitivity:         Off
Search user files:        On
Search system files:      Off
Permission errors:        On
Auto-open:                Off

```
Notice the readout of options displayed when search is executed.  The above settings are default, and can easily be [changed](#detailed-usage).

To exit, simply enter `q`. 
```
____________________________________________
Enter search, or "settings" for more options
DISKFRISK$ q                  
exiting...
USER-MBP:~ user$
```

All options and funcitonality is described in the [Detailed Usage](#detailed-usage) section.

## Installation
1. Clone this repository into your home directory.
2. While in the base project directory, enter `make`.
3. Add `source <path>/diskfrisk/bin/exec.sh` to an open line on whichever shell initialization script you typially use. 
4. Initiate DISKFRISK whenever you want to make a search with `dfstart` from any working directory.

(New users, make sure to replace `<path>` with the absolute path to the cloned repository.)

**Note**: It is convenient to shorten the initiate command to one key, `f` for example. To set a custom initiate command, open `bin/exec.sh` and change the function name:
```sh
function dfstart() {
```
To whatever you prefer (`f` in this example):
```sh
function f() {
```

## Detailed Usage

DISKFRISK was specifically designed with simplicity as its core value. All that is needed to execute a search is the filename or pattern that you wish to query.

Searches can be easily tailored to fit specific needs. To adjust the search settings, simply type `settings` into the DISKFRISK shell prompt.  This command opens `config.json`, listing all options and their current set values. By default, settings are opened using vi.
```json
{
  "auto open": false,
  "pattern match": true,
  "case sensitivity": false,
  "permission errors": true,
  "search user files": true,
  "search system files": false
}
~                                                                               
~                                                                               
~                                                                               
~                                                                               
~                                                                               
~                                                                               
~                                                                               
~                                                                               
~                                                                               
~                                                                               
~                                                                               
~                                                                               
"~/diskfrisk/config.json" 8L, 169C
```
**Note**: If you are unfamiliar with vi, or prefer to adjust settings in your default text editor, enter `settings-ext` instead to open `config.json` in an external window.

___
In addition, settings can be manipulated on the fly.  Option flags may be entered to temporarily adjust the settings for a search. Option flags are entered before the filename/pattern.

Option Flags:

| Syntax  | Purpose                                                |
|---------|--------------------------------------------------------|
| `-C`    | Toggle on case sensitivity (uppercase 'C')             |
| `-h`    | Select home directory as base for search (user files)  |
| `-s`    | Select root directory as base for search (system files)|
| `-o`    | Auto-open first file that matches the query            |
| `-e`    | Display file permission errors                         |
| `-p`    | Search by pattern match (default is exact match)       |

Example: 
- Say you need to open a particular file but you don't remember its extension and you typically don't require the auto-open or pattern match features.  Instead of opening the setttings file, making the adjustments, saving and closing, you may enter your search like so:
```
____________________________________________
Enter search, or "settings" for more options
DISKFRISK$ -op <filename>


DISKFRISK -- VERSION 0.0.0


Searching for:            [filename]
Search type:              Pattern match
Case sensitivity:         Off
Search user files:        On
Search system files:      Off
Permission errors:        On
Auto-open:                On
```
> This shows how options can be combined into a single flag. Options can appear in any order and can be combined together.  If not combined, they must be separated by whitespace.

## Testing
All tests are located in the `tests` folder.
1. cd into `tests`.
2. compile:
```
cc <test_filename>.c -o test_execs/<test_filename> -lcunit
```
3. run:
```
test_execs/<test_filename>
```

## Verified Build Environments

* macOS High Sierra Version 10.13.3
* Ubuntu 18.04.1 LTS 


## Other Useful Information

Included is the useful make utility `dfmake`, located in `src/exec.sh`.  
```
Making DISKFRISK...
cc    -c -o src/main.o src/main.c
cc    -c -o src/diskfrisk.o src/diskfrisk.c
cc    -c -o src/display.o src/display.c
cc    -c -o src/config.o src/config.c
gcc -o bin/find src/main.o src/diskfrisk.o src/display.o src/config.o
Make finished.
USER-MBP:~ user$ 
```
This executes the makefile from any directory, proving very convenient during development and testing.

## Contribute

Contribution Guideline can be found [here](https://github.com/parkerduckworth/diskfrisk/blob/master/CONTRIBUTING.md).
Please feel free to use, share, and extend this project. PR's welcome.

## Credits

Thanks to [Serge Zaitsev](https://zserge.com/) and his [JSMN](https://github.com/zserge/jsmn) JSON parsing library.  

---
[MIT](https://github.com/parkerduckworth/diskfrisk/blob/master/LICENSE) © [Parker Duckworth](https://github.com/parkerduckworth)
