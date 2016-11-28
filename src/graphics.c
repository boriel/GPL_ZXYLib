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

#include "graphics.h"
#include <stdlib.h>
#include <spectrum.h>

void setULAPlusMode( uint8_t mode ) {

    outp( 0xFC3B, 64 );
    outp( 0xFD3B, mode );

}

void setULAPlusPaletteRegister( uint8_t reg, uint8_t rrrggbb ) {

    outp( 0xBF3B, reg & 63 );
    outp( 0xFF3B, rrrggbb );

}

void paintGraphic8x8BlockPosition( uint16_t blockPosX, uint16_t blockPosY, uint8_t *graphic ) {

    uint8_t *addr = zx_cyx2saddr( blockPosY, blockPosX );
    uint8_t i;

    for ( i = 0; i < 8; i++ ) {

        *addr = *graphic++;

        addr = zx_saddrpdown( addr );

    }

}

void paintGraphicBlockPosition( uint16_t blockPosX, uint16_t blockPosY, uint16_t numBlocksX, uint16_t numBlocksY, uint8_t *graphic ) {

    uint16_t i;
    uint16_t maxi = blockPosY + numBlocksY;

    while ( numBlocksX > 0 ) {

        for ( i = blockPosY; i < maxi; i++ ) {

            paintGraphic8x8BlockPosition( blockPosX, i, graphic );

            graphic += 8;

        }

        numBlocksX--;
        blockPosX++;
    }

}

void paintGraphic8x8PixelPosition( uint16_t pixelPosX, uint16_t pixelPosY, uint8_t *graphic ) {

    uint8_t mask;
    uint8_t *addr = zx_pxy2saddr( pixelPosX, pixelPosY, &mask );
    uint8_t *addr2;
    uint8_t i;
    uint8_t pixelsGraphic;
    uint8_t pixelsScreen;
    uint16_t pixelPosXBlock = pixelPosX % 8;
    uint16_t pixelPosXBlockM8 = 8 - pixelPosXBlock;

    if ( mask == 128 ) {
        mask = 255;
    }
    else {
        mask = ( mask << 1 ) - 1;
    }

    for ( i = 0; i < 8; i++ ) {

        pixelsScreen = *addr & ( ~ mask );

        pixelsGraphic = *graphic++;

        *addr = pixelsScreen | ( pixelsGraphic >> pixelPosXBlock );

        if ( mask != 128 ) {

            addr2 = addr + 1;

            pixelsScreen = *addr2 & mask;

            *addr2 = pixelsScreen | ( pixelsGraphic << pixelPosXBlockM8 );

        }

        addr = zx_saddrpdown( addr );

    }


}

void paintGraphicPixelPosition( uint16_t pixelPosX, uint16_t pixelPosY, uint16_t numBlocksX, uint16_t numBlocksY, uint8_t *graphic ) {

    uint16_t i, y;
    uint16_t maxi = pixelPosY + numBlocksY;

    while ( numBlocksX > 0 ) {

        y = pixelPosY;

        for ( i = 0; i < numBlocksY; i++ ) {

            paintGraphic8x8PixelPosition( pixelPosX, y, graphic );

            graphic += 8;

            y+= 8;

        }

        numBlocksX--;
        pixelPosX+= 8;
    }

}
