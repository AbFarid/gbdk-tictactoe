#include <gb/gb.h>
#include <gb/console.h>
#include <gb/drawing.h>

#include "stdio.h"
#include "string.h"

// #include "../res/dungeon_map.h"
// #include "../res/dungeon_tiles.h"
#include "../res/board.h"
#include "../res/board_tiles.h"

#include "sprites.h"

void skip_frames(UINT8 n) {
  for (UINT8 i = 0; i < n; i++)
    wait_vbl_done();
}

void init_gfx()
{
  // Load Background tiles and then map
  // set_bkg_data(0, 79u, dungeon_tiles);
  // set_bkg_tiles(0, 0, 32u, 32u, dungeon_mapPLN0);
  set_bkg_data(128, 25, board_tiles);
  // set_sprite_data(128, 25, board_tiles);
  set_bkg_tiles(0, 0, boardWidth, boardHeight, board);
  // set_bkg_tile_xy(7, 2, 128+5);

  // set_sprite_data(128, 25, board_tiles);
  // set_sprite_tile(0, 128+17);

  // mark_draw_pos(&cross);
  // setup_nought();

  // Turn the background map on to make it visible
  SHOW_BKG;
  SHOW_SPRITES;
}

void main(void)
{
  init_gfx();
  
  setup_sprites();
  setup_cross();
  setup_nought();

  while (1)
  {

    // joypad input
    UINT8 J = joypad();

    if (J & (J_LEFT | J_RIGHT | J_UP | J_DOWN)) {
      mark_change_pos(J);
      skip_frames(6);
    }

    if(J & J_A) {
      mark_confirm_pos();
      skip_frames(6);
    }

    if(J & J_B) {
      board_reset();
      set_bkg_tiles(0, 0, boardWidth, boardHeight, board);
      skip_frames(6);
    }

    // if (J) {
    //   printf("J: %d\n", J);
    // }
    // // if(J == (J_B | J_A)) {
    // if((J & (J_B|J_A)) == (J_B|J_A)) {
    //   // mark_change();
      
    //   printf("J & (J_B | J_A): %d\n", J & (J_B | J_A));
    //   printf("J_B | J_A: %d\n", J_B | J_A);
    //   // printf("A and B pressed together.\n");
    // }

    wait_vbl_done();
  }
}
