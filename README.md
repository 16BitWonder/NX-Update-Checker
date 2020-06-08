<p align="center"><img src="https://github.com/16BitWonder/NX-Update-Checker/blob/master/icon.jpg"></p>

# NX-Update-Checker
A small console application that checks a Switch's installed contents against a provided versionlist for newer updates, and then writes the titles with newer updates and the latest version to sdmc:/Available-Updates.txt.

## Usage
- Grab the latest versionlist.json available from the [titledb Repository](https://github.com/blawar/titledb/blob/master/versions.txt), and save it in the same directory as NX-Update-Checker.nro
- Run the nro with your favorite variation of the HBMenu
- Watch the pretty text move across the screen
- Profit(?)


## Notice
This app was specifically re-written to parse the versions.txt posted in the titledb Repository, if you provide a versions.txt with even a slightly different format it is likely this application will not parse it correctly.

## Kudos
A big thank you to Blawar, for maintaining the titledb Repository.
Without a nice stream of updated versionlists this wouldn't be feasible.
