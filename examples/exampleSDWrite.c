/*
    This file is part of GPL_ZXYLib

    GPL_ZXYLib is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <input.h>
#include <stdlib.h>
#include <string.h>

#include "../src/textUtils.h"
#include "../src/esxdos.h"

#define getKey() in_Inkey()

#define BUFFER_SIZE ( 1024 )
uint8_t buffer[ BUFFER_SIZE ];


void main(void) {

    int16_t drive;

    int16_t f;

    uint8_t *filePath = "/escrito.txt";

    uint8_t buffer[258];

    uint16_t numBytes;



    textUtils_64ColumnsMode();
    textUtils_cls();

    textUtils_println( "This example writes a file to SD card. Press a key to start..." );

    waitKey();

    drive = ESXDOS_getDefaultDrive();

    iferror {
        textUtils_println( "SD card not inserted." );
        return;
    }

    textUtils_println( "SD card detected." );

    textUtils_print( "Drive: " );
    textUtils_println_l( drive );

    textUtils_print( "Opening file: " );
    textUtils_println( filePath );

    f = ESXDOS_fopen( filePath, ESXDOS_FILEMODE_WRITE_CREATE, drive );
    iferror {
        textUtils_print( "Could not open file for writing." );
        textUtils_print( "Error code= " );
        textUtils_println_l( f );
        return;
    }

    sprintf( buffer, "This is the content of the file." );
    numBytes = strlen( buffer );

    textUtils_print( "Num bytes to write= " );
    textUtils_println_l( numBytes );

    numBytes = ESXDOS_fwrite( buffer, numBytes, f );
    iferror {
        textUtils_println( "Error writing to the file." );
    }
    else {

        textUtils_print( "Bytes written: " );
        textUtils_println_l( numBytes );

    }

    ESXDOS_fsync( f );
    iferror {
        textUtils_println( "Error doing sync to the file." );
        return;
    }
    textUtils_println( "File sync'ed OK" );

    ESXDOS_fclose( f );
    iferror {
        textUtils_println( "Error closing the file." );
        return;
    }
    textUtils_println( "File closed OK" );

    textUtils_println( "End. Press a key." );
    waitKey();

}
