<p align="center"><img src="https://github.com/16BitWonder/NX-Update-Checker/blob/master/icon.jpg"></p>

# NX-Update-Checker
A small console application that checks a Switch's installed contents against a provided versionlist for newer updates, and then writes the titles with newer updates and the latest version to sdmc:/Available-Updates.txt.

## Usage
- In case of no internet connection, grab the latest versions.txt available from the [nx-versions Repository](https://github.com/16BitWonder/nx-versions/blob/master/versions.txt), and save it in the same directory as NX-Update-Checker.nro
- Run the nro with your favorite variation of the HBMenu
- Watch the pretty text move across the screen
- Profit(?)

## Additional Information
This app was specifically re-written to parse the versions.txt posted in the [nx-versions Repository](https://github.com/16BitWonder/nx-versions), if you provide a versions.txt with even a slightly different format it is likely this application will not parse it correctly.

As of writing (2020/07/14) binaries are built using switch-curl 7.69.1-1, as building with switch-curl 7.69.1-2 will not allow users with blanked cal0 to download versions.txt over the internet.

## Kudos
A big thank you to [blawar](https://github.com/blawar) for maintaining the titledb repository and [simontime](https://github.com/simontime) for maintaining Reswitched's VersionListBot.
Without a nice stream of updated versionlists this wouldn't be feasible.

Another huge thank you to [aedalzotto](https://github.com/aedalzotto), for both implementing the ability to download versions.txt over the internet and fixing my terrible parsing code.
