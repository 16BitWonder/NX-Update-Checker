<p align="center"><img src="https://github.com/16BitWonder/NX-Update-Checker/blob/master/icon.jpg"></p>

# NX-Update-Checker
A small console application that checks a Switch's installed contents against a provided versionlist for newer updates, and then writes the titles with newer updates and the latest version to sdmc:/Available-Updates.txt.

## Usage
- In case of no internet connection, grab the latest versions.txt available from the [nx-versions Repository](https://github.com/16BitWonder/nx-versions/blob/master/versions.txt), and save it in the same directory as NX-Update-Checker.nro
- Run the nro with your favorite variation of the HBMenu
- Watch the pretty text move across the screen
- Profit(?)

## Ignore List
Users can choose to ignore specific game updates and DLC if they choose by creating a file named `ignore.txt` and filling it out with the following structure:
- For each line a single, whole title ID must be entered for either an update or DLC
- ignore.txt must be placed in the same directory as NX-Update-Checker.nro

An example ignore.txt is shown below:
```
0100000000010800
0100E95004039001
01006A800016F002
```
The above ignore.txt would prevent NX-Update-Checker from showing new Super Mario Odyssey updates, Xenoblade 2 Torna DLC Binary updates and Super Smash Bros. Joker DLC Binary updates.

## Logging
Users can choose to produce logs while running the application for debugging purposes by creating an empty file named `log.flag` in the same directory as NX-Update-Checker.nro

Note however, that the application will run much slower while logging all operations it's making.

The logs will be produced at the root of the SD as a file named `NXUC.log`

## Additional Information
This app was specifically re-written to parse the versions.txt posted in the [nx-versions Repository](https://github.com/16BitWonder/nx-versions), if you provide a versions.txt with even a slightly different format it is likely this application will not parse it correctly.

As of writing (2020/07/14) binaries are built using switch-curl 7.69.1-1, as building with switch-curl 7.69.1-2 will not allow users with blanked cal0 to download versions.txt over the internet.

## Kudos
[blawar](https://github.com/blawar) - titledb

[simontime](https://github.com/simontime) - VersionListBot

[aedalzotto](https://github.com/aedalzotto) - Initial downloading code
